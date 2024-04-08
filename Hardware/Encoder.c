#include "stm32f10x.h"                  // Device header
#include "Timer.h"

#define Encoder_Port  GPIOB

#define Encoder_A_Pin  GPIO_Pin_6
#define Encoder_B_Pin  GPIO_Pin_7

extern int16_t Speed;

void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin  = Encoder_A_Pin | Encoder_B_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Encoder_Port, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure2;
	TIM_TimeBaseInitStructure2.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure2.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure2.TIM_Period = 65535-1;      //ARR
	TIM_TimeBaseInitStructure2.TIM_Prescaler = 1 - 1;        //PSC
	TIM_TimeBaseInitStructure2.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure2);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM4,ENABLE);
}


int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4,0);
	return Temp;
}



