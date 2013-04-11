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

 * - rlyeh
 */

#pragma once

#include <sstream>
#include <string>

/* public API */
namespace tests
{
    // optional read-only stats
    extern size_t passed, failed, executed;

    // optional overridable callbacks: ie, tests::on_warning = my_callback_fn;
    extern void (*on_warning)( const std::string &message );
    extern void (*on_report)( const std::string &right, const std::string &wrong, const std::string &footer );

    // optional macro
#   define autotest(n) \
        static void autotest$line(n)(); \
        const bool autotest$line(autotest_) = tests::detail::queue( autotest$line(n) ); \
        void autotest$line(n)()

    // main macro
#   define test1(A) do { \
        auto __a__ = (A); bool ok = __a__ != decltype(__a__)(0); \
        std::stringstream ss; \
        ss << (ok ? "[ OK ] " : "[FAIL] " ); \
        ss << "Test " << (tests::executed+1) << ": "; \
        ss << #A << ( ok ? " is true (" : " is false (" ) << __a__ << ") "; \
        ss << "seen at " << __FILE__ << ':' << __LINE__; \
        tests::detail::attach( ss.str(), ok ); \
    } while(0)

    // main macro
#   define test3(A,op,B) do { \
        auto __a__ = (A); auto __b__ = (B); bool ok = __a__ op __b__; \
        std::stringstream ss; \
        ss << (ok ? "[ OK ] " : "[FAIL] " ); \
        ss << "Test " << (tests::executed+1) << ": "; \
        ss << #A << ' ' << #op << ' ' << #B; \
        ss << " (" << __a__ << ' ' << #op << ' ' << __b__ << ") "; \
        ss << "seen at " << __FILE__ << ':' << __LINE__; \
        tests::detail::attach( ss.str(), ok ); \
    } while(0)
}

/* private API */
namespace tests
{
    namespace detail {
    extern void attach( const std::string &results, bool ok );
    extern bool queue( void (*fn)(void) );

#   define autotest$impl(str, num) str##num
#   define autotest$join(str, num) autotest$impl(str, num)
#   define autotest$line(str)      autotest$join(str, __LINE__)
    }
}

