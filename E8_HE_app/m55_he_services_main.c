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
#include <stdlib.h>
#include <inttypes.h>
#include <stdarg.h>
#include <time.h>
#include "mhu.h"
#include "mhu_driver.h"
#include "services_lib_interface.h"
#include "RTE_Components.h"
#include "power_modes.h"
#include <se_services_port.h>

#if defined(RTE_CMSIS_Compiler_STDIN_Custom)
#include "retarget_stdin.h"
#endif  /* RTE_Compiler_IO_STDIN */

#if defined(RTE_CMSIS_Compiler_STDOUT_Custom)
#include "retarget_stdout.h"
#endif  /* RTE_Compiler_IO_STDOUT */

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

#if defined(RTE_CMSIS_Compiler_STDIN_Custom)
#include "retarget_stdin.h"
#endif  /* RTE_Compiler_IO_STDIN */
#if defined(RTE_CMSIS_Compiler_STDOUT_Custom)
#include "retarget_stdout.h"
#endif  /* RTE_Compiler_IO_STDOUT */

/**
 * Test parameters
 * CONTINUOUS_RUN Will run forever if set to 1
 * LIMITED_RUN    Default is 1 so just once around
 */
#define CONTINUOUS_RUN         0
#define LIMITED_RUN            1

#define MAXIMUM_TIMEOUT        0x01000000

#define POWER_MSG_CNT       5


/*******************************************************************************
 *  T Y P E D E F S
 ******************************************************************************/

/*******************************************************************************
 *  G L O B A L   V A R I A B L E S
 ******************************************************************************/


/* Data Required for Power Mode */
static char   power_example_msg[POWER_MSG_CNT][64] =
{
    "\r\n\t1. GO_MODE\r\n",
    "\r\n\t2. READY_MODE\r\n",
    "\r\n\t3. IDLE_MODE\r\n",
    "\r\n\t4. STANDBY_MODE\r\n",
    "\r\n\t5. STOP_MODE\r\n",
};

static char   power_mode_msg[POWER_MSG_CNT][200] =
{
    "\r\n\t1. GO_MODE_1\r\n\t2. GO_MODE_2\r\n\t3. GO_MODE_3\r\n\t4. GO_MODE_4\r\n\t5. GO_MODE_5\r\n",
    "\r\n\t1. READY_MODE_1\r\n\r\n\t2. READY_MODE_2\r\n",
    "\r\n\t1. IDLE_MODE_1\r\n\r\n\t2. IDLE_MODE_2\r\n",
    "\r\n\t1. STANDBY_MODE_1\r\n",
    "\r\n\t1. STOP_MODE_1\r\n\t2. STOP_MODE_2\r\n\t3. STOP_MODE_3\r\n\t4. STOP_MODE_4\r\n\t5. STOP_MODE_5\r\n",
};


/*******************************************************************************
 *  C O D E
 ******************************************************************************/


/**
  @fn           void pm_usage_menu()
  @brief        Power Management Menu
  @return       none
*/
static void pm_usage_menu()
{
    int i;

    printf("\r\nSelect From Different Power mode type Below... \r\n");

    for(i = 0; i < POWER_MSG_CNT; i++)
    {
        printf("%s",&power_example_msg[i][0]);
    }
    printf("\r\n");

    return;
}

/**
  @fn           static int32_t bk_ram_wr(uint32_t *data, uint32_t offset)
  @brief        Shared memory write
  @param        data
  @param        offset
  @return       none
*/
static int32_t bk_ram_wr(uint32_t *data, uint32_t offset)
{
    if (offset < 1000) {
        volatile uint32_t *bk_ram = (uint32_t *)0x4902C000;
        *(bk_ram + offset) = *data;
        return 0;
    }
    return -1;
}

/**
  @fn           static bool validate_power_mode(uint32_t power_type_input, uint32_t power_mode)
  @brief        Validate input power mode based on respective power mode type
  @param        power_type_input
  @param        power_mode
  @return       none
*/
static uint32_t validate_power_mode(uint32_t power_type_input)
{
  uint32_t ret=0;
  char p='0';
  if (power_type_input == 1)
  {
      while ((p < '1') || (p > '5')) {
         scanf("%c", &p);
       }      
  }
  else if(power_type_input == 2)
  {
       while ((p < '1') || (p > '2')) {
         scanf("%c", &p);
       }
  }
  else if(power_type_input == 3)
  {
       while ((p < '1') || (p > '2')) {
         scanf("%c", &p);
       }
  }
  else if(power_type_input == 4)
  {
      while ((p < '1') || (p > '1')) {
         scanf("%c", &p);
       }  
  }
  else if (power_type_input == 5)
  {
      while ((p < '1') || (p > '5')) {
         scanf("%c", &p);
       }  
  }
     ret = p-'0';
     return ret;
}

/**
 * @brief Main function
 * @return
 */
int main(void)
{
  POWER_MODE_TYPE power_type = GOMODE;
  char *powermodes[]={"GO_MODE","READY_MODE","IDLE_MODE","STANDBY_MODE","STOP_MODE"};
  int32_t ret=-1;
  uint32_t power_type_input=0;
  uint32_t power_mode_input=0;
  uint32_t hp_start_flag=0;
  bool validate_display_mode=false;
  char s='0';

  
  SERVICES_print("[SE SERVICES] %s Test harness - STARTS\n", CPU_STRING);

   se_services_port_init();

#if defined(RTE_CMSIS_Compiler_STDIN_Custom)
    ret = stdin_init();
    if(ret != 0)
    {
        while(1)
        {
        }
    }
#endif

#if defined(RTE_CMSIS_Compiler_STDOUT_Custom)
    ret = stdout_init();
    if(ret != 0)
    {
        while(1)
        {
        }
    }
#endif

  printf("\r\n=========================================================\r\n");
  printf("\r\n    aiPM Test Application\r\n");
  printf("\r\n=========================================================\r\n");
  pm_usage_menu();
  while(1)
  {
     printf("\r\nEnter Power mode type to be run:  ");
     while ((s < '1') || (s > '5')) {
        scanf("%c", &s);
     }
     power_type_input= s-'0';
     if((power_type_input >=1) && (power_type_input <=5))
     {
         s='0';
         break;
     }
  }
  printf("\r\nSelected Power mode type is %s\n", powermodes[power_type_input-1]);
  printf("\r\n=========================================================\r\n");
  printf("\r\nSelect the Power mode from the following list below: ");
  printf("\r\n %s",power_mode_msg[power_type_input-1]);
  while(1)
  {
       printf("\r\nEnter the Power mode for the selected type: ");
       power_mode_input=validate_power_mode(power_type_input);
       if(power_mode_input != 0)
         break;
  }
  printf("\n");
  printf("\r\nSelected Power mode type is %s_%d\n", powermodes[power_type_input-1],power_mode_input);
  while(1)
  {
    if(((bk_ram_wr(&power_type_input,POWER_TYPE_OFFSET))!= -1) &&  (bk_ram_wr(&power_mode_input,POWER_MODE_OFFSET)!= -1))
    {
      power_type = power_type_input;
      break;
    }
  }

  while(1)
  {
    hp_start_flag=1;
    if((bk_ram_wr(&hp_start_flag,START_OFFSET))!= -1)
    break;
  }
  *(volatile uint32_t*)0x1A010400 = 4;

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

