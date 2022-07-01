import os
import sys

import subprocess

import fprime_gds.executables.utils

argc = len(sys.argv)
argv = sys.argv

dep_cmd = []


hostname_base = [127, 0, 1, 1]
node_port_base = 50100
id_base = 0x20202020

gds_hostname = [0, 0, 0, 0]
gds_port_base = 50000

base_gui = 5000
base_tts = 50050
base_ip  = 51000
procs = []

launchers_gds = []
launcers_dep = []
if argc > 1:
    nodes = int(argv[1]) 

    if nodes > 256: # limit maximum nodes to 255
        nodes = 256

    for x in range(nodes): # generate fprime-gds command
     
        gui_portn = base_gui + x
        tts_portn = base_tts + x
        ip_portn  = base_ip  + x 

        gds_cmd = ["fprime-gds", "-n", "--gui-port", str(gui_portn), "--tts-port", str(tts_portn), "--ip-port", str(ip_portn)]
        launchers_gds.append(gds_cmd)

    for x in range(nodes): # generate deployment command
        ip_portn = base_ip + x

        hostname_n = str(hostname_base[0]) + "." + str(hostname_base[1]) + "." + str(hostname_base[2]) + "." + str(hostname_base[3] + x)

        node_port_n = node_port_base + x

        id_n = id_base + x

        dep_cmd = ["./build-artifacts/Linux/bin/DepA", "-a", "0.0.0.0", "-p", str(ip_portn), "-j", hostname_n, "-g", str(node_port_n), "-d", str(id_n)]
        #launchers_gds.append(dep_cmd)

    print("[INFO] auto: Starting multiple fprime-gds instances")
    try:
        for launcher in launchers_gds: 
            print("Opening ", launcher) 
            child = subprocess.Popen(launcher, stdout=None, stderr=None)

            procs.append(child)

            fprime_gds.executables.utils.register_process_assassin(child)
            
        print("[INFO] auto: F prime is now running. CTRL-C to shutdown all components.")
        procs[-1].wait()
    except KeyboardInterrupt:
        print("[INFO] Auto: CTRL-C received. Exiting.")
    except Exception as exc:
        print(
            f"[INFO] Auto: Shutting down F prime due to error. {str(exc)}",
            file=sys.stderr
        )
       
    # Processes are killed atexit
    

