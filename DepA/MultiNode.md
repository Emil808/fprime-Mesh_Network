# Multi-Node Mesh Network (Swarm Network)

## To run the example for a 5 Node network
Example for a 5 Node network on Linux (tested on Ubuntu 20.04). Assuming you can build and run `fprime-gds` for the `/Ref` example.

1. Within the `/DepA` directory, run `fprime-gds build` 
2. Open up 10 Terminals, and change directory into `/DepA`
3. For 5 terminal, run each command to open a fprime-gds instance. The commands can be found in [commands_to_run.txt](commands_to_run.txt)
4. For the next 5 terminals, run each command to start a deployment instance. The commands can be found in [commands_to_run.txt](commands_to_run.txt)
Suggesetions to streamline the above process will be appreciated. 

Now that the Deployments are running, connected to each other and to the fprime-gds, on the fprime-gds interface you can issue these commands: 

### swarmFramerGPS.SET_DESTID 
- Sets the Destination ID for the Node to receive the sending Node's GPS position

### swarmFramer.SET_DESTID    
- Sets the Destination ID for the Node to receive the sending Node's Simple Producer data. 

### SimpleProducer.SET_NUM    
- Invokes the Simple Producer to output the argument (Float 32-bit). The receiving Node, based on the most recent swarmFramer.SET_DESTID, should send a Low Activity Event to the GDS it is connected to. 

Node Destination IDS:
- 0x20202020
- 0x20202021
- 0x20202022
- 0x20202023
- 0x20202024
- 0x30303030 - For now, a temporary Broadcast ID. Setting DEST_ID to this, all Nodes will receive the data packet. 




## About 
This deployment uses a custom implementation of the byte stream driver model, [Multi Tcp Server](../Drv/MultiTcpServer/), to allow data passing within a mesh network. Each node in the mesh network is a stand alone Fprime Deployment, and each deployment can be identical. 

To make each node unique, they will have their own Swarm ID. Below is an example of the data packet as it arrives to the Multi Tcp Server component before it is transmited over the TCP protocol: 

![Data Packet with Swarm Header](img/DataPacket.png)

The swarm header works in the similar way to the TCP's source and destination fields. 

A Multi Tcp Server consists of a Multi-Threaded TCP Server and multiple TCP Client Sockets. The TCP Server will listen and accept incoming TCP Client connection requests, and relegate data receiving to a read thread per client, similar to the current operation in [Drv/Ip/SocketReadTask.hpp](../Drv/Ip/SocketReadTask.hpp). This handles receiving data from other Nodes in the network. 

The multiple TCP Client Sockets handle sending data out to other Nodes in the network. The [node host file](node_host_file_0.txt) contains a list of Ip-addresses and ports of the other nodes in the network. The [socket open task](../Drv/MultiTcpServer/SocketOpenTask.hpp) will read an entry in the file, configure a TCP Client Socket, and attempt to connect to the other Node. On successesfull TCP handshake, the receiving node and sending node will do a Swarm handshake.

For the swarm handshake, the client side will send the swarm device's ID. The server, on receiving the device ID, will send its own device ID. The client receiving the server's device ID will store it within the [Multi Tcp Client Socket](../Drv/MultiTcpServer/MultiTcpClientSocket.hpp). When data to be sent comes into the component, the data packet will be scanned to find the Receiver/Destination ID, and send the data through the TCP Client socket that is connected to the appropriate node. 

A TCP Client within a node may also connect to the node's TCP Server for loop back operation. 

## This Example Deployment
This Example Deployment features a Simple Producer, Simple Receiver component. and a GPS component. 


## Simple Producer
The Simple producer takes in a command to set number. Setting the number sends the same number to an output port, go through the downlink, and uplink topologies as shown below, and into a Simple Receiver Component, where it outputs a low priority event that it received a number. 

### Downlink Topology
![Downlink](img/Downlink.png)

### Uplink Topology
![Uplink](img/Uplink.png) 

## GPS
For now, Earth Centered Earth Fixed coordinate data is simulated with rand(). 

The GPS, connected with the 1Hz rate group 1 component, will poll for GPS data, and send it out through the downlink chain to a destination node. On receiving GPS data, the component will poll its own GPS position and calculate the distance to the other node using the incoming GPS data.

## Swarm Framer and Deframer
The Swarm framer frames the data producing component's data with the swarm header into a buffer, before passing the buffer into the generic hub's buffersIn ports. For now, the swarm framer is initialized with the node's device ID for the sender/source ID, while also initializing the receiver/destination ID as the same to the node's device ID for loop back. 

The Swarm Deframer removes the swarm header before passing the data to the data receiving component. 
