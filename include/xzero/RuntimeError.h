// This file is part of the "x0" project
//   (c) 2009-2014 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once
#include <stdexcept>
#include <string.h>
#include <errno.h>

namespace xzero {

class RuntimeError : public std::runtime_error {
 public:
  RuntimeError(const char* what, const char* sourceFile, int sourceLine)
      : std::runtime_error(what),
        sourceFile_(sourceFile),
        sourceLine_(sourceLine) {}

  RuntimeError(const std::string& what, const char* sourceFile, int sourceLine)
      : std::runtime_error(what),
        sourceFile_(sourceFile),
        sourceLine_(sourceLine) {}

  const char* sourceFile() const { return sourceFile_; }
  int sourceLine() const noexcept { return sourceLine_; }

 private:
  const char* sourceFile_;
  int sourceLine_;
};

} // namespace xzero

#if !defined(BUG_ON)
  #define BUG_ON(cond) {                                                    \
    if (unlikely(cond)) {                                                   \
      throw ::xzero::RuntimeError("BUG_ON: (" #cond ")",                    \
                                  __FILE__, __LINE__);                      \
    }                                                                       \
  }
#endif

#if !defined(RAISE_IF_ERRNO)
  #define RAISE_IF_ERRNO() {                                                \
    if (errno != 0) {                                                       \
      char buf[256];                                                        \
      strerror_r(errno, buf, sizeof(buf);                                   \
      size_t n = strlen(buf);                                               \
      throw ::xzero::RuntimeError(std::string(buf, n), __FILE__, __LINE__); \
    }                                                                       \
  }
#endif