#ifndef DRV_MULTITHREADEDTCPSERVERTASK_HPP
#define DRV_MULTITHREADEDTCPSERVERTASK_HPP

#include <Fw/Buffer/Buffer.hpp>
#include <Os/Task.hpp>
#include <Os/Mutex.hpp>
#include <Drv/Ip/IpSocket.hpp>
#include <mesh_network-drivers/MultiTcpServer/MultiTcpServerManager.hpp>

#define MAX_TASKS 16

namespace Drv{
class AcceptClientTask{

    public: 
        /**
         * @brief Construct a new Accept Client Task object
         * 
         */
        AcceptClientTask(); 

        /**
         * @brief Destroy the Accept Client Task object
         * 
         */
        virtual ~AcceptClientTask(); 

        /**
         * @brief Start the socket Accept Task to start accepting sockets
         * 
         *  Starts up the accept task. This should be called before send calls are expected to work.
         *  priority, stack, and cpuAffinity are provided
         *  to the Os::Task::start call. cpuAffinity defaults to -1.
         * @param name          name of the task
         * @param reconnect     automaticall reconnect the socket when closed. Default: true //Emil's Note: unsure on how it works, not implemented
         * @param protocol      using ID protocol at the MultiTcpServerSocket level. Default: true
         * @param priority      priority of the started task. See: Os::Task::start. Default: -1 not prioritized
         * @param stack         stack size provided to the task. See: Os::Task::start. Default: -1, posix threads default
         * @param cpuAffinity   cpu affinity provided to task. see: Os::Task::start. Default: -1, don't care
         */
        void startAcceptTask(const Fw::StringBase &name,
                             const bool reconnect = true,
                             const bool protocol = true, 
                             const NATIVE_INT_TYPE priority = -1,
                             const NATIVE_INT_TYPE stack = -1,
                             const NATIVE_INT_TYPE cpuAffinity = -1);

        /**
         * @brief stop the Accept task and close associated sockets
         * 
         * Called to stop the Accept Task. it is an error to call this before the thread has benn started using the
         * startAcceptTask call. This will stop the Accept Task and close all associated Sockets
         */
        void stopAcceptTask(); 

        /**
         * @brief Joins the stopping Accept task to wait for it to close
         * 
         * called to join the accept socket task. this will block and return after the task has been stopped with a call
         * to the stopAcceptTask method
         * 
         * @param value_ptr a pointer to fill with data. Passed to the Os::Task::join call. NULL to ignore
         * @return Os::Task::TaskStatus passed back from the Os::Task::join call. 
         */
        Os::Task::TaskStatus joinAcceptTask(void** value_ptr); 

        static void acceptTask(void* pointer); 
        static void readTask(void* pointer); 

        // Helper functions for testing
        /**
         * @brief Get the current Active Task Count
         * 
         * @return U32 current count of Active read Task threads
         */
        U32 getTaskCount(); 

        /**
         * @brief Bool to let tasks know if Id Protocol is active
         * 
         * @return true     Use ID protocol
         * @return false    Do not use ID protocol
         */
        bool isUsingIdProtocol(); 
        
    PROTECTED:


        void startReadTask(I32 task_index);        
        void stopReadTask(I32 index);       //todo: not implemented
        Os::Task::TaskStatus joinReadTask(I32 index, void** value_ptr); 
        
        virtual MultiTcpServerManager& getSocketManager() = 0; 
        virtual Fw::Buffer getBuffer() = 0; 
        virtual void sendBuffer(Fw::Buffer buffer, SocketIpStatus status) = 0; 
        virtual void connected() = 0; 

        virtual U32 getDeviceID() = 0; 
        virtual bool getState() = 0; 
    private:
        Os::Task main_task;
        Os::Task task_pool[MAX_TASKS];  
        I32 taskCount; 
        bool m_stop;
        bool m_reconnect; 
        bool m_id_protocol; 

        NATIVE_INT_TYPE task_priority; 
        NATIVE_INT_TYPE task_stack;
        NATIVE_INT_TYPE task_affinity; 
        Os::Mutex m_lock; 

};

class ClientConnectTask{
    public: 
        ClientConnectTask(); 
        ~ClientConnectTask(); 

    private:
    

};
}
#endif