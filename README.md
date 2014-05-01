petitsuite
==========

- Petitsuite is a lightweight and simple test C++11 framework.
- Tiny. Less than 100 LOC. Barebone suite is only a single `test()` macro.
- Smart. LHS, RHS values are reported when tests fail.
- Easy to integrate. Single file, header only.
- Cross-platform. No extra dependencies.
- Optional breakpoints on tests.
- MIT licensed.

### Quick tutorial
Replace any `assert()` with `test()`. That's it.

```c++
#include "petitsuite.hpp"

int main() {
    int a = 1, b = 2;
    test( a < b );
    test( a > b );
}
```

### API
- `test(expr) << an << optional << title` macro expects given expression to be `true`.
  - returns `true` if test passes, else returns `false`
  - lhs, rhs values are shown and printed out.
- `tests(an << optional << title) { /*code...*/ }` suite of tests.
  - if `before main()` is found on `title`, then suite is executed before main() is run.
- `throws( /*code...*/ )` macro expects `[code...]` to throw any exception.
  - returns `true` if code throws any exception, else returns `false`

### Breakpoints
- Invoke debugger at runtime by setting `BREAKON` environment variable to the specified test number.

### Full sample
```c++
#include <string>
#include "petitsuite.hpp"

tests() {
    int a = 1, b = 2;
    test( a < b );
    test( a > b );
}

tests() {
    test( 1 < 2 ) << "test shall pass; comment built on " << __TIME__ << " " << __DATE__;
    test( 1 > 2 ) << "test shall fail; phone Aristotle (+30 " << 23760 << ") if this test fails";
}

tests( "that run before main() #" << 1 ) {
    if( test( 1 < 2 ) ) {
        // ok
    } else {
        // handle error here
    }
}

tests( "that run after main() #" << 2 ) {
    test( throws(
        std::string hello = "world";
        hello.at(10) = 'c';
    ) ) << "test shall pass, exception thrown";

    test( throws(
        std::string hello = "world";
        hello += hello;
    ) ) << "test shall fail, no exception thrown";
}

int main()
{}
```

### Possible output
```
~/petitsuite> g++ sample.cc --std=c++11 && ./a.out
[ OK ]  Test 1 at sample.cc:15 (0.000000 s) - start of suite: that run before main() #1
[ OK ]  Test 2 at sample.cc:16 (0.000000 s)
[ OK ]  Test 3 at sample.cc:15 (0.000000 s) - end of suite: that run before main() #1
[ OK ]  Test 4 at sample.cc:4 (0.000000 s) - start of suite:
[ OK ]  Test 5 at sample.cc:6 (0.000000 s)
[FAIL]  Test 6 at sample.cc:7 (0.000000 s)
        a > b
        1 > 2
        (unexpected)
[ OK ]  Test 7 at sample.cc:4 (0.000000 s) - end of suite:
[ OK ]  Test 8 at sample.cc:10 (0.000000 s) - start of suite:
[ OK ]  Test 9 at sample.cc:11 (0.000000 s) - test shall pass; comment built on 14:57:40 May  1 2014
[FAIL]  Test 10 at sample.cc:12 (0.000000 s) - test shall fail; phone Aristotle (+30 23760) if this test fails
        1 > 2
        (unexpected)
[ OK ]  Test 11 at sample.cc:10 (0.000000 s) - end of suite:
[ OK ]  Test 12 at sample.cc:23 (0.000000 s) - start of suite: that run after main() #2
[ OK ]  Test 13 at sample.cc:27 (0.000000 s) - test shall pass, exception thrown
[FAIL]  Test 14 at sample.cc:32 (0.000000 s) - test shall fail, no exception thrown
        throws( std::string hello = "world"; hello += hello; )
        0
        (unexpected)
[ OK ]  Test 15 at sample.cc:23 (0.000000 s) - end of suite: that run after main() #2

[FAIL]  Failure! 3/15 tests failed :(
        Breakpoints: 0 (*)
        Total time: 0.006000 seconds.

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
