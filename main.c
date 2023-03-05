/* File:  main.c                                     * Date: 02/19/2023
 * **************************************************************************************
 * Description: Function test program for SPI communication and CAN network.
 * 
 * Environment: MPLAB v6.05, XC8 v2.40, PIC18F4550, MCP2515 e TJA1050.
 *                     Placa de desenvolvimento FATEC (FATEC board, http://fatecsantoandre.edu.br/).
 * 
 * Author: Antonio Aparecido Ariza Castilho;
 * 
 * MIT License  (see at: LICENSE em github)
 * Copyright (c) 2021 Antonio Castilho <https://github.com/AntonioCastilho>
 ****************************************************************************************/
#include <xc.h>
#include "config_bits.h"
#include "can.h"
#include "hardware.h"

uint8_t dataRead[8];
uint8_t dataSend[8];


void main(void) 
{
    hardware_ini();
    
    dataSend[0] = 0xAB;
    
    canMessageSend.dlc = 8;
   
    /*canMessageSend.idh = 0x20;   //0001 0100
    canMessageSend.data[0]= 0xAA; //1010 1010
    canMessageSend.data[1]= 0xAF; //1010 1111
    canMessageSend.data[2]= 0xFA; //1111 1010
    canMessageSend.data[3]= 0xAB; //1010 1011
    canMessageSend.data[4]= 0xCE; //1100 1110
    canMessageSend.data[5]= 0xAE; //1010 1110
    canMessageSend.data[6]= 0xBE; //1011 1110
    canMessageSend.data[7]= 0xFE; //1111 1110
    
    /*canMessageSend.idh = 0x10;   //0000 1010
    canMessageSend.data[0]= 0x20; //0001 0100
    canMessageSend.data[1]= 0x30; //0001 1110
    canMessageSend.data[2]= 0x40; //0010 1000
    canMessageSend.data[3]= 0x50; //0011 0010
    canMessageSend.data[4]= 0x60; //0011 1100
    canMessageSend.data[5]= 0x70; //0100 0110
    canMessageSend.data[6]= 0x80; //0101 0000
    canMessageSend.data[7]= 0x90; //0101 1010*/
    
    while(1)
    {
        //mcp2515MessageSend(&canMessageSend);
        
        canSend(0, 0x10, 8, dataSend);
        
        //SPI_send(0xFE); //0b11111110
        
        LATBbits.LATB6 = 0;
        delayMS(100); 
        
        //SPI_send(0xAA); //0b10101010
        
        LATBbits.LATB6 = 1;
        canRead(0x20, dataRead);
        
        //mcp2515MessageRead(0x20, &canMessageReceived);
        
        delayMS(100);
        
    } // end while(1)
    
}// end main function
