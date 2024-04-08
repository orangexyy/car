#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define Buzzer_Pin  GPIO_Pin_15
#define Buzzer_Port  GPIOA

void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO ,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); /*使能SWD 禁用JTAG*/
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Buzzer_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Buzzer_Port ,&GPIO_InitStructure);
	
	GPIO_SetBits(Buzzer_Port,Buzzer_Pin);
}


void Buzzer_Level1(void)
{
	GPIO_SetBits(Buzzer_Port,Buzzer_Pin);
	Delay_ms(80);
	GPIO_ResetBits(Buzzer_Port,Buzzer_Pin);
	Delay_ms(20);
}

void Buzzer_Level2(void)
{
	GPIO_SetBits(Buzzer_Port,Buzzer_Pin);
	Delay_ms(400);
	GPIO_ResetBits(Buzzer_Port,Buzzer_Pin);
	Delay_ms(50);
}

void Buzzer_Level3(void)
{
	GPIO_SetBits(Buzzer_Port,Buzzer_Pin);
	Delay_ms(500);
	GPIO_ResetBits(Buzzer_Port,Buzzer_Pin);
	Delay_ms(50);
}


