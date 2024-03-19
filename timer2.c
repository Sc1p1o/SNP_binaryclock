#include <avr/io.h >
#include <avr/interrupt.h >
#include <stdint.h>
#include <util/delay.h>

#define TICKS_PER_SECOND 16000000UL

volatile uint16_t ms;
volatile uint8_t s;
volatile uint8_t min;
volatile uint8_t h;
int run = 0;

uint8_t reverseBits(uint8_t value) {

    uint8_t result = 0;

    for (int i = 0; i < 8; ++i) {
        result <<= 1;
        result |= (value & 1);
        value >>= 1;
    }

    return result;
}

int main () {
    DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) |(1 << PC4) |(1 << PC5) | (0 << PC6);
    DDRD |= (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4);
    DDRB |= (1 << PB3);
	
    PORTC = 0x00;

    //Timer 2
    TCCR2A |= (1 << WGM21);                 // CTC-Modus aktivieren
    OCR2A |= 125-1;                         // counter is cleared to zero when the
                                            // counter value (TCNT2) matches the OCR2A
    TIMSK2 |= (1 << OCIE2A)                 // Compare Match a interrupt is enable
    TCCR2B |= (1 << CS22);                  // Prescaler 64
    sei();                                  // enable global interrupts

	while(1) {
		run=1;

	    ledHs = h >> 1;
	    ledHs = reverseBits(ledHs);
	    PORTD &= 0x0f;
	    PORTD |= ledHs && 0xf0;
	}
}

// STUNDEN
// 0 -> PORTC=  ; PORTB=
// 1 -> PORTC=  ; PORTB=
// 2 -> PORTC=  ; PORTB=
// 3 -> PORTC=  ; PORTB=
// 4





ISR (TIMER0_COMPA_vect) {
    ms++;

    //actions every second
    if(ms >= 100) {
        ms=0;
        s++;
		
        //Actions every minute
        if(s >= 60) {
            s=0;
            min++;
            PORTC++;

            //Actions every hour
            if(min >= 60) {
                min=0;
                PORTC = 0x00;
                h++;
                PORTB = (h & 0x01);

                //Actions every day
                if(h >= 24) {
                    h=0;

                }
            }
        }
    }
}
