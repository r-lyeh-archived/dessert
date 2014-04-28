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
