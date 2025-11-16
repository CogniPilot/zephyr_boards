/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <fsl_clock.h>
#include <fsl_xbara.h>
#include <soc.h>

#include <stdint.h>

void board_early_init_hook(void)
{
	/* Must enable LPUART clock according to errata 50235 if we want to use CAN_CLK_SELECT 2. */
	CLOCK_EnableClock(kCLOCK_Lpuart1);
    CLOCK_EnableClock(kCLOCK_Timer3);
    CLOCK_EnableClock(kCLOCK_Xbar1);
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
