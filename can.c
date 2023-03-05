/* File:  can.c                                     * Date: 02/19/2023
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

// Includes
#include <xc.h>
#include "can.h"


/*******************************************************************************
 * FUNCTION: void mcp2515Reset()
 *Description:  Resets the MCP2515 module via software. It uses the SPI interface of the MCP2515 and 
 * the PIC18F4550.
 * Resetting the MCP2515 initializes it and puts it in configuration mode until a new mode is configured.
 *******************************************************************************/
void mcp2515Reset(void)
{
    CS = 0;
    SPI_send(CAN_RESET);
    delayUS(20);
    CS = 1 ;
    
} // end void mcp2515Reset(void) function


/***********************************************************************************************************************************************
 * FUNCTION: void mcp2515DataSend()
 * Description: It sends to the MCP2515 via the PIC's SPI a data of 1 byte (8 bits).
To use this function it is necessary to specify a registration address before
 **********************************************************************************************************************************************/
/*void mcp2515DataSend(uint8_t data)
{  
    CS = 0;
    SPI_send(CAN_WRITE);
    SPI_send(data);
    CS = 1;
} // end void mcp2515Send() function
*/

/*******************************************************************************
 * FUNCTION: void mcp2515MessageSend(struct dataFrame *data);
 * Description: Sends a message to MCP2515 buffer 0.
 * The message is a data structure (dataFrame data) defined in can.h.
 *******************************************************************************/
void mcp2515MessageSend(dataFrame *data)
{
    //TODO: Rewrite function to use all available buffers in MCP2515
    
    CS = 0;
    SPI_send(CAN_WRITE);
    SPI_send(TXB0SIDH);
    SPI_send(data->idh);
    SPI_send(TXB0DLC);
    SPI_send(data->dlc);
    for (uint8_t i=0; i < (data->dlc); i++)
    {
        SPI_send(data->data[i]);
    }
    
    SPI_send(CAN_RTS);
    mcp2515BitChange(TXB0CTRL, 0b00001000, 0b00001000);
    
    CS = 1;
    
} // end void mcp2515MessageSend(struct dataFrame *data); function


/*******************************************************************************
 * FUNCTION: void mcp2515WriteRegister(uint8_t address, uint8_t value)
 * Description: It sends data (value) to a specific register (address) of the MCP2515 
 * via SPI communication.
 *******************************************************************************/
void mcp2515WriteRegister(uint8_t address, uint8_t value)
{
    CS = 0;
    SPI_send(CAN_WRITE);
    SPI_send(address);
    SPI_send(value);
    CS = 1;
    
} // end void mcp2515WriteRegister(uint8_t address, uint8_t value) function


/***********************************************************************************************************************************************
 * FUNCTION: uint8_t mcp2515ReadRegister(uint8_t address)
 * Description: It reads and returns data (buffer variable) from a specific register (address) 
 * of the MCP2515.
 **********************************************************************************************************************************************/
uint8_t mcp2515ReadRegister(uint8_t address)
{
    uint8_t buffer;
    CS  = 0;
    SPI_send(CAN_READ);  //0x03   0b0000 0011
     
    SPI_send(address);
    buffer = SPI_receive();
    CS = 1;
    
    return buffer;
}


/***********************************************************************************************************************************************
 * FUNCTION: void mcp2515Start(void)
 * Description: Configures the MCP2515 module.
 * 
 * Based on MCP2515 CAN Controller IC and TJA1050 CAN Transceiver IC Datasheet.
 * FOSC  8 MHz                                                       * Tfosc =  /  MHz = 1.25 us
 * BRP = 1
 * TQ = 2 * (BRP + 1) / FOSC                                 * TQ =  2*(1+1)/8000000 = 0.5 us
 * Bit Rate = 125 Kbps                                            * Bit time = 1 / 125000 = 8 us
 * number of TQs  = 8 us / 0.5 us = 16 TQs
 * Sampling at 70% of bit time                                * (Tsync + TPS1) / (Tsync + TPS1 + TPS2) = 70%
 * 
 * TsyncSeg = 1 TQ    * TpropSeg = 2 TQ
 * TPS1 = 7 TQ           * TPS2 = 6 TQ
 * SJW = 1 TQ
 *  
 * TpropSeg + PS1 >= PS2 (ok)
 * TpropSeg + PS1 > Tdelay (ok)
 * PS2 > SJW (ok)
 * 
 * Determines Baud rate and Sample point position
 *	CNF1  => 0x01 // 0b00 00 0001 SJW = 1 * TQ; BRP = 1.
 *	CNF2 => 0xF1  // 0b1 1 110 001 BTLMODE = 1; SAM = 1. PHSEG1 [PS1] = 6;  PRSEG = 1
 *	CNF3 => 0xC5 // 0b1 1 000 101 SOF = 1; WAKFIL = 1; PHSEG2 [PS2] = 5.
 * 
 **********************************************************************************************************************************************/
void mcp2515Start(void)
{
    //TODO: Redo the settings for 250 or 500 Kbps, according to the SAE J1939 standard.
    
    delayMS(20);
    mcp2515Reset();
    delayUS(20);
    mcp2515BitChange(CANCTRL, (0b11100000), (0b01000000));
    delayUS(20);
    
    while (getMode() != 0x02);
   
    // Bit timing set. CNF1, CNF2, CNF3 registers
    mcp2515WriteRegister(CNF1, 0x01);
    mcp2515WriteRegister(CNF2, 0xF1);
    mcp2515WriteRegister(CNF3, 0xC5); 
    
    // TXRTSCTRL register
    mcp2515WriteRegister(TXB0CTRL, 0x00);
    mcp2515WriteRegister(TXB1CTRL, 0x00);
    mcp2515WriteRegister(TXB2CTRL, 0x00);
    
    // Filter registers control
    mcp2515WriteRegister(RXB0CTRL, 0x60); // Turns mask/filters off; receives any message.
    mcp2515WriteRegister(RXB1CTRL, 0x60); // Turns mask/filters off; receives any message.
    mcp2515WriteRegister(BFPCTRL, 0x00);  
    
    // Clear filters registers
    for (uint8_t i =0; i < 12; i++)
        mcp2515WriteRegister((RXF0SIDH + i), 0x00);
    
    for (uint8_t i = 0; i < 12; i++)
        mcp2515WriteRegister((RXF3SIDH + i), 0x00);
    
    // Mask registers            
    for (uint8_t i = 0; i < 8; i++)
         mcp2515WriteRegister((RXM0SIDH + i), 0x00);
    
    // Set Operation Mode, mask = 1110 0000 = 0xE0
    // Loopback mode 0100 0000 0x40
    // Normal mode 00000100
    mcp2515BitChange(CANCTRL, (0b11111111), (0b00000100));
    while (getMode() != 0x00);
    LATBbits.LATB7 = 0;
    
} // end void mcp2515Start(void); function


/*******************************************************************************
 * FUNCTION: void mcp2515MessageRead(dataFrame *data)
 * Description: Read data from the receiving registers (buffer 0 and buffer 1) of the MCP2515 module 
 * and store it in a variable of type structure (dataFrame data)
 *******************************************************************************/
void mcp2515MessageRead(uint8_t id, dataFrame *data)
{
    CS = 0;
    
    if ((mcp2515ReadRegister(RXB0SIDH)) == id)
    {
        data->idh = id;
        data->dlc = mcp2515ReadRegister(RXB0DLC);
        
        for (uint8_t i = 0; i < (data->dlc); i++)
        {
            data->data[i] = mcp2515ReadRegister(RXB0D0);
        }
    }
    else if ((mcp2515ReadRegister(RXB1SIDH)) == id )
    {
        data->idh = id;
        data->dlc = mcp2515ReadRegister(RXB1DLC);
        
        for (uint8_t i = 0; i < (data->dlc); i++)
        {
            data->data[i] = mcp2515ReadRegister(RXB1D0);
        }        
        
    }
    else 
        data->idh = 0xFF;

    CS = 1;    
    
} // end void mcp2515MessageRead(dataFrame *data) function

/*******************************************************************************
 * FUNCTION: mcp2515BitChange(uint8_t addressReg, uint8_t maskBit, uint8_t valueNew)
 * Description: Change the state of one or more bits of a register (addressReg) as selected in the 
 * mask (maskBit) to a new state (valueNew).
 *******************************************************************************/
void  mcp2515BitChange(uint8_t addressReg, uint8_t maskBit, uint8_t valueNew)
{
    	CS = 0;
	
	SPI_send(CAN_BIT_MODIFY);
	SPI_send(addressReg);
	SPI_send(maskBit);
	SPI_send(valueNew);
	
	CS = 1;
}

/***********************************************************************************************************************************************
 * FUNCTION: void canSend(uint8_t txb, uint8_t id, uint8_t lenght, uint8_t *data)
 * Description: It sends a message (data), with specific identifier (id) and size (length), to the 
 * CAN network, filling in the register (txb) of the MCP2515 module, via SPI communication.
  * The data variable is a predefined array.
 *******************************************************************************/
void canSend(uint8_t txb, uint8_t id, uint8_t lenght, uint8_t *data)
{
    CS = 0;
    SPI_send(CAN_WRITE);
    
    switch (txb)
    {
        case 1:
            SPI_send(TXB1SIDH);
            SPI_send(id);
            SPI_send(TXB1DLC);
            SPI_send(lenght);
            for (uint8_t i = 0; i < (lenght); i++)
            {
                SPI_send(data[i]);
            }
            SPI_send(CAN_RTS);
            mcp2515BitChange(TXB1CTRL, 0b00001000, 0b00001000);            
            break;
        case 2:
            SPI_send(TXB2SIDH);
            SPI_send(id);
            SPI_send(TXB2DLC);
            SPI_send(lenght);
            for (uint8_t i = 0; i < (lenght); i++)
            {
                SPI_send(data[i]);
            }
            SPI_send(CAN_RTS);
            mcp2515BitChange(TXB2CTRL, 0b00001000, 0b00001000);            
            break;
        default:
            SPI_send(TXB0SIDH);
            SPI_send(id);
            SPI_send(TXB0DLC);
            SPI_send(lenght);
            for (uint8_t i = 0; i < (lenght); i++)
            {
                SPI_send(data[i]);
            }
            SPI_send(CAN_RTS);
            mcp2515BitChange(TXB0CTRL, 0b00001000, 0b00001000);
            break;   
    }    
    CS = 1;
    
} // end void canSend(uint8_t id, uint8_t lenght, uint8_t *data) function

 /***********************************************************************************************************************************************
 * FUNCTION: void canRead(uint8_t id, uint8_t lenght, uint8_t *data)
  * Description: Searches the MCP2515 module receipt records by the specific identifier (id) and copies 
  * the message to the specific variable (data).
  * The data variable is a predefined array.
 *******************************************************************************/
void canRead(uint8_t id, uint8_t *data)
 {
    uint8_t lenght = 0;
    if ((mcp2515ReadRegister(RXB0SIDH)) == id)
    {
        lenght = mcp2515ReadRegister(RXB0DLC);
        
        for (uint8_t i = 0; i < (lenght); i++)
        {
            data[i] = mcp2515ReadRegister(RXB0D0);
        }
    }
    else if ((mcp2515ReadRegister(RXB1SIDH)) == id )
    {
        data = id;
        lenght = mcp2515ReadRegister(RXB1DLC);
        
        for (uint8_t i = 0; i < (lenght); i++)
        {
            data[i] = mcp2515ReadRegister(RXB1D0);
        }        
        
    }
    else 
        data = 0xFF;
    
 } // end void canRead(uint8_t id, uint8_t lenght, uint8_t *data) function

