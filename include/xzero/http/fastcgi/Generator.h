#pragma once

#include <xzero/Api.h>
#include <xzero/http/fastcgi/bits.h>

#if 0
/** generates a PARAM stream. */
class CgiParamStreamWriter {
private:
  x0::Buffer buffer_;

  inline void encodeLength(size_t length);

public:
  CgiParamStreamWriter();

  void encode(const char *name, size_t nameLength, const char *value, size_t valueLength);
  void encode(const char *name, size_t nameLength, const char *v1, size_t l1, const char *v2, size_t l2);

  void encode(const std::string& name, const std::string& value)
    { encode(name.data(), name.size(), value.data(), value.size()); }
  void encode(const x0::BufferRef& name, const x0::BufferRef& value)
    { encode(name.data(), name.size(), value.data(), value.size()); }
  void encode(const std::string& name, const x0::BufferRef& value)
    { encode(name.data(), name.size(), value.data(), value.size()); }

  template<typename V1, typename V2>
  void encode(const std::string& name, const V1& v1, const V2& v2)
    { encode(name.data(), name.size(), v1.data(), v1.size(), v2.data(), v2.size()); }

  template<typename PodType, std::size_t N, typename ValuePType, std::size_t N2>
  void encode(PodType (&name)[N], const ValuePType (&value)[N2])
    { encode(name, N - 1, value, N2 - 1); }

  template<typename PodType, std::size_t N, typename ValueType>
  void encode(PodType (&name)[N], const ValueType& value)
    { encode(name, N - 1, value.data(), value.size()); }

  x0::Buffer&& output() { return std::move(buffer_); }
};
#endif

