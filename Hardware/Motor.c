#include "stm32f10x.h"                  // Device header
#include "PWM.h"

#define Motor_Port  GPIOA

#define Motor_P_Pin  GPIO_Pin_4
#define Motor_N_Pin  GPIO_Pin_5

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = Motor_P_Pin | Motor_N_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Motor_Port, &GPIO_InitStructure);
}

void Motor_SetSpeed(int8_t Speed)
{
	if(Speed >= 0)   //正转
	{
		GPIO_SetBits(Motor_Port,Motor_P_Pin);
		GPIO_ResetBits(Motor_Port,Motor_N_Pin);
		PWM_SetCompare_M(Speed);
	}
	else             //反转
	{
		GPIO_ResetBits(Motor_Port,Motor_P_Pin);
		GPIO_SetBits(Motor_Port,Motor_N_Pin);
		PWM_SetCompare_M(-Speed);
	}
}

