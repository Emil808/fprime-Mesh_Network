module DepA{

    passive component SimpleReceiver{

    # ----------------------------------------------------------------------
    # General ports
    # ----------------------------------------------------------------------

    @ Port for receiving value from SimpleProducer
    sync input port valIn: SimplePort 

    # ----------------------------------------------------------------------
    # Special ports
    # ----------------------------------------------------------------------

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

    # ----------------------------------------------------------------------
    # Events
    # ----------------------------------------------------------------------

    event SR_RECEIVED_val(
        val1: F32
    ) \
    severity activity low \
    format "Simpe Receiver Value Received from Port: {f}"
    # ----------------------------------------------------------------------
    # Telemetry
    # ----------------------------------------------------------------------

    telemetry currentVal: F32
    }
}