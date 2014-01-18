#if !defined(TESTQUALIFIER_HPP_INCLUDED)
#define TESTQUALIFIER_HPP_INCLUDED

#include "unittest.h"
#include "utsdefs.h"

namespace uts
{
  /*
    TestQualifier is a simple test visitor which merely maintains the
    fully-qualified path of test objects while it's traversing the
    test tree. It's convenient to use it to help implement more
    complex visitors, because it can provide them with this path on
    demand.
  */
  class TestQualifier : public TestVisitor
  {
  public:
    enum {SEPCHAR = '/'};

    UTS_EXPORT virtual bool visitEnter(TestSuite&);
    UTS_EXPORT virtual bool visitLeave(TestSuite&);
    const std::string& currentPath() {return m_path;}
  private:
    std::string m_path;
  };
}

#endif
