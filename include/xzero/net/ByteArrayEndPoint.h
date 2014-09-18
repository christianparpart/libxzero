#pragma once

#include <xzero/Api.h>
#include <xzero/Buffer.h>
#include <xzero/net/EndPoint.h>

namespace xzero {

/**
 * Buffer-based dual-channel EndPoint.
 *
 * @see LocalEndPoint
 * @see InetEndPoint
 */
class XZERO_API ByteArrayEndPoint : public EndPoint {
 public:
  ByteArrayEndPoint();
  ~ByteArrayEndPoint();

  /**
   * Assigns an input to this endpoint.
   *
   * @param input a movable buffer object.
   */
  void setInput(Buffer&& input);

  /**
   * Assigns an input to this endpoint.
   *
   * @param input a C-string.
   */
  void setInput(const char* input);

  /**
   * Retrieves a reference to the input buffer.
   */
  const Buffer& input() const;

  /**
   * Retrieves a reference to the output buffer.
   */
  const Buffer& output() const;

  // overrides
  void close() override;
  bool isOpen() const override;
  std::string toString() const override;
  size_t fill(Buffer*) override;
  size_t flush(const BufferRef&) override;
  size_t flush(int fd, off_t offset, size_t size) override;
  void wantFill() override;
  void wantFlush() override;
  TimeSpan idleTimeout() override;
  void setIdleTimeout(TimeSpan timeout) override;
  bool isBlocking() const override;
  void setBlocking(bool enable) override;
  bool isCorking() const override;
  void setCorking(bool enable) override;

 private:
  Buffer input_;
  size_t readPos_;
  Buffer output_;
  bool closed_;
};

} // namespace xzero