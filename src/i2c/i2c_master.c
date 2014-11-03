#include "i2c_master.h"

#include <avr/io.h>
#include <util/twi.h>


void i2c_init()
{
    // Set Rate
    TWSR &= ~(_BV(TWPS1) | _BV(TWPS0));
    TWBR  = (uint8_t) (((F_CPU/I2C_BUS_FREQ)-16)/8);

    // Release SDA
    TWDR = 0xff;

    I2C_ENABLE();
}

void i2c_transmit(uint8_t address, uint8_t reg, uint8_t data)
{
    // START
    I2C_START();                    // send a start bit on i2c bus
    I2C_WAIT();                     // wait for confirmation of transmit

    // SLA_W
    TWDR = address & ~(1);          // load address of i2c device
    I2C_NEXT();                     // transmit
    I2C_WAIT();                     // wait for confirmation of transmit
    I2C_IF_STATUS(TW_MT_SLA_NACK) goto L_STOP;

    // reg
    TWDR = reg;
    I2C_NEXT();                     // transmit
    I2C_WAIT();                     // wait for confirmation of transmit

    // data
    TWDR = data;
    I2C_NEXT();                     // transmit
    I2C_WAIT();                     // wait for confirmation of transmit

L_STOP:
    // STOP
    I2C_STOP();                     // stop bit
}

uint8_t i2c_read(uint8_t address, uint8_t reg)
{
    char read_data = 0xff;

    // START
    I2C_START();                    // send a start bit on i2c bus
    I2C_WAIT();                     // wait for confirmation of transmit

    // SLA_W
    TWDR = address & ~(1);          // load address of i2c device
    I2C_NEXT();                     // transmit
    I2C_WAIT();                     // wait for confirmation of transmit
    I2C_IF_STATUS(TW_MT_SLA_NACK) goto L_STOP;

    // reg
    TWDR = reg;                     // send register number to read from
    I2C_NEXT();                     // transmit
    I2C_WAIT();                     // wait for confirmation of transmit

    // REP_START
    I2C_START();                    // send repeated start bit
    I2C_WAIT();                     // wait for confirmation of transmit

    // SLA_R
    TWDR = address | (1);           // transmit address of i2c device with readbit set
    I2C_NEXT() | (1 << TWEA);       // clear transmit interupt flag
    I2C_WAIT();                     // wait for confirmation of transmit

    I2C_NEXT();                     // transmit, nack (last byte request)
    I2C_WAIT();                     // wait for confirmation of transmit

    read_data = TWDR;               // and grab the target data

L_STOP:
    I2C_STOP();                // send a stop bit on i2c bus

    return read_data;
}
