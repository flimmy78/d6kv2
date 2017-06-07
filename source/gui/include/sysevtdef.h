#ifndef _SYS_EVENT_H
#define _SYS_EVENT_H


/*-------------------------------------------------------------------------*/

//�¼������
enum EventClass
{
	ENTCLASS_NULL = 0,						//0 ��������
	ENTCLASS_DEVICE,						//1 ϵͳ�е��豸�����������¼�
	ENTCLASS_SCADA,							//2 SCADA�¼�
	ENTCLASS_PROTECT,						//3 �ۺ��Զ��������¼�
	ENTCLASS_DAS,							//4 DASӦ���¼�
	ENTCLASS_DA,							//5 DAӦ���¼�
	ENTCLASS_DA_STUDY,						//6 DA�о�̬Ӧ���¼�
	ENTCLASS_EMS,							//7 EMSӦ���¼�
	ENTCLASS_OPERATION,						//8 �����¼�
	ENTCLASS_NOTICE,					    //9 ֪ͨ�¼�
	ENTCLASS_WEB,							//10 WEBӦ���¼�
	ENTCLASS_MAINT							//11 ά����Ӧ���¼�
};

enum EventVerionFlag
{
	NOMAL_EVENT_FLAG = 0,		
	EX_EVENT_FLAG 
};

//�¼����ͺ�
enum EventType
{
	ENTTYPE_NULL = 0,							//������

	//������¼���Ͷ���

	//SCADA����
	ENTTYPE_SCADA = 100,

	//������¼���Ͷ���
	//SCADA����
	ENTTYPE_SCADA_YK = 1,				//1 ң��
	ENTTYPE_SCADA_UPDOWN=5,				//5 ����
	ENTTYPE_SCADA_MANSET=9,				//9 �˹�����
	ENTTYPE_SCADA_SETFLAG,				//10 ���ñ�־
	ENTTYPE_SCADA_BYPASS,				//11 ��·���
 	ENTTYPE_SCADA_LOGIN,				//12 �û���¼���˳�
 	ENTTYPE_SCADA_UPDHISDB,				//13 ��ʷ���޸�
 	ENTTYPE_SCADA_YT,					//14 ң��

	//������ֵ����
	ENTTYPE_RELAY = 200,
    ENTTYPE_RELAY_PRECONFIG,					//201 Ԥ�ö�ֵ
    ENTTYPE_RELAY_CONFIG_CONFIRM,				//202 �ලȷ���޸Ķ�ֵ
    ENTTYPE_RELAY_CONFIG_TAKE,					//203 ִ���޸Ķ�ֵ
    ENTTYPE_RELAY_CONFIG_CANCEL,				//204 �����޸Ķ�ֵ

    ENTTYPE_RELAY_PRECONFIG_NO,					//205 Ԥ�ö�ֵ����
    ENTTYPE_RELAY_CONFIGNO_CONFIRM,				//206 �ලȷ���޸Ķ�ֵ����
    ENTTYPE_RELAY_CONFIGNO_TAKE,				//207 ִ���޸Ķ�ֵ����
    ENTTYPE_RELAY_CONFIGNO_CANCEL,				//208 �����޸Ķ�ֵ����

	//�ļ�����
	ENTTYPE_FILE = 300,
	ENTTYPE_FILE_SAVE,							//301�����ļ�
	ENTTYPE_FILE_DEL,							//302ɾ���ļ�

	//ʵʱ�����
	ENTTYPE_RDB = 400,
	ENTTYPE_RDB_SQL_UPDATE,						//401 ʵʱ��SQL�޸�
	ENTTYPE_RDB_PUT_FIELD,						//402 �޸���
	ENTTYPE_RDB_LOADTABLE,						//403 ���ؿ�
	ENTTYPE_RDB_LOADMODE,						//404 ����ģʽ

	ENTTYPE_SWITCH_HOST = 500,					//�л�

	//ϵͳ����״̬����
	ENTTYPE_SYS = 1000,
	ENTTYPE_SYS_NET,							//1001 ����
	ENTTYPE_SYS_SERVER,							//1002 ������
	ENTTYPE_SYS_PROCESS,						//1003 ����
	ENTTYPE_SYS_RTU,							//1004 RTU
	ENTTYPE_SYS_CH,								//1005 ͨ��
	ENTTYPE_SYS_FTU,							//1006 FTU
	ENTTYPE_SYS_MAIL,							//1007 �ʼ�
	ENTTYPE_SYS_DISK,							//1008 DISK
	ENTTYPE_SYS_CPU,							//1009 CPU
	ENTTYPE_SYS_DBSPACE,						//1010 ���ݿ�����
	ENTTYPE_SYS_PRINTER,						//1011 ��ӡ������
	ENTTYPE_SYS_GROUP_SWITCH,					//1012 ����л�


	//һ��������ң��Խ�ޡ�ң�ű�λ��SOE��
	ENTTYPE_POWERSYS = 2000,
	ENTTYPE_POWERSYS_OVERLIMI,					//2001 YCԽ��
	ENTTYPE_POWERSYS_STATUS,					//2002 ״̬��������λ
	ENTTYPE_POWERSYS_STATUSODD,					//2003 ״̬���쳣��λ
	ENTTYPE_POWERSYS_BREAK,						//2004 ����������λ
	ENTTYPE_POWERSYS_BREAKSG,					//2005 �����¹ʱ�λ
	ENTTYPE_POWERSYS_SWITCH,					//2006 ��բ������λ
	ENTTYPE_POWERSYS_PROTSIG,					//2007 �����źű�λ
	ENTTYPE_POWERSYS_STATUSSOE,					//2008 ״̬��SOE
	ENTTYPE_POWERSYS_BREAKSOE,					//2009 ����SOE
	ENTTYPE_POWERSYS_SWITCHSOE,					//2010 ��բSOE
	ENTTYPE_POWERSYS_PROTSIGSOE,				//2011 �����ź�SOE
	ENTTYPE_POWERSYS_SGOVERLIMIT,				//2012 �¹���բ����Խ��
	ENTTYPE_POWERSYS_MULTSTA,					//2013 ��̬��״̬�仯
	ENTTYPE_POWERSYS_YGSIG,						//2014 Ԥ���źű�λ
	ENTTYPE_POWERSYS_YGSIGSOE,					//2015 Ԥ���ź�SOE
	ENTTYPE_POWERSYS_CHGOVERLIMIT,				//2016 ��λ����Խ��
    ENTTYPE_POWERSYS_SUBSTATION,				//2017 ��վ��������
	ENTTYPE_POWERSYS_YXDOUDONG,					//2018 ң�Ŷ�������Խ��
	ENTTYPE_POWERSYS_WFBS,					    //2019 ���

	//�ۺ��Զ��������¼�
	ENTTYPE_PROTECT = 3000,
	ENTTYPE_PROTECT_EVENT,						//3001 �ۺ��Զ���������Ϣ
	ENTTYPE_PROTECT_DIAGNOSE,					//3002 �ۺ��Զ������������Ϣ 		
	ENTTYPE_RELAYFAULT,							//3003 ��������
    ENTTYPE_RELAYCHKERR,						//3004 �����Լ����
	ENTTYPE_PROTECT_DETECT,						//3005 �������

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////DMS�������Ͷ���/////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//RTU/FTU/DTU���ϼ������

	//�����վ�������Ͻ��
	ENTTYPE_DIAG=4000,                         //�������
	ENTTYPE_DIAG_SGZ,			               //4001 �����¹����ź�/�й���
	ENTTYPE_DIAG_NOFAULT,			           //4002 �޹���
	ENTTYPE_DIAG_INST,			               //4003 ˲�����
	ENTTYPE_DIAG_UNKNOWN_FAULT,			       //4004 �������Ͳ���ȷ
	ENTTYPE_DIAG_FAULT,						   //4005 �й���

	ENTTYPE_DIAG_GND=4010,                     //�ӵ�
	ENTTYPE_DIAG_S_GND,		                   //4011 ����ӵ�
	ENTTYPE_DIAG_S_GND_INST,		           //4012 ����˲ʱ�ӵ�
	ENTTYPE_DIAG_A_GND,		                   //4013 A��ӵ�
	ENTTYPE_DIAG_B_GND,			               //4014 B��ӵ�
	ENTTYPE_DIAG_C_GND,			               //4015 C��ӵ�

	ENTTYPE_DIAG_D_GND,			               //4016 ���ӵ�
	ENTTYPE_DIAG_D_GND_INST,		  	       //4017 ���˲ʱ�ӵ�
	ENTTYPE_DIAG_AB_GND,			           //4018 AB���ӵ�
	ENTTYPE_DIAG_BC_GND,		               //4019 BC���ӵ�
	ENTTYPE_DIAG_CA_GND,			           //4020 CA���ӵ�

	ENTTYPE_DIAG_T_GND,			               //4021 ABC���ӵ�

	ENTTYPE_DIAG_SHORT=4030,                   //��·
	ENTTYPE_DIAG_D_SHORT,				       //4031 ����·
	ENTTYPE_DIAG_D_SHORT_INST,		  	       //4032 ���˲ʱ��·
	ENTTYPE_DIAG_AB_SHORT,			           //4033 AB����·
	ENTTYPE_DIAG_BC_SHORT,			           //4034 BC����·
	ENTTYPE_DIAG_CA_SHORT,			           //4035 CA����·

	ENTTYPE_DIAG_T_SHORT,			           //4036 ABC�����·
	ENTTYPE_DIAG_T_SHORT_INST,		  	       //4037 ���˲ʱ��·

	//FTU��·��������
	ENTTYPE_REP=4100,                           //FTU����
	ENTTYPE_REP_SGZ,							//4101 �����¹����ź�/�й���
	ENTTYPE_REP_NOFAULT,				        //4102 �޹���
	ENTTYPE_REP_INST,			                //4103 ˲�����
	ENTTYPE_REP_UNKNOWN_FAULT,			        //4104 �������Ͳ���ȷ
	ENTTYPE_REP_FAULT,				            //4105 �й���
	
	ENTTYPE_REP_GND=4110,                       //�ӵ�
	ENTTYPE_REP_S_GND,  			            //4111 ����ӵ�
	ENTTYPE_REP_S_GND_INST,		                //4112 ����˲ʱ�ӵ�
	ENTTYPE_REP_A_GND,				            //4113 A��ӵ�
	ENTTYPE_REP_B_GND,			                //4114 B��ӵ�
	ENTTYPE_REP_C_GND,			                //4115 C��ӵ�

	ENTTYPE_REP_SHORT=4120,                     //��·
	ENTTYPE_REP_S_SHORT,		                //4121 �����·
	ENTTYPE_REP_A_SHORT,		                //4122 A���·
	ENTTYPE_REP_B_SHORT,			            //4123 B���·
	ENTTYPE_REP_C_SHORT,			            //4124 C���·
	ENTTYPE_REP_N_SHORT,			            //4125 �����·����ֵ��
	ENTTYPE_REP_D_SHORT,				        //4126 ����·
	ENTTYPE_REP_D_SHORT_INST,			        //4127 ���˲ʱ��·
	ENTTYPE_REP_AB_SHORT,				        //4128 AB����·
	ENTTYPE_REP_BC_SHORT,				        //4129 BC����·
	ENTTYPE_REP_CA_SHORT,				        //4130 CA����·

	ENTTYPE_REP_T_SHORT,		                //4131 ������·
	ENTTYPE_REP_T_SHORT_INST,		            //4132 �����˲ʱ��·

	ENTTYPE_REP_OVER=4140,                      //����
	ENTTYPE_REP_S_OVER,				            //4141 �������
	ENTTYPE_REP_S_OVER_INST,		            //4142 ����˲ʱ����
	ENTTYPE_REP_A_OVER,				            //4143 A�����
	ENTTYPE_REP_B_OVER,				            //4144 B�����
	ENTTYPE_REP_C_OVER,				            //4145 C�����
	ENTTYPE_REP_N_OVER,				            //4146 �������

	ENTTYPE_REP_D_OVER,				            //4147 ������
	ENTTYPE_REP_D_OVER_INST,		            //4148 ���˲ʱ����
	ENTTYPE_REP_AB_OVER,				        //4149 AB������
	ENTTYPE_REP_BC_OVER,				        //4150 BC������
	ENTTYPE_REP_CA_OVER,				        //4151 CA������

	//����ά������
	ENTTYPE_MAINT = 5000,
	ENTTYPE_MAINT_UPLOAD,						//��װ
	ENTTYPE_MAINT_DOWNLOAD,						//��װ
	NOTICETYPE_FRESHCARD  = 20000,              //ˢ����
	//�û��Զ�������
	ENTTYPE_USERDEF = 10000					//�û��Զ�������
};

#endif