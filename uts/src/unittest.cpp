#include "unittest.h"

#include <algorithm>

#include "unittestdef.h"

/*
  Important notes on initialization of the test hierarchy.

  Because the order of C++ compilation unit initialization is not well-defined,
  it is dangerous to rely on initialization of complex static data class objects
  that can refer to one another. This doesn't mean that you can't use static
  objects at all, however. Simple built-in data types are reliably initialized
  to zero by default.

  To set up the associations between tests and their parents in a simple,
  reliable, and low-overhead fashion, we rely on static TestSuite and
  TestCase objects that link themselves into a simple chain of siblings.
  The chain is built from simple pointers, so this is reliable, as long
  as there is no dependency on order of links in the chain. Every Test
  object has a next pointer, and initializes itself by putting itself in
  its parent suite's chain. When a TestSuite object is first visited, it
  copies the chain into a vector and sorts it by name.

*/

namespace uts
{

  TestVisitor::~TestVisitor()
  {
  }

  bool TestVisitor::visitLeave(TestSuite&)
  {
    return true;
  }

  TestContext::TestContext(std::ostream* logStream)
    : m_logStream(logStream), m_continueTests(true)
  {
  }

  TestContext::~TestContext()
  {
  }

  std::ostream& TestContext::logStream() const
  {
    return *m_logStream;
  }

  bool TestContext::shouldStop() const
  {
    return !m_continueTests;
  }

  void TestContext::stopTests()
  {
    m_continueTests = false;
  }

  Test::Test(const std::string& name, TestSuite& parent)
    : m_name(name)
  {
    m_next = parent.m_testChain;
    parent.m_testChain = this;
  }

  Test::Test()
    : m_name("")
  {
  }

  Test::~Test()
  {
  }

  TestCase::TestCase (const std::string& name, TestSuite& parent)
    : Test(name, parent)
  {
  }

  bool TestCase::accept(TestVisitor& v)
  {
    return v.visit(*this);
  }

  TestSuite::TestSuite(const std::string& name, TestSuite& parent)
    :Test(name, parent)
  {
  }

  TestSuite::TestSuite()
  {
    // Used only by root test suite.
  }

  TestSuite::~TestSuite()
  {
  }

  static bool NameLessThan(Test* a, Test* b)
  {
    return a->name() < b->name();
  }

  void TestSuite::initChildList()
  {
    Test* p = m_testChain;
    while (p) {
      m_children.push_back(p);
      p = p->m_next;
    }
    std::sort(m_children.begin(), m_children.end(), NameLessThan);
  }

  bool TestSuite::accept(TestVisitor& v)
  {
    if (v.visitEnter(*this)) {
      if (m_children.empty())
        initChildList();
      std::vector<Test*>::const_iterator i, i_end;
      i_end = m_children.end();
      for (i = m_children.begin(); i != i_end; ++i) {
        if (!(*i)->accept(v))
          break;
      }
      return v.visitLeave(*this);
    }
    return true;
  }


  bool TestVisitor::visitEnter(TestSuite&)
  {
    return true;
  }

XTestFailure::XTestFailure(const std::string& desc)
    : m_desc(desc), m_file("<unknown>"), m_line(0)
  {
  }

  XTestFailure::XTestFailure(const std::string& desc, const std::string& file, long line)
    : m_desc(desc), m_file(file), m_line(line)
  {
  }

  XTestFailure::~XTestFailure() throw()
  {
  }

  const char* XTestFailure::what() const throw()
  {
    return m_desc.c_str();
  }
}
