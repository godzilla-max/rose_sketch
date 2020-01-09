#include "WiFiEspServerClientWrapper.h"
#include "WiFiEspClient.h"
#include "WiFiEspServer.h"
#include "WiFiEspUdp.h"

#define DELAY_INTERVAL 1

size_t _esp_check_read_data(int timeout, int mode);

WiFiEspClient _WiFiEspClient;
WiFiEspUDP _WiFiEspUdp;

bool esp_connect_tcp(const char* host, uint16_t port)
{
    int connected = _WiFiEspClient.connect(host, port);
    if(connected == 1)
    {
        return true;
    }

    return false;
}

bool esp_connect_udp(const char* host, uint16_t port)
{
    uint8_t status = WiFi.status();
    switch(status) {
    case WL_NO_SHIELD:
        return false;
    case WL_CONNECTED:
        esp_stop_udp();
        break;
    case WL_IDLE_STATUS:
    case WL_DISCONNECTED:
    default:
        break;
    }

    uint8_t begined = _WiFiEspUdp.begin(UDP_LOCAL_PORT);
    if(begined != 1)
    {
        return false;
    }

    int begined_packet = _WiFiEspUdp.beginPacket(host, port);
    if(begined_packet != 1)
    {
        return false;
    }

    return true;
}

size_t esp_write_tcp(const uint8_t *buf, size_t size)
{
    return _WiFiEspClient.write(buf, size);
}

size_t esp_write_udp(const uint8_t *buf, size_t size)
{
    return _WiFiEspUdp.write(buf, size);
}

int esp_read_tcp(uint8_t* buf, size_t size, int timeout)
{
    size_t checked_read_data = _esp_check_read_data(timeout, TCP_MODE);
    if(!checked_read_data)
    {
        return 0;
    }

    int data_size = _WiFiEspClient.read(buf, size);
    if(data_size <= 0)
    {
        return -1;
    }

    return data_size;
}

int esp_read_udp(uint8_t* buf, size_t size, int timeout)
{
    size_t checked_read_data = _esp_check_read_data(timeout, UDP_MODE);
    if(!checked_read_data)
    {
        return 0;
    }

    int data_size = _WiFiEspUdp.read(buf, size);
    if(data_size <= 0)
    {
        return -1;
    }

    return data_size;
}

void esp_stop_tcp(void)
{
    _WiFiEspClient.stop();
}

void esp_stop_udp(void)
{
    _WiFiEspUdp.stop();
}

size_t _esp_check_read_data(int timeout, int mode)
{
    int tv_msec = timeout;
    if(tv_msec < 0)
    {
        tv_msec = 0;
    }

    size_t available_size = 0;
    while(tv_msec >= 0)
    {
        switch(mode) {
        case TCP_MODE:
        available_size = _WiFiEspClient.available();
            break;
        case UDP_MODE:
        available_size = _WiFiEspUdp.parsePacket();
            break;
        }

        if(available_size > 0)
        {
            break;
        }

        delay(DELAY_INTERVAL);
        tv_msec = tv_msec - DELAY_INTERVAL;
    }

    return available_size;
}
