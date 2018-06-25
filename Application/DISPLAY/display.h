#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "sys.h"
#include "config.h"

#define PUT_MSG_TIL(c)    { DPY_SetTil(c);}
#define PUT_MSG_CLEAR(c)  { DPY_Clear(); }
#define PUT_MSG_NEW(s, c) {	DPY_SetSpd(s);DPY_SetNewMsg(MSG_FLASH, c); }
#define PUT_MSG(s, c)     { DPY_SetSpd(s);	DPY_SetNewMsg(MSG_NO_FLASH, c); }

/* 水平显示的字符数 */
#define DPY_HOR_CHAR_NUM 21

#define MSG_BUF_HOR_NUM (DPY_HOR_CHAR_NUM+1)
#define MSG_BUF_VER_NUM 10

/* 信息刷新 */
enum DPY_MSG_FLASH_INFO
{
	MSG_FLASH = 1,
	MSG_NO_FLASH
};


_STATIC u8 gaMessageBuffer[MSG_BUF_VER_NUM * MSG_BUF_HOR_NUM];
/* 界面新行显示数据 */
extern void DPY_Init(void);
/* 屏幕显示新信息 */
extern int DPY_SetNewMsg(u8 nFlash, char *pStr);
/* 屏幕显示进度 */
extern int DPY_SetSpd(u16 nSpeed);    //显示进度
/* 屏幕设置标题 */
extern void DPY_SetTil(char *c);
/* 屏幕清除信息内容 */
extern void DPY_Clear(void);

/* 屏幕刷新界面，把显示缓存中的数据显示在屏幕上 */
_STATIC void DPY_RefreshWindow(void);
/* 信息缓存偏移 */
_STATIC int DPL_OfferMsgBuf(u16 nOfferSize);



#endif
