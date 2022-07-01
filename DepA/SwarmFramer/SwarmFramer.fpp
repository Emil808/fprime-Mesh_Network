module DepA{

    passive component SwarmFramer{
        @ Mutexed input communication port
        guarded input port portIn: serial

        @ Mutexed input Buffer send port
        guarded input port bufferIn: Fw.BufferSend

        @ Buffer send output port
        output port bufferDeallocate: Fw.BufferSend

        @ Framed allocate output port
        output port framedAllocate: Fw.BufferGet

        @ Framed output port
        output port framedOut: Fw.BufferSend

        @ Time get port
        time get port timeGet

        # ----------------------------------------------------------------------
        # Special ports
        # ----------------------------------------------------------------------

        @ Command receive port
        command recv port cmdIn

        @ Command registration port
        command reg port cmdRegOut

        @ Command response port
        command resp port cmdResponseOut

        @ Event port
        event port eventOut

        @ Telemetry port
        telemetry port tlmOut

        @ Text event port
        text event port textEventOut
       
        # ----------------------------------------------------------------------
        # Commands
        # ----------------------------------------------------------------------

        @ Set Destination ID for tranmission to the mesh network
        guarded command SET_DESTID(
            destId: U32 @< 4-byte hex ID of the destination node
        )

        # ----------------------------------------------------------------------
        # Events
        # ----------------------------------------------------------------------
        event DESTI_ID_SET(
            destId: U32 @< 4-byte hex ID of the destination node
        ) \
        severity activity low \
        format "Destination Node set to: {x}"

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------
        telemetry sourceId: U32
        telemetry destId: U32
    }

}