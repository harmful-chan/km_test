#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "sys.h"
#include "config.h"

#define PUT_MSG_TIL(c)    { DPY_SetTil(c);}
#define PUT_MSG_CLEAR(c)  { DPY_Clear(); }
#define PUT_MSG_NEW(s, c) {	DPY_SetSpd(s);DPY_SetNewMsg(MSG_FLASH, c); }
#define PUT_MSG(s, c)     { DPY_SetSpd(s);	DPY_SetNewMsg(MSG_NO_FLASH, c); }

/* ˮƽ��ʾ���ַ��� */
#define DPY_HOR_CHAR_NUM 21

#define MSG_BUF_HOR_NUM (DPY_HOR_CHAR_NUM+1)
#define MSG_BUF_VER_NUM 10

/* ��Ϣˢ�� */
enum DPY_MSG_FLASH_INFO
{
	MSG_FLASH = 1,
	MSG_NO_FLASH
};


_STATIC u8 gaMessageBuffer[MSG_BUF_VER_NUM * MSG_BUF_HOR_NUM];
/* ����������ʾ���� */
extern void DPY_Init(void);
/* ��Ļ��ʾ����Ϣ */
extern int DPY_SetNewMsg(u8 nFlash, char *pStr);
/* ��Ļ��ʾ���� */
extern int DPY_SetSpd(u16 nSpeed);    //��ʾ����
/* ��Ļ���ñ��� */
extern void DPY_SetTil(char *c);
/* ��Ļ�����Ϣ���� */
extern void DPY_Clear(void);

/* ��Ļˢ�½��棬����ʾ�����е�������ʾ����Ļ�� */
_STATIC void DPY_RefreshWindow(void);
/* ��Ϣ����ƫ�� */
_STATIC int DPL_OfferMsgBuf(u16 nOfferSize);



#endif
