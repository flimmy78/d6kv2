#ifndef GLINKOBJECT_H_HEADER_INCLUDED_BC2BC6A2
#define GLINKOBJECT_H_HEADER_INCLUDED_BC2BC6A2
#include "GPolylineObject.h"
class GScaleFactor;

//##ModelId=43D42CE70128
//##Documentation
//## ���壺
//##        ͼԪ������
//## ���ܣ�
//##        ����������
//## ��ƣ�
//##        ������
//## ���ڣ�
//##       2005-11-22
class GDE_EXPORT_DES GLinkObject : public GPolylineObject
{
  public:
    //##ModelId=43D42DB50186
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
    GLinkObject();

    //##ModelId=43D42DB50196
    //##Documentation
    //## ���ܣ�
    //##     ���캯��   
    //## ������
    //##     const GLinkObject&
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    GLinkObject(const GLinkObject& copy);

    //##ModelId=43D42DB50198
    //##Documentation
    //## ���ܣ�
    //##     ��ֵ����   
    //## ������
    //##     const GLinkObject&
    //## ���أ�
    //##     const GLinkObject&
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    const GLinkObject& operator=(const GLinkObject& copy);

    //##ModelId=43D42DB5019A
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

    //##ModelId=43D42F4000BB
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

    //##ModelId=43D42F4000BC
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
	//##ModelId=43D43D7A01E4
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ�߷��
    //## ������  
    //##   const GLineStyle& style //�߷��
    //## ���أ�
    //##   void
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    virtual void setLineStyle(const GLineStyle& style);

};



#endif /* GLINKOBJECT_H_HEADER_INCLUDED_BC2BC6A2 */
