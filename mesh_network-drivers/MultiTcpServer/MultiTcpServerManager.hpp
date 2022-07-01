#ifndef DRV_MTSMANAGER_HPP
#define DRV_MTSMANAGER_HPP

#include <Fw/Types/BasicTypes.hpp>
#include <mesh_network-drivers/MultiTcpServer/MultiTcpServerSocket.hpp> // for SocketIpStatus Enumeration
#include <IpCfg.hpp> 
#include <Cfg.hpp> 
#include <Os/Mutex.hpp>

#include <sys/select.h>

namespace Drv{
    class MultiTcpServerManager{
        public:
            // ----------------------------------------------------------------------
            // Construction, initialization, and destruction
            // ----------------------------------------------------------------------

            /**
             * @brief Construct a new Multi Tcp Server Manager object
             * 
             */
            MultiTcpServerManager();

            /**
             * @brief Destroy the Multi Tcp Server Manager object
             * 
             */
            ~MultiTcpServerManager(); 
            
            /**
             * @brief Configures the TcpServer settings for the listening socket.  
             * 
             * The TcpClient Component needs to connect to a remote TCP server. This call configures the hostname, port and
             * send timeouts for that socket connection. This call should be performed on system startup before recv or send
             * are called. Note: hostname must be a dot-notation IP address of the form "x.x.x.x". DNS translation is left up
             * to the user.
             * 
             * \param hostname: ip address of remote tcp server in the form x.x.x.x
             * \param port: port of remote tcp server
             * \param send_timeout_seconds: send timeout seconds component. Defaults to: SOCKET_TIMEOUT_SECONDS
             * \param send_timeout_microseconds: send timeout microseconds component. Must be less than 1000000. Defaults to:
             * SOCKET_TIMEOUT_MICROSECONDS
             * @return SocketIpStatus, status of the configure
             */
            SocketIpStatus configure(const char* hostname, const U16 port, const U32 send_timeout_seconds,
                             const U32 send_timeout_microseconds);

            /**
             * \brief startup the TcpServer
             *
             * This will launch the TcpServer's internal tcp connection, bind to the port, and listen. This call will *not*
             * block, nor does it accept incoming connections. It configures the port to listen and then returns. In order to
             * accept incoming connections the `open` call must be made.  Only when the startup method returns SOCK_SUCCESS has
             * the server successfully bound to the port/
             *
             * \return status of startup. SOCK_SUCCESS on success, something else on error.
             */            
            SocketIpStatus startup();

             /**
             * \brief shutdown the TcpServer
             *
             * This will shutdown the TcpServer including closing any active client connections, and then closing the listening
             * port of this server.
             */
            void shutdown(); 

            /**
             * @brief Opens a Client connection
             * 
             * This is a wrapper for the IpSocket's open implementation. The sockets in the sockets array has a boolean value
             * to indicate if they are open or not. (Is currently connected to a client). 
             * 
             * Internally, will check for an available socket from the socket array, and call its' open() function. Passing in
             * a reference to a NATIVE_INT_TYPE will hold the index of the socket that was opened. This is to be used by the 
             * tasks that wishes to interact with the specific socket. 
             * 
             * @param index Reference to a NATIVE_INT_TYPE to capture the index of the opened socket within the socket array
             * @return SocketIpStatus status of opening the socket. 
             */
            SocketIpStatus open(NATIVE_INT_TYPE& index); 

            /**
             * @brief Closes the socket server
             * 
             * TODO: Not yet implemented
             * 
             * 
             */
            void closeServer(); 

            /**
             * @brief Closes all sockets within the socket array
             * 
             */
            void closeAll();

            /**
             * @brief Closes a specifcic socket within the socket array as indicated by index
             * 
             * @param index Index of a socket in the socket array to be closed. 
             */
            void close(NATIVE_INT_TYPE index); 

            /**
             * @brief Get the Server File Descriptor
             * 
             * @return NATIVE_INT_TYPE The File Descriptor of the Server listening socket
             */
            NATIVE_INT_TYPE getServerFd(); 

            /**
             * @brief Get the amount of Sockets currently connected
             * 
             * @return I32 count of sockets currently connected
             */
            I32 getNConnected(); 

            /**
             * @brief Get the Socket Handler object by index
             * 
             * @param socket_index index of the specific socket handler
             * @return Drv::MultiTcpServerSocket& 
             */
            Drv::MultiTcpServerSocket& getSocketHandler(U32 socket_index); 

            /**
             * @brief Get the Socket Handler object by device ID
             * 
             * @param device_id unsigned 32-bit value of a device id
             * 
             * Expected format of the string: "IDXXXX". The string "ID" followed by 4 hexadecimal values. 
             * Typically treated as an array of unsigned 8-bit integers (char) rather than a valid character string.
             * For example: U8 ID[6] = {'I', 'D', 0x21, 0xA3, 0xF3, 0xC7}
             * 
             * ID must be converted from string to unsigned integer. before the function is called
             * 
             * @return Drv::MultiTcpServerSocket& 
             */
            Drv::MultiTcpServerSocket& getSocketHandlerByID(U32 device_id); 
            
            char* getDeviceIP(); 
        private:
            
            NATIVE_INT_TYPE m_base_fd;          // FIle descriptor of the server listening socket
            U32 m_timeoutSeconds;               // Ip Socket timeout, seconds
            U32 m_timeoutMicroseconds;          // Ip Socket timeout, microseconds
            U16 m_port;                         // Port Number for socket
            bool m_open;                        // boolean to keep server opent
            char m_hostname[SOCKET_MAX_HOSTNAME_SIZE];      // IP address "x.x.x.x"
            NATIVE_INT_TYPE current_client_connections;     // count of current socket connections opened
            Drv::MultiTcpServerSocket sockets[MAX_CLIENTS]; // array of IpSocket implementations. 

            fd_set listener_socket; 
            timeval set_timeout; 
    };
}
#endif