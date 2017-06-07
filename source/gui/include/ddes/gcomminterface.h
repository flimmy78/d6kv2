#ifndef _GCOMM_INTERFACE_H
#define _GCOMM_INTERFACE_H

#include "gcommfunc.h"
class GDC;

//��ɫת��
#define G2Q_RGB(rgb) convG2Q_RGB(rgb)
#define Q2G_RGB(rgb) convQ2G_RGB(rgb)
#define G2Q_CLR(rgb) convG2Q_CLR(rgb)
#define Q2G_CLR(rgb) convQ2G_GLR(rgb)
#define GPL2QPA(pl,pa) convGPL_QPA(pl,pa)
#define QPA2GPL(pa,pl) convQPA_GPL(pa,pl)

//·������
#define DEFAULT_ENV			getDefaultEnv()
#define DEFAULT_PATH		getDefaultPath()
#define DEFAULT_GRAPH_PATH	getDefaultGraphPath()
#define DEFAULT_PLUGIN_PATH getDefaultPluginPath()
#define DEFAULT_CUSTOM_PATH getDefaultCustomPath()
#define DEFAULT_SYMBOL_PATH getDefaultSymbolPath()
#define DEFAULT_EXEC_PATH	getDefaultExecPath()
#define DEFAULT_INI_PATH	getDefaultIniPath()

GDE_EXPORT_DES uint8	existFilePath(const QString& strPath);
GDE_EXPORT_DES void		createDirectory(const QString& strPath);
//���·������
//���û�ȡ��������
GDE_EXPORT_DES void    setDefaultEnv(const char* strEnv);
GDE_EXPORT_DES const char*   getDefaultEnv();

//��ʼ�����·��
GDE_EXPORT_DES void    initDefaultPath();
GDE_EXPORT_DES uint8   isInitDefaultPath();

//��ȡ���·��
GDE_EXPORT_DES const QString getDefaultPath();
GDE_EXPORT_DES const QString getDefaultGraphPath();
GDE_EXPORT_DES const QString getDefaultPluginPath();
GDE_EXPORT_DES const QString getDefaultCustomPath();
GDE_EXPORT_DES const QString getDefaultSymbolPath();
GDE_EXPORT_DES const QString getDefaultExecPath();
GDE_EXPORT_DES const QString getDefaultIniPath();

//�����ͷſ���ʱ�䴦��ˢ��
GDE_EXPORT_DES void	QProcessEvents();

//���˼��ͼԪ�Ƿ���������
typedef uint8 (*GTS_FILTER_FUNC)(GBaseObject *pObj,void* clientData);

#define DLGF_MST_INFO		0x00
#define DLGF_MST_DESC		0x01
#define DLGF_MST_WARNING	0x02
#define DLGF_MST_ERROR		0x03
#define DLGF_MST_STATUS		0x04

//���ع��̴���ص������ڶ�̬��ʾ���ع���
typedef void (*DYNLOAD_GRAPH_FUNC)(const char* msg,uint32 msgtype,uint32 n,uint32 total);

//���ö�̬��ʾ���ع���
GDE_EXPORT_DES void setDynLoadFunc(DYNLOAD_GRAPH_FUNC dynLoad);

//��ȡ��̬���ع���
GDE_EXPORT_DES DYNLOAD_GRAPH_FUNC getDynLoadFunc();

//���ö�̬���ع���
GDE_EXPORT_DES void dynLoadFunc(const char* msg="",uint32 nsgtype=DLGF_MST_STATUS,uint32 n=0,uint32 total=0);	

//ͼԪ��ɫ
GDE_EXPORT_DES void setObjectColor(GBaseObject* pObj,uint32 color);

//��ѹ��ͼԪ��ɫ
GDE_EXPORT_DES void	setObjectColor(GBaseObject* pObj,uint32 color,int32 sno);

//��ȡ�豸����
GDE_EXPORT_DES GGraphDynData* getDevPara(GBaseObject* pObj);

//��ȡʵʱ����
GDE_EXPORT_DES GGraphDynData* getRealPara(GBaseObject* pObj);

//��ȡ��������������Ͷ�Ӧֵ
GDE_EXPORT_DES uint8 getDynDataPara(GGraphDynData* pData,int32 type,QString& value);

//������������������Ͷ�Ӧֵ
GDE_EXPORT_DES uint8 setDynDataPara(GGraphDynData* pData,int32 type,const QString& value);

//����ͼԪ�����¼�
GDE_EXPORT_DES int32 receiveMouseEvent(GBaseObject* pObject,GDC* pDC , const GPoint& pt,uint8 lbtn , uint8 down , uint32 uflags );

#endif
