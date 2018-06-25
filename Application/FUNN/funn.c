#include "funn.h"

#include "rs485.h"
#include "adc.h"
#include "IO.h"
#include "display.h"

#define SYS_CLOSE() {\
DPY_SetNewMsg(MSG_FLASH, "[* * P  A * S  S * *]");\
Buz_Out_Over();\
while(1);}

/* 错误码列表，错误码以16进制显示 */
enum ERR_MSG{

	ERR_P1    = 0x00000001,
	ERR_P2    = 0x00000002,
	ERR_P3    = 0x00000004,
	ERR_P4    = 0x00000008,
	ERR_A1    = 0x00000010,
	ERR_A2    = 0x00000020,
	ERR_A3    = 0x00000040,
	ERR_A4    = 0x00000080,
	ERR_D1    = 0x00000100,
	ERR_D2    = 0x00000200,
	ERR_D3    = 0x00000400,
	ERR_D4    = 0x00000800,
	ERR_72V   = 0x00001000,
	ERR_33V   = 0x00002000,
	ERR_5V    = 0x00004000,
	ERR_COM   = 0x00010000,
	ERR_STORE = 0x00100000,
	ERR_SLEEP = 0X01000000
};

u16 nPwr72V = 0;
u16 nPwr33V = 0;
u16 nPwr5V = 0;
u32 nErr = 0;

char Time1[32] = {0};
char Time2[32] = {0};
char A[50] = {0};
char P[50] = {0};
char D[50] = {0};

//+++++++++++++++++++++++++++++++++++++界面功能实现++++++++++++++++++++++++++++++++++++++++++++++++++
/*
* 检测7.2V 和 3.3V
*/
float VolttoValue(float volt, float div)
{
	float ret = 0.0;
	ret = volt * 4096 / 3.3;
	ret *= div;
	return ret;
}

float ValuetoVolt(u16 value, float div)
{
	float ret = 0.0;
	ret = value * 3.3 / 4096;
	ret /= div;
	return ret;
}
void Check_72Vand33V(void)
{
	char aTmp[32] = {0};
		
	//7.2v电源检测
	memset(aTmp, '\0', 32);   
	strcat(aTmp, " * Power 7.2V  ");
	nPwr72V=Get_Adc_Average(ADC_Channel_1,10);	
	//if(nPwr72V < 0xe39 && nPwr72V > 0xdd5)    //3641 3541      7.3335    7.1321
	if(nPwr72V < (u16)VolttoValue(7.3335, 0.4) && nPwr72V > (u16)VolttoValue(7.1321, 0.4))
	{
		strcat(aTmp, "  [OK]");
	}
	else
	{ 
		strcat(aTmp, "[Fail]");
		nErr |= ERR_72V;
	}
	PUT_MSG_NEW(10, aTmp);
#if defined(ERROR_EXIT_IMMEDIATELY)
	if(nPwrErr)													
	{				
		SYS_CLOSE();
	}
#endif
	
	//3.3v电源检测		
	memset(aTmp, '\0', 32);
	strcat(aTmp, " * Power 3.3V  ");
	nPwr33V=Get_Adc_Average(ADC_Channel_3,10);	
	//if(nPwr33V < 0x83e && nPwr33V > 0x7c2)    //2275 1986    3.6657    3.2000
	if(nPwr33V < (u16)VolttoValue(3.6657, 0.5) && nPwr33V > (u16)VolttoValue(3.2000, 0.5))
	{
		strcat(aTmp, "  [OK]");
	}
	else
	{ 
		strcat(aTmp, "[Fail]");
		nErr |= ERR_33V;
	}
	PUT_MSG_NEW(15, aTmp);
#if defined(ERROR_EXIT_IMMEDIATELY)
	if(nPwrErr)													
	{				
		SYS_CLOSE();
	}
#endif
	
	PUT_MSG_NEW(18, " * Wait 5s ...   [..]");
	delay_ms(300);
	PUT_MSG(18, " * Wait 4s ...   [..]");
	delay_ms(300);
	PUT_MSG(18, " * Wait 3s ...   [..]");
	delay_ms(300);
	PUT_MSG(18, " * Wait 2s ...   [..]");
	delay_ms(300);
	PUT_MSG(18, " * Wait 1s ...   [..]");			
	delay_ms(300);
	PUT_MSG(20, " * Wait 0s ...   [OK]");	
}	

/*
* 检测485通讯和设置通道7777000000000000
*/
void Check_Rs485andSetPass(void)
{
	char aTmp[32] = {0};
	u8 aRecvTmp[256]; 
	char *p = NULL;
	char *pCmd = "666666#Q:7777000000000000P1:00000000P2:00000000P3:00000000P4:00000000C:1805150900#";    //M1:00001M2:00001M3:00001M4:00001S1:00000S2:00000S3:00000S4:00000T1:4.00T2:4.00T3:4.00T4:4.00
	char *pCmdCmp = "Q:7777000000000000;P1:00000000;P2:00000000;P3:00000000;P4:00000000";
	
	
	
	
	//check rs485 comminication
	PUT_MSG_NEW(30, " * RS485 COM     [..]");//--------------------------
	memset(aRecvTmp, '\0', 256);
	RS485_TXStr((u8 *)"666666#A:44444440#");
	RS485_RXStr(aRecvTmp);
	strcat(aTmp, " * RS485 COM   ");
	p = strstr((const char *)aRecvTmp, "ST:44444440;A:44444440");
	(p == NULL) ? strcat(aTmp, "[Fail]") :	strcat(aTmp, "  [OK]");
	if(p == NULL) nErr |= ERR_COM;
	PUT_MSG(32, aTmp);//-------------------------------------------------	
//#if defined(ERROR_EXIT_IMMEDIATELY)
	if(p == NULL)SYS_CLOSE();
//#endif

	
	
	//init P A D C         
	PUT_MSG_NEW(36, " * Init  PADC    [..]");//--------------------------
	memset(aTmp, '\0', 32);
	memset(aRecvTmp, '\0', 256);
	RS485_TXStr((u8 *)pCmd);
	RS485_RXStr(aRecvTmp);
	strcat(aTmp, " * Init  PADC  ");
	p = strstr((const char *)aRecvTmp, pCmdCmp);
	(p == NULL)?strcat(aTmp, "[Fail]"):	strcat(aTmp, "  [OK]");
	PI_Out();
	PUT_MSG(40, aTmp);//------------------------------------------------	
#if defined(ERROR_EXIT_IMMEDIATELY)
	if(p == NULL)SYS_CLOSE();
#endif
	
}

/*
* 检测5V 和 获取通道A1~4 P1~4 D1~4 的数据
*/
void Check_5VandPassValue(void)
{
	char aTmp[32] = {0};
	u8 aRecvTmp[256]; 
	char *p = NULL;		
	
	//check A,P,D 
	PUT_MSG_NEW(60, " * Check PAD     [..]");//--------------------------------------------	
	memset(aTmp, '\0', 32);
	memset(aRecvTmp, '\0', 256);
	RS485_TXStr((u8 *)"222222#Z#");           				   
	delay_ms(1000);
	nPwr5V = Get_Adc_Average(ADC_Channel_2, 10);    //5BV 
	RS485_RXStr(aRecvTmp);
	strcat(aTmp, " * Check PAD   ");
	p = strstr((const char *)aRecvTmp, "A1:0.37");         if(p == NULL){ strcat(A,"A1  "); nErr |= ERR_A1;}
	p = strstr((const char *)aRecvTmp, "A2:0.37");         if(p == NULL){ strcat(A,"A2  "); nErr |= ERR_A2;}
	p = strstr((const char *)aRecvTmp, "A3:0.14");         if(p == NULL){ strcat(A,"A3  "); nErr |= ERR_A3;}
	p = strstr((const char *)aRecvTmp, "A4:0.14");         if(p == NULL){ strcat(A,"A4  "); nErr |= ERR_A4;}
	p = strstr((const char *)aRecvTmp, "P1:00000004" );    if(p == NULL){ strcat(P,"P1  "); nErr |= ERR_P1;}
	p = strstr((const char *)aRecvTmp, "P2:00000004" );    if(p == NULL){ strcat(P,"P2  "); nErr |= ERR_P2;}
	p = strstr((const char *)aRecvTmp, "P3:00000004" );    if(p == NULL){ strcat(P,"P3  "); nErr |= ERR_P3;}
	p = strstr((const char *)aRecvTmp, "P4:00000004" );    if(p == NULL){ strcat(P,"P4  "); nErr |= ERR_P4;}
	p = strstr((const char *)aRecvTmp, "K1:0" );           if(p == NULL){ strcat(D,"D1  "); nErr |= ERR_D1;}
	p = strstr((const char *)aRecvTmp, "K2:0" );           if(p == NULL){ strcat(D,"D2  "); nErr |= ERR_D2;}
	p = strstr((const char *)aRecvTmp, "K3:0" );           if(p == NULL){ strcat(D,"D3  "); nErr |= ERR_D3;}
	p = strstr((const char *)aRecvTmp, "K4:0" );           if(p == NULL){ strcat(D,"D4  "); nErr |= ERR_D4;}     
	(*A != '\0' || *P != '\0' || *D != '\0') ? strcat(aTmp, "[Fail]") : strcat(aTmp, "  [OK]") ;
	PUT_MSG(62, aTmp);//---------------------------------------------------------------------
#if defined(ERROR_EXIT_IMMEDIATELY)
	if(*A != '\0' || *P != '\0' || *D != '\0')SYS_CLOSE();
#endif	
	
	
	
	
	//check 5v
	PUT_MSG_NEW(65, " * Power   5V    [..]");//-------------------------------------------	
	memset(aTmp, '\0', 32);
	strcat(aTmp, " * Power   5V  ");
	//if(nPwr5V < 0xc5d && nPwr5V > 0xbe0)    //3165	3040    5.0998    4.8984
	if(nPwr5V < (u16)VolttoValue(5.0998 , 0.5) && nPwr5V > (u16)VolttoValue(4.8984, 0.5))
	{
		strcat(aTmp, "  [OK]");
	}
	else
	{
		strcat(aTmp, "[Fail]");
		nErr |= ERR_5V;
	}
	PUT_MSG(70, aTmp);//-------------------------------------------------------------------
#if defined(ERROR_EXIT_IMMEDIATELY)
	if(nPwrErr)													
	{				
		SYS_CLOSE();
	}
#endif
}

/*
* 第一次读取设备时间
*/
void Check_ReadClock(void)
{
	u8 aRecvTmp[256]; 
	char aTmp[32];
	char *p = NULL;		
	
	//find clock the first
	PUT_MSG_NEW(75, " * Read Clock    [..]");
	memset(aRecvTmp, '\0', 256);
	memset(Time1, '\0', 32);
	memset(aTmp, '\0', 32);
	RS485_TXStr((u8 *)"666666#C?#");      
	RS485_RXStr(aRecvTmp);
	strcat(aTmp, " * Read Clock  ");
	p = strstr((const char *)aRecvTmp,"C:");
	if(p != NULL)
	{
		strcat(Time1,(const char *)aRecvTmp);
		strcat(aTmp, "  [OK]");
	}
	else
	{
		strcat(aTmp, "[Fail]");	
	}
	PUT_MSG(80, aTmp);
#if defined(ERROR_EXIT_IMMEDIATELY)
	if(p == NULL)SYS_CLOSE();
#endif
	
}

/*
* 第二次读取设备时间
*/
void Check_ReadClockAgain(void)
{
	u8 aRecvTmp[256]; 
	char aTmp[32];
	char *p = NULL;		
	
	//find clock the first
	PUT_MSG_NEW(90, " * Read Clock    [..]");
	memset(aRecvTmp, '\0', 256);
	memset(Time2, '\0', 32);
	memset(aTmp, '\0', 32);
	RS485_TXStr((u8 *)"666666#C?#");      
	RS485_RXStr(aRecvTmp);
	strcat(aTmp, " * Read Clock  ");
	p = strstr((const char *)aRecvTmp,"C:");
	if(p != NULL)
	{
		strcat(Time2,(const char *)aRecvTmp);
		strcat(aTmp, "  [OK]");
	}
	else
	{
		strcat(aTmp, "[Fail]");	
	}
	PUT_MSG(92, aTmp);
#if defined(ERROR_EXIT_IMMEDIATELY)
	if(p == NULL)SYS_CLOSE();
#endif	
}

/*
* 复位重启
*/
void Check_Reboot(void)
{	
	PUT_MSG_NEW(85, " * Reboot        [..]");
	P72=0;																	       //断开7.2v供电
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	P72=1;
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	PUT_MSG(88, " * Reboot        [OK]");
}

/*
* 睡眠模式
*/
void Check_Sleep(void)
{
	char aTmp[32];
	u8 aRecvTmp[256];
	char *p;
	
	//set deveice sleep model	
	PUT_MSG_NEW(92, " * Device Sleep  [..]");//--------------------------
	memset(aTmp, '\0', 32);
	memset(aRecvTmp, '\0', 256);
	RS485_TXStr((u8 *)"666666#G:1#");
	RS485_RXStr(aRecvTmp);
	strcat(aTmp, " * Device Sleep");
	p = strstr((const char *)aRecvTmp, "G:1");
	(p == NULL)?strcat(aTmp, "[Fail]"):	strcat(aTmp, "  [OK]");	
	if(p == NULL) nErr |= ERR_SLEEP;
	PUT_MSG(100, aTmp);//------------------------------------------------	
}


/*
* 发送错误信息
*/
void Check_ErrorMsg(void)
{
	char *p = NULL;
	char aTmp[32] = {0};
	memset(aTmp, '\0', 32);
	//if voltage error, output value
//	if(nErr & ERR_72V)
//	{
		memset(aTmp, '\0', 32);
		strcat(aTmp, "7.2V(+-100mV):");
		sprintf(aTmp+14, "%5f", ValuetoVolt(nPwr72V, 0.4));
		PUT_MSG_NEW(100, aTmp);
		
//	}
//	if(nErr & ERR_33V)
//	{
		memset(aTmp, '\0', 32);
		strcat(aTmp, "3.3V(+-100mV):");
		sprintf(aTmp+14, "%5f", ValuetoVolt(nPwr33V, 0.5));
		PUT_MSG_NEW(100, aTmp);		
//	}		
//	if(nErr & ERR_5V)
//	{
		memset(aTmp, '\0', 32);
		strcat(aTmp, "5.0V(+-100mV):");
		sprintf(aTmp+14, "%5f", ValuetoVolt(nPwr5V, 0.5));
		PUT_MSG_NEW(100, aTmp);
//	}
	
	//compare Time1 and Time2
	p = strstr(Time1,Time2);
	if(p == NULL)
	{	
		memset(aTmp, '\0', 32);
		strcat(aTmp, "Time 1:");
		strcat(aTmp, Time1+12);
		PUT_MSG_NEW(100, aTmp);
		memset(aTmp, '\0', 32);
		strcat(aTmp, "Time 2:");
		strcat(aTmp, Time2+12);
		PUT_MSG_NEW(100, aTmp);
		nErr |= ERR_STORE;
	}
	
	//check PAD error
	if(*A != '\0')
		PUT_MSG_NEW(100, A);
	if(*P != '\0')
		PUT_MSG_NEW(100, P);
	if(*D != '\0')
		PUT_MSG_NEW(100, D);
	
	//show error code
	memset(aTmp, '\0', 32);
	strcat(aTmp, "Error Code:");
	sprintf(aTmp+11, "%8X", nErr);
	PUT_MSG_NEW(100, aTmp);
	
	PUT_MSG_NEW(100, "... .. Finish .. ...");
}
