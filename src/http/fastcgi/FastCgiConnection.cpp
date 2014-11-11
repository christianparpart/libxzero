#include <xzero/http/fastcgi/FastCgiConnection.h>
#include <xzero/http/HttpChannel.h>
#include <xzero/http/HttpBufferedInput.h>

namespace xzero {
namespace fastcgi {

FastCgiConnection::FastCgiConnection(std::shared_ptr<EndPoint> endpoint,
                                     Executor* executor,
                                     const HttpHandler& handler,
                                     size_t maxRequestUriLength,
                                     size_t maxRequestBodyLength)
    : HttpTransport(endpoint, executor),
      handler_(handler),
      maxRequestUriLength_(maxRequestUriLength),
      maxRequestBodyLength_(maxRequestBodyLength),
      inputBuffer_(),
      inputOffset_(),
      parser_(this) {
}

void FastCgiConnection::abort() {
}

void FastCgiConnection::completed() {
}

void FastCgiConnection::send(HttpResponseInfo&& responseInfo, const BufferRef& chunk,
          CompletionHandler&& onComplete) {
}

void FastCgiConnection::send(HttpResponseInfo&& responseInfo, Buffer&& chunk,
          CompletionHandler&& onComplete) {
}

void FastCgiConnection::send(HttpResponseInfo&& responseInfo, FileRef&& chunk,
          CompletionHandler&& onComplete) {
}

void FastCgiConnection::send(Buffer&& chunk, CompletionHandler&& onComplete) {
}

void FastCgiConnection::send(FileRef&& chunk, CompletionHandler&& onComplete) {
}

void FastCgiConnection::send(const BufferRef& chunk, CompletionHandler&& onComplete) {
}

void FastCgiConnection::onOpen() {
  HttpTransport::onOpen();
  wantFill();

  // TODO support TCP_DEFER_ACCEPT here
#if 0
  if (connector()->deferAccept())
    onFillable();
  else
    wantFill();
#else
  wantFill();
#endif
}

void FastCgiConnection::onClose() {
  HttpTransport::onClose();
}

void FastCgiConnection::setInputBufferSize(size_t size) {
  inputBuffer_.reserve(std::min(4096lu, size));
}

HttpListener* FastCgiConnection::onCreateChannel() {
  channels_.emplace_back(new HttpChannel(
      this, handler_, std::unique_ptr<HttpInput>(new HttpBufferedInput()),
      maxRequestUriLength_, maxRequestBodyLength_, nullptr));

  return channels_.back().get();
}

} // namespace xzero
} // namespace fastcgi
