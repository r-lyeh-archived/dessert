#include "petitsuite.hpp"

int main() {
    int a = 1, b = 2;

    test1( 1 == 1 );    // pass
    test1( 1 <= 0 );    // fail

    test3( a, <, b );   // pass
    test3( a, >, b );   // fail
    test3( 1, !=, b);   // pass

    return 0;
}

autotest(equality_test) {
    test3( 2, ==, 2 );  // pass
    test3( 2, !=, 2 );  // fail
}
