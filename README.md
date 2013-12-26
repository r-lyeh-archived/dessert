petitsuite
==========

- Petitsuite is a lightweight and simple test C++11 framework.
- Smart. LHS, RHS values are reported when tests fail.
- Tiny. Around 200 LOC. Barebone suite is only a single `test4()` macro.
- Easy to integrate. One header and one source file.
- Cross-platform. No extra dependencies.
- Both automatic and batch-driven unit-testing.
- Optional user-defined callbacks.
- Optional breakpoints on tests.
- MIT licensed.

Tests and misses
----------------
testN(...) macros expect given expressions to be true. (N matches number of provided arguments).
- `test1(expression)` macro expects `expression` to be `true`.
- `test2(expression,std::ostream comment...)` macro adds `comment`.
- `test3(expression1,operator,expression2)` macro expects `expression1 operator expression2` to be `true`.
- `test4(expression1,operator,expression2,std::ostream comment...)` macro adds `comment`.

missN(...) macros expect given expressions to be false. (N matches number of provided arguments).
- `miss1(expression)` macro expects `expression` to be `false`.
- `miss2(expression,std::ostream comment...)` macro adds `comment`.
- `miss3(expression1,operator,expression2)` macro expects `expression1 operator expression2` to be `false`.
- `miss4(expression1,operator,expression2,std::ostream comment...)` macro adds `comment`.

```c++
int main() {
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
}
```

Exceptions and catches
----------------------
- `testthrow( [code...] )` macro expects `[code...]` to throw any exception.
- `testcatch( [code...] )` macro expects `[code...]` to catch any exception.

```c++
int main() {
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
```

Auto-tests
----------
- `autotest(before) { [code...] }` tests are executed before `main()`
- `autotest(after) { [code...] }` tests are executed after `main()`

```c++
autotest(after) {             // auto test that runs *after* main()
    test3( 1,==,1 );          // this shall pass
}
autotest(before) {            // auto test that runs *before* main()
    test3( 1,>,2 );           // this shall fail
}
int main() {
}
```

Unit-tests
----------
- `unittest([description...]) { [code...] }` defines a unit-test
- Unit-tests, unlike auto-tests, require explicit `petitsuite::units()` call

```c++
unittest("basic tests") {     // unit test that runs on petitsuite::units()
    test3( 1,==,1 );          // this shall pass
}
unittest() {                  // unittest description in parentheses is optional
    test3( 1,>,2 );           // this shall fail
}
int main() {
    petitsuite::units();
}
```

Breakpoints
-----------
- `petitsuite::has_breakpoint(test_number)` to check if a test has a breakpoint or not
- `petitsuite::set_breakpoint(test_number,bool)` to toggle breakpoints on a test
- Additionally, `PETITSUITE_BREAKON` environment variable can be set at runtime

```c++
unittest("basic tests") {     // unit test that runs on petitsuite::units()
    test3( 1,==,1 );          // this shall pass
}
unittest() {                  // unittest description in parentheses is optional
    test3( 1,>,2 );           // this shall fail
}
int main() {
    petitsuite::set_breakpoint( 2, true );
    petitsuite::units();
}
```

Stats, logs and callbacks
-------------------------
- `petitsuite::passed()` live number of passed tests
- `petitsuite::failed()` live number of failed tests
- `petitsuite::executed()` live number of executed tests
- `petitsuite::right()` live log of what is going right
- `petitsuite::wrong()` live log of what is going wrong
- `petitsuite::footer()` live brief footer
- `petitsuite::on_report(right,wrong,footer)` user-defined callback for whole log reporting
- `petitsuite::on_warning(message)` user-defined callback for line reporting
- `petitsuite::on_breakpoint(test_number)` user-defined callback for code breaking

Possible output
---------------
```
~/petitsuite>g++ sample.cc petitsuite.cpp -std=c++11 -g -o tests
~/petitsuite>./tests
[ OK ]  Test 2 at sample.cc:9 (0.184018s)
[ OK ]  Test 3 at sample.cc:10 (0.192019s)
[ OK ]  Test 4 at sample.cc:11 (0.184018s)
[ OK ]  Test 6 at sample.cc:16 (0.168017s)
[ OK ]  Test 7 at sample.cc:17 (0.160016s)
[ OK ]  Test 8 at sample.cc:18 (0.192019s)
[ OK ]  Test 9 at sample.cc:19 (0.255026s)
[ OK ]  Test 10 at sample.cc:26 (0.248025s)
[ OK ]  Test 12 at sample.cc:40 (0.215022s)
[ OK ]  Test 14 at sample.cc:57 (0.167017s)
[ OK ]  Test 16 at sample.cc:64 (0.191019s)

[FAIL]  Test 1 at sample.cc:67 (0.415042s)
        1 > 2
        false (true expected)
[FAIL]  Test 5 at sample.cc:12 (0.152015s) - please call Aristotle (phone no: +30 23760) if this test fails
        a > b
        1 > 2
        false (true expected)
[FAIL]  Test 11 at sample.cc:31 (0.224022s)
        given_code >> throws_no_exception
        { std::string hello = "world"; hello += hello; } >> throws no exception
        false (true expected)
[FAIL]  Test 13 at sample.cc:45 (0.184018s)
        given_code >> does_not_catch_thrown_exception
        { std::string hello = "world"; hello.at(10) = 'c'; } >> does not catch thrown exception
        false (true expected)
[FAIL]  Test 15 at sample.cc:60 (0.159016s)
        1 > 2
        false (true expected)

Failure: 5/16 tests failed :(
Total time: 3.29033 seconds.
~/petitsuite>
~/petitsuite>export PETITSUITE_BREAKON=5
~/petitsuite>./test
<petitsuite/petitsuite.cpp> says: breakpoint requested on test #5
        Assertion failed: ! "<petitsuite/petitsuite.cpp> says: breakpoint requested", file petitsuite.cpp, line 61
Aborted.
```
