#if !defined(TESTRUNNER_HPP_INCLUDED)
#define TESTRUNNER_HPP_INCLUDED

#include "testqualifier.h"

#include <string>
#include <iostream>

#include "utsdefs.h"

namespace uts
{

  /*
    This is a basic test runner class. It acts as a visitor to the various
    test and test suites, running each test and logging the result to the
    log stream provided by the context object. It tries to print errors
    out in a format which is compatible with the typical programming
    environment of the host system.

    The visit*() routines honor the context's shouldStop() method.
  */

  class TestRunner: public TestQualifier
  {
  public:
    UTS_EXPORT TestRunner(TestContext*);

    // TestRunner::visitEnter returns false if TestContext.shouldStop() or
    // if shouldVisit() is false.
    UTS_EXPORT virtual bool visitEnter(TestSuite&);
    // TestRunner::visitLeave returns false if TestContext.shouldStop().
    UTS_EXPORT virtual bool visitLeave(TestSuite&);
    // TestRunner::visit returns false if TestContext::shouldStop()
    UTS_EXPORT virtual bool visit(TestCase&);

    ////
    // Default implementation returns true. Derived classes
    // may override. This is called once before visiting each
    // suite or test.
    UTS_EXPORT virtual bool shouldVisit(const std::string& path) const;

    ////
    // Prints a summary of the test counts on the log stream.
    UTS_EXPORT void printSummary() const;
  protected:
    std::ostream& logS() const {return m_context->logStream();}

    TestContext* m_context;
    int m_tests;
    int m_failures;
    int m_errors;
  };
}

#endif
