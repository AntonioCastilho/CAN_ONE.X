/*
 * File:   delay.h
 * Author: Castilho
 *
 * Created on February 21, 2023, 7:49 AM
 */

#ifndef DELAY_H
#define	DELAY_H

#include <xc.h> 
#include "config_bits.h"

#ifndef _XTAL_FREQ
    //#define _XTAL_FREQ                      48000000
    #define _XTAL_FREQ                           8000000  // OSCCON = 0x72; // Iinternal frequency (Fosc) 8 MHZ 0b01110100

#endif

/***********************************************************************************************************************************************
 * FUNCTION PROTOTYPES 
 **********************************************************************************************************************************************/
void delayMS(uint16_t time);
void delayUS(uint16_t time);



#endif	/* DELAYH */

