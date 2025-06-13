#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdint.h>
#include <stdbool.h>

//RS & E Port $ Pin
// RS - > A1 E -> A3
#define LCD_RS_PORT GPIOA
#define LCD_RS_PIN GPIO1
#define LCD_E_PORT GPIOA
#define LCD_E_PIN GPIO3

//D4-D7 = PB0, PB1, PB10, PB11
// Data bus port and pin config
#define LCD_DATA_PORT GPIOB
#define LCD_D4 GPIO0
#define LCD_D5 GPIO1
#define LCD_D6 GPIO10
#define LCD_D7 GPIO11

static void delay_ms(int ms){
    for (int i =0; i < ms * 8000; i++) __asm__("nop");
}

static void lcd_pulse_enable() {
    gpio_set(LCD_E_PORT, LCD_E_PIN);
    delay_ms(1);
    gpio_clear(LCD_E_PORT, LCD_E_PIN);
    delay_ms(1);
}

static void lcd_write_nibble(uint8_t nibble) {
    gpio_clear(LCD_DATA_PORT, LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);

    if (nibble & 0x01) gpio_set(LCD_DATA_PORT, LCD_D4);
    if (nibble & 0x02) gpio_set(LCD_DATA_PORT, LCD_D5);
    if (nibble & 0x04) gpio_set(LCD_DATA_PORT, LCD_D6);
    if (nibble & 0x08) gpio_set(LCD_DATA_PORT, LCD_D7);

    lcd_pulse_enable();

}

static void lcd_send_byte(uint8_t byte, bool is_data) {

    if (is_data)
        gpio_set(LCD_RS_PORT, LCD_RS_PIN); // make RS 1 which will tell it that this is data, a character like 'A'

    else
        gpio_clear(LCD_RS_PORT, LCD_RS_PIN); // Set RS to zero


    lcd_write_nibble(byte >> 4); // send  upper 4 bits
    lcd_write_nibble(byte & 0x0F); // send lower 4 bits

    delay_ms(2);

}

static void lcd_command(uint8_t cmd){
    lcd_send_byte(cmd, false);
}

static void lcd_data(uint8_t data){
    lcd_send_byte(data, true);
}



static void lcd_init(){
    delay_ms(50);

    lcd_write_nibble(0x03); delay_ms(5); // First 0x03
    lcd_write_nibble(0x03); delay_ms(5); // Second 0x03
    lcd_write_nibble(0x03); delay_ms(5); // Third 0x03
    lcd_write_nibble(0x02); // Set 4-bit mode

    lcd_command(0x28); // Function set: 4-bit, 2-line, 5x8 dots
    lcd_command(0x0C); // Display on, cursor off, blink off
    lcd_command(0x06); // Entry mode: move cursor right
    lcd_command(0x01); // Clear display
    delay_ms(2);

}

static void lcd_set_cursor(uint8_t row, uint8_t col){

    uint8_t addr = (row == 0) ? 0x00 : 0x40; // row 1 = 0x00, row2 == 0x40
    lcd_command(0x80 | (addr + col));

}

static void lcd_write_string(const char *str){
    while (*str){
        lcd_data(*str++);
    }
}

int main(void){

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    gpio_set_mode(LCD_RS_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_RS_PIN);

    gpio_set_mode(LCD_E_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_E_PIN);

    gpio_set_mode(LCD_DATA_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);

    lcd_init();

    lcd_set_cursor(0, 0);

    lcd_write_string("Hello World");
    while (1);

}