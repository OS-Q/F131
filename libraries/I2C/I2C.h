
#if(ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

//#include <inttypes.h>

#ifndef I2C_h
#define I2C_h

/* TWI status codes */
/*
 * return values for functions that use the timeOut feature
 * will now return at what point in the transmission the timeout
 * occurred. Looking at a full communication sequence between a
 * master and slave (transmit data and then readback data) there
 * a total of 7 points in the sequence where a timeout can occur.
 *
 * These are listed below and correspond to the returned value:
 *
 * 0 - Function executed with no errors
 * 1 - Waiting for successful completion of a Start bit
 * 2 - Waiting for ACK/NACK while addressing slave in transmit mode (MT)
 * 3 - Waiting for ACK/NACK while sending data to the slave
 * 4 - Waiting for successful completion of a Repeated Start
 * 5 - Waiting for ACK/NACK while addressing slave in receiver mode (MR)
 * 6 - Waiting for ACK/NACK while receiving data from the slave
 * 7 - Waiting for successful completion of the Stop bit
 * 0x38	- arbitration lost
 */

#define LOST_ARBTRTN    0x38

void I2C_begin();
void I2C_end();
void I2C_timeOut(uint16_t);
void I2C_setSpeed(uint8_t);
void I2C_pullup(uint8_t);
void I2C_scan();
uint8_t I2C_available();
uint8_t I2C_receive();
uint8_t I2C_write(uint8_t, uint8_t);
uint8_t I2C_write_reg(uint8_t, uint8_t, uint8_t);
uint8_t I2C_write_s(uint8_t, uint8_t, char *);
uint8_t I2C_write_sn(uint8_t, uint8_t, uint8_t *, uint8_t);
uint8_t I2C_read(uint8_t address, uint8_t numberBytes);
uint8_t I2C_read_reg(uint8_t address, uint8_t registerAddress,
		     uint8_t numberBytes);
uint8_t I2C_readbuf(uint8_t address, uint8_t numberBytes, uint8_t * dataBuffer);
uint8_t I2C_readbuf_reg(uint8_t address, uint8_t registerAddress,
			uint8_t numberBytes, uint8_t * dataBuffer);

// not sure if these really need to be public:
//    extern uint8_t returnStatus;
//    extern uint8_t data[MAX_BUFFER_SIZE];

#endif
