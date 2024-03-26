#include <avr/io.h >
#include <avr/interrupt.h >
#include <stdint.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define TICKS_PER_SECOND 15000000UL

volatile uint16_t ms;
volatile uint8_t s;
volatile uint8_t min;
volatile uint8_t h;
volatile uint8_t ledHs;
volatile int pwm_time = 100;
volatile int pwm_counter = 0;
volatile uint8_t pwm_on = 1;
volatile uint8_t power_on = 1;
volatile uint8_t sleep_button_available = 1;
volatile uint8_t schalt_min_counter = 0;

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
	if (schalt_min_counter == 20) {
		s++;
		schalt_min_counter = 0;
	}

	if(sleep_button_available == 0) {
		sleep_button_available = 1;
	}
    //every minute
    if(s >= 60) {
        s=0;
		schalt_min_counter++;
        min++;
			
        //Actions every hour
        if(min >= 59) {
            min=0;
            h++;

            //every day
            if(h >= 24) {
                h=0;

            }
        }
    }
}
ISR(INT0_vect) {

	if(sleep_button_available == 1) {
		sleep_button_available = 0;
		if(power_on == 1) {
			power_on = 0;
		} else {
			power_on = 1;
		}
	}
	
}


int main () {
    DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) |(1 << PC4) |(1 << PC5) | (0 << PC6);
    DDRD |= (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4);
    DDRB |= (1 << PB0);

    PORTC = 0x00;

	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);
	EICRA |= (1 << ISC01);
	EIMSK |= (1 << INT0);

    //Timer 2
    TCCR2A |= 0x00;              
    TCCR2B |= (1 << CS20) | (1 << CS22);    // prescaler 128
    TIMSK2 |= (1 << TOIE2);                

	ASSR |= (1 << AS2); 			        // asynchronous mode for timer 2

	set_sleep_mode(SLEEP_MODE_PWR_SAVE);

    sei();           
	
	
	while(1) {

		//sleep mode handling
		if (power_on == 1) {
			//pwm on or off phase depending on pwm_on
			if (pwm_on >= pwm_time) {
				ledHs = h >> 1;
				PORTB = (h & 0x01);
				PORTD &= (0x0f | reverseBits(ledHs));
				PORTC = min;
				pwm_on = 0;

			} else {
				PORTB &= 0x00;
				PORTC &= 0x00;
				PORTD &= 0x0f;
				pwm_on++;
			}

		} else {
			sleep_mode();
			PORTB &= 0x00;
			PORTC &= 0x00;
			PORTD &= 0x0f;
		}
	}
}
