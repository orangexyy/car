#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define RGB_Port1  GPIOA
#define RGB_Port2  GPIOB

#define R_Pin  GPIO_Pin_11
#define G_Pin  GPIO_Pin_12
#define B_Pin  GPIO_Pin_5

void RGB_Init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = R_Pin | G_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RGB_Port1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = B_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RGB_Port2, &GPIO_InitStructure);
	
	GPIO_SetBits(RGB_Port1,R_Pin | G_Pin);
	GPIO_SetBits(RGB_Port2,B_Pin);
}

void R_ON(void)
{
	GPIO_SetBits(RGB_Port1 ,R_Pin);
}

void R_OFF(void)
{
	GPIO_ResetBits(RGB_Port1 ,R_Pin);
}

void G_ON(void)
{
	GPIO_SetBits(RGB_Port1 ,G_Pin);
}

void G_OFF(void)
{
	GPIO_ResetBits(RGB_Port1 ,G_Pin);
}

void B_ON(void)
{
	GPIO_SetBits(RGB_Port2 ,B_Pin);
}

void B_OFF(void)
{
	GPIO_ResetBits(RGB_Port2 ,B_Pin);
}

