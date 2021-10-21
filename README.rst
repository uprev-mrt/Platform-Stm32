STM32
=====

To use the stm32 Abstraction layer, create a repo with an STM32 project. The recommended tool is the STM32CubeIDE


Use the `mrt-config <https://github.com/uprev-mrt/mrtutils>`_ tool to add in submodules. Make sure to import the **Platforms/Common** and **Platforms/STM32 modules**

.. note:: after importing modules, right click the project and hit refresh so it sees the new directories

To use the STM32 platform, cofigure the following settings:

* Project->Properties->C/C++ General->Path and Symbols:

  * Under the Symbols tab add a symbol named **MRT_PLATFORM** with the value **MRT_STM32_HAL**
  * Under the Source Location tab click add and select the **Modules** directory under Mr T
  * Under the Includes tab, click add and add the path to the **Modules** directory under Mr T


Troubleshooting common problems 
-------------------------------

main.h no such file 
~~~~~~~~~~~~~~~~~~~
* `main.h` no such file or directory
    This issue is normally accompanied by a wrench icon on the MrT directory which indicates local directory settings overriding the workspace settins. To correct this, right click the folder and click `Resource configurations -> Reset to Defaults`



Using ACI BLE
-------------

.. important:: deprecrated. Gatt Interface should now use the `stm32_gatt_adapter`

To use the STM32 ACI interface for BLE:   

* Project->Properties->C/C++ General->Path and Symbols:
  * Under the Symbols tab add a symbol named ``STM32_GATT_MODULE_ENABLED``

Generate the services/profile using `mrt-ble <https://github.com/uprev-mrt/mrtutils>`_    

The output will be a header/source for each service, and a header/source for the profile. 
In main.c, before 'APPE_Init();' register the profile init function:

.. code:: c

  MRT_GATT_REGISTER_PROFILE_INIT(example_profile_init);

When the server is initialized by the system it will create and register all services and characteristics. To update a value use:

.. code:: c 

  MRT_GATT_UPDATE_CHAR(&env_svc.mTemp, (uint8_t*)&temp, sizeof(temp)); /* replace env_svc.mTemp with a char in one of your services*/


Enabling printf
---------------

The Stm32 programmers use the SWO pin to print messages back to the host. This can be useful to log out messages to the console for debug. To enable printf to work through the SWO pin follow these steps:

1. add '-lc -lrdimon' to linker flags
2. in the Debug configuration (little arrow by the bug icon) under 'Start Up' tab add "monitor arm semihosting enable" to initialization commands
3. add the following code snippets:

*top of main.c:*

.. code:: c

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


*inside main()*

.. code:: c 

  initialise_monitor_handles();

