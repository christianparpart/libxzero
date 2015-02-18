// This file is part of the "x0" project, http://xzero.io/
//   (c) 2009-2014 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include <xzero-base/Api.h>
#include <functional>

namespace xzero {

/**
 * Completion handler function type.
 *
 * The passed boolean parameter indicates whether the prior operation
 * did succeed or not.
 */
typedef std::function<void(bool /*succeed*/)> CompletionHandler;

} // namespace xzero