/*
 * Copyright (c) 2013-2015, Roland Bock
 * Copyright (c) 2018, Coloridad Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once
#include <sqlpp11/statement.h>

#include <sqlpp11/noop.h>
#include <sqlpp11/connection.h>
#include <sqlpp11/with.h>
#include <sqlpp11/select_flag_list.h>
#include <sqlpp11/select_column_list.h>
#include <sqlpp11/from.h>
#include <sqlpp11/where.h>
#include <sqlpp11/group_by.h>
#include <sqlpp11/having.h>
#include <sqlpp11/order_by.h>
#include <sqlpp11/limit.h>
#include <sqlpp11/for_update.h>
#include <sqlpp11/offset.h>
#include <sqlpp11/union.h>
#include <sqlpp11/expression.h>
#include <sqlpp11/wrong.h>

namespace sqlpp {
struct current_timestamp_name_t {
};

template<typename P>
struct current_timestamp_t : public statement_name_t<current_timestamp_name_t> {

  using _traits = make_traits<sqlpp::time_point, tag::is_expression>;
  current_timestamp_t(P prec) : _prec(prec)
  {
  }

  current_timestamp_t(const current_timestamp_t&) = default;
  current_timestamp_t(current_timestamp_t&&) = default;
  current_timestamp_t& operator=(const current_timestamp_t&) = default;
  current_timestamp_t& operator=(current_timestamp_t&&) = default;
  ~current_timestamp_t() = default;

  P _prec;
};

template<typename Context, typename P>
struct serializer_t<Context, current_timestamp_t<P>> {
  using _serialize_check = consistent_t;
  using T = current_timestamp_t<P>;

  static Context &_(const T &t, Context &context) {
    context << "CURRENT_TIMESTAMP(";
    context << t._prec;
    context << ")";
    return context;
  }
};

template<typename P>
auto current_timestamp(P prec) -> current_timestamp_t<P> {
  return {prec};
}
}  // namespace sqlpp

