/**
 * @file  services_aipm_stop_modes.c
 *
 * @brief Services Power example BASIC configure clocks and power domains for
 *        STOP Modes 1-5
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

#if defined(M55_HP)
#define PD_RTSS_LOCAL_MASK          PD_RTSS_HP_MASK
#define CLOCK_FREQUENCY_CPU         CLOCK_FREQUENCY_400MHZ
#elif defined(M55_HE)
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

#ifndef HW_REG32
#define HW_REG32(u,v)   (*(volatile uint32_t *)(u + v))
#endif

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

void execute_stop_mode_usecase(uint32_t mode_number)
{
    UNUSED(mode_number);

#if defined(M55_HE)
    /* STOP_1 through STOP_3 will have BOD enabled */
    if (mode_number < 4) {
        ANA_REG->VBAT_ANA_REG3 |= 1U << 8;
    } else {
        ANA_REG->VBAT_ANA_REG3 &= ~(1U << 8);
    }

    /* STOP_5 will have RTC and LFXO disabled */
    if (mode_number < 5) {
        VBAT->RTC_CLK_EN = 1;
    } else {
        VBAT->RTC_CLK_EN = 0;
        ANA_REG->MISC_CTRL &= ~1U;
        ANA_REG->VBAT_ANA_REG1 &= ~(1U << 12);
    }

#endif
    pm_core_enter_deep_sleep_request_subsys_off();
}

void configure_stop_mode_profiles(uint32_t mode_number,
        run_profile_t *runp, off_profile_t *offp)
{
    /* change default run profile */
    runp->cpu_clk_freq      = CLOCK_FREQUENCY_CPU;
    runp->scaled_clk_freq   = SCALED_FREQ_RC_ACTIVE_76_8_MHZ;
    runp->run_clk_src       = CLK_SRC_PLL;
    runp->aon_clk_src       = CLK_SRC_LFXO;
    runp->memory_blocks     = 0;
    runp->power_domains     = 0;
    runp->dcdc_mode         = DCDC_MODE_PWM;
    runp->dcdc_voltage      = DCDC_VOUT_0825;
    runp->vdd_ioflex_3V3    = IOFLEX_LEVEL_1V8;

    /* default off profile doesn't set vdd_ioflex_3V3 correctly */
    offp->wakeup_events     = WE_SERTC | WE_LPRTC | WE_LPTIMER | WE_LPGPIO;
    offp->ewic_cfg          = 0;
    offp->stby_clk_src      = CLK_SRC_HFRC;
    offp->stby_clk_freq     = SCALED_FREQ_RC_ACTIVE_76_8_MHZ;
    offp->power_domains     = 0;    // no power domains will request STOP Mode
    offp->dcdc_mode         = DCDC_MODE_PWM;
    offp->dcdc_voltage      = DCDC_VOUT_0825;
    offp->vdd_ioflex_3V3    = IOFLEX_LEVEL_1V8;
    offp->vtor_address      = 0;
    offp->vtor_address_ns   = 0;

    switch(mode_number) {
    case 1:
        /* STOP_1 through STOP_3 will have BOD enabled */
        offp->aon_clk_src       = CLK_SRC_LFXO;
        offp->memory_blocks     = SRAM4_1_MASK | SRAM4_2_MASK | SRAM5_1_MASK | SRAM5_2_MASK | BACKUP4K_MASK;
        break;
    case 2:
        offp->aon_clk_src       = CLK_SRC_LFXO;
        offp->memory_blocks     = BACKUP4K_MASK;
        break;
    case 3:
        offp->aon_clk_src       = CLK_SRC_LFXO;
        offp->memory_blocks     = 0;
        break;
    case 4:
        offp->aon_clk_src       = CLK_SRC_LFXO;
        offp->memory_blocks     = 0;
        break;
    case 5:
        offp->aon_clk_src       = CLK_SRC_LFRC;
        offp->memory_blocks     = 0;
        break;
    default:
        /* this should not happen */
        break;
    }
}

/**
 * @brief STOP Modes
 *
 * @param p_test_name
 * @param services_handle
 * @return error code
 */
uint32_t exercise_aipm_stop_modes(char *p_test_name,
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
              "STOP_%d\n",
              power_mode);

  configure_stop_mode_profiles(power_mode, &runp, &offp);

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

  SERVICES_wait_ms(1500);

  execute_stop_mode_usecase(power_mode);

  return msg_status;
}

/**
* @fn    void SERVICES_test(uint32_t services_handle)
* @brief Example test harness starting point
*/
void SERVICES_test_stop(uint32_t services_handle, uint32_t power_mode,char *power_example_type)
{
  TEST_init(services_handle);

  TEST_print(services_handle, "m55_power: %s example\n", power_example_type);

  SERVICES_wait_ms(1500);

  exercise_aipm_stop_modes("Exercise STOP Modes using aiPM\n",
                              services_handle,power_mode);
}
