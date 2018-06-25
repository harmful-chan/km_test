#include "display.h"
#include "RSCG12864B.h"
#include "stdio.h"
_STATIC u8 gaMessageBuffer[MSG_BUF_VER_NUM * MSG_BUF_HOR_NUM] = {0};    //10��22��

/*
* ��Ļ��ʾ��ʼ����Ϣ����������ȫ��д�ո�;
*/
void DPY_Init(void)
{	
	/* ��ʼ��IIC�ӿڣ�����Ļ���ȣ����� */
	I2C_Initial();
	delay_ms(200);
	brightness(0,200);    //��Ļ����
	clear(0x00);    //����
}

/*
* ��ʾ������
* nSpeed : ��ǰ���� 0~ 100
* >>>>>>>>>>>>>>>>>>>  100%    > 20����Ԫ ����26����Ԫ '\0'һ����Ԫ
*/
int DPY_SetSpd(u16 nSpeed)
{
	u8 tmp[MSG_BUF_HOR_NUM];
	u16 nSdpNum;
	u16 nB, nS, nG;
	int i;
	
	/* �������㣬�������ó��� */
	//������
	nSdpNum = nSpeed*3;                      //*3 
	nSdpNum = nSdpNum/20;    //(nSdpNum>>4) + (nSdpNum>>2);    //nSdpNum/20 = nSdpNum/(16+4)'
	//�������ְ�λ
	nB = nSpeed % 1000;
	nB = nB / 100;         //nB/100<=>(64+32+4);
	//��������ʮλ
	nS = nSpeed % 100;
	nS = nS / 10;                   //nB/10<=>(8+2);
	//�������ָ�λ
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
* ��ʾ���������Ϣ��ֻ��ʾ�ַ�����ͷ26���ַ�������'\0'��β֮ǰ������
* pStr : ��ʾ�ַ����յ�ַ
* ����ֵ :  >= 0 ��ʾ�ַ�����
*		    -1   ƫ�ƴ���
*/
int DPY_SetNewMsg(u8 nFlash, char *pStr)
{
	u8 nCount = 0;
	int ret = 0;
	int i;
	
	//����ƫ��
	if(nFlash == MSG_FLASH)
	{
		ret = DPL_OfferMsgBuf(MSG_BUF_HOR_NUM);
		if(ret < 0)
			return ret;
	}
	
	//д�ַ�
	while(*pStr != '\0' && nCount < (MSG_BUF_HOR_NUM-1))
		gaMessageBuffer[nCount++] = *pStr++;
	//�ַ����Ȳ�����' '
	for(i = nCount; i < (MSG_BUF_HOR_NUM-1); i++)
		gaMessageBuffer[i] = ' ';
	gaMessageBuffer[(MSG_BUF_HOR_NUM-1)] = '\0';    //��Ϣ��βΪ'\0'
	
	//ˢ����Ļ
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
* ��Ϣ��������ƫ��,�±�[0]��ʼ�ַ���ƫ�Ƶ��±�[nOfferSize]�����������ֺ���
* nOfferSize : ƫ����
* ����ֵ : >= 0 �±�[0]ƫ�Ƶ����±���ֵ
*		     -1 ƫ��ֵ�������
*/
_STATIC int DPL_OfferMsgBuf(u16 nOfferSize)
{
	int i;
	u16 nMsgBufLen = MSG_BUF_VER_NUM * MSG_BUF_HOR_NUM;    //�ܳ���
	u16 nSurMsgBufLen = nMsgBufLen - nOfferSize;    //�ܳ��� - ƫ�Ƴ��� = ��Ҫ�����ַ����ȣ��±�Ϊ0��ʼ���㣩

	if(0 != (nOfferSize % MSG_BUF_HOR_NUM))
		return -1;
	
	//�����ַ��������ƶ���gMessageBuffer��β
	for(i = nSurMsgBufLen; i > 0; i--)
		gaMessageBuffer[(nMsgBufLen - nSurMsgBufLen + i) - 1] = gaMessageBuffer[i - 1];
	
	return nMsgBufLen - nSurMsgBufLen;
}

/* 
* ˢ�´�����ʾ
*/
_STATIC void DPY_RefreshWindow()
{
	int i;
	
	for(i = 6; i > 0; i--)
		print_string_5x7_xy(0x00, 0,  8*i, &gaMessageBuffer[MSG_BUF_HOR_NUM*(6-i)]);
	
}



