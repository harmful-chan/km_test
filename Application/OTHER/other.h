#ifndef _OTHER_H_
#define _OTHER_H_

#include "config.h"

enum ERROR_INFO
{
	ERR_PWR72V = 0x0001,
	ERR_PWR33V = 0x0002,	
	ERR_COM = 0x0004,
	ERR_PWR5V = 0x0008,
	
	ERR_SET_Q = 0x0100,
	ERR_SET_P = 0x0200,
	ERR_SET_A = 0x0400,
	ERR_SET_D = 0x0800,
	ERR_SET_C = 0x1000
};

extern int main_other(void);

_STATIC void Check_DeviceInit(void);
_STATIC void Check_Power(void);
_STATIC void Check_AllPara(u8 eOper);
_STATIC void Check_StatSetup(void);
_STATIC void Check_AllStatReader(void);
_STATIC void Check_Reboot(void);
_STATIC void Close_System(void);
#endif
