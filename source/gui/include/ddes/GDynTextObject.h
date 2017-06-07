#ifndef GDYNTEXTOBJECT_H_HEADER_INCLUDED_BC7D01A3
#define GDYNTEXTOBJECT_H_HEADER_INCLUDED_BC7D01A3

#include "GTextObject.h"
class GScaleFactor;
class GFile;

//##ModelId=4382CFB4031C
//##Documentation
//## ���壺
//##        ͼԪ��̬�ı���
//## ���ܣ�
//##        ����ͼԪ��̬�ı���
//## ��ƣ�
//##        ������
//## ���ڣ�
//##       2005-11-22
class GDE_EXPORT_DES GDynTextObject : public GTextObject
{
  public:
    //##ModelId=4382D16A002E
    //##Documentation
    //## ���ܣ�
    //##     ���캯��   
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    GDynTextObject();

    //##ModelId=4382D16A002F
    //##Documentation
    //## ���ܣ�
    //##     ���캯��   
    //## ������
    //##     const GDynTextObject&
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    GDynTextObject(const GDynTextObject& copy);

    //##ModelId=4382D16A0031
    //##Documentation
    //## ���ܣ�
    //##     ��ֵ����   
    //## ������
    //##     const GDynTextObject&
    //## ���أ�
    //##     const GDynTextObject&
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    const GDynTextObject& operator=(const GDynTextObject& copy);

    //##ModelId=4340A54901C5
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ���ƹ��ܣ�����ͼԪ��̬ͼ��
    //## ������
    //##   GDC* pDC //���ƻ���
    //##   const GFillStyle& fs//�������
	//##   const GLineStyle& ls//�߿�����
	//##   uint8 bFill=1//�Ƿ����
	//##   uint8 bFrame=1//�Ƿ���Ʊ߿�
	//##   GScaleFactor* pSF//��������
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
	virtual void drawNormal(GDC *pDC ,\
		const GFillStyle& fs,\
		const GLineStyle& ls ,\
		uint8 bFill=1, uint8 bFrame=1 ,\
		GScaleFactor *pSF=NULL );

    //##ModelId=4382D16A003F
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

    //##ModelId=4382D16A0041
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

    //##ModelId=4382D16A0043
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      QDomElement& dom
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
    virtual uint8 readXML(QDomElement& dom);

    //##ModelId=4382D16A0045
    //##Documentation
    //## ���ܣ�
    //##      ���ر��溯��
    //## ������
    //##      QDomElement& dom
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
    virtual uint8 saveXML(QDomElement& dom);
	//##ModelId=43A0C35201B5
    //##Documentation
    //## ���ܣ�
    //##   �����ڲ�ʵʱֵ
    //## ������
    //##   const char* v
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-12-15
    virtual void setValue(const char* value);
	//##ModelId=43A0C35201B5
    //##Documentation
    //## ���ܣ�
    //##   �����ڲ�ʵʱֵ
    //## ������
    //##   const char* v
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-12-15
    virtual void setValue(FLOAT8 value);
    //##ModelId=4383117500FA
    //##Documentation
    //## ��Чλ��
    uint8 m_prec;
    //##ModelId=438311910119
    //##Documentation
    //## С��������
    uint8 m_sclnum ;
	//��ʾģʽ-������,��ͨģʽ
	uint8 m_dismode;
  private:



};



#endif /* GDYNTEXTOBJECT_H_HEADER_INCLUDED_BC7D01A3 */
