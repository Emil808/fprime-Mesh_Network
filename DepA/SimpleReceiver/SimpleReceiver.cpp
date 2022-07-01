// ======================================================================
// \title  SimpleReceiver.cpp
// \author emil
// \brief  cpp file for SimpleReceiver component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================


#include <DepA/SimpleReceiver/SimpleReceiver.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace DepA {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  SimpleReceiver ::
    SimpleReceiver(
        const char *const compName
    ) : SimpleReceiverComponentBase(compName)
  {

  }

  void SimpleReceiver ::
    init(
        const NATIVE_INT_TYPE instance
    )
  {
    SimpleReceiverComponentBase::init(instance);
  }

  SimpleReceiver ::
    ~SimpleReceiver()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void SimpleReceiver ::
    valIn_handler(
        const NATIVE_INT_TYPE portNum,
        F32 val1
    )
  {
    // TODO
    this->log_ACTIVITY_LO_SR_RECEIVED_val(val1); 
    this->tlmWrite_currentVal(val1); 
  }

} // end namespace DepA2
