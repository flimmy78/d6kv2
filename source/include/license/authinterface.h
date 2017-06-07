#ifndef AUTH_INTERFACE_H
#define AUTH_INTERFACE_H

#include <QObject>
#include <QTimer>
#include <QThread>

#include "license/file_interface.h"

#ifdef AUTH_DLL
# define AUTH_EXPORT Q_DECL_EXPORT
#else
# define AUTH_EXPORT Q_DECL_IMPORT
#endif


class CAuthInterface : public QObject
{
	Q_OBJECT

public:
	CAuthInterface();
	~CAuthInterface();

	// ����Ȩ�������߳�д������ʱ�䡣
	virtual int OpenAuth(QTimer* timer, QMutex& mutex,QStringList macaddrlist);

	// �ر���Ȩ��
	virtual void CloseAuth();

	// �Ƿ���Ȩ����
	virtual int IsAuthorizedExpired(QStringList macaddrlist);

private:

	// ��ʱ��
	QTimer* m_timer;

	// ��ȨȨ��
	bool m_bUnlimited;

	// ����Ȩ��־
	bool m_bOpenAuthFlag;

	// �澯����
	int GetWarningLevel();

	// �澯����
	static int m_nLevel;

	// file
	CFile* m_pFile;

	QMutex* m_mutex;

private:

	void SaveWarningLevel(int nLevel);

	public slots:

		// ��ʱд������ʱ��
		void TimerWriteRunTime();
};

extern "C" AUTH_EXPORT CAuthInterface* GetAuthDll(); //��ȡ��CAuthInterface�Ķ���


#endif
