#ifndef GSYMBOLIBMANAGER_H_HEADER_INCLUDED_BC5DE299
#define GSYMBOLIBMANAGER_H_HEADER_INCLUDED_BC5DE299
#include "GGraphLayer.h"
#include "GLibManager.h"
#include "ddef/ddes_comm.h"
class GFile;

//##ModelId=43A1052001E4
class GDE_EXPORT_DES GSymboLibManager : public GLibManager
{
  public:
    //##ModelId=43A105D70242
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
    //##ModelId=43A105D70290
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
    //##ModelId=43A1069E0148
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
    //##ModelId=43A1069E0196
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
    //##ModelId=43A107000128
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
    GSymboLibManager();
    //##ModelId=43A1072F003E
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
    virtual ~GSymboLibManager();

	//ָ���ļ��Ƿ�Ϊͼ���ļ�
	static uint8 isGraphLib(const char* strPath) ;
};



#endif /* GGRAPHLIBMANAGER_H_HEADER_INCLUDED_BC5EE1DE */
