#include "unittestdef.h"
#include "testrunner.h"
#include "testqualifier.h"

int main()
{
  using namespace uts;
  
  TestContext context;
  TestRunner runner(&context);
  uts::root().accept(runner);
  runner.printSummary();
  
  return 0;
}
