#ifndef __AT24C02_H__
#define __AT24C02_H__

void AT24C02_WriteByte(unsigned char WordAddress,unsigned char Data);
unsigned char AT24C02_ReadByte(unsigned char WordAddress);
void Time_Save(unsigned int *hr,unsigned int *min,unsigned int *sec,unsigned int *am_pm,unsigned int *day,unsigned int *mon,unsigned int *year);
void Time_Load(unsigned int *hr,unsigned int *min,unsigned int *sec,unsigned int *am_pm,unsigned int *day,unsigned int *mon,unsigned int *year);


#endif