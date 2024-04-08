#include "stm32f10x.h"                  // Device header

#define PWM1_Port GPIOA
#define PWM2_Port GPIOB

#define PWM1_Pin GPIO_Pin_1
#define PWM2_Pin GPIO_Pin_0

void PWM1_Init(void)					//舵机
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin  = PWM1_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PWM1_Port, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure1;
	TIM_TimeBaseInitStructure1.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure1.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure1.TIM_Period = 20000 - 1;         //ARR
	TIM_TimeBaseInitStructure1.TIM_Prescaler = 72- 1;        //PSC
	TIM_TimeBaseInitStructure1.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure1);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;                     //CCR
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);

	TIM_Cmd(TIM2,ENABLE);
}


void PWM2_Init(void)					//电机
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin  = PWM2_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PWM2_Port, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;         //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36- 1;        //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;                     //CCR
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);

	TIM_Cmd(TIM3,ENABLE);
}


void PWM_SetCompare_S(uint16_t Compare)		//舵机
{
	TIM_SetCompare2(TIM2,Compare);
}

void PWM_SetCompare_M(uint16_t Compare)			//电机
{
	TIM_SetCompare3(TIM3,Compare);
}
