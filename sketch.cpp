/* GR-ROSE Sketch template V0.95 */
#include <Arduino.h>
extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

void loop2(void *pvParameters);

void setup() {
	// LEDs
	pinMode(PIN_LED1, OUTPUT);
	pinMode(PIN_LED2, OUTPUT);

	// serial output to USB
	Serial.begin(9600);

	// loop2 task creation for sensing
	xTaskCreate(loop2, "LOOP2", 512, NULL, 2, NULL);
}

void loop() {
	digitalWrite(PIN_LED1, !digitalRead(PIN_LED1));
	Serial.println("loop");
	delay(100);
}

void loop2(void *pvParameters) {
	while (1) {
		digitalWrite(PIN_LED2, !digitalRead(PIN_LED2));
		Serial.println("loop2");
		delay(500);
	}
}
