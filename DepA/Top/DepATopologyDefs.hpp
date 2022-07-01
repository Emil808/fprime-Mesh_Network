#ifndef DepATopologyDefs_HPP
#define DepATopologyDefs_HPP

#include "DepA/Top/FppConstantsAc.hpp" 
#include "Svc/FramingProtocol/FprimeProtocol.hpp"
#include "Fw/Types/MallocAllocator.hpp"
#include "Svc/LinuxTimer/LinuxTimer.hpp"

namespace DepA{

    extern Svc::LinuxTimer linuxTimer; 
    
    namespace Allocation{
        extern Fw::MallocAllocator mallocator; 

    }

    struct TopologyState{
        TopologyState():
            hostName(""),
            portNumber(0),
            devID(0),
            deviceHostname(""),
            devicePortNumber(0)
        {

        }

        TopologyState(const char* hostName, U32 portNumber, U32 devID, const char* deviceHostname, U32 devicePortNumber) : 
            hostName(hostName), 
            portNumber(portNumber),
            devID(devID),
            deviceHostname(deviceHostname),
            devicePortNumber(devicePortNumber)
        {

        }
        const char* hostName; 
        U32 portNumber; 
        U32 devID; 
        const char* deviceHostname; 
        U32 devicePortNumber; 
    };

    namespace PingEntries{
        namespace chanTlm { enum { WARN = 3, FATAL = 5 }; }
        namespace cmdDisp { enum { WARN = 3, FATAL = 5 }; }
        namespace cmdSeq { enum { WARN = 3, FATAL = 5 }; }
        namespace eventLogger { enum { WARN = 3, FATAL = 5 }; }
        namespace fileDownlink { enum { WARN = 3, FATAL = 5 }; }
        namespace fileDownlinkP { enum { WARN = 3, FATAL = 5 }; }
        namespace fileManager { enum { WARN = 3, FATAL = 5 }; }
        namespace fileUplink { enum { WARN = 3, FATAL = 5 }; }
        namespace prmDb { enum { WARN = 3, FATAL = 5 }; }
        namespace rateGroup1Comp { enum { WARN = 3, FATAL = 5 }; }
        namespace rateGroup2Comp { enum { WARN = 3, FATAL = 5 }; }
        namespace rateGroup3Comp { enum { WARN = 3, FATAL = 5 }; }
    }

    
}

#endif