#include <stdint.h>
#include <stdio.h>
#include "main.h"

#include "wiegand.h"


enum GTXState {IDLE_TX, TRANSMIT} txState;

enum BState {START_TX_BIT, END_TX_BIT} bitState;

enum RXState {IDLE_RX, RECEIVE, RECEIVED} rxState;

TIM_HandleTypeDef *wiegandTim = NULL;//таймер, используемый для формирования времен

/*TX*/
static uint32_t dataTx = 0;

static uint32_t bitsTx = 0; //сколько бит передавать

volatile static int32_t curBit = 0;//номер текущего бита

/*RX*/
static uint32_t bitCnt = 0;//счетчик принятых бит

static uint32_t dataRx = 0;

/*RX*/
void StartTimeout();

/*TX and RX*/
void WiegandInit(TIM_HandleTypeDef *htim)
{
	wiegandTim = htim;
}

/*TX*/
void WiegandTransmit(uint32_t data, uint32_t bits)
{
	txState = TRANSMIT;
	bitState = START_TX_BIT;
	dataTx = data;
	bitsTx = bits;
	curBit = bits-1;
	//Запускаем таймер
	wiegandTim->Instance->ARR = WIEGAND_BIT_TIME;
	HAL_TIM_Base_Start_IT(wiegandTim);
}

uint32_t GetBit()
{
	if ((dataTx & (1 << (curBit--)))!=0) return 1;
	else return 0;
}

void WiegandProcessTx()
{
	switch (txState)
	{
		case IDLE_TX: break;
		case TRANSMIT:
		{
			switch (bitState)
			{
				case START_TX_BIT:
				{
					//если был передан последний бит завершаем передачу
					if (curBit < 0)
					{
						txState = IDLE_TX;
						HAL_TIM_Base_Stop_IT(wiegandTim);
						break;
					}

					if (GetBit()!=0)
					{
						HAL_GPIO_WritePin(WIEGAND_D1_GPIO_Port, WIEGAND_D1_Pin, GPIO_PIN_RESET);
					}
					else
					{
						HAL_GPIO_WritePin(WIEGAND_D0_GPIO_Port, WIEGAND_D0_Pin, GPIO_PIN_RESET);
					}

					bitState = END_TX_BIT;

					//Запускаем таймер на время передачи бита
					wiegandTim->Instance->ARR = WIEGAND_BIT_TIME - 1;

					break;
				}

				case END_TX_BIT:
				{
					//Без разбора устанавливаем обе линии в 1
					HAL_GPIO_WritePin(WIEGAND_D0_GPIO_Port, WIEGAND_D0_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(WIEGAND_D1_GPIO_Port, WIEGAND_D1_Pin, GPIO_PIN_SET);

					bitState = START_TX_BIT;

					//Запускаем таймер на период минус время передачи бита
					wiegandTim->Instance->ARR = WIEGAND_PERIOD - WIEGAND_BIT_TIME - 1;
					break;
				}

				default:break;
			}
			break;
		}
		default: break;
	}
}

/*RX*/
void WiegandSaveBit(uint32_t bit)
{
	if (rxState == IDLE_RX)
	{
		rxState = RECEIVE;
		bitCnt = 0;
	}
	dataRx = (dataRx << 1) | bit;
	bitCnt++;
	StartTimeout();
}

void StartTimeout()
{
	wiegandTim->Instance->ARR = WIEGAND_TIMEOUT_MS*10;
	wiegandTim->Instance->CNT = 0;
	HAL_TIM_Base_Start_IT(wiegandTim);
}


void WiegandTimeout()
{
	HAL_TIM_Base_Stop_IT(wiegandTim);
	rxState = RECEIVED;
	//Вывод/сохранение
	printf("Received %d bits, word = 0x%X\n", (int)bitCnt, (int)dataRx);
	rxState = IDLE_RX;
	dataRx = 0;
}



