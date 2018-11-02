/*
 * Copyright (c) 2018, Coloridad Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "compare.h"
#include "Sample.h"
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/alias_provider.h>

#include <iostream>

SQLPP_ALIAS_PROVIDER(delta_left)

int Functions(int, char*[])
{
  const auto tab = test::TabFoo{};

  compare(__LINE__, sqlpp::select(sqlpp::left(tab.delta, 8)).from(tab).unconditionally(),
          "SELECT LEFT(tab_foo.delta, 8) AS left_ FROM tab_foo");

  compare(__LINE__, sqlpp::select(sqlpp::left(tab.delta, 8).as(delta_left)).from(tab).unconditionally(),
          "SELECT LEFT(tab_foo.delta, 8) AS delta_left FROM tab_foo");

  const auto tab_time = test::TabDateTime{};
  compare(__LINE__, sqlpp::insert_into(tab_time).set(tab_time.colTimePoint = sqlpp::current_timestamp(6u)),
          "INSERT INTO tab_time_point (col_time_point) VALUES(CURRENT_TIMESTAMP(6))");
  return 0;
}
