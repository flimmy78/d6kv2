#ifndef GCUSTOMLIBMANAGER_H_HEADER_INCLUDED_BC5DF854
#define GCUSTOMLIBMANAGER_H_HEADER_INCLUDED_BC5DF854
#include "ddef/ddes_comm.h"
#include "GGraphLayer.h"
#include "GLibManager.h"
class GFile;

//##ModelId=43A20F3C03A9
class GDE_EXPORT_DES GCustomLibManager : public GLibManager
{
  public:
    //##ModelId=43A20F640301
    //##Documentation
    //## ���ܣ�
    //##       ���캯��   
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-12-16
    GCustomLibManager();

    //##ModelId=43A20F640302
    //##Documentation
    //## ���ܣ�
    //##       ��������   
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-12-16
    virtual ~GCustomLibManager();

    //##ModelId=43A20F6402EE
    //##Documentation
    //## ���ܣ�
    //##      ���ر��溯��
    //## ������
    //##      const char* strFile
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
    virtual uint8 saveXML(const char* strFile);

    //##ModelId=43A20F6402F0
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      const char* strFile
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
    virtual uint8 readXML(const char* strFile);

    //##ModelId=43A20F6402FD
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      GFile& fFile
    //## ���أ���int32��
    //##      if success return write real size
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 saveFile(GFile& fFile);

    //##ModelId=43A20F6402FF
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      GFile& fFile
    //## ���أ���int32��
    //##      if success return read real size
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 loadFile(GFile& fFile);

	//ָ���ļ��Ƿ�Ϊͼ���ļ�
	static uint8 isGraphLib(const char* strPath) ;
};



#endif /* GCUSTOMLIBMANAGER_H_HEADER_INCLUDED_BC5DF854 */
