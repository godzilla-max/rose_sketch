// Author Natan Biesmans
// Website https://github.com/NatanBiesmans/Arduino-POST-HTTP-Parser
//
// July 8 2019: Modified to use WiFiEsp for GR-ROSE by Yuuki Okamiya.

#ifndef POSTPARSER_h
#define POSTPARSER_h


#include "Arduino.h"
#ifndef GRROSE
#include "Ethernet.h"
#else
#include "WiFiEsp.h"
#endif

class PostParser {
  public:
    PostParser(WiFiEspClient client);
    void addHeaderCharacter(char c);
    String getHeader();
    void grabPayload();
    String getPayload();
    String getField(String key);
  private:
    String _header;
    String _payload;
    WiFiEspClient _client;

    String getHeaderField(String data, String key);
    String getLine(String data);
    String popLine(String data);
    String getLineFromIndex(String data, int index);
    String extractPayload(String payload, String key);
};

#endif
