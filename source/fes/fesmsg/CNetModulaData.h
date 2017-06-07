#ifndef CNETMODULADATA_H_
#define CNETMODULADATA_H_


#include <qglobal.h>
#include <QString>
#include <QLatin1String>
#include <QList>
#include <QDateTime>

#include "fes/fesscdshm.h"
#include "Protocol_IEC104.h"



//���Ĵ洢��
class CNetModulaData
{
public:
	CNetModulaData();
	~CNetModulaData();
//	bool MatchKeyWord(CNetKeyWord* pKeyWord);

private:
	char* m_pData;
	int m_nLength;
	int m_nType;            //�жϱ�����������
	uchar* m_pKeyFlag;
	QString m_exchange;   //char��ת������洢��QString��
	//int checkbox_type;
	int cdt_frame_count;  //��"*"�ָ�CDT�ַ�����������ʾѭ������
	QString protocol_type;   //0703���
	
	QDateTime msg_time;
public:
	

	bool SetData(char* pData, int nLength);
	void SetType(int nType) { m_nType = nType; };
	void SetProtolType(QString qprotocol){ protocol_type = qprotocol; };
	void SetTime(QDateTime datetime){ msg_time = datetime; };
	QString GetProtolType(){return protocol_type; };
	int GetCDTFramecount(){return cdt_frame_count;};
	char* GetData() { return m_pData; };  //ȡ�ó��洢���ַ�������ʾ����
	int GetDataLength() { return m_nLength; };  //����շ����ĳ���
	int GetType() { return m_nType; };
	uchar* GetKeyFlag() { return m_pKeyFlag; }
	

	void analyze_iec104_asc( char* pData, int nLength);
	void analyze_cdt_asc( BYTE* pData, int nLength, int nType);
	void cdt_untotal_frame(int nType) {m_exchange += _C2Q("��CDT����֡����֡�ϲ�����");};
	void cdt_rebuild( char* pData, int nLength, int nType);
	int GetexDataLength() { return m_exchange.length(); };
    P104_APDU *apdu_104;
	QString GetExchange() { return m_exchange; };  //���ش洢���ַ�����������ʾ

	QString GetStrTime(){return msg_time.toString("yyyy-MM-dd hh:mm:ss.zzz");};
};

#endif