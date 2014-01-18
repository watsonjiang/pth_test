#include <unittestdef.h>

/*
 Some sample tests, with the following hierarchy:
 root
 ExampleTests
 BasicTests
 SimpleStuff_1
 SimpleStuff_2
 AdvancedTests
 AdvancedStuff_1
 SeparateSingleTest

 */

void SimpleStuff_1(uts::TestContext& context)
{
  pass_if(true);
}


void AdvancedStuff_1_fails(uts::TestContext& context)
{
  // Throw anything, it should be caught.
  // This test will fail.
  pass_on_completion(throw true);
}

void SeparateSingleTest(uts::TestContext& context)
{
  // This test will succeed.
  fail_if(false);
}

DefineTestSuite(ExampleTests, uts::root());
DefineTestSuite(BasicTests, ExampleTests);
DefineTestCase(SimpleStuff_1, BasicTests);
//DefineTestSuite(AdvancedTests, ExampleTests);
//DefineTestCase(AdvancedStuff_1_fails, AdvancedTests);
//DefineTestCase(SeparateSingleTest, uts::root());

