/* 
 * File:   timeDelay.c
 * Author: Chase Mackenzie, Aaron Montesines, Patricia Agdamag
 *
 * Created on November 1, 2025, 1:05 PM
 */

#include "timeDelay.h"
#include "IOs.h"

uint8_t TMR2flag;

volatile uint16_t pwm_duty = 0;   // 0-100, updated from main
volatile uint8_t pwm_led = 0;     // 0 = LED1, 1 = LED2
volatile uint8_t pwm_on = 0;      // current state
volatile uint16_t pwm_counter = 0; 
volatile uint16_t blink_counter = 0; // for blinking


void timerInit() {
    newClk(8);
    
    T2CONbits.T32 = 0;
    T2CONbits.TCKPS = 3;
    T2CONbits.TCS = 0;
    T2CONbits.TSIDL = 0;
    IPC1bits.T2IP = 2;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    
    T3CONbits.TCKPS = 3; // set prescaler to 3:256
    T3CONbits.TCS = 0; // use internal clock
    T3CONbits.TSIDL = 0; //operate in idle mode
    IPC2bits.T3IP = 4; //7 is highest and 1 is lowest pri.
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1; //enable timer interrupt
    
    PR3 = 46875; // PR3 = 3 / (4000000^-1 * 256)
    
    
    // Timer1 for PWM (2 kHz)
    T1CONbits.TCKPS = 1; // prescaler 1:8
    T1CONbits.TCS = 0;   // internal clock
    TMR1 = 0;
    PR1 = 99;          // PWM period = 0.2ms, PWM freq = 5Khz 
    IPC0bits.T1IP = 4;   // interrupt priority
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    T1CONbits.TON = 1;   // start Timer1
    

}

void delayMS(uint16_t delay) {
    PR2 = (uint16_t)(((uint32_t)delay * (uint32_t)125 + 4) / 8); //delay * 125 / 8, rounded to the nearest int
    TMR2flag = 0;
    TMR2 = 0;
    T2CONbits.TON = 1;
    //Make sure other interrupts don't cause the delay to end early.
    while(!TMR2flag) {
        Idle();
    }
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    //Almost forgot to clear the interrupt flag!
    IFS0bits.T2IF = 0;
    T2CONbits.TON = 0;
    TMR2flag = 1;
}



void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0; // Clear interrupt flag

    // PWM counter increments 0 to 99 for each timer tick (0.2 ms per tick)
    // Tracks how long we are in a PWM cycle.
    pwm_counter++;
    if (pwm_counter >= 100) {
        pwm_counter = 0; // reset at end of PWM cycle
    }

    // PWM output
    // if pwm_counter < duty , stay on, otherwise turn off.
    if (pwm_counter < pwm_duty) {
        pwm_on = 1; // LED ON
    } else  {
        pwm_on = 0; // LED OFF
    }

    // Turns off and on the corresponding LED.
    if (pwm_led == 0) {
        LATBbits.LATB9 = pwm_on; // LED1
    } else {
        LATAbits.LATA6 = pwm_on; // LED2
    }

    // Blink counter increments for every Timer1 Interrupt (0.2ms per tick)
    // Full blink cycle is 1s = ON for 0.5s + OFF for 0.5s
    // So full cycle is 5000 ticks
    blink_counter++;
    if (blink_counter >= 5000){
        blink_counter = 0; // reset counter at end of full blink cycle
    } 
}
