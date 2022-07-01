// ======================================================================
// \title  SwarmFramer.cpp
// \author emil
// \brief  cpp file for SwarmFramer component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================


#include <DepA/SwarmFramer/SwarmFramer.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include <Fw/Types/Assert.hpp>
#include "Fw/Logger/Logger.hpp"

namespace DepA {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  SwarmFramer ::
    SwarmFramer(
        const char *const compName
    ) : SwarmFramerComponentBase(compName)
  {
    this->node_destId = 0x30303030; 
  }

  void SwarmFramer ::
    init(
        const NATIVE_INT_TYPE instance
    )
  {
    SwarmFramerComponentBase::init(instance);
  }

  SwarmFramer ::
    ~SwarmFramer()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void SwarmFramer ::
    bufferIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->frame(fwBuffer.getData(), fwBuffer.getSize()); 
    bufferDeallocate_out(0, fwBuffer);     
  }
  void SwarmFramer::setSourceId(U32 sourceId){
  
    this->idMutex.lock(); 
    this->node_sourceId = sourceId;
    this->idMutex.unLock(); 

   
  }
  // ----------------------------------------------------------------------
  // Handler implementations for user-defined serial input ports
  // ----------------------------------------------------------------------

  void SwarmFramer ::
    portIn_handler(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::SerializeBufferBase &Buffer /*!< The serialization buffer*/
    )
  {

    this->frame(Buffer.getBuffAddr(), Buffer.getBuffLength());    

  }
  
  void SwarmFramer::frame(const U8* const data, const U32 size){
    // get get buffer that fits framed
    U32 total_size = size + 12; 
    Fw::Buffer buffer = this->framedAllocate_out(0, total_size); 
    Fw::SerializeBufferBase& serializer = buffer.getSerializeRepr(); 

    //set source ID
    U8 swarm_header_entry[6] = {'I', 'D'}; 
    this->idMutex.lock(); 
    swarm_header_entry[2] = (this->node_sourceId >> 24) & 0xFF; 
    swarm_header_entry[3] = (this->node_sourceId >> 16) & 0xFF; 
    swarm_header_entry[4] = (this->node_sourceId >> 8) & 0xFF; 
    swarm_header_entry[5] = (this->node_sourceId >> 0) & 0xFF; 
    this->idMutex.unLock(); 
     //serialize source ID
    Fw::SerializeStatus status; 
    status = serializer.serialize(swarm_header_entry, sizeof(swarm_header_entry), true); 
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status); 

    this->idMutex.lock(); 
    swarm_header_entry[2] = (this->node_destId >> 24) & 0xFF; 
    swarm_header_entry[3] = (this->node_destId >> 16) & 0xFF; 
    swarm_header_entry[4] = (this->node_destId >> 8) & 0xFF; 
    swarm_header_entry[5] = (this->node_destId >> 0) & 0xFF;
    this->idMutex.unLock(); 

    status = serializer.serialize(swarm_header_entry, sizeof(swarm_header_entry), true); 
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status); 

    status = serializer.serialize(data, size, true); 
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status); 

    buffer.setSize(total_size); 
    
    
    framedOut_out(0, buffer); 

  }
  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void SwarmFramer ::
    SET_DESTID_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        U32 destId
    )
  {
    fprintf(stderr, "setting destination ID\n"); 
    this->idMutex.lock(); 
    this->node_destId = destId; 
    this->idMutex.unLock(); 

    this->log_ACTIVITY_LO_DESTI_ID_SET(this->node_destId); 
    this->tlmWrite_destId(this->node_destId); 
     this->tlmWrite_sourceId(this->node_sourceId); 
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

} // end namespace DepA
