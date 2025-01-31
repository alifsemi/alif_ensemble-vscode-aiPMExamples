/**
 * @file vbat_rtc.c
 *
 * @brief Source file for VBAT RTC timers
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2021 Alif Group. All rights reserved.
 * @ingroup DRIVERS
 */

/******************************************************************************
 *  I N C L U D E   F I L E S
 *****************************************************************************/
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
//#include "corestone.h"
//#include "irq_map.h"

/*******************************************************************************
 *  M A C R O   D E F I N E S
 ******************************************************************************/

#define SET_REGISTER_BITS_U32(Address, Value)  \
                  (*(volatile uint32_t *)(Address) |= (uint32_t)(Value))
#define CLEAR_REGISTER_BITS_U32(Address, Value)  \
                  (*(volatile uint32_t *)(Address) &= ~((uint32_t)(Value)))
#define WRITE_REGISTER_U32(Address, Value)  \
                        (*((uint32_t volatile*)(Address)) = (uint32_t)(Value))
#define READ_REGISTER_U32(Address)        (*((uint32_t volatile *)(Address)))

#define VBAT_RTC_A_CLK_ENABLE     (0x1A609000 + 0x10)

#define RTC_CCVR_ADDR(base_address)                   (base_address)
#define RTC_CMR_ADDR(base_address)                    (base_address + 0x4)
#define RTC_CLR_ADDR(base_address)                    (base_address + 0x8)
#define RTC_CCR_ADDR(base_address)                    (base_address + 0xC)
#define RTC_STAT_ADDR(base_address)                   (base_address + 0x10)
#define RTC_RSTAT_ADDR(base_address)                  (base_address + 0x14)
#define RTC_EOI_ADDR(base_address)                    (base_address + 0x18)
#define RTC_COMP_ADDR(base_address)                   (base_address + 0x1C)
#define RTC_CPSR_ADDR(base_address)                   (base_address + 0x20)
#define RTC_CPCVR_ADDR(base_address)                  (base_address + 0x24)

#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04

/*******************************************************************************
 *  T Y P E D E F S
 ******************************************************************************/

/*******************************************************************************
 *  C O D E
 ******************************************************************************/

/**
 * @brief Function to enable clock for RTC_A
 * @param enable
 */
void VBAT_rtc_a_clock_enable(bool enable)
{
  if (enable)
  {
    //SET_REGISTER_BITS_U32(VBAT_RTC_A_CLK_ENABLE, BIT0);
    WRITE_REGISTER_U32(VBAT_RTC_A_CLK_ENABLE, 0x1);
  }
  else
  {
    //CLEAR_REGISTER_BITS_U32(VBAT_RTC_A_CLK_ENABLE, BIT0);
    WRITE_REGISTER_U32(VBAT_RTC_A_CLK_ENABLE, 0x0);
  }
}

/**
 * @brief Function to clear RTC_A interrupt
 * @param base_address
 */
void VBAT_rtc_interrupt_clear(uint32_t base_address)
{
  READ_REGISTER_U32(RTC_EOI_ADDR(base_address));
}

/**
 * @brief Function to enable RTC_A interrupt
 * @param base_address
 * @param enable
 */
void VBAT_rtc_interrupt_enable(uint32_t base_address, bool enable)
{
  if (enable)
  {
    SET_REGISTER_BITS_U32(RTC_CCR_ADDR(base_address), BIT0);
  }
  else
  {
    CLEAR_REGISTER_BITS_U32(RTC_CCR_ADDR(base_address), BIT0);
  }
}

/**
 * @brief Function to enable RTC_A counter
 * @param base_address
 * @param enable
 */
void VBAT_rtc_counter_enable(uint32_t base_address, bool enable)
{
  if (enable)
  {
    SET_REGISTER_BITS_U32(RTC_CCR_ADDR(base_address), BIT2);
  }
  else
  {
    CLEAR_REGISTER_BITS_U32(RTC_CCR_ADDR(base_address), BIT2);
  }
}

/**
 * @brief Function to load RTC_A count value
 * @param base_address
 * @param load_value
 */
void VBAT_rtc_set_load_count(uint32_t base_address, uint32_t load_value)
{
  WRITE_REGISTER_U32(RTC_CLR_ADDR(base_address), load_value);
}

/**
 * @brief Function to set RTC_A match value
 * @param base_address
 * @param counter_match_value
 */
void VBAT_rtc_set_counter_match_value(uint32_t base_address,
                                      uint32_t counter_match_value)
{
  WRITE_REGISTER_U32(RTC_CMR_ADDR(base_address), counter_match_value);
}

/**
 * @brief Function to stop RTC_A
 * @param base_address
 */
void VBAT_rtc_stop(uint32_t base_address)
{
  // Clear rtc_ien
  VBAT_rtc_interrupt_enable(base_address, false);
  // Clear interrupt
  VBAT_rtc_interrupt_clear(base_address);
  // Clear rtc_en
  VBAT_rtc_counter_enable(base_address, false);
  VBAT_rtc_set_load_count(base_address, 0x1);
}

/**
 * @brief Function to start RTC_A
 * @param base_address
 * @param counter_match_value
 */
void VBAT_rtc_start(uint32_t base_address, uint32_t counter_match_value)
{
  // Stop timer
  VBAT_rtc_stop(base_address);

  // Counter load. counter increments from here
  VBAT_rtc_set_load_count(base_address, 0x1);
  // Counter match value
  VBAT_rtc_set_counter_match_value(base_address, counter_match_value);
  // Enable interrupt
  VBAT_rtc_interrupt_enable(base_address, true);
  // Start counter
  VBAT_rtc_counter_enable(base_address, true);
}

/**
 * @brief Function to initialize RTC_A
 */
void VBAT_rtc_a_initialize(void)
{
  VBAT_rtc_a_clock_enable(true);
}
