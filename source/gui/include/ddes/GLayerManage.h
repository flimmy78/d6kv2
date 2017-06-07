#ifndef GLAYERMANAGE_H_HEADER_INCLUDED_BC86EA29
#define GLAYERMANAGE_H_HEADER_INCLUDED_BC86EA29
#include "GGraphFile.h"
#include "ddef/ddes_comm.h"
class GGraphLayer;
class GScaleFactor;

//##ModelId=43797F5602BB
//##Documentation
//## ���壺
//##        ͼ�������
//## ���ܣ�
//##        ���ڹ���ͼ�㣬����ͼ��
//##        ��ӣ�ɾ���ȵ�������ָ���
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-15
class GDE_EXPORT_DES GLayerManage : public GGraphFile
{
  public:
    //##ModelId=4379808902CA
    //##Documentation
    //## ���ܣ�
    //##     ����/��������    
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
    GLayerManage();
    //##ModelId=4379809C0337
    //##Documentation
    //## ���ܣ�
    //##     ����/��������    
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
    ~GLayerManage();
    //##ModelId=437982ED01CE
    //##Documentation
    //## ���ܣ�
    //##   ���һ��
    //## ������
    //##   GGraphLayer* pLayer//ָ����
    //## ���أ�(GPOS)
    //##   if success return GPOS
    //##   else return NULL
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    virtual GPOS addLayer(GGraphLayer* pLayer);
    //##ModelId=437982ED025B
    //##Documentation
    //## ���ܣ�
    //##   ɾ��ָ���㣬�������а���������
    //## ������
    //##   GGraphLayer* pLayer //ָ����
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    virtual uint8 delLayer(GGraphLayer* pLayer);
    //##ModelId=437982ED02C8
    //##Documentation
    //## ���ܣ�
    //##   ������λ��
    //## ������
    //##   GGraphLayer* pLayer//����ָ����λ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    virtual uint8 upLayer(GGraphLayer* pLayer);
    //##ModelId=437982ED0345
    //##Documentation
    //## ���ܣ�
    //##   ������λ�õ�����
    //## ������
    //##   GGraphLayer* pLayer//����ָ����λ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-10
    virtual uint8 topLayer(GGraphLayer* pLayer);
    //##ModelId=437982ED03C2
    //##Documentation
    //## ���ܣ�
    //##   ���Ͳ�λ��
    //## ������
    //##   GGraphLayer* pLayer//����ָ����λ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    virtual uint8 downLayer(GGraphLayer* pLayer);
    //##ModelId=437982EE0057
    //##Documentation
    //## ���ܣ�
    //##   ���Ͳ�λ�õ����²�
    //## ������
    //##   GGraphLayer* pLayer//����ָ����λ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    virtual uint8 bottomLayer(GGraphLayer* pLayer);
    //##ModelId=437982EE00E4
    //##Documentation
    //## ���ܣ�
    //##   ������λ��
    //## ������
    //##   GGraphLayer* pLayer1//������λ��
    //##   GGraphLayer* pLayer2//������λ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    virtual uint8 chgLayer(GGraphLayer* pLayer1, GGraphLayer* pLayer2);
    //##ModelId=437982EE0171
    //##Documentation
    //## ���ܣ�
    //##   ȡ��ǰ��
    //## ������
    //##   ��
    //## ���أ�
    //##   GGraphLayer*
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    virtual GGraphLayer* getLayer();
    //##ModelId=437982EE01FD
    //##Documentation
    //## ���ܣ�
    //##   ���õ�ǰ��
    //## ������
    //##   GGraphLayer* pLayer //ָ����ǰ��
    //## ���أ�  
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    virtual void setLayer(GGraphLayer* pLayer);
    //##ModelId=437982EE027A
    //##Documentation
    //## ���ܣ�
    //##   ɾ�����в㣬����������
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    virtual uint8 remove();
    //##ModelId=437982EE02F7
    //##Documentation
    //## ���ܣ�
    //##       �ϲ�ɾ����ͼԪ��ָ����
    //## ������
    //##      GGraphLayer* pLayer
    //##      GGraphLayer* pRemove
    //## ���أ���uint8 ��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    virtual uint8 mergeLayer(GGraphLayer* pLayer, GGraphLayer* pRemove);
    //##ModelId=437982EE0384
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ���ƹ��ܣ�����ͼԪ��̬ͼ��
    //## ������
    //##   GDC* pDC         //���ƻ���
    //##   const GRect& rc  //��������
    //##   GScaleFactor* pSF//��������
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    virtual void onDraw(GDC* pDC, const GRect& rc, GScaleFactor* pSF = NULL);
    //##ModelId=437982EF0019
    //##Documentation
    //## ���ܣ�
    //##   ȡָ����ǰһ��
    //## ������
    //##   GGraphLayer* pLayer//����ָ����λ��
    //## ���أ�
    //##   if success return Layer
    //##   else return NULL
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    virtual GGraphLayer* prevLayer(GGraphLayer* pLayer);
    //##ModelId=437982EF00A5
    //##Documentation
    //## ���ܣ�
    //##   ȡָ�����һ��
    //## ������
    //##   GGraphLayer* pLayer//����ָ����λ��
    //## ���أ�
    //##   if success return Layer
    //##   else return NULL
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-10
    virtual GGraphLayer* nextLayer(GGraphLayer* pLayer);
    //##ModelId=437982EF0132
    //##Documentation
    //## ���ܣ�
    //##   ��������ʶ
    //## ������
    //##   ��
    //## ���أ�
    //##   uint32
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-10
    virtual uint32 maxID();
    //##ModelId=4379831401AF
    //##Documentation
    //## ���ܣ�
    //##      ���������ĵ�
    //## ������
    //##      GGraphFile* gFile//ָ�������ļ�
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    void setFile(GGraphFile* gFile);
    //##ModelId=4379833800C4
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ�����ĵ�
    //## ������
    //##      ��
    //## ���أ�
    //##      GGraphFile*
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    GGraphFile* getFile() const;
    //##ModelId=43798563021A
    //##Documentation
    //## ���ܣ�
    //##   �����Զ�ɾ�����еĶ���
    //## ������
    //##   uint8 bAutoDelete
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-14
    void setAutoDelete(uint8 bAutoDelete);
    //##ModelId=4379856302B7
    //##Documentation
    //## ���ܣ�
    //##  �ж��Ƿ��Զ�ɾ�����еĶ���
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1 
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-14
    uint8 getAutoDelete() const;
  protected:
    //##ModelId=4379856B03A1
    uint8 m_bAutoDelete;
    //##ModelId=437982F4028A
    GGraphFile* m_graphFile;
};

#endif /* GLAYERMANAGE_H_HEADER_INCLUDED_BC86EA29 */
