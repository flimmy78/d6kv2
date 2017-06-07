//
//�޶�DA������ʽ�ı������ݽṹ����Ϊһ��ȫ�ֱ����洢�������ڳ����Ҫ����ģʽ�ļ��
//�ڳ����У�����ʱ�����ݿ��ȡ���ڳ�������ʱ����Щ���õĸı�
//һ����д�����ݿ⣬һ����д��ȫ�ֱ���
//
//                  ----Writen by xiaowq 2003-4-24       
/////////////////////////////////////////

#ifndef __DASETTINGS_H_
#define __DASETTINGS_H_
#include "rdbapi.h"
#include "GlobalDef.h"


class CDaSetting
{
	typedef struct
	{
		short DaMode;//DA����ģʽ��0����վ���룬��վ�ָ� 1����վ���룬��վ�ָ� 2����վ���룬��վ�ָ�����վ��
		short BrkProtectStyle;//���ش����ֱ��� 0������������2�����汣��
		unsigned char WorkingState;//��ǰ�Ĺ���״̬��0x00��ʵʱ̬��0x01������̬
		char Machine[DESC_LEN];		
		unsigned char PerformMode;//�ָ�������ִ��ģʽ
		unsigned char ExtendIsolate;//�Ƿ�����Χ����
		unsigned char RemoveNoloadDev;//�Ƿ��г������豸
		unsigned char CanGetFaultDir;//Ftu�ܷ�ɼ������Ϸ���
		unsigned char BeforeFaultSecs;//���ϵ�ǰ��ʱ�䣨s��
		unsigned char AfterFaultSecs;//���ϵ����ʱ�䣨s��
		unsigned char ykmode;// 0-Ⱥ�� 1-���п���
		unsigned char optaim;//�Ż�Ŀ�� 0-������������ 1-���ز�������
		int ExpandFlag; ///////���������Ժ���չ
	} DASETTING;
public:
	CDaSetting();
	~CDaSetting();
public:
	BOOL ReadSetting();
	BOOL WriteSetting(char* fldname = NULL);
//private:
public:
	DASETTING* m_pDasettingAddr;
private:
	BOOL InitDaSettingPointer();
	Rdb_LocalTab rdbDaSetting;
	RDB_BTBL_TAB_STRU m_tabInfo;
};
#endif
