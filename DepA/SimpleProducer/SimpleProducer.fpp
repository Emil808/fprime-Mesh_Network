module DepA{

    active component SimpleProducer{
    # ----------------------------------------------------------------------
    # General ports
    # ----------------------------------------------------------------------
    
    @ Port for sending value
    output port valOut: SimplePort

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

    @ Time get port
    time get port timeGetOut

    # ----------------------------------------------------------------------
    # Commands
    # ----------------------------------------------------------------------

    @ Produce a number and send it to Consumer
    async command SET_NUM(
        val1: F32 @< Number to send to Consumer
    )

    # ----------------------------------------------------------------------
    # Events
    # ----------------------------------------------------------------------

    event COMMAND_RECV(
        val1: F32 @< Number to send to Consumer
    ) \
    severity activity low \
    format "Simple Producer Command received: {f}" 

    # ----------------------------------------------------------------------
    # Telemetry
    # ----------------------------------------------------------------------

    telemetry VAL: F32
    }
}
