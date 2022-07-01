// ======================================================================
// \title  ByteStreamDriverModel.hpp
// \author emil
// \brief  cpp file for ByteStreamDriverModel test harness implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include "Tester.hpp"
#include "STest/Pick/Pick.hpp"
#include "Os/Log.hpp"
#include <Drv/Ip/test/ut/PortSelector.hpp>
#include <Drv/Ip/test/ut/SocketTestHelper.hpp>
#include <Os/Task.hpp>

#include <stdio.h>
#include <stdlib.h>

#define INSTANCE 0
#define MAX_HISTORY_SIZE 1000

namespace Drv {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
      ByteStreamDriverModelGTestBase("Tester", MAX_HISTORY_SIZE),
      component("ByteStreamDriverModel"), m_data_buffer(m_data_storage, 0)
  {
    this->initComponents();
    this->connectPorts();
    ::memset(m_data_storage, 0, sizeof(m_data_storage));
  }

  Tester ::
    ~Tester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------
  void Tester::test_component_interface(U32 n_clients){
    /*
    U8 buffer[sizeof(m_data_storage)] = {};
    Drv::SocketIpStatus status1 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus status2 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus serverStat = Drv::SOCK_SUCCESS;
    I32 size = sizeof(m_data_storage);
    NATIVE_INT_TYPE fd; 

    //configure server
    U16 port = Drv::Test::get_free_port(); 
    ASSERT_NE(0, port); 

    this->component.configure("127.0.0.1", port, 0, 100); 
    serverStat = this->component.startup();               // Listening for clients starts here
    EXPECT_EQ(serverStat, SOCK_SUCCESS);

    ASSERT_EQ(this->component.getNConnected(), 0);        //No clients connected yet

    // set up client
    for(U32 i = 0; i < n_clients; i++){
    Drv::TcpClientSocket client; 
    client.configure("127.0.0.1", port, 0, 100); 
    status2  = client.open(); 
    // get client to connect
    ASSERT_EQ(status2, SOCK_SUCCESS);

    status1 = this->component.getSocketHandler().open(fd); 

    ASSERT_EQ(status1, SOCK_SUCCESS); 
    
    //client send something to receiver
    Drv::Test::force_recv_timeout(this->component.getSocketHandler());
    Drv::Test::force_recv_timeout(client);
    this->m_data_buffer.setSize(sizeof(this->m_data_storage));
    Drv::Test::fill_random_buffer(this->m_data_buffer);
    status2 = client.send(this->m_data_buffer.getData(), this->m_data_buffer.getSize()); 

    ASSERT_EQ(status2, SOCK_SUCCESS); 

    status1 = this->component.recv(fd, buffer, size); //not receiving properly? 
    ASSERT_EQ(status1, Drv::SOCK_SUCCESS);
    ASSERT_EQ(size, m_data_buffer.getSize());
    Drv::Test::validate_random_buffer(m_data_buffer, buffer);
    client.close(); 

    }

    this->component.shutdown(); 
    */


  }
  void Tester::test_socket_manager(){

  

  }

  void Tester::test_client_opening_ID(Drv::TcpClientSocket* client, U32 n_clients){
    ASSERT_TRUE(client); 
    ASSERT_GT(n_clients, 0);
    // U8 buffer[sizeof(m_data_storage)] = {};
    Drv::SocketIpStatus status1 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus status2 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus serverStat = Drv::SOCK_SUCCESS;
    // I32 size = sizeof(m_data_storage);
    // NATIVE_INT_TYPE fd; 

    U8 clientID[6] = "ID"; 

    //configure server
    U16 port = Drv::Test::get_free_port(); 
    ASSERT_NE(0, port); 

    this->component.configure("127.0.0.1", port, 0, 100); 
    serverStat = this->component.startup();               // Listening for clients starts here
    EXPECT_EQ(serverStat, SOCK_SUCCESS);

    I32 client_handler_index; 
    for(U32 i = 0; i < n_clients; i++){
      client[i].configure("127.0.0.1", port, 0, 100); 
        
      status2  = client[i].open();
      ASSERT_EQ(status2, SOCK_SUCCESS); 

      status1 = this->component.getSocketManager().open(client_handler_index); 

      ASSERT_EQ(status1, SOCK_SUCCESS); 
      
      //set up and RNG a device ID
      
      generate_device_id(clientID); 
      status2 = client[i].send(clientID, sizeof(clientID)); //client sends ID

      status1 = this->component.getSocketManager().getSocketHandler(client_handler_index).recvClientID();          //server receives and saves ID

      ASSERT_EQ(status2, SOCK_SUCCESS); 
      ASSERT_EQ(status1, SOCK_SUCCESS); 

      U32 receivedID = this->component.getSocketManager().getSocketHandler(client_handler_index).getClientDeviceID(); 

      verify_device_id(clientID, receivedID); 

      ASSERT_EQ(this->component.getSocketManager().getNConnected(), 1); 
      client[i].close(); 
      this->component.getSocketManager().close(client_handler_index); 
      ASSERT_EQ(this->component.getSocketManager().getNConnected(), 0);
    }
      this->component.getSocketManager().closeAll(); 
      this->component.shutdown(); 

  }
  void Tester::test_with_loop(U32 iterations, bool recv_thread)
  {
    ASSERT_GT(iterations, 0); 
    //U8 buffer[sizeof(m_data_storage)] = {};
    //Drv::SocketIpStatus status1 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus status2 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus serverStat = Drv::SOCK_SUCCESS;

    U16 port =  Drv::Test::get_free_port();
    ASSERT_NE(0, port);

     this->component.configure("127.0.0.1", port, 0, 100);
     serverStat = this->component.startup();                // Server starts listening here at success
     EXPECT_EQ(serverStat, SOCK_SUCCESS);

    // start accept task
    if (recv_thread){
      Os::TaskString name("Accept Client Thread");
      this->component.startAcceptTask(name, true, false, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT); 
    }

    for (U32 i = 0; i < iterations && serverStat == SOCK_SUCCESS; i++){
      Drv::TcpClientSocket client; 
      client.configure("127.0.0.1", port, 0, 100);
      status2 = client.open(); 
      //I32 size = sizeof(m_data_storage); 
      ASSERT_EQ(status2, SOCK_SUCCESS); 

      Os::Task::delay(10); //Delay to let other threads get to update Nconnected
      EXPECT_EQ(this->component.getSocketManager().getNConnected(), i+1); 
      EXPECT_EQ(this->component.getTaskCount(), i+1); 

      m_data_buffer.setSize(sizeof(m_data_storage));
      Drv::Test::fill_random_buffer(m_data_buffer);

      m_spinner = false;
      m_data_buffer.setSize(sizeof(m_data_storage));
      client.send(m_data_buffer.getData(), m_data_buffer.getSize());
      while (not m_spinner) {}

      client.close(); 

      // Want to do detect closure, but currently failing? 
      // Expecting getNConnected() to be 0 after close. 
    }
    this->component.shutdown();
    EXPECT_EQ(this->component.getSocketManager().getNConnected(), 0);
    this->component.stopAcceptTask(); 
    this->component.joinAcceptTask(nullptr); 
    
  }

  void Tester::test_parallel_recv(Drv::TcpClientSocket* socket_array, U32 n_clients){
    ASSERT_TRUE(socket_array); 
    ASSERT_GT(n_clients, 0); 
    //Drv::SocketIpStatus status2 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus serverStat = Drv::SOCK_SUCCESS;
    I32 client_count = static_cast<I32>(n_clients); 
    U16 port =  Drv::Test::get_free_port();
    ASSERT_NE(0, port);

     this->component.configure("127.0.0.1", port, 0, 100);
     serverStat = this->component.startup();                // Server starts listening here at success
     EXPECT_EQ(serverStat, SOCK_SUCCESS);

    Os::TaskString name("Accept Client Thread");
    this->component.startAcceptTask(name, true, false, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT); 
    
    //open multiple clients
    //have clients send, which invokes the recv thread

    for(int i = 0; i < client_count; i++){
      socket_array[i].configure("127.0.0.1", port, 0, 100);
      ASSERT_EQ(socket_array[i].open(), SOCK_SUCCESS); 
      Os::Task::delay(10); //delay for other threads to update N-connected
      EXPECT_EQ(this->component.getSocketManager().getNConnected(), i+1); 
    }
     
    
    ASSERT_EQ(this->component.getSocketManager().getNConnected(), n_clients); 

    for(int i = 0; i < client_count; i++){
      m_spinner = false;

      m_data_buffer.setSize(sizeof(m_data_storage));
      Drv::Test::fill_random_buffer(m_data_buffer);
      socket_array[i].send(m_data_buffer.getData(), m_data_buffer.getSize()); 
      while (not m_spinner) {}
    }


    for(int i = 0; i < client_count; i++){
      socket_array[i].close(); 
    }

    this->component.shutdown();
    EXPECT_EQ(this->component.getSocketManager().getNConnected(), 0);
    this->component.stopAcceptTask(); 
    this->component.joinAcceptTask(nullptr); 
  }

  void Tester::test_id_protocol(Drv::TcpClientSocket* socket_array, U32 n_clients){
    ASSERT_TRUE(socket_array); 
    ASSERT_GT(n_clients, 0);
    //Drv::SocketIpStatus status2 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus serverStat = Drv::SOCK_SUCCESS;
    I32 client_count = static_cast<I32>(n_clients); 
    U16 port =  Drv::Test::get_free_port();
    ASSERT_NE(0, port);

    this->component.configure("127.0.0.1", port, 0, 100);
    serverStat = this->component.startup();                // Server starts listening here at success
    EXPECT_EQ(serverStat, SOCK_SUCCESS);

    Os::TaskString name("Accept Client Thread");
    this->component.startAcceptTask(name, true, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT); 

    U8 device_ID[6] = "ID"; 

    for(int i = 0; i < client_count; i++){
      socket_array[i].configure("127.0.0.1", port, 0, 100);
      ASSERT_EQ(socket_array[i].open(), SOCK_SUCCESS); 
      Os::Task::delay(10); //delay for other threads to update N-connected
      EXPECT_EQ(this->component.getSocketManager().getNConnected(), i+1); 

      generate_device_id(device_ID);

      socket_array[i].send(device_ID, sizeof(device_ID)); 

      Os::Task::delay(10); 
    }
     
    
    ASSERT_EQ(this->component.getSocketManager().getNConnected(), n_clients); 

    for(int i = 0; i < client_count; i++){
      m_spinner = false;

      m_data_buffer.setSize(sizeof(m_data_storage));
      Drv::Test::fill_random_buffer(m_data_buffer);
      socket_array[i].send(m_data_buffer.getData(), m_data_buffer.getSize()); 
      while (not m_spinner) {}
    }
    for(int i = 0; i < client_count; i++){
      socket_array[i].close(); 
    }

    this->component.shutdown();
    EXPECT_EQ(this->component.getSocketManager().getNConnected(), 0);
    this->component.stopAcceptTask(); 
    this->component.joinAcceptTask(nullptr); 
  }

  void Tester::test_broadcast(Drv::TcpClientSocket* socket_array, U32 n_clients, U32 iterations){
    ASSERT_TRUE(socket_array); 
    ASSERT_GT(n_clients, 0);
    ASSERT_GT(iterations, 0); 
    Drv::SocketIpStatus status = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus serverStat = Drv::SOCK_SUCCESS;
    Drv::SendStatus sendStat = Drv::SendStatus::SEND_OK;  
    I32 client_count = static_cast<I32>(n_clients); 
    U16 port =  Drv::Test::get_free_port();
    ASSERT_NE(0, port);

    this->component.configure("127.0.0.1", port, 0, 100);
    serverStat = this->component.startup();                // Server starts listening here at success
    EXPECT_EQ(serverStat, SOCK_SUCCESS);

    // Start Accept Task
    Os::TaskString name("Accept Client Thread");
    this->component.startAcceptTask(name, true, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT); 

    U8 device_ID[6] = {'I', 'D'}; 

    // Set Up clients and connect. With deviceId protocol
    for(int i = 0; i < client_count; i++){
      socket_array[i].configure("127.0.0.1", port, 0, 100);
      ASSERT_EQ(socket_array[i].open(), SOCK_SUCCESS); 
      Os::Task::delay(10); //delay for other threads to update N-connected
      EXPECT_EQ(this->component.getSocketManager().getNConnected(), i+1); 

      generate_device_id(device_ID);

      socket_array[i].send(device_ID, sizeof(device_ID)); 

      Os::Task::delay(10); 
    }
     
    
    ASSERT_EQ(this->component.getSocketManager().getNConnected(), n_clients); 

    // Server broadcast to all
    U8 test_data[] = {'I', 'D', '2', '0', '2', '0', 'I', 'D', '0', '0', '0', '0', 'A', 'B', 'C'}; 

    m_data_buffer.setData(test_data); 
    m_data_buffer.setSize(sizeof(test_data)); 
    Drv::Test::validate_random_data(m_data_buffer.getData(), test_data, m_data_buffer.getSize()); 

    U8 buffer[sizeof(m_data_storage)] = {};
    I32 size = sizeof(m_data_storage);

    for(U32 k = 0; k < iterations; k++){
      test_data[14] += k; // changes test data a little bit each iteration

      sendStat = this->invoke_to_send(0, m_data_buffer); 

      ASSERT_EQ(sendStat, Drv::SendStatus::SEND_OK); 

      for(int i = 0; i < client_count; i++){
        status = socket_array[i].recv(buffer, size); 

        EXPECT_EQ(status, SOCK_SUCCESS); 

        Drv::Test::validate_random_data(m_data_buffer.getData(), buffer, m_data_buffer.getSize()); 
      }
    }
    
    //finished, so delete m_data_buffer
    m_data_buffer.setSize(0);  

    for(int i = 0; i < client_count; i++){
      socket_array[i].close(); 
    }

    this->component.shutdown();
    EXPECT_EQ(this->component.getSocketManager().getNConnected(), 0);
    this->component.stopAcceptTask(); 
    this->component.joinAcceptTask(nullptr); 

  }

  void Tester::test_client_bounce_back_from_broadcast(Drv::TcpClientSocket* socket_array, U32 n_clients, U32 iterations){
    ASSERT_TRUE(socket_array); 
    ASSERT_GT(n_clients, 0);
    ASSERT_GT(iterations, 0); 
    Drv::SocketIpStatus status = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus serverStat = Drv::SOCK_SUCCESS;
    Drv::SendStatus sendStat = Drv::SendStatus::SEND_OK;  
    I32 client_count = static_cast<I32>(n_clients); 
    U16 port =  Drv::Test::get_free_port();
    ASSERT_NE(0, port);

    U8 device_ID_list [n_clients][6]; 

    this->component.configure("127.0.0.1", port, 0, 100);
    serverStat = this->component.startup();                // Server starts listening here at success
    EXPECT_EQ(serverStat, SOCK_SUCCESS);

    // Start Accept Task
    Os::TaskString name("Accept Client Thread");
    this->component.startAcceptTask(name, true, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT); 

    U8 device_ID[6] = {'I', 'D'}; 

    // Set Up clients and connect. With deviceId protocol
    for(int i = 0; i < client_count; i++){
      socket_array[i].configure("127.0.0.1", port, 0, 100);
      ASSERT_EQ(socket_array[i].open(), SOCK_SUCCESS); 
      Os::Task::delay(10); //delay for other threads to update N-connected
      EXPECT_EQ(this->component.getSocketManager().getNConnected(), i+1); 

      generate_device_id(device_ID);
      device_ID_list[i][0] = device_ID[0]; 
      device_ID_list[i][1] = device_ID[1]; 
      device_ID_list[i][2] = device_ID[2]; 
      device_ID_list[i][3] = device_ID[3]; 
      device_ID_list[i][4] = device_ID[4]; 
      device_ID_list[i][5] = device_ID[5]; 
      socket_array[i].send(device_ID, sizeof(device_ID)); 

      Os::Task::delay(10); 
    }
     
    
    ASSERT_EQ(this->component.getSocketManager().getNConnected(), n_clients); 

    // Server broadcast to all
    // Test Header format
    // 6 bytes sender ID, 6 bytes receiver ID, 4 bytes data length, x-bytes data
    U8 test_data[] = {'I', 'D', '2', '0', '2', '0', 'I', 'D', '0', '0', '0', '0', '0', '0', '0', '3', 'A', 'B', 'C'}; 
    U8 bounce_data[19]; 
    m_data_buffer.setData(test_data); 
    m_data_buffer.setSize(sizeof(test_data)); 
    Drv::Test::validate_random_data(m_data_buffer.getData(), test_data, m_data_buffer.getSize()); 

    U8 buffer[sizeof(m_data_storage)] = {};
    I32 size = sizeof(m_data_storage);


    U8 senderID[6];
    U32 receiverID_int; 
    char data_length_s[5]; 
    U8 data[sizeof(m_data_storage)] ={}; 
    I32 data_length; 

    for(U32 k = 0; k < iterations; k++){
      m_data_buffer.setData(test_data); 
      m_data_buffer.setSize(sizeof(test_data)); 
      test_data[14] += k; // changes test data a little bit each iteration

      sendStat = this->invoke_to_send(0, m_data_buffer); //send broadcast message to all

      ASSERT_EQ(sendStat, Drv::SendStatus::SEND_OK); 

      for(int i = 0; i < client_count; i++){ //for each client 
        status = socket_array[i].recv(buffer, size); //receive, 
        EXPECT_EQ(status, SOCK_SUCCESS); 

        //Process 

        //extract sender ID, 
        senderID[0] = buffer[0]; 
        senderID[1] = buffer[1]; 
        senderID[2] = buffer[2]; 
        senderID[3] = buffer[3]; 
        senderID[4] = buffer[4]; 
        senderID[5] = buffer[5]; 

        //extract receive ID (expect self's ID or broadcasts
        receiverID_int = (buffer[8] << 24) + (buffer[9] << 16) + (buffer[10] << 8) + buffer[11]; 
        EXPECT_TRUE( (receiverID_int == 0x30303030)); //expecting broadcast
        //extract data length
        data_length_s[0] = buffer[12]; 
        data_length_s[1] = buffer[13];
        data_length_s[2] = buffer[14];
        data_length_s[3] = buffer[15];
        data_length = atoi(data_length_s); 

        for(I32 j = 0; j < data_length; j++){ //extract remaining data. 
          if(static_cast<U32>(j) > sizeof(m_data_storage)){
            break; 
          }
          data[i] = buffer[j+16]; 
        }
        
        //repackage data, 
        //pack senders ID
        bounce_data[0] = device_ID_list[i][0]; 
        bounce_data[1] = device_ID_list[i][1]; 
        bounce_data[2] = device_ID_list[i][2]; 
        bounce_data[3] = device_ID_list[i][3]; 
        bounce_data[4] = device_ID_list[i][4]; 
        bounce_data[5] = device_ID_list[i][5];

        //pack receiverd ID
        bounce_data[6] = senderID[0];
        bounce_data[7] = senderID[1];
        bounce_data[8] = senderID[2];
        bounce_data[9] = senderID[3];
        bounce_data[10] = senderID[4];
        bounce_data[11] = senderID[5];

        //pack data length
        bounce_data[12] = '0'; 
        bounce_data[13] = '0'; 
        bounce_data[14] = '0';
        bounce_data[15] = '3'; 
        
        //pack data
        bounce_data[16] = data[0]; 
        bounce_data[17] = data[1]; 
        bounce_data[18] = data[2]; 
        
        //send data
        m_data_buffer.setData(bounce_data); 
        m_data_buffer.setSize(sizeof(bounce_data)); 

        socket_array[i].send(bounce_data, sizeof(bounce_data)); 
        Os::Task::delay(10); // let receive thread process and server handle data coming in

        //recv handler will verify data
      }
    }
    Os::Task::delay(10); //let recv thread catch up

    //finished, so delete m_data_buffer
    m_data_buffer.setSize(0);  

    for(int i = 0; i < client_count; i++){
      socket_array[i].close(); 
    }

    this->component.shutdown();
    EXPECT_EQ(this->component.getSocketManager().getNConnected(), 0);
    this->component.stopAcceptTask(); 
    this->component.joinAcceptTask(nullptr); 

  } 

  void Tester::test_max_connections(Drv::TcpClientSocket* socket_array, U32 n_clients, U32 max_connections){
    ASSERT_TRUE(socket_array); 
    ASSERT_GT(n_clients, 0);
    ASSERT_EQ(max_connections, n_clients+1); 
    Drv::SocketIpStatus status = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus serverStat = Drv::SOCK_SUCCESS;
    Drv::SendStatus sendStat = Drv::SendStatus::SEND_OK;  
    
    U16 port =  Drv::Test::get_free_port();
    ASSERT_NE(0, port);

    
    this->component.configure("127.0.0.1", port, 0, 100);
    serverStat = this->component.startup();                // Server starts listening here at success
    EXPECT_EQ(serverStat, SOCK_SUCCESS);

    // Start Accept Task
    Os::TaskString name("Accept Client Thread");
    this->component.startAcceptTask(name, true, false, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT); //accept task no ID protocol

    //initialize n_clients clients
    for(U32 i = 0; i < n_clients; i++){
      socket_array[i].configure("127.0.0.1", port, 0, 100);
      ASSERT_EQ(socket_array[i].open(), SOCK_SUCCESS); 
      Os::Task::delay(10); //delay for other threads to update N-connected
    }

    EXPECT_EQ(this->component.getSocketManager().getNConnected(), n_clients); 

    //check their connections
    for(U32 i = 0; i < n_clients; i++){
      EXPECT_TRUE(socket_array[i].isOpened()); 
    }
    
    Os::Task::delay(10);
    // at this point, we should have reached maximum clients, of 16, then the 17th connection should fail? 
    // but it is getting accepted, and returning success

    //
    socket_array[max_connections-1].configure("127.0.0.1", port, 1, 100);
    status = socket_array[max_connections-1].open(); 
    while(1){}; 
    ASSERT_EQ(status, SOCK_FAILED_TO_CONNECT); //expect failure

    socket_array[0].close(); 
    Os::Task::delay(10);
    EXPECT_EQ(this->component.getSocketManager().getNConnected(), n_clients-1);

    
    status = socket_array[max_connections-1].open(); //connect 17th
    ASSERT_EQ(status, SOCK_SUCCESS); 
    
    EXPECT_TRUE(socket_array[max_connections-1].isOpened()); 

    for(U32 i = 1; i < max_connections; i++){
      socket_array[i].close(); 
    }

    this->component.shutdown();
    EXPECT_EQ(this->component.getSocketManager().getNConnected(), 0);
    this->component.stopAcceptTask(); 
    this->component.joinAcceptTask(nullptr); 
    
  }

  void Tester::test_setup(){

    SocketIpStatus status = SOCK_SUCCESS; 
    status = this->component.configure("127.0.0.1", 8080, 0, 100); //configure server
    ASSERT_EQ(status, SOCK_SUCCESS); 

    this->component.setDeviceID(45);                                   //set Device ID
    char hostfile_name[] = "test/ut/file/test_hostfile_0.txt";                       //set hostefile 
    this->component.setHostFile(hostfile_name, sizeof(hostfile_name)); 
    
    this->component.startup(); 

    Os::TaskString name_acceptTask("AcceptTask"); 
    this->component.startAcceptTask(name_acceptTask, true, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT);
    Os::TaskString name_openTask("OpenTask"); 
    this->component.startSocketOpenTask(name_openTask, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT); 

    //want to test that at least one task is opening, gdb will make the expect true,
    //running normally will cause it to fail
    // Os::Task::delay(100); 
    // EXPECT_GT(this->component.getTaskCount(), 0); 

    this->component.stopAcceptTask(); 
    this->component.stopSocketOpenTask(); 
    Os::Task::delay(10); 

    this->component.joinAcceptTask(NULL); 
    this->component.joinSocketOpenTask(NULL);     
    }
  
  void Tester::test_node(Drv::TcpServerSocket* server, Drv::TcpClientSocket* client){
    ASSERT_TRUE(server); 
    ASSERT_TRUE(client); 
     
    int basePort = 8080; 
    int baseID = 0x20202020; 
    SocketIpStatus status = SOCK_SUCCESS; 
    SocketIpStatus status1 = SOCK_SUCCESS; 
    status = this->component.configure("127.0.0.1", basePort, 0, 100); //configure node_0 (this)
    ASSERT_EQ(status, SOCK_SUCCESS); 

    this->component.setDeviceID(baseID); 

    //NOTE: File fprime-util check relative to the build cache in:
    // build-fprime-automatic-native-ut/F-Prime/Drv/MultiTcpServer/<location of Tester.cpp that check uses> 
    char hostfile_name[] = "../../../../Drv/MultiTcpServer/test/ut/file/test_hostfile_1.txt"; 
    this->component.setHostFile(hostfile_name, sizeof(hostfile_name)); 

    server->configure("127.0.0.1", 8081, 0, 100); //mock server node
    server->startup(); 

    client->configure("127.0.0.1", basePort, 0, 100); //mock client node

    //startup, start tasks

    this->component.startup(); 

    Os::TaskString name_acceptTask("AcceptTask"); 
    this->component.startAcceptTask(name_acceptTask, true, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT);
    Os::TaskString name_openTask("OpenTask"); 
    this->component.startSocketOpenTask(name_openTask, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT); 

    //mock server node operations
    status1 = server->open(); 
    EXPECT_EQ(status1, SOCK_SUCCESS); 

    Os::Task::delay(10); 

    this->mock_server_swarm_protocol(server, 0x20202025, baseID); //this client to node server
    
    status1 = client->open(); 
    EXPECT_EQ(status1, SOCK_SUCCESS); 
    this->mock_client_swarm_protocol(client, baseID, 0x20202025); //node client to this server
    Os::Task::delay(10); 

    //getting here, then asserts passed in the above mock swarm protocol functions

    U8 data[100] = {'I', 'D', 0x20, 0x20, 0x20, 0x20, 'I', 'D', 0x20, 0x20, 0x20, 0x25, 'A', 'B', 'D'}; // from device 0x20202020 to device 0x20202025
    //node 0 send data to node 1
    m_data_buffer.setData(data); 
    m_data_buffer.setSize(sizeof(data)); 
    m_data_buffer.setContext(0);   
    
    this->invoke_to_send(0, m_data_buffer); 

    U8 received_Data[100]; 

    I32 received_size = sizeof(received_Data); 
    status1 = server->recv(received_Data, received_size); 
    
    ASSERT_EQ(status1, SOCK_SUCCESS); 
    Drv::Test::validate_random_data(received_Data, m_data_buffer.getData(), sizeof(received_Data));

    m_data_buffer.setSize(0); 

    this->component.stopAcceptTask(); 
    this->component.stopSocketOpenTask(); 
    Os::Task::delay(10); 

    this->component.joinAcceptTask(NULL); 
    this->component.joinSocketOpenTask(NULL);    

    server->close(); 
    client->close();  
    this->component.shutdown(); 
  }

  void Tester::test_mesh(Drv::TcpServerSocket* mock_server, Drv::TcpClientSocket* mock_client, U32 n, const char* hostfile_name){
   
    //validate inputs
    ASSERT_TRUE(mock_server);
    ASSERT_TRUE(mock_client); 
    ASSERT_GT(n, 0); 
    ASSERT_TRUE(hostfile_name); 

    int basePort = 8080; 
    int baseID = 0x20202020; 
  
    SocketIpStatus status1 = SOCK_SUCCESS; 

    status1 = this->component.configure("127.0.0.1", basePort, 0, 100); 
    ASSERT_EQ(status1, SOCK_SUCCESS); 
    this->component.setDeviceID(baseID); 
    
    //char hostfile_name[] = "test/ut/file/test_hostfile_2.txt";
    
    this->component.setHostFile(hostfile_name, sizeof(hostfile_name)); 
    
    status1 = this->component.startup(); 

    ASSERT_EQ(status1, SOCK_SUCCESS); 

    for(U32 i = 1; i <= n; i++){ 
      fprintf(stderr, "configuring server at port: %d\n", basePort+i);
      mock_server[i-1].configure("127.0.0.1", basePort+i, 0, 100); 
      status1 = mock_server[i-1].startup(); 
      ASSERT_EQ(status1, SOCK_SUCCESS); 

      mock_client[i-1].configure("127.0.0.1", basePort, 0, 100);
    }

    Os::TaskString name_acceptTask("AcceptTask"); 
    this->component.startAcceptTask(name_acceptTask, true, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT);
    Os::TaskString name_openTask("OpenTask"); 
    this->component.startSocketOpenTask(name_openTask, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT); 

    Os::Task::delay(10); 
     fprintf(stderr, "START\n"); 
    // swarm connection
    for(U32 i = 1; i <= n; i++){ 
      fprintf(stderr, "opening server at port: %d\n", basePort+i); 
      mock_server[i-1].open(); 
      Os::Task::delay(10); 
      this->mock_server_swarm_protocol(&mock_server[i-1], baseID+i, baseID); 
      fprintf(stderr, "Mock Server %d opened, %d\n", i-1, basePort+i); 

      mock_client[i-1].open(); 
      Os::Task::delay(10); 
      this->mock_client_swarm_protocol(&mock_client[i-1], baseID, baseID+i); 
      fprintf(stderr, "Mock Client %d opened\n", i-1); 
    }

    //this node sending data
    U8 data[100] = {'I', 'D', 0x20, 0x20, 0x20, 0x20, 'I', 'D', 0x20, 0x20, 0x20, 0x20, 'A', 'B', 'D'}; 
    U8 received_Data[100]; 
    I32 received_Size = sizeof(received_Data); 
    //index 11: LSB of destination ID
    for(U32 i = 1; i <=n; i++){
      fprintf(stderr, "Checking Node to mock %d\n", i); 
      data[11] = 0x20 + i; 
      m_data_buffer.setData(data); 
      m_data_buffer.setSize(sizeof(data)); 
      m_data_buffer.setContext(0); 

      this->invoke_to_send(0, m_data_buffer); 

      status1 = mock_server[i-1].recv(received_Data, received_Size); 

      ASSERT_EQ(status1, SOCK_SUCCESS); 
      Drv::Test::validate_random_data(received_Data, m_data_buffer.getData(), sizeof(received_Data));

      m_data_buffer.setSize(0); 
    }


    //this node receiving data
    data[11] = 0x20; 
    for(U32 i = 1; i <= n; i++){
      data[5] = 0x20 + i; 
      m_data_buffer.setData(data); 
      m_data_buffer.setSize(sizeof(data)); 

      status1 = mock_client[i-1].send(m_data_buffer.getData(), m_data_buffer.getSize()); 
      Os::Task::delay(10); 
    }

    for(U32 i = 1; i <= n; i++){
      mock_client[i-1].close(); 
      mock_server[i-1].close(); 
    }

    this->component.stopAcceptTask(); 
    this->component.stopSocketOpenTask(); 

    this->component.joinAcceptTask(NULL); 
    this->component.joinSocketOpenTask(NULL);

  }
  void Tester::generate_device_id(U8* idBuffer){
    int size = sizeof(idBuffer); 
    if(size == 6){
      idBuffer[2] = STest::Pick::lowerUpper(0, 0xFF); 
      idBuffer[3] = STest::Pick::lowerUpper(0, 0xFF); 
      idBuffer[4] = STest::Pick::lowerUpper(0, 0xFF); 
      idBuffer[5] = STest::Pick::lowerUpper(0, 0xFF); 
    }
  } 

  void Tester::verify_device_id(U8* clientBuffer, U32 serverRecv){
    ASSERT_TRUE(clientBuffer); 
    int size = sizeof(clientBuffer); 
    if(size == 6){
      ASSERT_EQ(clientBuffer[5], (serverRecv & 0xFF)>>0); 
      ASSERT_EQ(clientBuffer[4], (serverRecv & 0xFF00)>>8);
      ASSERT_EQ(clientBuffer[3], (serverRecv & 0xFF0000)>>16);
      ASSERT_EQ(clientBuffer[2], (serverRecv & 0xFF000000)>>24);
    }
  }
  
  void Tester::mock_server_swarm_protocol(Drv::TcpServerSocket* server, U32 serverID, U32 ClientID){
    
    ASSERT_TRUE(server->isOpened()); 

    SocketIpStatus status = SOCK_SUCCESS; 

    //receive client ID, 
    U8 data[6]; 
    I32 size = sizeof(data); 
    U32 ID; 

    status = server->recv(data, size); 
    ASSERT_EQ(status, SOCK_SUCCESS); 

    ASSERT_EQ(data[0], 'I'); 
    ASSERT_EQ(data[1], 'D'); 
    ID = (data[2] << 24) + (data[3] << 16) + (data[4] << 8) + (data[5] << 0); //receive this client id

    ASSERT_EQ(ClientID, ID); 

    data[0] = 'I'; data[1] = 'D'; //package serverID
    data[2] = (serverID >> 24) & 0xFF; 
    data[3] = (serverID >> 16) & 0xFF; 
    data[4] = (serverID >> 8) & 0xFF; 
    data[5] = (serverID >> 0) & 0xFF; 

    status = server->send(data, sizeof(data)); //send serverID
    ASSERT_EQ(status, SOCK_SUCCESS); 

  }

  void Tester::mock_client_swarm_protocol(Drv::TcpClientSocket *client, U32 serverID, U32 ClientID){
    ASSERT_TRUE(client); 

    SocketIpStatus status = SOCK_SUCCESS; 

    //receive client ID, 
    U8 data[6]; 
    I32 size = sizeof(data); 
    U32 ID; 

    data[0] = 'I'; data[1] = 'D'; //package client ID
    data[2] = (ClientID >> 24) & 0xFF; 
    data[3] = (ClientID >> 16) & 0xFF; 
    data[4] = (ClientID >> 8) & 0xFF; 
    data[5] = (ClientID >> 0) & 0xFF; 

    status = client->send(data, sizeof(data)); //send client ID 
    ASSERT_EQ(status, SOCK_SUCCESS); 

    status = client->recv(data, size); //receive the server ID
    
    ASSERT_EQ(data[0], 'I'); 
    ASSERT_EQ(data[1], 'D'); 
    ID = (data[2] << 24) + (data[3] << 16) + (data[4] << 8) + (data[5] << 0); //receive node client id
    ASSERT_EQ(serverID, ID); 
  }
  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Fw::Buffer Tester ::
    from_allocate_handler(
        const NATIVE_INT_TYPE portNum,
        U32 size
    )
  {
    this->pushFromPortEntry_allocate(size);
    // TODO: Return a value
    Fw::Buffer buffer(new U8[size], size); 
    m_data_buffer2 = buffer; 
    return buffer; 
  }

  void Tester ::
    from_deallocate_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->pushFromPortEntry_deallocate(fwBuffer);
  }

  void Tester ::
    from_ready_handler(
        const NATIVE_INT_TYPE portNum
    )
  {
    this->pushFromPortEntry_ready();
  }

  void Tester ::
    from_recv_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &recvBuffer,
        const Drv::RecvStatus &recvStatus
    )
  {
    this->pushFromPortEntry_recv(recvBuffer, recvStatus);

    EXPECT_EQ(m_data_buffer.getSize(), recvBuffer.getSize()) << "Invalid transmission size";
    Drv::Test::validate_random_buffer(m_data_buffer, recvBuffer.getData());
    m_spinner = true;
    delete[] recvBuffer.getData();

  }

  // ----------------------------------------------------------------------
  // Helper methods
  // ----------------------------------------------------------------------

  void Tester ::
    connectPorts()
  {

    // poll
    this->connect_to_poll(
        0,
        this->component.get_poll_InputPort(0)
    );

    // send
    this->connect_to_send(
        0,
        this->component.get_send_InputPort(0)
    );

    // allocate
    this->component.set_allocate_OutputPort(
        0,
        this->get_from_allocate(0)
    );

    // deallocate
    this->component.set_deallocate_OutputPort(
        0,
        this->get_from_deallocate(0)
    );

    // ready
    this->component.set_ready_OutputPort(
        0,
        this->get_from_ready(0)
    );

    // recv
    this->component.set_recv_OutputPort(
        0,
        this->get_from_recv(0)
    );




  }

  void Tester ::
    initComponents()
  {
    this->init();
    this->component.init(
        INSTANCE
    );
  }

} // end namespace Drv
