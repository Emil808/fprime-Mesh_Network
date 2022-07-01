#include <mesh_network-drivers/MultiTcpServer/MultiTcpClientSocket.hpp>
#include <Fw/Logger/Logger.hpp>
#include <Fw/Types/BasicTypes.hpp>
#include <Fw/Types/Assert.hpp>
#include <Fw/Types/StringUtils.hpp>

#include <stdio.h>
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

namespace Drv{
    MultiTcpClientSocket::MultiTcpClientSocket(): TcpClientSocket(), serverDeviceID(0){}

    MultiTcpClientSocket::~MultiTcpClientSocket(){} 

    void MultiTcpClientSocket::setTimeout(const U32 timeout_seconds, const U32 timeout_microseconds){
        FW_ASSERT(timeout_microseconds < 1000000, timeout_microseconds);
        this->m_timeoutSeconds = timeout_seconds; 
        this->m_timeoutMicroseconds = timeout_microseconds; 

    }
    SocketIpStatus MultiTcpClientSocket::setHost(const char* hostname, const U16 port){
        return TcpClientSocket::configure(hostname, port, this->m_timeoutSeconds, this->m_timeoutMicroseconds); 
        
    }

    SocketIpStatus MultiTcpClientSocket::sendClientID(U32 ID){

        SocketIpStatus status = SOCK_SUCCESS; 
        U8 data[6]; 

        //format data into 'I', 'D', 0x, 0x, 0x, 0x
        data[0] = 'I'; data[1] = 'D'; 
        data[2] = (ID >> 24) & 0xFF; 
        data[3] = (ID >> 16) & 0xFF; 
        data[4] = (ID >> 8) & 0xFF; 
        data[5] = (ID >> 0) & 0xFF; 

        I32 size = sizeof(data);
        status = TcpClientSocket::send(data, size); 
        if(status != SOCK_SUCCESS){
            TcpClientSocket::close();  
        }

        return status; 
    }

    SocketIpStatus MultiTcpClientSocket::recvSrvrID(){
        SocketIpStatus status = SOCK_SUCCESS; 
        U8 data[24]; 
        I32 size = sizeof(data); 
        U32 ID; 
        
        //on successful open, initial data to receive is the client Device ID
        //receive from client
        //format: // 'I', 'D', 0x, 0x, '0x, 0x
        //fprintf(stderr, "starting recv\n"); 
        status = TcpClientSocket::recv(data, size); 
        //fprintf(stderr, "recv status = %d\n", status); 
        if(status != SOCK_SUCCESS){
            TcpClientSocket::close(); 
            return status; 
        }
        
        //check that ID is correct format
        if(data[0] != 'I' && data[1] != 'D'){
            //first packet after open should be 
            // 'I', 'D', 0x, 0x, 0x, 0x
            TcpClientSocket::close();
            return SOCK_FAILED_TO_GET_SOCKET;
        }    

        //save ID
        //convert the U8 data: hex -> int
        ID = (data[2] << 24) + (data[3] << 16) + (data[4] << 8) + (data[5] << 0); 
        this->serverDeviceID = ID; 
        //fprintf(stderr, "received ID\n"); 
        return SOCK_SUCCESS; 
    }

    U32 MultiTcpClientSocket::getServerDeviceID(){
        return this->serverDeviceID; 
    }

    void MultiTcpClientSocket::close(){
        TcpClientSocket::close(); 
        this->serverDeviceID = 0; 
    } 

    bool MultiTcpClientSocket::compareHost(const char* hostname, const U16 port){
        bool state = true; 

        for(long unsigned int i = 0; i < sizeof(hostname); i++){
            if(hostname[i] != this->m_hostname[i]){     // Compare IP address
                state = false;   
                break;                                  // Found difference, addresses are not the same
            }
        }

        if((port != this-> m_port) && state){       //addresses are same, but ports are different
            state = false; 
        }

        return state; 
    }
}