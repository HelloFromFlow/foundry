foundry is a straightforward logging utility written entirely in C

it currently doesn't have a lot of functionality but I'm looking forward towards expanding this project

code example:
```c
#include "foundry.h"


int main(void) {
  foundryRegisterApplication("test.c", "output.log");

  // foundryClean();
  /* this function is used for taking the file specified in "foundryRegisterApplication" (output.log in this case)
     and removing all lines where the name of the application is the same as the one specified above (test.c in this case) */

  foundryLog("INFO", "executable ran successfully");

  return 0;
}
```
