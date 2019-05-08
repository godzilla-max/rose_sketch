#ifndef GRROSE
#include "utility/w5100.h"
#include "Ethernet.h"
#include "Dhcp.h"
#else
#include "Ethernet.h"
extern "C" {
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "aws_system_init.h"
}
#endif

#ifndef GRROSE
// XXX: don't make assumptions about the value of MAX_SOCK_NUM.
uint8_t EthernetClass::_state[MAX_SOCK_NUM] = { 
  0, 0, 0, 0 };
uint16_t EthernetClass::_server_port[MAX_SOCK_NUM] = { 
  0, 0, 0, 0 };
#endif

#ifdef GRROSE
static uint8_t ucMACAddress[ 6 ] =
{
    configMAC_ADDR0,
    configMAC_ADDR1,
    configMAC_ADDR2,
    configMAC_ADDR3,
    configMAC_ADDR4,
    configMAC_ADDR5
}; //XXX
static const uint8_t ucIPAddress[ 4 ] =
{
    configIP_ADDR0,
    configIP_ADDR1,
    configIP_ADDR2,
    configIP_ADDR3
};
static const uint8_t ucNetMask[ 4 ] =
{
    configNET_MASK0,
    configNET_MASK1,
    configNET_MASK2,
    configNET_MASK3
};
static const uint8_t ucGatewayAddress[ 4 ] =
{
    configGATEWAY_ADDR0,
    configGATEWAY_ADDR1,
    configGATEWAY_ADDR2,
    configGATEWAY_ADDR3
};

/* The following is the address of an OpenDNS server. */
static const uint8_t ucDNSServerAddress[ 4 ] =
{
    configDNS_SERVER_ADDR0,
    configDNS_SERVER_ADDR1,
    configDNS_SERVER_ADDR2,
    configDNS_SERVER_ADDR3
};
#endif

int EthernetClass::begin(uint8_t *mac_address, unsigned long timeout, unsigned long responseTimeout)
{
#ifndef GRROSE
  static DhcpClass s_dhcp;
  _dhcp = &s_dhcp;


  // Initialise the basic info
  W5100.init();
  SPI.beginTransaction(SPI_ETHERNET_SETTINGS);
  W5100.setMACAddress(mac_address);
  W5100.setIPAddress(IPAddress(0,0,0,0).raw_address());
  SPI.endTransaction();

  // Now try to get our config info from a DHCP server
  int ret = _dhcp->beginWithDHCP(mac_address, timeout, responseTimeout);
  if(ret == 1)
  {
    // We've successfully found a DHCP server and got our configuration info, so set things
    // accordingly
    SPI.beginTransaction(SPI_ETHERNET_SETTINGS);
    W5100.setIPAddress(_dhcp->getLocalIp().raw_address());
    W5100.setGatewayIp(_dhcp->getGatewayIp().raw_address());
    W5100.setSubnetMask(_dhcp->getSubnetMask().raw_address());
    SPI.endTransaction();
    _dnsServerAddress = _dhcp->getDnsServerIp();
  }

  return ret;
#else
  // no implementation, just for setting IP.
  return false;
#endif
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip)
{
  // Assume the DNS server will be the machine on the same network as the local IP
  // but with last octet being '1'
#ifndef GRROSE
  IPAddress dns_server = local_ip;
  dns_server[3] = 1;
  begin(mac_address, local_ip, dns_server);
#else
  begin(mac_address, local_ip, ucDNSServerAddress);
#endif
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server)
{
  // Assume the gateway will be the machine on the same network as the local IP
  // but with last octet being '1'
#ifndef GRROSE
  IPAddress gateway = local_ip;
  gateway[3] = 1;
  begin(mac_address, local_ip, dns_server, gateway);
#else
  begin(mac_address, local_ip, dns_server, ucGatewayAddress);
#endif
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
#ifndef GRROSE
  IPAddress subnet(255, 255, 255, 0);
  begin(mac_address, local_ip, dns_server, gateway, subnet);
#else
  begin(mac_address, local_ip, dns_server, gateway, ucNetMask);
#endif
}

void EthernetClass::begin(uint8_t *mac, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
#ifndef GRROSE
  W5100.init();
  SPI.beginTransaction(SPI_ETHERNET_SETTINGS);
  W5100.setMACAddress(mac);
  W5100.setIPAddress(local_ip.raw_address());
  W5100.setGatewayIp(gateway.raw_address());
  W5100.setSubnetMask(subnet.raw_address());
  SPI.endTransaction();
  _dnsServerAddress = dns_server;
#endif
  _dnsServerAddress = dns_server;

	FreeRTOS_IPInit(local_ip.raw_address(), subnet.raw_address(),
			gateway.raw_address(), dns_server.raw_address(), mac);

	/* We should wait for the network to be up before we run any demos. */
	while (FreeRTOS_IsNetworkUp() == pdFALSE) {
		vTaskDelay(3000);
	}

}

#ifndef GRROSE
int EthernetClass::maintain(){
  int rc = DHCP_CHECK_NONE;
  if(_dhcp != NULL){
    //we have a pointer to dhcp, use it
    rc = _dhcp->checkLease();
    switch ( rc ){
      case DHCP_CHECK_NONE:
        //nothing done
        break;
      case DHCP_CHECK_RENEW_OK:
      case DHCP_CHECK_REBIND_OK:
        //we might have got a new IP.
        SPI.beginTransaction(SPI_ETHERNET_SETTINGS);
        W5100.setIPAddress(_dhcp->getLocalIp().raw_address());
        W5100.setGatewayIp(_dhcp->getGatewayIp().raw_address());
        W5100.setSubnetMask(_dhcp->getSubnetMask().raw_address());
        SPI.endTransaction();
        _dnsServerAddress = _dhcp->getDnsServerIp();
        break;
      default:
        //this is actually a error, it will retry though
        break;
    }
  }
  return rc;
}
#endif

IPAddress EthernetClass::localIP()
{
#ifndef GRROSE
  IPAddress ret;
  SPI.beginTransaction(SPI_ETHERNET_SETTINGS);
  W5100.getIPAddress(ret.raw_address());
  SPI.endTransaction();
#endif
  uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
  FreeRTOS_GetAddressConfiguration(
      &ulIPAddress,
      &ulNetMask,
      &ulGatewayAddress,
      &ulDNSServerAddress );
  return (IPAddress)ulIPAddress;
}

IPAddress EthernetClass::subnetMask()
{
#ifndef GRROSE
  IPAddress ret;
  SPI.beginTransaction(SPI_ETHERNET_SETTINGS);
  W5100.getSubnetMask(ret.raw_address());
  SPI.endTransaction();
  return ret;
#endif
  uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
  FreeRTOS_GetAddressConfiguration(
      &ulIPAddress,
      &ulNetMask,
      &ulGatewayAddress,
      &ulDNSServerAddress );
  return (IPAddress)ulNetMask;
}

IPAddress EthernetClass::gatewayIP()
{
#ifndef GRROSE
  IPAddress ret;
  SPI.beginTransaction(SPI_ETHERNET_SETTINGS);
  W5100.getGatewayIp(ret.raw_address());
  SPI.endTransaction();
  return ret;
#endif
  uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
  FreeRTOS_GetAddressConfiguration(
      &ulIPAddress,
      &ulNetMask,
      &ulGatewayAddress,
      &ulDNSServerAddress );
  return (IPAddress)ulGatewayAddress;
}

IPAddress EthernetClass::dnsServerIP()
{
  return _dnsServerAddress;
}

EthernetClass Ethernet;
