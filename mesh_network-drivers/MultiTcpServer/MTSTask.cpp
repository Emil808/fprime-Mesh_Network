#include <mesh_network-drivers/MultiTcpServer/MTSTask.hpp>
#include <Fw/Logger/Logger.hpp>
#include <Fw/Types/Assert.hpp>
#include <cerrno> 

#include <pthread.h>
#include <stdlib.h>
#define MAXIMUM_SIZE 0x7FFFFFFF
struct readTaskParams{
        NATIVE_INT_TYPE client_index;
        Drv::AcceptClientTask* thisPtr;  
};

namespace Drv{

    
    AcceptClientTask::AcceptClientTask(): taskCount(0), m_stop(false), m_id_protocol(true){}
    AcceptClientTask::~AcceptClientTask(){}

  
    void AcceptClientTask::startAcceptTask(const Fw::StringBase &name,
                             const bool reconnect,
                             const bool protocol, 
                             const NATIVE_INT_TYPE priority ,
                             const NATIVE_INT_TYPE stack ,
                             const NATIVE_INT_TYPE cpuAffinity ){
    this->m_id_protocol = protocol; 
    this->task_affinity = cpuAffinity; 
    this->task_priority = priority; 
    this->task_stack = stack; 

    FW_ASSERT(not main_task.isStarted()); 
    FW_ASSERT(not this->m_stop); 

    this->m_reconnect = reconnect; 
    
    Os::Task::TaskStatus stat = main_task.start(name, AcceptClientTask::acceptTask, this, priority, stack, cpuAffinity); 
    
    FW_ASSERT(Os::Task::TASK_OK == stat, static_cast<NATIVE_INT_TYPE>(stat));
}

    void AcceptClientTask::stopAcceptTask(){
        this->m_stop = true; 
        this->getSocketManager().closeAll(); 
    }

    Os::Task::TaskStatus AcceptClientTask::joinAcceptTask(void** value_ptr){
        return main_task.join(value_ptr); 
    } 

    U32 AcceptClientTask::getTaskCount(){
        return this->taskCount; 
    }


    //NOTE: how to handle integrity of index? 

    void AcceptClientTask::startReadTask(I32 task_index){
        Os::TaskString name = "ReadTask_";
        char index_string[3]; 
        snprintf(index_string, 3, "%d", task_index); 
        name += index_string; 

        struct readTaskParams params; 
        params.client_index = task_index; 
        params.thisPtr = this; 

        FW_ASSERT(not this->task_pool[task_index].isStarted());  // It is a coding error to start this task multiple times
        //FW_ASSERT(not this->m_stop);        // It is a coding error to stop the thread before it is started

        Os::Task::TaskStatus stat = this->task_pool[task_index].start(name, AcceptClientTask::readTask, &params, this->task_priority, this->task_stack, this->task_affinity);
        FW_ASSERT(Os::Task::TASK_OK == stat, static_cast<NATIVE_INT_TYPE>(stat));
    };

    Os::Task::TaskStatus AcceptClientTask::joinReadTask(I32 index, void** value_ptr){
        return task_pool[index].join(value_ptr); 
    }

    void AcceptClientTask::acceptTask(void* pointer){
        FW_ASSERT(pointer); 
        Fw::Logger::logMsg("[INFO] Entered acceptTask\n");
        SocketIpStatus status = SOCK_SUCCESS; 

        AcceptClientTask* self = reinterpret_cast<AcceptClientTask*>(pointer); 

        NATIVE_INT_TYPE clientFd = -1; //todo name change, this is actually the index within manager, not the Fd

        Os::TaskString name; 
        char new_task_name[FW_TASK_NAME_MAX_SIZE]; 

        //NATIVE_INT_TYPE priority, stack, cpuAffinity; 
        NATIVE_INT_TYPE join_status = 0; 
        
        struct readTaskParams params;
        do{
            //Fw::Logger::logMsg("[INFO] Entered accept task LOOP\n");
            //Fw::Logger::logMsg("[INFO] task count = %d\nMax Task = %d\n", self->taskCount, MAX_TASKS);
            //Fw::Logger::logMsg("[INFO] not self->taskCount == MAX_TASKS result = %d\n", not (self->taskCount == MAX_TASKS)); 
            if(not (self->taskCount == MAX_TASKS) && self->getSocketManager().getNConnected() < MAX_CLIENTS){ //not yet reached maximum clients
                //Fw::Logger::logMsg("[INFO] Attempting to accept\n");
                 
                status = self->getSocketManager().open(clientFd); //accept a client
                //fprintf(stderr, "server accept statues =  %d\n", status); 
                //changing of clientFd so soon is causing memory leak when passing params to read task/thread
                if(status != SOCK_SUCCESS){
                    
                    clientFd = -1; 
                     
                    //Fw::Logger::logMsg("Failed to accept client with status %d and errno d\n", status, errno); 
                    Os::Task::delay(10); //delay present in original socket read task 
                }
                
                if(status == SOCK_SUCCESS && self->m_id_protocol){ //run initial connection protocols
                    //fprintf(stderr, "Server accepted connection, receiving Client's ID\n"); 
                    status = self->getSocketManager().getSocketHandler(clientFd).recvClientID(); //receive client ID
                    if(status != SOCK_SUCCESS){
                        //Fw::Logger::logMsg("Client Failed to give Identification. Recv Status = %d\n", status, errno); 
                        self->getSocketManager().close(clientFd); 
                    }
                    else{
                        status = self->getSocketManager().getSocketHandler(clientFd).sendSrvrID(self->getDeviceID()); 
                    }
                }
                

                if(status == SOCK_SUCCESS){ // Successfully accepted a client
                    for(int i = 0; i < MAX_TASKS; i++){
                        //might need a lock where im accessing variables from self
                        if(not self->task_pool[i].isStarted()){ //find available task handler
                            self->taskCount += 1; 
                            self->task_pool[i].setStarted(true); 
                            snprintf(new_task_name, FW_TASK_NAME_MAX_SIZE, "MTSReadTask_%d", i);
                            name = new_task_name; 
                            params = {clientFd, self}; 
                            Os::Task::TaskStatus stat = self->task_pool[i].start(name, AcceptClientTask::readTask, &params); //using task default priority, stack, affinity
                            FW_ASSERT(Os::Task::TASK_OK == stat, static_cast<NATIVE_INT_TYPE>(stat));
                            break; 

                        }
                    }
                }
            
                status = SOCK_SUCCESS; //reset status to prevent do while from stopping
            }
            else{ // servicing maximum clients
                
                for(int i = 0; i < MAX_TASKS; i++){
                    if(self->task_pool[i].isStarted()){
                        join_status = pthread_tryjoin_np(*reinterpret_cast<pthread_t*>(self->task_pool[i].getRawHandle()), nullptr); 
                    }
                    if(join_status == 0){
                        if(self->taskCount > 0) {self->taskCount -= 1; }
                        self->task_pool[i].setStarted(false); 
                    }
                }

            }
        }while(not self->m_stop && (status == SOCK_SUCCESS || status == SOCK_INTERRUPTED_TRY_AGAIN || self->m_reconnect)); 
        
        //shutdown sequence

        for(int i = 0; i < MAX_TASKS; i++){ //join all tasks
                    if(self->task_pool[i].isStarted()){
                        join_status = pthread_tryjoin_np(*reinterpret_cast<pthread_t*>(self->task_pool[i].getRawHandle()), nullptr); 
                    }
                    if(join_status == 0){
                        if(self->taskCount > 0) {self->taskCount -= 1; }
                        self->task_pool[i].setStarted(false); 
                    }
                }

        self->getSocketManager().closeAll(); 
    }

    void AcceptClientTask::readTask(void* pointer){
        FW_ASSERT(pointer); 
        SocketIpStatus status = SOCK_SUCCESS; 
        struct readTaskParams* params = reinterpret_cast<struct readTaskParams*>(pointer); 
        AcceptClientTask* self = params->thisPtr; 

        
        I32 index = params->client_index;
        
        //receive and verify the device id.
        //could be a parameter in the accept task that can be set if user wants to use device IDs
        do{
            //Socket should be open when starting the readTask
            if(not self->getSocketManager().getSocketHandler(index).isOpened()){
                break; 
            }

            //Read from network connection
            if(self->getSocketManager().getSocketHandler(index).isOpened() and (not self->m_stop)){
                Fw::Buffer buffer = self->getBuffer(); //todo: handle failed getBuffer from bufferManager
                if(buffer.getSize() == 0){ // getBuffer returned with size of 0, skip loop and try again. 
                    self->sendBuffer(buffer, SOCK_READ_ERROR); //empty buffer causes assert when sending an empty buffer up, what to do??
                    //deallocate it? dummy data, then send it up? deframer would deallocate
                    continue; 
                }
                U8* data = buffer.getData(); 
                FW_ASSERT(data); 

                I32 size = static_cast<I32>(buffer.getSize()); 
                size = (size >= 0) ? size : MAXIMUM_SIZE; 

                // if only receiving, how to detect loss of connection? 
                // closing client will send an empty packet
                
                status = self->getSocketManager().getSocketHandler(index).recv(data, size); 

                if ((status != SOCK_SUCCESS) && (status != SOCK_INTERRUPTED_TRY_AGAIN)) {
                    Fw::Logger::logMsg("[WARNING] Failed to recv from port with status %d and errno %d\n", status, errno);
                    self->getSocketManager().getSocketHandler(index).close();
                    //buffer.setSize(0);
                } else {
                    // Send out received data
                    buffer.setSize(size);
                }
                self->sendBuffer(buffer, status);
            }
        }while(not self->m_stop && (status ==SOCK_SUCCESS || status == SOCK_INTERRUPTED_TRY_AGAIN)); 

        self->getSocketManager().close(index); 
    }

    bool AcceptClientTask::isUsingIdProtocol(){
        return this->m_id_protocol; 
    }
}