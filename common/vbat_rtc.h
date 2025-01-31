/**
 * @file vbat_rtc.h
 *
 * @brief Header file for vbat rtc module
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2022 Alif Group. All rights reserved.
 */

#ifndef __VBAT_RTC_H__
#define __VBAT_RTC_H__


/******************************************************************************
 *  I N C L U D E   F I L E S
 *****************************************************************************/


/*******************************************************************************
 *  M A C R O   D E F I N E S
 ******************************************************************************/


/*******************************************************************************
 *  T Y P E D E F S
 ******************************************************************************/


/*******************************************************************************
 *  F U N C T I O N   P R O T O T Y P E S
 ******************************************************************************/

void VBAT_rtc_interrupt_clear(uint32_t base_address);

void VBAT_rtc_a_initialize(void);
void VBAT_rtc_a_clock_enable(bool enable);

void VBAT_rtc_start(uint32_t base_address, uint32_t counter_match_value);
void VBAT_rtc_stop(uint32_t base_address);


#endif /* __VBAT_RTC_H__ */
