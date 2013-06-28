// Lightweight and simple C++11 test framework. No dependencies. MIT licensed.
// rlyeh, 2012-2013

#pragma once
#include <sstream>
#include <string>

namespace petitsuite
{
    // public API, primary macros
#   define test1(A)             testX( A, !=, 0,  true,           0 )
#   define miss1(A)             testX( A, !=, 0, false,           0 )
#   define test2(A,...)         testX( A, !=, 0,  true, __VA_ARGS__ )
#   define miss2(A,...)         testX( A, !=, 0, false, __VA_ARGS__ )
#   define test3(A,OP,B)        testX( A, OP, B,  true,           0 )
#   define miss3(A,OP,B)        testX( A, OP, B, false,           0 )
#   define test4(A,OP,B,...)    testX( A, OP, B,  true, __VA_ARGS__ )
#   define miss4(A,OP,B,...)    testX( A, OP, B, false, __VA_ARGS__ )

    // public API, primary macros
#   define testcatch(...) [&]() -> bool { try { { __VA_ARGS__; } return test1( true ); } catch(...) {} \
            petitsuite::detail::string given_code("{ " #__VA_ARGS__ " }"), does_not_catch_thrown_exception("does not catch thrown exception"); \
            return test3( given_code, >>, does_not_catch_thrown_exception ); } ()

#   define testthrow(...) [&]() -> bool { try { { __VA_ARGS__; } \
            petitsuite::detail::string given_code("{ " #__VA_ARGS__ " }"), throws_no_exception("throws no exception"); \
            return test3( given_code, >>, throws_no_exception ); } catch(...) {} \
            return test1( true ); } ()

    // public API, secondary macros (macros to create batches of primary macros)
#   define autotest(after) \
        static void petit$line(petitsuite)(); \
        static const bool petit$line(autotest_) = petitsuite::detail::queue_auto( petit$line(petitsuite), std::string(#after) != "after" ); \
        void petit$line(petitsuite)()

#   define unittest(...) \
        static void petit$line(petitsuite)(); \
        static const bool petit$line(autotest_) = petitsuite::detail::queue_unit( petit$line(petitsuite) ); \
        void petit$line(petitsuite)()

    // public API, runs a batch of all defined unittests (useful in continuous integration scenarios)
    std::string units();

    // public API, optional read-only stats and runtime logs
    extern size_t passed(), failed(), executed();
    extern std::string right(), wrong(), footer();

    // public API, enable/disable/check breakpoints on tests
    extern bool has_breakpoint( unsigned testno );
    extern void set_breakpoint( unsigned testno, bool enabled = true );

    // public API, optional overridable callbacks, ie: petitsuite::on_warning = my_callback_fn;
    extern void (*on_report)( const std::string &right, const std::string &wrong, const std::string &footer );
    extern void (*on_warning)( const std::string &message );
    extern void (*on_breakpoint)( int testno );

    /* private API */
    namespace detail {
    extern void try_breakpoint( unsigned testno );
    extern void queue_test( const std::string &results, bool ok );
    extern bool queue_auto( void (*fn)(void), bool exec_now );
    extern bool queue_unit( void (*fn)(void) );
#   define petit$impl(str, num) str##num
#   define petit$join(str, num) petit$impl(str, num)
#   define petit$line(str)      petit$join(str, __LINE__)
    struct string : public std::string {
        explicit string(const char *expr) : std::string(expr) {}
        bool operator>>( const string &other ) { return false; }};
#   define testX(A,OP,B,SIGN,...) [&]() -> bool { \
        auto _TESTN_ = 1+petitsuite::executed(); \
        auto _HASBP_ = petitsuite::has_breakpoint( _TESTN_ ); \
        petitsuite::detail::try_breakpoint( _TESTN_ ); \
        std::stringstream _SS_, _TT_; \
        auto _A_ = (A); auto _B_ = decltype(_A_)(B); auto _OK_ = ( _A_ OP _B_ ); if( !SIGN ) _OK_ = !_OK_; \
        _SS_ << ( _OK_ ? "[ OK ]" : "[FAIL]" ) << ( _HASBP_ ? "* " : "  " ) << "Test " << _TESTN_; \
        _SS_ << " at " __FILE__ ":" << __LINE__; if( !_OK_ ) { \
         if( std::string("0") != #__VA_ARGS__ ) _SS_ << " - ", _SS_ << __VA_ARGS__; \
         if( std::string("decltype(" #A ")(0)") != #B ) { \
            const char *_EXPR_ = "\n\t" #A " " #OP " " #B; \
            _TT_ << "\n\t" << _A_ << " " #OP " " << _B_; \
            _SS_ << ( _TT_.str() != _EXPR_ ? _EXPR_ : "" ) << _TT_.str(); \
         } else _SS_ << "\n\t" #A; \
         _SS_ << "\n\t" << ( _OK_ ^ (!SIGN) ? "true (false expected)" : "false (true expected)" ); } \
        petitsuite::detail::queue_test( _SS_.str(), _OK_ ); \
        return _OK_ ? true : false; }()
    }
}
