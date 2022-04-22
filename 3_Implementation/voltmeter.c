#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>
#include <stdlib.h>

#define enable            5
#define registerselection 6

void send_a_command(unsigned char command);
void send_a_character(unsigned char character);
void send_a_string(char *string_of_characters);
int main(void)
{
    DDRB = 0xFF;
    DDRA = 0;
    DDRD = 0xFF;
    _delay_ms(50);
        ADMUX |=(1<<REFS0);
    ADCSRA |=(1<<ADEN)|(1<<ADATE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
    float voltage = 0;
    char voltageshow [7];
    send_a_command(0x01); 
    _delay_ms(50);
    send_a_command(0x38);
    _delay_ms(50);
    send_a_command(0b00001111);
    _delay_ms(50);
    ADCSRA |=(1<<ADSC);
    while(1)
    {
        voltage = ADC/204.8*18;
        send_a_string ("WELCOME ");
        send_a_command(0x80 + 0x40 + 0);
        send_a_string ("VOLTAGE ");
        send_a_command(0x80 + 0x40 + 8);
        dtostrf(voltage, 5, 2, voltageshow);
        send_a_string(voltageshow);
        send_a_string("v  ");
        send_a_command(0x80 + 0);
        
    }
}
void send_a_command(unsigned char command)
{
    PORTB = command;
    PORTD &= ~ (1<<registerselection);
    PORTD |= 1<<enable;
    _delay_ms(2);
    PORTD &= ~1<<enable;
    PORTB = 0;
}
void send_a_character(unsigned char character)
{
    PORTB = character;
    PORTD |= 1<<registerselection;
    PORTD |= 1<<enable;
    _delay_ms(2);
    PORTD &= ~1<<enable;
    PORTB = 0;
}
void send_a_string(char *string_of_characters)
{
    while(*string_of_characters > 0)
    {
        send_a_character(*string_of_characters++);
    }
}