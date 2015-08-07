#include "dessert.hpp"
#include "dessert.hpp" // check header can be included twice

void cmp( int x ) {
    dessert( x );
    dessert( 0 == !x );
    dessert( !x == 0 );
    dessert( !!x );
    dessert( !!x );
}
void cmpL( int x, int y ) {
    cmp(x);
    cmp(y);
    dessert( x < y );
    dessert( y > x );
    dessert( x <= y );
    dessert( y >= x );
    dessert( x != y );
    dessert( y != x );

    dessert( !(x > y) );
    dessert( !(y < x) );
    dessert( !(x >= y) );
    dessert( !(y <= x) );
    dessert( !(x == y) );
    dessert( !(y == x) );
}
void cmpG( int x, int y ) {
    cmp(x);
    cmp(y);
    dessert( x > y );
    dessert( y < x );
    dessert( x >= y );
    dessert( y <= x );
    dessert( x != y );
    dessert( y != x );

    dessert( !(x < y) );
    dessert( !(y > x) );
    dessert( !(x <= y) );
    dessert( !(y >= x) );
    dessert( !(x == y) );
    dessert( !(y == x) );
}
void cmpE( int x, int y ) {
    cmp(x);
    cmp(y);
    dessert( x == y );
    dessert( y == x );

    dessert( !(x != y) );
    dessert( !(y != x) );
}

int main() {
    int a = 1, b = 2;
    dessert( a < b );
    dessert( b > a );
    dessert( a <= b );
    dessert( b >= a );
    dessert( a != b );
    dessert( b != a );

    int z = 0, o = 1, t = 2;
    dessert(o);
    dessert(o+o);
    dessert(!z);
    dessert( o == o );
    dessert( z != o );
    dessert( o < t );
    dessert( o <= t );
    dessert( t > o );
    dessert( t >= o );

    cmpL(8335,10002);
    cmpG(10002,8335);
    cmpE(8335,8335);
}
