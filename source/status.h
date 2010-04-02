#ifndef STATUS_H_
#define STATUS_H_

//bitwise status information
extern volatile uint8_t status;

//our bits
#define SLEEP 0
#define UPDATE_DISPLAY 1
#define DISPLAY_ON 2
#define POSITION_CHECK 3

#define is_sleeping() (status & _BV(SLEEP))

#define set_sleeping() status |= _BV(SLEEP);

#define clear_sleeping() status &= ~ _BV(SLEEP);

#define is_update_display() (status & _BV(UPDATE_DISPLAY))

#define set_update_display() status |= _BV(UPDATE_DISPLAY);

#define clear_update_display() status &= ~ _BV(UPDATE_DISPLAY);

#define is_display_on() (status & _BV(DISPLAY_ON))

#define set_display_on() status |= _BV(DISPLAY_ON);

#define clear_display_on() status &= ~ _BV(DISPLAY_ON);

#define is_position_check() (status & _BV(POSITION_CHECK))

#define set_position_check() status |= _BV(POSITION_CHECK);

#define clear_position_check() status &= ~ _BV(POSITION_CHECK);

#endif /*STATUS_H_*/
