/**
 * @file  services_aipm_go_modes.c
 *
 * @brief Services Power example BASIC configure clocks and power domains
 *        for GO Modes 1-
 * @ingroup services
 * @par
 * Copyright (C) 2023 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 *
 */

/******************************************************************************
 *  I N C L U D E   F I L E S
 *****************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "services_lib_interface.h"
#include "services_lib_api.h"
#include "cmsis_compiler.h"
#include "aipm.h"
#include "pm.h"
#include "power_modes.h"

/*******************************************************************************
 *  M A C R O   D E F I N E S
 ******************************************************************************/

#if defined(M55_HP) || defined(E8_M55_HP)
#define PD_RTSS_LOCAL_MASK          PD_RTSS_HP_MASK
#define CLOCK_FREQUENCY_CPU         CLOCK_FREQUENCY_400MHZ
#elif defined(M55_HE) || defined(E8_M55_HE) || defined(E1C_M55_HE)
#define PD_RTSS_LOCAL_MASK          PD_RTSS_HE_MASK
#define CLOCK_FREQUENCY_CPU         CLOCK_FREQUENCY_160MHZ
#endif

/*
 * Power counter can be input as a make parameter
 */
#ifndef POWER_COUNTER
#define POWER_COUNTER               0x6000000
#endif

#define UNUSED(x) (void)(x)

/*******************************************************************************
 * F U N C T I O N   P R O T O T Y P E S
 ******************************************************************************/

/*******************************************************************************
 *  T Y P E D E F S
 ******************************************************************************/

/*******************************************************************************
 *  G L O B A L   V A R I A B L E S
 ******************************************************************************/

/*******************************************************************************
 *  C O D E
 ******************************************************************************/

/**
 * @brief
 *
 * @param mode_number
 */
void execute_go_mode_usecase(uint32_t mode_number)
{
    volatile uint32_t debug_loop = 1;

    switch(mode_number) {
    case 1:
        /* function call to exercise NPU here */

    case 2:
        /* function call to exercise CPU here */
        while(1) if (debug_loop == 0) break;

        break;
    case 3:
#if defined(M55_HE) || defined(E8_M55_HE) || defined(E1C_M55_HE)
        /* go to subsystem off */
        while(1) 
        {
          __disable_irq();
          pm_core_enter_deep_sleep_request_subsys_off();
          __enable_irq();
        }
#endif
        /* function call to exercise CPU here */
        while(1) if (debug_loop == 0) break;

        break;
    case 4:
    case 5:
#if defined(M55_HP) || defined(E8_M55_HP)
        /* go to subsystem off */
        while(1) 
        {
          __disable_irq();
          pm_core_enter_deep_sleep_request_subsys_off();
          __enable_irq();
        }
#endif
        /* function call to exercise CPU here */
        while(1) if (debug_loop == 0) break;

        break;
    default:
        /* this should not happen */
        break;
    }
}

/**
 * @brief
 *
 * @param mode_number
 * @param runp
 * @param offp
 */
void configure_go_mode_profiles(uint32_t mode_number,
        run_profile_t *runp, off_profile_t *offp)
{
    runp->aon_clk_src = CLK_SRC_LFXO;
    runp->dcdc_mode = DCDC_MODE_PWM;
    runp->vdd_ioflex_3V3 = IOFLEX_LEVEL_1V8;
    runp->phy_pwr_gating = 0;

    offp->wakeup_events     = 0;
    offp->ewic_cfg          = 0;
    offp->aon_clk_src       = CLK_SRC_LFXO;
    offp->stby_clk_freq     = SCALED_FREQ_RC_ACTIVE_76_8_MHZ;
    offp->dcdc_mode         = DCDC_MODE_PFM_FORCED;
    offp->dcdc_voltage      = 825;
    offp->vdd_ioflex_3V3    = IOFLEX_LEVEL_1V8;
    offp->vtor_address      = SCB->VTOR;
    offp->vtor_address_ns   = SCB->VTOR;

    switch(mode_number) {
    case 1:
        runp->cpu_clk_freq = CLOCK_FREQUENCY_CPU;
        runp->scaled_clk_freq = SCALED_FREQ_RC_ACTIVE_76_8_MHZ;
        runp->run_clk_src = CLK_SRC_PLL;
        runp->dcdc_voltage = 825;
        offp->stby_clk_src  = CLK_SRC_HFXO;
        #if defined(E1C_M55_HE)
        runp->memory_blocks = SRAM2_MASK | SRAM3_MASK | MRAM_MASK;
        offp->memory_blocks = SRAM2_MASK | SRAM3_MASK | MRAM_MASK;
        #else
        runp->memory_blocks = SRAM0_MASK | SRAM1_MASK | MRAM_MASK;
        offp->memory_blocks = SRAM0_MASK | SRAM1_MASK | MRAM_MASK;
        #endif
        runp->power_domains = PD_SYST_MASK | PD_SESS_MASK;
        offp->power_domains = PD_SYST_MASK | PD_SESS_MASK;
        break;
    case 2:
        runp->cpu_clk_freq = CLOCK_FREQUENCY_CPU;
        runp->scaled_clk_freq = SCALED_FREQ_RC_ACTIVE_76_8_MHZ;
        runp->run_clk_src = CLK_SRC_PLL;
        runp->dcdc_voltage = 825;
        offp->stby_clk_src  = CLK_SRC_HFXO;
        #if defined(E1C_M55_HE)
        runp->memory_blocks = SRAM2_MASK | SRAM3_MASK | MRAM_MASK;
        offp->memory_blocks = SRAM2_MASK | SRAM3_MASK | MRAM_MASK;
        #else
        runp->memory_blocks = SRAM0_MASK | SRAM1_MASK | MRAM_MASK;
        offp->memory_blocks = SRAM0_MASK | SRAM1_MASK | MRAM_MASK;
        #endif
        runp->power_domains = PD_SYST_MASK | PD_SESS_MASK;
        offp->power_domains = PD_SYST_MASK | PD_SESS_MASK;
        break;
    case 3:
        runp->cpu_clk_freq = CLOCK_FREQUENCY_CPU;
        runp->scaled_clk_freq = SCALED_FREQ_RC_ACTIVE_76_8_MHZ;
        runp->run_clk_src = CLK_SRC_PLL;
        runp->dcdc_voltage = 825;
        runp->dcdc_mode = DCDC_MODE_PWM;
        offp->stby_clk_src  = CLK_SRC_HFXO;
        runp->memory_blocks = SERAM_MASK;
        offp->memory_blocks = SERAM_MASK;
        runp->power_domains = PD_SYST_MASK;         /* PD6_SYST=ON */
        offp->power_domains = PD_SYST_MASK;         /* PD6_SYST=ON */
        break;
    case 4:
        /* the below cpu_clk_freq selection will follow the scaled_clk_freq */
        runp->cpu_clk_freq = CLOCK_FREQUENCY_76_8_RC_MHZ;
        runp->scaled_clk_freq = SCALED_FREQ_RC_ACTIVE_76_8_MHZ;
        runp->run_clk_src = CLK_SRC_HFRC;
        runp->dcdc_voltage = 825;
        runp->dcdc_mode = DCDC_MODE_PWM;
        offp->stby_clk_src  = CLK_SRC_HFRC;
        runp->memory_blocks = SERAM_MASK;
        offp->memory_blocks = SERAM_MASK;
        runp->power_domains = PD_SSE700_AON_MASK;   /* PD6_SYST=OFF */
        offp->power_domains = PD_SSE700_AON_MASK;   /* PD6_SYST=OFF */
        break;
    case 5:
        /* the below cpu_clk_freq selection will follow the scaled_clk_freq */
        runp->cpu_clk_freq = CLOCK_FREQUENCY_76_8_RC_MHZ;
        runp->scaled_clk_freq = SCALED_FREQ_RC_ACTIVE_19_2_MHZ;
        runp->run_clk_src = CLK_SRC_HFRC;
        runp->dcdc_voltage = 825;
        runp->dcdc_mode = DCDC_MODE_PWM;
        offp->stby_clk_src  = CLK_SRC_HFRC;
        runp->memory_blocks = SERAM_MASK;
        offp->memory_blocks = SERAM_MASK;
        runp->power_domains = PD_SSE700_AON_MASK;   /* PD6_SYST=OFF */
        offp->power_domains = PD_SSE700_AON_MASK;   /* PD6_SYST=OFF */
        break;
    default:
        /* this should not happen */
        break;
    }
}

/**
 * @brief GO Modes reference example
 *
 * @param p_test_name
 * @param services_handle
 * @return error code
 */
uint32_t exercise_aipm_go_modes(char *p_test_name,
                                     uint32_t services_handle, uint32_t power_mode)
{
  uint32_t msg_status = SERVICES_REQ_SUCCESS;
  uint32_t service_resp;
  run_profile_t runp = {0};
  off_profile_t offp = {0};
  uint32_t aclk_div = 1;

  TEST_print(services_handle,
              "TEST %s",
              p_test_name);

  TEST_print(services_handle,
              "GO_%d\n",
              power_mode);

  configure_go_mode_profiles(power_mode, &runp, &offp);

  msg_status = SERVICES_set_run_cfg(services_handle, &runp, &service_resp);

  TEST_print(services_handle,
              "SERVICES_set_run_cfg() error_code=%s service_resp=0x%08X\n",
              SERVICES_error_to_string(msg_status),
              service_resp);

  msg_status = SERVICES_set_off_cfg(services_handle, &offp, &service_resp);

  TEST_print(services_handle,
              "SERVICES_set_off_cfg() error_code=%s service_resp=0x%08X\n",
              SERVICES_error_to_string(msg_status),
              service_resp);

  //Disabling systop in the SE Host register
  *(volatile uint32_t*)0x1A010400 = 0;
  SERVICES_wait_ms(1500);

if ( power_mode > 2)
{
  runp.dcdc_mode = DCDC_MODE_PFM_FORCED;

  msg_status = SERVICES_set_run_cfg(services_handle, &runp, &service_resp);

  TEST_print(services_handle,
              "SERVICES_set_run_cfg() error_code=%s service_resp=0x%08X\n",
              SERVICES_error_to_string(msg_status),
              service_resp);
}

if (power_mode < 4)
{
  /* ACLK divider value:
   * E5/E7 devices, use 0 for 400 MHz ACLK / 200 MHz HCLK / 100MHz PCLK
   * E3 devices with HP active, use 1 for 200 MHz ACLK / 100 MHz HCLK / 50MHz PCLK
   * E1 devices/E3 with only HE active, use 3 for 100 MHz ACLK / 50 MHz HCLK / 25MHz PCLK
   */
   if (power_mode == 3)
       aclk_div = 3;
   else if(power_mode == 2)
        aclk_div = 1;
   else
        aclk_div = 0;
   msg_status = SERVICES_clocks_set_divider(services_handle,
                          /*clock_divider_t*/   DIVIDER_ACLK,
                          /*uint32_t value*/    aclk_div,
                                                &service_resp);
}

#if defined(M55_HE) || defined(E8_M55_HE) || defined(E1C_M55_HE)
  if (power_mode > 3) {
    /* Allow HP core to finish running before powering down the SE */
    SERVICES_wait_ms(1500);

    /* Request to power off the Secure Enclave subsystem */
    msg_status = SERVICES_power_se_sleep_req(services_handle, 0, &service_resp);

    SERVICES_wait_ms(50);
  }
#endif

  execute_go_mode_usecase(power_mode);

  return msg_status;
}

/**
* @fn    void SERVICES_test(uint32_t services_handle)
* @brief Example test harness starting point
*/
void SERVICES_test_go(uint32_t services_handle,uint32_t power_mode,char *power_example_type)
{
  TEST_init(services_handle);

  TEST_print(services_handle, "m55_power: %s example\n", power_example_type);

  SERVICES_wait_ms(1500);

  exercise_aipm_go_modes("Exercise GO Modes using aiPM\n",
                              services_handle, power_mode);
}
