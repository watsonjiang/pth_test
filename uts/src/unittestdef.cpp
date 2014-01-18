#include "unittestdef.h"

namespace uts
{
  /*
    RootTestSuite is a special test suite that is used only to define the
    root of the test heirarchy.
  */
  class RootTestSuite : public TestSuite
  {
  public:
    RootTestSuite ();
    ~RootTestSuite();
  };

  uts::TestSuite& root()
  {
    static uts::RootTestSuite real_root;
    return real_root;
  }

  RootTestSuite::RootTestSuite()
  {
  }

  RootTestSuite::~RootTestSuite()
  {
  }

  IndirectTestCase::IndirectTestCase(const char* name, TestSuite& suite,
                                void (*func)(TestContext&))
    :TestCase(name, suite), m_func(func)
  {
  }

  void IndirectTestCase::run (TestContext& context)
  {
    (*m_func)(context);
  }

}
