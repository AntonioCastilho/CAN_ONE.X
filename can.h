/* File:  can.h                                    * Date: 02/19/2023
 * ******************************************************************************
 * Description: Functions for configuring and using the MCP2515 CAN communication module.
 * 
 * Environment: MPLAB v6.05, XC8 v2.40, PIC18F4550, MCP2515 e TJA1050.
 *                     Placa de desenvolvimento FATEC (FATEC board, http://fatecsantoandre.edu.br/).
 * 
 * Author: Antonio Aparecido Ariza Castilho;
 * 
 * MIT License  (see at: LICENSE em github)
 * Copyright (c) 2021 Antonio Castilho <https://github.com/AntonioCastilho>
 *******************************************************************************/

#ifndef CAN_H
    #define CAN_H

// Includes
#include <xc.h>
#include "config_bits.h"
#include "hardware.h"
#include "spi.h"
#include "REGS2515.h"
#include "delayMy.h"

// Defines and Macros
#define getMode()        ((mcp2515ReadRegister(CANSTAT))>> 5) // Checks the operation mode of the MCP2515


// PUBLIC VARIABLES

typedef struct 
{
    uint8_t idh;
    uint8_t dlc;
    uint8_t data[8];
}dataFrame;

dataFrame canMessageSend;
dataFrame canMessageReceived;

/***********************************************************************************************************************************************
 * FUNCTION PROTOTYPES 
 **********************************************************************************************************************************************/
void mcp2515Start(void);

void mcp2515DataReset(uint8_t data);

void mcp2515MessageSend(dataFrame *data);

void mcp2515MessageRead(uint8_t id, dataFrame *data);

void mcp2515WriteRegister(uint8_t address, uint8_t value);

uint8_t mcp2515ReadRegister(uint8_t address);

void  mcp2515BitChange(uint8_t addressReg, uint8_t maskBit, uint8_t valueNew);

void canSend(uint8_t txb, uint8_t id, uint8_t lenght, uint8_t *data);
void canRead(uint8_t id, uint8_t *data);

#endif	/* CAN_H */

