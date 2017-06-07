#ifndef GFILLSTYLE_H_HEADER_INCLUDED_BC93CAEB
#define GFILLSTYLE_H_HEADER_INCLUDED_BC93CAEB

#include <QtCore/QString>

#include "ddef/ddes_comm.h"

class GFile;

//##ModelId=436C3A4A000F
//##Documentation
//## ���壺
//##        �����
//## ���ܣ�
//##        ����ͼԪ�����
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-5
class GDE_EXPORT_DES GFillStyle
{
  public:
    //##ModelId=436C3BCE0213
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

    //##ModelId=436C3BD20148
    //##ModelId=436C3BD20148
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

    //##ModelId=436C3FBC00FA
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
    GFillStyle();
    //##ModelId=436C3FC200BB
    //##Documentation
    //## ���ܣ�
    //##   �������캯��
    //## ������
    //##   const GFillStyle&
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    GFillStyle(const GFillStyle& copy);
    //##ModelId=436C3FD50148
    //##Documentation
    //## ���ܣ�
    //##   ��ֵ����
    //## ������
    //##   const GFillStyle&
    //## ���أ�
    //##   const GFillStyle&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    const GFillStyle& operator=(const GFillStyle& copy);



    //##ModelId=436C3A9C00AB
    //##Documentation
    //## �������
    //##     0x00��͸����ʾ
    //##     0x01�������ɫ���洢��ɫ
    //##     0x02�����ģʽ���洢ģʽ
    //##     0x03�����ͼ�񣬴洢ͼ��
	//##	 0x04= ��դ��䣬�洢��դ

    uint8 mask;

    //##ModelId=436C3AC30399
    //##Documentation
    //## ����ɫ
    uint32 back;

    //##ModelId=436C3AF0029F
    //##Documentation
    //## ǰ��ɫ
    uint32 fore;

    //##ModelId=436C3B390109
    //##Documentation
    //## ���ģʽ
    uint8 pattern;

    //##ModelId=436C3B630119
    //##Documentation
    //## ����ͼ��Դ
    QString imgSrc;
    //##ModelId=436C621C01A5
    //##Documentation
    //## ����ģʽ
    //##    0x00=����
    //##    0x01=����
    //##    0x02=ƽ��
    //##    0x03=����
    uint32 imgMode;
	 //##ModelId=436D782F031C
    //##Documentation
    //## ��դ���ģʽ
    //##   ����
    //##    0x00=�����
    //##    0x01=˫ɫ����
    //##    0x02=˫ɫ����
    //##    0x03=�������
    //##   ����
    //##    0x000000=����
    //##    0x010000=����
	//##	0x020000=�Զ�
    uint32 raster;
  private:

};

#endif /* GFILLSTYLE_H_HEADER_INCLUDED_BC93CAEB */
