// This file is part of the "x0" project
//   (c) 2009-2014 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include <xzero/Api.h>

namespace xzero {

class HttpListener;

namespace fastcgi {

class XZERO_API FastCgiListener {
 public:
  virtual HttpListener* onCreateChannel() = 0;
};

} // namespace xzero
} // namespace fastcgi
