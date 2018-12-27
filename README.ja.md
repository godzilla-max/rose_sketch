[English version](./README.md)

���̃��|�W�g���́A[���l�T�XMCU RX65N�ɃC���v�������AWS FreeRTOS](https://github.com/renesas-rx/amazon-freertos/tree/master/demos/renesas)���eProsima����[Micro-XRCE-DDS-Client](https://github.com/eProsima/Micro-XRCE-DDS-Client)���������ADDS-XRCE�ʐM������������̂ł��B

# Micro-XRCE-DDS-Client���g�p�������l�T�XRX65N�ւ�ROS2�����f��

## �f���̊T�v

���l�T�X��RX65N���g�p����Micro-XRCE-DDS-Client�����������f���ł��B���l�T�X����j���[�X�����[�X����Ă���f���ŁAROS2�̕�����p���b�Z�[�W�^"std_msgs/String"�̑���M�s�����̂ł��B
RX65N�p�̕]���{�[�h���C���A�b�v�͈ȉ��ł����A�����ł́AGR-ROSE��Ńf�������s����菇�������܂��B  

GR-ROSE (������ЃR�A���2019�N3���ɗʎY�o�ח\��)  
RX65N Envision Kit (�ʎY��)  
Renesas Starter Kit+ for RX65N (�ʎY��)  

�Q�l�F  
���l�T�X�j���[�X�����[�X  
��ƂɂP��T�[�r�X���{�b�g�����y���鎞��Ɍ����āARX65N�}�C�R�����A���{�b�g�p�t���[�����[�N�uROS 2�v�����ʐM�K�iDDS-XRCE���T�|�[�g  
https://www.renesas.com/jp/ja/about/press-center/news/2018/news20181029.html  
RX65N�}�C�R���z�[���y�[�W  
https://www.renesas.com/jp/ja/products/microcontrollers-microprocessors/rx/rx600/rx65n-651.html  


## �f�����s��

�f���̎��s�ɕK�v�ȍ��ڂ͈ȉ��̂Ƃ���ł��B  

### [�n�[�h�E�F�A]

(1) GR-ROSE 1��  
�쐬�����C���[�W�����삷��^�[�Q�b�g�{�[�h�ł��B  

(2) Linux PC  
ROS2 Bouncy Bolson�AMicro-XRCE-DDS-Agent���C���X�g�[������Ă���K�v������܂��B  

(3) Windows PC  
GR-ROSE�ɃX�P�b�`���������ނ��߂ɕK�v�ł��B  
�܂��AWindows PC�ŋN�������^�[�~�i���\�t�g��GR-ROSE�̃��O�o�͂��s���܂��B  

(4) Ethernet�P�[�u�� 1�{  
GR-ROSE��Linux PC��ڑ����邽�߂ɕK�v�ł��B  

(5) USB�P�[�u�� 1�{  
A(�I�X) - MicroB(�I�X)�^�C�v��USB�P�[�u���ł��BGR-ROSE��Windows PC��ڑ����邽�߂ɕK�v�ł��B  

���̃f���ł́A��L(1)�`(5)�̋@��͈ȉ��̂悤�ɐڑ����܂��B  

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


### [�\�t�g�E�F�A]

#### Linux PC

�ȉ��̃\�t�g�E�F�A���C���X�g�[������Ă���K�v������܂��B  

(1) ROS2 (Bouncy Bolson)  
�C���X�g�[�����@�ɂ��Ă�Web�y�[�W��̃h�L�������g���Q�Ƃ��Ă��������B  
https://index.ros.org/doc/ros2/Installation/  

(2) Micro-XRCE-DDS-Agent  
�ȉ�����_�E�����[�h���܂��B�C���X�g�[�����@�ɂ��Ă�Web�y�[�W��̃h�L�������g���Q�Ƃ��Ă��������B  
https://github.com/eProsima/Micro-XRCE-DDS-Agent  

����m�F���s�����o�[�W�����́uv1.0.1�v�ł��B  

#### Windows PC

�ȉ��̃\�t�g�E�F�A���C���X�g�[������Ă���K�v������܂��B  

(1) e2studio  
�ȉ�����_�E�����[�h���܂��B�C���X�g�[�����@�ɂ��Ă�Web�y�[�W��̃h�L�������g���Q�Ƃ��Ă��������B  
https://www.renesas.com/jp/ja/products/software-tools/tools/ide/e2studio.html  

����m�F���s�����o�[�W�����́uV.7.0.0�v�ł��B  

(2) GCC for Renesas 4.8.4.201801-GNURX Windows Toolchain(ELF)  
�ȉ�����_�E�����[�h���܂��B�C���X�g�[�����@�ɂ��Ă�Web�y�[�W��̃h�L�������g���Q�Ƃ��Ă��������B  
https://gcc-renesas.com/ja/rx-download-toolchains/  

����m�F���s�����o�[�W�����́u4.8.4.201801�v�ł��B  


## �f�����s�菇

### [�v���W�F�N�g�̎擾]

[Clone or download]����[Download ZIP]��I������zip�t�@�C�����_�E�����[�h���Ă��������B  

### [�X�P�b�`�̃r���h]

(1) e2studio�́u�t�@�C���v���j���[�́u�C���|�[�g�v����Azip�t�@�C�����C���|�[�g���Ă��������B  

(2) �v���W�F�N�g���r���h���Ă��������BReleaseBin�t�H���_��rose_sketch.bin����������܂��B  

### [�X�P�b�`�̏�������]

(1) GR-ROSE��Windows PC��USB�P�[�u���Őڑ����Ă��������B  

(2) �ڑ���AGR-ROSE�̃��Z�b�g�{�^���������Ă��������BGR-ROSE��USB�������Ƃ��ĔF������܂��B  

(3) rose_sketch.bin��GR-ROSE�h���C�u�ɃR�s�[���Ă��������BGR-ROSE�ɃX�P�b�`���������܂�LED���_�ł��܂��B  

### [����m�F]

(1) Linux PC���Micro-XRCE-DDS-Agent���N�����܂��BLinux PC�Ń^�[�~�i�����N�����A�ȉ��̃R�}���h�����s���Ă��������B  

```
$ cd <Micro-XRCE-DDS-Agent��build�f�B���N�g��>
$ ./MicroXRCEAgent tcp 2020
```

Micro-XRCE-DDS-Agent�̃��O���ȉ��̂悤�ɕ\������܂��B  

```
TCP agent initialization... OK
Enter 'q' for exit
```

(2) Linux PC���ROS2�t���f��listener���N�����܂��BLinux PC�ŕʂ̃^�[�~�i�����N�����A�ȉ��̃R�}���h�����s���Ă��������B  

```
$ cd <ROS2���r���h�������[�N�X�y�[�X>
$ source install/setup.bash
$ ros2 run demo_nodes_cpp listener
```

Listener�̃��b�Z�[�W���ȉ��̂悤�ɕ\������܂��B  

```
[INFO] [listener]: I heard: [Hello DDS world!: 0]
[INFO] [listener]: I heard: [Hello DDS world!: 1]
[INFO] [listener]: I heard: [Hello DDS world!: 2]
[INFO] [listener]: I heard: [Hello DDS world!: 3]
[INFO] [listener]: I heard: [Hello DDS world!: 4]
....(SNIPPED)....
```

(3) GR-ROSE�ɐڑ����ꂽPC�Ń^�[�~�i���\�t�g���N������ƁATalker�̃��b�Z�[�W���ȉ��̂悤�ɕ\������܂��B  

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


## �f���̐؂�ւ����@

���s����f����؂�ւ���ꍇ�́A���s�������f���X�P�b�`��"sketch.cpp"�ɃR�s�[���y�[�X�g���A�X�P�b�`���r���h���Ă��������B  
�ȉ��ɁA�f���X�P�b�`���z�u����Ă���f�B���N�g���\���������܂��B  

```
rose_sketch/
|
|-- ...(SNIPPED)...
|-- examples/
|   |-- tcp_talker_besteffort.ino    [TCP/best-effort�X�g���[���ŒʐM����Talker�f��]
|   |
|   |-- tcp_talker_reliable.ino      [TCP/reliable�X�g���[���ŒʐM����Talker�f��]
|   |
|   |-- tcp_listener_besteffort.ino  [TCP/best-effort�X�g���[���ŒʐM����Listener�f��]
|   |
|   |-- tcp_listener_reliable.ino    [TCP/reliable�X�g���[���ŒʐM����Listener�f��]
|   |
|   |-- udp_talker_besteffort.ino    [UDP/best-effort�X�g���[���ŒʐM����Talker�f��]
|   |
|   |-- udp_talker_reliable.ino      [UDP/reliable�X�g���[���ŒʐM����Talker�f��]
|   |
|   |-- udp_listener_besteffort.ino  [UDP/best-effort�X�g���[���ŒʐM����Listener�f��]
|   |
|   `-- udp_listener_reliable.ino    [UDP/reliable�X�g���[���ŒʐM����Listener�f��]
|
|-- sketch.cpp
|-- ...(SNIPPED)...
```


## Appendix

### �v���W�F�N�g�ɂ���

���̃f���́AGR-ROSE��œ��삳����v���W�F�N�g�Ɉȉ��̃\�t�g�E�F�A���g�p���Ă��܂��B  

* Amazon FreeRTOS  
  https://github.com/renesas-rx/amazon-freertos  

* Micro-XRCE-DDS-Client v1.0.1  
  https://github.com/eProsima/Micro-XRCE-DDS-Client  

�Ȃ��AGR-ROSE���Amazon FreeRTOS��Micro-XRCE-DDS-Client�𓮍삳���邽�߁AMicro-XRCE-DDS-Client��Amazon FreeRTOS�Ɉȉ��̕ύX�������Ă��܂��B  

```
rose_sketch/
|
|-- arduino/
|-- examples/                    [Micro-XRCE-DDS-Client�̃f���X�P�b�`]
|-- FreeRTOS/
|   |-- application_code
|   |-- config_files/
|   |   |-- ...(SNIPPED)...
|   |   |-- FreeRTOSIPConfig.h   [�ݒ��ύX]
|   |   |                         �ȉ��̂悤�ɐݒ��ύX���܂����B
|   |   |                          �E DHCP�T�[�o���g�p���Ȃ��B
|   |   |                          �E FreeRTOS��API FreeRTOS_inet_addr()���g�p����B
|   |   |-- ...(SNIPPED)...
|   |
|   |-- lib/
|   |   |-- aws/
|   |   |   |-- ...(SNIPPED)...
|   |   |   |-- FreeRTOS-Plus-TCP/
|   |   |   |   |-- include/
|   |   |   |   |   |-- ...(SNIPPED)...
|   |   |   |   |   |-- FreeRTOS_IP_Private.h  [�u���[�h�L���X�g�A�h���X��ύX]
|   |   |   |   |   |                           Micro-XRCE-DDS-Client��Discovery(Multicast)�@�\�ɑΉ����邽�߁A
|   |   |   |   |   |                           �u���[�h�L���X�g�A�h���X��DDS�}���`�L���X�g�A�h���X�ɕύX���܂����B
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
|   |           |       |   |-- tcp_transport_freertos.h       [FreeRTOS�ˑ��̃w�b�_�t�@�C����ǉ�]
|   |           |       |   |-- tcp_transport_linux.h
|   |           |       |   |-- tcp_transport_windows.h
|   |           |       |   `-- tcp_transport.h
|   |           |       `-- udp/
|   |           |           |-- udp_transport_freertos.h       [FreeRTOS�ˑ��̃w�b�_�t�@�C����ǉ�]
|   |           |           |-- udp_transport_linux.h
|   |           |           |-- udp_transport_windows.h
|   |           |           `-- udp_transport.h
|   |           |-- ...(SNIPPED)...
|   |           |-- config.h                      [config.h.in���琶�������ݒ�t�@�C����ǉ�]
|   |           |-- dll.h
|   |           |-- transport.h                   [�C���N���[�h�t�@�C���̐ݒ��ύX]
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
|   |       |   |   |   `-- udp_transport_freertos_datagram.c    [FreeRTOS�ˑ��̃R�[�h��ǉ�]
|   |       |   |   `-- discovery.c
|   |       |   |-- session/
|   |       |   `-- transport/
|   |       |       |-- serial/
|   |       |       |-- tcp/
|   |       |       |   |-- tcp_transport_freertos.c    [FreeRTOS�ˑ��̃R�[�h��ǉ�]
|   |       |       |   |-- tcp_transport_internal.h
|   |       |       |   |-- tcp_transport_linux.c
|   |       |       |   |-- tcp_transport_windows.c
|   |       |       |   `-- tcp_transport.c
|   |       |       `-- udp/
|   |       |           |-- udp_transport_freertos.c    [FreeRTOS�ˑ��̃R�[�h��ǉ�]
|   |       |           |-- udp_transport_internal.h
|   |       |           |-- udp_transport_linux.c
|   |       |           |-- udp_transport_windows.c
|   |       |           `-- udp_transport.c
|   |       |
|   |       `-- util/
|   |           `-- time.c   [FreeRTOS�ˑ��̃R�[�h��ǉ�]
|   |-- thirdparty/
|   |   `-- microcdr/
|   |       |-- include/
|   |       |   `-- ucdr/
|   |       |       |-- ...(SNIPPED)...
|   |       |       |-- config.h              [config.h.in���琶�������ݒ�t�@�C����ǉ�]
|   |       |       |-- ...(SNIPPED)...
|   |       |       `-- config.h.in
|   |       |
|   |       |-- ...(SNIPPED)...
|   |
|   |-- ...(SNIPPED)...
|
|-- ros2_msg/                 [ROS2�̃��b�Z�[�W"std_msgs/String"�^��ǉ�]
|   |                          Micro-XRCE-DDS-Gen��"Ros2String.idl"���琶�������t�@�C���ł��B
|   |-- Ros2String.c
|   |-- Ros2String.h
|   `-- Ros2String.idl
|-- sketch.cpp
|
|-- ...(SNIPPED)...

```


## ��������

* ���̃f���́AMicro-XRCE-DDS-Client��TCP/UDP�ʐM�ɑΉ����Ă��܂����A�V���A���ʐM�ɂ͑Ή����Ă��܂���B  

* GR-ROSE����ROS2���b�Z�[�W�𑗐M����ۂɁA��M����𓯎��ɍs��Ȃ�Micro-XRCE-DDS-Client��API uxr_flash_output_streams()�͎g�p�ł��܂���B  

* Hello world�f���Ŋm�F�������b�Z�[�W�̃T�C�Y�̓f�t�H���g20�o�C�g�ŁAROS2�̕�����p���b�Z�[�W�^"std_msgs/String"�ȊO�̓���͖��m�F�ł��B  


## �Ɛӎ���

���f���̂����p�A�������͂����p�ɂȂ�Ȃ����Ƃɂ�蔭�����������Ȃ鑹�Q��s���v�ɂ��āA�����͈�؂̐ӔC�𕉂��܂���B���ȐӔC�̏�ł��g�p���������B  


