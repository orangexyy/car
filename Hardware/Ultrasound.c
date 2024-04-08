#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"

#define Ultrasound_Port  GPIOA

#define Trig_Pin GPIO_Pin_6
#define Echo_Pin GPIO_Pin_7

uint32_t Time;
uint32_t Time_end;

void Ultrasound_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Trig_Pin;				//TRIG
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Ultrasound_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = Echo_Pin;				//ECHO
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Ultrasound_Port,&GPIO_InitStructure);
	GPIO_ResetBits(Ultrasound_Port,Trig_Pin);
	Delay_us(15);
}

uint16_t Ultrasound_mm(void)
{
	uint32_t Distance = 0;
	uint32_t Distance_mm = 0;
	
	GPIO_SetBits(Ultrasound_Port,Trig_Pin);
	Delay_us(15);
	GPIO_ResetBits(Ultrasound_Port,Trig_Pin);
	while(GPIO_ReadInputDataBit(Ultrasound_Port,Echo_Pin) == 0);
	Time = 0;
	while(GPIO_ReadInputDataBit(Ultrasound_Port,Echo_Pin) == 1);
	Time_end = Time;
	
	if(Time_end/100<38)
	{
		Distance = (Time_end*346)/2;
		Distance_mm  = Distance/100;
	}
	return Distance_mm;
}

float  Ultrasound_m(void)
{
	uint32_t Distance = 0;
	uint32_t Distance_mm = 0;
	float Distance_m = 0;
	
	GPIO_SetBits(Ultrasound_Port,Trig_Pin);
	Delay_us(20);
	GPIO_ResetBits(Ultrasound_Port,Trig_Pin);
	while(GPIO_ReadInputDataBit(Ultrasound_Port,Echo_Pin) == 0);
	Time = 0;
	while(GPIO_ReadInputDataBit(Ultrasound_Port,Echo_Pin) == 1);
	Time_end = Time;
	
	if(Time_end/100<38)					//判断echo响应是否超时   若无判断的话会陷入死循环
	{
		Distance = (Time_end*346)/2;
		Distance_mm  = Distance/100;
		Distance_m = Distance_mm/1000;
	}
	return Distance_m;
}

/*
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == 1)
		{
			Time++;
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}
*/


