#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

#include <avr/io.h>
#include "i2c_master_conf.h"

void i2c_init();
void i2c_transmit(uint8_t address, uint8_t reg, uint8_t data);
uint8_t i2c_read(uint8_t address, uint8_t reg);


#define I2C_ENABLE()        TWCR |= _BV(TWEN)
#define I2C_DISABLE()       TWCR = 0
#define I2C_START()         TWCR = (_BV(TWEN) | _BV(TWINT) | _BV(TWSTA))
#define I2C_STOP()          TWCR = (_BV(TWEN) | _BV(TWINT) | _BV(TWSTO))
#define I2C_NEXT()          TWCR = (_BV(TWEN) | _BV(TWINT))
#define I2C_WAIT()          loop_until_bit_is_set(TWCR, TWINT)
#define I2C_IF_STATUS(st)   if(TWSR == st)


#endif /* I2C_MASTER_H_ */
