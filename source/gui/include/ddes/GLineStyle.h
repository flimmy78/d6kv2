#ifndef GLINESTYLE_H_HEADER_INCLUDED_BC93FEE6
#define GLINESTYLE_H_HEADER_INCLUDED_BC93FEE6
#include "ddef/ddes_comm.h"
class GFile;

//##ModelId=436C37E60000
//##Documentation
//## ���壺
//##        �߷��
//## ���ܣ�
//##        ����ͼԪ���ͷ��
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-5
class GDE_EXPORT_DES GLineStyle
{
  public:
    //##ModelId=436C3A17034B
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      GFile& fFile //�ļ�
    //## ���أ���int32 ��
    //##      if success return write real size
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 saveFile(GFile& fFile);

    //##ModelId=436C3A1A0119
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ��������ȡ�������ݰ�������
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
    //##ModelId=436C3F36005D
    //##Documentation
    //## ���ܣ�
    //##   ���캯��
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    GLineStyle();

    //##ModelId=436C3F42037A
    //##Documentation
    //## ���ܣ�
    //##   �������캯��
    //## ������
    //##   const GLineStyle&
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    GLineStyle(const GLineStyle& copy);

    //##ModelId=436C3F6402CE
    //##Documentation
    //## ���ܣ�
    //##   ���캯��
    //## ������
    //##   const GLineStyle&
    //## ���أ�
    //##   const GLineStyle&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    const GLineStyle& operator=(const GLineStyle& copy);

    //##ModelId=436C390F0290
    //##Documentation
    //## �߷��
    uint8 style;

    //##ModelId=436C395B00BB
    //##Documentation
    //## �߿�
    int16 width;

    //##ModelId=436C397E0242
    //##Documentation
    //## ����ɫ
    uint32 color;

    //##ModelId=436C399A0290
    //##Documentation
    //## ���ͷ�����ֽ�Ϊ�ߴ磬���ֽ�Ϊ���
    uint32 arwLeft;

    //##ModelId=436C39C50167
    //##Documentation
    //## �Ҽ�ͷ�����ֽ�Ϊ�ߴ磬���ֽ�Ϊ���
    uint32 arwRight;
  protected:

};

#endif /* GLINESTYLE_H_HEADER_INCLUDED_BC93FEE6 */
