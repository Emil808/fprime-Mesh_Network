set(SOURCE_FILES 
        "${CMAKE_CURRENT_LIST_DIR}/MultiTcpServerComponentImpl.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/MultiTcpServerSocket.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/MultiTcpServerManager.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/MTSTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/MultiTcpClientSocket.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/SocketOpenTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/TcpClientManager.cpp"
    )
    set(MOD_DEPS
    Os
    Fw/Buffer
	"Fw/Logger"
	"Drv/ByteStreamDriverModel"
	"Drv/Ip"
    )
register_fprime_module()

### UTs ###
#set(UT_SOURCE_FILES
#	"${CMAKE_CURRENT_LIST_DIR}/../ByteStreamDriverModel/ByteStreamDriverModel.fpp"
#    "${CMAKE_CURRENT_LIST_DIR}/test/ut/TestMain.cpp"
#	"${CMAKE_CURRENT_LIST_DIR}/test/ut/Tester.cpp"
#)
set(UT_MOD_DEPS
    Os
	STest
	PortSelector
)
register_fprime_ut()
