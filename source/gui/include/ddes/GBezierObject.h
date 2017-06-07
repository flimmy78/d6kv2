#ifndef GBEZIEROBJECT_H_HEADER_INCLUDED_BC0A2377
#define GBEZIEROBJECT_H_HEADER_INCLUDED_BC0A2377

#include "GPolylineObject.h"
class GScaleFactor;

//##ModelId=43F5E9A5002C
//##Documentation
//## ���壺
//##        �ĵ��������
//## ���ܣ�
//##        ��������
//## ��ƣ�
//##        ������
//## ���ڣ�
//##       2006-2��17
class GDE_EXPORT_DES GBezierObject : public GPolylineObject
{
  public:
    //##ModelId=43F5EA2C002C
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
    GBezierObject();

    //##ModelId=43F5EA2C004B
    //##Documentation
    //## ���ܣ�
    //##     ���캯��   
    //## ������
    //##     const GBezierObject&
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    GBezierObject(const GBezierObject& copy);

  	//##ModelId=43F674F90213
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ���ƹ��ܣ�����ͼԪ��̬ͼ��
    //## ������
    //##   GDC* pDC //���ƻ���
    //##   uint32 nFlags
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    virtual void drawFocus(GDC* pDC, uint32 nFlags);

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

    //##ModelId=43F5EA2C0050
    //##Documentation
    //## ���ܣ�
    //##     ��ֵ����   
    //## ������
    //##     const GBezierObject&
    //## ���أ�
    //##     const GBezierObject&
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    const GBezierObject& operator=(const GBezierObject& copy);

};



#endif /* GBEZIEROBJECT_H_HEADER_INCLUDED_BC0A2377 */
