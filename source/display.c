#include <stdint.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/power.h>

#include "util.h"
#include "accel.h"
#include "leds.h"
#include "display.h"
#include "status.h"

//how much internalt positions do we have
#define SCALE 50u
#define SCALED_POS 12u*SCALE
//how is the physics model configured
#define FORCE_FACTOR 0.0075
#define DAMPING 0.9925
//what is the display speed
#define PRESCALER 4
#define TOP 100

int16_t current_position=0;
volatile uint8_t position;
double velocity=0;
const double arc_factor = (1.0/((double)SCALED_POS))*M_PI*2;

//local helpers
void test_display(void); 
void start_loop(void); 
void stop_loop(void);

void init_display() {
	test_display();
	start_loop();
	set_display_on();
	//we want the pos scaled to 12 leds & 10 steps
}

void update_display() {

	if (is_display_on()) {
		struct accel_vect accel_vector = read_accel();

		//convert pos to real arc angle
		double angle = current_position*arc_factor;
		velocity -= FORCE_FACTOR*((double)accel_vector.y*sin(angle)-(double)accel_vector.x*cos(angle));
		velocity *=DAMPING;

		current_position += (int)velocity;

		if (current_position<0) {
			current_position = SCALED_POS + current_position;
		} else if (current_position >= SCALED_POS) {
			current_position = current_position - SCALED_POS;
		}

		//little trick to do something like real rounding
		position=(uint8_t)((current_position+(SCALE/2))/SCALE);
		light_led(position);
		//light_led(get_current_position());
	}
	clear_update_display();
}

uint8_t get_current_position(void) {
	(uint8_t)(position);
}

void test_display(void) {
	light_pattern(0xfff);
	delay_ms(250);
	light_pattern(0);
	delay_ms(250);
	light_pattern(0xfff);
	delay_ms(250);
	light_pattern(0);
	uint8_t i;
	for (i=0; i<12; i++) {
		light_led(i);
		delay_ms(70);
	}
}

void switch_on_display(void) {
	set_display_on();
	start_loop();
}

void switch_off_display(void) {
	stop_loop();
	clear_display_on();
	leds_off();
}

void start_loop() {
	power_timer0_enable();

	//
	// Timer 0 einstellen
	//  
	// Modus 14:
	//    FastPWM on compare mode
	//
	//     WGM02   WGM01    WGM00 (fast PWM)
	//     0       1        1
	//
	//
	//  Steuerung des Ausgangsport: None
	//     COM0A1   COM0A0
	//       0        0

	TCCR0A = (0 <<COM0B1) | (0 << COM0B0) | (1<<WGM01) | (0<<WGM00);
	TCCR0B = (0<<WGM02) | PRESCALER;

	//
	//  den Endwert (TOP) für den Zähler setzen
	//  der Zähler zählt bis zu diesem Wert
	OCR0A=TOP;

	//enable interrupt
	TIMSK0 |= _BV(OCIE0A);
}

void stop_loop() {
	TCCR0B &= ~ (_BV(CS02) | _BV(CS01) | _BV(CS00));

	TIMSK0 &= ~ _BV(OCIE0A);

	power_timer0_disable();
}

ISR(TIMER0_COMPA_vect)
{

	//halt the timer to reconfigure	
	GTCCR = _BV(TSM) | _BV(PSRSYNC);

	set_update_display();

	//and reset & run!	
	GTCCR = 0;
}
