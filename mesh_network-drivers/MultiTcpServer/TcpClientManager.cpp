#include <cstring>
#include <Drv/Ip/IpSocket.hpp>
#include <Fw/Types/Assert.hpp>
#include <Fw/Types/BasicTypes.hpp>
#include <Fw/Types/StringUtils.hpp>
#include <Fw/Logger/Logger.hpp>
#include <mesh_network-drivers/MultiTcpServer/TcpClientManager.hpp>

#ifdef TGT_OS_TYPE_VXWORKS
    #include <socket.h>
    #include <inetLib.h>
    #include <fioLib.h>
    #include <hostLib.h>
    #include <ioLib.h>
    #include <vxWorks.h>
    #include <sockLib.h>
    #include <taskLib.h>
    #include <sysLib.h>
    #include <errnoLib.h>
    #include <cstring>
#elif defined TGT_OS_TYPE_LINUX || TGT_OS_TYPE_DARWIN
    #include <sys/socket.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#else
    #error OS not supported for IP Socket Communications
#endif

namespace Drv{
    TcpClientManager::TcpClientManager(): m_timeoutSeconds(0), m_timeoutMicroseconds(0), current_opened_connections(0){}

    TcpClientManager::~TcpClientManager(){}

    SocketIpStatus TcpClientManager::configure(const U32 send_timeout_seconds, const U32 send_timeout_microseconds){
        FW_ASSERT(send_timeout_microseconds < 1000000, send_timeout_microseconds);
        this->m_timeoutSeconds = send_timeout_seconds;
        this->m_timeoutMicroseconds = send_timeout_microseconds; 
        for(int i = 0; i < MAX_CLIENTS; i++){
            sockets[i].setTimeout(send_timeout_seconds, send_timeout_microseconds); 
        }
        return SOCK_SUCCESS; 
    }

    void TcpClientManager::closeAll(){
        for(int i = 0; i < MAX_CLIENTS; i++){
            this->sockets[i].close(); 
        }
    } 
    void TcpClientManager::close(NATIVE_INT_TYPE index){
        this->sockets[index].close(); 
        
    }

    I32 TcpClientManager::getFreeSocketIndex(){
        I32 index = -1; 
        for(I32 i = 0; i < MAX_CLIENTS; i++){
            if(!sockets[i].isOpened()){
                index = i; 
                break; 
            }
        }
        return index; 
    }
    I32 TcpClientManager::getNConnected(){
        return this->current_opened_connections;
    }
    Drv::MultiTcpClientSocket& TcpClientManager::getSocketHandler(U32 socket_index){
        FW_ASSERT(socket_index < MAX_CLIENTS); 
        return this->sockets[socket_index]; 
    }

    
    Drv::MultiTcpClientSocket& TcpClientManager::getSocketHandlerByID(U32 device_id){
        FW_ASSERT(device_id);
        for(int i = 0; i < MAX_CLIENTS; i++){
            if(this->sockets[i].getServerDeviceID() == device_id){
                return this->sockets[i]; 
            }

        }
        return this->sockets[0]; //todo, better handle on failure to find correct handler
    }

    bool TcpClientManager::checkIfAlreadyConnected(U32 device_id){
        
        for(int i = 0; i < MAX_CLIENTS; i++){
            if(this->sockets[i].isOpened() && this->sockets[i].getServerDeviceID() == device_id){
                return true; 
            }
        }
        
        return false; 
    }

    bool TcpClientManager::checkIfAlreadyConnected(const char* hostname, const U16 port){
        bool state = false; 

        for(int i = 0; i < MAX_CLIENTS; i++){
            if(this->sockets[i].isOpened() && this->sockets[i].compareHost(hostname, port)){ 
                //a port is opened, and is connected to the host in question
                state = true; 
                break; 
            }
            
        }
        return state; 
    }
}