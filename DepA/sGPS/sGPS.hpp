// ======================================================================
// \title  sGPS.hpp
// \author emil
// \brief  hpp file for sGPS component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef sGPS_HPP
#define sGPS_HPP

#include "DepA/sGPS/sGPSComponentAc.hpp"
#include <stdlib.h>

namespace DepA {

  class sGPS :
    public sGPSComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object sGPS
      //!
      sGPS(
          const char *const compName /*!< The component name*/

      );

      //! Initialize object sGPS
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object sGPS
      //!
      ~sGPS();

    protected:
      virtual void GPS_get_ec_position(DepA::ec_pos& position_struct); 

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for ping
      //!
      void ping_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< 
      The call order
      */
      );

      //! Handler implementation for remotePosition
      //!
      void remotePosition_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          const DepA::ec_pos &position 
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for SET_DESTID command handler
      //! Set Destination ID for tranmission to the mesh network
      void SET_DESTID_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          U32 destId /*!< 
          4-byte hex ID of the destination node
          */
      );


      // ----------------------------------------------------------------------
      // Private variables
      // ----------------------------------------------------------------------

      U32 destinationID; 
      U32 sourceID; 
    };

} // end namespace DepA

#endif
