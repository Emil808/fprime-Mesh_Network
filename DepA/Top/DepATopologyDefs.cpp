#include "DepATopologyDefs.hpp"

namespace DepA {

    namespace Allocation {
        Fw::MallocAllocator mallocator; 
    }

    Svc::LinuxTimer linuxTimer(FW_OPTIONAL_NAME("linuxTimer")); 
}