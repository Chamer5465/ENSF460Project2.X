/* 
 * File:   ADC.h
 * Author: Chase Mackenzie, Aaron Montesines, Patricia Agdamag
 *
 * Created on November 1, 2025, 1:05 PM
 */

#ifndef ADC_H
#define	ADC_H

#include <xc.h>

#define MAX_ADC_VALUE 0x3FF

uint16_t do_ADC();

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void);

#endif	/* ADC_H */

