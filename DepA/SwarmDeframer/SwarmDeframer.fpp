module DepA{
    passive component SwarmDeframer{

        @ serial out port
        output port portOut: serial

        @ Buffer send port
        output port bufferOut: Fw.BufferSend

        @ Buffer allocate port
        output port bufferAllocate: Fw.BufferGet

        @ Buffer deallocate port
        output port bufferDeallocate: Fw.BufferSend

        @ Mutex framed input port
        guarded input port framedIn: Fw.BufferSend

        @ Framed deallocate port
        output port framedDeallocate: Fw.BufferSend

        @ Telemetry port
        telemetry port tlmOut

        @ Time get port
        time get port timeGetOut

        telemetry packetFromNode: U32

        
    }
}