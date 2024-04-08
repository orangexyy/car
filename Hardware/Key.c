#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define Key1_Port  GPIOC
#define Key2_Port  GPIOC
#define Key3_Port  GPIOA

#define Key1_Pin  GPIO_Pin_14
#define Key2_Pin  GPIO_Pin_15
#define Key3_Pin  GPIO_Pin_0

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = Key1_Pin | Key2_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Key1_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = Key3_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if(GPIO_ReadInputDataBit(Key1_Port,Key1_Pin) == 1)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(Key1_Port,Key1_Pin) == 1);
		Delay_ms(20);
		KeyNum = 1;
	}
	if(GPIO_ReadInputDataBit(Key2_Port,Key2_Pin) == 1)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(Key2_Port,Key2_Pin) == 1);
		Delay_ms(20);
		KeyNum = 2;
	}
	if(GPIO_ReadInputDataBit(Key3_Port,Key3_Pin) == 1)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(Key3_Port,Key3_Pin) == 1);
		Delay_ms(20);
		KeyNum = 3;
	}
	return KeyNum;
}
