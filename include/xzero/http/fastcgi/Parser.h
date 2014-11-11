// This file is part of the "x0" project
//   (c) 2009-2014 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include <xzero/Api.h>
#include <xzero/Buffer.h>
#include <xzero/http/fastcgi/bits.h>
#include <unordered_map>

namespace xzero {

class HttpListener;

namespace fastcgi {

class FastCgiListener;

/**
 * Parses a PARAM stream and reads out name/value pairs.
 */
class XZERO_API ParamStreamReader {
private:
  std::vector<char> name_;  // name token
  std::vector<char> value_;  // value token

public:
  ParamStreamReader();

  void processParams(const BufferRef& chunk);
  virtual void onParam(const BufferRef& name, const BufferRef& value) = 0;
};

class XZERO_API Parser {
 public:
  explicit Parser(FastCgiListener* listener);

  size_t parseFragment(const BufferRef& chunk);

 private:
  void process(const Record* r);
  void terminate(RequestID requestId, ProtocolStatus protocolStatus);

  // server->application
  void beginRequest(const BeginRequestRecord* record);
  void streamParams(const Record* record);
  void streamStdIn(const Record* record);
  void streamData(const Record* record);
  void abortRequest(const AbortRequestRecord* record);

  // application->server
  void process(const EndRequestRecord* record);
  void processStdOut(const BufferRef& chunk);
  void processStdErr(const BufferRef& chunk);

 private:
  class Stream;
  std::unordered_map<RequestID, Stream*> streams_;
  FastCgiListener* listener_;
};

class Parser::Stream : public ParamStreamReader {
 public:
  Stream(RequestID requestId, HttpListener* listener);

  void onParam(const BufferRef& name, const BufferRef& value) override;

  void onData(const BufferRef& data);
  void onError(const BufferRef& data);

 private:
  RequestID requestId_;
  HttpListener* listener_;
};

} // namespace fastcgi
} // namespace xzero
