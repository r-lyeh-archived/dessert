petitsuite
==========

- Lightweight and simple test C++11 framework.
- Easy to integrate. One header and one source file.
- Cross-platform. No extra dependencies.
- Tiny. Couple of macros.
- MIT licensed.

test1()
-------
- `test1(expression)` macro expects given `expression` to be true.
- `test1(expression)` behaves similar to `assert(expression)` but does not break execution on fails.
- `test1(expression)` will output verbosely all fails. Macro requires expression type to be `std::ostream<<` friendly.

test3()
-------
- `test3(expression1,operator,expression2)` macro expects given `expression1 operator expression2` to be true.
- `test3(expression1,operator,expression2)` behaves similar to `assert(expression1 operator expression2)` but does not break execution on fails.
- `test3(expression1,operator,expression2)` will output verbosely all fails. Macro requires both expression types to be `std::ostream<<` friendly.

testexception()
---------------
- `testexception( /*code*/ )` macro expects given `/*code*/` to throw any exception.
- `testexception( /*code*/ )` will output verbosely all fails.

autotest()
----------
- `autotest(before) { /*code*/ }`'s are always performed before start of program.
- `autotest(after) { /*code*/ }`'s are always performed after end of program.

unittest()
----------
- `unittest() { /*code*/ }`'s are always performed when `petitsuite::run()` is invoked.

Sample
------
```
#include "petitsuite.hpp"

int main() {
    int a = 1, b = 2;

    test1( 1 == 1 );    // pass
    test1( 1 <= 0 );    // fail

    test3( 1, <, 2 );   // pass
    test3( a, >, b );   // fail
    test3( 1, !=, b);   // pass

    testexception(      // fail, no exception is thrown
        int a, b, c = 100;
        std::string hello = "hello world";
    );
    testexception(      // pass, an exception is thrown
        throw int(-100);
    );

    petitsuite::run();  // run all optional unit tests

    return 0;
}

autotest(before) {            // auto test that runs before main()
    test3( true, ==, false ); // fail
}

autotest(after) {             // auto test that runs after main()
    test3( true, >, false );  // pass
}

unittest() {                  // unit test that runs everytime petitsuite::run() is invoked
    test1( 2 + 2 == 4 );      // pass
}

unittest() {                  // unit test that runs everytime petitsuite::run() is invoked
    test3( 2 + 2, >, 4 );     // fail
}
```

Possible output
---------------
```
~/petitsuite>./test
[ OK ] Test 2 at sample.cc:6
[ OK ] Test 4 at sample.cc:9
[ OK ] Test 6 at sample.cc:11
[ OK ] Test 8 at sample.cc:19
[ OK ] Test 9 at sample.cc:35
[ OK ] Test 11 at sample.cc:31

[FAIL] Test 1 at sample.cc:27
        true == false
        1 == 0
        (false)
[FAIL] Test 3 at sample.cc:7
        1 <= 0
        (false)
[FAIL] Test 5 at sample.cc:10
        a > b
        1 > 2
        (false)
[FAIL] Test 7 at sample.cc:16
        given_expression >> does_not_throw
        { int a, b, c = 100; std::string hello = "hello world"; } >> produces no exception
        (false)
[FAIL] Test 10 at sample.cc:39
        2 + 2 > 4
        4 > 4
        (false)

Oops! 5/11 tests failed! :(
~/petitsuite>
```
