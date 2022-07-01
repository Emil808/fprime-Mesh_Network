// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "Tester.hpp" 
#include <Drv/Ip/TcpClientSocket.hpp>

/*
TEST(Nominal, SingleClient_ComponentInterface){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[1]; 
    tester.test_client_opening_ID(socket, 1); 
}

TEST(Nominal, MultiClient_ComponentInterface_2){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[2]; 
    tester.test_client_opening_ID(socket, 2); 
}

TEST(Nominal, MultiClient_ComponentInterface_5){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[5]; 
    tester.test_client_opening_ID(socket, 5); 
}

TEST(Nominal, MultiClient_ComponentInterface_16){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[16]; 
    tester.test_client_opening_ID(socket, 16); 
}

TEST(Nominal, Accpet_Read_Threads_0){
    Drv::Tester tester;
    tester.test_with_loop(1, true); 
}

TEST(Nominal, Accpet_Read_Threads_5){
    Drv::Tester tester;
    tester.test_with_loop(5, true); 
}

TEST(Nominal, Accpet_Read_Threads_10){
    Drv::Tester tester;
    tester.test_with_loop(10, true); 
}

TEST(Nominal, Parallel_Read_2){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[2]; 
    tester.test_parallel_recv(socket, 2); 
}

TEST(Nominal, Parallel_Read_5){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[5]; 
    tester.test_parallel_recv(socket, 5); 
}

TEST(Nominal, Parallel_Read_10){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[10]; 
    tester.test_parallel_recv(socket, 10); 
}

TEST(Nominal, id_protocol_1){

    Drv::Tester tester;
    Drv::TcpClientSocket socket[1]; 
    tester.test_id_protocol(socket, 1); 
}


TEST(Nominal, broadcast_1){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[1]; 
    tester.test_broadcast(socket, 1, 5); 
}

TEST(Nominal, broadcast_5){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[5]; 
    tester.test_broadcast(socket, 5, 20); 
}

TEST(Nominal, broadcast_10){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[10]; 
    tester.test_broadcast(socket, 10, 30); 
}


TEST(Nominal, bounce_back_1){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[1]; 
    tester.test_client_bounce_back_from_broadcast(socket, 1, 1); 
}

TEST(Nominal, bounce_back_5){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[5]; 
    tester.test_client_bounce_back_from_broadcast(socket, 5, 10); 
}

TEST(Nominal, bounce_back_10){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[10]; 
    tester.test_client_bounce_back_from_broadcast(socket, 10, 30); 
}

TEST(Nominal, max_connections_0){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[17]; 
    tester.test_max_connections(socket, 16, 17); 
}
*/

// TEST(Nominal, test_setup){
//     Drv::Tester tester;
//     tester.test_setup();
// }

// TEST(Nominal, test_mesh_2){
//     Drv::Tester tester;

//     Drv::TcpClientSocket node_client; 
//     Drv::TcpServerSocket node_server; 
//     tester.test_node(&node_server, &node_client); 
    
// }

// TEST(Nominal, test_mesh_3){
//     Drv::Tester tester;
//     Drv::TcpClientSocket mock_client[2]; 
//     Drv::TcpServerSocket mock_server[2]; 
//     char hostfile_name[] = "../../../../Drv/MultiTcpServer/test/ut/file/test_hostfile_2.txt";
//     tester.test_mesh(mock_server, mock_client, 2, hostfile_name); 
// }

TEST(Nominal, test_mesh_5){
   
    Drv::Tester tester; 
    Drv::TcpClientSocket mock_client[4]; 
    Drv::TcpServerSocket mock_server[4]; 
    char hostfile_name[] = "../../../../Drv/MultiTcpServer/test/ut/file/test_hostfile_4.txt";
    tester.test_mesh(mock_server, mock_client, 4, hostfile_name); 
}
int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); 
}