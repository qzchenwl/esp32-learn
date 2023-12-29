#ifndef ESP32_LEARN_SSEHTTPCLIENT_H
#define ESP32_LEARN_SSEHTTPCLIENT_H

#include <HTTPClient.h>
#include "HttpSseBuffer.h"

class SSEHTTPClient: public HTTPClient {
public:
  int pollEventData(char* eventData, size_t len);

protected:
  HttpSseBuffer httpSseBuffer;
};


#endif //ESP32_LEARN_SSEHTTPCLIENT_H
