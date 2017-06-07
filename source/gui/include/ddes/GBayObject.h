#ifndef G_BAY_OBJECT_H_
#define G_BAY_OBJECT_H_

#include "GComObject.h"

class GScaleFactor;
class GFile;
class GBaseObject;

//�����
class GDE_EXPORT_DES GBayObject : public GComObject
{
public:
	GBayObject();
	virtual ~GBayObject();

	//д���ļ�
	virtual int32 saveFile(GFile& fFile);
	//��ȡ�������ݰ�������
	virtual int32 loadFile(GFile& fFile);
	//����ͼԪ������ͼԪ��̬ͼ��
	virtual uint8 drawObj(GDC* pDC, GScaleFactor* pSF = NULL);
	//���ض�ȡXML����
	virtual uint8 readXML(QDomElement& dom);
	//���ر���XML����
	virtual uint8 saveXML(QDomElement& dom);
	//���Ե�ǰ����ͼԪ���λ��
	virtual uint32 hitTest(const GPoint& pt);
	//����ͼԪ����
	virtual void calcRect();
	////X,Yƫ����
	//virtual void offsetXY(FLOAT8 x, FLOAT8 y, uint32 flags = 0);
	////X,Y����ƫ����
	//virtual void resizeXY(const GPoint& pt, FLOAT8 x, FLOAT8 y, uint32 flags = 0);
	////��ͼԪ���о�����
	//virtual void mirrorXY(uint32 op, uint32 flags = 0);
	////��ָ��ԭ���ͼԪ���о�����
	//virtual void mirrorXY(const GPoint& pto, uint32 op, uint32 flags = 0);
	//��ͼԪ�ڲ���ת���ĵ���תth��
	virtual void rotateXY(FLOAT8 th, uint32 flags = 0);
	////��ָ����ת���ĵ�ptc��תth��
	//virtual void rotateXY(const GPoint& ptc, FLOAT8 th, uint32 flags = 0);
	//���������
	//virtual int32 onMouseEnter(GDC* pDC, const GPoint& pt, uint32 flags);
	////����ڶ������ƶ�����
	//virtual int32 onMouseMove(GDC* pDC, const GPoint& pt, uint32 flags);
	////����뿪����
	//virtual int32 onMouseLeave(GDC* pDC, const GPoint& pt, uint32 flags);
	////���������������
	//virtual int32 onLButtonUp(GDC* pDC, const GPoint& pt, uint32 flags);
	////������������
	//virtual int32 onLButtonDown(GDC* pDC, const GPoint& pt, uint32 flags);
	////������Ҽ��������
	//virtual int32 onRButtonUp(GDC* pDC, const GPoint& pt, uint32 flags);
	////������Ҽ�����
	//virtual int32 onRButtonDown(GDC* pDC, const GPoint& pt, uint32 flags);
	////�ṩ�������ż��㣬�޷��أ�ֱ�Ӹı���ƶ���
	////Ĭ�Ϻ��ԣ���Ҫ�ڸú����е���pSF->scaleObject
	////virtual void scaleFactor(GScaleFactor* pSF);
	////ͼԪ�������,����ָ�����ܶ���ͼԪ
	//virtual void alignPos(uint32 op, FLOAT8 v);
	//ͼԪ�ȴ�С����,����ָ�����ܵȴ�СͼԪ
	virtual void sameSize(uint32 op, FLOAT8 w, FLOAT8 h);
	//////���Ըö����Ƿ������ת
	////virtual uint8 canRotate() const;
	////����ͼԪ��������
	//virtual void setFillStyle(const GFillStyle& style);
	////����ͼԪ�߷��
	//virtual void setLineStyle(const GLineStyle& style);
	////����ͼԪ�ı������Ϣ
	//virtual void setTextStyle(const GTextStyle& style);
	////�������ö�������ID
	////resetΪ0ʱ����OwnerID���������OwnerID
	////nIDΪ-1ʱ����ǰIDΪ�Ӷ��ӵ�����������ָ��IDΪ����
	////pObjΪ NULL ��ǰ����Ϊ�Ӷ��ӵ�����������ָ������Ϊ����	
	////�ɹ�����1��ʧ�ܷ���0
	//virtual void setPortOwner(uint8 reset=0,uint32 nID=(uint32)-1,\
	//	GBaseObject* pObj=NULL);
	////ͼԪ��ʱ��Ӧ����
	////ymd ������ = yyyymmdd
	////hms ʱ���� = hhmmss
	//virtual uint8 onTimer(uint32 ymd, uint32 hmsm);
	////ͼԪ�Ƿ�����ʱ��Ӧ����
	////�Ƿ���1���񷵻�0
	//virtual uint8 canTimer() const;
	//��ȡ�ؼ�����
	//virtual QString getPluginName() const;
	////�����ڲ�ʵʱֵ
	//virtual void setValue(FLOAT8 value);
	////����ͼԪ��ǰ��
	//virtual void setLayer(GGraphLayer* pLayer);
	//��ȡ����ͼԪ����
	//uint8 loadElems(const char* libPath,uint32 libID);
	////�����Զ�ɾ�����еĶ���
	//void setAutoDelete(uint8 bAutoDelete);
	////ͼԪ�Ƿ񼤻�
	//virtual void setActive(uint8 active );
	////�ж��Ƿ��Զ�ɾ�����еĶ���
	//uint8 getAutoDelete() const;

	//һ�㻯����Ӽ��ģ��ʱ��������ɼ���ĸ���ͼԪ�еĶ�̬����
	//�γ�ģ��
	int generalize( QString& msg );
	uint16 keyDeviceType(void) const {return m_keyDevType;};
	QString keyDevicePrefix(void) const {return m_keyDevPrefix;};

protected:
	QString m_keyDevPrefix;		//���豸�����ǰ׺
	uint16 m_keyDevType;		//���豸������
	uint16 m_keyDevVolH;		//���豸��ѹ�ȼ�
	uint16 m_keyDevVolM;
	uint16 m_keyDevVolL;

};
#endif