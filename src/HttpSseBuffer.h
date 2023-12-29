#ifndef HTTP_SSE_BUFFER_H
#define HTTP_SSE_BUFFER_H

#include <stdint.h>
#include <stdlib.h>

#define HTTPC_ERROR_SSE_EOF         (-12)
#define HTTPC_ERROR_SSE_INCOMPLETE  (-13)
#define HTTPC_ERROR_SSE_FORMAT      (-14)
#define HTTPC_ERROR_BUFFER_OVERFLOW (-15)
#define HTTPC_ERROR_CHUNKED_FORMAT  (-16)

class HttpSseBuffer {

public:
  HttpSseBuffer(size_t maxBufferLength = 1022);
  ~HttpSseBuffer();
  char* getDecodedBuffer();
  char* getChunkedBuffer();
  char* getEndOfChunkedBuffer();
  size_t getRemainingBuffer();

  int notifyDataWritten(size_t len);
  int decodeChunks();
  int pollEventData(char* target, size_t len);
  bool empty();

  const char* errorToString(int error);
  bool isComplete() { return complete; }

protected:
  int parseNextChunkSize(size_t* chunkHeaderLength, size_t* chunkSize);
  void moveDecodedData(size_t chunkHeaderLength, size_t chunkSize);
  void consumeDecodedData(size_t len);
  void cleanBuffer();


private:
  char* buffer;
  size_t maxBufferLength;
  size_t decodedLength = 0;
  size_t chunkedLength = 0;
  bool complete = false;
};

#endif //HTTP_SSE_BUFFER_H
