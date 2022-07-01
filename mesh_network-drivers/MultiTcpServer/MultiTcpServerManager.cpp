#include <cstring>
#include <Drv/Ip/IpSocket.hpp>
#include <Fw/Types/Assert.hpp>
#include <Fw/Types/BasicTypes.hpp>
#include <Fw/Types/StringUtils.hpp>
#include <Fw/Logger/Logger.hpp>
#include <mesh_network-drivers/MultiTcpServer/MultiTcpServerManager.hpp>

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
    MultiTcpServerManager::MultiTcpServerManager(): m_base_fd(-1), m_timeoutSeconds(0), m_timeoutMicroseconds(0), m_port(0), m_open(false), current_client_connections(0) {
        ::memset(m_hostname, 0, sizeof(m_hostname));
        FD_ZERO(&this->listener_socket); 
    }

    MultiTcpServerManager::~MultiTcpServerManager(){} 

    SocketIpStatus MultiTcpServerManager::configure(const char* hostname, const U16 port, const U32 send_timeout_seconds,
                             const U32 send_timeout_microseconds){
    FW_ASSERT(send_timeout_microseconds < 1000000, send_timeout_microseconds);
    FW_ASSERT(port != 0, port);
    this->m_timeoutSeconds = send_timeout_seconds;
    this->m_timeoutMicroseconds = send_timeout_microseconds;
    this->m_port = port;
    (void) Fw::StringUtils::string_copy(this->m_hostname, hostname, SOCKET_MAX_HOSTNAME_SIZE);

    this->set_timeout.tv_sec = send_timeout_seconds; 
    this->set_timeout.tv_usec = send_timeout_microseconds; 
    return SOCK_SUCCESS;
    }

    SocketIpStatus MultiTcpServerManager::startup(){
        NATIVE_INT_TYPE serverFd = -1;
        struct sockaddr_in address;
        this->shutdown();

        // Acquire a socket, or return error
        if ((serverFd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            return SOCK_FAILED_TO_GET_SOCKET;
        }
        // Set up the address port and name
        address.sin_family = AF_INET;
        address.sin_port = htons(this->m_port); 
        
        // OS specific settings
        #if defined TGT_OS_TYPE_VXWORKS || TGT_OS_TYPE_DARWIN
            address.sin_len = static_cast<U8>(sizeof(struct sockaddr_in));
        #endif

        // First IP address to socket sin_addr
        if(this->sockets[0].addressToIp4(m_hostname, &(address.sin_addr)) != SOCK_SUCCESS){
            ::close(serverFd); 
            return SOCK_INVALID_IP_ADDRESS; 
        }

        // TCP requires bind to an address to the socket
        if (::bind(serverFd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0) {
            ::close(serverFd);
            return SOCK_FAILED_TO_BIND;
        }
        this->m_base_fd = serverFd; 

        for(int i = 0; i < MAX_CLIENTS; i++){
            this->sockets[i].setServerFd(this->m_base_fd); 
        }
        Fw::Logger::logMsg("Listening for single client at %s:%hu\n", reinterpret_cast<POINTER_CAST>(m_hostname), m_port);
        // TCP requires listening on a the socket. Second argument prevents queueing of anything more than a single client.
        if (::listen(serverFd, 1) < 0) {
            ::close(serverFd);
            return SOCK_FAILED_TO_LISTEN; // What we have here is a failure to communicate
        }

        FD_SET(this->m_base_fd, &this->listener_socket); 
        return SOCK_SUCCESS; 

    }

    
    void MultiTcpServerManager::closeServer(){
        ::close(this->m_base_fd); 
    }

    SocketIpStatus MultiTcpServerManager::open(NATIVE_INT_TYPE& index){
        SocketIpStatus status = SOCK_FAILED_TO_ACCEPT; 
        int activity = select(this->m_base_fd+1, &this->listener_socket, NULL, NULL, &this->set_timeout);
        if(activity){ //activity on the listener socket
            for(int i = 0; i < MAX_CLIENTS; i++){
                if(sockets[i].isOpened() == false){
                    index = i; 
                    status = sockets[i].open(); 
                    break; 
                }
            }
            if(status == SOCK_SUCCESS){
                this->current_client_connections += 1; 
            }
             
        }
        else{
            FD_SET(this->m_base_fd, &this->listener_socket); //this would casue a run time error, m_base_fd would change to -1 on shutdown; 
        }
        //will get to here if no open socket, or accepting timed out
        return status;
    }
    void MultiTcpServerManager::shutdown(){
        (void)::shutdown(this->m_base_fd, SHUT_RDWR);
        (void)::close(this->m_base_fd); 
        this->m_base_fd = -1;  
        this->closeAll();
    }

    void MultiTcpServerManager::closeAll(){
        //mutex lock
        for(int i = 0; i < MAX_CLIENTS; i++){
            this->sockets[i].close(); 
        }
        this->current_client_connections = 0; 

        //mutex unlock
    }

    void MultiTcpServerManager::close(NATIVE_INT_TYPE index){
        //mutex lock
        this->sockets[index].close(); 
        if(this->current_client_connections > 0){
            this->current_client_connections -= 1; 
        }
        //mutex unlock
    }

    NATIVE_INT_TYPE MultiTcpServerManager::getServerFd(){
        return this->m_base_fd; 
    }
    I32 MultiTcpServerManager::getNConnected(){
        return this->current_client_connections; 
    }

    Drv::MultiTcpServerSocket& MultiTcpServerManager::getSocketHandler(U32 socket_index){
        FW_ASSERT(socket_index < MAX_CLIENTS); 
        return this->sockets[socket_index];
    }

    Drv::MultiTcpServerSocket& MultiTcpServerManager::getSocketHandlerByID(U32 deviceId){
        FW_ASSERT(deviceId); 

        for(int i = 0; i < MAX_CLIENTS; i++){
            if(this->sockets[i].getClientDeviceID() == deviceId){
                return this->sockets[i]; 
            }
        }
        return this->sockets[0]; //todo, better handle on failure to find correct handler


    }

    char* MultiTcpServerManager::getDeviceIP(){
        return this->m_hostname; 
    }
}