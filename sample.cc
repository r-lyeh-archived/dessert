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
