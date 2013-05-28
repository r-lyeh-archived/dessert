/* Lightweight and simple C++11 test framework. No dependencies.
 * Copyright (c) 2012,2013 Mario 'rlyeh' Rodriguez

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
 * - Memory tests.

 * - rlyeh ~~ listening to Led Zeppelin / No Quarter
 */

#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <cstdio>

#include "petitsuite.hpp"

namespace petitsuite
{
    namespace
    {
        size_t _passed = 0, _failed = 0, _executed = 0;

        void default_on_report( const std::string &right, const std::string &wrong, const std::string &footer ) {
            // We are using fprintf(stderr...) instead of std::cerr just because global C++ objects might
            // get destroyed before we reach this function (as report is usually shown after main() has ended)
            if(  right.size() ) fprintf( stderr, "%s\n",  right.c_str() );
            if(  wrong.size() ) fprintf( stderr, "%s\n",  wrong.c_str() );
            if( footer.size() ) fprintf( stderr, "%s\n", footer.c_str() );
        }

        struct on_shutdown
        {
            std::string right, wrong;
            std::set< void(*)(void) > autos;
            std::set< void(*)(void) > units;

            std::string footer() const {
                std::stringstream ss;
                if( !_failed ) return ss << "Ok! All " << _executed << " tests succeeded! :)", ss.str();
                else           return ss << "Oops! " << _failed << '/' << _executed << " tests failed! :(", ss.str();
            }

            ~on_shutdown() {
                for( auto test = autos.begin(), end = autos.end(); test != end; ++test )
                    (**test)();

                if( _executed && on_report ) (*on_report)( right, wrong, footer() );
            }
        };

        on_shutdown &get() {
            static on_shutdown _;
            return _;
        }
    }

    namespace detail
    {
        void queue_test( const std::string &results, bool ok ) {
            if( ok ) _executed++, _passed++, get().right += results + '\n';
            else     _executed++, _failed++, get().wrong += results + '\n';

            if( on_warning ) (*on_warning)( results );
        }

        bool queue_unit( void (*N)(void) ) {
            return ( get().units.insert(N), true );
        }

        bool queue_auto( void (*N)(void), bool exec_now ) {
            return ( exec_now ? ( N(), true ) : ( get().autos.insert(N), true ) );
        }
    }

    size_t executed() { return _executed; }
    size_t failed()   { return   _failed; }
    size_t passed()   { return   _passed; }

    std::string right()  { return get().right    + '\n'; }
    std::string wrong()  { return get().wrong    + '\n'; }
    std::string footer() { return get().footer() + '\n'; }

    void (*on_warning)( const std::string &message ) = 0;
    void (*on_report)( const std::string &right, const std::string &wrong, const std::string &footer ) = default_on_report;

    std::string units() {
        size_t logerrorlen = get().wrong.size();
        for( auto test = get().units.begin(), end = get().units.end(); test != end; ++test )
            (**test)();
        return get().wrong.substr( logerrorlen );
    }
}
