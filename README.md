[Japanese version](./README.ja.md)

This repository contains the code which integrates eProsima [Micro-XRCE-DDS-Client](https://github.com/eProsima/Micro-XRCE-DDS-Client) and communicates with the Micro-XRCE-DDS-Agent. Renesas RX65N is the target MCU and the code is implemented at the top of the [AWS FreeRTOS](https://github.com/renesas-rx/amazon-freertos/tree/master/demos/renesas).

# DDS-XRCE demonstration for the Renesas RX65N MCU

## Demonstration overview

This demonstration implements eProsima Micro-XRCE-DDS-Client as a DDS-XRCE implementation to RX65N MCU. It was described in the [Renesas news release](https://www.renesas.com/us/en/about/press-center/news/2018/news20181029.html) . The software can send/receive ROS 2 "std_msgs/String" to/from Micro-XRCE-DDS-Agent. It is implemented at the top of the AWS FreeRTOS and has room for other embedded applications to run.  
The below is the RX65N evaluation board line-up and GR-ROSE is used for this demonstration.  
* [GR-ROSE](http://gadget.renesas.com/en/product/rose.html) ([Purchase from Chip One Stop, Inc](https://www.chip1stop.com/USA/en/view/dispDetail/DispDetail?partId=COR1-0000004&keyword=GR-ROSE))  

![スライド1](https://user-images.githubusercontent.com/32696676/54973245-6bb10300-4fd2-11e9-843a-045367a17b18.JPG)

* Renesas RX65N Envision Kit (in mass production)  
* Renesas Starter Kit+ for RX65N (in mass production)  

Useful reference:

Renesas new release  
[Renesas Electronics RX65N Microcontrollers Support DDS-XRCE Communication Protocol for ROS 2]( https://www.renesas.com/us/en/about/press-center/news/2018/news20181029.html)  
[RX65N product homepage](https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rx/rx600/rx65n-651.html)  

### [Hardware requirement]

(1) GR-ROSE  
Target board with RX65N implemented  

(2) Linux PC  
Linux PC is to run Micro-XRCE-DDS-Agent.  

(3) Windows PC  
The PC is to write software to the GR-ROSE and monitor log outputs from the GR-ROSE by using terminal software.  

(4) Ethernet cable  
When Ethernet connection is used, connect between GR-ROSE and Linux PC via an Ethernet cable.  
When Wi-Fi connection is used, connect between Wi-Fi router and Linux PC via an Ethernet cable.  

(5) Wi-Fi router  
When Wi-Fi connection is used, connect between GR-ROSE and Linux PC via an Wi-Fi router.  

(6) USB cable  
A(male)-MicroB(male) type USB. Connects GR-ROSE and Windows PC.  

Connection diagram  

```
Ethernet connection

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
```
Wi-Fi connection

           192.168.2.101     Ethernet                           Get IP address automatically
       +-------------------+   or Wi-Fi  +--------------+ Wi-Fi +-------------------+
       |      Linux PC     +-------------+ Wi-Fi router +-------+      GR-ROSE      |
       +-------------------+             +--------------+       +-------------------+
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

(1) ROS 2 (Dashing Diademata)  
See install instructions on the web site.  
https://index.ros.org/doc/ros2/Installation/  

(2) Micro-XRCE-DDS-Agent  
Download from the web site and follow the instructions.  
https://github.com/eProsima/Micro-XRCE-DDS-Agent  

The version used for this demonstration is v1.1.0.  

#### Windows PC

Software below must be installed  

(1) e2studio
Download from the web site and follow the instructions.  
https://www.renesas.com/us/en/products/software-tools/tools/ide/e2studio.html  

The version used for this demonstration is V.7.5.0.  

(2) GCC for Renesas 4.8.4.201902-SPI1-GNURX Windows Toolchain(ELF)  
Download from the web site and follow the instructions.  
https://gcc-renesas.com/rx-download-toolchains/  

The version used for this demonstration is 4.8.4.201902.  

## Demonstration procedure

### [Get the project]

Download the project by selecting [Download ZIP] under [Clone or download] menu.  

### [Modify the sketch]

(1) Import the downloaded ZIP file by selecting [Import] under [File] menu of e2studio.  

(2) Copy tcp_talker_besteffort.ino in the examples folder to the sketch.cpp.  

(3) Switch transport setting in the project following the [Switch Ethernet/Wi-Fi] below.

### [Switch Ethernet/Wi-Fi]

In case of Wi-Fi :  
(1) To switch transport setting in demo sketch, set the variable "use_ethernet" to "false" as below.
```
rose_sketch/examples/tcp_talker_besteffort.ino

static bool use_ethernet = false;
```
(2) Set your Wi-Fi router's SSID and password in the demo sketch as below.
```
rose_sketch/examples/tcp_talker_besteffort.ino

char ssid[] = "SSID";           // your network SSID (name)
char pass[] = "PASSWORD";       // your network password  
```

In case of Ethernet:  
(1) To switch transport setting in the demo sketch, set the variable "use_ethernet" to "true" as below. 
```
rose_sketch/examples/tcp_talker_besteffort.ino

static bool use_ethernet = true;
```
### [Build the sketch]

(1) Build the project and it generates rose_sketch.bin in the ReleaseBin folder.  

### [Write the sketch to GR-ROSE]

(1) Use the USB cable to connect GR-ROSE and Window PC.   

(2) Reset the GR-ROSE and it recognizes the GR-ROSE as a USB storage.  

(3) Move the rose_sketch.bin to the GR-ROSE drive and LED brinks once programming completed.  

### [Demonstration]

(1) Run Micro-XRCE-DDS-Agent on the Linux PC and open terminal software to execute the commands as below.  

```
$ cd <build directory of Micro-XRCE-DDS-Agent>
$ ./MicroXRCEAgent tcp -p 2020 -d
```

You should see Micro-XRCE-DDS-Agent log in the terminal as below.  

```
Enter 'q' for exit
[1567490860.309055] info     | TCPServerLinux.cpp | init                     | running...             | port: 2020
[1567490860.309423] info     | DiscoveryServerLinux.cpp | init                     | running...             | Port: 7400
```

(2) Run the listener demonstration which is a part of ROS 2 package and open new terminal window to run the commands as below.  

```
$ cd <Work space directory of ROS 2>
$ source install/setup.bash
$ ros2 run demo_nodes_cpp listener
```

Run GR-ROSE in the method of removing and inserting USB cable connected to GR-ROSE.   
You should see the listener message in the terminal as below.  

```
[INFO] [listener]: I heard: [Hello DDS world!: 0]
[INFO] [listener]: I heard: [Hello DDS world!: 1]
[INFO] [listener]: I heard: [Hello DDS world!: 2]
[INFO] [listener]: I heard: [Hello DDS world!: 3]
[INFO] [listener]: I heard: [Hello DDS world!: 4]
...(SNIPPED)...
```

(3) You can also monitor the talker messages by terminal software running on PC which is connected to the GR-ROSE.  

```
...(SNIPPED)...
Found agent => ip: 192.168.2.101, port: 2020
Chosen agent => ip: 192.168.2.101, port: 2020
 Sent topic: Hello DDS world!: 0
 Sent topic: Hello DDS world!: 1
 Sent topic: Hello DDS world!: 2
 Sent topic: Hello DDS world!: 3
 Sent topic: Hello DDS world!: 4
...(SNIPPED)...
```

## Other demonstrations

You can run other demonstrations. Choose a target sketch and copy it to the sketch.cpp, and then build the project includes it.  
Below is the demo sketch directory.  

```
rose_sketch/
|
|-- ...(SNIPPED)...
|-- examples/
|   |-- tcp_talker_besteffort.ino    [Talker: TCP/best-effort stream]
|   |
|   |-- tcp_talker_reliable.ino      [Talker: TCP/reliable stream]
|   |
|   |-- tcp_listener_besteffort.ino  [Listener: TCP/best-effort stream]
|   |
|   |-- tcp_listener_reliable.ino    [Listener: TCP/reliable stream]
|   |
|   |-- udp_talker_besteffort.ino    [Talker: UDP/best-effort stream]
|   |
|   |-- udp_talker_reliable.ino      [Talker: UDP/reliable stream]
|   |
|   |-- udp_listener_besteffort.ino  [Listener: UDP/best-effort stream]
|   |
|   `-- udp_listener_reliable.ino    [Listener: UDP/reliable stream]
|
|-- sketch.cpp
|-- ...(SNIPPED)...
```

## Appendix

These demonstrations contain the following software running on the GR-ROSE. However, there are some modifications necessary to make it work. The below is the summary of the modifications.  

* Amazon FreeRTOS  
  https://github.com/renesas-rx/amazon-freertos  

* Micro-XRCE-DDS-Client v1.1.1  
  https://github.com/eProsima/Micro-XRCE-DDS-Client  

* WiFiEsp  
  https://github.com/godzilla-max/rose_sketch/tree/master/arduino/lib/WiFiEsp

```
rose_sketch/
|
|-- arduino/
|   |-- lib/
|   |   |-- WiFiEsp/
|   |   |   |-- WiFiEspServerClientWrapper.cpp   [Add code for Wi-Fi connection]
|   |   |   |-- WiFiEspServerClientWrapper.h     [Add code for Wi-Fi connection]
|   |   |   |-- ...(SNIPPED)...
|   |   |-- ...(SNIPPED)...
|   |-- ...(SNIPPED)...
|-- examples/                    [Demo sketch of Micro-XRCE-DDS-Client]
|-- FreeRTOS/
|   |-- application_code
|   |-- config_files/
|   |   |-- ...(SNIPPED)...
|   |   |-- FreeRTOSIPConfig.h   [Change mode]
|   |   |                         mode changes as below
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
|   |   |   |   |   |                           to enable discovery functionality of Micro-XRCE-DDS-Client
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
|   |           |       `-- discovery.h           [Add API for transport setting]
|   |           |   `-- transport/
|   |           |       |-- serial/
|   |           |       |-- tcp/
|   |           |       |   |-- tcp_transport_freertos.h       [Add header file for FreeRTOS]/Add API for transport setting]
|   |           |       |   |-- tcp_transport_linux.h
|   |           |       |   |-- tcp_transport_windows.h
|   |           |       |   `-- tcp_transport.h
|   |           |       `-- udp/
|   |           |           |-- udp_transport_freertos.h      [Add header file for FreeRTOS]/[Add API for transport setting]
|   |           |           |-- udp_transport_linux.h
|   |           |           |-- udp_transport_windows.h
|   |           |           `-- udp_transport.h
|   |           |-- ...(SNIPPED)...
|   |           |-- config.h                      [Add configuration file generated by config.h.in]/[Add code for Wi-Fi connection]
|   |           |-- transport.h                   [Modify setting for include files]
|   |           |-- visibility.h
|   |           |-- ...(SNIPPED)...
|   |           `-- config.h.in
|   |-- src/
|   |   `-- c/
|   |       |-- core/
|   |       |-- profile/
|   |       |   |-- discovery/
|   |       |   |   |-- transport/
|   |       |   |   |   |-- udp_transport_datagram_freertos.c               [Add code for FreeRTOS]
|   |       |   |   |   |-- udp_transport_datagram_freertos_esp8266_wifi.c  [Add code for Wi-Fi connection]
|   |       |   |   |   `-- udp_transport_datagram_internal.h               [Add code for Wi-Fi connection]
|   |       |   |   `-- discovery.c    [Add API for transport setting]
|   |       |   `-- transport/
|   |       |       |-- serial/
|   |       |       |-- tcp/
|   |       |       |   |-- tcp_transport_freertos.c               [Add code for FreeRTOS]
|   |       |       |   |-- tcp_transport_freertos_esp8266_wifi.c  [Add code for Wi-Fi connection]
|   |       |       |   |-- tcp_transport_internal.h
|   |       |       |   `-- tcp_transport.c
|   |       |       `-- udp/
|   |       |           |-- udp_transport_freertos.c               [Add code for FreeRTOS]
|   |       |           |-- udp_transport_freertos_esp8266_wifi.c  [Add code for Wi-Fi connection]
|   |       |           |-- udp_transport_internal.h
|   |       |           `-- udp_transport.c
|   |       |
|   |       `-- util/
|   |           |-- time.c   [Add code for FreeRTOS]
|   |           `-- time_internal.h
|   |-- thirdparty/
|   |   `-- microcdr/
|   |       |-- include/
|   |       |   `-- ucdr/
|   |       |       |-- ...(SNIPPED)...
|   |       |       |-- config.h              [Add configuration file generated by config.h.in]
|   |       |       |-- ...(SNIPPED)...
|   |       |       `-- config.h.in
|   |       |
|   |       |-- ...(SNIPPED)...
|   |
|   |-- ...(SNIPPED)...
|
|-- ros2_msg/                 [Add ROS 2 message ROS 2 message type "std_msgs/String"]
|   |                          it is generated by Micro-XRCE-DDS-Gen from "Ros2String.idl"
|   |-- Ros2String.c
|   |-- Ros2String.h
|   `-- Ros2String.idl
|-- sketch.cpp
|
|-- ...(SNIPPED)...

```


## Known issues/limitations

This demonstration is for Micro-XRCE-DDS-Client TCP and UDP communication and no serial communication.  
TCP/reliable stream demonstration with Wi-Fi communication is not available.  
Message default size used for the demonstration was 20 Byte and no message type other than "std_msgs/String" has been confirmed.  

## Disclaimer

We will not be liable for damages or losses arising from your use or inability to use this demonstration. Use at your own risk.  


