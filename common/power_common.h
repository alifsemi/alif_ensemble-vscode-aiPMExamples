/**
 * @file power.h
 *
 * @brief Header file for power functions
 * @defgroup host_services Host Services
 * @par
 *
 * COPYRIGHT NOTICE: (c) 2023 Alif Group. All rights reserved.
 */

#ifndef __SERVICES_POWER_H__
#define __SERVICES_POWER_H__

/******************************************************************************
 *  I N C L U D E   F I L E S
 *****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 *  M A C R O   D E F I N E S
 ******************************************************************************/

#define UNUSED(x) (void)(x)

#define SET_REGISTER_BITS_U32(Address, Value)  \
                  (*(volatile uint32_t *)(Address) |= (uint32_t)(Value))
#define CLEAR_REGISTER_BITS_U32(Address, Value)  \
                  (*(volatile uint32_t *)(Address) &= ~((uint32_t)(Value)))
#define CLEAR_AND_SET_BITS_U32(Address, AndData, OrData)  \
            WRITE_REGISTER_U32(Address, (READ_REGISTER_U32(Address) & ~(uint32_t)(AndData)) | (uint32_t)(OrData))
#define READ_REGISTER_U32(Address)        (*((uint32_t volatile *)(Address)))
#define WRITE_REGISTER_U32(Address, Value)  \
                        (*((uint32_t volatile*)(Address)) = (uint32_t)(Value))

/*
 * 0x50000   original value
 * 0x1000    delay approximately 112 msec in simulation
 * 0x200     delay approximately  4 msec in simulation
 */
#ifndef TIMER_LOAD_VALUE
//#define TIMER_LOAD_VALUE    0x25000
#define TIMER_LOAD_VALUE    0x120000
#endif

#define LP_TIMER_LOAD_VALUE           0x00110000
#define LP_TIMER_LOAD_VALUE_25_SEC    0x000C3410 // 25s
#define LP_TIMER_LOAD_VALUE_12_5_SEC  0x00061a08 // 12.5s

/* LPTIMER Control Register bit Definition */
#define LPTIMER_CONTROL_REG_TIMER_ENABLE_BIT                0x01U
#define LPTIMER_CONTROL_REG_TIMER_MODE_BIT                  0x02U
#define LPTIMER_CONTROL_REG_TIMER_INTERRUPT_MASK_BIT        0x04U
#define LPTIMER_CONTROL_REG_TIMER_PWM_BIT                   0x08U
#define LPTIMER_CONTROL_REG_TIMER_ON_100PWM_BIT             0x10U
#define LP_TIMER_BASE         0x42001014
#define LP_TIMER_LOADCOUNT    (LP_TIMER_BASE)
#define LP_TIMER_CURRENTVAL   (LP_TIMER_BASE + 0x4)
#define LP_TIMER_CONTROLREG   (LP_TIMER_BASE + 0x8)
#define LP_TIMER_EOI          (LP_TIMER_BASE + 0xC)
#define LP_TIMER_INTSTAT      (LP_TIMER_BASE + 0x10)


/*******************************************************************************
 *  T Y P E D E F S
 ******************************************************************************/


/*******************************************************************************
 *  G L O B A L   D E F I N E S
 ******************************************************************************/

/*******************************************************************************
 *  F U N C T I O N   P R O T O T Y P E S
 ******************************************************************************/

int POWER_start_rtc_a(uint32_t count);
void POWER_stop_rtc_a(void);
void POWER_set_lptimer(uint32_t count);
void POWER_lptimer_clear_intr(void);


#ifdef __cplusplus
}
#endif
#endif /* __SERVICES_POWER_H__ */
