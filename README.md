petitsuite
==========

- Petitsuite is a lightweight and simple test C++11 framework.
- Tiny. Less than 100 LOC. Barebone suite is only a single `test()` macro.
- Smart. LHS, RHS values are reported when tests fail.
- Easy to integrate. Single file, header only.
- Cross-platform. No extra dependencies.
- Optional breakpoints on tests.
- MIT licensed.

Sample
------

```c++
#include <string>
#include "petitsuite.hpp"

int main() {
    int a = 1, b = 2;

    test( a < b );
    test( a < b ) << "this shall pass; comment built on " << __TIME__ << " " << __DATE__;
    test( a > b ) << "please call Aristotle (phone no: +30 " << 23760 << ") if this test fails";

    test( throws(
        std::string hello = "world";
        hello.at(10) = 'c';
    ) ) << "test shall pass, exception thrown";

    test( throws(
        std::string hello = "world";
        hello += hello;
    ) ) << "test shall fail, no exception thrown";
}

autotest(before) {            // auto test that runs *before* main()
    test( 1 > 2 ) << "this shall fail";
    test( 1 + 1 ) << "this shall pass";
}

autotest(after) {             // auto test that runs *after* main()
    test( 1 + 1 ) << "this shall pass";
}
```

API
---
- `test(...)` macro expect given expression to be `true`
- `throws( [code...] ) )` macro expects `[code...]` to throw any exception.
- `autotest(before) { [code...] }` tests are executed before `main()`
- `autotest(after) { [code...] }` tests are executed after `main()`

Breakpoints
-----------
- Invoke debugger at runtime by setting `BREAKON` environment variable to the specified test number.

Possible output
---------------

```
~/petitsuite> g++ sample.c --std=c++11
[FAIL]  Test 1 at sample.cc:23 (0.000000 s) - this shall fail
        1 > 2
        (unexpected)
[ OK ]  Test 2 at sample.cc:24 (0.000000 s) - this shall pass
[ OK ]  Test 3 at sample.cc:7 (0.000000 s)
[ OK ]  Test 4 at sample.cc:8 (0.000000 s) - this shall pass; comment built on 17:08:28 Apr 22 2014
[FAIL]  Test 5 at sample.cc:9 (0.000000 s) - please call Aristotle (phone no: +30 23760) if this test fails
        a > b
        1 > 2
        (unexpected)
[ OK ]  Test 6 at sample.cc:14 (0.000000 s) - test shall pass, exception thrown
[FAIL]  Test 7 at sample.cc:19 (0.000000 s) - test shall fail, no exception thrown
        throws( std::string hello = "world"; hello += hello; )
        0
        (unexpected)
[ OK ]  Test 8 at sample.cc:28 (0.000000 s) - this shall pass

Breakpoints: 0 (*)
Total time: 0.002000 seconds.
Failure: 3/8 tests failed :(

~/petitsuite>
~/petitsuite> export BREAKON=5
~/petitsuite> gdb -ex run --args ./a.out
<petitsuite/petitsuite.hpp> says: breaking on test #5
        Assertion failed: ! "<petitsuite/petitsuite.hpp> says: breakpoint requested", file c:\prj\dbg\petitsuite\petitsuite.hpp, line 62
Aborted.
Program received signal SIGABRT, Aborted.
0x00007fff9447d866 in petit::suite::break (62)
(gdb) where
#0  0x00007fff9447d866 in petit::suite::break (62)
#1  0x00007fff9229835c in main (12)
```
