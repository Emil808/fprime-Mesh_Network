// ======================================================================
// \title  MultiTcpServerSocket.hpp
// \author Emil808
// \brief  hpp file for MultiTcpServerSocket core implementation classes
// Drv/Ip/IpSocket only handles ONE client fd. This will handle multiple
// client file descriptors with a max according to IpCfg
// Many Functions and methods are from IpSocket and TCPServerSocket
//
// IpSocket was originally meant to be inherited and open, receive, and send protocols implemented
// specific types of connection protocols (TCP server, client, UDP).
// BUT the TCP Server was designed to only work with one client. 
// 
// This Multi-TCP Server Socket implementation's goal is to implement a multi threaded TCP Server that can 
// service multiple clients at a time. 
//
// ======================================================================
#ifndef DRV_MULTITCPSERVER_TCPHELPER_HPP_
#define DRV_MULTITCPSERVER_TCPHELPER_HPP_

#include <Fw/Types/BasicTypes.hpp>
#include <Drv/Ip/TcpServerSocket.hpp> // for SocketIpStatus Enumeration
#include <IpCfg.hpp> 
#include <Os/Mutex.hpp>

// Wrapper for IpSocket implementations, to provide features to handle multiple of them. 

namespace Drv{
    class MultiTcpServerSocket : public TcpServerSocket{
        public:

            MultiTcpServerSocket(); 
            ~MultiTcpServerSocket(); 
            
            SocketIpStatus configure(const U32 send_timeout_seconds, const U32 send_timeout_microseconds);
             void setServerFd(NATIVE_INT_TYPE fd);
            // SocketIpStatus startup();   
            // void shutdown();  
            
            SocketIpStatus addressToIp4(const char* address, void* ip4); 

            SocketIpStatus open();
            SocketIpStatus openProtocol(NATIVE_INT_TYPE& fd); 
            SocketIpStatus recvClientID(); 
            SocketIpStatus sendSrvrID(U32 ID); 
            U32 getClientDeviceID();

            void close(); 
        PROTECTED:
            
        private:
            U32 clientDeviceID;
            NATIVE_INT_TYPE serverFd; 
    };
}
#endif