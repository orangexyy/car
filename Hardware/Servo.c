#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Servo_Init(void)
{
	PWM1_Init();
}

void Servo_SetAngle(float Angle)
{
	PWM_SetCompare_S(Angle/180*2000+500);
}

