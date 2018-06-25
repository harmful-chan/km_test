#include "cmd.h"
#include "RS485.h"

char gaStatTab[CMD_STAT_BUF_LINE][CMD_STAT_BUF_LEN] = {0};

const char *gaCmdTab[CMD_TABLE_NUM][2] = {
	{"A", "44440001"},
	{"C", "1805150900"},
	{"Q", "640044400622"},
	{"D", "1"},
	{"E5", "0000CC1"},
	{"APN", "cmnet*"},
	{"F5", "183.62.7.42:5013"},
	{"G", "2"},
	{"M1", "14"},
	{"S5", "-0.002"},
	{"T1", "04"},
	{"ME", "00"},
	{"TE", "00"},
	{"CM1", "5081"},
	{"OT", "8"},
	{"O", "6"}
};



/*
* ��ʼ������ͨѶ
*/
void CmdComminica_Init(void)
{
	RS485_Init(9600);
	memset(gaStatTab[0], '\0', CMD_STAT_BUF_LEN*CMD_STAT_BUF_LINE);
}


/*
* ���ò���
* pPara : �������������û��ȡ
*/
CmdRtnInfo_Typedef CMD_OperPara(u8 eOper, char *pPara[])
{
	int ret = 0;
	char *p = NULL;
	CmdRtnInfo_Typedef eInfo = FAIL;
	char aTmp[64] = {0};
	char aTmp1[64] = {0};
	
	/* ƴ�ӷ���ָ�� */
	memset(aTmp, '\0', 64);
	strcat(aTmp, "666666#");
	strcat(aTmp, *pPara);	
	if(eOper == SET_PARA)
	{

		strcat(aTmp, ":");
		strcat(aTmp, *(pPara+1));
	}
	else
	{
		strcat(aTmp, "?");
	}
	strcat(aTmp, "#");
	
	/* ���ܲ��ɹ����Ϸ��� */
	ret = RS485_Com((u8 *)aTmp, (u8 *)aTmp);
	if(ret < 0)
	{
		eInfo = FAIL;
		return eInfo;
	}
	
	//eInfo = SUCC;
	/* �ȽϷ��������Ƿ�����ȷ��Ϣ */
	memset(aTmp1, '\0', 64);
	strcat(aTmp, *(pPara+1));
	
	p = strstr(*pPara, "C");
	if(p != NULL)
	{
		p = aTmp1;
		while(*p++ != '\0');
		*(--p) = '0';	
	}
	
	p = strstr(*pPara, "APN");
	if(p != NULL)
	{
		p = aTmp1;
		while(*p++ != '\0');
		*(--p) = '0';	
	}
		
			
	
	p = NULL;
	p = strstr((const char *)aTmp, aTmp1);
	if(NULL == p)
		eInfo = FAIL;
	else
		eInfo = SUCC;
	
	return eInfo;
}


/*
* ��ȡ��Ϣȫ����Ϣ��������gaStatTab��
*/          
//��ܣ���֮������
#define _SEND(c)                                     \
{	                                                 \
	memset(aTmp, '\0',256);                          \
	strcat(aTmp, c);                                 \
	ret1 = RS485_Com((u8 *)aTmp, (u8 *)aTmp);\
	if(ret1 < 0)                                      \
	{                                                \
		return ret1;                                  \
	}                                                \
}
#define _FIND(NAME) {FindStat(pChk, "##NAME##:", STAT_##NAME);}
int CMD_GetAllStat(void)
{
	int ret1 = 0;
	int ret = 0;
	char aTmp[256];
	char *pChk = NULL;    //��ʼ����ַ
	char *pOldChk = NULL;
	
	/* ��ѯͨ������ */
	_SEND("222222#Z##");
	
	/* ��ȡ���� */
	pChk = aTmp;
	while(*pOldChk != *pChk)
	{
		pOldChk = pChk;
     	   
		ret += FindStat(pChk, "P1:", STAT_P1);     //������ϻ�ı�ָ��
		ret += FindStat(pChk, "P2:", STAT_P2);
		ret += FindStat(pChk, "P3:", STAT_P3);
		ret += FindStat(pChk, "P4:", STAT_P4);
		ret += FindStat(pChk, "A1:", STAT_A1);
		ret += FindStat(pChk, "A2:", STAT_A2);
		ret += FindStat(pChk, "A3:", STAT_A3);
		ret += FindStat(pChk, "A4:", STAT_A4);
		ret += FindStat(pChk, "K1:", STAT_K1);
		ret += FindStat(pChk, "K2:", STAT_K2);
		ret += FindStat(pChk, "K3:", STAT_K3);
		ret += FindStat(pChk, "K4:", STAT_K4);
	}
	
	//��ѯ״̬����
	_SEND("222222#Y##");
	/* ��ȡ���� */
	pOldChk = NULL;
	pChk = aTmp;
	while(*pOldChk != *pChk)
	{
		pOldChk = pChk;
     	   
		ret += FindStat(pChk, "ST:", STAT_ST);     //������ϻ�ı�ָ��
		ret += FindStat(pChk, "HW:", STAT_HW);
		ret += FindStat(pChk, "SW:", STAT_SW);
		ret += FindStat(pChk, "Net:", STAT_NET);
		ret += FindStat(pChk, "CSCA:", STAT_CSCA);
		ret += FindStat(pChk, "IP:", STAT_IP);
		ret += FindStat(pChk, "V:", STAT_V);
		ret += FindStat(pChk, "Reset:", STAT_RESET);
	}	
	
	return ret;
}

/*
* ���ַ�����ʶ���ض����ݣ��Ž�gaStatTab[nStat] ��
* pChk, ����ַ���ͷ��ַ
* pHear, �������
* nStat, gaStatTab�е�λ��
*/
_STATIC int FindStat(char *pChk, char *pHear, u8 nStat)
{
	int ret = 0;
	char *pDstTmp = NULL;    //Ŀ�껺���ַ    
	char *p = NULL;    // ��λ��������ָ���ַ�����λ��
	
	p = strstr(pChk, pHear);         
	if(NULL != p)                     
	{                      
		memset(gaStatTab[nStat], '\0', CMD_STAT_BUF_LEN);
		pDstTmp = gaStatTab[nStat];  
		//p += sizeof(pHear)-1;            
		while(*p != ';' && *p != '\0')               
			*pDstTmp++ = *p++;         
		*pDstTmp = '\0';               
		pChk = p;     //���ı�ָ��Ļ�������Դ�ַ�����ѯ��                                                                                   
		ret++;
	}  	
	
	return ret;
}
