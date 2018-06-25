#include "display.h"
#include "RSCG12864B.h"
#include "stdio.h"
_STATIC u8 gaMessageBuffer[MSG_BUF_VER_NUM * MSG_BUF_HOR_NUM] = {0};    //10行22列

/*
* 屏幕显示初始化信息，缓存数组全部写空格;
*/
void DPY_Init(void)
{	
	/* 初始化IIC接口，调屏幕亮度，清屏 */
	I2C_Initial();
	delay_ms(200);
	brightness(0,200);    //屏幕最亮
	clear(0x00);    //清屏
}

/*
* 显示进度条
* nSpeed : 当前进度 0~ 100
* >>>>>>>>>>>>>>>>>>>  100%    > 20个单元 数字26个单元 '\0'一个单元
*/
int DPY_SetSpd(u16 nSpeed)
{
	u8 tmp[MSG_BUF_HOR_NUM];
	u16 nSdpNum;
	u16 nB, nS, nG;
	int i;
	
	/* 数字运算，尽量不用除法 */
	//进度条
	nSdpNum = nSpeed*3;                      //*3 
	nSdpNum = nSdpNum/20;    //(nSdpNum>>4) + (nSdpNum>>2);    //nSdpNum/20 = nSdpNum/(16+4)'
	//进度数字百位
	nB = nSpeed % 1000;
	nB = nB / 100;         //nB/100<=>(64+32+4);
	//进度数字十位
	nS = nSpeed % 100;
	nS = nS / 10;                   //nB/10<=>(8+2);
	//进度数字个位
	nG = nSpeed % 10;
	
	for(i = 0; i < 15; i++)
		if(i < nSdpNum) 
			tmp[i] = '>';
		else            
			tmp[i] = ' ';
	
	tmp[15] = ' '; 
	tmp[16] = ' '; 
	tmp[17] = nB + '0';
	tmp[18] = nS + '0';
	tmp[19] = nG + '0';
	tmp[20] = '%';
	tmp[21] = '\0';
		

	print_string_5x7_xy(0x00, 0, 56, tmp);
		
	return nB*100 + nS*10 + nG;
	
}

/* 
* 显示屏输出新信息，只显示字符串开头26个字符，或以'\0'结尾之前的数据
* pStr : 显示字符串收地址
* 返回值 :  >= 0 显示字符个数
*		    -1   偏移错误
*/
int DPY_SetNewMsg(u8 nFlash, char *pStr)
{
	u8 nCount = 0;
	int ret = 0;
	int i;
	
	//缓存偏移
	if(nFlash == MSG_FLASH)
	{
		ret = DPL_OfferMsgBuf(MSG_BUF_HOR_NUM);
		if(ret < 0)
			return ret;
	}
	
	//写字符
	while(*pStr != '\0' && nCount < (MSG_BUF_HOR_NUM-1))
		gaMessageBuffer[nCount++] = *pStr++;
	//字符长度不够补' '
	for(i = nCount; i < (MSG_BUF_HOR_NUM-1); i++)
		gaMessageBuffer[i] = ' ';
	gaMessageBuffer[(MSG_BUF_HOR_NUM-1)] = '\0';    //信息结尾为'\0'
	
	//刷新屏幕
	DPY_RefreshWindow();

	return nCount;
}

void DPY_SetTil(char *c)
{ 
	print_string_5x7_xy(0x00, 0,  0, (u8 *)c); 
}


void DPY_Clear(void)
{ 
	int i, j;
	
	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 21; j++)
			gaMessageBuffer[i*22+j] = ' ';
	}
	DPY_RefreshWindow();
		
}



/* 
* 信息缓存整体偏移,下标[0]开始字符串偏移到下标[nOfferSize]处，超出部分忽略
* nOfferSize : 偏移量
* 返回值 : >= 0 下标[0]偏移到的下标数值
*		     -1 偏移值输入错误
*/
_STATIC int DPL_OfferMsgBuf(u16 nOfferSize)
{
	int i;
	u16 nMsgBufLen = MSG_BUF_VER_NUM * MSG_BUF_HOR_NUM;    //总长度
	u16 nSurMsgBufLen = nMsgBufLen - nOfferSize;    //总长度 - 偏移长度 = 需要保留字符长度（下表为0开始起算）

	if(0 != (nOfferSize % MSG_BUF_HOR_NUM))
		return -1;
	
	//保留字符串整体移动到gMessageBuffer结尾
	for(i = nSurMsgBufLen; i > 0; i--)
		gaMessageBuffer[(nMsgBufLen - nSurMsgBufLen + i) - 1] = gaMessageBuffer[i - 1];
	
	return nMsgBufLen - nSurMsgBufLen;
}

/* 
* 刷新窗口显示
*/
_STATIC void DPY_RefreshWindow()
{
	int i;
	
	for(i = 6; i > 0; i--)
		print_string_5x7_xy(0x00, 0,  8*i, &gaMessageBuffer[MSG_BUF_HOR_NUM*(6-i)]);
	
}



