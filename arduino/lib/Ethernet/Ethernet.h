#ifndef ethernet_h
#define ethernet_h

#include <inttypes.h>
//#include "w5100.h"
#include "IPAddress.h"
#ifndef GRROSE
#include "EthernetClient.h"
#include "EthernetServer.h"
#include "Dhcp.h"

#define MAX_SOCK_NUM 4
#endif
class EthernetClass {
private:
  IPAddress _dnsServerAddress;
#ifndef GRROSE
  DhcpClass* _dhcp;
#endif
public:
#ifndef GRROSE
  static uint8_t _state[MAX_SOCK_NUM];
  static uint16_t _server_port[MAX_SOCK_NUM];
#endif
  // Initialise the Ethernet shield to use the provided MAC address and gain the rest of the
  // configuration through DHCP.
  // Returns 0 if the DHCP configuration failed, and 1 if it succeeded
  int begin(uint8_t *mac_address, unsigned long timeout = 60000, unsigned long responseTimeout = 4000);
  void begin(uint8_t *mac_address, IPAddress local_ip);
  void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server);
  void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway);
  void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);
  int maintain();

  IPAddress localIP();
  IPAddress subnetMask();
  IPAddress gatewayIP();
  IPAddress dnsServerIP();

#ifndef GRROSE
  friend class EthernetClient;
  friend class EthernetServer;
#endif
};

extern EthernetClass Ethernet;

#endif
