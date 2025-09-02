/* Copyright (C) 2022 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 *
 * @file  m55_services_main.c
 * @brief Source file for SERVICES test harness
 * @par
 */

/******************************************************************************
 *  I N C L U D E   F I L E S
 *****************************************************************************/
#include <stdio.h>
#include <inttypes.h>
#include <stdarg.h>
#include <time.h>
#include "mhu.h"
#include "mhu_driver.h"
#include "services_lib_interface.h"
#include "RTE_Components.h"
#include "power_modes.h"
#include <se_services_port.h>

/*******************************************************************************
 *  M A C R O   D E F I N E S
 ******************************************************************************/

#if defined(M55_HE)
#define CPU_STRING "M55_HE"
#include "core_defines.h"
#elif defined(M55_HP)
#define CPU_STRING "M55_HP"
#include "core_defines.h"
#else
#define CPU_STRING "<unknown>"
#endif

/**
 * Test parameters
 * CONTINUOUS_RUN Will run forever if set to 1
 * LIMITED_RUN    Default is 1 so just once around
 */
#define CONTINUOUS_RUN         0
#define LIMITED_RUN            1


#define MAXIMUM_TIMEOUT        0x01000000

/*******************************************************************************
 *  T Y P E D E F S
 ******************************************************************************/

/*******************************************************************************
 *  G L O B A L   V A R I A B L E S
 ******************************************************************************/


extern void SERVICES_test(uint32_t services_handle);

/*******************************************************************************
 *  C O D E
 ******************************************************************************/

static int32_t bk_ram_rd(uint32_t *data, uint32_t offset)
{
    if ((offset < 1000)) {
        volatile uint32_t *bk_ram = (uint32_t *)0x4902C000;
        *data = *(bk_ram + offset);
        return 0;
    }
    return -1;
}

/**
 * @brief Main function
 * @return
 */
int main(void)
{
  uint32_t power_type_input=0;
  uint32_t power_mode_input=0;
  uint32_t hp_start_flag=0;
  char *powermodes[]={"GO_MODE","READY_MODE","IDLE_MODE","STANDBY_MODE","STOP_MODE"};
  POWER_MODE_TYPE power_type = GOMODE;
  SERVICES_print("[SE SERVICES] %s Test harness - STARTS\n", CPU_STRING);

  se_services_port_init();

  while(1)
  {
    if(bk_ram_rd(&hp_start_flag,START_OFFSET)!= -1)
    {
      if(hp_start_flag == 1)
       break;
    }
  }
  if((bk_ram_rd(&power_type_input,POWER_TYPE_OFFSET)!= -1) && (bk_ram_rd(&power_mode_input,POWER_MODE_OFFSET) != -1))
  {
    if((power_type_input >=1) && (power_type_input <=5))
    {
      power_type = power_type_input;

    }
  }

  /**
   * Call the SERVICES test harness
   */
  switch(power_type)
  {
    case GOMODE: 
        SERVICES_test_go(se_services_s_handle,power_mode_input,powermodes[power_type_input-1]);
       break;
    case READYMODE:
        SERVICES_test_ready(se_services_s_handle,power_mode_input,powermodes[power_type_input-1]);
        break;
    case IDLEMODE:
        SERVICES_test_idle(se_services_s_handle,power_mode_input,powermodes[power_type_input-1]);
        break;
    case STANDBYMODE:
        SERVICES_test_standby(se_services_s_handle,power_mode_input,powermodes[power_type_input-1]);
        break;
    case STOPMODE:
        SERVICES_test_stop(se_services_s_handle,power_mode_input,powermodes[power_type_input-1]);
        break;
    default:
       break;
  }
  

  SERVICES_print("[SE SERVICES] Test harness ENDS\n");

  while(1);

  return 0;
}

