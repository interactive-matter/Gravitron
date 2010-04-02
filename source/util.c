#include <inttypes.h>
#include <util/delay.h>


void delay_ms(uint8_t ms) {
	uint8_t wait = ms;
	while (wait) {
		_delay_us(980);
		wait--;
	}
}
