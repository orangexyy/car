#include "stm32f10x.h"                  // Device header

#define IrDA_Port  GPIOB

#define IrDA_R_Pin  GPIO_Pin_3
#define IrDA_L_Pin  GPIO_Pin_4

uint16_t Ir_Blocked = 0;
uint16_t Ir_Free = 1;

void IrDA_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); /*使能SWD 禁用JTAG*/
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = IrDA_R_Pin | IrDA_L_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IrDA_Port,&GPIO_InitStructure);
}

uint16_t IrDA_R_Getvalue(void)
{
	if(GPIO_ReadInputDataBit(IrDA_Port,IrDA_R_Pin) == Ir_Free)
	{
		return 1;
	}
	else return 0;
}

uint16_t IrDA_L_Getvalue(void)
{
	if(GPIO_ReadInputDataBit(IrDA_Port,IrDA_L_Pin) == Ir_Free)
	{
		return 1;
	}
	else return 0;
}

