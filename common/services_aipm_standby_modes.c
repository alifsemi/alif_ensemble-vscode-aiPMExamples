/**
 * @file  services_aipm_standby_modes.c
 *
 * @brief Services Power example BASIC configure clocks and power domains
 *        for STBY Mode 1
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

void execute_standby_mode_usecase(uint32_t mode_number)
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
void configure_standby_mode_profiles(uint32_t mode_number,
                                     run_profile_t *runp, off_profile_t *offp)
{
    UNUSED(mode_number);

    runp->cpu_clk_freq      = CLOCK_FREQUENCY_76_8_RC_MHZ;
    /* below should update the 1/X divider which is in effect when any PDs are on */
    runp->scaled_clk_freq   = SCALED_FREQ_RC_STDBY_0_6_MHZ;
    runp->run_clk_src       = CLK_SRC_HFRC;
    runp->aon_clk_src       = CLK_SRC_LFXO;
    runp->memory_blocks     = 0;
    runp->power_domains     = PD_SSE700_AON_MASK;
    runp->dcdc_mode         = DCDC_MODE_PWM;
    runp->dcdc_voltage      = DCDC_VOUT_0825;
    runp->vdd_ioflex_3V3    = IOFLEX_LEVEL_1V8;

    offp->wakeup_events     = 0;
    offp->ewic_cfg          = 0;
    offp->aon_clk_src       = CLK_SRC_LFXO;
    offp->stby_clk_src      = CLK_SRC_HFRC;
    /* below should update the 1/Y divider which is in effect when all PDs are off */
    offp->stby_clk_freq     = SCALED_FREQ_RC_STDBY_0_6_MHZ;
    offp->memory_blocks     = SRAM4_1_MASK | SRAM4_2_MASK | SRAM5_1_MASK | SRAM5_2_MASK | SERAM_MASK;
    offp->power_domains     = PD_SSE700_AON_MASK;
    offp->dcdc_mode         = DCDC_MODE_PWM;
    offp->dcdc_voltage      = DCDC_VOUT_0825;
    offp->vdd_ioflex_3V3    = IOFLEX_LEVEL_1V8;
    offp->vtor_address      = SCB->VTOR;
    offp->vtor_address_ns   = SCB->VTOR;
}

/**
 * @brief STANDBY Modes reference example
 *
 * @param p_test_name
 * @param services_handle
 * @return error code
 */
uint32_t exercise_aipm_standby_modes(char *p_test_name,
                                     uint32_t services_handle, uint32_t power_mode)
{
  uint32_t msg_status = SERVICES_REQ_SUCCESS;
  uint32_t service_resp;
  run_profile_t runp = {0};
  off_profile_t offp = {0};

  TEST_print(services_handle,
              "TEST %s",
              p_test_name);

  TEST_print(services_handle,
              "STANDBY_%d\n",
              power_mode);

  configure_standby_mode_profiles(power_mode, &runp, &offp);

  *(volatile uint32_t*)0x1A010400 = 0; // Switching the systop in host register off
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

#if defined(M55_HE) || defined(E1C_M55_HE) || defined(E8_M55_HE)
  /* Allow HP core to finish running before powering down the SE */
  SERVICES_wait_ms(1500);

  runp.dcdc_voltage = 760;
  runp.dcdc_mode = DCDC_MODE_PFM_FORCED;
  msg_status = SERVICES_set_run_cfg(services_handle, &runp, &service_resp);
  SERVICES_wait_ms(50);

  /* Request to power off the Secure Enclave subsystem */
  msg_status = SERVICES_power_se_sleep_req(services_handle, 0, &service_resp);

  SERVICES_wait_ms(50);

  /* Disable the MRAM LDO */
  *((volatile uint32_t *)0x1A60A03C) &= ~(1U << 5);
#endif

  execute_standby_mode_usecase(power_mode);

  return msg_status;
}


/**
* @fn    void SERVICES_test(uint32_t services_handle)
* @brief Example test harness starting point
*/
void SERVICES_test_standby(uint32_t services_handle, uint32_t power_mode, char *power_example_type)
{
  TEST_init(services_handle);

  TEST_print(services_handle, "m55_power: %s example\n", power_example_type);

  SERVICES_wait_ms(1500);

  exercise_aipm_standby_modes("Exercise STANDBY Modes using aiPM\n",
                              services_handle, power_mode);
}
