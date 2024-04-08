#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "stdarg.h"

#define Bluetooth_Port  GPIOA

#define TX_1_Pin  GPIO_Pin_9
#define RX_1_Pin  GPIO_Pin_10

uint32_t Bluetooth_RxData;
uint32_t Bluetooth_RxFlag;

void Bluetooth_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = TX_1_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Bluetooth_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = RX_1_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Bluetooth_Port,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;      //波特率为9600
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	
	USART_Cmd(USART1,ENABLE);
}

void Bluetooth_SendByte(uint8_t Byte)		//发送单个字符
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}


void Bluetooth_SendArray(uint8_t *Array, uint16_t Length)		//发送一个数组
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Bluetooth_SendByte(Array[i]);
	}
}
void Bluetooth_SendString(char *String)		//发送一串字符
{
	uint16_t i;
	for(i=0;String[i] != 0;i++)
	{
		Bluetooth_SendByte(String[i]);
	}
}

uint32_t Bluetooth_Pow(uint32_t X, uint32_t Y)		//次方函数  Result = X的Y次方
{
	uint32_t Result = 1;
	while(Y--)
	{
		Result *= X;
	}
	return Result;
}

void Bluetooth_SendNumber(uint32_t Number, uint8_t Length)		//发送数字
{
	uint16_t i;
	for(i = 0; i < Length ;i++)
	{
		Bluetooth_SendByte(Number / Bluetooth_Pow(10, Length - i -1) % 10 + '0');
	}
}

int fputc(int ch,FILE *f)		//printf函数的底层函数  只能用于一个串口
{
	Bluetooth_SendByte(ch);
	return ch;
}

void Bluetooth_Printf(char *format,...)		//打印可变参数 可移植
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Bluetooth_SendString(String);
}

uint8_t Bluetooth_GetRxFlag(void)		//自定义可查询标志位
{
	if(Bluetooth_RxFlag == 1)
	{
	Bluetooth_RxFlag = 0;
	return 1;
	}
	else
	return 0;
}

uint8_t Bluetooth_ReadRxData(void)		//读取数据
{
	return Bluetooth_RxData;
}

void USART1_IRQHandler(void)		//接受数据，进入中断函数进行数据转存
{
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
	{
		Bluetooth_RxData = USART_ReceiveData(USART1);
		Bluetooth_RxFlag = 1;
		USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);
	}
}
