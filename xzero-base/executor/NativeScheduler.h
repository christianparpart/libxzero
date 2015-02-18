// This file is part of the "x0" project
//   (c) 2009-2014 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT
#pragma once

#include <xzero-base/sysconfig.h>
#include <xzero-base/executor/PosixScheduler.h>

namespace xzero {

#if defined(__linux__)
// TODO using NativeScheduler = LinuxScheduler;
using NativeScheduler = PosixScheduler;
#else
using NativeScheduler = PosixScheduler;
#endif

} // namespace xzero
