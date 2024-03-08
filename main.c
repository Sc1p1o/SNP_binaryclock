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

    PORTC = 0x00;

    TCCR1B |= (1 << WGM12);                 // CTC-Modus aktivieren
    OCR1A = TICKS_PER_SECOND / 1000 - 1;    // Vergleichswert für 1ms
    TIMSK1 |= (1 << OCIE1A);                // Compare-Match-Interrupt aktivieren
    TCCR1B |= (1 << CS10) | (1 << CS11);    // Prescaler 64
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

                //Actions every day
                if(h >= 24) {
                    h=0;

                }
            }
        }
    }
}