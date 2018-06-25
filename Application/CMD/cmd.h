#ifndef _CMD_H_
#define _CMD_H_

#include "config.h"

/* ��������ֵ���壬ֻ�гɹ��Ͳ��ɹ� */
typedef enum CMD_RTN_INFO
{
	SUCC = 0,
	FAIL = -1
}CmdRtnInfo_Typedef;


/* CMD_OperPara() �Ķ����б� */
enum CMD_PEOR_INFO
{
	GET_PARA = 0,
	SET_PARA = 1
};

/* �豸״̬����gaStatTab[][] �ܵ����� */
#define CMD_STAT_BUF_LEN 32
#define CMD_STAT_BUF_LINE 64

/* ���������� */
#define CMD_TABLE_NUM 16

enum CMD_STAT_NUM
{
	STAT_P1=0,  STAT_P2,  STAT_P3,  STAT_P4, 
	STAT_P5,  STAT_P6,  STAT_P7,  STAT_P8, 
	STAT_P9,  STAT_P10, STAT_P11, STAT_P12, 
	STAT_P13, STAT_P14, STAT_P15, 
	
	STAT_A1,  STAT_A2,  STAT_A3,  STAT_A4, 
	STAT_A5,  STAT_A6,  STAT_A7,  STAT_A8, 
	STAT_A9,  STAT_A10, STAT_A11, STAT_A12, 
	STAT_A13, STAT_A14, STAT_A15,
	
	STAT_K1,  STAT_K2,  STAT_K3,  STAT_K4,
	
	STAT_ST, STAT_DV, STAT_HW,STAT_SW,
	STAT_NET, STAT_CSCA, STAT_IP,STAT_RESET,
	STAT_DATA, STAT_V
};
extern char gaStatTab[CMD_STAT_BUF_LINE][CMD_STAT_BUF_LEN];
extern const char *gaCmdTab[CMD_TABLE_NUM][2];

extern void CmdComminica_Init(void);    /* ͨѶ��ʼ�� */
extern CmdRtnInfo_Typedef CMD_OperPara(u8 eOper, char *pPara[]);    /* ���û���ȡ���� */
extern int CMD_GetAllStat(void);    /* ��ȡȫ��״̬���������ض�λ�� */
_STATIC int FindStat(char *pChk, char *pHear, u8 nStat);    /* ���ַ�����ʶ���ض����ݣ��Ž�gaStatTab[nStat] �� */


#endif
