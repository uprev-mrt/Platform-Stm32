# Abstraction Later for STM32

To use the stm32 Abstraction layer, create a repo with an STM32 project. The recommended tools are SW4STM32 and STM32CubeMx

* **SW4STM32** is a free IDE for delevoping on the stm32 family of MCU's. It is an Eclipse distribution set up for C/C++ that comes prepackaged with the toolchains and examples for Stm32 processors

* **STM32CubeMx** is a free configuration tool created by St micro. It provides a GUI for configuring peripherals and generating a project. It can directly create a project for SW4STM


>


>SW4STM: http://www.openstm32.org/System%2BWorkbench%2Bfor%2BSTM32
<br/>
>STM32CubeMx: https://www.st.com/en/development-tools/stm32cubemx.html

Once the software is installed run STM32CubeMx and follow the steps to configure the project. When you are ready to create the project go to the **Project Manager** tab to specify where to create the project. Under 'Toolchain/ IDE' select 'SW4STM32', then click 'Generate Code'

Next follow the steps in the MrT/Config Repo to import the Mr T configuration tool as a submodule. This tool will let you manage your Mr T modules. Make sure to import the **Platforms/Common** and **Platforms/STM32 modules**

To use the STM32 platform, cofigure the following settings:

**Project->Properties->C/C++ General->Path and Symbols**:
>Under the Symbols tab add a symbol named **MRT_PLATFORM_STM32_HAL**<br/>
Under the Source Location tab click add and select the **Modules** directory under Mr T

**Project->Properties->C/C++ Build->Settings->Includes**:

> Add The modules directory to the include list for GCC (and G++ if used)

**NOTE**: Sometimes reconfiguring the project with STM32CubeMx will overwrite these changes.

>To test that it is configured Correctly, build the project and look at the output. There should be a message that says "MrT Platform: STM32"



---

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
