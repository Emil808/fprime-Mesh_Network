#include <getopt.h>
#include <cstdlib>
#include <ctype.h>

#include <Os/Log.hpp>
#include <DepA/Top/DepATopologyAc.hpp>

void print_usage(const char* app) {
    (void) printf("Usage: ./%s [options]\n-p\tport_number\n-a\thostname/IP address\n",app);
}

#include <signal.h>
#include <cstdio>

DepA::TopologyState state;
// Enable the console logging provided by Os::Log
Os::Log logger;

volatile sig_atomic_t terminate = 0;

static void sighandler(int signum) {
    DepA::teardown(state);
    terminate = 1;
}

int main(int argc, char* argv[]) {
    U32 port_number = 0; // Invalid port number forced
    I32 option;
    char *hostname, *deviceHostName;
    U32 devID, devicePortNumber; 
    option = 0;
    hostname = nullptr;
    deviceHostName = nullptr; 
    devicePortNumber = 0; 
    while ((option = getopt(argc, argv, "hp:a:d:j:g:")) != -1){
        switch(option) {
            case 'h':
                print_usage(argv[0]);
                return 0;
                break;
            case 'p':
                port_number = static_cast<U32>(atoi(optarg));
                break;
            case 'a':
                hostname = optarg;
                
                break;
            case 'd':
                devID = static_cast<U32>(atoi(optarg)); 
                break; 
            case 'j': 
                deviceHostName = optarg; 
                break; 
            case 'g':
                devicePortNumber = static_cast<U32>(atoi(optarg)); 
                break; 
            case '?':
                return 1;
            default:
                print_usage(argv[0]);
                return 1;
        }

    }

    (void) printf("Hit Ctrl-C to quit\n");

    state = DepA::TopologyState(hostname, port_number, devID, deviceHostName, devicePortNumber);
    DepA::setup(state);

    // register signal handlers to exit program 
    signal(SIGINT,sighandler);
    signal(SIGTERM,sighandler);

    DepA::linuxTimer.startTimer(100); 
    
    // Give time for threads to exit
    (void) printf("Waiting for threads...\n");
    Os::Task::delay(1000);

    (void) printf("Exiting...\n");

    return 0;
}
