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

autotest()
----------
- `autotest(before) { /*code*/ }`'s are always performed before start of program.
- `autotest(after) { /*code*/ }`'s are always performed after end of program.

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

    return 0;
}

autotest(before) {            // auto test that runs before main()
    test3( true, ==, false ); // fail
}

autotest(after) {             // auto test that runs after main()
    test3( true, >, false );  // pass
}
```

Possible output
---------------
```
~/petitsuite>./test
[ OK ] Test 2 at sample.cc:6
[ OK ] Test 4 at sample.cc:9
[ OK ] Test 6 at sample.cc:11
[ OK ] Test 7 at sample.cc:21

[FAIL] Test 1 at sample.cc:17
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

Oops! 3/7 tests failed! :(
~/petitsuite>
```
