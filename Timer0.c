#include <REGX52.H>

//函式定義：
/**
  * @brief  Timer0初始化函式
  * @param  無
  * @retval 無
  */

void Timer0_Init()
{
	TMOD&=0xF0;
	TMOD|=0x01;
	TF0=0;
	//晶振為11.0592MHz;MCU：12T mode;Timer 1次週期約1.085(us)(12/11.0592)
	//1(ms)/1.085(us)=922(次),所以16位Timer初值為65536-922=64614
	TH0=64614/256; //65536-922,高8位
	TL0=64614%256; //低8位
	TR0=1;
	ET0=1;
	EA=1;
	PT0=0;
}
