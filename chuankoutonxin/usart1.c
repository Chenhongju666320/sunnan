#include "usart1.h"

uint8_t *p_tx_str;  //待发送字符串指针
uint16_t tx_index;  //当前发送索引

//串口1初始化，开启TXE发送中断
void USART1_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

	//PA9 TX 复用推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//PA10 RX 浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStruct);
	
	//开启串口发送中断 TXE
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
}

//中断方式发送字符串
void USART1_SendString_IT(uint8_t *str)
{
	p_tx_str = str;
	tx_index = 0;
	//先发送第一个字节，触发TXE中断
	USART_SendData(USART1, p_tx_str[tx_index++]);
}

//串口1中断服务函数
void USART1_IRQHandler(void)
{
	//判断是发送寄存器空中断 TXE
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		if(p_tx_str[tx_index] != '\0')
		{
			USART_SendData(USART1, p_tx_str[tx_index++]);
		}
		else
		{
			//全部发完，关闭TXE中断，防止一直进中断
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}
	}
}