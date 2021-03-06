#include <stdio.h>
#include "JsScriptRuntime.h"
#include "JsScript.h"

using namespace Js;

int
main(int argc, char ** argv)
{
  if(argc == 1)
  {
    printf("usage: js <file>\n");
    return -1; 
  }

  char * file = argv[1];
  ScriptRuntime *rt = new ScriptRuntime();
  std::string error;
  Script* script = rt->getScript(file, error);
  if(NULL == script){  
     printf("%s\n", error.c_str());
     return -1;
  }
  ScriptExecutionContext* ctx = rt->newContext();
  int rs = rt->runScript(ctx, script, error);
  if(rs != 0){
     printf("%s\n", error.c_str());
  } 
}

