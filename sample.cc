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
