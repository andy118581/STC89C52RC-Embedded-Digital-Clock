#include <REGX52.H>
#include "Delay.h"

unsigned char Key_Scan(void)
{
	unsigned char Num=0;
	if(P3_1==0&&P3_0==0)
	{
		Delay(30);
		if(P3_1==0&&P3_0==0)
		{
			while(P3_1==0||P3_0==0);
			Delay(30);
			Num=4;
		}
	}else if(P3_1==0)
	{
		Delay(30);
		if(P3_1==0&&P3_0!=0)
		{
			while(P3_1==0);
			Delay(30);
			Num=1;
		}
	}else if(P3_0==0)
	{
		Delay(30);
		if(P3_0==0&&P3_1!=0)
		{
			while(P3_0==0);
			Delay(30);
			Num=2;
		}
	}else if(P3_2==0)
	{
		Delay(30);
		if(P3_2==0)
		{
			while(P3_2==0);
			Delay(30);
			Num=3;
		}
	}
	return Num;
}