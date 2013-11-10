#include <string>
#include "petitsuite.hpp"

int main() {
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

    // petitsuite::units() runs batch of all *unit*tests (defined below).
    // however, *auto*tests do not need this (also defined below).
    petitsuite::units();

    // we are done. logs will be printed to stdout when app finishes.
    // to change this behaviour point on_report/on_warning callbacks to your own.
    return 0;
}

unittest("basic tests") {     // unit test that runs on petitsuite::units()
    test3( 1,==,1 );          // this shall pass
}
unittest() {                  // unittest description in parentheses is optional
    test3( 1,>,2 );           // this shall fail
}

const char *hello = "world";

autotest(after) {             // auto test that runs *after* main()
    miss1( hello );           // this shall fail
}
autotest(before) {            // auto test that runs *before* main()
    test3( 1, <, 2 );
}
