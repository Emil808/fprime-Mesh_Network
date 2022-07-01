
#include <mesh_network-drivers/MultiTcpServer/MultiTcpServerSocket.hpp>
#include <Fw/Logger/Logger.hpp>
#include <Fw/Types/BasicTypes.hpp>
#include <Fw/Types/Assert.hpp>
#include <Fw/Types/StringUtils.hpp>

#ifdef TGT_OS_TYPE_VXWORKS
#include <socket.h>
    #include <inetLib.h>
    #include <fioLib.h>
    #include <hostLib.h>
    #include <ioLib.h>
    #include <vxWorks.h>
    #include <sockLib.h>
    #include <fioLib.h>
    #include <taskLib.h>
    #include <sysLib.h>
    #include <errnoLib.h>
    #include <cstring>
#elif defined TGT_OS_TYPE_LINUX || TGT_OS_TYPE_DARWIN
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <arpa/inet.h>
#else
#error OS not supported for IP Socket Communications
#endif

#include <cstring> 

namespace Drv{

    MultiTcpServerSocket::MultiTcpServerSocket(): TcpServerSocket(), clientDeviceID(0){}
    
    MultiTcpServerSocket::~MultiTcpServerSocket(){}
    
    SocketIpStatus MultiTcpServerSocket::configure(const U32 send_timeout_seconds, const U32 send_timeout_microseconds){
        FW_ASSERT(send_timeout_microseconds < 1000000, send_timeout_microseconds);
        this->m_timeoutSeconds = send_timeout_seconds; 
        this->m_timeoutMicroseconds = send_timeout_microseconds; 
        return SOCK_SUCCESS;
    }

    // SocketIpStatus MultiTcpServerSocket::startup(){
    //     //Multi Tcp Server Manager will be the one to run start up
    //     //this overrides the TcpServerSocket::startup() implementation
    //     //return SOCK_DISCONNECTED; 

    //     //for current testing
    //     return this->TcpServerSocket::startup(); 
    // }

    // void MultiTcpServerSocket::shutdown(){

    //     //Multi TcpServer Manager will be the one actually shutdown the server port
    //     //this overrides the TcpServerSocket::shutdown() implementation
        
    //     //for current testing
    //     //this->TcpSocketServer::shutdown(); 

    //     //here we'll just set the serverFd to -1
    //     //this->serverFd = -1; 
        
    // }

    U32 MultiTcpServerSocket::getClientDeviceID(){
        return this->clientDeviceID; 
    }

    void MultiTcpServerSocket::setServerFd(NATIVE_INT_TYPE fd){
        this->serverFd = fd; 
    }

    SocketIpStatus MultiTcpServerSocket::addressToIp4(const char* address, void* ip4){
        return this->TcpServerSocket::addressToIp4(address, ip4); 
    }

    SocketIpStatus MultiTcpServerSocket::open(){
        SocketIpStatus status = SOCK_SUCCESS; 
        status = TcpServerSocket::open(); 

        
        if(status != SOCK_SUCCESS){
            TcpServerSocket::close(); 
            return status; 
        }
        return SOCK_SUCCESS; 
    }

    SocketIpStatus MultiTcpServerSocket::openProtocol(NATIVE_INT_TYPE &fd){
        NATIVE_INT_TYPE clientFd = -1;
        // TCP requires accepting on a the socket to get the client socket file descriptor.
        if ((clientFd = ::accept(serverFd, nullptr, nullptr)) < 0) {
            return SOCK_FAILED_TO_ACCEPT; // What we have here is a failure to communicate
        }
        // Setup client send timeouts
        if (IpSocket::setupTimeouts(clientFd) != SOCK_SUCCESS) {
            ::close(clientFd);
            return SOCK_FAILED_TO_SET_SOCKET_OPTIONS;
        }
        Fw::Logger::logMsg("Accepted client at %s:%hu\n", reinterpret_cast<POINTER_CAST>(m_hostname), m_port);
        fd = clientFd;
        return SOCK_SUCCESS;
    }

    SocketIpStatus MultiTcpServerSocket::recvClientID(){

        SocketIpStatus status = SOCK_SUCCESS; 
        U8 data[6]; 
        I32 size = sizeof(data); 
        U32 ID; 

        //on successful open, initial data to receive is the client Device ID
        //receive from client
        //format: // 'I', 'D', 0x, 0x, '0x, 0x
        status = TcpServerSocket::recv(data, size); 
        if(status != SOCK_SUCCESS){
            TcpServerSocket::close(); 
            return status; 
        }

        //check that ID is correct format
        if(data[0] != 'I' && data[1] != 'D'){
            //first packet after open should be 
            // 'I', 'D', 0x, 0x, 0x, 0x
            TcpServerSocket::close();
            return SOCK_FAILED_TO_GET_SOCKET;
        }    

        //save ID
        //convert the U8 data: hex -> int
        ID = (data[2] << 24) + (data[3] << 16) + (data[4] << 8) + (data[5] << 0); 
        this->clientDeviceID = ID; 
        
        return SOCK_SUCCESS; 

    }

    SocketIpStatus MultiTcpServerSocket::sendSrvrID(U32 ID){
        SocketIpStatus status = SOCK_SUCCESS; 
        U8 data[6]; 
        
        //format data into 'I', 'D', 0x, 0x, 0x, 0x
        data[0] = 'I'; data[1] = 'D'; 
        data[2] = (ID >> 24) & 0xFF; 
        data[3] = (ID >> 16) & 0xFF; 
        data[4] = (ID >> 8) & 0xFF; 
        data[5] = (ID >> 0) & 0xFF; 

        status = TcpServerSocket::send(data, sizeof(data)); 
        if(status != SOCK_SUCCESS){
            TcpServerSocket::close();  
        }

        return status; 
        
    }

    void MultiTcpServerSocket::close(){
        TcpServerSocket::close(); 
        this->clientDeviceID = 0; 
    }

}