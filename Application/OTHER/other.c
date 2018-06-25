#include "other.h"
#include "adc.h"    //电源电压检测
#include "IO.h"    //P操作
#include "display.h"    //显示屏应用
#include "cmd.h"
#include "RS485.h"

u32 gnErrMsg = 0;

int main_other(void)
{		
	/*初始化系统外设*/ 
	SystemInit ();
	delay_init();	  
	uart_init(115200);	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	
	DPY_Init();
	PUT_NEW_MSG( 100, "LCD Display      [OK]");
	PUT_MSG_CLEAR();    //清屏
	
	PUT_MSG_TIL("1----Init Device----8");
	Check_DeviceInit();
	delay_ms(1000);
	PUT_MSG_CLEAR();    //清屏	
	
	PUT_MSG_TIL("2----Check Power----8");
	Check_Power();
	delay_ms(1000);
	PUT_MSG_CLEAR();    //清屏
	
	PUT_MSG_TIL("3-Check Para Setup--8");
	Check_AllPara(SET_PARA);
	delay_ms(1000);	
	PUT_MSG_CLEAR();    //清屏
	
	PUT_MSG_TIL("4-Check Stat Setup--8");
	Check_StatSetup();
	delay_ms(1000);	
	PUT_MSG_CLEAR();    //清屏
	
	PUT_MSG_TIL("5-Check Stat Reader-8");
	Check_AllStatReader();
	delay_ms(1000);	
	PUT_MSG_CLEAR();    //清屏

	PUT_MSG_TIL("6----Check Reboot---8");
	Check_Reboot();
	delay_ms(1000);
	PUT_MSG_CLEAR();    //清屏

	PUT_MSG_TIL("7-Check Para Reader-8");
	Check_AllPara(GET_PARA);
	delay_ms(1000);	
	PUT_MSG_CLEAR();    //清屏
	
	PUT_MSG_TIL("8-Check Stat Reader-8");
	Check_AllStatReader();
	delay_ms(1000);	
	PUT_MSG_CLEAR();    //清屏
	
	PUT_NEW_MSG(100, "Text Finish ...      ");
	Close_System();    //死循环

	
    return 0;
} 

_STATIC void Check_DeviceInit(void)
{
	CmdComminica_Init();
	PUT_NEW_MSG( 40, "RS485 9600       [OK]");
	IO_Init();      
	PUT_NEW_MSG( 60, "IO               [OK]");   
	Adc_Init();		
	PUT_NEW_MSG(100, "ADC Ch1 Ch2 Ch3  [OK]");
	  
}

_STATIC void Check_Power(void)
{
	u16 nPwr72V;
	u16 nPwr33V; 	
	char tmp[27] = {0};
	
	//7.2V
	memset(tmp, '\0', 27);
	strcpy(tmp, "Power 7.2V     ");
	nPwr72V=Get_Adc_Average(ADC_Channel_1,10);	
	if(nPwr72V < 0xe39 && nPwr72V > 0xdd5)
	{	
		strcat(tmp, "  [OK]");	
	}
	else
	{	
		strcat(tmp, "[Fail]");
		gnErrMsg |= ERR_PWR72V;	
	}
	PUT_NEW_MSG(50, tmp);
	
	//3.3V
	memset(tmp, '\0', 27);
	strcpy(tmp, "Power 3.3V     ");
	nPwr33V=Get_Adc_Average(ADC_Channel_3,10);	
	if(nPwr33V < 0x83e && nPwr33V > 0x7c2)
	{	
		strcat(tmp, "  [OK]");	
	}
	else
	{	
		strcat(tmp, "[Fail]");
		gnErrMsg |= ERR_PWR33V;		
	}
	PUT_NEW_MSG(100, tmp);
}

_STATIC void Check_AllPara(u8 eOper)
{
	int i;
	CmdRtnInfo_Typedef eInfo;
	char aTmp[22] = {0};
	int nLen = 0;
	char *p;
	
	for(i = 0; i < CMD_TABLE_NUM; i++)
	{		
		memset(aTmp, '\0', 22);
		strcat(aTmp, "Commend ");
		strcat(aTmp, *gaCmdTab[i]);    //读取命令头
		strcat(aTmp, "..");
		while(aTmp[nLen] != '\0') nLen++;    //计算命令头字符数
		for( p = &aTmp[nLen]; (*p == '\0') && (nLen < (21-6)); *p++ = ' ', nLen++);    //补空格
		nLen = 0;    //清0重新计数
		
		eInfo = CMD_OperPara(eOper, (char **)gaCmdTab[i]);    //设置参数比较是否正确
		if(eInfo == SUCC) strcat(aTmp, "  [OK]");
		else              strcat(aTmp, "[Fail]");

		PUT_NEW_MSG(20*i/3, aTmp);
	}
}

_STATIC void Check_StatSetup(void)
{

	
	PUT_NEW_MSG(35, "Set P1~4 K1~4    [..]");
	PI_Out();    //输出4个脉冲,3个开关量
	PUT_MSG(35, "Set P1~4 K1~4    [OK]");

}
_STATIC void Check_AllStatReader(void)
{
	int i = 0;
	char aTmp[32] = {0};
	int ret = 0;
	char *p;	
	
	PUT_NEW_MSG(67, "Finding Stat     [..]");
	ret = CMD_GetAllStat();
	if(ret > 0)
	{
		PUT_MSG(67,"Finding Stat     [OK]");
	}
	else
	{
		PUT_MSG(67,"Finding Stat   [Fail]");
	}
	
	memset(aTmp, '\0', 32);
	strcat(aTmp, "Receive Num:  ");
	p = aTmp;
	while(*p++ != '\0');
	p--;
	*p++ = ret%100/10 + '0'; RS485_TX(*(p-1));
	*p++ = ret%10 + '0';     RS485_TX(*(p-1)); RS485_TX(0X0D); RS485_TX(0X0A);
	*p = '\0';
	PUT_NEW_MSG(100, aTmp);	
	
	for(i = 0; i < CMD_STAT_BUF_LINE; i++)
	{
		if(*gaStatTab[i] != '\0')
		{
			PUT_NEW_MSG(i, gaStatTab[i]);
		}
	}
}
//_STATIC void Check_Reboot(void)
//{
//	PUT_NEW_MSG(28, "Device Stop ...  [OK]");
//	P72=0;
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
//	P72=1;
//	PUT_NEW_MSG(67, "Device Reboot    [..]");
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
//	PUT_MSG(67, "Device Reboot    [OK]");
//}

_STATIC void Close_System(void)
{
	PUT_NEW_MSG(100, "Auto Close Proess ..5");
	PUT_NEW_MSG(100, "Auto Close Proess ..4");
	PUT_NEW_MSG(100, "Auto Close Proess ..3");
	PUT_NEW_MSG(100, "Auto Close Proess ..2");
	PUT_NEW_MSG(100, "Auto Close Proess ..1");
	PUT_NEW_MSG(100, "|...... Close ......|");
	while(1)
	{
		PUT_MSG(100,"\\...... Close ....../");
		PUT_MSG(100,"-...... Close ......-");
		PUT_MSG(100,"/...... Close ......\\");
		PUT_MSG(100,"|...... Close ......|");
	}	
}
