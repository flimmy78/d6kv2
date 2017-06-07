#ifndef GBUSOBJECT_H_HEADER_INCLUDED_BC2B9C4C
#define GBUSOBJECT_H_HEADER_INCLUDED_BC2B9C4C

#include "GRectObject.h"
class GScaleFactor;

//##ModelId=43D42C8B03B9
//##Documentation
//## ���壺
//##        ͼԪĸ��
//## ���ܣ�
//##        ����ĸ��
//## ��ƣ�
//##        ������
//## ���ڣ�
//##       2005-11-22
class GDE_EXPORT_DES GBusObject : public GRectObject
{
  public:
    //##ModelId=43D42F460148
    //##Documentation
    //## ���ܣ�
    //##   �ж��Ƿ�����ճ��
    //## ������
    //##   ��
    //## ���أ�
    //##   if is return 1
    //##   else return  0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-25
    virtual uint8 canSnap() const;

    //##ModelId=43D42F460157
    //##Documentation
    //## ���ܣ�
    //##   �ж�ָ�����Ƿ�λ��ճ����Χ
    //## ������  
    //##   const GPoint& pt
    //## ���أ�
    //##   uint8
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    virtual uint8 hitSnap(const GPoint& pt);

    //##ModelId=43D42F610109
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
    GBusObject();

    //##ModelId=43D42F61010A
    //##Documentation
    //## ���ܣ�
    //##     ���캯��   
    //## ������
    //##     const GBusObject&
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    GBusObject(const GBusObject& copy);

    //##ModelId=43D42F61010C
    //##Documentation
    //## ���ܣ�
    //##     ��ֵ����   
    //## ������
    //##     const GBusObject&
    //## ���أ�
    //##     const GBusObject&
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    const GBusObject& operator=(const GBusObject& copy);
	//##ModelId=43D435480148
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ��������
    //## ������  
    //##   const GFillStyle& //�����
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    virtual void setFillStyle(const GFillStyle& style);

    //##ModelId=43D42F610119
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ���ƹ��ܣ�����ͼԪ��̬ͼ��
    //## ������
    //##   GDC* pDC //���ƻ���
    //##   GScaleFactor* pSF//��������
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    virtual uint8 drawObj(GDC* pDC, GScaleFactor* pSF = NULL);

};



#endif /* GBUSOBJECT_H_HEADER_INCLUDED_BC2B9C4C */
