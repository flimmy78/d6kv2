#ifndef G_BAY_LIB_MANAGER_H_
#define G_BAY_LIB_MANAGER_H_
#include "ddef/ddes_comm.h"
#include "GGraphLayer.h"
#include "GLibManager.h"
class GFile;

class GDE_EXPORT_DES GBayLibManager : public GLibManager
{
public:
	//���캯��   
	GBayLibManager();
	//��������   
	~GBayLibManager();
	//���ر��溯��
	virtual uint8 saveXML(const char* strFile);
	//���ض�ȡ����
	virtual uint8 readXML(const char* strFile);
	//����д�뺯��
	virtual int32 saveFile(GFile& fFile);
	//���ض�ȡ����
	virtual int32 loadFile(GFile& fFile);
	//ָ���ļ��Ƿ�Ϊͼ���ļ�
	static uint8 isGraphLib(const char* strPath);
};



#endif
