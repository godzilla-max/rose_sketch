[Japanese version](./README.ja.md)

This repository contains the code which integrates eProsima [Micro-XRCE-DDS Client](https://github.com/eProsima/Micro-XRCE-DDS-Client) and communicates with the Micro-XRCE-DDS Agent. Renesas RX65N is the target MCU and the code is implemented at the top of the [AWS FreeRTOS](https://github.com/renesas-rx/amazon-freertos/tree/master/demos/renesas).

# DDS-XRCE demonstration for the Renesas RX65N MCU

## Demonstration overview

This demonstration implements eProsima Micro-XRCE-DDS Client as a DDS-XRCE implementation to RX65N MCU. It was described in the [Renesas news release](https://www.renesas.com/us/en/about/press-center/news/2018/news20181029.html) . The software can send/receive ROS2 "std_msgs/String" to/from Micro-XRCE-DDS Agent. It is implemented at the top of the AWS FreeRTOS and has room for other embedded applications to run.  
The below is the RX65N evaluation board line-up and GR-ROSE was used for this demonstration.  
GR-ROSE (Will be available in March/2019 from Core Corporation)  
Renesas RX65N Envision Kit (in mass production)  
Renesas Starter Kit+ for RX65N (in mass production)  

Useful reference:

Renesas new release  
[Renesas Electronics RX65N Microcontrollers Support DDS-XRCE Communication Protocol for ROS 2]( https://www.renesas.com/us/en/about/press-center/news/2018/news20181029.html)  
[RX65N product homepage](https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rx/rx600/rx65n-651.html)  

### [Hardware requirement]

(1) GR-ROSE  
Target board with RX65N implemented  

(2) Linux PC  
Linux PC is to run Micro-XRCE-DDS Agent.  

(3) Windows PC  
The PC is to write software to the GR-ROSE and monitor log outputs from the GR-ROSE by using terminal software.  

(4) Ethernet cable  
Patch between GR-ROSE and Linux PC  

(5) USB cable  
A(male)-MicroB(male) type USB. Connects GR-ROSE and Windows PC.  

Connection diagram  

```

           192.168.2.101                                192.168.2.52
       +-------------------+        Ethernet       +-------------------+
       |      Linux PC     +-----------------------+      GR-ROSE      |
       +-------------------+                       +-------------------+
                                                             |
                                                             | USB
                                                             |
                                                   +---------+---------+
                                                   |     Windows PC    |
                                                   +-------------------+ 

```


### [Software]

#### Linux PC

Software below must be installed.  

(1) ROS2 (Bouncy Bolson)  
See install instructions on the web site.  
https://index.ros.org/doc/ros2/Installation/  

(2) Micro-XRCE-DDS Agent  
Download from the web site and follow the instructions.  
https://github.com/eProsima/Micro-XRCE-DDS-Agent  

The version used for this demonstration was v1.0.1.  

#### Windows PC

Software below must be installed  

(1) e2studio
Download from the web site and follow the instructions.  
https://www.renesas.com/us/en/products/software-tools/tools/ide/e2studio.html  

The version used for this demonstration was V.7.0.0.  

(2) GCC for Renesas 4.8.4.201801-GNURX Windows Toolchain(ELF)  
Download from the web site and follow the instructions.  
https://gcc-renesas.com/rx-download-toolchains/  

The version used for this demonstration was 4.8.4.201801.  

## Demonstration procedure

### [Get the project]

Download the project by selecting [Download ZIP] under [Clone or download] menu.  

### [Build the sketch]

(1) Import the downloaded ZIP file by selecting [Import] under [File] menu of e2studio.  

(2) Build the project and it generates rose_sketch.bin in ReleaseBin folder.  

### [Write the sketch to GR-ROSE]

(1) Use the USB cable to connect GR-ROSE and Window PC.   

(2) Reset the GR-ROSE and it recognizes the GR-ROSE as a USB storage.  

(3) Move the rose_sketch.bin to the GR-ROSE drive and LED brinks once programming completed.  

### [Demonstration]

(1) Run Micro-XRCE-DDS Agent on the Linux PC and open terminal software to execute the commands as below;  

```
$ cd <build directory of Micro-XRCE-DDS Agent>
$ ./MicroXRCEAgent tcp 2020
```

You should see Micro-XRCE-DDS Agent log in the terminal as below;  

```
TCP agent initialization... OK
Enter 'q' for exit
```

(2) Run the listener demo which is a part of ROS2 package and open new terminal window to run the commands as below;  

```
$ cd <Work space directory of ROS2>
$ source install/setup.bash
$ ros2 run demo_nodes_cpp listener
```

You should see the listener message in the terminal as below.  

```
[INFO] [listener]: I heard: [Hello DDS world!: 0]
[INFO] [listener]: I heard: [Hello DDS world!: 1]
[INFO] [listener]: I heard: [Hello DDS world!: 2]
[INFO] [listener]: I heard: [Hello DDS world!: 3]
[INFO] [listener]: I heard: [Hello DDS world!: 4]
....(SNIPPED)....
```

(3) You can also monitor the Talker messages by terminal software running on PC which is connected to the GR-ROSE.  

```
....(SNIPPED)....
Found agent => ip: 192.168.2.101, port: 2020
Chosen agent => ip: 192.168.2.101, port: 2020
 Sent topic: Hello DDS world!: 0
 Sent topic: Hello DDS world!: 1
 Sent topic: Hello DDS world!: 2
 Sent topic: Hello DDS world!: 3
 Sent topic: Hello DDS world!: 4
....(SNIPPED)....
```

## Other demonstrations

You can run other demonstrations. Demo sketch must be copy and past to the sletch.cpp and build the sketch.  
The below is the demo sketch directory.  

```
rose_sketch/
|
|-- ...(SNIPPED)...
|-- examples/
|   |-- tcp_talker_besteffort.ino    [Talker: TCP/best-effort stream]
|   |
|   |-- tcp_talker_reliable.ino      [Talker: TCP/reliable stream]
|   |
|   |-- tcp_listener_besteffort.ino  [Listner: TCP/best-effort stream]
|   |
|   |-- tcp_listener_reliable.ino    [Listner: TCP/reliable stream]
|   |
|   |-- udp_talker_besteffort.ino    [Talker: UDP/best-effort stream]
|   |
|   |-- udp_talker_reliable.ino      [Talker: UDP/reliable stream]
|   |
|   |-- udp_listener_besteffort.ino  [Listner: UDP/best-effor steam]
|   |
|   `-- udp_listener_reliable.ino    [Listner: UDP/reliable stream]
|
|-- sketch.cpp
|-- ...(SNIPPED)...
```

## Appendix

This demo contains the following software running on the GR-ROSE. However, there are some modifications necessary to make it work. The below is the summary of the changes.  

* Amazon FreeRTOS  
  https://github.com/renesas-rx/amazon-freertos  

* Micro-XRCE-DDS Client v1.0.1  
  https://github.com/eProsima/Micro-XRCE-DDS-Client  

```
rose_sketch/
|
|-- arduino/
|-- examples/                    [Demo sketch of Micro-XRCE-DDS Client]
|-- FreeRTOS/
|   |-- application_code
|   |-- config_files/
|   |   |-- ...(SNIPPED)...
|   |   |-- FreeRTOSIPConfig.h   [Change mode]
|   |   |                         mode canges as below;
|   |   |                          * Don't use DHCP server
|   |   |                          * Use FreeRTOS API (FreeRTOS_inet_addr())
|   |   |-- ...(SNIPPED)...
|   |
|   |-- lib/
|   |   |-- aws/
|   |   |   |-- ...(SNIPPED)...
|   |   |   |-- FreeRTOS-Plus-TCP/
|   |   |   |   |-- include/
|   |   |   |   |   |-- ...(SNIPPED)...
|   |   |   |   |   |-- FreeRTOS_IP_Private.h  [Change broadcast address]
|   |   |   |   |   |                           Change to DDS multicast address as broadcast address
|   |   |   |   |   |                           to enable discovery functionality of Micro-XRCE-DDS Client
|   |   |   |   |   |-- ...(SNIPPED)...
|   |   |   |   |
|   |   |   |   `-- source/
|   |   |   |-- ...(SNIPPED)...
|   |   |
|   |   `-- third_party/
|   `-- src/
|
|-- Micro-XRCE-DDS-Client/
|   |
|   |-- include/
|   |   `-- uxr/
|   |       `-- client/
|   |           |-- core/
|   |           |-- profile/
|   |           |   |-- discovery/
|   |           |   |-- session/
|   |           |   `-- transport/
|   |           |       |-- serial/
|   |           |       |-- tcp/
|   |           |       |   |-- tcp_transport_freertos.h       [Add header file for FreeRTOS]
|   |           |       |   |-- tcp_transport_linux.h
|   |           |       |   |-- tcp_transport_windows.h
|   |           |       |   `-- tcp_transport.h
|   |           |       `-- udp/
|   |           |           |-- udp_transport_freertos.h      [Add header file for FreeRTOS]
|   |           |           |-- udp_transport_linux.h
|   |           |           |-- udp_transport_windows.h
|   |           |           `-- udp_transport.h
|   |           |-- ...(SNIPPED)...
|   |           |-- config.h                      [add configuration file generated by config.h.in]
|   |           |-- dll.h
|   |           |-- transport.h                   [change setting for include files]
|   |           |-- ...(SNIPPED)...
|   |           `-- config.h.in
|   |-- src/
|   |   `-- c/
|   |       |-- core/
|   |       |-- profile/
|   |       |   |-- discovery/
|   |       |   |   |-- transport/
|   |       |   |   |   |-- udp_transport_datagram_internal.h
|   |       |   |   |   |-- udp_transport_linux_datagram.c
|   |       |   |   |   `-- udp_transport_freertos_datagram.c    [add code for FreeRTOS]
|   |       |   |   `-- discovery.c
|   |       |   |-- session/
|   |       |   `-- transport/
|   |       |       |-- serial/
|   |       |       |-- tcp/
|   |       |       |   |-- tcp_transport_freertos.c    [add code for FreeRTOS]
|   |       |       |   |-- tcp_transport_internal.h
|   |       |       |   |-- tcp_transport_linux.c
|   |       |       |   |-- tcp_transport_windows.c
|   |       |       |   `-- tcp_transport.c
|   |       |       `-- udp/
|   |       |           |-- udp_transport_freertos.c    [add code for FreeRTOS]
|   |       |           |-- udp_transport_internal.h
|   |       |           |-- udp_transport_linux.c
|   |       |           |-- udp_transport_windows.c
|   |       |           `-- udp_transport.c
|   |       |
|   |       `-- util/
|   |           `-- time.c   [add code for FreeRTOS]
|   |-- thirdparty/
|   |   `-- microcdr/
|   |       |-- include/
|   |       |   `-- ucdr/
|   |       |       |-- ...(SNIPPED)...
|   |       |       |-- config.h              [add configuration file generated by config.h.in]
|   |       |       |-- ...(SNIPPED)...
|   |       |       `-- config.h.in
|   |       |
|   |       |-- ...(SNIPPED)...
|   |
|   |-- ...(SNIPPED)...
|
|-- ros2_msg/                 [add ROS2 message ROS2 message type "std_msgs/String"]
|   |                          it is generated by Micro-XRCE-DDS-Gen from "Ros2String.idl"
|   |-- Ros2String.c
|   |-- Ros2String.h
|   `-- Ros2String.idl
|-- sketch.cpp
|
|-- ...(SNIPPED)...

```


## Known issues/limitations

This demonstration is for Micro-XRCE-DDS Client TCP and UDP communication and no serial communication.  
This demonstration does not support "uxr_flash_output_streams()" API of the Micro-XRCE-DDS Client which does not receive a message when transmits a message.  
Message default size used for the demonstration was 20 Byte and no message type other than "std_msgs/String" has been confirmed.  

## Disclaimer

We will not be liable for damages or losses arising from your use or inability to use this demonstration. Use at your own risk.  


