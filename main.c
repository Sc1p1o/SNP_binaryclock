#include <avr/io.h >
#include <avr/interrupt.h >
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

#define TICKS_PER_SECOND 15000000UL

volatile uint16_t ms;
volatile uint8_t s;
volatile uint8_t min;
volatile uint8_t h;
volatile uint8_t ledHs;
volatile int pwm_time = 10;
volatile int pwm_counter = 0;
volatile uint8_t pwm_on = 1;

uint8_t reverseBits(uint8_t value) {

    uint8_t result = 0;

    for (int i = 0; i < 8; ++i) {
        result <<= 1;
        result |= (value & 1);
        value >>= 1;
    }

    return result;
}

ISR(TIMER2_OVF_vect) {
    //every second
    s++;
	

    //every minute
    if(s == 60) {
        s=0;
        min++;
        PORTC++;

        //Actions every hour
        if(min == 59) {
            min=0;
            PORTC &= 0b1000000;
            h++;
            PORTB = (h & 0x01);

            //every day
            if(h == 24) {
                h=0;

            }
        }
    }
    
}


int main () {
    DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) |(1 << PC4) |(1 << PC5) | (0 << PC6);
    DDRD |= (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4);
    DDRB |= (1 << PB0);

    PORTC = 0x00;

    //Timer 2
    TCCR2A |= 0x00; 				//normal mode                	
    TCCR2B |= (1 << CS20) | (1 << CS22);    	// prescaler 128
    TIMSK2 |= (1 << TOIE2);                 	// enable compare match interrupt

	ASSR |= (1 << AS2); 			        // asynchronous mode for timer 2

    sei();           
	
	
		while(1) {

	    if(pwm_on) {
	        ledHs = h >> 1;
	        ledHs = reverseBits(ledHs);
	        PORTD &= 0x0f;
	        PORTD |= ledHs && 0xf0;
	        PORTC = min;

	    } else {
	        PORTD &= 0x0f;
	        PORTC = 0x00;
	        PORTB &= 0b11111110;
	    }
	}
}

// STUNDEN
// 0 -> PORTD= 0x00 (0000 0000); PORTB= 0x00
// 1 -> PORTD= 0x00 (0000 0000); PORTB= 0x01

// 2 -> PORTD= 0x80 (1000 0000); PORTB= 0x00
// 3 -> PORTD= 0x80 (1000 0000); PORTB= 0x01

// 4 -> PORTD= 0x40 (0100 0000); PORTB= 0x00
// 5 -> PORTD= 0x40 (0100 0000); PORTB= 0x01

// 6 -> PORTD= 0xC0 (1100 0000); PORTB= 0x00
// 7 -> PORTD= 0xC0 (1100 0000); PORTB= 0x01

// 8 -> PORTD= 0x20 (0010 0000); PORTB= 0x00
// 9 -> PORTD= 0x20 (0010 0000); PORTB= 0x01

// 10 -> PORTD= 0xA0 (1010 0000); PORTB= 0x00
// 11 -> PORTD= 0xA0 (1010 0000); PORTB= 0x01

// 12 -> PORTD= 0x60 (0110 0000); PORTB= 0x00
// 13 -> PORTD= 0x60 (0110 0000); PORTB= 0x01

// 14 -> PORTD= 0xE0 (1110 0000); PORTB= 0x00
// 15 -> PORTD= 0xE0 (1110 0000); PORTB= 0x01

// 16 -> PORTD= 0x10 (0001 0000); PORTB= 0x00
// 17 -> PORTD= 0x10 (0001 0000); PORTB= 0x01

// 18 -> PORTD= 0x90 (1001 0000); PORTB= 0x00
// 19 -> PORTD= 0x90 (1001 0000); PORTB= 0x01

// 20 -> PORTD= 0x50 (0101 0000); PORTB= 0x00
// 21 -> PORTD= 0x50 (0101 0000); PORTB= 0x01

// 22 -> PORTD= 0xD0 (1101 0000); PORTB= 0x00
// 23 -> PORTD= 0xD0 (1101 0000); PORTB= 0x01
