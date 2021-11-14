/*
 * File:   main.c
 * Author: marie
 *
 * Created on 13 avril 2021, 10:03
 */

#include "TS.h"
#include <xc.h>
//ISR declaration
void interrupt ISR()
{
    if(PIR1bits.ADIF)
    {
        PIR1bits.ADIF = 0;
        
    }
}


//function
void setupADCR0()
{
    TRISA = 0b00000011; // set pin RA0 and RA1 as input
    
    //set ADC
    
    ADCON0 = 0b00000001; //enable the AD conversion and select R0 as the input pin
    ADCON1 = 0b00111110; // set voltage reference and RA0 as analog pin
    ADCON2bits.ADFM = 1; // set read bit as : MSB is on the right : MSB0 0000000LSB
    
    PIR1bits.ADIF = 0; // clear ADC interrupt flag
    PIE1bits.ADIE = 1; // ADC interrupt enable
     
    
    INTCONbits.GIE = 1;//enable global interrupt
    INTCONbits.PEIE = 1;//enable peripheral interrupt
}
void setupADCR1()
{
    TRISA = 0b00000011; // set pin RA0 and RA1 as input
    
    //set ADC
    
    ADCON0 = 0b00000101; //enable the AD conversion and select RA1 as the input pin
    ADCON1 = 0b00111101; // set voltage reference and RA1 as analog pin
    ADCON2bits.ADFM = 1; // set read bit as : MSB is on the right : MSB0 0000000LSB
    
    PIR1bits.ADIF = 0; // clear ADC interrupt flag
    PIE1bits.ADIE = 1; // ADC interrupt enable
     
    
    INTCONbits.GIE = 1;//enable global interrupt
    INTCONbits.PEIE = 1;//enable peripheral interrupt
}

int R1Value()
{
    setupADCR1();
    ADCON0bits.GO=1; //start the AD conversion 
    while(ADCON0bits.GO==1); //wait for the conversion
    return ADRES; 
}
int R0Value()
{
    setupADCR0();
    ADCON0bits.GO=1; //start the AD conversion 
    while(ADCON0bits.GO==1); //wait for the conversion
    return ADRES;
}

int readX()
{
    int value = R0Value();
    return (128*value)/1024;
}

int readY()
{
    int value = R1Value();
    return (64*value)/1024;
}