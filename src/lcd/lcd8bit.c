#include "lcd8bit.h"

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define send()                  LCD_CONT_PORT |=  (1 << LCD_CONT_EN);   \
                                _delay_us(0.5);                         \
                                LCD_CONT_PORT &= ~(1 << LCD_CONT_EN)

#define data()                  LCD_CONT_PORT |=  (1 << LCD_CONT_RS)

#define inst()                  LCD_CONT_PORT &= ~(1 << LCD_CONT_RS)

#define write()                 LCD_DATA_DDR = 0xFF;                    \
                                LCD_CONT_PORT &= ~(1 << LCD_CONT_RW)

#define read()                  LCD_DATA_DDR = 0x00;                    \
                                LCD_DATA_PORT = 0xFF;                   \
                                LCD_CONT_PORT |=  (1 << LCD_CONT_RW)

#define lcd_wait()              while(lcd_inst_read() & 0b10000000);

static int write_char(char c, FILE *stream) {
    (void) stream;

    lcd_data_write(c);
    return 0;
}

static FILE file_out = FDEV_SETUP_STREAM(write_char, 0, _FDEV_SETUP_WRITE);

FILE *lcd_file_out() {
    return &file_out;
}

void lcd_display_mode(uint8_t display_on, uint8_t cursor_on, uint8_t blink_on) {
    uint8_t        tmp =  0b00001000;
    if(display_on) tmp |= 0b00000100;
    if(cursor_on)  tmp |= 0b00000010;
    if(blink_on)   tmp |= 0b00000001;
    lcd_inst_write(tmp);
}

void lcd_cursor_mode(uint8_t inc, uint8_t shift) {
    uint8_t   tmp =  0b00000100;
    if(inc)   tmp |= 0b00000010;
    if(shift) tmp |= 0b00000001;
    lcd_inst_write(tmp);
}

void lcd_init() {
    LCD_CONT_DDR |= (1 << LCD_CONT_RS) | (1 << LCD_CONT_RW) | (1 << LCD_CONT_EN);
    lcd_inst_write(0b00111000); // 8 bit, 2 line, 5x8 font
    lcd_cursor_mode(1, 0);
    lcd_display_mode(1, 0, 0);

}

void lcd_inst_write(uint8_t data) {
    lcd_wait();
    write(); inst();
    LCD_DATA_PORT = data;
    send();

}

uint8_t lcd_inst_read() {
    read(); inst();
    send();
    return LCD_DATA_PIN;
}

void lcd_data_write(uint8_t data) {
    lcd_wait();

    write(); data();
    LCD_DATA_PORT = data;
    send();

}

uint8_t lcd_data_read() {
    read(); data();
    send();
    return LCD_DATA_PIN;
}
