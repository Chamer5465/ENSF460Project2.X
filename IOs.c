/* 
 * File:   IOs.c
 * Author: Chase Mackenzie, Aaron Montesines, Patricia Agdamag
 *
 * Created on November 1, 2025, 1:05 PM
 */

#include "IOs.h"

uint8_t buttonState;

uint8_t button1;
uint8_t button2;
uint8_t button3;

uint8_t inputTaken = 0;
uint8_t LONGflag;

uint8_t CNflag;

void IOInit() {
    //Set the LED ports as outputs and set them to 0
    //LED1 => RB9, LED2 => RA6
    TRISBbits.TRISB9 = 0;
    LATBbits.LATB9 = 0;
    TRISAbits.TRISA6 = 0;
    LATAbits.LATA6 = 0;
    
    //Set the PushButton ports as input ports with pull up resistors and with CN interrupts enabled
    //PB1 => RB7, PB2 => RB4, PB3 => RA4
    TRISAbits.TRISA4 = 1;
    CNPU1bits.CN0PUE = 1;
    CNEN1bits.CN0IE = 1;
    TRISBbits.TRISB4 = 1;
    CNPU1bits.CN1PUE = 1;
    CNEN1bits.CN1IE = 1;
    TRISBbits.TRISB7 = 1;
    CNPU2bits.CN23PUE = 1;
    CNEN2bits.CN23IE = 1;
    IPC4bits.CNIP = 3;
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    
   
}

void IOCheck() {
    delayMS(50); //We wanna wait in case any other button is pressed after the initial press
    
    CNflag = 0;
    
    //Read all buttons
    button1 = PRT1;
    button2 = PRT2;
    button3 = PRT3;
    
    if (button1 && button2 && button3 && !LONGflag) {
        inputTaken = 1;
        T3CONbits.TON = 0;
        return;
    }
    
    if (!T3CONbits.TON) {
        TMR3 = 0;
        T3CONbits.TON = 1;
    }
    
    LONGflag = 0;
    
    inputTaken = 0;
    
    if (!button1) {
        buttonState |= PB1;
    }
    if (!button2) {
        buttonState |= PB2;
    }
    if (!button3) {
        buttonState |= PB3;
    }
}    

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    //Always clear the flag!
    IFS1bits.CNIF = 0;
    CNflag = 1;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    IFS0bits.T3IF = 0;
    T3CONbits.TON = 0;
    LONGflag = 1;
    inputTaken = 1;
}
