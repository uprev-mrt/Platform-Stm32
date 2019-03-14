# Abstraction Later for STM32



# Using with SW4STM32

### Enabling printf
To enable printf to work through the SWO pin follow these steps:

1. add '-lc -lrdimon' to linker flags
2. in the Debug configuration under 'Start Up' tab add "monitor arm semihosting enable" to initialization commands
3. add the following code snippets:

***top of main.c:***

```
#include "stdio.h"

int __io_putchar(int ch)
{
  ITM_SendChar(ch);
  return(ch);
}


int _write(int file, char *ptr, int len)
{
  int DataIdx;
  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
         __io_putchar(*ptr++);
  }
  return len;
}

extern void initialise_monitor_handles(void);

```
*** inside main() ***
```
initialise_monitor_handles();
```
