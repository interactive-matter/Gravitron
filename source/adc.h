#ifndef ADC_H_
#define ADC_H_

void init_measurement(uint8_t mux, uint8_t voltageReference);
int16_t read_channel(uint8_t mux, uint8_t voltageReference);
void finish_measurement(void);

#endif /*ADC_H_*/
