// ======================================================================
// \title  SwarmDeframer.cpp
// \author emil
// \brief  cpp file for SwarmDeframer component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================


#include <DepA/SwarmDeframer/SwarmDeframer.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include <Fw/Types/Assert.hpp>
#include "Utils/Types/CircularBuffer.hpp"

namespace DepA {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  SwarmDeframer ::
    SwarmDeframer(
        const char *const compName
    ) : SwarmDeframerComponentBase(compName)
  {

  }

  void SwarmDeframer ::
    init(
        const NATIVE_INT_TYPE instance
    )
  {
    SwarmDeframerComponentBase::init(instance);
  }

  SwarmDeframer ::
    ~SwarmDeframer()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void SwarmDeframer ::
    framedIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &recvBuffer
    )
  {
      U32 size = recvBuffer.getSize() -12; 
      U8* rawData = recvBuffer.getData() +12; 
      
      U32 sourceNodeID; 

      //confirm first 12 is swarm header
      if(recvBuffer.getData()[0] == 'I' && recvBuffer.getData()[6] == 'I' && recvBuffer.getData()[1] == 'D' && recvBuffer.getData()[7]== 'D'){
        Fw::ExternalSerializeBuffer wrapper(rawData, size); 
        wrapper.setBuffLen(size); 
 
        

        //extract source ID [2:5]
        sourceNodeID = (recvBuffer.getData()[2] << 24) | (recvBuffer.getData()[3] << 16) | (recvBuffer.getData()[4] << 8) | (recvBuffer.getData()[5]);
        this->tlmWrite_packetFromNode(sourceNodeID); 

        portOut_out(0, wrapper);        
      }
    
    framedDeallocate_out(0, recvBuffer); 
  }

} // end namespace DepA2
