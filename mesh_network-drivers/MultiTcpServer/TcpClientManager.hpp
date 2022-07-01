#ifndef DRV_CLIENTMANAGER_HPP
#define DRV_CLIENTMANAGER_HPP

#include <Fw/Types/BasicTypes.hpp>
#include <mesh_network-drivers/MultiTcpServer/MultiTcpClientSocket.hpp>
#include <IpCfg.hpp>
#include <Cfg.hpp> 
#include <Fw/Types/BasicTypes.hpp>
#include <Os/Mutex.hpp>

//todo: Do multiple client sockets on a node

namespace Drv{
    class TcpClientManager{
        public: 
            TcpClientManager(); 
            ~TcpClientManager(); 
            
            SocketIpStatus configure(const U32 send_timeout_seconds,
                             const U32 send_timeout_microseconds);

            void closeAll(); 
            void close(NATIVE_INT_TYPE index); 

            I32 getFreeSocketIndex(); 
            I32 getNConnected(); 
            Drv::MultiTcpClientSocket& getSocketHandler(U32 socket_index); 

            Drv::MultiTcpClientSocket& getSocketHandlerByID(U32 device_id); 

            bool checkIfAlreadyConnected(U32 device_id); 
            bool checkIfAlreadyConnected(const char* hostname, const U16 port); 

        private:
            U32 m_timeoutSeconds;               // Ip Socket timeout, seconds
            U32 m_timeoutMicroseconds;          // Ip Socket timeout, microseconds

            I32 current_opened_connections; 
            Drv::MultiTcpClientSocket sockets[MAX_CLIENTS]; 

    };
}
#endif 