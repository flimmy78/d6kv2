///streadata.h
///ʵʱ������
///��ȡ�������ϵ�ʵʱ�ɼ�����------ң�ţ�����״̬
///��ȡ����բ�ϵ�ʵʱ�ɼ�����------ң�ţ���բ״̬
///��ȡ���豸�Ĵ���״̬
///����ͣ�翪ʼ�ͽ�����Ϣ
//////////////////////////////////////////////////////////////////////////////////////							

#ifndef _STREAM_DATA_H_ 
#define _STREAM_DATA_H_

#include "globaldef.h"
#include "rdb/rdbapi.h"
#include "net/netapi.h"

#include <QMap>
#include <QString>
#include <QStringList>

class CStreamData
{
	enum POWERDIR
	{
		NEGATIVE = -1,
		POSITIVE = 1,
	};
public:
	CStreamData();
	~CStreamData();
	int initStreamPara();	//�����ݲ�����

	void readStreamData();	//���ݶ�ȡ
	void stopStreamData();	//ֹͣ������ˢ��
	int saveStartData();	//����ͣ�翪ʼ��Ϣ
	int saveEndData();		//����ͣ�������Ϣ
	
private:
	RDB_FIELD_STRU* m_pStreamData;

	//ʵʱ���ݶ���
	unsigned char *m_rtDataState[REALDATA_BUF_SIZE];

	//�豸����״̬����
	unsigned char *m_tpDevActive[REALDATA_BUF_SIZE]; 

	//ʵʱ���ʶ���
	float *m_analogPValue[REALDATA_BUF_SIZE];

	//�豸ͣ�翪ʼʱ��
	QMap< QString, int > m_name2StartTime;

	//ͣ������豸
	QStringList m_endDevList;

	Rdb_QuickStream m_rdbStream;

	//��д��־����ֹ�����߳�ͬʱ����
	int bUsing[ REALDATA_BUF_SIZE ];
	int m_curWritePointer;
	int m_saveRdataTime[ REALDATA_BUF_SIZE ];	//�������ݵ�ʱ�����
	int m_firstFlag;	//�״������ݱ�ʶ
	DBApi m_dbApi;
private:
	void removeEndData();//ɾ���Ѿ�����������
};
#endif
