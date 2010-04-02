#ifndef ACCEL_H_
#define ACCEL_H_

#define ACCEL_VCC 0
#define ACCEL_GND 1

#define ACCEL_X 2
#define ACCEL_Y 3

#define ACCEL_TEST 5

struct accel_vect {
	int16_t x;
	int16_t y;
};

struct accel_vect read_accel(void);

#endif /*ACCEL_H_*/
