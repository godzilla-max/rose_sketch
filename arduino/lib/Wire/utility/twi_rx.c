/* Hardware I2cMaster Library
 * Copyright (C) 2014 Renesas Electronics
 *
 * This file is part of the RX63N Hardware I2cMaster Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the RX63N Hardware I2cMaster Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 *  Modified 10 Jun 2014 by Nozomu Fujita : twi_rx_init(uint8_t, int) の SCI1 と SCI2 への対応追加
 *  Modified 10 Jun 2014 by Nozomu Fujita : twi_rx_setFrequency(uint8_t, int) のコード簡略化
 *  Modified 13 Jun 2014 by Nozomu Fujita : PCR と PMR へのアクセスに BSET() を使用するよう変更した
 *
 */
#include <Arduino.h>
#include "twi_rx.h"
#include "utilities.h"
#include "util.h"
#include "rx65n/interrupt_handlers.h"
#include "iodefine.h"

static uint8_t twi_rxBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_rxBufferIndex;

static uint8_t slaveAddress = 0;
static void (*twi_onSlaveTransmit)(void);
static void (*twi_onSlaveReceive)(uint8_t*, int);
typedef struct {
    volatile uint32_t* mstpcr;
    uint8_t mstpBit;
    uint8_t sci;
    uint8_t sdaPin;
    uint8_t sclPin;
    uint8_t psel;
    uint32_t reg_offset;
    uint8_t ir_rie;
    uint8_t ir_tie;
    uint8_t irtei_offset;
} SciTable;

#define SCITABLE(sci, mstpcr, mstpBit, sdaPin, sclPin, psel, reg_offset, ir_rie, ir_tie, irtei_offset) { \
    mstpcr, \
    mstpBit, \
    sci, \
    sdaPin, \
    sclPin, \
	psel, \
	reg_offset, \
	ir_rie, \
	ir_tie, \
	irtei_offset, \
}

static const SciTable sciTable[] = {
#if defined(GRROSE)
	SCITABLE(0, 0, 0, 0, 0, 0, 0, 0, 0, 0), // 0: SoftwareSerial
    SCITABLE(2, &SYSTEM.MSTPCRB.LONG, 29, PIN_WIRE_SDA, PIN_WIRE_SCL, 0b01010, 0x40, 62, 63, 4),	// SCI2
    SCITABLE(1, &SYSTEM.MSTPCRB.LONG, 30, 9, 8, 0b01010, 0x20, 60, 61, 2),	// SCI1
#else
#error "no board is defined"
#endif
};

void twi_rx_init(uint8_t channel, int freq){

    int sdaPort = digitalPinToPort(sciTable[channel].sdaPin);
    int sclPort = digitalPinToPort(sciTable[channel].sclPin);
    int sdaBit = digitalPinToBit(sciTable[channel].sdaPin);
    int sclBit = digitalPinToBit(sciTable[channel].sclPin);

    SYSTEM.PRCR.WORD = 0xA50BU;
    *sciTable[channel].mstpcr &= ~(1UL << sciTable[channel].mstpBit);
    SYSTEM.PRCR.WORD = 0xA500U;

    // Configure SCI0's SCL, SDA pins for peripheral functions.

    BSET(portModeRegister(sdaPort), sdaBit); //SDA
    BSET(portOpendrainRegister(sdaPort, sdaBit), sdaBit*2);
    BSET(portModeRegister(sclPort), sclBit); //SCL
    BSET(portOpendrainRegister(sdaPort, sclBit), sclBit*2);

    // Configure the SCK, TX and RX.
    if(sciTable[channel].sci & 0x10){
        assignPinFunction(sciTable[channel].sdaPin, sciTable[channel].psel, 0, 0); // RIIC
        assignPinFunction(sciTable[channel].sclPin, sciTable[channel].psel, 0, 0); // RIIC
        RIIC0.ICCR1.BIT.ICE = 0;
        RIIC0.ICCR1.BIT.IICRST = 1;
        RIIC0.ICCR1.BIT.ICE = 1;
        RIIC0.SARL0.BYTE = slaveAddress; // lower slave address
        RIIC0.SARU0.BYTE = 0x00; // upper slave address
        RIIC0.ICSER.BYTE = 0x01;
        RIIC0.ICFER.BIT.NACKE = 0; // continue transfer even when NACK
        twi_rx_setFrequency(channel, freq);
        RIIC0.ICIER.BIT.RIE = 1; // for slave
        IPR(RIIC0, RXI0) = 0x3;
        IEN(RIIC0, RXI0) = 0x1;
        IR(RIIC0, RXI0) = 0x0;
        RIIC0.ICIER.BIT.TIE = 1; // for slave
        IPR(RIIC0, TXI0) = 0x3;
        IEN(RIIC0, TXI0) = 0x1;
        IR(RIIC0, TXI0) = 0x0;

        RIIC0.ICCR1.BIT.IICRST = 0;
    } else {
        assignPinFunction(sciTable[channel].sdaPin, sciTable[channel].psel, 0, 0); // SCI
        assignPinFunction(sciTable[channel].sclPin, sciTable[channel].psel, 0, 0); // SCI
        SCIx_I2C_SCR_BYTE(sciTable[channel].reg_offset) = 0x00;
        SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) = 0xF0;
        SCIx_I2C_SCMR_BYTE(sciTable[channel].reg_offset) = 0xFA; // MSB first
        twi_rx_setFrequency(channel, freq);
        SCIx_I2C_SIMR1_BYTE(sciTable[channel].reg_offset) = 0x19;
        SCIx_I2C_SIMR2_BYTE(sciTable[channel].reg_offset) = 0x23;
    }
}

void twi_rx_setAddress(uint8_t address)
{
  // set twi slave address (skip over TWGCE bit)
  slaveAddress = address << 1;
}

//------------------------------------------------------------------------------
/** Read a byte and send Ack if more reads follow else Nak to terminate read.
 *
 * \param[in] last Set true to terminate the read else false.
 *
 * \return The byte read from the I2C bus.
 */
uint8_t twi_rx_read(uint8_t channel, uint8_t last) {
    uint8_t b = 0;

    if(channel & 0x10){

        while(!RIIC0.ICSR2.BIT.RDRF);
        b = RIIC0.ICDRR;
        if(last){
            RIIC0.ICMR3.BIT.ACKWP = 1;
            RIIC0.ICMR3.BIT.ACKBT = 1; // Set NACK
            while(!RIIC0.ICSR2.BIT.RDRF);
            RIIC0.ICSR2.BIT.STOP = 0;
            RIIC0.ICCR2.BIT.SP = 1;
            char temp = RIIC0.ICDRR;
            while(RIIC0.ICSR2.BIT.STOP == 0);
            //for next transfer
            RIIC0.ICSR2.BIT.NACKF = 0;
            RIIC0.ICSR2.BIT.STOP = 0;
        }

    } else {
        SCIx_I2C_SIMR2_BYTE(sciTable[channel].reg_offset) &= 0b11011111;
        SCIx_I2C_SCR_BYTE(sciTable[channel].reg_offset) |= 0b01000000; // Enable RIE
        if (last){
            SCIx_I2C_SIMR2_BYTE(sciTable[channel].reg_offset) |= 0b00100000; // Set Ack
        }
        SCIx_I2C_TDR_BYTE(sciTable[channel].reg_offset) = 0xFF; // dummy write
        while(!SCIx_I2C_IR_RIE_BYTE(sciTable[channel].ir_rie));
        SCIx_I2C_IR_RIE_BYTE(sciTable[channel].ir_rie) = 0;
        b = SCIx_I2C_RDR_BYTE(sciTable[channel].reg_offset); // read data
        while(!SCIx_I2C_IR_TIE_BYTE(sciTable[channel].ir_tie));
        SCIx_I2C_IR_TIE_BYTE(sciTable[channel].ir_tie) = 0;
    }
    return b;
}
//------------------------------------------------------------------------------
/** Issue a restart condition.
 *
 * \param[in] addressRW I2C address with read/write bit.
 *
 * \return The value true, 1, for success or false, 0, for failure.
 */
bool twi_rx_restart(uint8_t channel, uint8_t addressRW) {
    if(channel & 0x10){
        RIIC0.ICCR2.BIT.RS = 1; // issue re-start condition
    } else {
    	SCIx_I2C_SCR_BYTE(sciTable[channel].reg_offset) = 0b10110000; // Enable TIE, TE, RE
        SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) &= 0b11110111; // Clear STIF
        SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) = 0b01010010; // Generate re-start condition
        while((SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) & 0b00001000) == 0); // wait for generating
    }
    return twi_rx_write(channel, addressRW);
}
//------------------------------------------------------------------------------
/** Issue a start condition.
 *
 * \param[in] addressRW I2C address with read/write bit.
 *
 * \return The value true, 1, for success or false, 0, for failure.
 */
bool twi_rx_start(uint8_t channel, uint8_t addressRW) {
    if(channel & 0x10){
        while(RIIC0.ICCR2.BIT.BBSY);
        RIIC0.ICCR2.BIT.ST = 1; // issue start condition
        while(!RIIC0.ICCR2.BIT.MST){}
    } else {
    	SCIx_I2C_SCR_BYTE(sciTable[channel].reg_offset) = 0b10110000; // Enable TIE, TE, RE
        SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) &= 0b11110111; // Clear STIF
        SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) = 0b01010001; // Generate start condition
        while((SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) & 0b00001000) == 0); // wait for generating
    }
    return twi_rx_write(channel, addressRW);
}
//------------------------------------------------------------------------------
  /**  Issue a stop condition. */
void twi_rx_stop(uint8_t channel) {
    if(channel & 0x10){

        if(RIIC0.ICCR2.BIT.TRS == 1){ // transfer mode
            while(!RIIC0.ICSR2.BIT.TEND);
            RIIC0.ICCR2.BIT.SP = 1; // issue stop condition
        } else {
            return;
        }

        while(RIIC0.ICSR2.BIT.STOP == 0);
        //for next transfer
        RIIC0.ICSR2.BIT.NACKF = 0;
        RIIC0.ICSR2.BIT.STOP = 0;

    } else {
        SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) &= 0b11110111; // Clear STIF
        SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) = 0b01010100; // Generate stop condition
        while((SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) & 0b00001000) == 0); // wait for generating
        SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) = 0b11110000;
    }
}

//------------------------------------------------------------------------------
/**
 * Write a byte.
 *
 * \param[in] data The byte to send.
 *
 * \return The value true, 1, if the slave returned an Ack or false for Nak.
 */
bool twi_rx_write(uint8_t channel, uint8_t data) {
    if(channel & 0x10){
        if(RIIC0.ICCR2.BIT.MST == 0){ // slave mode

            while(!RIIC0.ICSR2.BIT.TDRE){}
            RIIC0.ICFER.BIT.NACKE = 1;
            RIIC0.ICDRT = data;
            while(!RIIC0.ICSR2.BIT.TEND){}
            if(RIIC0.ICSR2.BIT.NACKF){
                char temp = RIIC0.ICDRR; // dummy read
                while(!RIIC0.ICSR2.BIT.STOP); // wait for stop condition
                //for next transfer
                RIIC0.ICSR2.BIT.NACKF = 0;
                RIIC0.ICSR2.BIT.STOP = 0;

            }
            RIIC0.ICFER.BIT.NACKE = 0;

        } else {  // master mode
            while(!RIIC0.ICSR2.BIT.TDRE);
            RIIC0.ICDRT = data;
        }
        return 0;
    } else {
        SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) &= 0b11110111; // Clear STIF
        SCIx_I2C_SIMR3_BYTE(sciTable[channel].reg_offset) &= 0b00001111; // Output SDA and SCL
        SCIx_I2C_TDR_BYTE(sciTable[channel].reg_offset) = data;
        while(!SCIx_I2C_IR_TIE_BYTE(sciTable[channel].ir_tie));
        SCIx_I2C_IR_TIE_BYTE(sciTable[channel].ir_tie) = 0;
        return ((SCIx_I2C_SISR_BYTE(sciTable[channel].reg_offset) & 0b00000001 ) == 0);
    }
}

//------------------------------------------------------------------------------
/**
 * Set frequency.
 *
 * \param[in] data The byte to send.
 *
 * \return The value true, 1, if the slave returned an Ack or false for Nak.
 */
void twi_rx_setFrequency(uint8_t channel, int freq) {
    if(channel & 0x10){
        RIIC0.ICMR1.BYTE = (uint8_t)(0x3u << 4); // 100kHz @ PCLK 48MHz
        RIIC0.ICBRH.BYTE = 0xF8;
        RIIC0.ICBRL.BYTE = 0xFD;

    } else {
        if (freq > (PCLK / ((32 << (2 * 3)) * 256)) && freq <= (PCLK / 32)) {
            int n;
            int N;
            for (n = 0; n <= 3; n++) {
                N = ((PCLK + (32 << (2 * n)) * freq - 1) / ((32 << (2 * n)) * freq)) - 1;
                if (N >= 0 && N <= 255) {
                    SCIx_I2C_SMR_BYTE(sciTable[channel].reg_offset) = (SCIx_I2C_SMR_CKS_UNMASK & SCIx_I2C_SMR_BYTE(sciTable[channel].reg_offset)) | n;
                    SCIx_I2C_BRR_BYTE(sciTable[channel].reg_offset) = N;
                    break;
                }
            }
        }

    }
}

/*
 * Function twi_attachSlaveRxEvent
 * Desc     sets function called before a slave read operation
 * Input    function: callback function to use
 * Output   none
 */
void twi_attachSlaveRxEvent( void (*function)(uint8_t*, int) )
{
  twi_onSlaveReceive = function;
}
/*
 * Function twi_attachSlaveTxEvent
 * Desc     sets function called before a slave write operation
 * Input    function: callback function to use
 * Output   none
 */
void twi_attachSlaveTxEvent( void (*function)(void) )
{
  twi_onSlaveTransmit = function;
}

void isr_riic0_receive(){
    if(RIIC0.ICCR2.BIT.MST == 0){ // slave mode
        twi_rxBufferIndex = 0;
        char b = RIIC0.ICDRR; // dummy read
        while(!RIIC0.ICSR2.BIT.STOP){ // detect stop condition
            while(!RIIC0.ICSR2.BIT.RDRF); // wait for data
            if(twi_rxBufferIndex < TWI_BUFFER_LENGTH){
              // put byte in buffer and ack
              twi_rxBuffer[twi_rxBufferIndex++] = RIIC0.ICDRR;
            }
        }
        RIIC0.ICSR2.BIT.STOP = 0;
        // put a null char after data if there's room
        if(twi_rxBufferIndex < TWI_BUFFER_LENGTH){
          twi_rxBuffer[twi_rxBufferIndex] = '\0';
        }
        // callback to user defined callback
        twi_onSlaveReceive(twi_rxBuffer, twi_rxBufferIndex);
        // since we submit rx buffer to "wire" library, we can reset it
        twi_rxBufferIndex = 0;
    }
}

void isr_riic0_transmit(){
    if(RIIC0.ICCR2.BIT.MST == 0){ // slave mode
        twi_onSlaveTransmit();
    }
}
