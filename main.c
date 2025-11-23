/*
 * File:   main.c
 * Author: UPDATE THIS WITH YOUR GROUP MEMBER NAMES OR POTENTIALLY LOSE POINTS
 *
 * Created on: USE THE INFORMATION FROM THE HEADER MPLAB X IDE GENERATES FOR YOU
 */

// FBS
#pragma config BWRP = OFF               // Table Write Protect Boot (Boot segment may be written)
#pragma config BSS = OFF                // Boot segment Protect (No boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Segment Code Flash Write Protection bit (General segment may be written)
#pragma config GCP = OFF                // General Segment Code Flash Code Protection bit (No protection)

// FOSCSEL
#pragma config FNOSC = FRC              // Oscillator Select (Fast RC oscillator (FRC))
#pragma config IESO = OFF               // Internal External Switch Over bit (Internal External Switchover mode disabled (Two-Speed Start-up disabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = ON            // CLKO Enable Configuration bit (CLKO output disabled; pin functions as port I/O)
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
#pragma config FCKSM = CSECMD           // Clock Switching and Monitor Selection (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscale Select bits (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (WDT prescaler ratio of 1:128)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Standard WDT selected; windowed WDT disabled)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
#pragma config PWRTEN = ON              // Power-up Timer Enable bit (PWRT enabled)
#pragma config I2C1SEL = PRI            // Alternate I2C1 Pin Mapping bit (Default location for SCL1/SDA1 pins)
#pragma config BORV = V18               // Brown-out Reset Voltage bits (Brown-out Reset set to lowest voltage (1.8V))
#pragma config MCLRE = ON               // MCLR Pin Enable bit (MCLR pin enabled; RA5 input pin disabled)

// FICD
#pragma config ICS = PGx2               // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

// FDS
#pragma config DSWDTPS = DSWDTPSF       // Deep Sleep Watchdog Timer Postscale Select bits (1:2,147,483,648 (25.7 Days))
#pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select bit (DSWDT uses LPRC as reference clock)
#pragma config RTCOSC = SOSC            // RTCC Reference Clock Select bit (RTCC uses SOSC as reference clock)
#pragma config DSBOREN = ON             // Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR enabled in Deep Sleep)
#pragma config DSWDTEN = ON             // Deep Sleep Watchdog Timer Enable bit (DSWDT enabled)

// #pragma config statements should precede project file includes.

#include <xc.h>
#include <p24F16KA101.h>
#include "clkChange.h"
#include "UART2.h"
#include "IOs.h"
#include "ADC.h"

extern uint8_t CNflag;
extern uint8_t LONGflag;
extern uint8_t buttonState;

/**
 * You might find it useful to add your own #defines to improve readability here
 */

int main(void) {
    
    /** This is usually where you would add run-once code
     * e.g., peripheral initialization. For the first labs
     * you might be fine just having it here. For more complex
     * projects, you might consider having one or more initialize() functions
     */
    
    /* Let's set up our UART */    
    InitUART2();
    IOInit();
    timerInit();
    
    uint8_t mode = 0;
    uint16_t sample = 0; 
    uint16_t brightness = 0;
    uint8_t blinking = 0;
    uint8_t started = 0;
    
    uint8_t led = 0;
    
    
    while(1) {
        sample = do_ADC();
        brightness = (sample + MAX_ADC_VALUE / 2) * 100 / MAX_ADC_VALUE;
        if (CNflag) {// Only check IO if a change happens
            IOCheck();
        }
        switch (buttonState) {
            case PB1: //PB1 toggles the mode.
                if (LONGflag) {
                    led ^= 1;
                    LONGflag = 0;
                    buttonState = 0;
                    break;
                } else {
                    mode ^= 1;
                    blinking = 0;
                    started = 0;
                    buttonState = 0;
                    break;
                }
            case PB2:
                blinking ^= 1;
                buttonState = 0;
            case PB3: //PB3 starts transmission in mode 1
                if (mode) {
                    if (!started) {
                        started = 1;
                        buttonState = 0;
                    } else {
                        started = 0;
                    }
                }
                break;
        }
        
        if (mode) {
            if (started) {
                //UART transmission
            }
            if (blinking) {
                //Change the intensity based on how many cycles have passed maybe?
            }
            //PWM stuff here
        } else if (blinking) {
            //Blink when in off mode
        } else {
            Idle(); //Idle if in off mode and not blinking
        }
    }
    
    return 0;
}
