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

#ifndef SQLPP11_DATA_TYPES_TEXT_MATCH_H
#define SQLPP11_DATA_TYPES_TEXT_MATCH_H

#include <sqlpp11/expression_operators.h>
#include <sqlpp11/alias_operators.h>
#include <sqlpp11/type_traits.h>
#include <sqlpp11/char_sequence.h>
#include <sstream>

namespace sqlpp
{
  struct boolean;

  template <typename Column, typename Pattern>
  struct match_t : public expression_operators<match_t<Column, Pattern>, boolean>,
                  public alias_operators<match_t<Column, Pattern>>
  {
    using _traits = make_traits<boolean, tag::is_expression, tag::is_selectable>;
    using _nodes = detail::type_vector<Column, Pattern>;

    struct _alias_t
    {
      // workaround for msvc bug https://connect.microsoft.com/VisualStudio/Feedback/Details/2100550
      //      static constexpr const char _literal[] = "match_";
      static constexpr const char _literal[7] = "match_";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template <typename T>
      struct _member_t
      {
        T match;
        T& operator()()
        {
          return match;
        }
        const T& operator()() const
        {
          return match;
        }
      };
    };

    match_t(Column column, Pattern pattern) : _column(column), _pattern(pattern)
    {
    }

    match_t(const match_t&) = default;
    match_t(match_t&&) = default;
    match_t& operator=(const match_t&) = default;
    match_t& operator=(match_t&&) = default;
    ~match_t() = default;

    Column _column;
    Pattern _pattern;
  };

  template <typename Context, typename Column, typename Pattern>
  struct serializer_t<Context, match_t<Column, Pattern>>
  {
    using _serialize_check = serialize_check_of<Context, Column, Pattern>;
    using T = match_t<Column, Pattern>;

    static Context& _(const T& t, Context& context)
    {
      context << "MATCH(";
      serialize_operand(t._column, context);
      context << ") AGAINST (";
      serialize(t._pattern, context);
      context << " IN NATURAL LANGUAGE MODE)";
      return context;
    }
  };
}  // namespace sqlpp

#endif
