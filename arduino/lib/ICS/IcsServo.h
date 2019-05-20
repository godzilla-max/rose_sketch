#ifndef _ICS_SERVO_H_
#define _ICS_SERVO_H_

// KONDO ICS Serial Servo
// Based on ICS3.5

#include <stdint.h>

// buffer size
#define ICS_TX_BUFF_SIZE  3
#define ICS_RX_BUFF_SIZE  3

// Error Code
#define ERROR_TIMEOUT           0x01    // timeout
#define ERROR_VERIFY            0x02    // verify mismatch
#define ERROR_ABNORMAL          0x03    // abnormal state
#define ERROR_UNATTACHED        0x04    // unattached to controller

// ICS Serial Servo Controller
class IcsController;

// ICS Serial Servo
class IcsServo
{
    friend class IcsController;
    
public:
    // Common API
    void attach(IcsController& controller, uint8_t id, uint8_t icsver = 36); // set ICS controller and servo ID
    
    // Synchronous API
    uint16_t setPosition(uint16_t pos);     // set & get position
    uint8_t getStretch();                   // get stretch
    uint8_t getSpeed();                     // get speed
    uint8_t getCurrent();                   // get current
    uint8_t getTemperature();               // get temperature
    uint16_t getPosition();                 // get position
    bool setStretch(uint8_t stretch);       // set stretch
    bool setSpeed(uint8_t speed);           // set speed
    bool setCurrent(uint8_t current);       // set current limit
    bool setTemperature(uint8_t temp);      // set temperature limit
    bool readEEPROM(uint8_t *data);         // read data from EEPROM
    bool writeEEPROM(uint8_t *data);        // write data to EEPROM
    // only for ONE-ON-ONE connection!
    uint8_t readID();                       // read servo ID
    bool writeID(uint8_t id);               // write servo ID
    
    // Asynchronous API
    void requestPosition(uint16_t pos);     // request to set & get position
    void requestCurrent();                  // request to get current
    void requestTemperature();              // request to get temperature
    bool isReady();                         // requested communication completed?
    
    // ICS protocol version (used for getting position)
    void setProtcolVersion(uint8_t version);

public:
    uint16_t posTarget;         // target position (14bit raw value)
    uint16_t position;          // current position (14bit raw value)
    uint8_t  temperature;       // temperature (7bit raw value)
    uint8_t  current;           // current (7bit raw value)
    int      error;             // error code
    
private:
    uint8_t getParameter(uint8_t sc);               // read parameter
    bool setParameter(uint8_t sc, uint8_t value);   // write parameter
    bool transfer(uint8_t* tx_data, int tx_size, uint8_t* rx_data, int rx_size, int timeout); // UART send / receive
    void sendAsync();                               // asynchronous send
    void receiveAsync();                            // asynchronous receive
    void receivePositionAsync();                    // asynchronous receive (POSITION command)
    void receiveReadAsync();                        // asynchronous receive (READ command)
    void setError(uint8_t error);                   // set error code
    
private:
    IcsController *controller;  // servo controller
    IcsServo *next;             // next servo (for communication scheduling)
    uint8_t  ID;                // ICS servo ID
    
    // for Asynchronous API
    uint8_t request;                  // request flags
    uint8_t commandNow;               // current command
    uint8_t txData[ICS_TX_BUFF_SIZE]; // send buffer
    uint8_t rxData[ICS_RX_BUFF_SIZE]; // receive buffer
    int     rxCnt;                    // receive count
    bool    isReceiving;              // receiving flag

    uint8_t icsver;
};

#endif
