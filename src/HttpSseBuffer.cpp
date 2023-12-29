#include <string.h>

#include "HttpSseBuffer.h"

size_t min(size_t x, size_t y) {
  return x > y ? y : x;
}

HttpSseBuffer::HttpSseBuffer(size_t maxBufferLength) {
  this->maxBufferLength = maxBufferLength;
  this->buffer = (char*) malloc(maxBufferLength + 2);
  this->buffer[0] = '\0';
  this->buffer[1] = '\0';
}

HttpSseBuffer::~HttpSseBuffer() {
  free(this->buffer);
}

int HttpSseBuffer::notifyDataWritten(size_t len) {
  size_t remaining = getRemainingBuffer();
  if (len > remaining) {
    return HTTPC_ERROR_BUFFER_OVERFLOW;
  }

  chunkedLength += len;
  buffer[decodedLength + 1 + chunkedLength] = 0;

  return decodeChunks();
}

int HttpSseBuffer::pollEventData(char *target, size_t len) {
  char* decodedBuffer = getDecodedBuffer();
  char* endOfMessage = strstr(decodedBuffer, "\r\n\r\n");

  if (!endOfMessage) {
    if (complete) {
      return HTTPC_ERROR_SSE_EOF;
    } else {
      return HTTPC_ERROR_SSE_INCOMPLETE;
    }
  }
  size_t messageLength = endOfMessage - decodedBuffer + 4;

  target[0] = '\0';
  size_t totalLength = 0;

  char* currentPos = decodedBuffer;
  while (currentPos < endOfMessage) {
    char* dataStart = strstr(currentPos, "data:");
    if (!dataStart || dataStart > endOfMessage) {
      break;
    }

    char* contentStart = dataStart + 5;
    while (*contentStart == ' ' || *contentStart == '\r' || *contentStart == '\n') {
      contentStart++;
    }

    char* endOfLine = strstr(contentStart, "\r\n");
    if (!endOfLine || endOfLine > endOfMessage) {
      break;
    }

    if (contentStart == endOfLine) {
      return HTTPC_ERROR_SSE_FORMAT;
    }

    size_t lineLength = endOfLine - contentStart;
    if (totalLength + lineLength >= len) {
      return HTTPC_ERROR_BUFFER_OVERFLOW;
    }

    if (currentPos != decodedBuffer) {
      strcat(target, "\n");
    }
    strncat(target, contentStart, lineLength);
    totalLength += lineLength;
    currentPos = endOfLine + 2;
  }

  consumeDecodedData(messageLength);

  return 0;
}

int HttpSseBuffer::decodeChunks() {
  while (chunkedLength > 0) {
    size_t chunkHeaderLength;
    size_t chunkSize;
    int ret = parseNextChunkSize(&chunkHeaderLength, &chunkSize);
    if (ret < 0) {
      return ret;
    }
    if (chunkHeaderLength == 0 || chunkSize > chunkedLength - chunkHeaderLength - 2) {
      break;
    }

    moveDecodedData(chunkHeaderLength, chunkSize);
    if (chunkSize == 0) {
      complete = true;
    }
  }
  return 0;
}

bool HttpSseBuffer::empty() {
  return decodedLength == 0 && chunkedLength == 0;
}

int HttpSseBuffer::parseNextChunkSize(size_t *chunkHeaderLength, size_t *chunkSize) {
  char* chunkedBuffer = getChunkedBuffer();
  char* endOfLine = (char*) strstr(chunkedBuffer, "\r\n");
  if (!endOfLine) {
    *chunkHeaderLength = 0;
    *chunkSize = 0;
    return 0;
  }
  size_t lineLength = endOfLine - chunkedBuffer + 2;

  *endOfLine = '\0';
  char* endPtr;
  long size = strtol(chunkedBuffer, &endPtr, 16);
  *endOfLine = '\r';

  if (endPtr != endOfLine) {
    return HTTPC_ERROR_CHUNKED_FORMAT;
  }

  if (size < 0) {
    return HTTPC_ERROR_CHUNKED_FORMAT;
  }

  *chunkSize = (size_t) size;
  *chunkHeaderLength = lineLength;
  return 0;
}

void HttpSseBuffer::moveDecodedData(size_t chunkHeaderLength, size_t chunkSize) {
  size_t newChunkedOffset = decodedLength + 1 + chunkHeaderLength + chunkSize + 2;
  size_t newChunkedLength = chunkedLength - chunkHeaderLength - chunkSize - 2;

  memmove(buffer + decodedLength, buffer + decodedLength + 1 + chunkHeaderLength, chunkSize);
  decodedLength += chunkSize;
  buffer[decodedLength] = 0;

  memmove(buffer + decodedLength + 1, buffer + newChunkedOffset, newChunkedLength);
  chunkedLength = newChunkedLength;
  buffer[decodedLength + 1 + chunkedLength] = 0;

  cleanBuffer();
}

void HttpSseBuffer::consumeDecodedData(size_t len) {
  len = min(len, decodedLength);
  memmove(buffer, buffer + min(len, decodedLength), decodedLength + 1 + chunkedLength + 1 - len);
  decodedLength -= len;
  cleanBuffer();
}

void HttpSseBuffer::cleanBuffer() {
  memset(buffer + decodedLength + 1 + chunkedLength + 1, 0, maxBufferLength - decodedLength - chunkedLength);
}

char* HttpSseBuffer::getDecodedBuffer() {
  return buffer;
}

char* HttpSseBuffer::getChunkedBuffer() {
  return buffer + decodedLength + 1;
}

char* HttpSseBuffer::getEndOfChunkedBuffer() {
  return buffer + decodedLength + 1 + chunkedLength;
}

size_t HttpSseBuffer::getRemainingBuffer() {
  return maxBufferLength - decodedLength - chunkedLength;
}

const char *HttpSseBuffer::errorToString(int error) {
  switch (error) {
    case HTTPC_ERROR_SSE_EOF:
      return "HTTPC_ERROR_SSE_EOF";
    case HTTPC_ERROR_SSE_INCOMPLETE:
      return "HTTPC_ERROR_SSE_INCOMPLETE";
    case HTTPC_ERROR_SSE_FORMAT:
      return "HTTPC_ERROR_SSE_FORMAT";
    case HTTPC_ERROR_BUFFER_OVERFLOW:
      return "HTTPC_ERROR_BUFFER_OVERFLOW";
    case HTTPC_ERROR_CHUNKED_FORMAT:
      return "HTTPC_ERROR_CHUNKED_FORMAT";
    default:
      return "UNKNOWN";
  }
}
