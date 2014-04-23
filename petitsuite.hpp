/* Lightweight and simple C++11 test framework. No dependencies.
 * Copyright (c) 2012,2013,2014 Mario 'rlyeh' Rodriguez

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

 * - rlyeh ~~ listening to Led Zeppelin / No Quarter
 */

#pragma once
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <deque>
#include <functional>
#include <string>

/* Public API */
#define throws(...)  ( [&](){ try { __VA_ARGS__; } catch( ... ) { return true; } return false; } () )
#define test(...)    ( petit::suite(#__VA_ARGS__,bool(__VA_ARGS__),__FILE__,__LINE__) < __VA_ARGS__ )
#define tests(...) \
        static void petit$line(ptSuite)(); \
        static const bool petit$line(ptSuite_) = petit::suite::queue( [&](){ \
            test(1) << "start of suite: " #__VA_ARGS__; petit$line(ptSuite)(); test(1) << "end of suite: " #__VA_ARGS__; \
            }, #__VA_ARGS__ ); \
        void petit$line(ptSuite)()

/* API Details */
namespace petit {
    class suite {
        int ok = false;
        bool has_bp = false;
        std::deque< std::string > expr;
        using timer = std::chrono::high_resolution_clock;
        std::chrono::high_resolution_clock::time_point start = timer::now();
        enum { BREAKPOINT, BREAKPOINTS, PASSED, FAILED, TESTNO };
        template<int VAR> static unsigned &get() { static unsigned var[16] = {0}; return var[VAR]; }
        static std::string time( std::chrono::high_resolution_clock::time_point start ) {
            return std::to_string( double((timer::now() - start).count()) * timer::period::num / timer::period::den );
        }
    public:
        static bool queue( const std::function<void()> &fn, const std::string &text ) {
            static auto start = timer::now();
            static struct install {
                std::deque< std::function<void()> > all;
                install() {
                    get<BREAKPOINT>() = std::stoul( std::getenv("BREAKON") ? std::getenv("BREAKON") : "0" );
                }
                ~install() {
                    for( auto &fn : all ) fn();
                    std::string ss, exec = std::to_string( get<PASSED>() + get<FAILED>() );
                    ss += "Breakpoints: " + std::to_string( get<BREAKPOINTS>() ) + " (*)\n";
                    ss += "Total time: " + time(start) + " seconds.\n";
                    if( !get<FAILED>() ) ss += "Success: " + exec + " tests passed :)\n";
                    else                 ss += "Failure: " + std::to_string( get<FAILED>() ) + '/' + exec + " tests failed :(\n";
                    std::fprintf( stderr, "\n%s", ss.c_str() );
                }
            } queue;
            return text.find("before main()") == std::string::npos ? ( queue.all.push_back( fn ), false ) : ( fn(), true );
        }
        suite( const char *const text, bool result, const char *const file, int line )
        :   ok(result), expr( {std::string(file) + ':' + std::to_string(line), " - ", text, "" } ) {

            expr[0] = "Test " + std::to_string(++get<TESTNO>()) + " at " + expr[0];
            queue( [&](){ if( result ) get<PASSED>()++; else get<FAILED>()++; }, "before main()" );

            has_bp = ( get<TESTNO>() == get<BREAKPOINT>() );
            if( has_bp ) {
                get<BREAKPOINTS>()++;
                std::fprintf( stderr, "<petitsuite/petitsuite.hpp> says: breaking on test #%d\n\t", get<TESTNO>() );
                    assert(! "<petitsuite/petitsuite.hpp> says: breakpoint requested" );
                std::fprintf( stderr, "%s", "\n<petitsuite/petitsuite.hpp> says: breakpoint failed!\n" );
            };
        }
        ~suite() {
            std::string res[] = { "[FAIL]", "[ OK ]" }, bp[] = { "  ", " *" }, tab[] = { "        ", "" };
            expr[0] = res[ok] + bp[has_bp] + expr[0] + " (" + time(start) + " s)";
            if( expr[1].size() > 3 ) expr[0] += expr[1];
            expr.erase( expr.begin() + 1 );
            if( !ok ) {
                expr[2] = expr[2].substr( expr[2][2] == ' ' ? 3 : 4 );
                if( expr[1] == expr[2] ) {
                    expr[1].clear();
                }
                expr.push_back( "(unexpected)" );
            } else expr = { expr[0] };
            for( auto begin = expr.begin(), end = expr.end(), it = begin; it != end; ++it ) {
                if( it->size() ) std::fprintf( stderr, "%s%s\n", tab[ it == begin ].c_str(), it->c_str() );
            }
        }

#       define petit$glue(str, num) str##num
#       define petit$join(str, num) petit$glue(str, num)
#       define petit$line(str)      petit$join(str, __LINE__)
#       define petit$impl(OP) \
        template<typename T> suite &operator OP( const T &rhs ) { return expr[3] += " "#OP" " + std::to_string(rhs), *this; }
        template<typename T> suite &operator <<( const T &t                ) { return expr[1] += std::to_string(t),  *this; }
        template< size_t N > suite &operator <<( const char (&string)[N]   ) { return expr[1] += string,             *this; }
        template<          > suite &operator <<( const std::string &string ) { return expr[1] += string,             *this; }
        operator bool() const { return ok; }
        petit$impl( <); petit$impl(<=);
        petit$impl( >); petit$impl(>=);
        petit$impl(!=); petit$impl(==);
        petit$impl(^=);
        petit$impl(&=); petit$impl(&&);
        petit$impl(|=); petit$impl(||);
    };
}
