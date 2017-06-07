#ifndef FILE_INTERFACE_H
#define FILE_INTERFACE_H

#include <QFile>
#include <QTextStream>
#include <QMutex>

#include "license/AES.h"

#define  NEW_LINE    "\r\n"

typedef struct
{
	QString value;
	qint64 seek;
}VALUE_SEEK;

// QT �ļ�������װ
class LICENSELIB_EXPORT CFile
{
public:

	CFile(const char* fileName);
	~CFile();

	// ���ļ�
	bool OpenFile(QIODevice::OpenMode ioFlags);

	// �ر��ļ�
	void CloseFile();

	bool isExist();

	// ��ȡ�ļ�
	QString ReadFile();

	// ��ָ����seekλ��׷������
	void InsertData(QString data, qint64 seek);

	// ��ָ����seekλ��׷�Ӽ��ܺ������
	int InsertData(QString strKey, QString data, AES& aes);

	// ��ָ������׷������
	void InsertData(void* data, int row);

	// ���ļ�β��׷������
	void AppendData(QString data, bool isNewLine = true);

	// �޸�����
	void ModifyData(QString data, qint64 seek);

	// ɾ��ָ������
	void DeleteFileData(int row);

	// ɾ��ָ��λ�á�ָ����С������
	void DeleteFileData(long long seek, long long size);

	// ����key������value
	VALUE_SEEK* SearchValueFromKey(QString key, AES& aes);

	// ���õ�ǰseekλ��
	void SetCurrentSeek(qint64 pos);

	// ��ȡ��ǰseekλ��
	qint64 GetCurrentSeek() const;

	// seek��λ
	void RecoverySeek();

	// �ļ���С
	qint64 FileSize();

	// ˢ��
	void Flush();

private:

	// �ļ���
	const char* m_fileName;

	// QT �ļ���������
	QFile* m_pFile;

	// ��
	QMutex m_mutex;

public:

	// λ��
	qint64 m_seek;

	QTextStream* m_pStream;

};

#endif
