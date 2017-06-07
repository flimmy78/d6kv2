#ifndef _DATPDATA_H_
#define _DATPDATA_H_

#include "sysdef.h"
#include "model/scdtp.h"

#include <QMap>
#include <QString>

#define  BASEOBJTYPE_PWOER 20
//�����д���ʵʱ���豸,�Ժ������Ҫ��¼ͣ���豸���������ڱ��ṹ��չ
typedef struct
{
	char devName[CODE_LEN];	//�豸����
	int	 devType;			//�豸����
	char name[DESC_LEN];	//ʵʱ������
	int	 tpid;//��Ӧ���˽ṹ�������±�
} RTDATA;

//ͣ���������ṹ
typedef struct
{
	char	name[CODE_LEN];	//�豸����
	char	desc[DESC_LEN];	//
	char	devName[CODE_LEN]; //
	short	baseobjtype;
	float	value;
} ANALOG_P_DATA;

class CTpData
{
public:
	CTpData();
	~CTpData();

public:
	int readTpData();	//��ȡscdtp����Ϣ

public:
	RTDATA *m_rtData;
	int m_rtDataNum;//ʵʱ���ݸ���
	
	TPRCD* m_topoRcd;
	int m_topoRcdNum;	//�����豸����

	ANALOG_P_DATA* m_analogPData;	
	int m_analogPNum;	//ʵʱ���ʸ���


	//scdtp�д��뵽��¼�±�
	QMap< QString, int > m_name2ID;
};
#endif