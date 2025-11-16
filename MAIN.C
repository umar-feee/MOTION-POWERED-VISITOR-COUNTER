// Pin Definitions
#define PIR1_PIN PD2
#define PIR2_PIN PD3
#define LED1_PIN PD4
#define LED2_PIN PD5
#define BUZZER_PIN PD6
#define LCD_RS PD7
#define LCD_E PB0
#define LCD_D4 PB1
#define LCD_D5 PB2
#define LCD_D6 PB3
#define LCD_D7 PB4

volatile int visitor_count = 0;
volatile uint8_t pir1_flag = 0;
volatile uint8_t pir2_flag = 0;

void pulse_enable() {
    PORTB |= (1 << LCD_E);
    _delay_us(1);
    PORTB &= ~(1 << LCD_E);
    _delay_us(50);
}

void lcd_send_nibble(uint8_t nibble) {
    if (nibble & 0x01) PORTB |= (1 << LCD_D4); else PORTB &= ~(1 << LCD_D4);
    if (nibble & 0x02) PORTB |= (1 << LCD_D5); else PORTB &= ~(1 << LCD_D5);
    if (nibble & 0x04) PORTB |= (1 << LCD_D6); else PORTB &= ~(1 << LCD_D6);
    if (nibble & 0x08) PORTB |= (1 << LCD_D7); else PORTB &= ~(1 << LCD_D7);
}

void lcd_command(uint8_t cmd) {
    PORTD &= ~(1 << LCD_RS);
    lcd_send_nibble(cmd >> 4);
    pulse_enable();
    lcd_send_nibble(cmd & 0x0F);
    pulse_enable();
    if (cmd == 0x01 || cmd == 0x02) _delay_ms(2);
     else _delay_us(50);
}

void lcd_data(uint8_t data) {
    PORTD |= (1 << LCD_RS);
    lcd_send_nibble(data >> 4);
    pulse_enable();
    lcd_send_nibble(data & 0x0F);
    pulse_enable();
    _delay_us(50);
}

void lcd_init() {
    DDRB |= (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7) | (1 << LCD_E);
    DDRD |= (1 << LCD_RS);
    _delay_ms(50);

    PORTD &= ~(1 << LCD_RS);
    lcd_send_nibble(0x03); pulse_enable(); _delay_ms(5);
    pulse_enable(); _delay_ms(1);
    pulse_enable(); _delay_us(200);
    lcd_send_nibble(0x02); pulse_enable(); _delay_us(50);

    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);
}

void lcd_string(const char *str) {
    while (*str) lcd_data(*str++);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? col : (0x40 + col);
    lcd_command(0x80 | addr);
}

void lcd_print_int(int val) {
    char buffer[16];
    sprintf(buffer, "%d", val);
    lcd_string(buffer);
}

void buzz_and_flash() {
    PORTD |= (1 << BUZZER_PIN);
    _delay_ms(100);
    PORTD &= ~(1 << BUZZER_PIN);
}

int main() {
    // LED1, LED2, BUZZER and LCD RS pins outputs.
    DDRD |= (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << BUZZER_PIN) | (1 << LCD_RS);
    //E and data pins on Port B outputs
    DDRB |= (1 << LCD_E) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);
     // PIRs input
    DDRD &= ~((1 << PIR1_PIN) | (1 << PIR2_PIN)); 

    lcd_init();
    lcd_set_cursor(0, 0);
    lcd_string("Visitors:");

    while (1) {
        uint8_t pir1_state = PIND & (1 << PIR1_PIN);
        uint8_t pir2_state = PIND & (1 << PIR2_PIN);

        // LED indicators
        if (pir1_state) PORTD |= (1 << LED1_PIN); else PORTD &= ~(1 << LED1_PIN);
        if (pir2_state) PORTD |= (1 << LED2_PIN); else PORTD &= ~(1 << LED2_PIN);

        // Entry Logic: PIR1 then PIR2
        if (pir1_state && !pir1_flag && !pir2_flag) {
            pir1_flag = 1;
        }
        if (pir2_state && pir1_flag) {
            visitor_count++;
            buzz_and_flash();
            pir1_flag = 0;
            pir2_flag = 0;
            while (pir1_state || pir2_state) { // wait for reset
                pir1_state = PIND & (1 << PIR1_PIN);
                pir2_state = PIND & (1 << PIR2_PIN);
            }
        }

        // Exit Logic: PIR2 then PIR1
        if (pir2_state && !pir2_flag && !pir1_flag) {
            pir2_flag = 1;
        }
        if (pir1_state && pir2_flag) {
            if (visitor_count > 0) visitor_count--;
            buzz_and_flash();
            pir1_flag = 0;
            pir2_flag = 0;
            while (pir1_state || pir2_state) { // wait for reset
                pir1_state = PIND & (1 << PIR1_PIN);
                pir2_state = PIND & (1 << PIR2_PIN);
            }
        }

        // Update LCD
        lcd_set_cursor(1, 0);
        lcd_print_int(visitor_count);
        lcd_string("   ");
    }
}  







