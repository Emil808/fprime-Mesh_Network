// ======================================================================
// \title  SwarmDeframer.hpp
// \author emil
// \brief  hpp file for SwarmDeframer component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef SwarmDeframer_HPP
#define SwarmDeframer_HPP

#include "DepA/SwarmDeframer/SwarmDeframerComponentAc.hpp"

namespace DepA{

  class SwarmDeframer :
    public SwarmDeframerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object SwarmDeframer
      //!
      SwarmDeframer(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object SwarmDeframer
      //!
      void init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object SwarmDeframer
      //!
      ~SwarmDeframer();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for framedIn
      //!
      void framedIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &recvBuffer
      );


    };

} // end namespace DepA2

#endif
