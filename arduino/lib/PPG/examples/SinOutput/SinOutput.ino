// Programmable Pulse Generator(PPG) sample 
#include <Arduino.h>
#include <PPG.h>

#define	CYCLE_TIME		(1000)

void setup() {
  //Use Pin 1, Pin 3, Pin 5, Pin 7
  PPG.begin(true, true, true, true, 1000, 0b0000);
  PPG.setTrigger(0, 0, 0);
  PPG.setTrigger(1, 0, 0);
  PPG.setTrigger(2, 0, 0);
  PPG.setTrigger(3, 0, 0);
  PPG.start();
}

void loop() {
  int32_t		a[4], b[4], amp;

  amp = CYCLE_TIME / 4;
  for(int16_t th = 0; th < 360; th +=5 ) {
    delay(50);
    for(int i = 0; i < 4; i++) {
      a[i] = (amp * 1) + (int32_t)((float)amp * sinf((float)(th + (i * 90) +  0) * 3.14f / 180.0f));
      b[i] = (amp * 3) + (int32_t)((float)amp * sinf((float)(th + (i * 90) + 45) * 3.14f / 180.0f));
      PPG.setTrigger(i, a[i], b[i]);
    }
    PPG.enableTrigger();
  }
}
