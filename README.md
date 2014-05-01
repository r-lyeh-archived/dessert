dessert :cake:
=======

- Dessert is a lightweight and simple test C++11 framework.
- Tiny. Less than 100 LOC. Barebone suite is only a single `dessert()` macro.
- Smart. LHS, RHS values are printed when tests fail.
- Easy to integrate. Single file, header-only.
- Cross-platform. No extra dependencies.
- Optional breakpoints on tests.
- MIT licensed.

### Quick tutorial
Replace any `assert()` with `dessert()`. That's it. Piece of cake :cake:

```c++
#include "dessert.hpp"

int main() {
    int a = 1, b = 2;
    dessert( a < b );
    dessert( a > b );
}
```

### Possible output
```
[ OK ]  Test 1 at tiny.cc:5 (0.000000 s)
[FAIL]  Test 2 at tiny.cc:6 (0.000000 s)
        a > b
        1 > 2
        (unexpected)

[FAIL]  Failure! 1/2 tests failed :(
        Breakpoints: 0 (*)
        Total time: 0.001000 seconds.
```

### API
- `dessert(expr) << an << optional << title` macro expects given expression to be `true`.
  - returns `true` if test passes, else returns `false`
  - lhs, rhs values are shown and printed out.
- `desserts(an << optional << title) { /*code...*/ }` suite of dessert tests.
  - if `before main()` is found on `title`, then suite is executed before main() is run.
- `throws( /*code...*/ )` macro expects `[code...]` to throw any exception.
  - returns `true` if code throws any exception, else returns `false`

### Breakpoints
- Invoke debugger at runtime by setting `BREAKON` environment variable to the specified test number.

### Sample usage
```c++
~/dessert>
~/dessert> cat sample.cc

#include <string>
#include "dessert.hpp"

desserts() {
    int a = 1, b = 2;
    dessert( a < b );
    dessert( a > b );
}

desserts() {
    dessert( 1 < 2 ) << "test shall pass; comment built on " << __TIME__ << " " << __DATE__;
    dessert( 1 > 2 ) << "test shall fail; phone Aristotle (+30 " << 23760 << ") if this test fails";
}

desserts( "that run before main() #" << 1 ) {
    if( dessert( 1 < 2 ) ) {
        // ok
    } else {
        // handle error here
    }
}

desserts( "that run after main() #" << 2 ) {
    dessert( throws(
            std::string hello = "world";
            hello.at(10) = 'c';
    ) ) << "test shall pass, exception thrown";

    dessert( throws(
            std::string hello = "world";
            hello += hello;
    ) ) << "test shall fail, no exception thrown";
}

int main()
{}

~/dessert>
~/dessert> g++ sample.cc --std=c++11 && ./a.out

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

[FAIL]  Failure! 3/15 desserts failed :(
        Breakpoints: 0 (*)
        Total time: 0.006000 seconds.

~/dessert>
~/dessert> export BREAKON=5
~/dessert> gdb -ex run --args ./a.out

<dessert/dessert.hpp> says: breaking on test #5
        Assertion failed: ! "<dessert/dessert.hpp> says: breakpoint requested", file dessert.hpp, line 20
Aborted.
Program received signal SIGABRT, Aborted.
0x00007fff9447d866 in dessert::suite::break (20)

(gdb) where
#0  0x00007fff9447d866 in dessert::suite::break (20)
#1  0x00007fff9229835c in main (12)
```
