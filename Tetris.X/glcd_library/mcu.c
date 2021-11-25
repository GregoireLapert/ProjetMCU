/*
 * File:   mcu.c
 * Author: grego
 *
 * Created on 21 novembre 2021, 16:03
 */


#include <xc.h>
#include "mcu.h"
#define _XTAL_FREQ 8000000 // 4Mhz

char EEPROM_Read (int address)
{
EEADR=address; // Read data at location 0x00
EECON1bits.WREN=0; // Read operation
EECON1bits.EEPGD=0; // Access data in EEPROM
EECON1bits.RD=1; // To Read data from EEPROM
return(EEDATA); // return the value
}

void EEPROM_Write (int address, char data)
{
EEADR=address; // Write address to the EEADR reg.
EEDATA=data; // Copy data to the EEDATA reg.
EECON1bits.EEPGD=0; // Access data EEPROM mem.
EECON1bits.CFGS=0; // Access program or data mem.
EECON1bits.WREN=1; // Allow write to the memory
INTCONbits.GIE=0; // Disable global interrupt
EECON2=0x55; // mandatory to avoid glitches
EECON2=0xaa;
EECON1bits.WR=1; // Start writing data to EEPROM memory
INTCONbits.GIE=1; // Enable interrupt
while(PIR2bits.EEIF==0); // Wait for write operation complete
PIR2bits.EEIF=0; // Reset EEIF for further write operation
}

void setupPWM(int PR2value)
{
    TRISCbits.RC2 = 0;// set RC2 as an output
    
    //set CCP1 as PWM output
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    
    
    //enable global and peripheral interrupt
    INTCONbits.GIE = 1; 
    INTCONbits.PEIE = 1; 
    
    //set timer 2
    TMR2 = 0;
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
    //set prescaler of TMR2 at 4
    T2CONbits.T2CKPS1 = 0;
    T2CONbits.T2CKPS0 = 1;
    //set PR2 register
    PR2 = PR2value; // deduce due to the frequency of the buzzer
    
    //set duty cycle
    CCPR1L = 0b00000000;
    CCP1CONbits.DC1B1 = 0;
    CCP1CONbits.DC1B0 = 1;

    pwmStart();
    //pwmStop();
    
}

void pwmStart()
{
    T2CONbits.TMR2ON = 1;

}

void pwmStop()
{
    T2CONbits.TMR2ON = 0;
}