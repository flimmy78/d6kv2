#ifndef GTEXTOBJECT_H_HEADER_INCLUDED_BC7D2513
#define GTEXTOBJECT_H_HEADER_INCLUDED_BC7D2513
#include "GRectObject.h"
class GScaleFactor;
class GFile;

//##ModelId=4382CFAB00BB
//##Documentation
//## ���壺
//##        ͼԪ�ı���
//## ���ܣ�
//##        ����ͼԪ�ı���
//## ��ƣ�
//##        ������
//## ���ڣ�
//##       2005-11-22
class GDE_EXPORT_DES GTextObject : public GRectObject
{
  public:
    //##ModelId=4382D14300CB
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
    GTextObject();

    //##ModelId=4382D14300CC
    //##Documentation
    //## ���ܣ�
    //##     ���캯��   
    //## ������
    //##     const GTextObject&
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    GTextObject(const GTextObject& copy);

    //##ModelId=4382D14300CE
    //##Documentation
    //## ���ܣ�
    //##     ��ֵ����   
    //## ������
    //##     const GTextObject&
    //## ���أ�
    //##     const GTextObject&
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    const GTextObject& operator=(const GTextObject& copy);

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

    //##ModelId=4382D14300DE
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

    //##ModelId=4382D14300E0
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

    //##ModelId=4382D14300E2
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

    //##ModelId=4382D14300E4
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
    //##ModelId=4383112B0138
    //##Documentation
    //## �ı�����
    QString m_text;
  private:


};



#endif /* GTEXTOBJECT_H_HEADER_INCLUDED_BC7D2513 */
