#include "Arduino.h"
#include "postParser.h"

//private


String getLine(String data) {
  int endOfLineIndex = data.indexOf("\r\n");
  return data.substring(0, endOfLineIndex );
}

String popLine(String data) {
  int endOfLineIndex = data.indexOf("\r\n");
  //Serial.println(data.substring(endOfLineIndex + 2, data.length() - 1));
  return data.substring(endOfLineIndex + 2, data.length());
}

String getHeaderField(String data, String key) {

  int keyIndex = data.indexOf(key);
  if (keyIndex == -1) {
    return "";
  }
  int startIndex = data.indexOf(": ", keyIndex);
  int stopIndex = data.indexOf("\r\n", keyIndex);

  return data.substring(startIndex + 2, stopIndex);
}

String getLineFromIndex(String data, int index) {
  String bufferData = data;
  for (int i = 0; i < index; i++) {
    bufferData = popLine(bufferData);
  }
  return getLine(bufferData);
}

String getContentType(String header) {
  String contentType = getHeaderField(header, "content-type");
  if (contentType == "") {
    contentType = getHeaderField(header, "Content-Type");
  }
  return contentType;
}

String readPayLoad(WiFiEspClient client, int payLoadSize) {
  String payload = String("");
  for (int i = 0; i < payLoadSize; i++) {
    char c = client.read();
    payload += String(c);
  }
  return payload;
}


int getPayLoadSize(String header) {
  String contentLength = getHeaderField(header, "content-length");
  if (contentLength == "") {
    contentLength = getHeaderField(header, "Content-Length");
  }
  return contentLength.toInt();
}


//public
PostParser::PostParser(WiFiEspClient client) {
  //save connection
  _client = client;


  //clear data
  _header = String("");
  _payload = String("");
}

void PostParser::addHeaderCharacter(char c) {
  _header += String(c);
}

String PostParser::getHeader() {
  return _header;
}

void PostParser::grabPayload() {
  //if (getContentType(_header) == "application/x-www-form-urlencoded") {
  _payload = readPayLoad(_client, getPayLoadSize(_header));
  //}
}

String PostParser::getPayload() {
  return _payload;
}

String PostParser::getField(String key) {

  int keyIndex = _payload.indexOf(key);
  int startIndex = _payload.indexOf("=", keyIndex);
  int stopIndex = _payload.indexOf("&", keyIndex);

  return _payload.substring(startIndex + 1, stopIndex);

}





