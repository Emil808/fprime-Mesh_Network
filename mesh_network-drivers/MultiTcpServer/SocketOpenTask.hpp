#ifndef DRV_SOCKETOPENTASK_HPP
#define DRV_SOCKETOPENTASK_HPP

#include <Drv/Ip/IpSocket.hpp>
#include <Os/Task.hpp> 
#include <mesh_network-drivers/MultiTcpServer/TcpClientManager.hpp>
namespace Drv{
    class SocketOpenTask{
        public: 

            SocketOpenTask(); 
            virtual ~SocketOpenTask(); 

            void startSocketOpenTask(const Fw::StringBase &name,
                         const bool reconnect = true,
                         const NATIVE_INT_TYPE priority = -1,
                         const NATIVE_INT_TYPE stack = -1,
                         const NATIVE_INT_TYPE cpuAffinity = -1);

            void stopSocketOpenTask(); 

            Os::Task::TaskStatus joinSocketOpenTask(void** value_ptr);
            virtual U32 getDeviceID() = 0; 
            virtual char* getDeviceIP() = 0; 
        PROTECTED: 
            virtual TcpClientManager& getClientSocketManager() = 0; 
            virtual bool getHostFileEntry(char* entry) = 0; 
            static void openTask(void* pointer); 

            virtual void connected() = 0; 

        private:
            bool m_stop; 
            Os::Task m_task; 
            
    };
}
#endif 