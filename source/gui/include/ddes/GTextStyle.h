#ifndef GTEXTSTYLE_H_HEADER_INCLUDED_BC93F287
#define GTEXTSTYLE_H_HEADER_INCLUDED_BC93F287

#include <QtCore/QString>
#include "ddef/ddes_comm.h"

class GFile;

//##ModelId=436C3BF4002E
//##Documentation
//## ���壺
//##        �ı����
//## ���ܣ�
//##        ����ͼԪ�ı����
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-5
class GDE_EXPORT_DES GTextStyle
{
  public:
    //##ModelId=436C3D3C0251
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

    //##ModelId=436C3D3E01B5
    //##ModelId=436C3D3E01B5
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
    
    //##ModelId=436C3FFE02CE
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
    GTextStyle();
    //##ModelId=436C400801C5
    //##Documentation
    //## ���ܣ�
    //##   �������캯��
    //## ������
    //##   const GTextStyle&
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    GTextStyle(const GTextStyle& copy);
    //##ModelId=436C401E0222
    //##Documentation
    //## ���ܣ�
    //##   ��ֵ����
    //## ������
    //##   const GTextStyle&
    //## ���أ�
    //##   const GTextStyle&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    const GTextStyle& operator=(const GTextStyle& copy);



    //##ModelId=436C3C2E007D
    //##Documentation
    //## �ı���ɫ
    uint32 color;

    //##ModelId=436C3C40005D
    //##Documentation
    //## ����ɫ
    uint32 back;

    //##ModelId=436C3C620148
    //##Documentation
    //## ͸��
    uint8 trans;

    //##ModelId=436C3C7C038A
    //##Documentation
    //## ������ ���������壬б�壬��Ӱ
    uint32 style;

    //##ModelId=436C3CFE0109
    //##Documentation
    //## ����
    QString family;

    //##ModelId=436C3D0E0261
    //##Documentation
    //## ��С
    int16 size;
    //##ModelId=436DB28702FD
    //##Documentation
    //## �ı������ʽ
    uint32 align;
};


#endif /* GTEXTSTYLE_H_HEADER_INCLUDED_BC93F287 */
