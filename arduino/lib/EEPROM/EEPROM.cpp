/*
  EEPROM.cpp - EEPROM library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/* Modified 20 April 2015 by Yuuki Okamiya for GR-SAKURA */
/* Modified 22 Feb. 2019 by Yuuki Okamiya for GR-ROSE */

/******************************************************************************
 * Includes
 ******************************************************************************/

#ifndef __RX600__
#include <avr/eeprom.h>
#else
extern "C" {
#include "r_flash_rx_if.h"
}
#endif
#include "EEPROM.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/

#ifdef GRROSE
#define MASK_PGM   0xfffffffc
#define MASK_BLOCK 0xffffffc0
#endif

EEPROMClass::EEPROMClass(){
	R_FLASH_Open();
}

uint8_t EEPROMClass::read(int address)
{
#ifndef __RX600__
	return eeprom_read_byte((unsigned char *) address);
#else
	flash_err_t err;
	flash_res_t result;
	err = R_FLASH_BlankCheck((uint32_t)FLASH_DF_BLOCK_0 + (address & MASK_PGM),
			FLASH_DF_MIN_PGM_SIZE,
			&result);
	if( err != FLASH_SUCCESS){
		//error
	} else {
		if(result == FLASH_RES_BLANK){
		    //blank
		    return 0xff;
		}
	}
	return *(volatile unsigned char *)(address + FLASH_DF_BLOCK_0);
#endif //__RX600__
}

void EEPROMClass::write(int address, uint8_t value)
{
#ifndef __RX600__
	eeprom_write_byte((unsigned char *) address, value);
#else
	uint8_t wbuf[FLASH_DF_MIN_PGM_SIZE];
	uint8_t rbuf[FLASH_DF_BLOCK_SIZE]; // for block write
	uint8_t bbuf[FLASH_DF_BLOCK_SIZE]; // for blank check
	uint32_t w_addr = address & MASK_PGM; // needed for blank check to target address
	uint32_t b_addr = address & MASK_BLOCK; // block address

	for (int i = 0; i < FLASH_DF_MIN_PGM_SIZE; i++){
	    if((address & (FLASH_DF_MIN_PGM_SIZE-1)) == i){
	        wbuf[i] = value;
	    } else {
            wbuf[i] = this->read(w_addr + i);
	    }
	}

	flash_res_t result;
	if(R_FLASH_BlankCheck((uint32_t)FLASH_DF_BLOCK_0 + w_addr,
			FLASH_DF_MIN_PGM_SIZE,
			&result) != FLASH_SUCCESS){
	} else {
		if(result == FLASH_RES_BLANK){
		    //blank
			R_FLASH_Write((uint32_t)wbuf, (uint32_t)FLASH_DF_BLOCK_0 + w_addr, FLASH_DF_MIN_PGM_SIZE);
		    return;
		} else {
		    // not blank, needed to read data in a block
		    for(uint8_t i = 0; i < FLASH_DF_BLOCK_SIZE; i+=FLASH_DF_MIN_PGM_SIZE){
		    	R_FLASH_BlankCheck((uint32_t)FLASH_DF_BLOCK_0 + b_addr + i,
		    				FLASH_DF_MIN_PGM_SIZE,
		    				&result);
		        if(result == FLASH_RES_BLANK){
		        	// blank, no need to read & write
	                bbuf[i] = 1;
		        } else {
		            // not blank, need to store the current data
	                bbuf[i] = 0;
	                if (i == (w_addr & (FLASH_DF_BLOCK_SIZE-1))){
	                    for(int j = 0 ; j < FLASH_DF_MIN_PGM_SIZE; j++){
	                        rbuf[i + j]   = wbuf[j];
	                    }
	                } else {
	                    for(int j = 0 ; j < FLASH_DF_MIN_PGM_SIZE; j++){
	                        rbuf[i + j]   = this->read(b_addr + i + j);
	                    }
	                }
		        }
		    }
		    R_FLASH_Erase((flash_block_address_t)(FLASH_DF_BLOCK_0 + (address & MASK_BLOCK)), 1);

		    for(int i = 0; i < FLASH_DF_BLOCK_SIZE; i+=FLASH_DF_MIN_PGM_SIZE){
	            if(bbuf[i]){ // do nothing because of blank

	            } else {
	                R_FLASH_Write((uint32_t)&rbuf[i], FLASH_DF_BLOCK_0 + b_addr + i, FLASH_DF_MIN_PGM_SIZE);
	            }
	        }
		}
	}

#endif //__RX600__
	return;
}

void EEPROMClass::erase(){
    R_FLASH_Erase(FLASH_DF_BLOCK_0, FLASH_NUM_BLOCKS_DF);
}


EEPROMClass EEPROM;
