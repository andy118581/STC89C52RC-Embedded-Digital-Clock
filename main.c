#include <REGX52.H>
#include "Timer0.h"
#include "LCD1602.h"
#include "Delay.h"
#include "Key.h"
#include "Time_Setting.h"
#include "AT24C02.h"
#include "UART.h"

void Data_update();

unsigned char SettingMode=0; 
unsigned char KeyNum=0;      //遞增或遞減及確認鍵變數

unsigned int min=59;
unsigned int hr=11;
unsigned int sec=50;

unsigned int day=31;
unsigned int mon=12;
unsigned int year=2026;
unsigned int am_pm=1;
unsigned int T0Count=0;


void main()
{	
	LCD_Init();     //LCD初始化
	Time_Load(&hr,&min,&sec,&am_pm,&day,&mon,&year);    //上電即讀取E2PROM時間
	Timer0_Init();  //定時器初始化呼叫
	UART_Init();    //UART初始化
	while(1)
	{	
		if(am_pm%2==0)           //顯示AM PM
		{
			LCD_ShowString(1,1,"AM");
		}else if(am_pm%2==1)
		{
			LCD_ShowString(1,1,"PM");
		}
		LCD_ShowNum(1,9,hr,2);     //顯示小時
		LCD_ShowChar(1,11,':');
		LCD_ShowNum(1,12,min,2);   //顯示分鐘
		LCD_ShowChar(1,14,':');
		LCD_ShowNum(1,15,sec,2);   //顯示秒數
		LCD_ShowNum(2,7,year,4);   //顯示年份
		LCD_ShowChar(2,11,'/');
		LCD_ShowNum(2,12,mon,2);   //顯示月份
		LCD_ShowChar(2,14,'/');
		LCD_ShowNum(2,15,day,2);   //顯示日期
		
		KeyNum=Key_Scan();
		if(KeyNum==4&&SettingMode==0)
		{
			Time_Save(&hr,&min,&sec,&am_pm,&day,&mon,&year);
		}
		
		
		/*調時狀態機*/
		TimeSetting(&SettingMode,KeyNum,&hr,&min,&sec,&am_pm,&day,&mon,&year);
		
		
		if(P3_3==0)
		{
			Delay(30);
			while(P3_3==0);
			Delay(30);
			UART_SendTime(hr,min,sec);
		}
	}
}

void Timer0_RoCount() interrupt 1
{
	TH0=64614/256;
	TL0=64614%256;

	T0Count++;

	if(T0Count>=1000)
	{
		T0Count=0;
		sec++;
	}
		//  秒
	if(sec==60)
	{
		sec=0;
		min++;
	}
		
	//  分
	if(min==60)
	{
		min=0;
		hr++;
		//  時
		if(hr%12==0)
		{
			am_pm=1-am_pm;
			if(am_pm==0)
			{
				day++;
				Data_update();
			}
		}
		if(hr==13)
		{
			hr=1;
		}
	}
}

void Data_update(){
	//  日(大月)
	if(mon==1||mon==3||mon==5||mon==7||mon==8||mon==10||mon==12)
	{
		if(day==32)
		{
			day=1;
			mon++;
		}

	//  日(小月)
	}else if(mon==4||mon==6||mon==9||mon==11)
	{
		if(day==31)
		{
			day=1;
			mon++;
		}
			
	//  日(閏年2月)
	}else if(mon==2&&((year%4==0&&year%100!=0)||(year%100==0&&year%400==0)))
	{
		if(day==30)
		{
			day=1;
			mon++;
		}
		
	//  日(平年2月)
	}else if(mon==2&&!((year%4==0&&year%100!=0)||(year%400==0)))
	{
		if(day==29)
		{
			day=1;
			mon++;
		}
	}
	//  月
	if(mon==13)
	{
		mon=1;
		year++;
	}
}
