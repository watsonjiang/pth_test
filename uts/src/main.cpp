#include "unittestdef.h"
#include "testrunner.h"
#include "testqualifier.h"
#include <pth.h>

void * testThread(void *)
{
  using namespace uts;
  
  TestContext context;
  TestRunner runner(&context);
  uts::root().accept(runner);
  runner.printSummary();

  return NULL;
}

int main()
{
  pth_init();
  pth_t tid = pth_spawn(PTH_ATTR_DEFAULT, testThread, NULL);
  pth_join(tid, NULL);  

  return 0;
}
