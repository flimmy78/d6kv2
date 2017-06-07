#ifndef GGRAPHFILE_H_HEADER_INCLUDED_BCB8BEF8
#define GGRAPHFILE_H_HEADER_INCLUDED_BCB8BEF8
#include "ddef/ddes_comm.h"
#include "GGraphLayer.h"
#include "GVersion.h"
#include "GFillStyle.h"
class GFile;
class GRect;
class GScaleFactor;
class GDC;

//##ModelId=433D33F3000F
//##Documentation
//## ���壺
//##        ͼ�δ洢�ṹ
//## ���ܣ�
//##        ͼ�δ洢�ļ�ͷ�ļ��ṹ
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
class GDE_EXPORT_DES GGraphFile
{
  public:
    //##ModelId=433FE4C90290
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
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
	//##ModelId=433FE4C90290
    //##Documentation
    //## ���ܣ�
    //##      ��ȡͷ�ļ���Ϣ����
    //## ������
    //##      GFile& fFile
    //## ���أ���int32��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 loadHead(GFile& fFile);

    //##ModelId=433FE4CC004E
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

    //##ModelId=434103AC02EE
    //##Documentation
    //## ���ܣ�
    //##       ����/��������    
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-3
    GGraphFile();

    //##ModelId=434103B5005D
    //##Documentation
    //## ���ܣ�
    //##       ����/��������    
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-3
    virtual ~GGraphFile();

     //##ModelId=434716400271
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

    //##ModelId=434716BE03D8
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

    //##ModelId=434717520203
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

    //##ModelId=4347176C0196
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

    //##ModelId=434717ED0203
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

    //##ModelId=4347185F01E4
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

    //##ModelId=4347189003B9
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

    //##ModelId=43471E0F008C
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

	//##ModelId=43471D2001A5
    //##Documentation
    //## ���ܣ�
    //##   ɾ��ɾ�����ͼԪ������ͼԪ����
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
	uint8 removeDels();

    //##ModelId=4347218D02BF
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
    //##ModelId=434DE57801DF
    //##Documentation
    //## ���ܣ�
    //##   ȡ��ͼԪ��̬�ߴ磬���ڴ洢ͼԪ
    //## ������
    //##   ��
    //## ���أ�uint32
    //##   if success return realsize
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    virtual uint32 getSize();
    //##ModelId=435DA05901C7
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      const char* strFile
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
    virtual uint8 readXML(const char* strFile);
	//##ModelId=435DA05901C7
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      const char* strFile
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
    virtual uint8 readHead(const char* strFile);
    //##ModelId=435D9573021B
    //##Documentation
    //## ���ܣ�
    //##      ���ر��溯��
    //## ������
    //##      const char* strFile
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
    virtual uint8 saveXML(const char* strFile);
    //##ModelId=436C5B9002DE
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ���ƹ��ܣ�����ͼԪ��̬ͼ��
    //## ������
    //##   GDC* pDC         //���ƻ���
    //##   GRect* pRect     //��������
    //##   GScaleFactor* pSF//��������
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    void onDraw(GDC* pDC, const GRect& rc , GScaleFactor* pSF = NULL);
	//##ModelId=4372DFFC007D
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
    //##ModelId=4372E03B0280
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
    //##ModelId=4372E06A008C
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
    virtual GGraphLayer* prevLayer(GGraphLayer* pLayer) const;
    //##ModelId=4372E0B9035B
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
    virtual GGraphLayer* nextLayer(GGraphLayer* pLayer) const;
	//##ModelId=4372FEA00167
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
	 //##ModelId=437B1CEC001F
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ��һ��λ��
    //## ������
    //##      ��
    //## ���أ�
    //##      GPOS
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    virtual GPOS firstPos() const;
    //##ModelId=437B1CEC00AB
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ��ǰλ�ö���ǰ������һ������
    //## ������
    //##      GPOS&
    //## ���أ�
    //##      GGraphLayer*
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    virtual GGraphLayer* nextPos(GPOS& gPos) const;
    //##ModelId=437B1CEC0148
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ���һ��λ��
    //## ������
    //##      ��
    //## ���أ�
    //##      GPOS
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    virtual GPOS lastPos() const;
    //##ModelId=437B1CEC01F4
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ��ǰλ�ö���ǰ������һ������
    //## ������
    //##      GPOS&
    //## ���أ�
    //##      GGraphLayer*
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    virtual GGraphLayer* prevPos(GPOS& gPos) const;
    //##ModelId=437B1CEC0290
    //##Documentation
    //## ���ܣ�
    //##      ��ȡָ������λ��
    //## ������
    //##      GGraphLayer* layer
    //## ���أ�
    //##      GPOS
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
    virtual GPOS findPos(GGraphLayer* layer) const;

	//##ModelId=437B1CEC0290
    //##Documentation
    //## ���ܣ�
    //##      ִ�а�ť�궯��
    //## ������
    //##      GBaseObject*
    //## ���أ�
    //##      void
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
    virtual void execBtnMacro(GBaseObject* pObj);

	//##ModelId=437B1CEC0290
    //##Documentation
    //## ���ܣ�
    //##      ִ�а�ť����
    //## ������
    //##      GBaseObject*
    //## ���أ�
    //##      void
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
    virtual void execBtnCMD(GBaseObject* pObj);

	//##ModelId=437B1CEC0290
    //##Documentation
    //## ���ܣ�
    //##      ִ�а�ť�л�������
    //## ������
    //##      GBaseObject*
    //## ���أ�
    //##      void
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
    virtual void execBtnLayer(GBaseObject* pObj);

	//##ModelId=437B1CEC0290
    //##Documentation
    //## ���ܣ�
    //##      ִ�а�ť��ͼ����
    //## ������
    //##      GBaseObject*
    //## ���أ�
    //##      void
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
    virtual void execBtnFile(GBaseObject* pObj);

	//##ModelId=437B1CEC0290
    //##Documentation
    //## ���ܣ�
    //##      ����ͼ��
    //## ������
    //##      GBaseObject*
    //## ���أ�
    //##      void
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
	virtual void update();
	
	//##ModelId=437B1CEC0290
    //##Documentation
    //## ���ܣ�
    //##      ����ָ��ͼԪ��ID��ʶ
    //## ������
    //##      GBaseObject* pObj
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
	void setID(GBaseObject* pObj);

	//##ModelId=437B1CEC0290
    //##Documentation
    //## ���ܣ�
    //##      �����ļ�������ͼԪ��ID��ʶ
    //## ������
    //##      ��
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
	void  resetID();
	
	//##ModelId=437B1CEC0290
    //##Documentation
    //## ���ܣ�
    //##      ����ͼ������ͼԪ
    //## ������
    //##      ��
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
	uint8  travelObj(GTS_FILTER_FUNC filter,void* clientData);

	//##ModelId=437B1CEC0290
    //##Documentation
    //## ���ܣ�
    //##      ����ָ����������ͼԪ
    //## ������
    //##      GGraphLayer* pLayer
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
	uint8  travelObj(GGraphLayer* pLayer,GTS_FILTER_FUNC filter,void* clientData);

	//����ͼ�ΰ�ȫ��
	void setSafeLevel( uint8 level );

	//����ͼ�ΰ�ȫ��
	uint8 safeLevel();

    //##ModelId=433D351E00FA
    //##Documentation
    //## ͼ���ļ���־{bgf}
    char m_gFlag[4];

    //##ModelId=433D3AA80203
    //##Documentation
    //## ���Ź���{0-1:1,1-�Զ�����,2-��ȷ���,3-�߶ȷ���}
    uint8 m_scaleRule;

    //##ModelId=433D3C590157
    //##Documentation
    //## ͼ�η�Χ
    GRect m_boundBox;

	//##ModelId=45060BBF035B
    //##Documentation
    //## ͼ����������
	QString m_owner;
	//##ModelId=45060BDF0242
    //##Documentation
    //## ͼ�����
    QString m_type;
    //##ModelId=433FC5A602DE
    //##Documentation
    //## ͼ���б�
    GListLAYERS m_lstLayers;
    //##ModelId=433D37190271
    //##Documentation
    //## �汾������
    GVersion m_vers;
    //##ModelId=436C62D302BF
    //##Documentation
    //## ���������
    GFillStyle m_fillStyle;
	//##ModelId=43812B3E01E4
    //##Documentation
    //## ɾ��ͼ����������洢
    uint32 m_cntDel;


	//ͼ�εİ�ȫ��
	uint8 m_SafeLevel;

	//##ModelId=437B1CEC0290
    //##Documentation
    //## ���ܣ�
    //##      �ж��ļ���ʽ
    //## ������
    //##      const char* strPath
    //## ���أ�
    //##     
	//##	GT_FAILED		0x00
    //##	GT_BINGRAPH		0x01
    //##	GT_XMLGRAPH		0x02
	//##
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
	static uint8 isGraphFile(const char* strPath) ;

  protected:
    //##ModelId=434718D80251
    //##Documentation
    //## ����ʱʹ�ã��ṩ��ǰ�����
    GGraphLayer* m_curLayer;
	//##ModelId=4372FE6F0138
    //##Documentation
    //## ��ǰ����ʶ
    uint32 m_maxID;
  private:


};

#endif /* GGRAPHFILE_H_HEADER_INCLUDED_BCB8BEF8 */
