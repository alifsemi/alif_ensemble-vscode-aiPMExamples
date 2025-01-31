#include <stdio.h>
#include <stdint.h>

#include "debug_clks.h"

extern void TEST_print(uint32_t services_handle, char *fmt, ...);

uint32_t DEBUG_lf_xtal_enabled() {
    uint32_t reg_data;
    reg_data = *((volatile uint32_t *)(0x1A60A038));
    reg_data = (reg_data >> 12) & 1;
    return reg_data;
}

uint32_t DEBUG_hf_xtal_enabled() {
    uint32_t reg_data;
    reg_data = *((volatile uint32_t *)(0x1A605020));
    reg_data &= 1;
    return reg_data;
}

uint32_t DEBUG_pll_enabled() {
    uint32_t reg_data;
    reg_data = *((volatile uint32_t *)(0x1A602004));
    reg_data &= 1;
    return reg_data;
}

uint32_t DEBUG_lf_osc_source() {
    uint32_t reg_data;
    reg_data = *((volatile uint32_t *)(0x1A60A000));
    reg_data &= 1;
    return reg_data;
}

uint32_t DEBUG_hf_osc_source() {
    uint32_t reg_data;
    reg_data = *((volatile uint32_t *)(0x1A602000));
    reg_data &= 0x11;
    return reg_data;
}

uint32_t DEBUG_pll_clk_select() {
    uint32_t reg_data;
    reg_data = *((volatile uint32_t *)(0x1A602008));
    return reg_data;
}

uint32_t DEBUG_extsys_clk_select() {
    uint32_t reg_data;
    reg_data = *((volatile uint32_t *)(0x1A602010));
    return reg_data;
}

uint32_t DEBUG_clk_enabled() {
    uint32_t reg_data;
    reg_data = *((volatile uint32_t *)(0x1A602014));
    return reg_data;
}

uint32_t DEBUG_hostcpuclk_status() {
    /* HOSTCPUCLK_CTRL value at [7:0] and
     * HOSTCPUCLK_DIV0 value at [15:8] and
     * HOSTCPUCLK_DIV1 value at [23:16] */
    uint32_t reg_data, ret_data = 0;
    reg_data = *((volatile uint32_t *)(0x1A010800));
    ret_data |= (reg_data >> 8) & 0xFF;
    reg_data = *((volatile uint32_t *)(0x1A010804));
    ret_data |= ((reg_data >> 16) & 0xFF) << 8;
    reg_data = *((volatile uint32_t *)(0x1A010808));
    ret_data |= ((reg_data >> 16) & 0xFF) << 16;
    return ret_data;
}

uint32_t DEBUG_gicclk_status() {
    /* GICCLK_CTRL value at [7:0] and GICCLK_DIV0 value at [15:8] */
    uint32_t reg_data, ret_data = 0;
    reg_data = *((volatile uint32_t *)(0x1A010810));
    ret_data |= (reg_data >> 8) & 0xFF;
    reg_data = *((volatile uint32_t *)(0x1A010814));
    ret_data |= ((reg_data >> 16) & 0xFF) << 8;
    return ret_data;
}

uint32_t DEBUG_ctrlclk_status() {
    /* CTRLCLK_CTRL value at [7:0] and CTRLCLK_DIV0 value at [15:8] */
    uint32_t reg_data, ret_data = 0;
    reg_data = *((volatile uint32_t *)(0x1A010830));
    ret_data |= (reg_data >> 8) & 0xFF;
    reg_data = *((volatile uint32_t *)(0x1A010834));
    ret_data |= ((reg_data >> 16) & 0xFF) << 8;
    return ret_data;
}

uint32_t DEBUG_dbgclk_status() {
    /* DBGCLK_CTRL value at [7:0] and DBGCLK_DIV0 value at [15:8] */
    uint32_t reg_data, ret_data = 0;
    reg_data = *((volatile uint32_t *)(0x1A010840));
    ret_data |= (reg_data >> 8) & 0xFF;
    reg_data = *((volatile uint32_t *)(0x1A010844));
    ret_data |= ((reg_data >> 16) & 0xFF) << 8;
    return ret_data;
}

uint32_t DEBUG_hsuartclk_status() {
    /* HOSTUARTCLK_CTRL value at [7:0] and HOSTUARTCLK_DIV0 value at [15:8] */
    uint32_t reg_data, ret_data = 0;
    reg_data = *((volatile uint32_t *)(0x1A010850));
    ret_data |= (reg_data >> 8) & 0xFF;
    reg_data = *((volatile uint32_t *)(0x1A010854));
    ret_data |= ((reg_data >> 16) & 0xFF) << 8;
    return ret_data;
}

uint32_t DEBUG_aclk_status() {
    /* ACLK_CTRL value at [7:0] and ACLK_DIV0 value at [15:8] */
    uint32_t reg_data, ret_data = 0;
    reg_data = *((volatile uint32_t *)(0x1A010820));
    ret_data |= (reg_data >> 8) & 0xFF;
    reg_data = *((volatile uint32_t *)(0x1A010824));
    ret_data |= ((reg_data >> 16) & 0xFF) << 8;
    return ret_data;
}

uint32_t DEBUG_hclk_pclk_status() {
    uint32_t reg_data;
    reg_data = *((volatile uint32_t *)(0x1A604020));
    return reg_data;
}

uint32_t DEBUG_hfrc_divider() {
    /* 1/X value at [7:0] and 1/Y value at [15:8] */
    uint32_t reg_data, ret_data = 0;
    reg_data = *((volatile uint32_t *)(0x1A60A03C));
    ret_data |= (reg_data >> 11) & 7;
    ret_data |= ((reg_data >> 19) & 7) << 8;
    return ret_data;
}

uint32_t DEBUG_hf_xtal_divider() {
    uint32_t reg_data;
    reg_data = *((volatile uint32_t *)(0x1A604030));
    reg_data = (reg_data >> 13) & 15;
    return reg_data;
}

void DEBUG_clocks(uint32_t services_handle) {
    TEST_print(services_handle, "LFXO Enabled:\t%d\n",       DEBUG_lf_xtal_enabled());
    TEST_print(services_handle, "HFXO Enabled:\t%d\n",       DEBUG_hf_xtal_enabled());
    TEST_print(services_handle, "PLL  Enabled:\t%d\n",       DEBUG_pll_enabled());
    TEST_print(services_handle, "LF OSC Select:\t%d\n",      DEBUG_lf_osc_source());
    TEST_print(services_handle, "HF OSC Select:\t0x%02X\n",  DEBUG_hf_osc_source());
    TEST_print(services_handle, "PLL CLK Select:\t0x%06X\n", DEBUG_pll_clk_select());
    TEST_print(services_handle, "ESx CLK Select:\t0x%04X\n", DEBUG_extsys_clk_select());
    TEST_print(services_handle, "CLK Enabled: \t0x%08X\n",   DEBUG_clk_enabled());
    TEST_print(services_handle, "A32 CPU Clock:\t0x%06X\n",  DEBUG_hostcpuclk_status());
    TEST_print(services_handle, "A32 GIC Clock:\t0x%04X\n",  DEBUG_gicclk_status());
    TEST_print(services_handle, "CTRL    Clock:\t0x%04X\n",  DEBUG_ctrlclk_status());
    TEST_print(services_handle, "DBG     Clock:\t0x%04X\n",  DEBUG_dbgclk_status());
    TEST_print(services_handle, "HSUART  Clock:\t0x%04X\n",  DEBUG_hsuartclk_status());
    TEST_print(services_handle, "AXI Bus Clock:\t0x%04X\n",  DEBUG_aclk_status());
    TEST_print(services_handle, "AHB/APB Clock:\t0x%03X\n",  DEBUG_hclk_pclk_status());
    TEST_print(services_handle, "HFRC Divider:\t0x%04X\n",   DEBUG_hfrc_divider());
    TEST_print(services_handle, "HFXO Divider:\t%d\n\n",     DEBUG_hf_xtal_divider());
}

void DEBUG_frequencies(uint32_t services_handle) {
    uint32_t hfrc_top_clock = 76800000;
    uint32_t hfxo_top_clock = 38400000;
    uint32_t osc_mix_clk;   // result of sys_xtal_sel[0] in hf_osc_sel register
    uint32_t hfosc_clk;     // result of periph_xtal_sel[4] in hf_osc_sel register
    uint32_t cpupll_clk, syspll_clk, a32_cpuclk;
    uint32_t syst_refclk, syst_aclk, syst_hclk, syst_pclk;
    uint32_t rtss_hp_clk, rtss_he_clk, pd4_clk;

    uint32_t dcdc_pfm_enable;
    uint32_t hfrc_div_active;
    uint32_t hfrc_div_standby;
    uint32_t hfxo_div;
    uint32_t bus_clk_div;
    uint32_t hf_osc_sel;
    uint32_t pll_clk_sel;
    uint32_t es_clk_sel;
    uint32_t clk_ena;

    uint32_t reg_data;

    /* fetch dcdc pfm enable bit, for B2 only */
    reg_data = *((volatile uint32_t *)0x1A60A034);  // DCDC_REG2
    dcdc_pfm_enable = (reg_data >> 7) & 1U;

    reg_data = *((volatile uint32_t *)0x1A60A03C);  // VBAT_ANA_REG2
    hfrc_div_active = (reg_data >> 11) & 7U;
    hfrc_div_standby = (reg_data >> 19) & 7U;

    if (hfrc_div_standby > 6) hfrc_div_standby += 3;        // 2^(7   + 3) = 1024 (75k)
    else if (hfrc_div_standby > 3) hfrc_div_standby += 2;   // 2^(4-6 + 2) = 64-256 (1.2M-300k)
    else if (hfrc_div_standby > 2) hfrc_div_standby += 1;   // 2^(3   + 1) = 16 (4.8M)
                                                            // 2^(0-2 + 0) = 1-4 (76.8M-19.2M)

    hfxo_div = DEBUG_hf_xtal_divider();             // MISC_REG1
    if (hfxo_div > 7) {
        hfxo_div -= 8;
        if (hfxo_div > 6) hfxo_div += 3;            // 2^(7   + 3) = 1024 (75k)
        else if (hfxo_div > 3) hfxo_div += 2;       // 2^(4-6 + 2) = 64-256 (1.2M-300k)
        else if (hfxo_div > 2) hfxo_div += 1;       // 2^(3   + 1) = 16 (4.8M)
                                                    // 2^(0-2 + 0) = 1-4 (76.8M-19.2M)
    }
    hfxo_top_clock >>= hfxo_div;

    TEST_print(services_handle, "Top Level Clock Sources\n");
    TEST_print(services_handle, "HFRC ACTIVE  %8d Hz%s\n", hfrc_top_clock >> hfrc_div_active, dcdc_pfm_enable ? "[ ]" : "[x]");
    TEST_print(services_handle, "HFRC STDBY   %8d Hz%s\n", hfrc_top_clock >> hfrc_div_standby, dcdc_pfm_enable ? "[x]" : "[ ]");
    TEST_print(services_handle, "HFXO CLK     %8d Hz\n", hfxo_top_clock);
    TEST_print(services_handle, "[x] means clock is selected\n\n");

    /* calculate hfrc_top_clock based on dcdc pfm enable bit, for B2 only */
    if (dcdc_pfm_enable) {
        hfrc_top_clock >>= hfrc_div_standby;
    } else {
        hfrc_top_clock >>= hfrc_div_active;
    }

    hf_osc_sel = DEBUG_hf_osc_source();
    pll_clk_sel = DEBUG_pll_clk_select();
    es_clk_sel = DEBUG_extsys_clk_select();
    clk_ena = DEBUG_clk_enabled();
    bus_clk_div = DEBUG_hclk_pclk_status();

    /* calculate the osc_mix_clk */
    if (hf_osc_sel & 1) {
        osc_mix_clk = hfxo_top_clock;
    }
    else {
        osc_mix_clk = hfrc_top_clock;
    }

    /* calculate the hfosc_clk */
    if (hf_osc_sel & 16) {
        hfosc_clk = hfxo_top_clock;
    }
    else {
        hfosc_clk = hfrc_top_clock;
    }
    TEST_print(services_handle, "HFXO_OUT     %8d Hz%s\n", osc_mix_clk, (clk_ena & (1U << 18)) == 0 ? "*" : "");
    TEST_print(services_handle, "HFOSC_CLK    %8d Hz%s\n", hfosc_clk, (clk_ena & (1U << 23)) == 0 ? "*" : "");

    /* calculate the CPUPLL_CLK and SYSPLL_CLK */
    if (pll_clk_sel & 16) {
        cpupll_clk = 800000000;
        syspll_clk = 400000000;
    }
    else {
        cpupll_clk = osc_mix_clk;
        syspll_clk = osc_mix_clk;
    }
    TEST_print(services_handle, "CPUPLL_CLK  %9d Hz%s\n", cpupll_clk, (clk_ena & (1U << 4)) == 0 ? "*" : "");
    TEST_print(services_handle, "SYSPLL_CLK  %9d Hz%s\n", syspll_clk, (clk_ena & (1U << 0)) == 0 ? "*" : "");

    /* calculate the SYST_REFCLK */
    if (pll_clk_sel & 1) {
        syst_refclk = 100000000;
    }
    else {
        syst_refclk = osc_mix_clk;
    }
    TEST_print(services_handle, "SYST_REFCLK %9d Hz\n", syst_refclk);

    /* calculate the APSS_CLK */
    /* HOSTCPUCLK_CTRL value at [7:0] and
     * HOSTCPUCLK_DIV0 value at [15:8] and
     * HOSTCPUCLK_DIV1 value at [23:16] */
    uint32_t apssclk_status = DEBUG_hostcpuclk_status();
    if ((apssclk_status & 0xF) == 4) {
        a32_cpuclk = cpupll_clk;
        a32_cpuclk /= ((apssclk_status >> 16) & 0xF) + 1;     // apssclk divider is (n + 1)
    }
    else if ((apssclk_status & 0xF) == 2) {
        a32_cpuclk = syspll_clk;
        a32_cpuclk /= ((apssclk_status >> 8) & 0xF) + 1;      // apssclk divider is (n + 1)
    }
    else if ((apssclk_status & 0xF) == 1) {
        a32_cpuclk = syst_refclk;
    }
    else {
        a32_cpuclk = 0;
    }
    TEST_print(services_handle, "A32_CPUCLK  %9d Hz%s\n", a32_cpuclk, (apssclk_status & 0xF) == 0 ? "*" : "");

    /* calculate the SYST_ACLK, HCLK, PCLK */
    /* ACLK_CTRL value at [7:0] and
     * ACLK_DIV0 value at [15:8] */
    uint32_t aclk_status = DEBUG_aclk_status();
    if ((aclk_status & 0xF) == 2) {
        syst_aclk = syspll_clk;
        syst_aclk /= ((aclk_status >> 8) & 0xF) + 1;        // aclk divider is (n + 1)
    }
    else if ((aclk_status & 0xF) == 1) {
        syst_aclk = syst_refclk;
    }
    else {
        syst_aclk = 0;
    }

    syst_hclk = syst_aclk >> ((bus_clk_div >> 8) & 0xF);    // hclk & pclk divider is (2^n)
    syst_pclk = syst_aclk >> (bus_clk_div & 0xF);

    TEST_print(services_handle, "SYST_ACLK   %9d Hz%s\n", syst_aclk, (aclk_status & 0xF) == 0 ? "*" : "");
    TEST_print(services_handle, "SYST_HCLK   %9d Hz\n", syst_hclk);
    TEST_print(services_handle, "SYST_PCLK   %9d Hz\n", syst_pclk);
    TEST_print(services_handle, "SRAM0_CLK   %9d Hz%s\n", syst_aclk, (clk_ena & (1U << 24)) == 0 ? "*" : "");
    TEST_print(services_handle, "SRAM1_CLK   %9d Hz%s\n", syst_aclk, (clk_ena & (1U << 28)) == 0 ? "*" : "");

    const uint32_t pll_rtss_hp_clocks[4] = {100000000UL, 200000000UL, 300000000UL, 400000000UL};
    const uint32_t pll_rtss_he_clocks[4] = {60000000UL, 80000000UL, 120000000UL, 160000000UL};

    /* calculate the RTSS_HP_CLK */
    if (pll_clk_sel & (1U << 16)) {
        rtss_hp_clk = pll_rtss_hp_clocks[es_clk_sel & 3U];
    }
    else {
        switch((es_clk_sel >> 8) & 3U) {
        case 0:
            rtss_hp_clk = hfrc_top_clock;
            break;
        case 1:
            rtss_hp_clk = hfrc_top_clock >> 1;
            break;
        case 2:
            rtss_hp_clk = 76800000;
            break;
        case 3:
            rtss_hp_clk = hfxo_top_clock;
            break;
        }
    }
    TEST_print(services_handle, "RTSS_HP_CLK %9d Hz%s\n", rtss_hp_clk, (clk_ena & (1U << 12)) == 0 ? "*" : "");

    /* calculate the RTSS_HE_CLK */
    if (pll_clk_sel & (1U << 20)) {
        rtss_he_clk = pll_rtss_he_clocks[(es_clk_sel >> 4) & 3U];
    }
    else {
        switch((es_clk_sel >> 12) & 3U) {
        case 0:
            rtss_he_clk = hfrc_top_clock;
            break;
        case 1:
            rtss_he_clk = hfrc_top_clock >> 1;
            break;
        case 2:
            rtss_he_clk = 76800000;
            break;
        case 3:
            rtss_he_clk = hfxo_top_clock;
            break;
        }
    }
    TEST_print(services_handle, "RTSS_HE_CLK %9d Hz%s\n", rtss_he_clk, (clk_ena & (1U << 13)) == 0 ? "*" : "");

    uint32_t pll_pd4_clocks[4] = {hfxo_top_clock, 0, 120000000UL, 160000000UL};

    /* calculate PD4 SRAM6 to SRAM9 clocks */
    if (*((volatile uint32_t *)0x1A60504C) & 1U) {
        pd4_clk = pll_pd4_clocks[*((volatile uint32_t *)0x1A605040) & 3U];
    }
    else {
        pd4_clk = pll_pd4_clocks[0];
    }
    /* check if PD4 is alive */
    reg_data = *((volatile uint32_t *)0x1A605058);
    reg_data &= 0xFFF;
    TEST_print(services_handle, "PD4_CLK     %9d Hz%s\n", pd4_clk, reg_data > 1 ? "" : "*");

    TEST_print(services_handle, "160M_CLK    %9d Hz%s\n", 160000000, (clk_ena & (1U << 20)) == 0 ? "*" : "");
    TEST_print(services_handle, "100M_CLK    %9d Hz%s\n", 100000000, (clk_ena & (1U << 21)) == 0 ? "*" : "");
    TEST_print(services_handle, "20M/10M_CLK %9d Hz%s\n", 20000000, (clk_ena & (1U << 22)) == 0 ? "*" : "");
    TEST_print(services_handle, "* means clock is gated\n\n");
}
