#ifndef GFLOWOBJECT_H_HEADER_INCLUDED_BC2BBC30
#define GFLOWOBJECT_H_HEADER_INCLUDED_BC2BBC30
#include "GPolylineObject.h"
class GScaleFactor;

//##ModelId=43D42CC40148
//##Documentation
//## ���壺
//##        ͼԪ������
//## ���ܣ�
//##        ���峱����
//## ��ƣ�
//##        ������
//## ���ڣ�
//##       2005-11-22
class GDE_EXPORT_DES GFlowObject : public GPolylineObject
{
  public:
    //##ModelId=43D42D5E032C
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
    GFlowObject();

    //##ModelId=43D42D5E032D
    //##Documentation
    //## ���ܣ�
    //##     ���캯��   
    //## ������
    //##     const GFlowObject&
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    GFlowObject(const GFlowObject& copy);

    //##ModelId=43D42D5E032F
    //##Documentation
    //## ���ܣ�
    //##     ��ֵ����   
    //## ������
    //##     const GFlowObject&
    //## ���أ�
    //##     const GFlowObject&
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    const GFlowObject& operator=(const GFlowObject& copy);

    //##ModelId=43D42D5E033C
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
	//##ModelId=43D43D85036B
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

	//�ı���ʼ�����ֹ��
	GPoint  m_pts ;
	FLOAT8	m_lastValue;

};



#endif /* GFLOWOBJECT_H_HEADER_INCLUDED_BC2BBC30 */
