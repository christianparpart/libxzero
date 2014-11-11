#include <xzero/http/fastcgi/Parser.h>
#include <xzero/http/HttpListener.h>
#include <xzero/http/HttpListener.h>

namespace xzero {
namespace fastcgi {

Parser::Parser(FastCgiListener* listener)
    : listener_(listener) {
}

size_t Parser::parseFragment(const BufferRef& chunk) {
  size_t offset = 0;

  // process each fully received packet ...
  while (offset + sizeof(Record) <= chunk.size()) {
    auto record = reinterpret_cast<const Record*>(chunk.data() + offset);

    if (chunk.size() - offset < record->size())
      break;

    offset += record->size();

    process(record);
  }

  return offset;
}

void Parser::process(const Record* record) {
  switch (record->type()) {
    case RecordType::BeginRequest:
      beginRequest(static_cast<const BeginRequestRecord *>(record));
      break;
    case RecordType::AbortRequest:
      abortRequest(static_cast<const AbortRequestRecord *>(record));
      break;
    case RecordType::Params:
      streamParams(record);
      break;
    case RecordType::StdIn:
      streamStdIn(record);
      break;
    case RecordType::Data:
      streamData(record);
      break;
    case RecordType::GetValues:
      //TODO getValues(...);
      break;
    default:
      // TODO throw UnknownFastCgiRecord(record->type(), record->requestId());
      break;
  }
}

void Parser::beginRequest(const BeginRequestRecord* record) {
}

void Parser::streamParams(const Record* record) {
}

void Parser::streamStdIn(const Record* record) {
}

void Parser::streamData(const Record* record) {
}

void Parser::abortRequest(const AbortRequestRecord* record) {
}


// {{{
void Parser::process(const EndRequestRecord* record) {
}

void Parser::processStdOut(const BufferRef& chunk) {
}

void Parser::processStdErr(const BufferRef& chunk) {
}
// }}}

// {{{ ParamStreamReader
ParamStreamReader::ParamStreamReader()
    : name_(), value_()
{
}

void ParamStreamReader::processParams(const BufferRef& chunk) {
  const char *i = chunk.data();
  size_t pos = 0;

  while (pos < chunk.size()) {
    size_t nameLength = 0;
    size_t valueLength = 0;

    if ((*i >> 7) == 0) { // 11 | 14
      nameLength = *i & 0xFF;
      ++pos;
      ++i;

      if ((*i >> 7) == 0) { // 11
        valueLength = *i & 0xFF;
        ++pos;
        ++i;
      } else { // 14
        valueLength = (((i[0] & ~(1<<7)) & 0xFF) << 24)
                    + ((i[1] & 0xFF) << 16)
                    + ((i[2] & 0xFF) << 8)
                    + ((i[3] & 0xFF));
        pos += 4;
        i += 4;
      }
    } else { // 41 || 44
      nameLength = (((i[0] & ~(1<<7)) & 0xFF) << 24)
                 + ((i[1] & 0xFF) << 16)
                 + ((i[2] & 0xFF) << 8)
                 + ((i[3] & 0xFF));
      pos += 4;
      i += 4;

      if ((*i >> 7) == 0) { // 41
        valueLength = *i;
        ++pos;
        ++i;
      } else { // 44
        valueLength = (((i[0] & ~(1<<7)) & 0xFF) << 24)
                    + ((i[1] & 0xFF) << 16)
                    + ((i[2] & 0xFF) << 8)
                    + ((i[3] & 0xFF));
        pos += 4;
        i += 4;
      }
    }

    const char *name = i;
    pos += nameLength;
    i += nameLength;

    const char *value = i;
    pos += valueLength;
    i += valueLength;

    onParam(BufferRef(name, nameLength), BufferRef(value, valueLength));
  }
}
// }}}

} // namespace xzero
} // namespace fastcgi
