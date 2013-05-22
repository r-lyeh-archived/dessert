/*
 * A lightweight and simple test framework with no dependencies. Requires C++11.
 * Copyright (c) 2012-2013, Mario 'rlyeh' Rodriguez

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.

 * @todo:
 * - Mockups.
 * - Suites and chain tests.
 * - Benchmark tests.
 * - Throw tests.
 * - Memory tests.

 * - rlyeh ~~ listening to Led Zeppelin / No Quarter
 */

#pragma once

#include <sstream>
#include <string>

namespace petitsuite
{
    /* public API */
    // optional read-only stats
    extern size_t passed, failed, executed;

    // optional overridable callbacks: ie, petitsuite::on_warning = my_callback_fn;
    extern void (*on_warning)( const std::string &message );
    extern void (*on_report)( const std::string &right, const std::string &wrong, const std::string &footer );

    // optional macro
#   define autotest(after) \
        static void autotest$line(n)(); \
        const bool autotest$line(autotest_) = petitsuite::detail::queue( autotest$line(n), std::string(#after) != "after" ); \
        void autotest$line(n)()

    // optional macro
#   define testexception(...) do { try { { __VA_ARGS__; } { \
            struct custom : public std::string { \
                custom(const char *expr_ = "") : std::string(expr_) {} \
                bool operator>>( const custom &other ) { return false; }}; \
            custom given_expression("{ " #__VA_ARGS__ " }"), does_not_throw("produces no exception"); \
            test3( given_expression, >>, does_not_throw ); } \
        } catch(...) { test1( true ); } } while(0)

    // main macro
#   define test3(A,op,B) do { \
        std::stringstream _SS_, _TT_; \
        auto _A_ = (A); decltype(_A_) _B_ = decltype(_A_)(B); auto _OK_ = ( _A_ op _B_ ); \
        _SS_ << ( _OK_ ? "[ OK ]" : "[FAIL]" ) << " Test " << (1+petitsuite::executed); \
        _SS_ << " at " __FILE__ ":" << __LINE__; if( !_OK_ ) { \
         if( std::string("decltype(" #A ")(0)") != #B ) { \
            const char *_EXPR_ = "\n\t" #A " " #op " " #B; \
            _TT_ << "\n\t" << _A_ << " " #op " " << _B_; \
            _SS_ << ( _TT_.str() != _EXPR_ ? _EXPR_ : "" ) << _TT_.str(); \
         } else _SS_ << "\n\t" #A; \
         _SS_ << "\n\t" << ( _OK_ ? "(true)" : "(false)" ); } \
        petitsuite::detail::attach( _SS_.str(), _OK_ ); \
    } while(0)

    // main macro
#   define test1(A) test3(A,!=,decltype(A)(0))

    /* private API */
    namespace detail {
    extern void attach( const std::string &results, bool ok );
    extern bool queue( void (*fn)(void), bool run );
#   define autotest$impl(str, num) str##num
#   define autotest$join(str, num) autotest$impl(str, num)
#   define autotest$line(str)      autotest$join(str, __LINE__)
    }
}
