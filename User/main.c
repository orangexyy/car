#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "Key.h"
#include "OLED.h"
#include "PWM.h"
#include "Servo.h"
#include "Motor.h"
#include "Track.h"
#include "RGB.h"
#include "Buzzer.h"
#include "Ultrasound.h"
#include "Bluetooth.h"
#include "IrDA.h"
#include "Encoder.h"

uint8_t KeyNum;
uint8_t State;

uint32_t RxData;

float Angle;
float Distance;
int16_t Distance_mm;
int16_t Distance_cm;
int16_t Distance_cm_p;
int16_t Distance_m;
int16_t Distance_m_p;
float Speed;
int16_t Distance_L_mm;
int16_t Distance_R_mm;

uint64_t Numlen(uint64_t num)//计算数字的长度
{
    uint64_t len = 1;        // 初始长度为1
    for(; num > 9; ++len)    // 判断num是否大于9，否则长度+1
        num /= 10;	         // 使用除法进行运算，直到num小于1
    return len;              // 返回长度的值
}

int main(void)
{
	Timer1_Init();
	Timer2_Init();
	Key_Init();
	OLED_Init();
	PWM1_Init();
	PWM2_Init();
	Servo_Init();
	Motor_Init();
	Track_Init();
	RGB_Init();
	Buzzer_Init();
	Ultrasound_Init();
	Bluetooth_Init();
	IrDA_Init();
	Encoder_Init();
	
	

	OLED_ShowString(1,1,"WELCOME!        ");
	OLED_ShowString(2,1,"CXY's Car 1.0   ");
	OLED_ShowString(3,1,"CHOOSE THE MODE!");
	OLED_ShowString(4,1,"MODE:           ");

	R_OFF();
	G_OFF();
	B_OFF();
	
	while(1)
	{	
		Distance_mm=Ultrasound_mm();			//获取距离测量结果，单位毫米（mm）		
		Distance = Distance_mm;
//按键模式选择		
		KeyNum = Key_GetNum();	
		if(KeyNum == 1)					
		{
			State = 1;
			OLED_Clear();
		}
		if(KeyNum == 2)
		{
			State = 2;	
			OLED_Clear();
		}
		if(KeyNum == 3)
		{
			State = 3;
			OLED_Clear();			
		}
		
		if(Bluetooth_GetRxFlag() == SET)		//蓝牙数据接收标志
		{
			RxData = Bluetooth_ReadRxData();
			Bluetooth_SendByte(RxData);
		}
//串口模式选择
		if(RxData == 0x91)				
		{
			State = 1;
			OLED_Clear();
			RxData = 0;
		}
		if(RxData == 0x92)
		{
			State = 2;
			OLED_Clear();
			RxData = 0;
		}
		if(RxData == 0x93)
		{
			State = 3;
			OLED_Clear();
			RxData = 0;
		}
		
//模式1		
		if(State == 1)
		{	
			R_ON();
			G_OFF();
			B_OFF();
			Buzzer_Init();
			OLED_ShowNum(1,12,State,1);
			OLED_ShowString(1,1,"Speed:");
			OLED_ShowString(2,1,"Towards:");
			OLED_ShowString(3,1,"Angle:");
			OLED_ShowString(4,1,"Distance:");
			
			OLED_Clear_Part(4,10,16);			//将OLDE屏第4行部分清屏
			OLED_ShowNum(4, 10,Distance_mm, Numlen(Distance_mm));		//显示单位为毫米的距离结果
			OLED_ShowString(4, 10 + Numlen(Distance_mm), "mm");
			Delay_ms(100);	
			
			Bluetooth_SendNumber(Distance_mm,Numlen(Distance_cm));
			Bluetooth_SendString("mm\r\n");
			
			if(Bluetooth_GetRxFlag() == SET)			//蓝牙数据接收标志
			{
				RxData = Bluetooth_ReadRxData();
				Bluetooth_SendByte(RxData);
			}
//串口模式切换			
			if(RxData == 0x91)
			{
				State = 1;
				OLED_Clear();
				RxData = 0;
			}
			if(RxData == 0x92)
			{
				State = 2;
				OLED_Clear();
				RxData = 0;
			}
			if(RxData == 0x93)
			{
				State = 3;
				OLED_Clear();
				RxData = 0;
			}
			//停车复位
			if(RxData == 0x22)
			{
				R_OFF();
				Angle = 90;
				Speed = 0;
				Servo_SetAngle(Angle);
				Motor_SetSpeed(Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Stop   ");
				OLED_ShowNum(3,7,Angle-90,2);
				RxData = 0;
			}
			//停车
			if(RxData == 0x01)
			{
				R_OFF();
				Angle = 90;
				Speed = 0;
				Servo_SetAngle(Angle);
				Motor_SetSpeed(Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Stop   ");
				RxData = 0;
			}
			//车轮复位
			if(RxData == 0x02)
			{
//				if(Angle < 90 && Angle >= 0)
//				{
//					Angle += 15;
//					OLED_ShowNum(3,7,90-Angle,2);
//				}
//				if(Angle <= 180 && Angle > 90)
//				{
//					Angle -= 15;
//					OLED_ShowNum(3,7,Angle-90,2);
//				}
				R_OFF();
				Angle = 90;
				Servo_SetAngle(Angle);
				OLED_ShowString(2,9,"Forward");
				RxData = 0;
			}
			//前油门
			if(RxData == 0x03)
			{
				R_OFF();
				Speed += 2;
				if(Speed >= 100)
				{
					Speed = 100;
				}
				Motor_SetSpeed(Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Forward");
				RxData = 0;
			}
			//前刹车
			if(RxData == 0x04)
			{
				R_OFF();
				Speed -= 6;
				if(Speed <= 30)
				{
					Speed = 30;
				}
				Motor_SetSpeed(Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Forward");
				RxData = 0;
			}
			//后油门
			if(RxData == 0x05)
			{
				R_OFF();
				Speed += 2;
				if(Speed >= 100)
				{
					Speed = 100;
				}
				Motor_SetSpeed(-Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Back   ");
				RxData = 0;
			}
			//后刹车
			if(RxData == 0x06)
			{
				R_OFF();
				Speed -= 6;
				if(Speed <= 30)
				{
					Speed = 30;
				}
				Motor_SetSpeed(-Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Back   ");
				RxData = 0;
			}
			//前轮左转
			if(RxData == 0x07)
			{
				Angle -= 15;
				if(Angle <= 0)
				{
					Angle = 0;
				}
				Servo_SetAngle(Angle);
				R_ON();
				OLED_ShowString(2,9,"Left   ");
				OLED_ShowNum(3,7,90-Angle,2);
				RxData = 0;
			}
			//前轮右转
			if(RxData == 0x08)
			{
				Angle += 15;
				if(Angle >= 180)
				{
					Angle = 180;
				}
				Servo_SetAngle(Angle);
				R_ON();
				OLED_ShowString(2,9,"Right  ");
				OLED_ShowNum(3,7,Angle-90,2);
				RxData = 0;
			}
			//定速前进
			if(RxData == 0x09)
			{
				R_OFF();
				Angle = 90;
				Speed = 40;
				Servo_SetAngle(Angle);
				Motor_SetSpeed(Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Forward");
				OLED_ShowNum(3,7,Angle-90,2);
				RxData = 0;
			}
			//定速后退
			if(RxData == 0x10)
			{
				R_OFF();
				Angle = 90;
				Speed = 40;
				Servo_SetAngle(Angle);
				Motor_SetSpeed(-Speed);
				OLED_ShowNum(1,7,-Speed,3);
				OLED_ShowString(2,9,"Back   ");
				OLED_ShowNum(3,7,Angle-90,2);
				RxData = 0;
			}
			//定速前进 前轮左转
			if(RxData == 0x11)
			{
				Angle -= 15;
				if(Angle <= 0)
				{
					Angle = 0;
				}
				Speed = 40;
				Motor_SetSpeed(Speed);
				Servo_SetAngle(Angle);
				R_ON();
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Left   ");
				OLED_ShowNum(3,7,90-Angle,2);
				RxData = 0;
			}
			//定速前进 前轮右转
			if(RxData == 0x12)
			{
				Angle += 15;
				if(Angle >= 180)
				{
					Angle = 180;
				}
				Speed = 40;
				Motor_SetSpeed(Speed);
				Servo_SetAngle(Angle);
				R_ON();
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Right  ");
				OLED_ShowNum(3,7,Angle-90,2);
				RxData = 0;
			}
			//定速后退 前轮左转
			if(RxData == 0x13)
			{
				Angle -= 15;
				if(Angle <= 0)
				{
					Angle = 0;
				}
				Speed = 40;
				Motor_SetSpeed(-Speed);
				Servo_SetAngle(Angle);
				R_ON();
				OLED_ShowNum(1,7,-Speed,3);
				OLED_ShowString(2,9,"Left   ");
				OLED_ShowNum(3,7,90-Angle,2);
				RxData = 0;
			}
			//定速后退 前轮右转
			if(RxData == 0x14)
			{
				Angle += 15;
				if(Angle >= 180)
				{
					Angle = 180;
				}
				Speed = 40;
				Motor_SetSpeed(-Speed);
				Servo_SetAngle(Angle);
				R_ON();
				OLED_ShowNum(1,7,-Speed,3);
				OLED_ShowString(2,9,"Right  ");
				OLED_ShowNum(3,7,Angle-90,2);
				RxData = 0;
			}
		}
//模式2
		if(State == 2)
		{
			R_OFF();
			G_ON();
			B_OFF();
			OLED_ShowNum(1,12,State,1);
			OLED_ShowString(1,1,"Speed:");
			OLED_ShowString(2,1,"Left:");
			OLED_ShowString(3,1,"Right:");
			OLED_ShowString(4,1,"Distance:");
			
			OLED_Clear_Part(4,10,16);			//将OLDE屏第4行部分清屏
			OLED_ShowNum(4, 10,Distance_mm, Numlen(Distance_mm));		//显示单位为毫米的距离结果
			OLED_ShowString(4, 10 + Numlen(Distance_mm), "mm");
			Delay_ms(100);	
			
			Bluetooth_SendNumber(Distance_mm,Numlen(Distance_cm));
			Bluetooth_SendString("mm\r\n");
			
			if(Bluetooth_GetRxFlag() == SET)			//蓝牙数据接收标志
			{
				RxData = Bluetooth_ReadRxData();
				Bluetooth_SendByte(RxData);
			}
//串口模式切换			
			if(RxData == 0x91)
			{
				State = 1;
				OLED_Clear();
				RxData = 0;
			}
			if(RxData == 0x92)
			{
				State = 2;
				OLED_Clear();
				RxData = 0;
			}
			if(RxData == 0x93)
			{
				State = 3;
				OLED_Clear();
				RxData = 0;
			}
			
			if(Distance > 200)
			{
				if(IrDA_R_Getvalue()==1 && IrDA_L_Getvalue()==1)
				{
					G_ON();
					Buzzer_Level2();
					Angle = 90;
					Speed = 35;
					Servo_SetAngle(Angle);
					Motor_SetSpeed(Speed);
					OLED_ShowString(2,6,"FREE      ");
					OLED_ShowString(3,7,"FREE     ");
				}
				if(IrDA_R_Getvalue()==1 && IrDA_L_Getvalue()==0)
				{
					G_ON();
					Buzzer_Level2();
					Angle = 180;
					Speed = 30;
					Servo_SetAngle(Angle);
					Motor_SetSpeed(Speed);
					OLED_ShowString(2,6,"FREE      ");
					OLED_ShowString(3,7,"BLOCKED  ");
				}
				if(IrDA_R_Getvalue()==0 && IrDA_L_Getvalue()==1)
				{
					G_ON();
					Buzzer_Level2();
					Angle = 0;
					Speed = 30;
					Servo_SetAngle(Angle);
					Motor_SetSpeed(Speed);
					OLED_ShowString(2,6,"BLOCKED   ");
					OLED_ShowString(3,7,"FREE     ");
				}
//				if(IrDA_R_Getvalue()==0 && IrDA_L_Getvalue()==0)
//				{
//					G_ON();
//					Buzzer_Level2();
//					Angle = 90;
//					Speed = 25;
//					Servo_SetAngle(Angle);
//					Motor_SetSpeed(-Speed);
//					OLED_ShowString(2,6,"BLOCKED   ");
//					OLED_ShowString(3,7,"BLOCKED  ");
//				}
			}
			if(Distance > 40 && Distance <= 200)
			{
				if(IrDA_R_Getvalue()==1 && IrDA_L_Getvalue()==1)
				{
					G_ON();
					Buzzer_Level2();
					Angle = 90;
					Speed = 30;
					Servo_SetAngle(Angle);
					Motor_SetSpeed(Speed);
					OLED_ShowString(2,6,"FREE      ");
					OLED_ShowString(3,7,"FREE     ");
				}
				if(IrDA_R_Getvalue()==1 && IrDA_L_Getvalue()==0)
				{
					G_ON();
					Buzzer_Level2();
					Angle = 180;
					Speed = 30;
					Servo_SetAngle(Angle);
					Motor_SetSpeed(Speed);
					OLED_ShowString(2,6,"FREE      ");
					OLED_ShowString(3,7,"BLOCKED  ");
				}
				if(IrDA_R_Getvalue()==0 && IrDA_L_Getvalue()==1)
				{
					G_ON();
					Buzzer_Level2();
					Angle = 0;
					Speed = 30;
					Servo_SetAngle(Angle);
					Motor_SetSpeed(Speed);
					OLED_ShowString(2,6,"BLOCKED   ");
					OLED_ShowString(3,7,"FREE     ");
				}
				if(IrDA_R_Getvalue()==0 && IrDA_L_Getvalue()==0)
				{
					G_ON();
					Buzzer_Level2();
					Angle = 90;
					Speed = 30;
					Servo_SetAngle(Angle);
					Motor_SetSpeed(-Speed);
					OLED_ShowString(2,6,"BLOCKED   ");
					OLED_ShowString(3,7,"BLOCKED  ");
				}
			}
			if(Distance <= 40)
			{
				G_ON();
				Delay_ms(200);
				G_OFF();
				Buzzer_Level1();
				Angle = 90;
				Speed = 30;
				Servo_SetAngle(Angle);
				Motor_SetSpeed(-Speed);	
			}
		}
//模式3		
		if(State == 3)
		{
			R_OFF();
			G_OFF();
			B_ON();
			Buzzer_Init();
			OLED_ShowNum(1,12,State,1);
			OLED_ShowString(1,1,"Speed:");
			OLED_ShowString(2,1,"Towards:");
			OLED_ShowString(3,1,"Angle:");
			OLED_ShowString(4,1,"Distance:");
			
			OLED_Clear_Part(4,10,16);			//将OLDE屏第4行部分清屏
			OLED_ShowNum(4, 10,Distance_mm, Numlen(Distance_mm));		//显示单位为毫米的距离结果
			OLED_ShowString(4, 10 + Numlen(Distance_mm), "mm");
			Delay_ms(100);	
			
			Bluetooth_SendNumber(Distance_mm,Numlen(Distance_cm));
			Bluetooth_SendString("mm\r\n");
			
			if(Bluetooth_GetRxFlag() == SET)				//蓝牙数据接收标志
			{
				RxData = Bluetooth_ReadRxData();
				Bluetooth_SendByte(RxData);
			}
//串口模式切换
			if(RxData == 0x91)
			{
				State = 1;
				OLED_Clear();
				RxData = 0;
			}
			if(RxData == 0x92)
			{
				State = 2;
				OLED_Clear();
				RxData = 0;
			}
			if(RxData == 0x93)
			{
				State = 3;
				OLED_Clear();
				RxData = 0;
			}
			//中间有/没有黑线 左右没有黑线 前进
			if((Track_L2()==0)&&(Track_L1()==0)&&(Track_M()==1 || Track_M()==0)&&(Track_R1()==0)&&(Track_R2()==0))
			{
				R_ON();
				G_OFF();
				B_OFF();
				Angle = 90;
				Speed = 30;
				Servo_SetAngle(Angle);
				Motor_SetSpeed(Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Forward");
				OLED_ShowNum(3,7,Angle-90,2);
			}
			//中间有黑线 左右有黑线 前进
			if((Track_L2()==1 || Track_L1()==1)&&(Track_M()==1)&&(Track_R1()==1 || Track_R2()==1))
			{
				R_ON();
				G_OFF();
				B_OFF();
				Angle = 90;
				Speed = 30;
				Servo_SetAngle(Angle);
				Motor_SetSpeed(Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Forward");
				OLED_ShowNum(3,7,Angle-90,2);
			}
			//中间没有黑线 左右有黑线 左转90度
			if((Track_L2()==1 || Track_L1()==1)&&(Track_M()==0)&&(Track_R1()==1 || Track_R2()==1))
			{
				R_OFF();
				G_ON();
				B_OFF();
				Angle = 0;
				Speed = 30;
				Servo_SetAngle(Angle);
				Motor_SetSpeed(Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Left   ");
				OLED_ShowNum(3,7,90-Angle,2);
			}
			//中间有/没有黑线 左边1有黑线 右边没黑线 左转30
			if((Track_L2()==0) && (Track_L1()==1) && (Track_M()==1 || Track_M()==0) && (Track_R1()==0) && (Track_R2()==0))
			{
				R_OFF();
				G_ON();
				B_OFF();
				Angle = 60;
				Speed = 30;
				Servo_SetAngle(Angle);
				Motor_SetSpeed(Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Left   ");
				OLED_ShowNum(3,7,90-Angle,2);
			}
			//中间有/没有黑线 左边2有黑线 右边没黑线 左转60  
			if((Track_L2()==1) && (Track_M()==1 || Track_M()==0) && (Track_R1()==0) && (Track_R2()==0))
			{
				R_OFF();
				G_ON();
				B_OFF();
				Angle = 30;
				Speed = 30;
				Servo_SetAngle(Angle);
				Motor_SetSpeed(Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Left   ");
				OLED_ShowNum(3,7,90-Angle,2);
			}
			//中间有/没有黑线 右边1有黑线 左边没黑线 右转30
			if((Track_L2()==0) && (Track_L1()==0) && (Track_M()==1 || Track_M()==0) && (Track_R1()==1) && (Track_R2()==0))
			{
				R_OFF();
				G_OFF();
				B_ON();
				Angle = 120;
				Speed = 30;
				Servo_SetAngle(Angle);
				Motor_SetSpeed(Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Right  ");
				OLED_ShowNum(3,7,Angle-90,2);
			}
			//中间有/没有黑线 右边2有黑线 左边没黑线 右转60  
			if((Track_L2()==0) && (Track_L1()==0) && (Track_M()==1 || Track_M()==0) && (Track_R2()==1))
			{
				R_OFF();
				G_OFF();
				B_ON();
				Angle = 150;
				Speed = 30;
				Servo_SetAngle(Angle);
				Motor_SetSpeed(Speed);
				OLED_ShowNum(1,7,Speed,3);
				OLED_ShowString(2,9,"Right  ");
				OLED_ShowNum(3,7,Angle-90,2);
			}
	
			
			
		}
		

		
		
		

	}
}



			
