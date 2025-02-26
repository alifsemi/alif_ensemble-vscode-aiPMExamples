/**
 * @file  services_aipm_idle_modes.c
 *
 * @brief Services Power example BASIC configure clocks and power domains
 *        for IDLE Modes 1-2
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

void execute_idle_mode_usecase(uint32_t mode_number)
{
    UNUSED(mode_number);

    while(1) 
    {
      __disable_irq();
      pm_core_enter_deep_sleep_request_subsys_off();
      __enable_irq();
    }
}

/**
 * @brief
 *
 * @param mode_number
 * @param runp
 * @param offp
 */
void configure_idle_mode_profiles(uint32_t mode_number,
                                  run_profile_t *runp, off_profile_t *offp)
{
    switch(mode_number) {
    case 1:
        runp->cpu_clk_freq      = CLOCK_FREQUENCY_38_4_XO_MHZ;
        runp->scaled_clk_freq   = SCALED_FREQ_RC_ACTIVE_38_4_MHZ;
        runp->run_clk_src       = CLK_SRC_HFXO;
        runp->aon_clk_src       = CLK_SRC_LFXO;
        runp->memory_blocks     = 0;
        runp->power_domains     = PD_SYST_MASK;
        runp->dcdc_mode         = DCDC_MODE_PFM_FORCED;
        runp->dcdc_voltage      = DCDC_VOUT_0825;
        runp->vdd_ioflex_3V3    = IOFLEX_LEVEL_1V8;

        offp->wakeup_events     = 0;    // only applicable in STOP Mode
        offp->ewic_cfg          = 0;    // only applicable in STOP Mode
        offp->aon_clk_src       = CLK_SRC_LFXO;
        offp->stby_clk_src      = CLK_SRC_HFXO;
        offp->stby_clk_freq     = SCALED_FREQ_RC_STDBY_38_4_MHZ;
        offp->memory_blocks     = BACKUP4K_MASK;
        offp->power_domains     = PD_SYST_MASK;
        offp->dcdc_mode         = DCDC_MODE_PFM_FORCED;
        offp->dcdc_voltage      = DCDC_VOUT_0825;
        offp->vdd_ioflex_3V3    = IOFLEX_LEVEL_1V8;
        offp->vtor_address      = SCB->VTOR;
        offp->vtor_address_ns   = SCB->VTOR;
        break;
    case 2:
        runp->cpu_clk_freq      = CLOCK_FREQUENCY_76_8_RC_MHZ;
        runp->scaled_clk_freq   = SCALED_FREQ_RC_ACTIVE_38_4_MHZ;    // will scale this down to 600kHz after printing debug info
        runp->run_clk_src       = CLK_SRC_HFRC;
        runp->aon_clk_src       = CLK_SRC_LFXO;
        runp->memory_blocks     = 0;
        runp->power_domains     = PD_SYST_MASK;
        runp->dcdc_mode         = DCDC_MODE_PFM_FORCED;
        runp->dcdc_voltage      = DCDC_VOUT_0825;
        runp->vdd_ioflex_3V3    = IOFLEX_LEVEL_1V8;

        offp->wakeup_events     = 0;    // only applicable in STOP Mode
        offp->ewic_cfg          = 0;    // only applicable in STOP Mode
        offp->aon_clk_src       = CLK_SRC_LFXO;
        offp->stby_clk_src      = CLK_SRC_HFRC;
        offp->stby_clk_freq     = SCALED_FREQ_RC_STDBY_0_6_MHZ;
        offp->memory_blocks     = BACKUP4K_MASK | SRAM5_1_MASK | SRAM5_2_MASK ;
        offp->power_domains     = PD_SYST_MASK;
        offp->dcdc_mode         = DCDC_MODE_PFM_FORCED;
        offp->dcdc_voltage      = DCDC_VOUT_0825;
        offp->vdd_ioflex_3V3    = IOFLEX_LEVEL_1V8;
        offp->vtor_address      = SCB->VTOR;
        offp->vtor_address_ns   = SCB->VTOR;
        break;
    default:
        /* this should not happen */
        break;
    }
}

/**
 * @brief IDLE Modes reference example
 *
 * @param p_test_name
 * @param services_handle
 * @return error code
 */
uint32_t exercise_aipm_idle_modes(char *p_test_name,
                                     uint32_t services_handle,uint32_t power_mode)
{
  uint32_t msg_status = SERVICES_REQ_SUCCESS;
  uint32_t service_resp;
  run_profile_t runp = {0};
  off_profile_t offp = {0};

  TEST_print(services_handle,
              "TEST %s",
              p_test_name);

  TEST_print(services_handle,
              "IDLE_%d\n",
              power_mode);

  configure_idle_mode_profiles(power_mode, &runp, &offp);

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

#if defined(M55_HE) 
  if (power_mode == 2)
  {
    /* Allow HP core to finish running before next steps */
     SERVICES_wait_ms(1500);

     runp.scaled_clk_freq   = SCALED_FREQ_RC_ACTIVE_0_6_MHZ;

     msg_status = SERVICES_set_run_cfg(services_handle, &runp, &service_resp);

     /* Allow HP core to finish running before powering down the SE */
     SERVICES_wait_ms(1500);

     /* Request to power off the Secure Enclave subsystem */
     msg_status = SERVICES_power_se_sleep_req(services_handle, 0, &service_resp);
  }

#endif

  execute_idle_mode_usecase(power_mode);

  return msg_status;
}

/**
* @fn    void SERVICES_test(uint32_t services_handle)
* @brief Example test harness starting point
*/
void SERVICES_test_idle(uint32_t services_handle, uint32_t power_mode,char *power_example_type)
{
  TEST_init(services_handle);

  TEST_print(services_handle, "m55_power: %s example\n", power_example_type);

  SERVICES_wait_ms(1500);

  exercise_aipm_idle_modes("Exercise IDLE Modes using aiPM\n",
                              services_handle,power_mode);
}
