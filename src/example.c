#include <avr/io.h>
#include <util/delay.h>

#include "i2c_master.h"
#include "lcd8bit.h"

#define ALAMAT_SENSOR       0xE0
#define MAX_RANGE_CM        150
#define MAX_RANGE_REGISTER  ((((MAX_RANGE_CM * 10) - 85) / 43) + 1)

int main() {
    lcd_init();
    stdout = lcd_file_out();

    i2c_init();

    // set max range cm
    i2c_transmit(ALAMAT_SENSOR, 2, MAX_RANGE_REGISTER);

    printf("SRF08");

    while(1) {
        int16_t last_value;

        // probe dengan mode centimeter
        i2c_transmit(ALAMAT_SENSOR, 0, 81);

        // set delay 65ms, recomended dari
        // http://www.robot-electronics.co.uk/htm/srf08tech.shtml
        _delay_ms(65);

        // baca jarak, dan simpan ke last_value
        last_value = (i2c_read(ALAMAT_SENSOR, 2) << 8)
                   | (i2c_read(ALAMAT_SENSOR, 3) << 0);

        lcd_set_ddram_addr(0x40);
        printf("%8d", last_value);

        _delay_ms(500);
    }
}
