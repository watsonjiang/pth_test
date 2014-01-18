#if !defined(UNITTESTDEF_HPP_INCLUDED)
#define UNITTESTDEF_HPP_INCLUDED

#include <memory>
#include <exception>

#include "unittest.h"

#include "utsdefs.h"

///////////////////////////////////////////////////////////////////////////////
// Macros to help define test suites and test case functions.
// These are used at file level, i.e. not inside a function or class.
// See the test file "uts_test.cpp" for some simple examples.

// DefineTestSuite defines a TestSuite object with the given name, and
// with the given suite object as parent.
// You may prefix this macro with a storage qualifier such as "static".
#define DefineTestSuite(_name_, _suite_) \
   uts::TestSuite _name_(#_name_, (_suite_))

// DefineTestCase defines a test case function with a context parameter.
// It also does some stuff to make sure that the test case is associated
// with the suite that you specify.
// You follow an invocation of this macro with the body of your test
// function, including the enclosing braces.
// You may prefix this macro with a storage qualifier such as "static".
// - Test case functions must use pass_if/fail_if to detect & record test
//   failures.
// - Test case functions must call context.stopTests() prior to pass_if/fail_if
//   on the control expression if failure requires all testing to halt.  Yes,
//   that means storing the expression result in a variable prior to calling
//   pass_if/fail_if.
#define DefineTestCase(_name_, _suite_) \
  static uts::IndirectTestCase _name_##TestCase(#_name_, _suite_, &_name_); \

///////////////////////////////////////////////////////////////////////////////
// Macros to help define the bodies of test case functions.
// If you use these macros, test runner objects should be able to print out
// error messages that tell you exactly which line of your test function
// failed, making debugging a whole lot easier.

// The do ... while constructs in the macro definitions are a clever
// way of making sure that the expansions work in all statement contexts,
// including "if" clauses without braces. (Taken from "C -- A Reference Manual"
// by Harbison & Steele.)

// pass_on_completion throws a test exception if the expression given to
// it throws any exception.
#define pass_on_completion(_expr_) \
  do { \
    try {_expr_;} \
      catch(const uts::XTestFailure&) \
        {throw;} \
      catch(const std::exception& e) \
        {throw uts::XTestFailure(e.what(), __FILE__, __LINE__);} \
      catch(...) \
        {throw uts::XTestFailure("unknown exception", __FILE__, __LINE__);} \
   } while (0)

#define fail(_desc_) \
  do{ \
    pass_on_completion( \
        throw uts::XTestFailure(#_desc_, __FILE__, __LINE__)); \
  } while (0)

#define pass_if(_expr_) \
  do { \
    pass_on_completion( \
      if (!(_expr_)) throw uts::XTestFailure(#_expr_, __FILE__, __LINE__)); \
  } while (0)

#define fail_if(_expr_) \
  do { \
    pass_on_completion(\
      if (_expr_) throw uts::XTestFailure(#_expr_, __FILE__, __LINE__)); \
  } while (0)

#define fail_and_stop_tests_if(_expr_, _context_) \
  do \
    { \
    pass_on_completion(\
                       if (_expr_) \
                         { \
                         _context_.stopTests(); \
                         throw uts::XTestFailure(#_expr_, __FILE__, __LINE__); \
                         } \
                      ); \
    } while (0)

///////////////////////////////////////////////////////////////////////////////

namespace uts
{
  /*
    This retuns the root suite object. You'll need uts::root() as an argument
    to DefineTestSuite or DefineTestCase when you want your tests to be
    in the top level of the test hierarchy.
  */
  UTS_EXPORT uts::TestSuite& root();


  ////////////////////////////////////////////////////////////////////////////
  // From here on are internal definitions that support the macros above.

  /*
    An IndirectTestCase is simply a test case object that maintains a pointer
    to a function that executes the actual test. This makes it a little
    simpler to define typical test cases, because they can be simple
    functions rather than objects.
  */
  class IndirectTestCase : public TestCase
  {
  public:
    UTS_EXPORT IndirectTestCase(const char*, TestSuite&,
                                void (*func)(TestContext&));
    UTS_EXPORT void run (TestContext&);
  private:
    void (*m_func)(TestContext&);
  };

}
#endif
