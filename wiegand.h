#ifndef WIEGANT_H
#define WIEGANT_H

#define WIEGAND_PERIOD 		2000
#define WIEGAND_BIT_TIME 	200

#define WIEGAND_TIMEOUT_MS		50

void WiegandInit(TIM_HandleTypeDef *htim);

/*TX*/
void WiegandProcessTx(void);
void WiegandTransmit(uint32_t data, uint32_t bits);

/*RX*/
void WiegandSaveBit(uint32_t bit);
void WiegandTimeout();

#endif
