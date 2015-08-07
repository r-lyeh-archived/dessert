dessert :cake: <a href="https://travis-ci.org/r-lyeh/dessert"><img src="https://api.travis-ci.org/r-lyeh/dessert.svg?branch=master" align="right" /></a>
=======

- Dessert is a lightweight unit-testing framework (C++11).
- Dessert is smart. LHS, RHS values are automatically outstreamed when tests fail.
- Dessert is tiny. 100 LOC. Barebone suite is only a single `dessert()` macro.
- Dessert is easy to integrate. Single file, header-only.
- Dessert is cross-platform. No extra dependencies.
- Dessert is handy. Optional breakpoints and interruption on selected tests.
- Dessert is zlib/libpng licensed.

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
- `dessert(expr) << optional << messages << here` macro expects given expression to be `true`.
  - if test passes, some info is printed, then expression returns `true`
  - if test fails, lhs, rhs (and optional messages) are printed, then expression returns `false`
- `desserts(title...) { /*code...*/ }` suite of dessert tests.
  - if `before main()` is found on `title`, then suite is executed before `main()` is run.
  - else suite is executed after `main()` is run (default behaviour).
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

desserts( "Tests before main()" ) {
    if( dessert( 1 < 2 ) ) {
        // ok
    } else {
        // handle error here
    }
}

desserts( "Tests after main()" ) {
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

------  Tests before main()
[ OK ]  Test 1 at sample.cc:38 (0 s)
------  Suite
[ OK ]  Test 2 at sample.cc:7 (0 s)
[FAIL]  Test 3 at sample.cc:8 (0 s)
        a > b
        1 > 2
        (unexpected)
[ OK ]  Test 4 at sample.cc:13 (0 s)
[ OK ]  Test 5 at sample.cc:14 (0 s)
[ OK ]  Test 6 at sample.cc:15 (0 s)
[ OK ]  Test 7 at sample.cc:16 (0 s)
[ OK ]  Test 8 at sample.cc:18 (0 s)
[ OK ]  Test 9 at sample.cc:19 (0.001 s)
[ OK ]  Test 10 at sample.cc:20 (0 s)
[ OK ]  Test 11 at sample.cc:21 (0 s)
[FAIL]  Test 12 at sample.cc:23 (0 s) - this shall fail
        "hell0" != "hell0"
        hell0 != hell0
        (unexpected)
------  Suite
[ OK ]  Test 13 at sample.cc:27 (0 s)
[FAIL]  Test 14 at sample.cc:28 (0 s) - test shall fail; phone Aristotle (+30 23760) if this test fails
        1 > 2
        (unexpected)
------  Suite
[ OK ]  Test 15 at sample.cc:33 (0 s)
[FAIL]  Test 16 at sample.cc:34 (0 s) - test shall fail
        once()
        0
        (unexpected)
------  Tests after main()
[ OK ]  Test 17 at sample.cc:49 (0.001 s)
[FAIL]  Test 18 at sample.cc:54 (0 s) - test shall fail, no exception thrown
        throws( std::string hello = "world"; hello += hello; )
        0
        (unexpected)

[FAIL]  Failure! 5/18 tests failed :(
        Breakpoints: 0 (*)
        Total time: 0.237 seconds.

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

## changelog
- v1.0.1 (2015/08/07)
  - fixed `<=` expression
- v1.0.0 (2015/05/22)
  - semantic versioning adherence
  - cosmetic fixes
- v0.0.0 (2013/xx/xx)
  - initial commits
