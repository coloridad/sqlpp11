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

#ifndef SQLPP11_STRING_FUNCTIONS_LEFT_H
#define SQLPP11_STRING_FUNCTIONS_LEFT_H

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/data_types/text/data_type.h>
#include <sqlpp11/alias_operators.h>
#include <sqlpp11/serialize.h>
#include <sqlpp11/data_types/text/expression_operators.h>

namespace sqlpp
{
  struct left_alias_t
  {
    struct _alias_t
    {
      static constexpr const char _literal[] = "left_";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;

      template <typename T>
      struct _member_t
      {
        T left;
        T& operator()()
        {
          return left;
        }

        const T& operator()() const
        {
          return left;
        }
      };
    };
  };

  template <typename Expr>
  struct left_t : public expression_operators<left_t<Expr>, text>,
                  public alias_operators<left_t<Expr>>
  {
    using _traits = make_traits<text, tag::is_expression>;

    using _nodes = detail::type_vector<Expr, string_function>;
    using _can_be_null = std::true_type;

    using _auto_alias_t = left_alias_t;

    left_t(const Expr expr, unsigned int len) : _expr(expr), _len(len)
    {
    }

    left_t(const left_t&) = default;
    left_t(left_t&&) = default;
    left_t& operator=(const left_t&) = default;
    left_t& operator=(left_t&&) = default;
    ~left_t() = default;

    Expr _expr;
    unsigned int _len;
  };

  template <typename Context, typename Expr>
  struct serializer_t<Context, left_t<Expr>>
  {
    using _serialize_check = consistent_t;
    using T = left_t<Expr>;

    static Context& _(const T& t, Context& context)
    {
      context << "LEFT(";
      sqlpp::serialize(t._expr, context);
      context << ", " << t._len << ")";

      return context;
    }
  };

  template <typename T>
  auto left(T t, unsigned int len) -> left_t<T>
  {
    static_assert(is_expression_t<wrap_operand_t<T>>::value, "left() requires an expression as argument");
    static_assert(is_column_t<wrap_operand_t<T>>::value, "left() requires a column as argument");
    static_assert(is_text_t<wrap_operand_t<T>>::value, "left() requires a text column as argument");

    return {t, len};
  }
}

#endif //SQLPP11_STRING_FUNCTIONS_LEFT_H
