/*
 * --------------------------------------------------
 *   �ļ�����webconstant.h
 *   �������DS3210�������ϵͳWeb������   
 *   �ļ���Ҫ���ݣ�WEB�������ػ��߳�
 *
 *   Copyright ��������������ҵ��, 2002
 *   �汾�ţ�v1.0.0,	2002/06/03
 *					 
 *   ��ƣ�����ͼ
 *   ˵����
 * --------------------------------------------------
 */


#if !defined(WEBCONSTANT)
#define WEBCONSTANT

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "db/dbapi.h"

#define		WEBPORT			7200	//�������˿ں�
#define		MAXBUFSIZE		80*1024   /*��ģ������64K debug*/
#define		WEBNODENAMELEN	40		/*������ڵ����Ƴ���*/
#define		INITCLIENTNUMS	100		//��ʼ�Ŀͻ�������
#define		CLIENTSTOPTIME	90		//50		//�ͻ����˳�ʱ�䣬��
#define		CODESIZE		256		//���볤��
#define		MSGHEADLEN		12		/*MESSAGEͷ�ĳ���*/
#define     DEVTYPENUM		32     //�豸������
#define		INVALIDF		-1	//THREADMANAGE��validf��Ч

/*Win32 begin*/
#ifdef WIN32
	#define		EWOULDBLOCK		WSAEWOULDBLOCK
	#define		EISCONN			WSAEISCONN
	#define		EINPROGRESS		WSAEINPROGRESS

	#ifndef		EINTR
		#define		EINTR		WSAEINTR
	#endif

#endif
/*Win32 end*/

//������ʷ���Ƿ�Ϊ�ձ�
//#define		USE_YC_DAYTABLE  


//C/Sͨ�ű������Ͷ���
#define		S_SUCCESS		8			//ע��ɹ�
#define		S_FAILLOG		9			//�ͻ���¼ʧ��
#define		MSGTYPE			10			//֡����
#define     S_STREAM		MSGTYPE+0   //������
#define		S_HISEVENT		MSGTYPE+2	//��ʷ����
#define		S_REALEVENT		MSGTYPE+3	//ʵʱ�����
#define		S_DEVPARA		MSGTYPE+4	//�豸����
#define     S_TOPOLOGY      MSGTYPE+8   //���˽ṹ��ʵʱ�⣩


#define     S_GROUPTABLE    20			//�������
#define		S_RTUNAME		MSGTYPE+11	//�ն˲�����
#define		S_YCCODE		22			//ң�����
#define     S_CURVE         23			//��������(���߲鿴��)
#define		S_DDCODE        24			//��ȴ���
#define		S_CARDRCD		25			//��־�Ƽ�¼��
#define 	S_REALCURVE		26			//ʵʱ������������
#define		S_FILE			MSGTYPE+20	//���ļ�
#define		S_ALLFILE		MSGTYPE+21	//��ȫ���ļ�
#define		S_USER			50			//���û���
#define		S_GRAPHGROUP    51			//webͼ�η����
#define     S_USERRIGHT		52			//webȨ�޲�����
#define     S_TOPOCOLOR     53			//topocolor��
#define     S_STATION	    54          //վ����ϵ������
#define     S_ANALOGVIEW    55          //ģ������ͼ

#define		S_TEST			66			//���Ա���
#define		S_ACK			99			//�ͻ���(socket)ƽ������
#define		S_TYPE			100			//�ͻ�������
#define		S_SRVEXIT		199			//�������ر�
#define     S_ERROR			200       	//�������


#define		DAYYCCURVE		1			//��ң������
#define		MONTHYCCURVE	2			//��ң������
#define		DAYKWHCURVE		10			//�յ������
#define		MONTHKWHCURVE	11			//�µ������
#define     REALYCCURVE		0			//ʵʱң������
#define     REALKWHCURVE	1			//ʵʱ�������



#define		USERNAME		"600268"//"scada"		//ʵʱ������û���
#define		PASSWORD		""//"scada"		//ʵʱ������û���
#define		BASEWINNO		INITCLIENTNUMS  //������ʱ��winno = BASEWINNO*threadno +winno,=INITCLIENTNUMS
#define     WAIT_TIME       40		 	//ֹͣ���󣬵ȴ�ʱ�䡣�ȴ������ݴ����̣߳������ڴ��ͻ,���Ըĳ�20

#define		WEBSCADA		1			//�û�������webscada
#define		GISTOWEB		10			//gis�û�
#define		WEBTABLE		100			//web table


#define		WEBVISITTABLE   "webͼ�η��ʼ�¼��"
#define     WEBUPDATETABLE  "web�ͻ������ļ���"
#define		YCPARATABLE		"ң�������"	//ң�������
#define     TOPOTABLE       "SCADA��̬���˽ṹ��¼��"
#define     TOPOCOLORTABLE  "topocolor"


#define		CARDRCD			"cardrcd"	//��־�Ƽ�¼��
#define	    STATION			"station"	//վ����ϵ������
#define     TOPO            "scdtp"     //SCADA��̬���˽ṹ��¼��

#define		MAXLOOPNUMS      100000		//���ѭ����

#define MAXSQLWORDLEN		1024
#define MAXFIELDWIDTH		256//Sybase�������

 


//�����豸���ͺ�,�ɵ���Ա���涨��
#define DevNULL			0	//--�ǵ����豸
#define DevBreak		1	//--����
#define DevSwitch		2	//--��բ
#define DevMelt			3	//--�۶���
#define DevHanbrk		4	//--�ֳ�
#define DevMTran		5	//--����ѹ��
#define DevDTran		6	//--����ѹ��
#define DevCap			7	//--������
#define DevReact		8	//--�翹
#define DevLoop			9	//--������Ȧ
#define DevPT			10	//--PT
#define DevCT			11	//--CT
#define DevArres		12	//--������
#define DevGround		13	//--�ӵ���
#define DevPowln		14	//--�ܿ���
#define DevCable		15	//--����
#define DevBus			16	//--ĸ��
#define DevGener		17	//--�����
#define DevWeld			18	//--����
#define DevLnkln		19	//--������
#define DevJoint		20	//--����
#define DevMaxType		21	//--�����豸������ͺ�


#define FREE(p) if(p) { \
				 free(p); \
				 p = NULL;	\
				}

#define DELETEP(p) if(p) { \
				 delete (p); \
				 p = NULL;	\
				}


#ifndef WIN32
	#include <assert.h>
	#define ASSERT assert

	#define FALSE   0
	#define TRUE    1
#ifndef _AIX
	#define NULL    0
#endif
	typedef unsigned char byte;
	typedef unsigned int  UINT;
	typedef  void *(*PTHREAD_CALLFUNC)(void *);
#endif


#define TOPU_MAX_COUNT						512                   //MAX COUNT 
#define CHAR_BUF_LEN_DEF					CODE_LEN



#define  DEV_MEAS_TABNAME                    "analog"
#define  DEV_BREAK_TABNAME                   "break"
#define  DEV_SWITCH_TABNAME                  "switch" 
	
	
	
#define DEV_FRAME_STYLE_HYPERLINK           1
#define DEV_FRAME_STYLE_DEVPROP             2
	
#define DEV_LINE_HYPERLINK                  "LineHyperlink"
	
	
#define DEV_MEAS							"measlayer"
	//����
#define DEV_BREAK							"breakerlayer"    
#define DEV_SWITCH	    					"switchlayer"
#define DEV_LINE							"aclinelayer"
	
#define DEV_BUS								"buslayer"
#define DEV_TRANSFORMER3LAYER	            "transformer3layer"
#define DEV_SUBTRANSFORMER					"transformer2layer"
	
#define DEV_MLBREAK							"mcblayer"
#define DEV_LOAD							"loadslayer"
#define DEV_CAPACITOR						"capacitorlayer"
	
#define DEV_COIL							"tgrlayer"
#define DEV_GNDSWITCH						"tgslayer"
	
#define HYPERLINK_MAX_FILENAME              10
#define CHAR_BUF_LEN						255
	//�����׺����
#define BREAK_POSTFIX						"Kg"
#define SWITCH_POSTFIX						"Dz"


#endif // !defined(WEBCONSTANT)

