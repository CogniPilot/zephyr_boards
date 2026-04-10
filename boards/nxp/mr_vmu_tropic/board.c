/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <fsl_clock.h>
#include <fsl_iomuxc.h>
#include <fsl_gpio.h>
#include <fsl_xbara.h>
#include <soc.h>

#include <stdint.h>

/*
 * Simple early delay using DWT cycle counter:
 * Safe in board_early_init_hook because it doesn't depend on Zephyr timers.
 */
static void early_delay_us(uint32_t us)
{
    /* SystemCoreClock is usually set very early on i.MX RT */
    uint32_t cycles = (SystemCoreClock / 1000000U) * us;

    /* Enable DWT CYCCNT */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    uint32_t start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < cycles) {
        /* busy wait */
    }
}

#define STRAP_PAD_CFG_INPUT_NOPULL  (0x10B0u)
#define STRAP_PAD_CFG_OUTPUT        (0x10B0u)

void board_early_init_hook(void)
{
	/* Must enable LPUART clock according to errata 50235 if we want to use CAN_CLK_SELECT 2. */
	CLOCK_EnableClock(kCLOCK_Lpuart1);
    CLOCK_EnableClock(kCLOCK_Timer3);
    CLOCK_EnableClock(kCLOCK_Xbar1);

    /* GPIO_B1_11 -> GPIO2_IO27  (Open / input) */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_GPIO2_IO27, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_GPIO2_IO27, STRAP_PAD_CFG_INPUT_NOPULL);

    /* GPIO_B1_05 -> GPIO2_IO21  (Drive High) */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_05_GPIO2_IO21, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_05_GPIO2_IO21, STRAP_PAD_CFG_OUTPUT);

    /* GPIO_B1_04 -> GPIO2_IO20  (Open / input) */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_GPIO2_IO20, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_04_GPIO2_IO20, STRAP_PAD_CFG_INPUT_NOPULL);

    /* GPIO_B1_06 -> GPIO2_IO22  (Drive Low) */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_GPIO2_IO22, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_06_GPIO2_IO22, STRAP_PAD_CFG_OUTPUT);

    /* Reset pin: GPIO_B0_14 -> GPIO2_IO14 */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_14_GPIO2_IO14, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_14_GPIO2_IO14, STRAP_PAD_CFG_OUTPUT);

    gpio_pin_config_t cfg_in = {
        .direction = kGPIO_DigitalInput,
        .outputLogic = 0U,
        .interruptMode = kGPIO_NoIntmode
    };

    gpio_pin_config_t cfg_out_hi = {
        .direction = kGPIO_DigitalOutput,
        .outputLogic = 1U,
        .interruptMode = kGPIO_NoIntmode
    };

    gpio_pin_config_t cfg_out_lo = {
        .direction = kGPIO_DigitalOutput,
        .outputLogic = 0U,
        .interruptMode = kGPIO_NoIntmode
    };

    /* Strap pins */
    GPIO_PinInit(GPIO2, 27U, &cfg_in);      /* Open */
    GPIO_PinInit(GPIO2, 21U, &cfg_out_hi);  /* High */
    GPIO_PinInit(GPIO2, 20U, &cfg_in);      /* Open */
    GPIO_PinInit(GPIO2, 22U, &cfg_out_lo);  /* Low */
    GPIO_PinInit(GPIO2, 14U, &cfg_out_lo);  /* assert reset (low) */

    early_delay_us(1000U);   /* 1ms assert */

    /* Release reset */
    GPIO_WritePinOutput(GPIO2, 14U, 1U);

    early_delay_us(1000U);   /* 1ms assert */
}

#if 0 //QTMR based external clock for FlexPWM
void board_late_init_hook(void)
{	
    /* Disable QTMR3 CH0 */
    TMR3->CHANNEL[0].CTRL = 0;

    /* COMP1 = 5-1, COMP2 = 4-1 for divide-by-9 toggle scheme */
    TMR3->CHANNEL[0].COMP1 = 5 - 1;
    TMR3->CHANNEL[0].COMP2 = 4 - 1;

    TMR3->CHANNEL[0].CAPT = 0;
    TMR3->CHANNEL[0].LOAD = 0;
    TMR3->CHANNEL[0].CNTR = 0;

    /* Enable output */
    TMR3->CHANNEL[0].SCTRL = TMR_SCTRL_OEN(1);

    TMR3->CHANNEL[0].CMPLD1 = 0;
    TMR3->CHANNEL[0].CMPLD2 = 0;
    TMR3->CHANNEL[0].CSCTRL = 0;
    TMR3->CHANNEL[0].FILT = 0;
    TMR3->CHANNEL[0].DMA = 0;

    /* PCS = DIV1, CM = toggle on compare, LENGTH = reload */
    TMR3->CHANNEL[0].CTRL =
		TMR_CTRL_CM(1) |
        TMR_CTRL_PCS(8) |
        TMR_CTRL_OUTMODE(4) |
        TMR_CTRL_LENGTH(1);

    XBARA_Type *xbar = XBARA1;
    XBARA_Init(xbar);

    XBARA_SetSignalsConnection(xbar, kXBARA1_InputQtimer3Tmr0Output,
                    kXBARA1_OutputFlexpwm1ExtClk);
                    
    XBARA_SetSignalsConnection(xbar, kXBARA1_InputQtimer3Tmr0Output,
                    kXBARA1_OutputFlexpwm234ExtClk);
}
#endif
