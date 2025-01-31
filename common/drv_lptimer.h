#ifndef INC_DRV_LPTIMER_H_
#define INC_DRV_LPTIMER_H_

#include <stdint.h>

typedef void (*ARM_LPTIMER_SignalEvent_t) ();

#define LPTIMER_TICK_RATE                                   (32768)
#define LPTIMER_MAX_CHANNEL_NUMBER                          (4)

#define LPTIMER_CONTROL_REG_TIMER_ENABLE_BIT                0x00000001
#define LPTIMER_CONTROL_REG_TIMER_MODE_BIT                  0x00000002
#define LPTIMER_CONTROL_REG_TIMER_INTERRUPT_MASK_BIT        0x00000004

typedef struct {
    volatile uint32_t load_count;                           /**< Channel load count register >*/
    volatile uint32_t current_value;                        /**< Channel current running count register >*/
    volatile uint32_t control_reg;                          /**< Channel operation control register >*/
    volatile uint32_t eoi;                                  /**< Channel end of interrupt register>*/
    volatile uint32_t int_status;                           /**< Channel Interrupt status register>*/
} CHANNEL_CONTROL_REG;

typedef struct {
    CHANNEL_CONTROL_REG ch_cntrl_reg[LPTIMER_MAX_CHANNEL_NUMBER];   /**< 4 Channels register instance>*/
    volatile const uint32_t reserved[20];
    volatile uint32_t int_status;                                   /**< Interrupt status register >*/
    volatile uint32_t eoi;                                          /**< Interrupt end of interrupt register >*/
    volatile uint32_t raw_int_status;                               /**< raw Interrupt status register >*/
    volatile uint32_t comp_ver;                                     /**< Timer component version info >*/
    volatile uint32_t load_count2[LPTIMER_MAX_CHANNEL_NUMBER];      /**< 4 channel instance of load count 2 register >*/
} LPTIMER_reg_info;

void     LPTIMER_ll_Set_Count_Value(uint8_t channel, uint32_t count);
uint32_t LPTIMER_ll_Get_Count_Value(uint8_t channel);
void     LPTIMER_ll_Initialize     (uint8_t channel);
void     LPTIMER_ll_Start          (uint8_t channel);
uint32_t LPTIMER_ll_Started        (uint8_t channel);
void     LPTIMER_ll_Wait           (uint8_t channel);
void     LPTIMER_ll_Stop           (uint8_t channel);
void     LPTIMER_ll_InterruptClear (uint8_t channel);

#endif /* INC_DRV_LPTIMER_H_ */
