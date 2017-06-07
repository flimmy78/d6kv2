#ifndef _DRTDBO_H
#define _DRTDBO_H

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QMap>

#include "ddbi/ddbi_def.h"
#include "ddbo/ddbo.h"

class GDE_EXPORT_DRTDBO GykQueue:public Ddbo
{
public:
	GykQueue();
	~GykQueue();

	int32 exist( const char* name);
};

//ʵʱ��ykpara��
typedef struct __tagYKPARA
{
	char  name[ GDB_CODE_LEN  ];		//����
	char  desc[ GDB_DESC_LEN ];			//����
	uint16	terminalno;					//�ն����
	char	termdesc[ GDB_DESC_LEN ];	//�ն�����
	uint16	yxno;						//ң�����
	uint16	ykno;						//ң�����
	char	monflag;					//����ģʽ
	uint8	requesttime;				//ң������ʱ����
	uint8	waitchktime;				//ң�ط�Уʱ����
	uint8	sendcmdtime;				//ң�ط���ʱ����
	uint8	execcmdtime;				//ң��ִ��ʱ����
	uint8	lockykf;					//����ң�ر�־
	float	value;						//ֵ
	char	ykcode[ GDB_CODE_LEN * 2 ];		//ң��У����
	char	tablename[ GDB_CODE_LEN ];	//ң������ʵʱ����
	uint8	exykflag;					//����ң�ر�־
	char	ykmode;
	int32   securegrade;				//��ȫ����
}YKPARA,*PYKPARA;

class GDE_EXPORT_DRTDBO GykPara:public Ddbo
{
public:
	
	GykPara();
	~GykPara();

	uint8  readData( const char* name, const char* tableName, char* errormsg );
	uint8  readHData( const char* name, Ddbi *hDbi );
	PYKPARA data();
protected:
	
	YKPARA		m_ykPara ;
};

//
typedef struct __tagYTPARA
{
	char  name[ GDB_CODE_LEN ];			//����
	char  desc[ GDB_DESC_LEN ];			//����
	uint16	terminalno;					//�ն����
	char  termdesc[ GDB_DESC_LEN ];		//�ն�����
	uint16	upykno;						//��ң�غ�
	uint16	downykno;					//��ң�غ�
	uint16	stopykno;					//��ͣң�غ�
	uint16	ytno;						//ң����
	char	monflag;					//����ģʽ
	uint8	requesttime;				//ң������ʱ����
	uint8	waitchktime;				//ң�ط�Уʱ����
	uint8	sendcmdtime;				//ң�ط���ʱ����
	uint8	execcmdtime;				//ң��ִ��ʱ����
	uint8	lockytf;					//������־
	float	value;						//ֵ
	char	ytcode[ GDB_CODE_LEN * 2 ];		//ң��У����
	char	tablename[ GDB_CODE_LEN ];	//�������ڱ�����analog or updownpara
	uint8	yttype;						//ң�����ͣ�������ʽ������㷽ʽ
}YTPARA,*PYTPARA;

class GDE_EXPORT_DRTDBO GytPara:public Ddbo
{
public:
	
	GytPara();
	~GytPara();

	uint8  readData( const char* name, char* errormsg  );
	uint8  readHData( const char* name, Ddbi *hDbi );
	PYTPARA data();
protected:
	
	YTPARA		m_ytPara ;
};

typedef struct _tagCardHisInfo
{
	int16	sort;
	int16	type;
	char	desc[ GDB_DESC_LEN ];
	char	symbolpath[ 255 ];
	int16	symbolid;
}CARDHISINFO, *PCARDHISINFO;

typedef struct _tagCardRtInfo
{
	int16	objecttype;
	char	objectname[ GDB_CODE_LEN ];
	int16	cardtype;
	int16	cardno;
	FLOAT8	x;
	FLOAT8	y;
	int16	symbolid;
	char	hostname[ GDB_CODE_LEN ];
	char	username[ GDB_CODE_LEN ];
	char	cardtime[ GDB_DESC_LEN * 2 ];
	char	graphpath[ GDB_PATH_LEN ];
	char	symbolpath[ GDB_PATH_LEN ];
	char	note[ GDB_PATH_LEN ];
	char	devdesc[ GDB_DESC_LEN ];
}CARDRTINFO, *PCARDRTINFO;


class GDE_EXPORT_DRTDBO GcardHisPara:public Ddbo
{
public:
	
	GcardHisPara();
	~GcardHisPara();

	uint8  readData(); 
	PCARDHISINFO data();
	uint16 count();
protected:

	PCARDHISINFO	m_cardHisInfo;
	int32			m_cardHisCount;
};

class GDE_EXPORT_DRTDBO GcardRtPara:public Ddbo
{
public:
	
	GcardRtPara();
	~GcardRtPara();

	uint8  readData(); 
	PCARDRTINFO data();
	uint16 count();
protected:
	void clear();

	PCARDRTINFO		m_cardRtInfo ;
	int32			m_cardRtCount;
};

typedef struct _tagBreakRepInfo
{
	char	breakrep[ GDB_CODE_LEN ];
	char	breakrepdesc[ GDB_DESC_LEN ];
	char	breakname[ GDB_CODE_LEN ];
	char	breakdesc[ GDB_DESC_LEN ];
	uint8	replacetype;
	char	plswitch[ GDB_CODE_LEN ];
	uint8	replacemode;
}BREAKREPINFO, *PBREAKREPINFO;

class GDE_EXPORT_DRTDBO GbreakRepPara:public Ddbo
{
public:
	
	GbreakRepPara();
	~GbreakRepPara();

	uint8  readData( const char * breakrepname ); 
	PBREAKREPINFO data();
	uint16 count();
	uint8	m_replacedFlag; //�������־
	char	m_replacedBreak[ GDB_CODE_LEN ];//���������
	char	m_replacedBreakDesc[ GDB_DESC_LEN ];
protected:
	PBREAKREPINFO		m_breakRepInfo ;
	int32			m_breakRepCount;
};

class GDE_EXPORT_DRTDBO GName2DescMap:public Ddbo
{
public:
	GName2DescMap(){};
	~GName2DescMap(){};

	uint8  readData();
	
	uint8 getDesc( const char * name, char * desc );

	uint16 count();

protected:
	QMap< QString, QString > m_name2DescMap;
};

#endif
