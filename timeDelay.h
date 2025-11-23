/* 
 * File:   timeDelay.c
 * Author: Chase
 *
 * Created on November 1, 2025, 1:08 PM
 */

#ifndef TIMEDELAY_C
#define	TIMEDELAY_C

#include <xc.h>
#include "clkChange.h"

void timerInit();

void delayMS(uint16_t delay);

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);

#endif	/* TIMEDELAY_C */

