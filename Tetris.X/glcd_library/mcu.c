/*
 * File:   mcu.c
 * Author: grego
 *
 * Created on 21 novembre 2021, 16:03
 */


#include <xc.h>
#include "mcu.h"

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

