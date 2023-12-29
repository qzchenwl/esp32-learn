#include "SSEHTTPClient.h"

int SSEHTTPClient::pollEventData(char* event, size_t len) {
  int ret = httpSseBuffer.pollEventData(event, len);
  if (ret != HTTPC_ERROR_SSE_INCOMPLETE) {
    return ret;
  }

  // read more data
  if(!connected()) {
    return returnError(HTTPC_ERROR_NOT_CONNECTED);
  }
  if (_transferEncoding != HTTPC_TE_CHUNKED) {
    return returnError(HTTPC_ERROR_ENCODING);
  }
  int sizeAvailable = _client->available();
  if (sizeAvailable <= 0) {
    return HTTPC_ERROR_SSE_INCOMPLETE;
  }
  size_t remainingBuffer = httpSseBuffer.getRemainingBuffer();
  int readBytes = sizeAvailable < remainingBuffer ? sizeAvailable : remainingBuffer;

  // write and maintain state of httpSseBuffer
  char* endOfChunkedBuffer = httpSseBuffer.getEndOfChunkedBuffer();
  int bytesRead = _client->readBytes(endOfChunkedBuffer, readBytes);
  httpSseBuffer.notifyDataWritten(bytesRead);
  // end

  return httpSseBuffer.pollEventData(event, len);
}
