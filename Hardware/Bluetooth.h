#ifndef __BLUETOOTH_
#define __BLUETOOTH_

#include<stdio.h>

void Bluetooth_Init(void);
void Bluetooth_SendByte(uint8_t Byte);
void Bluetooth_SendArray(uint8_t *Array, uint16_t Length);
void Bluetooth_SendString(uint8_t *String);
void Bluetooth_SendNumber(uint32_t Number, uint8_t Length);
void Bluetooth_Printf(char *format,...);
uint8_t Bluetooth_ReadRxData(void);
uint8_t Bluetooth_GetRxFlag(void);



#endif
