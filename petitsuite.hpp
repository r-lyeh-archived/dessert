// Lightweight and simple C++11 test framework. No dependencies.
// rlyeh, 2012-2013. MIT licensed.

#pragma once
#include <sstream>
#include <string>

namespace petitsuite
{
    // public API, main macros
#   define test3(A,op,B) do { \
        std::stringstream _SS_, _TT_; \
        auto _A_ = (A); decltype(_A_) _B_ = decltype(_A_)(B); auto _OK_ = ( _A_ op _B_ ); \
        _SS_ << ( _OK_ ? "[ OK ]" : "[FAIL]" ) << " Test " << petitsuite::executed(); \
        _SS_ << " at " __FILE__ ":" << __LINE__; if( !_OK_ ) { \
         if( std::string("decltype(" #A ")(0)") != #B ) { \
            const char *_EXPR_ = "\n\t" #A " " #op " " #B; \
            _TT_ << "\n\t" << _A_ << " " #op " " << _B_; \
            _SS_ << ( _TT_.str() != _EXPR_ ? _EXPR_ : "" ) << _TT_.str(); \
         } else _SS_ << "\n\t" #A; \
         _SS_ << "\n\t" << ( _OK_ ? "(true)" : "(false)" ); } \
        petitsuite::detail::queue_test( _SS_.str(), _OK_ ); \
    } while( 0 )

#   define test1(A) \
        test3(A,!=,decltype(A)(0))

#   define testexception(...) do { try { { __VA_ARGS__; } { \
            struct custom : public std::string { \
                custom(const char *expr_ = "") : std::string(expr_) {} \
                bool operator>>( const custom &other ) { return false; }}; \
            custom given_expression("{ " #__VA_ARGS__ " }"), does_not_throw("produces no exception"); \
            test3( given_expression, >>, does_not_throw ); } \
        } catch(...) { test1( true ); } } while(0)

    // public API, secondary macros (for batches)
#   define autotest(after) \
        static void test$line(n)(); \
        const bool test$line(autotest_) = petitsuite::detail::queue_auto( test$line(n), std::string(#after) != "after" ); \
        void test$line(n)()

#   define unittest() \
        static void test$line(n)(); \
        const bool test$line(autotest_) = petitsuite::detail::queue_unit( test$line(n) ); \
        void test$line(n)()

    // public API, optional read-only stats and runtime logs
    extern size_t passed(), failed(), executed();
    extern std::string right(), wrong(), footer();

    // public API, perform a batch run of all unittests (useful for continuous integration scenarios)
    std::string units();

    // public API, optional overridable callbacks, ie: petitsuite::on_warning = my_callback_fn;
    extern void (*on_warning)( const std::string &message );
    extern void (*on_report)( const std::string &right, const std::string &wrong, const std::string &footer );

    /* private API */
    namespace detail {
    extern void queue_test( const std::string &results, bool ok );
    extern bool queue_auto( void (*fn)(void), bool exec_now );
    extern bool queue_unit( void (*fn)(void) );
#   define test$impl(str, num) str##num
#   define test$join(str, num) test$impl(str, num)
#   define test$line(str)      test$join(str, __LINE__)
    }
}
