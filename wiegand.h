#ifndef WIEGANT_H
#define WIEGANT_H

#define WIEGAND_PERIOD 		2000
#define WIEGAND_BIT_TIME 	200

void wiegandProcess(void);
void wiegandTransmit(uint32_t data, uint32_t bits);
void wiegandInit(TIM_HandleTypeDef *htim);


#endif
