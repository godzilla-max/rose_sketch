#ifndef WiFiEspServerClientWrapper_h
#define WiFiEspServerClientWrapper_h

#include <stdint.h>
#include <stddef.h>

#define ESP_MULTICAST_DEFAULT_IP          "239.255.0.2"
#define ESP_MULTICAST_DEFAULT_PORT        7400
#define UDP_LOCAL_PORT                    7300

#ifdef __cplusplus
extern "C" {
#endif
    bool   esp_connect_tcp(const char* host, uint16_t port);
    bool   esp_connect_udp(const char* host, uint16_t port);
    size_t esp_write_tcp(const uint8_t *buf, size_t size);
    size_t esp_write_udp(const uint8_t *buf, size_t size);
    int    esp_read_tcp(uint8_t* buf, size_t size, int timeout);
    int    esp_read_udp(uint8_t* buf, size_t size, int timeout);
    void   esp_stop_tcp(void);
    void   esp_stop_udp(void);
#ifdef __cplusplus
}
#endif

#endif
