#include "stm32f10x.h"                  // Device header

#define Track_Port1  GPIOA
#define Track_Port2  GPIOB

#define Track_L2_Pin  GPIO_Pin_12
#define Track_L1_Pin  GPIO_Pin_13
#define Track_M_Pin  GPIO_Pin_14
#define Track_R1_Pin  GPIO_Pin_15
#define Track_R2_Pin  GPIO_Pin_8

void Track_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Track_R2_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Track_Port1,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Track_L2_Pin | Track_L1_Pin | Track_M_Pin | Track_R1_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Track_Port2,&GPIO_InitStructure);
}

uint8_t Track_L2(void)
{
	if(GPIO_ReadInputDataBit(Track_Port2,Track_L2_Pin) == 1)
	{
		return 1;
	}
	else return 0;
}

uint8_t Track_L1(void)
{
	if(GPIO_ReadInputDataBit(Track_Port2,Track_L1_Pin) == 1)
	{
		return 1;
	}
	else return 0;
}

uint8_t Track_M(void)
{
	if(GPIO_ReadInputDataBit(Track_Port2,Track_M_Pin) == 1)
	{
		return 1;
	}
	else return 0;
}

uint8_t Track_R1(void)
{
	if(GPIO_ReadInputDataBit(Track_Port2,Track_R1_Pin) == 1)
	{
		return 1;
	}
	else return 0;
}

uint8_t Track_R2(void)
{
	if(GPIO_ReadInputDataBit(Track_Port1,Track_R2_Pin) == 1)
	{
		return 1;
	}
	else return 0;
}

