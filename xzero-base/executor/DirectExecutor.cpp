// This file is part of the "x0" project, http://xzero.io/
//   (c) 2009-2014 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#include <xzero-base/executor/DirectExecutor.h>
#include <xzero-base/logging/LogSource.h>
#include <stdio.h>

namespace xzero {

static LogSource directExecutorLogger("executor.DirectExecutor");
#ifndef NDEBUG
#define TRACE(msg...) do { directExecutorLogger.trace(msg); } while (0)
#else
#define TRACE(msg...) do {} while (0)
#endif

DirectExecutor::DirectExecutor(
    bool recursive,
    std::function<void(const std::exception&)> eh)
    : Executor(std::move(eh)),
      recursive_(recursive),
      running_(0),
      deferred_() {
}

void DirectExecutor::execute(Task task) {
  if (isRunning() && !isRecursive()) {
    deferred_.push_back(std::move(task));
    TRACE("%p execute: enqueue task (%zu)", this, deferred_.size());
    return;
  }

  running_++;

  TRACE("%p execute: run top-level task", this);
  safeCall(task);

  while (!deferred_.empty()) {
    TRACE("%p execute: run deferred task (-%zu)", this, deferred_.size());
    safeCall(deferred_.front());
    deferred_.pop_front();
  }

  running_--;
}

std::string DirectExecutor::toString() const {
  char buf[128];
  snprintf(buf, sizeof(buf), "DirectExecutor@%p", this);
  return buf;
}

} // namespace xzero