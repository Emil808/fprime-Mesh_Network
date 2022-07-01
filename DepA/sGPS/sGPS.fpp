module DepA{

    struct ec_pos{x: I32, y: I32, z: I32} 

    port ecPort(
        position: ec_pos
    )


    active component sGPS{
        
        output port ecPosition: ecPort
        guarded input port remotePosition: ecPort
        # --------------------------------------------
        # Special ports
        # --------------------------------------------

        async input port ping: Svc.Sched

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

        @ Time get port
        time get port timeGetOut 
        
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

        event DIST_TO_REMOTE(
            distance: I32 
        ) \
        severity activity low \
        format "Distance to remote = {x}" 

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------
        telemetry gpsTimeWeek: U32  
        telemetry gpsTimeSecond: U32
        telemetry gpsPositionX: F32  
        telemetry gpsPositionY: F32 
        telemetry gpsPositionZ: F32 

        telemetry distance: I32
        telemetry remoteID: U32
    }
}