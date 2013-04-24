#include "petitsuite.hpp"

int main() {
    int a = 1, b = 2;

    test1( 1 == 1 );    // pass
    test1( 1 <= 0 );    // fail

    test3( 1, <, 2 );   // pass
    test3( a, >, b );   // fail
    test3( 1, !=, b);   // pass

    return 0;
}

autotest(before) {            // auto test that runs before main()
    test3( true, ==, false ); // fail
}

autotest(after) {             // auto test that runs after main()
    test3( true, >, false );  // pass
}
