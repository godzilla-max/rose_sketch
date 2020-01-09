/* GR-ROSE Sketch template V1.05 */
#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

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
	vTaskDelay(100);
}

void loop2(void *pvParameters) {
	while (1) {
		digitalWrite(PIN_LED2, !digitalRead(PIN_LED2));
		Serial.println("loop2");
		vTaskDelay(500);
	}
}
