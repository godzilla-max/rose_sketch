/*
 * serial_wrapper.c
 *
 *  Created on: 2019/03/22
 *      Author: a5034000
 */

#include <Arduino.h>
extern "C" void serial_print(const char* str){
	Serial.print(str);
}
