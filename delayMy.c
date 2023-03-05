/* File:  delayMy.c                                     * Date: 02/19/2023
 * ******************************************************************************
 * Description: Functions to control the wait times required to wait for hardware executions without 
 * stopping the MCU PIC peripherals.
 * 
 * Environment: MPLAB v6.05, XC8 v2.40, PIC18F4550, MCP2515 e TJA1050.
 *                     Placa de desenvolvimento FATEC (FATEC board, http://fatecsantoandre.edu.br/).
 * 
 * Author: Antonio Aparecido Ariza Castilho;
 * 
 * MIT License  (see at: LICENSE em github)
 * Copyright (c) 2021 Antonio Castilho <https://github.com/AntonioCastilho>
 *******************************************************************************/

#include <xc.h>
#include "config_bits.h"
#include "delayMy.h"

/***********************************************************************************************************************************************
 * FUNCTION: void delayMS(uint8_t time)
 * Description: Wait a time (time) in milliseconds.
 **********************************************************************************************************************************************/
void delayMS(uint16_t time)
{
    for(uint16_t i=0; i < time; i++)
    {
        for (uint16_t ms=0; ms < 182; ms++); // 48 MHz = 1086 ; 8 MHz = 182 ; 20MHz = ?
    }
    
} // end void delayMS(uint8_t time) function


/***********************************************************************************************************************************************
 * FUNCTION: void delayUS(uint8_t time)
*  Description: Wait a time (time) in microseconds.
 **********************************************************************************************************************************************/
void delayUS(uint16_t time)
{   
    //TODO: Make improvements to the microsecond operation, it seems that the minimum time 
    // that can be achieved is 3 microseconds, so the time must be estimated accordingly.
    
    for(uint16_t i=0; i < time; i++); // Minimum time, usually around 3 microseconds.
    
}

