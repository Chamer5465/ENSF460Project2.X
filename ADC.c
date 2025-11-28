/* 
 * File:   ADC.c
 * Author: Chase Mackenzie, Aaron Montesines, Patricia Agdamag
 *
 * Created on November 1, 2025, 1:05 PM
 */

#include "ADC.h"

uint8_t ADCflag = 0;

uint16_t do_ADC() {
    uint16_t ADCValue; // Used to store a snapshot of the ADCBUF0 value
    
    AD1CON1bits.ADSIDL = 0; 
    AD1CON1bits.FORM = 0; //Unsigned int, easiest to work with
    AD1CON1bits.SSRC = 7; //Start converting as soon as sample is done (Auto-convert)
    AD1CON1bits.ASAM = 1; //Next sample starts after conversion finishes (Doesn't really matter, but may stop extra samples being started for no reason)
    
    AD1CON2bits.VCFG = 0; //Vr+ = AVdd, Vr- = AVss
    AD1CON2bits.OFFCAL = 0; //No offset, raw value
    AD1CON2bits.SMPI = 0; //interrupt each sample
    AD1CON2bits.BUFM = 0; //One 16-word buffer
    AD1CON2bits.ALTS = 0; //Always just use MUXA
    
    AD1CON3bits.ADRC = 0; //Use the system clock
    AD1CON3bits.SAMC = 31; //Auto-sample time = 31 * 2 / Fclk
    
    AD1PCFG = 0xEFFF; //Set AN12 as an analog input
    
    AD1CHSbits.CH0SA = 12; // Choose positive input as AN12
    AD1CHSbits.CH0NA = 0; // Choose negative input as GND
    
    IPC3bits.AD1IP = 5; // More important than others cause it will halt everything till the conversion is done
    IEC0bits.AD1IE = 1; //Interrupts on to know when it is done without polling
    IFS0bits.AD1IF = 0; // Don't you dare forget about the flag
    
    AD1CON1bits.ADON = 1; // Turn it on
    
    AD1CON1bits.SAMP = 1; //Start sampling
    
    while(!ADCflag) { //Just in case any other interrupts happen
        Idle(); //Wait until conversion is done
    }
    ADCValue = ADC1BUF0; //Take a snapshot of the value
    
    AD1CON1bits.SAMP = 0; //Stop sampling
    AD1CON1bits.ADON = 0; //Turn the ADC off
    
    ADCflag = 0;
    
    return ADC1BUF0;
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {
    IFS0bits.AD1IF = 0; // You almost forgot about the flag didn't you
    ADCflag = 1;
}#include "ADC.h"

uint8_t ADCflag = 0;

uint16_t do_ADC() {
    uint16_t ADCValue; // Used to store a snapshot of the ADCBUF0 value
    
    AD1CON1bits.ADSIDL = 0; 
    AD1CON1bits.FORM = 0; //Unsigned int, easiest to work with
    AD1CON1bits.SSRC = 7; //Start converting as soon as sample is done (Auto-convert)
    AD1CON1bits.ASAM = 1; //Next sample starts after conversion finishes (Doesn't really matter, but may stop extra samples being started for no reason)
    
    AD1CON2bits.VCFG = 0; //Vr+ = AVdd, Vr- = AVss
    AD1CON2bits.OFFCAL = 0; //No offset, raw value
    AD1CON2bits.SMPI = 0; //interrupt each sample
    AD1CON2bits.BUFM = 0; //One 16-word buffer
    AD1CON2bits.ALTS = 0; //Always just use MUXA
    
    AD1CON3bits.ADRC = 0; //Use the system clock
    AD1CON3bits.SAMC = 31; //Auto-sample time = 31 * 2 / Fclk
    
    AD1PCFG = 0xEFFF; //Set AN12 as an analog input
    
    AD1CHSbits.CH0SA = 12; // Choose positive input as AN12
    AD1CHSbits.CH0NA = 0; // Choose negative input as GND
    
    IPC3bits.AD1IP = 5; // More important than others cause it will halt everything till the conversion is done
    IEC0bits.AD1IE = 1; //Interrupts on to know when it is done without polling
    IFS0bits.AD1IF = 0; // Don't you dare forget about the flag
    
    AD1CON1bits.ADON = 1; // Turn it on
    
    AD1CON1bits.SAMP = 1; //Start sampling
    
    while(!ADCflag) { //Just in case any other interrupts happen
        Idle(); //Wait until conversion is done
    }
    ADCValue = ADC1BUF0; //Take a snapshot of the value
    
    AD1CON1bits.SAMP = 0; //Stop sampling
    AD1CON1bits.ADON = 0; //Turn the ADC off
    
    ADCflag = 0;
    
    return ADC1BUF0;
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {
    IFS0bits.AD1IF = 0; // You almost forgot about the flag didn't you
    ADCflag = 1;
}
