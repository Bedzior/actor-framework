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

#pragma once

#include "caf/detail/message_data.hpp"
#include "caf/detail/offset_at.hpp"
#include "caf/detail/type_list.hpp"
#include "caf/message.hpp"

namespace caf {

template <class... Ts>
class typed_message {
public:
  // -- member types -----------------------------------------------------------

  using data_ptr = intrusive_cow_ptr<detail::message_data>;

  // -- constructors, destructors, and assignment operators --------------------

  typed_message() noexcept : data_(nullptr) {
    // nop
  }

  typed_message(typed_message&&) noexcept = default;

  typed_message(const typed_message&) noexcept = default;

  typed_message& operator=(typed_message&&) noexcept = default;

  typed_message& operator=(const typed_message&) noexcept = default;

  explicit operator bool() const noexcept {
    return data_ != nullptr;
  }

  static typed_message from_message(const message& msg) {
    if (msg.types() == make_type_id_list<Ts...>())
      return typed_message{msg.data_};
    return typed_message{};
  }

  static typed_message from_message(message&& msg) {
    if (msg.types() == make_type_id_list<Ts...>())
      return typed_message{std::move(msg.data_)};
    return typed_message{};
  }

  /// @private
  static typed_message unsafe_from_message(message msg) {
    return typed_message{std::move(msg.data_)};
  }

  // -- conversion -------------------------------------------------------------

  message to_message() const& {
    return message{data_};
  }

  message to_message() && {
    return message{std::move(data_)};
  }

  // -- modifiers --------------------------------------------------------------

  void swap(typed_message& other) noexcept {
    data_.swap(other.data_);
  }

  /// @private
  auto& data() {
    return data_.unshared();
  }

  /// @private
  auto& data() const {
    return *data_;
  }

private:
  explicit typed_message(data_ptr dptr) : data_(std::move(dptr)) {
    // nop
  }

  data_ptr data_;
};

template <size_t Index, class... Ts>
const auto& get(const typed_message<Ts...>& x) {
  static_assert(Index < sizeof...(Ts));
  using type = detail::tl_at_t<detail::type_list<Ts...>, Index>;
  return reinterpret_cast<const type*>(x.data().at(Index));
}

template <size_t Index, class... Ts>
auto& get_mutable(typed_message<Ts...>& x) {
  static_assert(Index < sizeof...(Ts));
  using type = detail::tl_at_t<detail::type_list<Ts...>, Index>;
  return reinterpret_cast<type*>(x.data().at(Index));
}

template <class... Ts>
auto make_typed_message(Ts&&... xs) {
  using type = typed_message<detail::strip_and_convert_t<Ts>...>;
  return type::unsafe_from_message(make_message(std::forward<Ts>(xs)...));
}

} // namespace caf
