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
- `autotest(name) { /*code*/ }`'s are always performed at end of program.

Sample
------
<pre>
#include "petitsuite.hpp"

int main() {
    int a = 1, b = 2;

    test1( 1 == 1 );    // pass
    test1( 1 &lt;= 0 );    // fail

    test3( a, &lt;, b );   // pass
    test3( a, &gt;, b );   // fail
    test3( 1, !=, b);   // pass

    return 0;
}

autotest(equality_test) {
    test3( 2, ==, 2 );  // pass
    test3( 2, !=, 2 );  // fail
}
</pre>

Possible output
---------------
<pre>
~/petitsuite>./test
[ OK ] Test 1 at sample.cc:6
[ OK ] Test 3 at sample.cc:9
[ OK ] Test 5 at sample.cc:11
[ OK ] Test 6 at sample.cc:17

[FAIL] Test 2 at sample.cc:7
        1 &lt;= 0
        false
[FAIL] Test 4 at sample.cc:10
        a &gt; b
        1 &gt; 2
        false
[FAIL] Test 7 at sample.cc:18
        2 != 2
        2 != 2
        false

Oops! 3/7 tests failed! :(
~/petitsuite>
</pre>
