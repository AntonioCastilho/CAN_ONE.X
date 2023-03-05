/* File:  hardware.c                                     * Date: 02/19/2023
 * **************************************************************************************
 * Description: Hardware definitions
 * 
 * Environment: MPLAB v6.05, XC8 v2.40, PIC18F4550, MCP2515 e TJA1050.
 *                     Placa de desenvolvimento FATEC (FATEC board, http://fatecsantoandre.edu.br/).
 * 
 * Author: Antonio Aparecido Ariza Castilho;
 * 
 * MIT License  (see at: LICENSE em github)
 * Copyright (c) 2021 Antonio Castilho <https://github.com/AntonioCastilho>
 ****************************************************************************************/

/****************************************************************************************
 * Includes
 ****************************************************************************************/
#include <xc.h>
#include "config_bits.h"
#include "hardware.h"

/****************************************************************************************
 * Function void hardware_ini();
 * Initialize hardware peripherals and pins.
 ****************************************************************************************/
void hardware_ini()
{
    OSCCON = 0x72; // Iinternal frequency (Fosc) 8 MHZ 0b01110100
    
    // I/O definitions
    TRISB = 0X00;
    LATB = 0XFF;

    // I/O pins definition for SPI 
    TRISBbits.TRISB1 = 0;  // Pin 34-Serial Clock (SCK) - RB1/AN10/INT1/SCK/SCL
    TRISCbits.TRISC7 = 0;  // Pin 26-Serial Data Out (SDO) - RC7/RX/DT/SDO
    TRISAbits.TRISA5 = 0;  // Pin 07-Slave Select (SS) - RA5/AN4/SS/HLVDIN/C2OUT
    TRISBbits.TRISB0 = 1;  // Pin 33-Serial Data In (SDI) - RB0/AN12/INT0/FLT0/SDI/SDA
    TRISBbits.TRISB2 = 1;   // MCP_INT: INT2
    
    // Interrupt
   /* INTCON   = 0b11000000;
    INTCON2 = 0b00000100; // PORTB pull-ups are enabled by individual port latch values. TMR0 High priority. Pg. 102
    INTCON3 = 0b00001000;
    PIR1        = 0b00000000; // SPI transmit flag.
    PIR2        = 0b00000000;
    PIE1        = 0b00001000; // SPI interupt enable.
    PIE2        = 0b00000000;
    IPR1        = 0b00000000;
    IPR2        = 0b00000000;*/
    
    // PIE1bits.SPPIE = 1;         // SPI interupt enable.
    // PIR1bits.SPPIF = 0;         // SPI transmit flag.
    
    SPI_ini();
    delayMS(100);
    mcp2515Start();
    delayMS(100);
    
} // end function hardware_ini().


