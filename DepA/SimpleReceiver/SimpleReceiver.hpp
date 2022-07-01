// ======================================================================
// \title  SimpleReceiver.hpp
// \author emil
// \brief  hpp file for SimpleReceiver component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef SimpleReceiver_HPP
#define SimpleReceiver_HPP

#include "DepA/SimpleReceiver/SimpleReceiverComponentAc.hpp"

namespace DepA {

  class SimpleReceiver :
    public SimpleReceiverComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object SimpleReceiver
      //!
      SimpleReceiver(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object SimpleReceiver
      //!
      void init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object SimpleReceiver
      //!
      ~SimpleReceiver();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for valIn
      //!
      void valIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          F32 val1 
      );


    };

} // end namespace DepA2

#endif
