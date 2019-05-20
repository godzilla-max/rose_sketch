// KONDO ICS Serial Servo
// Based on ICS3.5

#include <stdio.h>
#include <string.h>
#include "IcsController.h"
#include "IcsServo.h"

/****************************************
 Constants of ICS Protocol
 ****************************************/

// CMD(command)
#define CMD_POSITION    0x80    // POSITION command
#define CMD_READ        0xA0    // READ command
#define CMD_WRITE       0xC0    // WRITE command
#define CMD_ID          0xE0    // ID command

#define CMD_MASK        0xE0    // bit mask of command
#define ID_MASK         0x1F    // bit mask of ID
#define MSB_MASK        0x7F    // bit mask of MSB

// SC(sub command) of READ/WRITE command
#define SC_EEPROM       0x00    // EEPROM
#define SC_STRETCH      0x01    // stretch
#define SC_SPEED        0x02    // speed
#define SC_CURRENT      0x03    // current
#define SC_TEMPERATURE  0x04    // temperature
#define SC_POSITION     0x05    // position (read only)

// SC(sub command) of ID command
#define SC_READ_ID      0x00    // read ID
#define SC_WRITE_ID     0x01    // write ID

// verify CMD
#define ICS_CMD_CHECK(rx, tx, tx_size) \
    ((rx[0] == tx[0]) && ((rx[tx_size+0] & MSB_MASK) == (tx[0] & MSB_MASK)))

// verify CMD (read ID command)
#define ICS_CMD_GET_ID_CHECK(rx, tx, tx_size) \
    ((rx[0] == tx[0]) && ((rx[tx_size+0] & CMD_MASK & MSB_MASK) == (CMD_ID & MSB_MASK)))

// verify SC
#define ICS_SC_CHECK(rx, tx, tx_size) \
    ((rx[1] == tx[1]) && (rx[tx_size+1] == tx[1]))

// verify DATA
#define ICS_DATA_CHECK(rx, tx, tx_size) \
    ((rx[2] == tx[2]) && (rx[tx_size+2] == tx[2]))

/****************************************
 Local Constants
 ****************************************/

// request flag bit
#define REQ_POSITION       0x01    // set & get position
#define REQ_CURRENT        0x02    // get current
#define REQ_TEMPERATURE    0x04    // get temperature

/****************************************
 Common API
 ****************************************/

// set ICS controller and servo ID
void IcsServo::attach(IcsController& controller, uint8_t id, uint8_t icsver)
{
    this->controller = &controller;
    this->ID = id;
    this->icsver = icsver;
    
    // add this servo to servo chain
    if(controller.servoFirst == NULL)
    {
        controller.servoFirst = this;
        controller.servoLast = this;
        controller.servoNow = this;
    }
    else
    {
        controller.servoLast->next = this;
        controller.servoLast = this;
    }
    this->next = NULL;
}

/****************************************
 Synchronous API
 ****************************************/

// set & get position
// pos : target position
// return : current position
uint16_t IcsServo::setPosition(uint16_t pos)
{
    // command data
    uint8_t tx_data[3];
    tx_data[0] = CMD_POSITION | (ID & ID_MASK);// CMD
    tx_data[1] = (uint8_t)(pos >> 7) & 0x7F;   // POS_H
    tx_data[2] = (uint8_t)(pos & 0x7F);        // POS_L
    
    // send command and receive response
    uint8_t rx_data[3+3]={0};
    if(!this->transfer(tx_data, sizeof(tx_data), rx_data, sizeof(rx_data), 1)){
        return 0xFFFF;  // Error
    }
    
    // verify response data
    uint16_t retval;
    if(ICS_CMD_CHECK(rx_data, tx_data, 3)) // verify CMD
    {
        // current position
        retval = ((uint16_t)rx_data[3+1] << 7) | (uint16_t)rx_data[3+2];
    }else{
        retval = 0xFFFF;  // Error
    }
    return retval;
}

// read parameter (common routine)
// sc : sub command (type of parameter)
// return : value of parameter
uint8_t IcsServo::getParameter(uint8_t sc)
{
    // command data
    uint8_t tx_data[2];
    tx_data[0] = CMD_READ | (ID & ID_MASK); // CMD
    tx_data[1] = sc;  // SC
    
    // send command and receive response
    uint8_t rx_data[2+3]={0};
    if(!this->transfer(tx_data, sizeof(tx_data), rx_data, sizeof(rx_data), 1)){
        return 0xFF;  // Error
    }
    
    // verify response data
    uint8_t retval;
    if(   ICS_CMD_CHECK(rx_data, tx_data, 2) // verify CMD
       && ICS_SC_CHECK (rx_data, tx_data, 2)) // verify SC
    {
        // value of parameter
        retval = rx_data[2+2];
    }else{
        retval = 0xFF; // Error
    }
    return retval;
}

// get stretch
// return : stretch
uint8_t IcsServo::getStretch()
{
    return this->getParameter(SC_STRETCH);
}

// get speed
uint8_t IcsServo::getSpeed()
{
    return this->getParameter(SC_SPEED);
}

// get current
uint8_t IcsServo::getCurrent()
{
    return this->getParameter(SC_CURRENT);
}

// get temperature
uint8_t IcsServo::getTemperature()
{
    return this->getParameter(SC_TEMPERATURE);
}

// get position
uint16_t IcsServo::getPosition()
{
	if(icsver >= 36) {
	    // command data
	    uint8_t tx_data[2];
	    tx_data[0] = CMD_READ | (ID & ID_MASK); // CMD
	    tx_data[1] = SC_POSITION;  // SC

	    // send command and receive response
	    uint8_t rx_data[2+4]={0};
	    if(!this->transfer(tx_data, sizeof(tx_data), rx_data, sizeof(rx_data), 1)){
	        return 0xFF;  // Error
	    }

	    // verify response data
	    uint16_t retval;
	    if(   ICS_CMD_CHECK(rx_data, tx_data, 2) // verify CMD
	       && ICS_SC_CHECK (rx_data, tx_data, 2)) // verify SC
	    {
	        // value of parameter
	        retval = ((uint16_t)rx_data[3+1] << 7) | (uint16_t)rx_data[3+2];
	    }else{
	        retval = 0xFFFF; // Error
	    }
	    return retval;

	} else {
		uint16_t retval = setPosition(0);
		setPosition(retval);
	    return retval;
	}
}


// write parameter (common routine)
// sc : sub command (type of parameter)
// value : value of parameter
// return : success or failure
bool IcsServo::setParameter(uint8_t sc, uint8_t value)
{
    // command data
    uint8_t tx_data[3];
    tx_data[0] = CMD_WRITE | (ID & ID_MASK); // CMD
    tx_data[1] = sc;    // SC
    tx_data[2] = value; // value of parameter
    
    // send command and receive response
    uint8_t rx_data[3+3]={0};
    if(!this->transfer(tx_data, sizeof(tx_data), rx_data, sizeof(rx_data), 1)){
        return false;  // Error
    }
    
    // verify response data
    bool retval;
    if(   ICS_CMD_CHECK (rx_data, tx_data, 3)  // verify CMD
       && ICS_SC_CHECK  (rx_data, tx_data, 3)  // verify SC
       && ICS_DATA_CHECK(rx_data, tx_data, 3)) // verify DATA
    {
        retval = true;
    }else{
        retval = false;
    }
    return retval;
}

// set stretch
// stretch : stretch
// return : success or failure
bool IcsServo::setStretch(uint8_t stretch)
{
    return this->setParameter(SC_STRETCH, stretch);
}

// set speed
// speed : speed
// return : success or failure
bool IcsServo::setSpeed(uint8_t speed)
{
    return this->setParameter(SC_SPEED, speed);
}

// set current limit
// current : current limit
// return : success or failure
bool IcsServo::setCurrent(uint8_t current)
{
    return this->setParameter(SC_CURRENT, current);
}

// set temperature limit
// temp : temprature limit
// return : success or failure
bool IcsServo::setTemperature(uint8_t temp)
{
    return this->setParameter(SC_TEMPERATURE, temp);
}

// read data from EEPROM
// data : EEPROM data (out)
// return : success or failure
bool IcsServo::readEEPROM(uint8_t *data)
{
    // command data
    uint8_t tx_data[2];
    tx_data[0] = CMD_READ | (ID & ID_MASK); // CMD
    tx_data[1] = SC_EEPROM;  // SC
    
    // send command and receive response
    uint8_t rx_data[2+66]={0};
    if(!this->transfer(tx_data, sizeof(tx_data), rx_data, sizeof(rx_data), 10)){
        return false;  // Error
    }
    
    // verify response data
    bool retval;
    if(   ICS_CMD_CHECK(rx_data, tx_data, 2) // verify CMD
       && ICS_SC_CHECK (rx_data, tx_data, 2)) // verify SC
    {
        // EEPROM data
        memcpy(data, &rx_data[2+2], 64);
        
        retval = true;
    }else{
        retval = false;
    }
    return retval;
}

// write data to EEPROM
// data : EEPROM data
// return : success or failure
bool IcsServo::writeEEPROM(uint8_t *data)
{
    // command data
    uint8_t tx_data[66];
    tx_data[0] = CMD_WRITE | (ID & ID_MASK); // CMD
    tx_data[1] = SC_EEPROM;  // SC
    memcpy(&tx_data[2], data, 64);
    
    // send command and receive response
    uint8_t rx_data[66+2]={0};
    if(!this->transfer(tx_data, sizeof(tx_data), rx_data, sizeof(rx_data), 10)){
        return false;  // Error
    }
    
    // verify response data
    bool retval;
    if(   ICS_CMD_CHECK(rx_data, tx_data, 66) // verify CMD
       && ICS_SC_CHECK (rx_data, tx_data, 66)) // verify SC
    {
        retval = true;
    }else{
        retval = false;
    }
    return retval;
}

// read servo ID
// only for ONE-ON-ONE connection!
// return : servo ID
uint8_t IcsServo::readID()
{
    // command data
    uint8_t tx_data[4];
    tx_data[0] = CMD_ID;     // CMD
    tx_data[1] = SC_READ_ID; // SC
    tx_data[2] = SC_READ_ID; // SC
    tx_data[3] = SC_READ_ID; // SC
    
    // send command and receive response
    uint8_t rx_data[4+1]={0};
    if(!this->transfer(tx_data, sizeof(tx_data), rx_data, sizeof(rx_data), 1)){
        return 0xFF;  // Error
    }
    
    // verify response data
    uint8_t retval;
    if(ICS_CMD_GET_ID_CHECK(rx_data, tx_data, 4)) // verify CMD
    {
        // ID
        retval = rx_data[4+0] & ID_MASK;
    }else{
        retval = 0xFF; // Error
    }
    return retval;
}

// write servo ID
// only for ONE-ON-ONE connection!
// id : servo ID
// return : success or failure
bool IcsServo::writeID(uint8_t id)
{
    // command data
    uint8_t tx_data[4];
    tx_data[0] = CMD_ID | (id & ID_MASK); // CMD
    tx_data[1] = SC_WRITE_ID;// SC
    tx_data[2] = SC_WRITE_ID;// SC
    tx_data[3] = SC_WRITE_ID;// SC
    
    // send command and receive response
    uint8_t rx_data[4+1]={0};
    if(!this->transfer(tx_data, sizeof(tx_data), rx_data, sizeof(rx_data), 1)){
        return false;  // Error
    }
    
    // verify response data
    bool retval;
    if(ICS_CMD_CHECK(rx_data, tx_data, 4)) // verify CMD
    {
        retval = true;
    }else{
        retval = false;
    }
    return retval;
}

/****************************************
 Asynchronous API
 ****************************************/

// request to set & get position
// pos : target position
void IcsServo::requestPosition(uint16_t pos)
{
    this->error = 0;
    this->posTarget = pos;
    this->request |= REQ_POSITION;
}

// request to get current
void IcsServo::requestCurrent()
{
    this->error = 0;
    this->request |= REQ_CURRENT;
}

// request to get temperature
void IcsServo::requestTemperature()
{
    this->error = 0;
    this->request |= REQ_TEMPERATURE;
}

// requested communication completed?
bool IcsServo::isReady()
{
    // no more requests and not receiving?
    return ((this->request == 0) && !this->isReceiving);
}

/****************************************
 Private Functions
 ****************************************/

// UART send / receive
// tx_data: send data
// tx_size: send data size
// rx_data: receive data
// rx_size: receive data size
// timeout: time limit [ms]
// return: success or failure
bool IcsServo::transfer(uint8_t* tx_data, int tx_size, uint8_t* rx_data, int rx_size, int timeout)
{
    if(controller == NULL){
        setError(ERROR_UNATTACHED);
        return false;
    }
    
    // set time limit
    controller->setTimeout(timeout);
    
    // send command
    controller->write(tx_data, tx_size);
    
    // receive response
    bool retval = false;
    int rx_index = 0;
    while(rx_index < rx_size){
        int len = controller->getRxSize();
        int i;
        for(i=0;i<len;i++){
            rx_data[rx_index] = controller->read();
            rx_index++;
            if(rx_index >= rx_size){
                retval = true;
                break;
            }
        }
        // check time limit
        if(controller->isTimeout()){
            setError(ERROR_TIMEOUT);
            retval = false;
            break;
        }
        delayMicroseconds(1);
    }
    return retval;
}

#if 0
extern IcsController ics1;
extern IcsController ics2;
#endif

// asynchronous send
void IcsServo::sendAsync()
{
    if(controller == NULL){
        setError(ERROR_UNATTACHED);
    }
    
    int tx_size = 0;
    
    // Position
    if(request & REQ_POSITION){
        request &= ~REQ_POSITION;
        commandNow = CMD_POSITION;
        
        uint16_t pos = posTarget;
        txData[0] = CMD_POSITION | (ID & ID_MASK);// CMD
        txData[1] = (uint8_t)(pos >> 7) & 0x7F;   // POS_H
        txData[2] = (uint8_t)(pos & 0x7F);        // POS_L
        tx_size = 3;
    }
    // Current
    else if(request & REQ_CURRENT){
        request &= ~REQ_CURRENT;
        commandNow = CMD_READ;
        
        txData[0] = CMD_READ | (ID & ID_MASK); // CMD
        txData[1] = SC_STRETCH;  // SC
        tx_size = 2;
    }
    // Temperature
    else if(request & REQ_TEMPERATURE){
        request &= ~REQ_TEMPERATURE;
        commandNow = CMD_READ;
        
        txData[0] = CMD_READ | (ID & ID_MASK); // CMD
        txData[1] = SC_TEMPERATURE;  // SC
        tx_size = 2;
    }
    
    // send command
    if(tx_size != 0)
    {
        controller->write(txData, tx_size);
#if 0
        if(controller == &ics1){
            Serial.print("s");
        }else{
            Serial.print("S");
        }
#endif
        this->isReceiving = true;
        rxCnt = 0;
        
        // set time limit
        controller->setTimeout(1);
    }
}

// set error code
// error : error code
void IcsServo::setError(uint8_t error)
{
    this->error = error;
    this->isReceiving = false;
    
    if((controller != NULL) && (controller->onError != NULL))
    {
        controller->onError(error, ID);
    }
}

// asynchronous receive (POSITION command)
void IcsServo::receivePositionAsync()
{
    // proccess each byte of received data
    int len = controller->getRxSize();
    int i;
    for(i=0;i<len;i++){
        uint8_t bData = controller->read();
        
        // receive data format
        switch(rxCnt)
        {
        // [0-2] loopback
        case 0:
        case 1:
        case 2:
            // verify loopback
            if(bData == txData[rxCnt]){
                rxCnt++;
            }else{
                setError(ERROR_VERIFY); // Error
                return;
            }
            break;
        // [3] CMD
        case 3:
            // verify CMD and ID
            if((bData & MSB_MASK) == (txData[0] & MSB_MASK)){
                rxCnt++;
            }else{
                setError(ERROR_VERIFY); // Error
                return;
            }
            break;
        // [4] position upper 7bit
        case 4:
            rxData[0] = bData;
            rxCnt++;
            break;
        // [5] position lower 7bit
        case 5:
            rxData[1] = bData;
            // current position
            position = ((uint16_t)rxData[0] << 7) | (uint16_t)rxData[1];
            isReceiving = false;
#if 0
            if(controller == &ics1){
                Serial.print("r");
            }else{
                Serial.print("R");
            }
#endif
            return; // success
        // abnormal state
        default:
            setError(ERROR_ABNORMAL); // Error
            return;
        }
    }
}

// asynchronous receive (READ command)
void IcsServo::receiveReadAsync()
{
    // proccess each byte of received data
    int len = controller->getRxSize();
    int i;
    for(i=0;i<len;i++){
        uint8_t bData = controller->read();
        
        // receive data format
        switch(rxCnt)
        {
        // [0-1] loopback
        case 0:
        case 1:
            // verify loopback
            if(bData == txData[rxCnt]){
                rxCnt++;
            }else{
                setError(ERROR_VERIFY); // Error
                return;
            }
            break;
        // [2] CMD
        case 2:
            // verify CMD and ID
            if((bData & MSB_MASK) == (txData[0] & MSB_MASK)){
                rxCnt++;
            }else{
                setError(ERROR_VERIFY); // Error
                return;
            }
            break;
        // [3] SC
        case 3:
            // verify SC
            if(bData == txData[1]){
                rxCnt++;
            }else{
                setError(ERROR_VERIFY); // Error
                return;
            }
            break;
        // [4] DATA
        case 4:
            switch(txData[1]){
            // Current
            case SC_CURRENT:
                current = bData;
                break;
            // Temperature
            case SC_TEMPERATURE:
                temperature = bData;
                break;
            }
            isReceiving = false;
            return; // success
        // abnormal state
        default:
            setError(ERROR_ABNORMAL); // Error
            return;
        }
    }
}

// asynchronous receive
void IcsServo::receiveAsync()
{
    if(controller == NULL){
        setError(ERROR_UNATTACHED);
        return;
    }
    
    switch(commandNow)
    {
    // POSITION command
    case CMD_POSITION:
        this->receivePositionAsync();
        break;
    // READ command
    case CMD_READ:
        this->receiveReadAsync();
        break;
    }
    // check time limit
    if(controller->isTimeout()){
#if 0
        if(controller == &ics1){
            Serial.print("e");
        }else{
            Serial.print("E");
        }
#endif
        setError(ERROR_TIMEOUT); // Error
    }
    delayMicroseconds(1);
}

// ICS protocol version (used for getting position)
void IcsServo::setProtcolVersion(uint8_t version){
	icsver = version;
}
