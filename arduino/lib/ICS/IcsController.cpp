// KONDO ICS Serial Servo Controller
// Based on ICS3.5

#include <Arduino.h>
#include "IcsController.h"
#include "IcsServo.h"

/****************************************
 API
 ****************************************/

// constructor
// serial : serial port for ICS communication
IcsController::IcsController(HardwareSerial& serial)
{
    this->serial = &serial;
    
    onError = NULL;
    servoFirst = NULL;
    servoLast  = NULL;
    servoNow   = NULL;
    
    waiting = false;
}

// start ICS controller
void IcsController::begin(int baud)
{
    // ICS servo needs wait after communication
    T4 = 6*11*1000000 / baud + 400;
    
    // data 8bit, parity even, stop 1bit
    serial->begin(baud, SERIAL_8E1);
#ifdef GRROSE
    serial->direction(HALFDUPLEX);
#endif
}

// call this in main loop (for Asynchronous API)
void IcsController::loop()
{
    // ICS servo needs wait after communication
    if(waiting){
        uint32_t now = micros();
        uint32_t elapsed = now - T3;
        if((elapsed > T4)
           && (elapsed < UINT32_MAX/2)) // for GR-ROSE(beta)'s bug
        { 
            waiting = false;
        }
    }
    if(waiting) return;
    
    // if no servo, do nothing
    if(servoNow == NULL) return;
    
    // receive response
    if(servoNow->isReceiving)
    {
        servoNow->receiveAsync();
    }
    // send command
    else
    {
        // any request?
        if(servoNow->request)
        {
            servoNow->sendAsync();
            
            // ICS servo needs wait after communication
            waiting = true;
            T3 = micros();
        }
        // select next servo
        else
        {
            servoNow = (servoNow != servoLast) ? servoNow->next : servoFirst;
        }
    }
}

// requested communication completed? (for Asynchronous API)
bool IcsController::isReady()
{
    // if no servo, ready
    if(servoFirst == NULL) return true;
    
    // all servo is ready?
    for(IcsServo* servo = servoFirst; servo != NULL; servo = servo->next)
    {
        if(!servo->isReady()) return false;
    }
    return true;
}

/****************************************
 Private Functions
 ****************************************/

// get received data size
// return : received data size [byte]
int IcsController::getRxSize()
{
    return serial->available();
}

// read received data (1 byte)
// return : received data (1 byte)
uint8_t IcsController::read()
{
    return (uint8_t)(serial->read());
}

// write data to send
// data : data to send
// len : size of data to send
// return : size of sent data
int IcsController::write(uint8_t *data, int len)
{
    // discard trash data
    while(serial->available() > 0)
    {
        serial->read();
    }
    
    return serial->write(data, len);
}

// set timeout [msec]
// ms : time limit [msec]
void IcsController::setTimeout(int msec)
{
    uint32_t cnt = (uint32_t)(msec * 1000);
    T1 = micros();
    T2 = cnt;
}

// is timeout?
bool IcsController::isTimeout()
{
    uint32_t now = micros();
    uint32_t elapsed = now - T1;
    
    bool timeup = (
        (elapsed > T2)
    );
    return timeup;
}

