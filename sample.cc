#include <string>
#include "dessert.hpp"

desserts() {
    int a = 1, b = 2;

    dessert( a < b );
    dessert( a > b );

    std::string hello = "hello";
    std::string hell0 = "hell0";

    dessert( "hello" ==  hello );
    dessert(  hello  == "hello" );
    dessert(  hello  ==  hello );
    dessert( "hello" == "hello" );

    dessert( "hello" !=  hell0 );
    dessert(  hello  != "hell0" );
    dessert(  hello  !=  hell0 );
    dessert( "hello" != "hell0" );

    dessert( "hell0" != "hell0" ) << "this shall fail";
}

desserts() {
    dessert( 1 < 2 ) << "test shall pass; comment built on " << __TIME__ << " " << __DATE__;
    dessert( 1 > 2 ) << "test shall fail; phone Aristotle (+30 " << 23760 << ") if this test fails";
}

desserts() {
    auto once = []{ static int num = 0; return 0 == num++; };
    dessert( once() );
    dessert( once() ) << "test shall fail";
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
