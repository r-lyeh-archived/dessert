petitsuite
==========

- Petitsuite is a lightweight and simple test C++11 framework.
- Smart. Lhs, rhs values are reported when tests fail.
- Tiny. Around 200 LOC. Barebone suite is only a single `test4()` macro.
- Easy to integrate. One header and one source file.
- Cross-platform. No extra dependencies.
- Both automatic and batch-driven unit-testing.
- Optional user-defined callbacks.
- Optional breakpoints on tests.
- MIT licensed.

Tests and misses
----------------
- `test1(expression)` macro expects `expression` to be `true`.
- `miss1(expression)` macro expects `expression` to be `false`.
- `test2(expression,comment)` macro adds `comment`.
- `miss2(expression,comment)` macro adds `comment`.
  - `comment` is a `std::ostream` expression.
- `test3(expression1,operator,expression2)` macro expects `expression1 operator expression2` to be `true`.
- `miss3(expression1,operator,expression2)` macro expects `expression1 operator expression2` to be `false`.
- `test4(expression1,operator,expression2,comment)` macro adds `comment`.
- `miss4(expression1,operator,expression2,comment)` macro adds `comment`.
  - `comment` is a `std::ostream` expression.

Exceptions and catches
----------------------
- `testthrow( /*code*/ )` macro expects `/*code*/` to throw any exception.
- `testcatch( /*code*/ )` macro expects `/*code*/` to catch any exception.

Optional unit-tests
-------------------
- `unittest(description...) { /*code*/ }` defines a unit-test.
- `description` is optional.
- Call `petitsuite::units()` to run a batch of all defined unit-tests.

Optional auto-tests
-------------------
- `autotest(before) { /*code*/ }` tests are run automatically before `main()`.
- `autotest(after) { /*code*/ }` tests are run automatically after `main()`.
- Auto-tests, unlike unit-tests, do not need a `petitsuite::units()` call.

Optional breakpoints
--------------------
- `petitsuite::has_breakpoint(test_number)` to check if a test has a breakpoint or not.
- `petitsuite::set_breakpoint(test_number,on)` to toggle breakpoints on a test.
- Additionally, `PETITSUITE_BREAKON` environment variable can be set at runtime.

Optional stats
--------------
- `petitsuite::passed()` live number of passed tests
- `petitsuite::failed()` live number of failed tests
- `petitsuite::executed()` live number of executed tests

Optional logs
-------------
- `petitsuite::right()` live log of what is going right
- `petitsuite::wrong()` live log of what is going wrong
- `petitsuite::footer()` live brief footer

Optional callbacks
------------------
- `petitsuite::on_report(right,wrong,footer)` user-defined callback for whole log reporting.
- `petitsuite::on_warning(message)` user-defined callback for line reporting.
- `petitsuite::on_breakpoint(test_number)` user-defined callback for code breaking.

Sample
------
```c++
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
    test3( 1,==,1 );          // this shall pass
}

int main() {
    // petitsuite::units() runs batch of all unit-tests defined above.
    // however, auto-tests defined below do not need this.
    petitsuite::units();
    // we are done. logs will be printed to stdout when app finishes.
    // to change this behaviour point on_report/on_warning callbacks to your own.
    return 0;
}

autotest(before) {            // auto test that runs *before* main()
    test3( 1, <, 2 );
}

autotest(before) {            // auto test that runs *before* main()
    test3( 1, <, 20 );
}

const char *hello = "world";

autotest(after) {             // auto test that runs *after* main()
    miss1( hello );           // this shall fail
}
```

Possible output
---------------
```
~/petitsuite>g++ sample.cc petitsuite.cpp -std=c++11 -g -o test
~/petitsuite>./test
[ OK ]  Test 1 at sample.cc:63
[ OK ]  Test 2 at sample.cc:67
[ OK ]  Test 3 at sample.cc:10
[ OK ]  Test 4 at sample.cc:11
[ OK ]  Test 5 at sample.cc:12
[ OK ]  Test 7 at sample.cc:17
[ OK ]  Test 8 at sample.cc:18
[ OK ]  Test 9 at sample.cc:19
[ OK ]  Test 10 at sample.cc:20
[ OK ]  Test 11 at sample.cc:27
[ OK ]  Test 13 at sample.cc:41
[ OK ]  Test 15 at sample.cc:50

[FAIL]  Test 6 at sample.cc:13 - please call Aristotle (phone no: +30 23760) if this test fails
        a > b
        1 > 2
        false (true expected)
[FAIL]  Test 12 at sample.cc:32
        given_code >> throws_no_exception
        { std::string hello = "world"; hello += hello; } >> throws no exception
        false (true expected)
[FAIL]  Test 14 at sample.cc:46
        given_code >> does_not_catch_thrown_exception
        { std::string hello = "world"; hello.at(10) = 'c'; } >> does not catch thrown exception
        false (true expected)
[FAIL]  Test 16 at sample.cc:73
        hello
        world
        true (false expected)

Oops! 4/16 tests failed! :(
~/petitsuite>
~/petitsuite>export PETITSUITE_BREAKON=10
~/petitsuite>./test
<petitsuite/petitsuite.cpp> says: breakpoint requested on test #10
        Assertion failed: ! "<petitsuite/petitsuite.cpp> says: breakpoint requested", file petitsuite.cpp, line 61
Aborted.
```
