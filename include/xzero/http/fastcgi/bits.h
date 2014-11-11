// This file is part of the "x0" project
//   (c) 2009-2014 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include <xzero/Api.h>

// this file holds the protocol bits

#include <stdint.h>    // uint16_t, ...
#include <string.h>    // memset
#include <arpa/inet.h> // htons/ntohs/ntohl/htonl

#include <string>
#include <vector>

namespace xzero {
namespace fastcgi {

enum class RecordType {
  BeginRequest    = 1,
  AbortRequest    = 2,
  EndRequest      = 3,
  Params          = 4,
  StdIn           = 5,
  StdOut          = 6,
  StdErr          = 7,
  Data            = 8,
  GetValues       = 9,
  GetValuesResult = 10,
  UnknownType     = 11
};

enum class Role {
  Responder  = 1,
  Authorizer = 2,
  Filter     = 3
};

enum class ProtocolStatus {
  RequestComplete     = 0,
  CannotMpxConnection = 1,
  Overloaded          = 2,
  UnknownRole         = 3
};

typedef uint16_t RequestID;

struct XZERO_PACKED Record {
 private:
  uint8_t version_;
  uint8_t type_;
  uint16_t requestId_;
  uint16_t contentLength_;
  uint8_t paddingLength_;
  uint8_t reserved_;

 public:
  Record(RecordType type, uint16_t requestId, uint16_t contentLength, uint8_t paddingLength)
      : version_(1),
        type_(static_cast<uint8_t>(type)),
        requestId_(htons(requestId)),
        contentLength_(htons(contentLength)),
        paddingLength_(paddingLength),
        reserved_() {}

  static Record *create(RecordType type, uint16_t requestId, uint16_t contentLength);

  int version() const { return version_; }
  RecordType type() const { return static_cast<RecordType>(type_); }
  int requestId() const { return ntohs(requestId_); }
  int contentLength() const { return ntohs(contentLength_); }
  int paddingLength() const { return paddingLength_; }
  uint8_t reserved1() const { return reserved_; }

  const char *content() const { return reinterpret_cast<const char *>(this) + sizeof(*this); }

  const char *data() const { return reinterpret_cast<const char *>(this); }
  uint16_t size() const { return sizeof(Record) + contentLength() + paddingLength(); }

  bool isManagement() const { return requestId() == 0; }
  bool isApplication() const { return requestId() != 0; }

  const char *type_str() const {
    static const char *map[] = {
      0, "BeginRequest", "AbortRequest", "EndRequest", "Params", "StdIn", "StdOut",
      "StdErr", "Data", "GetValues", "GetValuesResult", "UnknownType"
    };

    return type_ > 0 && type_ < 12 ? map[type_] : "invalid";
  }
};

struct XZERO_PACKED BeginRequestRecord : public Record {
 private:
  uint16_t role_;
  uint8_t flags_;
  uint8_t reserved_[5];

 public:
  BeginRequestRecord(Role role, uint16_t requestId, bool keepAlive)
      : Record(RecordType::BeginRequest, requestId, 8, 0),
        role_(htons(static_cast<uint16_t>(role))),
        flags_(keepAlive ? 0x01 : 0x00) {
    memset(reserved_, 0, sizeof(reserved_));
  }

  Role role() const { return static_cast<Role>(ntohs(role_)); }

  bool isKeepAlive() const { return flags_ & 0x01; }

  const char *role_str() {
    switch (role()) {
      case Role::Responder: return "responder";
      case Role::Authorizer: return "authorizer";
      case Role::Filter: return "filter";
      default: return "invalid";
    }
  }
};

struct XZERO_PACKED AbortRequestRecord : public Record {
 public:
  AbortRequestRecord(uint16_t requestId) :
    Record(RecordType::AbortRequest, requestId, 0, 0) {}
};

struct XZERO_PACKED UnknownTypeRecord : public Record {
 private:
  uint8_t unknownType_;
  uint8_t reserved_[7];

 public:
  UnknownTypeRecord(RecordType type, uint16_t requestId)
      : Record(RecordType::UnknownType, requestId, 8, 0),
        unknownType_(static_cast<uint8_t>(type)) {
    memset(&reserved_, 0, sizeof(reserved_));
  }

  int unknownType() const { return unknownType_; }
};

struct XZERO_PACKED EndRequestRecord : public Record {
 private:
  uint32_t appStatus_;
  uint8_t protocolStatus_;
  uint8_t reserved_[3];

 public:
  EndRequestRecord(uint16_t requestId, uint32_t appStatus,
                   ProtocolStatus protocolStatus)
      : Record(RecordType::EndRequest, requestId, 8, 0),
        appStatus_(appStatus),
        protocolStatus_(htonl(static_cast<uint8_t>(protocolStatus))) {
    reserved_[0] = 0;
    reserved_[1] = 0;
    reserved_[2] = 0;
  }

  uint32_t appStatus() const { return ntohl(appStatus_); }
  ProtocolStatus protocolStatus() const { return static_cast<ProtocolStatus>(protocolStatus_); }
};

inline Record *Record::create(RecordType type,
                              uint16_t requestId,
                              uint16_t contentLength) {
  int paddingLength = (sizeof(Record) + contentLength) % 8;
  char* p = new char[sizeof(Record) + contentLength + paddingLength];
  return new (p) Record(type, requestId, contentLength, paddingLength);
}

#if 0 // {{{
// CgiParamStreamWriter
inline CgiParamStreamWriter::CgiParamStreamWriter() :
  buffer_()
{
}

inline void CgiParamStreamWriter::encodeLength(size_t length)
{
  if (length < 127)
    buffer_.push_back(static_cast<char>(length));
  else {
    unsigned char value[4];
    value[0] = ((length >> 24) & 0xFF) | 0x80;
    value[1] = (length >> 16) & 0xFF;
    value[2] = (length >> 8) & 0xFF;
    value[3] = length & 0xFF;

    buffer_.push_back(&value, sizeof(value));
  }
}

inline void CgiParamStreamWriter::encode(const char *name, size_t nameLength, const char *value, size_t valueLength)
{
  encodeLength(nameLength);
  encodeLength(valueLength);

  buffer_.push_back(name, nameLength);
  buffer_.push_back(value, valueLength);
}

inline void CgiParamStreamWriter::encode(const char *name, size_t nameLength,
  const char *v1, size_t l1, const char *v2, size_t l2)
{
  encodeLength(nameLength);
  encodeLength(l1 + l2);

  buffer_.push_back(name, nameLength);
  buffer_.push_back(v1, l1);
  buffer_.push_back(v2, l2);
}
#endif
// }}}

} // namespace fastcgi
} // namespace xzero
