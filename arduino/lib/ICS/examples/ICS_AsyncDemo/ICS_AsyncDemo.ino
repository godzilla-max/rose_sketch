// ICS Servo sample (Asynchrotous API)
#include <Arduino.h>
#include <ICS.h>

IcsController ICS(Serial1);
IcsServo servo1;
IcsServo servo2;

void setup() {

  Serial.begin(115200);

  ICS.begin();
  servo1.attach(ICS, 0x00);
  servo2.attach(ICS, 0x01);

  servo1.setPosition(7500-4000);
  servo2.setPosition(7500+4000);

  delay(1000);
}

void loop() {

  for(int position=-4000; position<=4000; position+=100){
    servo1.requestPosition(7500+position);
    servo2.requestPosition(7500-position);

    while(!ICS.isReady())
    {
        ICS.loop();
        ; // Do any other process while communicating!
    }

    Serial.print(position); Serial.print("\t");
    Serial.print(servo1.position); Serial.print("\t");
    Serial.print(servo2.position); Serial.print("\n");

    delay(20);
  }
  for(int position=-4000; position<=4000; position+=100){
    servo1.requestPosition(7500-position);
    servo2.requestPosition(7500+position);

    while(!ICS.isReady())
    {
        ICS.loop();
        ; // Do any other process while communicating!
    }

    Serial.print(position); Serial.print("\t");
    Serial.print(servo1.position); Serial.print("\t");
    Serial.print(servo2.position); Serial.print("\n");

    delay(20);
  }
}
