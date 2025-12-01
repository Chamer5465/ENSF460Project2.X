/* 
 * File:   main.c
 * Author: Chase Mackenzie, Aaron Montesines, Patricia Agdamag
 *
 * Created on November 1, 2025, 1:05 PM
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
#include  "timeDelay.h"

extern uint8_t CNflag;
extern uint8_t LONGflag;
extern uint8_t buttonState;
extern uint8_t inputTaken;

/**
 * You might find it useful to add your own #defines to improve readability here
 */

extern uint16_t pwm_duty;   // 0-100, updated from main
extern uint8_t pwm_led;     // 0 = LED1, 1 = LED2
extern uint8_t pwm_on;      // current state
extern uint16_t pwm_counter; 
extern uint16_t blink_counter; // for blinking

    
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
        brightness = ((uint32_t)sample * 100) / MAX_ADC_VALUE;

        if (CNflag) {// Only check IO if a change happens
            IOCheck();
        }
        
        if(inputTaken) {
            switch (buttonState) {
                case PB1: //PB1 toggles the mode.
                    if (LONGflag) {
                        led ^= 1;
                        LONGflag = 0;
                        LATBbits.LATB9 = 0;
                        LATAbits.LATA6 = 0;
                        break;
                    } else {
                        mode ^= 1;
                        blinking = 0;
                        started = 0;
                        break;
                    }
                case PB2:
                    blinking ^= 1;
                    break;
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
            inputTaken = 0;
            buttonState = 0;
        }
                
        uint16_t effective_duty = brightness;
        
        if (mode) {
            if (started) {
                //UART transmission
                Disp2String("ADC:");
                Disp2Dec(sample);

                Disp2String("INT:");
                Disp2Dec(effective_duty);
                //delayMS(100);  
            }
            if (blinking) {    
                // 0.5s is 2500 ticks (Timer1 ticks at 0.2ms)
                if (blink_counter < 2500) {
                    effective_duty = brightness; // LED on current brightness
                }
                else {
                    effective_duty = 0; // LED off
                }
            }           
                
            pwm_duty = effective_duty; // Set duty cycle for PWM
            pwm_led = led;             // Select which LED to control

        } else if (blinking) {
            //Blink when in off mode at full brightness
            if (blink_counter < 2500) {
                pwm_duty = 100; // LED fully on at 100 brightness
            } 
            else {
                pwm_duty = 0; // LED OFF
            }
            pwm_led = led; // Select which LED to control
            
        } else {
            LATBbits.LATB9 = 0;
            LATAbits.LATA6 = 0;
            pwm_duty = 0;
            Idle(); //Idle if in off mode and not blinking
        }
    }
    
    return 0;
} 

