// ======================================================================
// \title  MultiTcpServerComponentImpl.hpp
// \author Emil808
// \brief  hpp file for TcpServerComponentImpl component implementation class
//         Heavily inspired by Drv/TcpServer
// \copyright 
// ======================================================================

#ifndef MultiTcpServerComponentImpl_HPP
#define MultiTcpServerComponentImpl_HPP

#include <IpCfg.hpp>
#include <Cfg.hpp> 
#include <Drv/Ip/IpSocket.hpp>
#include <mesh_network-drivers/MultiTcpServer/MultiTcpServerSocket.hpp>
#include <mesh_network-drivers/MultiTcpServer/MTSTask.hpp>
#include <mesh_network-drivers/MultiTcpServer/MultiTcpServerManager.hpp>
#include <mesh_network-drivers/MultiTcpServer/TcpClientManager.hpp>
#include <mesh_network-drivers/MultiTcpServer/SocketOpenTask.hpp>

#include "Drv/ByteStreamDriverModel/ByteStreamDriverModelComponentAc.hpp"

#include <Fw/Buffer/Buffer.hpp>
#include <Os/Task.hpp>
#include <stdio.h> 
namespace Drv{

class MultiTcpServerComponentImpl : public ByteStreamDriverModelComponentBase, public AcceptClientTask, public SocketOpenTask {

    public: 
        // ----------------------------------------------------------------------
        // Construction, initialization, and destruction
        // ----------------------------------------------------------------------

        /** 
         * \brief construct the MultiTcpCLient component.
         * \param compName: name of this component
         */ 
        MultiTcpServerComponentImpl(const char* const compName); 

        /**
         * \brief Initialize this componnet
         * \param instance: instance number of this component
         */
        void init(const NATIVE_INT_TYPE instance = 0); 

        /**
         * \brief Deconstruct the component
         */
        ~MultiTcpServerComponentImpl(); 

        // ----------------------------------------------------------------------
        // Helper methods
        // ----------------------------------------------------------------------

        /**
         * \brief Configures the MultiTcpClient settings but does not open the connection
         *
         * The MultiTcpClientComponent needs to connect to a remote TCP server. This call configures the hostname, port and
         * send timeouts for that socket connection. This call should be performed on system startup before recv or send
         * are called. Note: hostname must be a dot-notation IP address of the form "x.x.x.x". DNS translation is left up
         * to the user.
         *
         * \param hostname: ip address of remote tcp server in the form x.x.x.x
         * \param port: port of remote tcp server
         * \param send_timeout_seconds: send timeout seconds component. Defaults to: SOCKET_TIMEOUT_SECONDS
         * \param send_timeout_microseconds: send timeout microseconds component. Must be less than 1000000. Defaults to:
         * SOCKET_TIMEOUT_MICROSECONDS
         * \return status of the configure
         */
        SocketIpStatus configure(const char* hostname,
                                const U16 port,
                                const U32 send_timeout_seconds = SOCKET_SEND_TIMEOUT_SECONDS,
                                const U32 send_timeout_microseconds = SOCKET_SEND_TIMEOUT_MICROSECONDS);


        /**
         * \brief start up the Multi Threaded Tcp Server
         * 
         * This will launch the TcpServer's internal tcp connection, bind to the port and listen. This call will *not*
         * block, nor does it accept incoming connections. It configures the port to listen and then returns. In order to
         * accept incoming connections the `open` call must be made.  Only when the startup method returns SOCK_SUCCESS has
         * the server successfully bound to the port/
         * 
         * \return status of startup. SOCK_SUCCESS on success, something else on error. 
         * @return SocketIpStatus 
         */
        SocketIpStatus startup(); 

        /**
         * \brief shutdown the TcpServer
         * 
         * This will shutdown the TcpServer including closing any active client connectins, and then closing the listening 
         * port of this server
         * 
         */
        void shutdown(); 

        /** 
         * \brief returns a reference to the socket Manager Handler
         * 
         * Gets a reference to the socket Manager handler. The SocketManager handles multiple instances
         * of an IpSocket implementation. For now, the Manager contains the Drv::TcpServer implementation.
         * View the MultiTcpServerManager.hpp for more information. 
         * 
         * Todo: generalize it to be flexible for any multi connection implementation.
         * 
         * \return MultiTcpServerManager reference
         */
        MultiTcpServerManager& getSocketManager(); 

        TcpClientManager& getClientSocketManager(); 
        
        
        U32 getDeviceID();
        char* getDeviceIP(); 
        void setDeviceID(U32 ID); 
        void setHostFile(const char* filename, U32 size); 
    PROTECTED:
        bool getHostFileEntry(char* entry); 

        bool getState(); 
        /**
         * \brief returns a buffer to fill with data
         *
         * Gets a reference to a buffer to fill with data. This allows the component to determine how to provide a
         * buffer and the socket read task just fills said buffer.
         *
         * \return Fw::Buffer to fill with data
         */
        Fw::Buffer getBuffer(); 

        /**
         * \brief sends a buffer to be filled with data
         *
         * Sends the buffer gotten by getBuffer that has now been filled with data. This is used to delegate to the
         * component how to send back the buffer. Ignores buffers with error status error.
         *
         * \return Fw::Buffer filled with data to send out
         */
        void sendBuffer(Fw::Buffer buffer, SocketIpStatus status); 

        /**
         * \brief called when the IPv4 system has been connected
         */
        void connected(); 

        
    
    PRIVATE:

         /**
         * \brief Send data out of the TcpClient
         *
         * Passing data to this port will send data from the TcpClient to whatever TCP server this component has connected
         * to. Should the socket not be opened or was disconnected, then this port call will return SEND_RETRY and critical
         * transmissions should be retried. SEND_ERROR indicates an unresolvable error. SEND_OK is returned when the data
         * has been sent.
         *
         * Note: this component delegates the reopening of the socket to the accept thread and thus the caller should retry
         * after the accept thread has attempted to reopen the port but does not need to reopen the port manually.
         *
         * TODO:
         * Changes based on usage, the fwBuffer expects the format to be in a work-in-progress protocol 
         * Internally, with the Socket Manager, each socket has a DeviceId to identify what device the socket is conencted to.
         * 
         * Im still working on the specifics to it. 
         * 
         * \param portNum: fprime port number of the incoming port call
         * \param fwBuffer: buffer containing data to be sent
         * \return SEND_OK on success, SEND_RETRY when critical data should be retried and SEND_ERROR upon error
         */
        Drv::SendStatus send_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer); 

         /**
         * \brief **not supported**
         *
         * IP based ByteStreamDrivers don't support polling.
         */
        Drv::PollStatus poll_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer); 

        /**
         * @brief Private function to update the adjacency matrix
         * 
         */
        void update_adjacency_matrix(); 

        /**
         * @brief Initializes the adjacency matrix
         * 
         */
        void init_adjacency_matrix();

        /**
         * @brief Checks connection mask in adjacency matrix
         * 
         */
        bool check_adjacency_matrix(U32 receiverID); 

        Drv::MultiTcpServerManager m_manager; //!< Manager that to handle multiple sockets. 
        Drv::TcpClientManager m_CManager; 
        U32 DeviceID; 
        Os::Mutex state_lock; 
        I32 state; 

        char hostfile_name[MAX_HOSTFILE_NAME_SIZE]; 
        FILE *fp; 

        FILE *adjFP;
        U8 adj_M[MAX_CLIENTS]; //adjacency matrix 
    

}; //class end
}; //namespace end
#endif //MultiTcpServerComponentImpl_HPP
