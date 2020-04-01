# Abstraction Later for STM32

To use the stm32 Abstraction layer, create a repo with an STM32 project. The recommended tool is the STM32CubeIDE


Use the [mrt-config](https://github.com/uprev-mrt/mrtutils) tool to add in submodules. Make sure to import the **Platforms/Common** and **Platforms/STM32 modules**

> note: after importing modules, right click the project and hit refresh so it sees the new directories

To use the STM32 platform, cofigure the following settings:

**Project->Properties->C/C++ General->Path and Symbols**:
>* Under the Symbols tab add a symbol named **MRT_PLATFORM** with the value **MRT_STM32_HAL**
>
>* Under the Source Location tab click add and select the **Modules** directory under Mr T
>
>* Under the Includes tab, click add and add the path to the **Modules** directory under Mr T


---

### USING ACI BLE

To use the STM32 ACI interface for BLE:

**Project->Properties->C/C++ General->Path and Symbols**:
>* Under the Symbols tab add a symbol named **STM32_GATT_MODULE_ENABLE** 

Generate the services/profile using [mrt-ble](https://github.com/uprev-mrt/mrtutils)



### Enabling printf

The Stm32 programmers use the SWO pin to print messages back to the host. This can be useful to log out messages to the console for debug. To enable printf to work through the SWO pin follow these steps:

1. add '-lc -lrdimon' to linker flags
2. in the Debug configuration (little arrow by the bug icon) under 'Start Up' tab add "monitor arm semihosting enable" to initialization commands
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
