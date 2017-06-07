/**
@file analyzeutils.h
@brief ����ģ�飺
�������������Ϣ��ʸ��ͼ��г����ͼ������ͼ�������������Ϣ���Լ����ߺϳ� 

@author lujiashun 
@version 1.0.0
@date 2016-11-07
*/

#ifndef ANALYZEUTILS_H
#define ANALYZEUTILS_H

#include "comtrade.h"
#include "channelunit.h"
#include "complexs.h"
#include "selectdetailimpl.h"

class QAnalyze
{
public:
	QAnalyze();
	QAnalyze(Comtrade* c);
	~QAnalyze();

	void SetComtrade(Comtrade* c);
	QString FaultFile();
	int CursorPos()					{ return m_nCursorPos; }
	void SetCursorPos(int pos)		{ m_nCursorPos = pos; }
	Channels* AnaChannels()		{ return m_pAnaChannels; }
	Channels* DigChannels()		{ return m_pDigChannels; }
	QDetails* Details(int index);
	Comtrade* comtrade()			{ return m_Comtrade; }

	// ������Ϣ
	int ValuePage()					{ return m_nValuePage; }
	void SetValuePage(int n);
	bool ShowAllSwitch()			{ return m_bShowAllSwitch; }
	void SetShowAllSwitch(bool b)	{ m_bShowAllSwitch = b; }
	bool ShowNoSwitch()				{ return m_bShowNoSwitch; }
	void SetShowNoSwitch(bool b)	{ m_bShowNoSwitch = b; }
	int AverCount()					{ return m_nAverCount; }
	void SetAverCount(int n)		{ m_nAverCount = n; }
	QString CalAnaData(int ch, int pos, int idx);
	QString CalDigData(int ch, int pos, int idx);
	QString CalHarmData(int ch, int pos, int idx);
	int GetDetailIndex(QDetails* det, int idx);
	void GetSwitchInfo(int count, bool* args);

	// ʸ������
	int VectorPage()				{ return m_nVectorPage; }
	int VectorItemIndex()			{ return m_nHarmValueIndex; }
	void SetVectorItemIndex(int n)	{ m_nHarmValueIndex = n; }
	View_data* HarmChannel()		{ return m_pHarmChannel; }
	void SetHarmChannel(View_data* p)	{ m_pHarmChannel = p; }
	int HarmStyle()					{ return m_nHarmStyle; }
	void SetHarmStyle(int n)		{ m_nHarmStyle = n; }
	int HarmValueIndex()			{ return m_nHarmValueIndex; }
	void SetHarmValueIndex(int n)	{ m_nHarmValueIndex = n; }
	bool AddVector();
	Complex VectorValue(int idx, int pos);
	bool AddHarm();
	double HarmValue(int idx, int pos);
	int SelectOneChannel(const QString& str);

private:
	Comtrade* m_Comtrade;
	Channels* m_pAnaChannels;
	Channels* m_pDigChannels;
	int		m_nCursorPos;			// �����ϴ��α��λ��

	// ������Ϣ
	int		m_nValuePage;			// ���ڼ���״̬��������Ϣҳ��
	bool	m_bShowAllSwitch;		// ��ʾ���б�λ��Ϣ��־
	bool	m_bShowNoSwitch;		// ��ʾ�ޱ�λͨ����־
	QDetails* m_pDetails[3];		// �����ʾ��Ϣ
	int		m_nAverCount;			// ����ƽ��ֵ����	

    // ʸ��/г��
	int		m_nVectorPage;			// ���ڼ���״̬��ʸ��/г��ҳ��
	int		m_nVectorIndex;			// ʸ��ͼ��׼
	int		m_nHarmStyle;			// г����ʾģʽ
	int		m_nHarmValueIndex;		// г��ֵ��ʾģʽ
	View_data*	m_pHarmChannel;		// г��ͨ������

protected:
	void Init();
    void InitData();
	void InitAnaDetail();
	void InitDigDetail();

	int MaxHarmValue();
	int VectorIndex(int idx);
	void AddHarmBar(int idx, const QString& str);
	void InitHarmBarDetail();
	
};

#endif