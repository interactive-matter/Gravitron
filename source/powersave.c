#include <stdlib.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "powersave.h"
#include "accel.h"
#include "display.h"
#include "status.h"
#include "leds.h"

//the treshold for movments
#define TRESHOLD 10

//how often to check for movements
//#define WATCHDOG_TIMING WDTO_1S
#define WATCHDOG_TIMING _BV(WDP3)

//how long to wait before going to sleep
#define SLEEP_TIMEOUT 3

struct accel_vect last_position;
uint8_t sleep_wait = 0;

//local helpers
void scan(int8_t pos);
#define SCAN_DELAY 30

void initialize_sleep(void) {
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();

	//prepare the watchog
	WDTCSR = _BV(WDCE) | _BV(WDE);

	//
	// Watchdog einstellen
	//  
	// Modus:
	//    Just enable Watchdog interrupt
	//
	//     WDTON (fuse)   WDE    WDIE
	//     0               1       1 
	//

	WDTCSR =  _BV(WDIE) | _BV(WDE) |  WATCHDOG_TIMING;
	//wdt_enable(WATCHDOG_TIMING);

}

ISR(WDT_vect)
{
	WDTCSR |= _BV(WDIE);
	set_position_check();
}

void check_position() {
	
	struct accel_vect curr_position = read_accel();	
	
	if ((abs(last_position.x-curr_position.x)<TRESHOLD) 
			&& (abs(last_position.y-curr_position.y)<TRESHOLD)) {
		if (!is_sleeping()) { 
			//no movement anymore
			if (sleep_wait==0) {
			}
			if (sleep_wait<SLEEP_TIMEOUT) {
				scan(get_current_position());
				sleep_wait++;
			} else if (sleep_wait==SLEEP_TIMEOUT) {
				light_pattern(0xfff);				
				switch_off_display();			
				delay_ms(SCAN_DELAY);
				//go to sleep
				set_sleeping();
			}
		}
	} else {
		if (is_sleeping()) {
			//wake up
			clear_sleeping();
			switch_on_display();
		}
		sleep_wait=0;		
	}

	last_position=curr_position;	
	clear_position_check();
}

void scan(int8_t pos) {
	int8_t i;
	for (i=0; i<12; i++) {		
		light_led((pos + i) % 12);
		delay_ms(SCAN_DELAY);
	}
	light_led(pos);
	delay_ms(SCAN_DELAY);
	for (i=11; i>=0; i--) {
		light_led((pos +i) % 12);
		delay_ms(SCAN_DELAY);
	}
}
