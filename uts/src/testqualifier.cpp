#include "testqualifier.h"
namespace uts
{

  bool TestQualifier::visitEnter(TestSuite& suite)
  {
    m_path += suite.name();
    m_path += SEPCHAR;
    return true;
  }

  bool TestQualifier::visitLeave(TestSuite& suite)
  {
    m_path.resize(m_path.size() - suite.name().size() - 1);
    return true;
  }
}
