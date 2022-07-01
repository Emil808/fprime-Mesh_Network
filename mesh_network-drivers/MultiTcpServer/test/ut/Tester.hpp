// ======================================================================
// \title  ByteStreamDriverModel/test/ut/Tester.hpp
// \author emil
// \brief  hpp file for ByteStreamDriverModel test harness implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef TESTER_HPP
#define TESTER_HPP

#include "GTestBase.hpp"
#include "Drv/MultiTcpServer/MultiTcpServerComponentImpl.hpp"
#include "Drv/MultiTcpServer/MultiTcpServerSocket.hpp"

#include "Drv/Ip/TcpClientSocket.hpp"


#define SEND_DATA_BUFFER_SIZE 1024

namespace Drv {

  class Tester :
    public ByteStreamDriverModelGTestBase
  {

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

    public:

      //! Construct object Tester
      //!
      Tester();

      //! Destroy object Tester
      //!
      ~Tester();

    public:

      // ----------------------------------------------------------------------
      // Tests
      // ----------------------------------------------------------------------

      
      void test_with_loop(U32 iterations, bool recv_thread = false); 

      void test_component_interface(U32 n_clients); 

      void test_client_opening_ID(Drv::TcpClientSocket* socket_array, U32 n_clients); 

      void test_parallel_recv(Drv::TcpClientSocket* socket_array, U32 n_clients); 
      
      void test_id_protocol(Drv::TcpClientSocket* socket_array, U32 n_clients); 

      void test_broadcast(Drv::TcpClientSocket* socket_array, U32 n_clients, U32 iterations); 
      
      void test_client_bounce_back_from_broadcast(Drv::TcpClientSocket* socket_array, U32 n_clients, U32 iterations); 

      void test_max_connections(Drv::TcpClientSocket* socket_array, U32 n_clients, U32 max_connections); 
      void test_socket_manager();

      void test_setup(); 

      void test_node(Drv::TcpServerSocket* server, Drv::TcpClientSocket* client); 

      void test_mesh(Drv::TcpServerSocket* mock_server, Drv::TcpClientSocket* mock_client, U32 n, const char* hostfile_name); 
      
      // ----------------------------------------------------------------------
      // Helpers
      // ----------------------------------------------------------------------

      void generate_device_id(U8* idBuffer); 

      void verify_device_id(U8* clientBuffer, U32 serverRecv); 

      void mock_server_swarm_protocol(Drv::TcpServerSocket* server, U32 serverID, U32 nodeClientID); 
    
      void mock_client_swarm_protocol(Drv::TcpClientSocket *client, U32 serverID, U32 nodeClientID);

      
    private:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------

      //! Handler for from_allocate
      //!
      Fw::Buffer from_allocate_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 size 
      );

      //! Handler for from_deallocate
      //!
      void from_deallocate_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

      //! Handler for from_ready
      //!
      void from_ready_handler(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

      //! Handler for from_recv
      //!
      void from_recv_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &recvBuffer, 
          const Drv::RecvStatus &recvStatus 
      );

    private:

      // ----------------------------------------------------------------------
      // Helper methods
      // ----------------------------------------------------------------------

      //! Connect ports
      //!
      void connectPorts();

      //! Initialize components
      //!
      void initComponents();

    private:

      // ----------------------------------------------------------------------
      // Variables
      // ----------------------------------------------------------------------

      //! The component under test
      //!
      MultiTcpServerComponentImpl component;
      Fw::Buffer m_data_buffer; 
      Fw::Buffer m_data_buffer2; 
      U8 m_data_storage[SEND_DATA_BUFFER_SIZE]; 
      bool m_spinner; 

  };

} // end namespace Drv

#endif
