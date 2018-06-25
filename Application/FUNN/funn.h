#ifndef _FUNN_H_
#define _FUNN_H_

#include "sys.h"
extern u32 nErr;				
extern char Time1[32];
extern char Time2[32];
extern char A[50];
extern char P[50];
extern char D[50];

extern float VolttoValue(float volt, float div);
extern float ValuetoVolt(u16 value, float div);

extern void Check_72Vand33V(void);
extern void Check_Rs485andSetPass(void);
extern void Check_5VandPassValue(void);
extern void Check_ReadClock(void);
extern void Check_ReadClockAgain(void);
extern void Check_Reboot(void);
extern void Check_Sleep(void);
extern void Check_ErrorMsg(void);

#endif
