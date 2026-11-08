#include <REGX52.H>
#include "I2C.h"
#include "Delay.h"
#include "LCD1602.h"
#include "Time_Setting.h"

#define AT24C02_ADDRESS     0xA0 
//定義從機地址,前4位為1010 後3位為000,最後1位寫為0讀為1
//	ex:1010 0000==0xA0
//	ex:1010 0001==0xA1

/*
定義寫數據及讀數據之函式
*/



void AT24C02_WriteByte(unsigned char WordAddress,unsigned char Data) //寫數據之函式 
//WordAddress為字地址,Data為要寫入之數據
{
	I2C_Start();                   //依時序圖呼叫起始條件
	I2C_SendByte(AT24C02_ADDRESS); //寫從機地址+寫,呼叫Byte Write
	I2C_ReceiveAck();              //主機接收從機應答,呼叫ACK Detection
	I2C_SendByte(WordAddress);     //寫字地址,呼叫Byte Write
	I2C_ReceiveAck();              //主機接收從機應答,呼叫ACK Detection
	I2C_SendByte(Data);            //寫要在字地址存的數據,呼叫Byte Write
	I2C_ReceiveAck();	           //主機接收從機應答,呼叫ACK Detection
	I2C_Stop();                    //呼叫終止條件
	Delay(5);
}



unsigned char AT24C02_ReadByte(unsigned char WordAddress) //讀數據之函式
//WordAddress為字地址
{
	unsigned char Data;
	I2C_Start();                   //依時序圖呼叫起始條件
	I2C_SendByte(AT24C02_ADDRESS); //寫從機地址+寫,呼叫Byte Write
	I2C_ReceiveAck();              //主機接收從機應答,呼叫ACK Detection
	I2C_SendByte(WordAddress);     //寫字地址,呼叫Byte Write
	I2C_ReceiveAck();              //主機接收從機應答,呼叫ACK Detection	
	I2C_Start();                   //依時序圖呼叫起始條件
	I2C_SendByte(AT24C02_ADDRESS|0x01); //寫從機地址+讀,呼叫Byte Write
	//AT24C02_ADDRESS|0x01就能讓10100000變成10100001
	I2C_ReceiveAck();              //主機接收從機應答,呼叫ACK Detection	
	Data=I2C_ReceiveByte();        //從機獲取總線,將從機發的資料指定給Data
	//Data=I2C_ReceiveByte()就相當於讓主機讀數據了
	I2C_SendAck(1);                //主機發送應答,0為應答1為非應答
	I2C_Stop();                    //呼叫終止條件
	return Data;
}


void Time_Save(unsigned int *hr,unsigned int *min,unsigned int *sec,unsigned int *am_pm,unsigned int *day,unsigned int *mon,unsigned int *year)
{
	unsigned char TimerWasRunning;
	TimerWasRunning=TR0;			//紀錄T0原本是否運作
	TR0=0; 							//暫停計時
	/*寫入時間*/
	AT24C02_WriteByte(0,0);
	AT24C02_WriteByte(1,(unsigned char)*hr);
	AT24C02_WriteByte(2,(unsigned char)*min);
	AT24C02_WriteByte(3,(unsigned char)*sec);
	AT24C02_WriteByte(4,(unsigned char)*am_pm);
	/*寫入日期*/
	AT24C02_WriteByte(5,(unsigned char)*day);
	AT24C02_WriteByte(6,(unsigned char)*mon);
	/*寫入年份(拆成高低位)*/
	AT24C02_WriteByte(7,(unsigned char)(*year%256));
	AT24C02_WriteByte(8,(unsigned char)(*year/256));
	AT24C02_WriteByte(0,1);
	
	LCD_ShowString(2,1,"Write");
	Delay(1000);
	LCD_ShowString(2,1,"     ");
	
	if(TimerWasRunning)
	{
		TR0=1;
	}
}


void Time_Load(unsigned int *hr,unsigned int *min,unsigned int *sec,unsigned int *am_pm,unsigned int *day,unsigned int *mon,unsigned int *year)
{
	if(AT24C02_ReadByte(0)!=1)
	{
		return;  //無有效資料,保留初始值
	}
	/*讀取時間*/
	*hr=AT24C02_ReadByte(1);
	*min=AT24C02_ReadByte(2);
	*sec=AT24C02_ReadByte(3);
	*am_pm=AT24C02_ReadByte(4);
	/*讀取日期*/
	*day=AT24C02_ReadByte(5);
	*mon=AT24C02_ReadByte(6);
	/*讀取年份(高低位組成16bit)*/
	*year=(unsigned int)AT24C02_ReadByte(7);
	*year|=(unsigned int)AT24C02_ReadByte(8)<<8;
	/*若E2PROM資料範圍有誤,恢復為初始值*/
	if(*hr<1||*hr>12||*min>59||*sec>59||*am_pm>1||*year<1||*mon<1||*mon>12||*day<1||*day>GetMaxDay(*mon,*year))
	//短路運算
	{
		*hr=11;*min=59;*sec=50;*am_pm=1;*year=2026;*mon=12;*day=31;
	}else
	{
		LCD_ShowString(2,1,"Read");
		Delay(1000);
		LCD_ShowString(2,1,"    ");
	}
}