#include <avr/io.h >
#include <avr/interrupt.h >
#include <stdint.h>
#include <util/delay.h>

#define TICKS_PER_SECOND 15000000UL

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

    // Pin PB2 als Eingang konfigurieren
    DDRD &= ~(1 << PD2);

    // INT0 konfigurieren: Low-Level-Trigger
    EICRA |= (0 << ISC01) | (0 << ISC00);

    // INT0 aktivieren
    EIMSK |= (1 << INT0);


    DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) |(1 << PC4) |(1 << PC5) | (0 << PC6);
    DDRD |= (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4);
    DDRB |= (1 << PB0);
	
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

// Resultat: 
// PORTB immer abwechselnd 0 und 1
// PORTD "zählt nach rechts hoch"



ISR (TIMER2_COMPA_vect) {
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

//POWER CHANGE INTERRUPT
ISR(INT0_vect){

}

ISR(PCINT1_vect){

}

ISR(PCINT2_vect){

}
