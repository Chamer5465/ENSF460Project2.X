#include "timeDelay.h"

uint8_t TMR2flag;

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
