/**
 * @file  power.c
 *
 * @brief Source file for power functions
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
#include <inttypes.h>
#include <stdbool.h>
#include "vbat_rtc.h"
#include "services_lib_interface.h"
#include "power_common.h"

/*******************************************************************************
 *  M A C R O   D E F I N E S
 ******************************************************************************/

#define SET_REGISTER_BITS_U32(Address, Value)  \
                  (*(volatile uint32_t *)(Address) |= (uint32_t)(Value))
#define CLEAR_REGISTER_BITS_U32(Address, Value)  \
                  (*(volatile uint32_t *)(Address) &= ~((uint32_t)(Value)))
#define CLEAR_AND_SET_BITS_U32(Address, AndData, OrData)  \
            WRITE_REGISTER_U32(Address, (READ_REGISTER_U32(Address) & ~(uint32_t)(AndData)) | (uint32_t)(OrData))
#define READ_REGISTER_U32(Address)        (*((uint32_t volatile *)(Address)))
#define WRITE_REGISTER_U32(Address, Value)  \
                        (*((uint32_t volatile*)(Address)) = (uint32_t)(Value))

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
 * @brief LP timer set up
 * @param lptimer_n
 */
void POWER_set_lptimer(uint32_t count)
{
  // Disable timer
  CLEAR_REGISTER_BITS_U32(LP_TIMER_CONTROLREG,
                          LPTIMER_CONTROL_REG_TIMER_ENABLE_BIT);

  // Free running mode
  CLEAR_REGISTER_BITS_U32(LP_TIMER_CONTROLREG,
                          LPTIMER_CONTROL_REG_TIMER_MODE_BIT);

  // Unmask interrupt
  CLEAR_REGISTER_BITS_U32(LP_TIMER_CONTROLREG,
                          LPTIMER_CONTROL_REG_TIMER_INTERRUPT_MASK_BIT);

  // Enable wake up source from LP timer
  CLEAR_AND_SET_BITS_U32(0x1A60A008, (2 << 8), (2 << 8));

  // Load count
  WRITE_REGISTER_U32(LP_TIMER_LOADCOUNT, count);

  // Enable timer
  SET_REGISTER_BITS_U32(LP_TIMER_CONTROLREG,
                        LPTIMER_CONTROL_REG_TIMER_ENABLE_BIT);
}

void POWER_lptimer_clear_intr(void)
{
  (void) *((volatile uint32_t *)LP_TIMER_EOI);
}

/**
 * @brief Function to initialize stop mode service
 * @return
 */
int POWER_start_rtc_a(uint32_t count)
{
  // Init RTC_A
  VBAT_rtc_a_initialize();

  // Start RTC_A
  VBAT_rtc_start(0x42000000ul, count);

  return 0;
}

/**
 * @brief Function to end stop mode service
 */
void POWER_stop_rtc_a(void)
{
  VBAT_rtc_stop(0x42000000ul);
}

/**
 * @brief RTC_A interrupt handler
 */
void LPRTC_IRQHandler(void)
{
  POWER_stop_rtc_a();
}
