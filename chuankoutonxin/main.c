#include "stm32f10x.h"

void TIM2_OC_Init(void);

int main(void)
{
    SystemInit();
    TIM2_OC_Init();
    while(1)
    {
        // 无软件操作，硬件自动翻转PA0(TIM2_CH1)
    }
}

void TIM2_OC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    // 1.开启时钟：GPIOA + TIM2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // PA0 复用推挽输出（TIM2_CH1复用功能！重点）
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 2.定时器时基配置 72M -> 10kHz计数，1s溢出
    TIM_TimeBaseStruct.TIM_Prescaler = 7199;
    TIM_TimeBaseStruct.TIM_Period = 9999;
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

    //3.输出比较配置：匹配时电平翻转 Toggle模式
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Toggle;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 9999;   // 计到该值触发翻转
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStruct);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_Cmd(TIM2, ENABLE);
}