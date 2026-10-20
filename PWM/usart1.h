#ifndef __USART1_H
#define __USART1_H

#include "stm32f10x.h"

void USART1_Init(uint32_t baudrate);
void USART1_SendByte(uint8_t dat);
void USART1_SendString(uint8_t *str);
void USART1_SendString_IT(uint8_t *str);
#endif