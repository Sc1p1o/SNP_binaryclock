#include <avr/io.h >
#include <avr/interrupt.h >
#include <stdint.h>
#include <util/delay.h>

#define TICKS_PER_SECOND 16000000UL

volatile uint16_t ms;
volatile uint8_t s;
volatile uint8_t min;
volatile uint8_t h;

void main () {
    DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) |(1 << PC4) |(1 << PC5);
    DDRD |= (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4);
    DDRB |= (1 << PB3);

    PORTC = 0x00;

    TCCR0A |= (1 << WGM01);                 // CTC-Modus aktivieren
    OCR0A = TICKS_PER_SECOND / 1000 - 1;    // Vergleichswert für 1ms
    TIMSK0 |= (1 << OCIE0B);                // Compare-Match-Interrupt aktivieren
    TCCR0B |= (1 << CS02);                  // Prescaler 64
    sei();                                  // enable global interrupts
}

ISR (TIMER0_COMPA_vect) {
    ms++;

    //actions every second
    if(ms >= 1000) {
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
                PORTB = (hours & 0x01);
                PORTD = (hours << 4);

                //Actions every day
                if(h >= 24) {
                    h=0;

                }
            }
        }
    }
}

// Stunden Ports aufsteigend: PB0, PD7, PD6, PD5, PD4