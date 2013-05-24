/* Simple test framework. No dependencies. MIT licensed.
 * - rlyeh, 2012-2013
 */

#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <cstdio>

namespace petitsuite
{
    size_t passed = 0, failed = 0, executed = 0;

    void default_on_warning( const std::string &message )
    {}

    void default_on_report( const std::string &right, const std::string &wrong, const std::string &footer ) {
        // We could use iostreams in here (std::cerr). However, we are using fprintf()
        // because global C++ objects like std::cerr might get destroyed before
        // we reach this function (report is usually shown after main() has finished).

        if( right.size() )
            fprintf( stderr, "%s\n", right.c_str() );

        if( wrong.size() )
            fprintf( stderr, "%s\n", wrong.c_str() );

        if( footer.size() )
            fprintf( stderr, "%s\n", footer.c_str() );
    }

    void (*on_warning)( const std::string &message ) = default_on_warning;
    void (*on_report)( const std::string &right, const std::string &wrong, const std::string &footer ) = default_on_report;

    namespace
    {
        struct on_shutdown
        {
            std::string passed, errors;
            std::set< void(*)(void) > autos;
            std::set< void(*)(void) > units;

            ~on_shutdown() {
                for( auto test = autos.begin(), end = autos.end(); test != end; ++test )
                    (**test)();

                if( !executed )
                    return;

                if( !on_report )
                    return;

                std::stringstream footer, right, wrong;

                right << passed;
                wrong << errors;

                if( !failed )
                    footer << "Ok! All " << executed << " tests succeeded! :)" << std::endl;
                else
                    footer << "Oops! " << failed << '/' << executed << " tests failed! :(" << std::endl;

                (*on_report)( right.str(), wrong.str(), footer.str() );
            }
        };

        on_shutdown &get() {
            static on_shutdown _;
            return _;
        }
    }

    bool run() {
        size_t _failed = failed;
        for( auto test = get().units.begin(), end = get().units.end(); test != end; ++test )
            (**test)();
        return _failed == failed;
    }

    namespace detail
    {
        void attach( const std::string &results, bool ok ) {
            if( ok ) executed++, passed++, get().passed += results + '\n';
            else     executed++, failed++, get().errors += results + '\n';

            if( (!ok) && on_warning )
                (*on_warning)( results );
        }

        bool queue( void (*N)(void), bool is_unittest, bool exec_now ) {
            if( !is_unittest )
                return exec_now ? ( N(), true ) : ( get().autos.insert(N), true );
            return get().units.insert(N), true;
        }
    }
}
