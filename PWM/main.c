#include "stm32f10x.h"

void TIM2_PWM_Init(void);
void Delay_ms(uint32_t ms);

int main(void)
{
    uint16_t ccr_val = 0;
    int8_t dir = 1;    // 1:占空比增加，-1:占空比减小

    SystemInit();
    TIM2_PWM_Init();

    while(1)
    {
        TIM_SetCompare1(TIM2, ccr_val);  // 修改TIM2 CH1的CCR，改变占空比

        ccr_val += dir;
        if(ccr_val >= 199)
        {
            dir = -1;
        }
        else if(ccr_val <= 0)
        {
            dir = 1;
        }
        Delay_ms(8);   // 调整这个值修改呼吸快慢，8ms 总呼吸周期约1.6s
    }
}

// TIM2_CH1 PA0 PWM初始化
void TIM2_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // PA0 复用推挽输出 TIM2_CH1
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 时基
    TIM_TimeBaseStruct.TIM_Prescaler = 71;
    TIM_TimeBaseStruct.TIM_Period = 199;
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

    // PWM模式1：CNT<CCR时输出有效电平
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 0;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStruct);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

// 简单软件延时
void Delay_ms(uint32_t ms)
{
    uint32_t i,j;
    for(i=0; i<ms; i++)
        for(j=0; j<7200; j++);
}