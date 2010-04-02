#include <stdint.h>
#include <avr/io.h>

#include "adc.h"
#include "accel.h"

#define VOLTAGE_REFERENCE (_BV(REFS0))
#define X_CHANNEL 6
#define Y_CHANNEL 7

#define ACCEL_OFFSET 512.0

struct accel_vect read_accel(void) {
	struct accel_vect result;
	init_measurement(X_CHANNEL, VOLTAGE_REFERENCE);	
	result.x = read_channel(X_CHANNEL, VOLTAGE_REFERENCE) - ACCEL_OFFSET;
	result.y = ACCEL_OFFSET-read_channel(Y_CHANNEL, VOLTAGE_REFERENCE);
	
	return result;
}
