#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "status.h"
#include "util.h"
#include "display.h"
#include "accel.h"
#include "powersave.h"


//global status
volatile uint8_t status;

int main(void) {

	//reset watchdog first
	MCUSR = 0;
	wdt_disable();
	wdt_reset();

	//DDRC connected to accel
	//DDRC = ( _BV(ACCEL_VCC) | _BV(ACCEL_GND) | (_BV(ACCEL_TEST)));
	//PORTC = (_BV(ACCEL_VCC)); //enable it
	
	//set data directions
	DDRC = 15; //lower 6 outputs
	DDRD = 255; //lower 6 outputs

	//disable all leds	
	PORTB = 0;
	PORTD = 0;

	sei();
	init_display();
	initialize_sleep();

	while (1) {
		//main loop
		cli();
		if (is_position_check()) {
			wdt_reset();
			check_position();
		}
		if (is_update_display()) {
			update_display();
		}
		sei();
		if (is_sleeping()) {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_cpu();
		}
	}
}
