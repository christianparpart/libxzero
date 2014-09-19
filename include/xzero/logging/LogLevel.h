#pragma once

#include <xzero/Api.h>
#include <string>

namespace xzero {

enum class LogLevel {
  None = 0,
  Error = 1,
  Warning = 2,
  Info = 3,
  Debug = 4,
};

XZERO_API std::string to_string(LogLevel value);
XZERO_API LogLevel to_loglevel(const std::string& value);

}  // namespace xzero