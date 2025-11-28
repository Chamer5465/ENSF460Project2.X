/* 
 * File:   IOs.h
 * Author: Chase Mackenzie, Aaron Montesines, Patricia Agdamag
 *
 * Created on November 1, 2025, 1:05 PM
 */

#ifndef IOS_H
#define	IOS_H

#include "timeDelay.h"

#include <xc.h>

//I don't wanna write PORTXbits a thousand times, hooray for macros
#define PRT1 PORTBbits.RB7
#define PRT2 PORTBbits.RB4
#define PRT3 PORTAbits.RA4

//I like the idea of having the bits of the buttonState variable contain the button state, so I defined them so I don't have to remember to use 1 2 and 4
#define PB1 1
#define PB2 2
#define PB3 4

#define LED1 LATBbits.LATB9
#define LED2 LATAbits.LATA6

//Initialize our IO
void IOInit();

//Take the input from the buttons, state is stored in the buttonState variable
void IOCheck();

#endif	/* IOS_H */

