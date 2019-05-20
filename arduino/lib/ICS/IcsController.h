#ifndef _ICS_CONTROLLER_H_
#define _ICS_CONTROLLER_H_

// KONDO ICS Serial Servo Controller
// Based on ICS3.5

#include <stdint.h>
#include <Arduino.h>

// ICS Serial Servo
class IcsServo;

// ICS Serial Servo Controller
class IcsController
{
    friend class IcsServo;
    
public:
    // API
    IcsController(HardwareSerial& serial);
    void begin(int baud=115200); // start ICS controller
    void loop();    // call this in main loop (for Asynchronous API)
    bool isReady(); // requested communication completed? (for Asynchronous API)
    
    void (*onError)(uint8_t, uint8_t);  // callback on error

private:
    int getRxSize();                    // get received data size
    uint8_t read();                     // read received data (1 byte)
    int write(uint8_t *data, int len);  // write data to send
    void setTimeout(int ms);            // set timeout [msec]
    bool isTimeout();                   // is timeout?
    
private:
    HardwareSerial *serial; // serial port
    IcsServo *servoFirst;   // first attached servo (for communication scheduling)
    IcsServo *servoLast;    // last attached servo (for communication scheduling)
    IcsServo *servoNow;     // now communicating servo (for communication scheduling)
    uint32_t T1;            // for timeout (start time)
    uint32_t T2;            // for timeout (limit time)
    
    // ICS servo needs wait after communication
    bool waiting;           // wait flag
    uint32_t T3;            // start time of wait
    uint32_t T4;            // time to wait
};

#endif
