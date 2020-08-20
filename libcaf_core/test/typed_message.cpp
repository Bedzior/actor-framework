/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright 2011-2020 Dominik Charousset                                     *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#define CAF_SUITE typed_message

#include "caf/typed_message.hpp"

#include "caf/test/dsl.hpp"

using namespace caf;

namespace {

struct fixture {};

} // namespace

CAF_TEST_FIXTURE_SCOPE(typed_message_tests, fixture)

CAF_TEST(default constructed typed messages are invalid) {
  CAF_CHECK(!typed_message<int>{});
}

CAF_TEST(messages are convertible to typed messages) {
  CAF_CHECK(typed_message<int>::from_message(make_message(42)));
  CAF_CHECK(!typed_message<int>::from_message(make_message(42.0)));
}

CAF_TEST(typed message are convertible to regular messages) {
}

CAF_TEST_FIXTURE_SCOPE_END()
