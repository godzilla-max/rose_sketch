[English version](./README.md)

このリポジトリは、[ルネサスMCU RX65NにインプリされるAWS FreeRTOS](https://github.com/renesas-rx/amazon-freertos/tree/master/demos/renesas)上にeProsima製の[Micro-XRCE-DDS-Client](https://github.com/eProsima/Micro-XRCE-DDS-Client)を実装し、DDS-XRCE通信を実現するものです。

# Micro-XRCE-DDS-Clientを使用したルネサスRX65NへのROS2実装デモ

## デモの概要

ルネサスのRX65Nを使用してMicro-XRCE-DDS-Clientを実装したデモです。ルネサスからニュースリリースされているデモで、ROS2の文字列用メッセージ型"std_msgs/String"の送受信行うものです。
RX65N用の評価ボードラインアップは以下ですが、ここでは、GR-ROSE上でデモを実行する手順を示します。  

GR-ROSE (株式会社コアより2019年3月に量産出荷予定)  
RX65N Envision Kit (量産中)  
Renesas Starter Kit+ for RX65N (量産中)  

参考：  
ルネサスニュースリリース  
一家に１台サービスロボットが普及する時代に向けて、RX65Nマイコンが、ロボット用フレームワーク「ROS 2」向け通信規格DDS-XRCEをサポート  
https://www.renesas.com/jp/ja/about/press-center/news/2018/news20181029.html  
RX65Nマイコンホームページ  
https://www.renesas.com/jp/ja/products/microcontrollers-microprocessors/rx/rx600/rx65n-651.html  


## デモ実行環境

デモの実行に必要な項目は以下のとおりです。  

### [ハードウェア]

(1) GR-ROSE 1枚  
作成したイメージが動作するターゲットボードです。  

(2) Linux PC  
ROS2 Bouncy Bolson、Micro-XRCE-DDS-Agentがインストールされている必要があります。  

(3) Windows PC  
GR-ROSEにスケッチを書き込むために必要です。  
また、Windows PCで起動したターミナルソフトでGR-ROSEのログ出力を行います。  

(4) Ethernetケーブル 1本  
GR-ROSEとLinux PCを接続するために必要です。  

(5) USBケーブル 1本  
A(オス) - MicroB(オス)タイプのUSBケーブルです。GR-ROSEとWindows PCを接続するために必要です。  

このデモでは、上記(1)～(5)の機器は以下のように接続します。  

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


### [ソフトウェア]

#### Linux PC

以下のソフトウェアがインストールされている必要があります。  

(1) ROS2 (Bouncy Bolson)  
インストール方法についてはWebページ上のドキュメントを参照してください。  
https://index.ros.org/doc/ros2/Installation/  

(2) Micro-XRCE-DDS-Agent  
以下からダウンロードします。インストール方法についてはWebページ上のドキュメントを参照してください。  
https://github.com/eProsima/Micro-XRCE-DDS-Agent  

動作確認を行ったバージョンは「v1.0.1」です。  

#### Windows PC

以下のソフトウェアがインストールされている必要があります。  

(1) e2studio  
以下からダウンロードします。インストール方法についてはWebページ上のドキュメントを参照してください。  
https://www.renesas.com/jp/ja/products/software-tools/tools/ide/e2studio.html  

動作確認を行ったバージョンは「V.7.0.0」です。  

(2) GCC for Renesas 4.8.4.201801-GNURX Windows Toolchain(ELF)  
以下からダウンロードします。インストール方法についてはWebページ上のドキュメントを参照してください。  
https://gcc-renesas.com/ja/rx-download-toolchains/  

動作確認を行ったバージョンは「4.8.4.201801」です。  


## デモ実行手順

### [プロジェクトの取得]

[Clone or download]から[Download ZIP]を選択してzipファイルをダウンロードしてください。  

### [スケッチのビルド]

(1) e2studioの「ファイル」メニューの「インポート」から、zipファイルをインポートしてください。  

(2) プロジェクトをビルドしてください。ReleaseBinフォルダにrose_sketch.binが生成されます。  

### [スケッチの書き込み]

(1) GR-ROSEとWindows PCをUSBケーブルで接続してください。  

(2) 接続後、GR-ROSEのリセットボタンを押してください。GR-ROSEがUSBメモリとして認識されます。  

(3) rose_sketch.binをGR-ROSEドライブにコピーしてください。GR-ROSEにスケッチが書き込まれLEDが点滅します。  

### [動作確認]

(1) Linux PC上でMicro-XRCE-DDS-Agentを起動します。Linux PCでターミナルを起動し、以下のコマンドを実行してください。  

```
$ cd <Micro-XRCE-DDS-Agentのbuildディレクトリ>
$ ./MicroXRCEAgent tcp 2020
```

Micro-XRCE-DDS-Agentのログが以下のように表示されます。  

```
TCP agent initialization... OK
Enter 'q' for exit
```

(2) Linux PC上のROS2付属デモlistenerを起動します。Linux PCで別のターミナルを起動し、以下のコマンドを実行してください。  

```
$ cd <ROS2をビルドしたワークスペース>
$ source install/setup.bash
$ ros2 run demo_nodes_cpp listener
```

Listenerのメッセージが以下のように表示されます。  

```
[INFO] [listener]: I heard: [Hello DDS world!: 0]
[INFO] [listener]: I heard: [Hello DDS world!: 1]
[INFO] [listener]: I heard: [Hello DDS world!: 2]
[INFO] [listener]: I heard: [Hello DDS world!: 3]
[INFO] [listener]: I heard: [Hello DDS world!: 4]
....(SNIPPED)....
```

(3) GR-ROSEに接続されたPCでターミナルソフトを起動すると、Talkerのメッセージが以下のように表示されます。  

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


## デモの切り替え方法

実行するデモを切り替える場合は、実行したいデモスケッチを"sketch.cpp"にコピー＆ペーストし、スケッチをビルドしてください。  
以下に、デモスケッチが配置されているディレクトリ構造を示します。  

```
rose_sketch/
|
|-- ...(SNIPPED)...
|-- examples/
|   |-- tcp_talker_besteffort.ino    [TCP/best-effortストリームで通信するTalkerデモ]
|   |
|   |-- tcp_talker_reliable.ino      [TCP/reliableストリームで通信するTalkerデモ]
|   |
|   |-- tcp_listener_besteffort.ino  [TCP/best-effortストリームで通信するListenerデモ]
|   |
|   |-- tcp_listener_reliable.ino    [TCP/reliableストリームで通信するListenerデモ]
|   |
|   |-- udp_talker_besteffort.ino    [UDP/best-effortストリームで通信するTalkerデモ]
|   |
|   |-- udp_talker_reliable.ino      [UDP/reliableストリームで通信するTalkerデモ]
|   |
|   |-- udp_listener_besteffort.ino  [UDP/best-effortストリームで通信するListenerデモ]
|   |
|   `-- udp_listener_reliable.ino    [UDP/reliableストリームで通信するListenerデモ]
|
|-- sketch.cpp
|-- ...(SNIPPED)...
```


## Appendix

### プロジェクトについて

このデモは、GR-ROSE上で動作させるプロジェクトに以下のソフトウェアを使用しています。  

* Amazon FreeRTOS  
  https://github.com/renesas-rx/amazon-freertos  

* Micro-XRCE-DDS-Client v1.0.1  
  https://github.com/eProsima/Micro-XRCE-DDS-Client  

なお、GR-ROSE上のAmazon FreeRTOSでMicro-XRCE-DDS-Clientを動作させるため、Micro-XRCE-DDS-ClientとAmazon FreeRTOSに以下の変更を加えています。  

```
rose_sketch/
|
|-- arduino/
|-- examples/                    [Micro-XRCE-DDS-Clientのデモスケッチ]
|-- FreeRTOS/
|   |-- application_code
|   |-- config_files/
|   |   |-- ...(SNIPPED)...
|   |   |-- FreeRTOSIPConfig.h   [設定を変更]
|   |   |                         以下のように設定を変更しました。
|   |   |                          ・ DHCPサーバを使用しない。
|   |   |                          ・ FreeRTOSのAPI FreeRTOS_inet_addr()を使用する。
|   |   |-- ...(SNIPPED)...
|   |
|   |-- lib/
|   |   |-- aws/
|   |   |   |-- ...(SNIPPED)...
|   |   |   |-- FreeRTOS-Plus-TCP/
|   |   |   |   |-- include/
|   |   |   |   |   |-- ...(SNIPPED)...
|   |   |   |   |   |-- FreeRTOS_IP_Private.h  [ブロードキャストアドレスを変更]
|   |   |   |   |   |                           Micro-XRCE-DDS-ClientのDiscovery(Multicast)機能に対応するため、
|   |   |   |   |   |                           ブロードキャストアドレスをDDSマルチキャストアドレスに変更しました。
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
|   |           |       |   |-- tcp_transport_freertos.h       [FreeRTOS依存のヘッダファイルを追加]
|   |           |       |   |-- tcp_transport_linux.h
|   |           |       |   |-- tcp_transport_windows.h
|   |           |       |   `-- tcp_transport.h
|   |           |       `-- udp/
|   |           |           |-- udp_transport_freertos.h       [FreeRTOS依存のヘッダファイルを追加]
|   |           |           |-- udp_transport_linux.h
|   |           |           |-- udp_transport_windows.h
|   |           |           `-- udp_transport.h
|   |           |-- ...(SNIPPED)...
|   |           |-- config.h                      [config.h.inから生成される設定ファイルを追加]
|   |           |-- dll.h
|   |           |-- transport.h                   [インクルードファイルの設定を変更]
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
|   |       |   |   |   `-- udp_transport_freertos_datagram.c    [FreeRTOS依存のコードを追加]
|   |       |   |   `-- discovery.c
|   |       |   |-- session/
|   |       |   `-- transport/
|   |       |       |-- serial/
|   |       |       |-- tcp/
|   |       |       |   |-- tcp_transport_freertos.c    [FreeRTOS依存のコードを追加]
|   |       |       |   |-- tcp_transport_internal.h
|   |       |       |   |-- tcp_transport_linux.c
|   |       |       |   |-- tcp_transport_windows.c
|   |       |       |   `-- tcp_transport.c
|   |       |       `-- udp/
|   |       |           |-- udp_transport_freertos.c    [FreeRTOS依存のコードを追加]
|   |       |           |-- udp_transport_internal.h
|   |       |           |-- udp_transport_linux.c
|   |       |           |-- udp_transport_windows.c
|   |       |           `-- udp_transport.c
|   |       |
|   |       `-- util/
|   |           `-- time.c   [FreeRTOS依存のコードを追加]
|   |-- thirdparty/
|   |   `-- microcdr/
|   |       |-- include/
|   |       |   `-- ucdr/
|   |       |       |-- ...(SNIPPED)...
|   |       |       |-- config.h              [config.h.inから生成される設定ファイルを追加]
|   |       |       |-- ...(SNIPPED)...
|   |       |       `-- config.h.in
|   |       |
|   |       |-- ...(SNIPPED)...
|   |
|   |-- ...(SNIPPED)...
|
|-- ros2_msg/                 [ROS2のメッセージ"std_msgs/String"型を追加]
|   |                          Micro-XRCE-DDS-Genが"Ros2String.idl"から生成したファイルです。
|   |-- Ros2String.c
|   |-- Ros2String.h
|   `-- Ros2String.idl
|-- sketch.cpp
|
|-- ...(SNIPPED)...

```


## 制限事項

* このデモは、Micro-XRCE-DDS-ClientのTCP/UDP通信に対応していますが、シリアル通信には対応していません。  

* GR-ROSEからROS2メッセージを送信する際に、受信動作を同時に行わないMicro-XRCE-DDS-ClientのAPI uxr_flash_output_streams()は使用できません。  

* Hello worldデモで確認したメッセージのサイズはデフォルト20バイトで、ROS2の文字列用メッセージ型"std_msgs/String"以外の動作は未確認です。  


## 免責事項

当デモのご利用、もしくはご利用になれないことにより発生したいかなる損害や不利益について、当方は一切の責任を負いません。自己責任の上でご使用ください。  


