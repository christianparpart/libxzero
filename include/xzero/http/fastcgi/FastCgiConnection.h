#pragma once

#include <xzero/Api.h>
#include <xzero/http/HttpHandler.h>
#include <xzero/http/HttpChannel.h>
#include <xzero/http/HttpTransport.h>
#include <xzero/http/fastcgi/Parser.h>
#include <xzero/http/fastcgi/Generator.h>
#include <xzero/http/fastcgi/FastCgiListener.h>
#include <list>
#include <memory>

namespace xzero {

namespace fastcgi {

class XZERO_API FastCgiConnection
    : public HttpTransport,
      public FastCgiListener {
 public:
  FastCgiConnection(std::shared_ptr<EndPoint> endpoint,
                    Executor* executor,
                    const HttpHandler& handler,
                    size_t maxRequestUriLength,
                    size_t maxRequestBodyLength);

  // HttpTransport
  void abort() override;
  void completed() override;

  void send(HttpResponseInfo&& responseInfo, const BufferRef& chunk,
            CompletionHandler&& onComplete) override;
  void send(HttpResponseInfo&& responseInfo, Buffer&& chunk,
            CompletionHandler&& onComplete) override;
  void send(HttpResponseInfo&& responseInfo, FileRef&& chunk,
            CompletionHandler&& onComplete) override;

  void send(Buffer&& chunk, CompletionHandler&& onComplete) override;
  void send(FileRef&& chunk, CompletionHandler&& onComplete) override;
  void send(const BufferRef& chunk, CompletionHandler&& onComplete) override;

  // Connection
  void onOpen() override;
  void onClose() override;
  void setInputBufferSize(size_t size) override;

  // FastCgiListener
  HttpListener* onCreateChannel() override;

 private:
  HttpHandler handler_;
  size_t maxRequestUriLength_;
  size_t maxRequestBodyLength_;

  Buffer inputBuffer_;
  size_t inputOffset_;

  Parser parser_;
  //Generator generator_;

  std::list<std::unique_ptr<HttpChannel>> channels_;
};

} // namespace fastcgi
} // namespace xzero
