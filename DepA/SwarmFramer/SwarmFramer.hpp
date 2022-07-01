// ======================================================================
// \title  SwarmFramer.hpp
// \author emil
// \brief  hpp file for SwarmFramer component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef SwarmFramer_HPP
#define SwarmFramer_HPP

#include "DepA/SwarmFramer/SwarmFramerComponentAc.hpp"

namespace DepA {

  class SwarmFramer :
    public SwarmFramerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object SwarmFramer
      //!
      SwarmFramer(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object SwarmFramer
      //!
      void init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object SwarmFramer
      //!
      ~SwarmFramer();

      // Set source ID
      //
      void setSourceId(U32 sourceId); 

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for bufferIn
      //!
      void bufferIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined serial input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for portIn
      //!
      void portIn_handler(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::SerializeBufferBase &Buffer /*!< The serialization buffer*/
      );

    PRIVATE:

      void frame(const U8* const data, const U32 size); 

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

      U32 node_sourceId;
      U32 node_destId;
      Os::Mutex idMutex; 

    };

} // end namespace DepA

#endif
