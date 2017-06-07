#ifndef CNETMODULA_H_
#define CNETMODULA_H_

#include <QString>
#include <QDateTime>
#include <QMutex>
#include <QList>
#include <QTextStream>

#include "fes/fesscdshm.h"

#define NETVIEW_MAXBYTE		40960
#define NETVIEW_TOTAL_MAX	40*1024*1024

class QFile;
class CNetModulaData;


//���Ĳ�����
class CNetModula
{
public:
	CNetModula();
	~CNetModula();

public:
	//CNetModulaData�洢�ı���ת�浽�ˣ�ָ��CNetModulaData�ļ���,���ļ���
	QList<CNetModulaData*> m_ListData;

private:
	QString m_strName;
	int m_nTotalBytes;
	bool m_bPause;
	int m_nMaxBytes;
	//
	bool m_bSaveToFile;
	QString m_savePath;
	QDateTime m_tEndSaveTime;
	QFile* m_saveFile;

	QDateTime m_save_begintime;   //���Ĵ洢��ʼʱ��,0803,chengzhu
	QDateTime m_save_endtime;     //���Ĵ洢����ʱ��
	int m_autosave_checkbox_state;


	

public:
	void SetPause(bool bPause) { m_bPause = bPause; }
	bool GetPause() { return m_bPause; }
	//����ͨ������
	void SetName(QString strName) { m_strName = strName; }
	QString GetName() { return m_strName; }

	void SetMaxBytes(int nMaxBytes) { m_nMaxBytes = nMaxBytes; }
	bool AddData(char* pData, int nLength, int nType, QString qprotocol, QDateTime save_begintime, QDateTime save_endtime,\
		int save_checkbox, QString channel_name, FETIME* msgtime);   //nType��ʾ�����У�pType��ʾ��Լ
	
	void RemoveData(int nTotal);
	//�������
	void ClearListData();
	
	//���汨��
	void SetSaveToFile(bool b); //{ m_bSaveToFile = b; }
	bool GetSaveToFile() { return m_bSaveToFile; }
	//���ı���·��
	void SetSavePath(QString strPath) { m_savePath = strPath; }
	QString GetSavePath() { return m_savePath; }
	void SetEndTime(QDateTime t) { m_tEndSaveTime = t; }
	QDateTime GetEndTime() { return m_tEndSaveTime; }
	void TraceToFile(char* pdata,int nlength);

	//�Զ��洢����
	QDateTime Get_autosave_begintime() {return m_save_begintime;}
	QDateTime Get_autosave_endtime() {return m_save_endtime;}
	int Get_autosave_checkbox_state() {return m_autosave_checkbox_state;}

	void Set_autosave_begintime(QDateTime begintime) {m_save_begintime = begintime;}
	void Set_autosave_endtime(QDateTime endtime) {m_save_endtime = endtime;}
	void Set_autosave_checkbox_state(int check_box_type) {m_autosave_checkbox_state = check_box_type;}

	void AutoSaveMessage(CNetModulaData* pModulaData, QDateTime save_begintime, QDateTime save_endtime,QTextStream &out);
};


#endif 