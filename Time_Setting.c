#include <REGX52.H>
#include "LCD1602.h"

unsigned char GetMaxDay(unsigned int mon,unsigned int year)
{
	if(mon==2)
	{
		if((year%4==0&&year%100!=0)||(year%400==0))
		{
			return 29;
		}else
		{
			return 28;
		}
	}else if(mon==4||mon==6||mon==9||mon==11)
	{
		return 30;
	}else
	{
		return 31;
	}
}



		/*調時狀態機*/
void TimeSetting(unsigned char *SettingMode,unsigned char KeyNum,unsigned int *hr,unsigned int *min,unsigned int *sec,unsigned int *am_pm,unsigned int *day,unsigned int *mon,unsigned int *year)
{
	switch((*SettingMode))
	{
		case 0:
			if(KeyNum==1)
			{
				TR0=0;
				*SettingMode=1;
			}
			break;
		case 1:
			if(KeyNum==1)
			{
				(*hr)++;
				if(*hr>12)
				{
					*hr=1;
				}
				LCD_ShowNum(1,9,*hr,2);
			}else if(KeyNum==2)
			{
				if(*hr<=1)
				{
					*hr=12;
				}else
				{
					(*hr)--;
				}
				LCD_ShowNum(1,9,*hr,2);
			}else if(KeyNum==3)
			{
				*SettingMode=2;
			}
			break;
		case 2:
			if(KeyNum==1)
			{
				(*min)++;
				if(*min>59)
				{
					*min=0;
				}
				LCD_ShowNum(1,12,*min,2);
			}else if(KeyNum==2)
			{
				if(*min==0)
				{
					*min=59;
				}else
				{
					(*min)--;
				}
				LCD_ShowNum(1,12,*min,2);
			}else if(KeyNum==3)
			{
				*SettingMode=3;
			}
			break;
		case 3:
			if(KeyNum==1)
			{
				(*sec)++;
				if(*sec>59)
				{
					*sec=0;
				}
				LCD_ShowNum(1,15,*sec,2);
			}else if(KeyNum==2)
			{
				if(*sec==0)
				{
					*sec=59;
				}else
				{
					(*sec)--;
				}
				LCD_ShowNum(1,15,*sec,2);
			}else if(KeyNum==3)
			{
				*SettingMode=4;
			}
			break;
		case 4:
			if(KeyNum==1)
			{
				*am_pm=1-*am_pm;
			}else if(KeyNum==2)
			{
				*am_pm=1-*am_pm;
			}else if(KeyNum==3)
			{
				*SettingMode=5;
			}
			if(*am_pm==0)
			{
				LCD_ShowString(1,1,"AM");
			}else
			{
				LCD_ShowString(1,1,"PM");
			}
			break;
		case 5:
			if(KeyNum==1)
			{
				(*year)++;
				if(*year>9999)
				{
					*year=1;
				}
				LCD_ShowNum(2,7,*year,4);
			}else if(KeyNum==2)
			{
				if(*year<=1)
				{
					*year=9999;
				}else
				{
					(*year)--;
				}
				LCD_ShowNum(2,7,*year,4);
			}else if(KeyNum==3)
			{
				*SettingMode=6;
			}
			break;
		case 6:
			if(KeyNum==1)
			{
				(*mon)++;
				if((*mon)>12)
				{
					(*mon)=1;
				}
				LCD_ShowNum(2,12,(*mon),2);
			}else if(KeyNum==2)
			{
				if((*mon)==1)
				{
					(*mon)=12;
				}else
				{
					(*mon)--;
				}
				LCD_ShowNum(2,12,*mon,2);
			}else if(KeyNum==3)
			{
				*SettingMode=7;
			}
			break;
		case 7:
			if(KeyNum==1)
			{
				(*day)++;
				if(*day>GetMaxDay(*mon,*year))
				{
					*day=1;
				}
				LCD_ShowNum(2,15,*day,2);
			}else if(KeyNum==2)
			{
				if(*day<=1)
				{
					*day=GetMaxDay(*mon,*year);
				}else
				{
					(*day)--;
				}
				LCD_ShowNum(2,15,*day,2);
			}else if(KeyNum==3)
			{
				*SettingMode=8;
			}
			break;
		case 8:
			TR0=1;
			*SettingMode=0;
		break;
	}	
}
