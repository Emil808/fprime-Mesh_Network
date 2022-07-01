#ifndef DRV_MULTITCPCLIENT_TCPHELPER_HPP_
#define DRV_MULTITCPCLIENT_TCPHELPER_HPP_

#include <Fw/Types/BasicTypes.hpp>
#include <Drv/Ip/TcpClientSocket.hpp> // for SocketIpStatus Enumeration
#include <IpCfg.hpp> 
#include <Os/Mutex.hpp>

namespace Drv{
    class MultiTcpClientSocket : public TcpClientSocket{
        public:
            MultiTcpClientSocket(); 
            ~MultiTcpClientSocket(); 

            SocketIpStatus sendClientID(U32 ID); 
            SocketIpStatus recvSrvrID(); 

            void setTimeout(const U32 timeout_seconds, const U32 timeout_microseconds); 

            SocketIpStatus setHost(const char* hostname, const U16 port); 

            U32 getServerDeviceID(); 
            void close(); 

            bool compareHost(const char* hostname, const U16 port);  
        private: 
            U32 m_timeoutSeconds;               // Ip Socket timeout, seconds
            U32 m_timeoutMicroseconds;          // Ip Socket timeout, microseconds
 
            U32 serverDeviceID;
    };
}
#endif 