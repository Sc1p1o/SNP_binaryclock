#include <avr/interrupt.h >
#include <avr/io.h >
#include <stdint.h>

volatile uint16_t ms;
volatile uint8_t s;
volatile uint8_t min;
volatile uint8_t h;

void main () {
    TCCR0B |= (1 << CS01 );     // ps =8
    OCR0A = 255 - 1;
    TCCR0A |= (1 << WGM01 );    // enable CTC
    TIMSK0 |= (1 << OCIE0A );   // enable compare interrupt 1A
    sei() ;                     // enable global interrupts
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

            //Actions every hour
            if(min >= 60) {
                min=0;
                h++;

                //Actions every day
                if(h >= 24) {
                    h=0;

                }
            }
        }
    }
}