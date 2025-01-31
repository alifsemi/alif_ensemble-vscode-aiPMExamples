#include "RTE_Components.h"
#include CMSIS_device_header
#include "drv_lptimer.h"

#define LPTIMER_BASE    0x42001000UL

void LPTIMER_ll_Initialize (uint8_t channel) {

    LPTIMER_reg_info *reg_ptr = (LPTIMER_reg_info*) LPTIMER_BASE;

    reg_ptr->ch_cntrl_reg[channel].control_reg &= ~LPTIMER_CONTROL_REG_TIMER_ENABLE_BIT;

	reg_ptr->ch_cntrl_reg[channel].control_reg |=  LPTIMER_CONTROL_REG_TIMER_MODE_BIT;

    reg_ptr->ch_cntrl_reg[channel].control_reg &= ~LPTIMER_CONTROL_REG_TIMER_INTERRUPT_MASK_BIT;

    *((uint32_t *)(0x1A609000 + 0x4)) &= ~(0x3UL << (channel * 4));
}

void LPTIMER_ll_Set_Count_Value (uint8_t channel, uint32_t count) {

    LPTIMER_reg_info *reg_ptr = (LPTIMER_reg_info*) LPTIMER_BASE;

    reg_ptr->ch_cntrl_reg[channel].load_count = count;
    reg_ptr->ch_cntrl_reg[channel].current_value = count;
}

uint32_t LPTIMER_ll_Get_Count_Value (uint8_t channel) {

    LPTIMER_reg_info *reg_ptr = (LPTIMER_reg_info*) LPTIMER_BASE;

    return reg_ptr->ch_cntrl_reg[channel].current_value;
}

void LPTIMER_ll_Start (uint8_t channel) {

    LPTIMER_reg_info *reg_ptr = (LPTIMER_reg_info*) LPTIMER_BASE;

    reg_ptr->ch_cntrl_reg[channel].control_reg |= LPTIMER_CONTROL_REG_TIMER_ENABLE_BIT;
}

uint32_t LPTIMER_ll_Started (uint8_t channel) {

    LPTIMER_reg_info *reg_ptr = (LPTIMER_reg_info*) LPTIMER_BASE;

    return reg_ptr->ch_cntrl_reg[channel].control_reg & LPTIMER_CONTROL_REG_TIMER_ENABLE_BIT;
}

void LPTIMER_ll_Stop (uint8_t channel) {

    LPTIMER_reg_info *reg_ptr = (LPTIMER_reg_info*) LPTIMER_BASE;

    reg_ptr->ch_cntrl_reg[channel].control_reg &= ~LPTIMER_CONTROL_REG_TIMER_ENABLE_BIT;
}

void LPTIMER_ll_Wait (uint8_t channel) {

    LPTIMER_reg_info *reg_ptr = (LPTIMER_reg_info*) LPTIMER_BASE;

    while (1) {
        if (reg_ptr->ch_cntrl_reg[channel].int_status)
            break;
    }

    (void) reg_ptr->ch_cntrl_reg[channel].eoi;
}

void LPTIMER_ll_InterruptClear (uint8_t channel) {

    LPTIMER_reg_info *reg_ptr = (LPTIMER_reg_info*) LPTIMER_BASE;

    (void) reg_ptr->ch_cntrl_reg[channel].eoi;
}
