petitsuite
==========

- Lightweight and simple test C++11 framework.
- Easy to integrate. One header and one source file.
- Cross-platform. No extra dependencies.
- Tiny. Couple of macros.
- MIT licensed.

test1()
-------
- `test1(expression)` macro expects given expression to be true.
- `test1(expression)` behaves similar to `assert(expression)` but does not break execution on fails.

test3()
-------
- `test3(variable1,operator,variable2)` expects given expression to be true.
- `test3(variable1,operator,variable2)` will output value of arguments when expression fails. It requires both variables to be `std::ostream<<` friendly.

autotest()
----------
- `autotest(name) { /*code*/ }`'s are always performed at end of program.

Sample
------
<pre>
#include "test.hpp"

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
[ OK ] Test 1: 1 == 1 is true (1) seen at test.sample.cpp:6
[ OK ] Test 3: a &lt; b (1 &lt; 2) seen at test.sample.cpp:9
[ OK ] Test 5: 1 != b (1 != 2) seen at test.sample.cpp:11
[ OK ] Test 6: 2 == 2 (2 == 2) seen at test.sample.cpp:17

[FAIL] Test 2: 1 &lt;= 0 is false (0) seen at test.sample.cpp:7
[FAIL] Test 4: a &gt; b (1 &gt; 2) seen at test.sample.cpp:10
[FAIL] Test 7: 2 != 2 (2 != 2) seen at test.sample.cpp:18
~/petitsuite>
</pre>
