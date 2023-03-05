/* File:  spi.c                                     * Date: 02/19/2023
 * ******************************************************************************
 * Description: Functions for SPI communication.
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
#include "hardware.h"
#include "config_bits.h"
#include "spi.h"

/*******************************************************************************
 * Function void SPI_ini();
 * Initialize SPI hardware
 *******************************************************************************/
void SPI_ini()
{
    uint8_t dataFlushing;
    
    // Master mode.
    // Sample at midle. Transmit on active-to-idle clock transition
    SSPSTAT = 0x00;  // 0b01000000 MSSP status register (SPI mode) pg. 196
    // Enables serial port and configures SCK SDO SDI SS. SPI clock FOSC/16
    SSPCON1 = 0x32; // 0b00100001 MSSP status register (SPI mode) pg. 197
    
    ADCON0 = 0x01;    // Analog channel selected. Pg. 261
    ADCON1 = 0x0B;   // Analog ports will be limited to AN:AN0 (4 ports). Pg 262
    
    PIR1bits.SSPIF = 0;           // SSPBUF flag. Pg. 104
    
} // end function SPI_ini()
    

/*******************************************************************************
 * Function void SPI_send(uint8_t data);
 * Send data by SPI
 *******************************************************************************/
void SPI_send(uint8_t data)
{
    uint8_t dataFlushing;
    PIR1bits.SSPIF = 0; 
    //CS = 0;
    SSPBUF = data;
    
    while(!PIR1bits.SSPIF); 
    
    PIR1bits.SSPIF = 0; 
    //CS = 1;
    dataFlushing = SSPBUF;
    
} // end void SPI_send(uint8_t date).


/*******************************************************************************
 * Function uint8_t SPI_receive();
 * Receive data from the SPI.
 *******************************************************************************/
uint8_t SPI_receive()
{
    PIR1bits.SSPIF = 0; 
    uint8_t data;
    //CS = 0;
    SSPBUF = 0xFF;
    while(!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
    data = SSPBUF;
    //CS = 1;
    return (data);
    
    
} // end function uint8_t SPI_receive()

/*******************************************************************************
 * Function uint8_t SPI_dataIn();
 * Checks for data in the SPI buffer.
 *******************************************************************************/
uint8_t SPI_dataIn()
{
  if ( SSPSTATbits.BF )
    return (1); // data in SSPBUF register
  else
    return (0); // no data in SSPBUF register
  
} // end function uint8_t SPI_dataIn.