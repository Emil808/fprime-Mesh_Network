// ======================================================================
// \title  sGPS.cpp
// \author emil
// \brief  cpp file for sGPS component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================


#include <DepA/sGPS/sGPS.hpp>
#include "Fw/Types/BasicTypes.hpp"

#include <math.h> 

namespace DepA {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  sGPS ::
    sGPS(
        const char *const compName
    ) : sGPSComponentBase(compName)
  {
  }

  void sGPS ::
    init(
        const NATIVE_INT_TYPE queueDepth,
        const NATIVE_INT_TYPE instance
    )
  {
    sGPSComponentBase::init(queueDepth, instance);
  }

  sGPS ::
    ~sGPS()
  {

  }

  void sGPS::GPS_get_ec_position(DepA::ec_pos& position_struct){
    I32 gpsPosX = 144 + rand() % 100; 
    I32 gpsPosY = 124 + rand() % 100; 
    I32 gpsPosZ = 903 + rand() % 100; 
    position_struct.setx(gpsPosX);
    position_struct.sety(gpsPosY);
    position_struct.setz(gpsPosZ);

  }
  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void sGPS ::
    ping_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
   
    //get gps data
    DepA::ec_pos position; 

    this->GPS_get_ec_position(position); 
    this->tlmWrite_gpsPositionX(position.getx()); 
    this->tlmWrite_gpsPositionY(position.gety()); 
    this->tlmWrite_gpsPositionZ(position.getz()); 
    
 
    this->ecPosition_out(0, position); 
  }

  void sGPS ::
    remotePosition_handler(
        const NATIVE_INT_TYPE portNum,
        const DepA::ec_pos &position
    )
  {
    
    DepA::ec_pos self_position; 
    this->GPS_get_ec_position(self_position); 

    I32 distance = pow(position.getx() - self_position.getx(), 2) + pow(position.gety() - self_position.gety(), 2) + pow(position.getz() - self_position.getz(), 2); 
    distance = sqrt(distance); 

    this->tlmWrite_distance(distance); 
    //this->log_ACTIVITY_LO_DIST_TO_REMOTE(distance);  
  }


  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void sGPS ::
    SET_DESTID_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        U32 destId
    )
  {
    this->destinationID = destId; 
    
    this->log_ACTIVITY_LO_DESTI_ID_SET(destId); 
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

} // end namespace DepA
