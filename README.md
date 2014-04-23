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

tests( that run before main() ) {
    test( 1 < 2 ); // test shall pass
}

tests( that run after main() ) {
    int a = 1, b = 2;
    test( a < b ) << "this shall pass; comment built on " << __TIME__ << " " << __DATE__;
    test( a > b ) << "this shall fail; phone Aristotle (+30 " << 23760 << ") if this test fails";
}

int main() {
    test( 1 + 1 );

    test( throws(
        std::string hello = "world";
        hello.at(10) = 'c';
    ) ) << "test shall pass, exception thrown";

    test( throws(
        std::string hello = "world";
        hello += hello;
    ) ) << "test shall fail, no exception thrown";
}
```

API
---
- `test(...)` macro expects given expression to be `true`.
  - returns `true` if test passes, else returns `false`
  - any `<< message` is appended to print output.
  - lhs, rhs values are shown and printed out.
- `tests(description) { /*code...*/ }` suite of tests.
  - if `before main()` is found on `description`, then suite is executed before program entrypoint.
- `throws( /*code...*/ )` macro expects `[code...]` to throw any exception.
  - returns `true` if code throws any exception, else returns `false`

Breakpoints
-----------
- Invoke debugger at runtime by setting `BREAKON` environment variable to the specified test number.

Possible output
---------------

```
~/petitsuite> g++ sample.cc --std=c++11 && ./a.out
[ OK ]  Test 1 at sample.cc:4 (0.000000 s) - start of suite: that run before main()
[ OK ]  Test 2 at sample.cc:5 (0.000000 s)
[ OK ]  Test 3 at sample.cc:4 (0.000000 s) - end of suite: that run before main()
[ OK ]  Test 4 at sample.cc:15 (0.000000 s)
[ OK ]  Test 5 at sample.cc:20 (0.000000 s) - test shall pass, exception thrown
[FAIL]  Test 6 at sample.cc:25 (0.000000 s) - test shall fail, no exception thrown
        throws( std::string hello = "world"; hello += hello; )
        0
        (unexpected)
[ OK ]  Test 7 at sample.cc:8 (0.000000 s) - start of suite: that run after main()
[ OK ]  Test 8 at sample.cc:10 (0.000000 s) - this shall pass; comment built on 21:01:40 Apr 23 2014
[FAIL]  Test 9 at sample.cc:11 (0.000000 s) - this shall fail; phone Aristotle (+30 23760) if this test fails
        a > b
        1 > 2
        (unexpected)
[ OK ]  Test 10 at sample.cc:8 (0.000000 s) - end of suite: that run after main()

Breakpoints: 0 (*)
Total time: 0.001000 seconds.
Failure: 2/10 tests failed :(

~/petitsuite>
~/petitsuite> export BREAKON=5
~/petitsuite> gdb -ex run --args ./a.out
<petitsuite/petitsuite.hpp> says: breaking on test #5
        Assertion failed: ! "<petitsuite/petitsuite.hpp> says: breakpoint requested", file petitsuite.hpp, line 20
Aborted.
Program received signal SIGABRT, Aborted.
0x00007fff9447d866 in petit::suite::break (20)
(gdb) where
#0  0x00007fff9447d866 in petit::suite::break (20)
#1  0x00007fff9229835c in main (12)
```
