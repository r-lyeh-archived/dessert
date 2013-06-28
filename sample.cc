#include <string>
#include "petitsuite.hpp"

unittest("basic tests")
{
    int a = 1, b = 2;

    // testN(...) macros expect given expression(s) to be true
    // N matches number of provided arguments
    test1( a < b );
    test2( a < b, "this shall pass; comment built on " << __DATE__ );
    test3( a,<,b );
    test4( a,>,b, "please call Aristotle (phone no: +30 " << 23760 << ") if this test fails" );

    // missN(...) macros expect given expression(s) to be false
    // N matches number of provided arguments
    miss1( a >= b );
    miss2( a >= b, "this shall pass; comment built on " << __DATE__ );
    miss3( a,>=,b );
    miss4( a,>=,b, "false positive; it's ok" );

    // testthrow(/*code*/) to test exception throwing
    testthrow(
        // this shall pass, exception thrown
        std::string hello = "world";
        hello.at(10) = 'c';
    );
    testthrow(
        // this shall fail, no exception thrown
        std::string hello = "world";
        hello += hello;
    );

    // testcatch(/*code*/) to test exception catching
    testcatch(
        // this shall pass, exceptions catched
        try {
            std::string hello = "world";
            hello.at(10) = 'c';
        } catch(...) {}
    );
    testcatch(
        // this shall fail, no exceptions catched
        std::string hello = "world";
        hello.at(10) = 'c';
    );
}

unittest() {                  // unittest description in parentheses is optional
    test3( 1,==,2 );          // this shall fail
}

int main() {
    // run all defined unit-tests above.
    // auto-tests below do not need this.
    petitsuite::units();
    return 0;
}

autotest(before) {            // auto test that runs *before* main()
    test3( 1, <, 2 );
}

autotest(before) {            // auto test that runs *before* main()
    test3( 1, <, 20 );
}

autotest(after) {             // auto test that runs *after* main()
    test3( 1, <, 2 );
}
