#ifndef __TIME_SETTING_H__
#define __TIME_SETTING__

unsigned char GetMaxDay(unsigned int mon,unsigned int year);
void TimeSetting(unsigned char *SettingMode,unsigned char KeyNum,unsigned int *hr,unsigned int *min,unsigned int *sec,unsigned int *am_pm,unsigned int *day,unsigned int *mon,unsigned int *year);

#endif


