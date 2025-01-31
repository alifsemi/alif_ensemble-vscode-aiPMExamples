#include <stdio.h>
#include <stdint.h>

#include "debug_pwr.h"

extern void TEST_print(uint32_t services_handle, char *fmt, ...);

void DEBUG_power_decoded(uint32_t services_handle) {
    uint32_t reg_data;

    /* power domains list */
    TEST_print(services_handle, "Power Domains:\n");
    reg_data = *((uint32_t *)0x1A60A004);
    TEST_print(services_handle, "PD0=AON_STOP\n");
    TEST_print(services_handle, "PD1=%d\n",                  (reg_data & 1U));
    TEST_print(services_handle, "PD2=AON_STBY\n");
    TEST_print(services_handle, "PD3=0x%03X\n",              *((uint32_t *)0x1A601008));
    TEST_print(services_handle, "PD4=0x%X\n",                *((uint32_t *)0x1A605058) & 0xFFFU);
    TEST_print(services_handle, "PD5=1\n");
    TEST_print(services_handle, "EFUSE=0x%X\n",              (reg_data >> 4) & 7U);
    TEST_print(services_handle, "SRAM0=0x%X\n",              (reg_data >> 8) & 3U);// ? 0 : 1);
    TEST_print(services_handle, "SRAM1=0x%X\n",              (reg_data >> 12) & 3U);// ? 0 : 1);
    TEST_print(services_handle, "MRAM=0x%X\n",               (reg_data >> 16) & 3U);
    reg_data = *((uint32_t *)0x1A010404);
    TEST_print(services_handle, "PD6=0x%X\n",                (reg_data >> 3) & 7U);
    TEST_print(services_handle, "PD7=0x%03X\n",              *((uint32_t *)0x1A600008));
    TEST_print(services_handle, "PD8=%d\n",                  (reg_data >> 2) & 1U);
    TEST_print(services_handle, "PD9=n/a\n\n");

    /* m55 vtors & stop mode wakeup source enables */
    TEST_print(services_handle, "VTOR Settings:\n");
    TEST_print(services_handle, "HP_NS_VTOR=0x%08X\n",       *((uint32_t *)0x1A605010));
    TEST_print(services_handle, "HP_S_VTOR =0x%08X\n",       *((uint32_t *)0x1A605014));
    TEST_print(services_handle, "HE_NS_VTOR=0x%08X\n",       *((uint32_t *)0x1A60A020));
    TEST_print(services_handle, "HE_S_VTOR =0x%08X\n",       *((uint32_t *)0x1A60A024));
    TEST_print(services_handle, "VBAT_WKUP =0x%08X\n\n",     *((uint32_t *)0x1A60A008));

    /* retention settings */
    TEST_print(services_handle, "RET Settings:\n");
    reg_data = *((uint32_t *)0x1A60A038);   // VBAT_ANA_REG1
    TEST_print(services_handle, "LDO VOUT=%d\n",                 (reg_data >> 4) & 15U);
    TEST_print(services_handle, "LDO VBAT_EN=%d MAIN_EN=%d\n",   (reg_data >> 8) & 1U, (reg_data >> 10) & 1U);
    reg_data = *((uint32_t *)0x1A60900C);   // VBATALL RET_CTRL
    TEST_print(services_handle, "BK RAM=0x%X HE TCM=0x%X\n",     (reg_data & 3U), (reg_data >> 4) & 15U);
    reg_data = *((uint32_t *)0x1A60A018);   // VBATSEC RET_CTRL
    TEST_print(services_handle, "FW RAM=0x%X SE RAM=0x%X\n\n",   (reg_data & 3U), (reg_data >> 4) & 3U);

    TEST_print(services_handle, "PHY Power:\n");
    reg_data = *((uint32_t *)0x1A609008);   // VBATALL PWR_CTRL
    TEST_print(services_handle, "PWR_CTRL=0x%08X\n\n", reg_data);

    /* bandgap trim settings */
    TEST_print(services_handle, "TRIM Settings:\n");
    reg_data = *((uint32_t *)0x1A60A038);   // VBAT_ANA_REG1
    TEST_print(services_handle, "RC OSC 32.768k=%d\n",       (reg_data & 15U));
    reg_data = *((uint32_t *)0x1A60A03C);   // VBAT_ANA_REG2
    TEST_print(services_handle, "RC OSC 76.800M=%d.%d\n",    (reg_data >> 14) & 31U, (reg_data >> 10) & 1U);
    TEST_print(services_handle, "DIG LDO VOUT=%d EN=%d\n",   (reg_data >> 6) & 15U, (reg_data >> 5) & 1U);
    TEST_print(services_handle, "Bandgap PMU=%d\n",          (reg_data >> 1) & 15U);
    reg_data = *((uint32_t *)0x1A60A040);   // VBAT_ANA_REG2
    TEST_print(services_handle, "Bandgap AON=%d\n",          (reg_data >> 23) & 15U);
    TEST_print(services_handle, "AON LDO VOUT=%d\n",         (reg_data >> 27) & 15U);
    TEST_print(services_handle, "MAIN LDO VOUT=%d\n\n",      (reg_data & 7U));

    /* miscellaneous */
    TEST_print(services_handle, "MISC Settings:\n");
    TEST_print(services_handle, "GPIO_FLEX=%s\n",            ((uint32_t *)0x1A609000) ? "1.8V" : "3.3V");
    reg_data = *((uint32_t *)0x1A60A034);
    TEST_print(services_handle, "DCDC_MODE=%d\n",            (reg_data >> 6) & 3U);
    reg_data = *((uint32_t *)0x1A60A030);
    TEST_print(services_handle, "DCDC_SEL[4:3]=%d\n",        (reg_data >> 3) & 3U);
    TEST_print(services_handle, "DCDC_TRIM[8:6]=%d\n\n",     (reg_data >> 6) & 7U);
}

void DEBUG_power(uint32_t services_handle) {
    TEST_print(services_handle, "aonexp_all_regs\n");
    TEST_print(services_handle, "ES0_CTRL:\t0x%08X\n",       *((uint32_t *)0x1A604000));
    TEST_print(services_handle, "ES0_RESET:\t0x%08X\n",      *((uint32_t *)0x1A604004));
    TEST_print(services_handle, "ES1_CTRL:\t0x%08X\n",       *((uint32_t *)0x1A604010));
    TEST_print(services_handle, "ES1_RESET:\t0x%08X\n",      *((uint32_t *)0x1A604014));
    TEST_print(services_handle, "LPUART_CLK:\t0x%08X\n",     *((uint32_t *)0x1A60401C));
    TEST_print(services_handle, "SYSTOP_CLK:\t0x%08X\n",     *((uint32_t *)0x1A604020));
    TEST_print(services_handle, "MISC_REG1:\t0x%08X\n",      *((uint32_t *)0x1A604030));
    TEST_print(services_handle, "ANATOP_REG1:\t0x%08X\n",    *((uint32_t *)0x1A604034));
    TEST_print(services_handle, "ANATOP_READ:\t0x%08X\n",    *((uint32_t *)0x1A604038));
    TEST_print(services_handle, "PERIPH_REG1:\t0x%08X\n",    *((uint32_t *)0x1A604040));

    TEST_print(services_handle, "aonexp_secenc_regs\n");
    TEST_print(services_handle, "SE_CTRL:\t0x%08X\n",        *((uint32_t *)0x1A605000));
    TEST_print(services_handle, "BISR_STAT:\t0x%08X\n",      *((uint32_t *)0x1A605004));
    TEST_print(services_handle, "HP_NS_VTOR:\t0x%08X\n",     *((uint32_t *)0x1A605010));
    TEST_print(services_handle, "HP_S_VTOR:\t0x%08X\n",      *((uint32_t *)0x1A605014));
    TEST_print(services_handle, "XO_REG1:\t0x%08X\n",        *((uint32_t *)0x1A605020));
    TEST_print(services_handle, "PLL_REG1:\t0x%08X\n",       *((uint32_t *)0x1A605024));
    TEST_print(services_handle, "PLL_REG2:\t0x%08X\n",       *((uint32_t *)0x1A605028));
    TEST_print(services_handle, "PLL_REG3:\t0x%08X\n",       *((uint32_t *)0x1A60502C));

    TEST_print(services_handle, "vbatall_regs\n");
    TEST_print(services_handle, "GPIO_CTRL:\t0x%08X\n",      *((uint32_t *)0x1A609000));
    TEST_print(services_handle, "TIMER_SEL:\t0x%08X\n",      *((uint32_t *)0x1A609004));
    TEST_print(services_handle, "PWR_CTRL:\t0x%08X\n",       *((uint32_t *)0x1A609008));
    TEST_print(services_handle, "RET_CTRL:\t0x%08X\n",       *((uint32_t *)0x1A60900C));
    TEST_print(services_handle, "RTC_CLK:\t0x%08X\n",        *((uint32_t *)0x1A609010));

    TEST_print(services_handle, "vbatsec_regs\n");
    TEST_print(services_handle, "MISC_CTRL:\t0x%08X\n",      *((uint32_t *)0x1A60A000));
    TEST_print(services_handle, "PWR_CTRL:\t0x%08X\n",       *((uint32_t *)0x1A60A004));
    TEST_print(services_handle, "WKUP_CTRL:\t0x%08X\n",      *((uint32_t *)0x1A60A008));
    TEST_print(services_handle, "RET_CTRL:\t0x%08X\n",       *((uint32_t *)0x1A60A018));
    TEST_print(services_handle, "HE_NS_VTOR:\t0x%08X\n",     *((uint32_t *)0x1A60A020));
    TEST_print(services_handle, "HE_S_VTOR:\t0x%08X\n",      *((uint32_t *)0x1A60A024));
    TEST_print(services_handle, "DCDC_REG1:\t0x%08X\n",      *((uint32_t *)0x1A60A030));
    TEST_print(services_handle, "DCDC_REG2:\t0x%08X\n",      *((uint32_t *)0x1A60A034));
    TEST_print(services_handle, "VBAT_ANA_REG1:\t0x%08X\n",  *((uint32_t *)0x1A60A038));
    TEST_print(services_handle, "VBAT_ANA_REG2:\t0x%08X\n",  *((uint32_t *)0x1A60A03C));
    TEST_print(services_handle, "VBAT_ANA_REG3:\t0x%08X\n",  *((uint32_t *)0x1A60A040));

    TEST_print(services_handle, "vbatmdm_regs\n");
    TEST_print(services_handle, "RET_CTRL:\t0x%08X\n",       *((uint32_t *)0x1A60B000));
    TEST_print(services_handle, "MDM_CTRL:\t0x%08X\n",       *((uint32_t *)0x1A60B004));
    TEST_print(services_handle, "PWR_CTRL:\t0x%08X\n",       *((uint32_t *)0x1A60B008));
}
