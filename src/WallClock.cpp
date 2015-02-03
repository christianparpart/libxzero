// This file is part of the "x0" project, http://xzero.io/
//   (c) 2009-2014 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#include <xzero/WallClock.h>
#include <xzero/DateTime.h>
#include <xzero/sysconfig.h>
#include <ctime>

namespace xzero {

class SystemClock : public WallClock {
 public:
  DateTime get() const override;
};

DateTime SystemClock::get() const {
#ifdef HAVE_CLOCK_GETTIME
  timespec ts;
  memset(&ts, 0, sizeof(ts));
  if (clock_gettime(CLOCK_REALTIME, &ts) < 0)
    return DateTime(std::time(nullptr));

  return DateTime(
      static_cast<double>(ts.tv_sec) +
      TimeSpan::fromNanoseconds(ts.tv_nsec).value());
#else
  return DateTime(std::time(nullptr));
#endif
}

WallClock* WallClock::system() {
  static SystemClock clock;
  return &clock;
}

} // namespace xzero
