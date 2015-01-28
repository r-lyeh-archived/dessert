/* Lightweight unit-testing C++11 framework. BOOST licensed.
 * - rlyeh ~~ listening to Led Zeppelin / No Quarter
 */

/* Public API */
#define dessert(...) ( bool(__VA_ARGS__) ? \
        ( dessert::suite(#__VA_ARGS__,__FILE__,__LINE__,1) < __VA_ARGS__ ) : \
        ( dessert::suite(#__VA_ARGS__,__FILE__,__LINE__,0) < __VA_ARGS__ ) )
#define desserts(...) \
        static void dessert$line(dessert)(); \
        static const bool dessert$line(dsstSuite_) = dessert::suite::queue( [&](){ \
            std::string title = "" __VA_ARGS__; if( title.empty() ) title = "Suite"; \
            fprintf( stderr, "------  %s\n", title.c_str() ); \
            dessert$line(dessert)(); \
            }, "" #__VA_ARGS__ ); \
        void dessert$line(dessert)()
#define throws(...) ( [&](){ try { __VA_ARGS__; } catch( ... ) { return true; } return false; }() )

/* API details following */
#pragma once
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <deque>
#include <functional>
#include <string>
#include <sstream>
namespace dessert {
    using namespace std;
    using timer = chrono::high_resolution_clock;
    template<typename T> inline string to_str( const T &t ) { stringstream ss; return (ss << t) ? ss.str() : "??"; }
    template<          > inline string to_str( const timer::time_point &start ) {
        return to_str( double((timer::now() - start).count()) * timer::period::num / timer::period::den );
    }
    class suite {
        timer::time_point start = timer::now();
        deque< string > xpr;
        int ok = false, has_bp = false;
        enum { BREAKPOINT, BREAKPOINTS, PASSED, FAILED, TESTNO };
        static unsigned &get(int i) { static unsigned var[TESTNO+1] = {}; return var[i]; }
    public:
        static bool queue( const function<void()> &fn, const string &text ) {
            static auto start = timer::now();
            static struct install : public deque<function<void()>> {
                install() : deque<function<void()>>() {
                    get(BREAKPOINT) = stoul( getenv("BREAKON") ? getenv("BREAKON") : "0" );
                }
                ~install() {
                    for( auto &fn : *this ) fn();
                    string ss, run = to_str( get(PASSED)+get(FAILED) ), res = get(FAILED) ? "[FAIL]  " : "[ OK ]  ";
                    if( get(FAILED) ) ss += res + "Failure! " + to_str(get(FAILED)) + '/'+ run + " tests failed :(\n";
                    else              ss += res + "Success: " + run + " tests passed :)\n";
                    ss += "        Breakpoints: " + to_str( get(BREAKPOINTS) ) + " (*)\n";
                    ss += "        Total time: " + to_str(start) + " seconds.\n";
                    fprintf( stderr, "\n%s", ss.c_str() );
                }
            } queue;
            return text.find("before main()") == string::npos ? ( queue.push_back( fn ), 0 ) : ( fn(), 1 );
        }
        suite( const char *const text, const char *const file, int line, bool result )
        :   xpr( {string(file) + ':' + to_str(line), " - ", text, "" } ), ok(result) {
            xpr[0] = "Test " + to_str(++get(TESTNO)) + " at " + xpr[0];
            if( 0 != ( has_bp = ( get(TESTNO) == get(BREAKPOINT) )) ) {
                get(BREAKPOINTS)++;
                fprintf( stderr, "<dessert/dessert.hpp> says: breaking on test #%d\n\t", get(TESTNO) );
                    assert(! "<dessert/dessert.hpp> says: breakpoint requested" );
                fprintf( stderr, "%s", "\n<dessert/dessert.hpp> says: breakpoint failed!\n" );
            };
        }
        ~suite() {
            if( xpr.empty() ) return;
            operator bool(), queue( [&](){ get(ok ? PASSED : FAILED)++; }, "before main()" );
            string res[] = { "[FAIL]", "[ OK ]" }, bp[] = { "  ", " *" }, tab[] = { "        ", "" };
            xpr[0] = res[ok] + bp[has_bp] + xpr[0] + " (" + to_str(start) + " s)" + (xpr[1].size() > 3 && !ok ? xpr[1] : tab[1]);
            xpr.erase( xpr.begin() + 1 );
            if( ok ) xpr = { xpr[0] }; else {
                xpr[2] = xpr[2].substr( xpr[2][2] == ' ' ? 3 : 4 );
                xpr[1].resize( (xpr[1] != xpr[2]) * xpr[1].size() );
                xpr.push_back( "(unexpected)" );
            }
            for( unsigned it = 0; it < xpr.size(); ++it ) {
             if(xpr[it].size()) fprintf( stderr, "%s%s\n", tab[ !it ].c_str(), xpr[it].c_str() );
        } }
#       define dessert$join(str, num) str##num
#       define dessert$glue(str, num) dessert$join(str, num)
#       define dessert$line(str)      dessert$glue(str, __LINE__)
#       define dessert$(OP) \
        template<typename T> suite &operator OP( const T &rhs         ) { return xpr[3] += " "#OP" " + to_str(rhs), *this; } \
        template<unsigned N> suite &operator OP( const char (&rhs)[N] ) { return xpr[3] += " "#OP" " + to_str(rhs), *this; }
        template<typename T> suite &operator <<( const T &t           ) { return xpr[1] += to_str(t),               *this; }
        template<unsigned N> suite &operator <<( const char (&str)[N] ) { return xpr[1] += to_str(str),             *this; }
        operator bool() {
            return xpr.size() >= 3 && xpr[3].size() >= 6 && [&]() -> bool {
                char sign = xpr[3].at(xpr[3].size()/2+1);
                bool equal = xpr[3].substr( 4 + xpr[3].size()/2 ) == xpr[3].substr( 3, xpr[3].size()/2 - 3 );
                return ok = ( sign == '=' ? equal : ( sign == '!' ? !equal : ok ) );
            }(), ok;
        }
        dessert$(<); dessert$(<=); dessert$(>); dessert$(>=); dessert$(!=); dessert$(==); dessert$(&&); dessert$(||);
    };
}
