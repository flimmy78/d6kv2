#ifndef GLIBMANAGER_H_HEADER_INCLUDED_BC052778
#define GLIBMANAGER_H_HEADER_INCLUDED_BC052778
#include "ddef/ddes_comm.h"
#include "GGraphLayer.h"
class GFile;
class GBaseObject;

//##ModelId=43FACD920251
class GDE_EXPORT_DES GLibManager : public GGraphLayer
{
  public:
    //##ModelId=43FACDBA0232
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
    GLibManager();

    //##ModelId=43FACDBA0242
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
    virtual ~GLibManager();

    //##ModelId=43FACDBA0243
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

    //##ModelId=43FACDBA0245
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

    //##ModelId=43FACDBA0251
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

    //##ModelId=43FACDBA0253
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

    //##ModelId=43FACDBA0255
    //##Documentation
    //## ���ܣ�
    //##      ��ȡָ��λ��ͼԪ���
    //## ������
    //##      uint nindex
    //## ���أ�
    //##      GBaseObject*
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-16
    GBaseObject* findIndex(uint32 nindex = 0) const;

	//##ModelId=43FACDBA0255
    //##Documentation
    //## ���ܣ�
    //##      ��ȡָ����ͼԪ���
    //## ������
    //##      const char* name
    //## ���أ�
    //##      GBaseObject*
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-16
    GBaseObject* findName(const char* name = 0) const;

    //##ModelId=43FACEF002CE
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ�Ƿ�Ϊ���ſ�
    //## ������
    //##      ��
    //## ���أ�
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-16
    uint8 isSymboLib() const;
	//##ModelId=4372FEA00167
    //##Documentation
    //## ���ܣ�
    //##   ��������ʶ
    //## ������
    //##   ��
    //## ���أ�
    //##   uint32
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-10
    virtual uint32 maxID();
	//##ModelId=4372FEA00167
    //##Documentation
    //## ���ܣ�
    //##   �������ñ�ʶ
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-10
	void	resetID();

    //##ModelId=43FACDDF0119
    //##Documentation
    //## ͼ���ļ���־{GDL}
    char m_gFlag[4];
	//##ModelId=4372FE6F0138
    //##Documentation
    //## ��ǰ����ʶ
    uint32 m_maxID;
};



#endif /* GLIBMANAGER_H_HEADER_INCLUDED_BC052778 */
