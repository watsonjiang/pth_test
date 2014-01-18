#ifdef _WIN32
#include "TestRunner.h"

#include <iostream>
#include <cassert>
#include <sstream>


namespace uts
{
  TestRunner::TestRunner(TestContext* context)
    : m_context(context), m_tests(0), m_failures(0), m_errors(0)
  {
    assert(context);
  }

  bool TestRunner::shouldVisit(const std::string& path) const
  {
    return true;
  }

  void TestRunner::printSummary() const
  {
    logS()
      << "\n"
      << "============================================================\n"
      << "Test Summary:\n"
            << m_tests << " test(s)\n"
            << m_failures << " failure(s)\n"
            << m_errors << " error(s)\n"
      << "============================================================\n"
      << std::endl;
  }

  inline std::string outputLine(const std::string& file, long line)
  {
    std::stringstream ss;
    ss << file << ':' << line << ": ";
    return ss.str();
  }

  bool TestRunner::visitEnter(TestSuite& suite)
  {
    if (m_context->shouldStop())
      return false;

    if (!TestQualifier::visitEnter(suite))
      return false;

    if (shouldVisit(currentPath()))
      return true;

    TestQualifier::visitLeave(suite);
    return false;
  }

  bool TestRunner::visitLeave(TestSuite& suite)
  {
    return TestQualifier::visitLeave(suite) && !m_context->shouldStop();
  }
  /*
   * run the test case
   * return 0 for pass. -1 for fail.
   */
  inline int runTestCase( TestCase & test, TestContext & context, std::ostream & log)
  {
    int rst = 0;
    try {
      test.run(context);
      log << "Passed ";
      rst = 0;
    }
    catch (XTestFailure& e) {
      log << outputLine(e.m_file, e.m_line) << e.what() << '\n' << "FAILED: ";
      rst = -1;
    }
    catch (std::exception& e) {
      log << outputLine(__FILE__, __LINE__)
          << "caught standard exception - " << e.what() << '\n'
          << "FAILED: ";
      rst = -1;
    }
    return rst;
  }

  bool TestRunner::visit(TestCase& test)
  {
    if (m_context->shouldStop())
      return false;
    if (!shouldVisit(currentPath() + test.name()))
      return false;

    int fd[2] = {0};
    pipe(fd);
    int pid = fork();
    if( pid != 0 )
    {
      //child process
      close(fd[0]);
      int rst = runTestCase(test, *m_context, logS());
      logS() << currentPath() << test.name() << std::endl;
      write(fd[1],&rst, sizeof(rst));
      close(fd[1]);
      exit(0);
    }
    close(fd[1]);
    int rst = 0;
    int rcvn = 0;
    rcvn = read(fd[0],&rst,sizeof(rst));
    if ((sizeof(rst) == rcvn))
    {
      if( 0 != rst )
        m_failures++;
    }else{
      //test core dumped.
      logS()<<"CORE-DUMPED:" << currentPath() << test.name() << std::endl;
      m_errors++;
    }
    m_tests++;
    // Always continue, regardless of test result, unless the context
    // object says otherwise.
    return !m_context->shouldStop();
  }
}
#endif