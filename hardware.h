/* File:  hardware.c                                     * Date: 02/19/2023
 * ******************************************************************************
 * Description: Hardware definitions.
 * 
 * Environment: MPLAB v6.05, XC8 v2.40, PIC18F4550, MCP2515 e TJA1050.
 *                     Placa de desenvolvimento FATEC (FATEC board, http://fatecsantoandre.edu.br/).
 * 
 * Author: Antonio Aparecido Ariza Castilho;
 * 
 * MIT License  (see at: LICENSE em github)
 * Copyright (c) 2021 Antonio Castilho <https://github.com/AntonioCastilho>
 *******************************************************************************/

#ifndef HARDWARE_H
#define	HARDWARE_H

/****************************************************************************************
 * Includes
 ****************************************************************************************/
#include <xc.h>
#include "config_bits.h"
#include "spi.h"
#include "can.h"
#include "delayMy.h"

#define _XTAL_FREQ     8000000

/****************************************************************************************
 * Definitions hardware mapping
 ****************************************************************************************/
// LEDs Pin
#define LED_7                             PORTBbits.RB7
#define LED_6                             PORTBbits.RB6
#define LED_5                             PORTBbits.RB5
#define LED_4                             PORTBbits.RB5

// SPI Pins
 #define CS                  PORTAbits.RA5  // Slave Select (SS) ? RA5/AN4/SS/HLVDIN/C2OUT
 #define SCK                PORTBbits.RB1  // Serial Clock (SCK) ? RB1/AN10/INT1/SCK/SCL
 #define SDO               PORTCbits.RC7 // Serial Data Out (SDO) ? RC7/RX/DT/SDO
 #define SDI                PORTBbits.RB0  // Serial Data In (SDI) ? RB0/AN12/INT0/FLT0/SDI/SDA
 #define MCP_INT        PORTBbits.RB2

/****************************************************************************************
 * Function prototypes
 ****************************************************************************************/
void hardware_ini();



#endif	/* HARDWARE_H*/

