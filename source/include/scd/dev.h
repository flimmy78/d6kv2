#ifndef _dev_objname_h
#define _dev_objname_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "devdef.h"
#include "rdb/rdbdef.h"


//////////////// ϵͳ���ô��붨�� //////////////////

#define  SYS_SUBST_NAME		"sys"

#define  SYS_P_NAME			"SsystotalP"
#define  SYS_Q_NAME			"SsystotalQ"
#define  SYS_HZ_NAME		"SsysPl"	

#define  SYS_MPKWH_NAME		"SsystotalKWH"

#define  SYS_LAST_TIME		"SsysstopTime"


//////////////// �������ͷ��෽������ /////////////

#define OBJTPMODE_SCDTAB				0	//��SCADA�����

//////////////// �������������  ////////////////////

#define  OBJTPCLASS_NULL			   -1	//�����

//��SCADA�����
#define  OBJTPCLASS_SCDTAB_ANALOG		1	//ģ������
#define  OBJTPCLASS_SCDTAB_KWH			2	//�����
#define  OBJTPCLASS_SCDTAB_BREAK		3	//������
#define  OBJTPCLASS_SCDTAB_SWITCH		4	//��բ��
#define  OBJTPCLASS_SCDTAB_PROTSIG		5	//�����ź���
#define  OBJTPCLASS_SCDTAB_STATUS		6	//״̬����
#define  OBJTPCLASS_SCDTAB_DMSSTATUS	7	//�����ϼ��״̬����

//////////////// �������ʹ��붨�� //////////////////

#define  OBJTYPENM_I				   "I"	//����
#define  OBJTYPENM_Ia				   "Ia"	//A�����
#define  OBJTYPENM_Ib				   "Ib"	//B�����
#define  OBJTYPENM_Ic				   "Ic"	//C�����
#define  OBJTYPENM_Iab				   "Id"	//AB�����
#define  OBJTYPENM_Ibc			       "Ie"	//BC�����
#define  OBJTYPENM_Ica				   "If"	//CA�����

#define  OBJTYPENM_I1				   "I1"	//�ߵ���
#define  OBJTYPENM_I2				   "I2"	//�е���
#define  OBJTYPENM_I3				   "I3"	//�͵���
#define  OBJTYPENM_Ia1				   "Ia1"	//A��ߵ���
#define  OBJTYPENM_Ib1				   "Ib1"	//B��ߵ���
#define  OBJTYPENM_Ic1				   "Ic1"	//C��ߵ���
#define  OBJTYPENM_Ia2				   "Ia2"	//A���е���
#define  OBJTYPENM_Ib2				   "Ib2"	//B���е���
#define  OBJTYPENM_Ic2				   "Ic2"	//C���е���
#define  OBJTYPENM_Ia3				   "Ia3"	//A��͵���
#define  OBJTYPENM_Ib3				   "Ib3"	//B��͵���
#define  OBJTYPENM_Ic3				   "Ic3"	//C��͵���

#define  OBJTYPENM_I00				   "I00"	//�������
#define  OBJTYPENM_I11				   "I11"	//�������
#define  OBJTYPENM_I22				   "I22"	//�������


#define  OBJTYPENM_P				   "P"	//�й�
#define  OBJTYPENM_Pa				   "Pa"	//A���й�
#define  OBJTYPENM_Pb				   "Pb"	//B���й�
#define  OBJTYPENM_Pc				   "Pc"	//C���й�
#define  OBJTYPENM_Pab				   "Pd"	//AB���й�
#define  OBJTYPENM_Pbc				   "Pe"	//BC���й�
#define  OBJTYPENM_Pca				   "Pf"	//CA���й�

#define  OBJTYPENM_P1				   "P1"	//���й�
#define  OBJTYPENM_P2				   "P2"	//���й�
#define  OBJTYPENM_P3				   "P3"	//���й�
#define  OBJTYPENM_Pa1				   "Pa1"//A����й�
#define  OBJTYPENM_Pa2				   "Pa1"//A�����й�
#define  OBJTYPENM_Pa3				   "Pa1"//A����й�
#define  OBJTYPENM_Pb1				   "Pb1"//B����й�
#define  OBJTYPENM_Pb2				   "Pb2"//B�����й�
#define  OBJTYPENM_Pb3				   "Pb3"//B����й�
#define  OBJTYPENM_Pc1				   "Pc1"//C����й�
#define  OBJTYPENM_Pc2				   "Pc2"//C�����й�
#define  OBJTYPENM_Pc3				   "Pc3"//C����й�


#define  OBJTYPENM_Q				   "Q"	//�޹�
#define  OBJTYPENM_Qa				   "Qa"	//A���޹�
#define  OBJTYPENM_Qb				   "Qb"	//B���޹�
#define  OBJTYPENM_Qc				   "Qc"	//C���޹�
#define  OBJTYPENM_Qab			       "Qd"	//AB���޹�
#define  OBJTYPENM_Qbc				   "Qe"	//BC���޹�
#define  OBJTYPENM_Qca				   "Qf"	//CA���޹�

#define  OBJTYPENM_Q1				   "Q1"	//���޹�
#define  OBJTYPENM_Q2				   "Q2"	//���޹�
#define  OBJTYPENM_Q3				   "Q3"	//���޹�
#define  OBJTYPENM_Qa1				   "Qa1"	//A����޹�
#define  OBJTYPENM_Qa2				   "Qa2"	//A�����޹�
#define  OBJTYPENM_Qa3				   "Qa3"	//A����޹�
#define  OBJTYPENM_Qb1				   "Qb1"	//B����޹�
#define  OBJTYPENM_Qb2				   "Qb2"	//B�����޹�
#define  OBJTYPENM_Qb3				   "Qb3"	//B����޹�
#define  OBJTYPENM_Qc1				   "Qc1"	//C����޹�
#define  OBJTYPENM_Qc2				   "Qc2"	//C�����޹�
#define  OBJTYPENM_Qc3				   "Qc3"	//C����޹�



#define  OBJTYPENM_S				   "S"	//���ڹ���
#define  OBJTYPENM_COSYC			   "Cs"	//���ʣ�����������

#define  OBJTYPENM_ANGLE			   "Jd"	//�Ƕ�
#define  OBJTYPENM_HZ				   "Pl"	//Ƶ��
#define  OBJTYPENM_TEMPERATURE		   "Wd"	//�¶�
#define  OBJTYPENM_PRESS			   "Yl"	//ѹ��
#define  OBJTYPENM_WATER			   "Sw"	//ˮλ
#define  OBJTYPENM_FLOW				   "Ll"	//����
#define  OBJTYPENM_HUMIDITY			   "Sd"	//ʪ��
#define  OBJTYPENM_DW				   "Dw"	//��ѹ����λ
#define  OBJTYPENM_YC				   "Yc"	//����ң��


#define  OBJTYPENM_V				   "V"	//��ѹ
#define  OBJTYPENM_Va			  	   "Va" //A���ѹ
#define  OBJTYPENM_Vb				   "Vb" //B���ѹ
#define  OBJTYPENM_Vc				   "Vc" //C���ѹ
#define  OBJTYPENM_Vab				   "Vd" //AB���ѹ
#define  OBJTYPENM_Vbc				   "Ve" //BC���ѹ
#define  OBJTYPENM_Vca				   "Vf" //CA���ѹ
#define  OBJTYPENM_V0				   "V0" //�����ѹ
#define  OBJTYPENM_V1				   "V1" //�����ѹ
#define  OBJTYPENM_V2				   "V2" //�����ѹ


#define  OBJTYPENM_PKWH				   "Wh"	//�й����
#define  OBJTYPENM_PosPKWH			   "Wp"	//�����й����
#define  OBJTYPENM_NegPKWH			   "Wn"	//�����й����
#define  OBJTYPENM_QKWH				   "Vh"	//�޹����
#define  OBJTYPENM_PosQKWH			   "Vp"	//�����޹����
#define  OBJTYPENM_NegQKWH			   "Vn"	//�����޹����
#define  OBJTYPENM_RATE				   "Rt"	//�����ۻ���

#define  OBJTYPENM_BREAK			   "Kg"	//����״̬
#define  OBJTYPENM_SWITCH			   "Dz"	//��բ״̬
#define  OBJTYPENM_SGZ				   "Sg"	//�¹���
#define  OBJTYPENM_YG				   "Yg"	//Ԥ���ź�
#define  OBJTYPENM_PROTSIG			   "Bh"	//�����ź�
#define  OBJTYPENM_STATUS			   "Zt"	//����״̬��

#define  OBJTYPENM_DEVICE			   "Dv"	//�豸����


////////////////////////// ���������Ͷ��� ////////////////////////////////


//ң�����ͱ��붨��
#define		OBJTYPE_I					1	//����
#define		OBJTYPE_Ia					2	//A�����
#define		OBJTYPE_Ib					3	//B�����
#define		OBJTYPE_Ic					4	//C�����
#define     OBJTYPE_Iab					5   //AB�����
#define     OBJTYPE_Ibc					6   //BC�����
#define     OBJTYPE_Ica					7   //CA�����
#define		OBJTYPE_I00					8	//�������
#define     OBJTYPE_I11					9   //�������
#define     OBJTYPE_I22					10  //�������

#define		OBJTYPE_P					20	//�й�����
#define		OBJTYPE_Pa					21  //A���й�
#define		OBJTYPE_Pb					22  //B���й�
#define		OBJTYPE_Pc					23  //C���й�
#define		OBJTYPE_Pab					24  //AB���й�
#define		OBJTYPE_Pbc					25  //BC���й�
#define		OBJTYPE_Pca					26  //CA���й�

#define		OBJTYPE_Q					40	//�޹�����
#define		OBJTYPE_Qa					41  //A���޹�
#define		OBJTYPE_Qb					42  //B���޹�
#define		OBJTYPE_Qc					43  //C���޹�
#define		OBJTYPE_Qab					44  //AB���޹�
#define		OBJTYPE_Qbc					45  //BC���޹�
#define		OBJTYPE_Qca					46  //CA���޹�

#define		OBJTYPE_S					60	//���ڹ���
#define		OBJTYPE_COSYC				61	//���ʣ�����������

#define		OBJTYPE_I1					62  //�ߵ���
#define		OBJTYPE_I2					63	//�е���
#define		OBJTYPE_I3				    64	//�͵���
#define		OBJTYPE_Ia1					65	//A��ߵ���
#define		OBJTYPE_Ia2					66	//A���е���
#define		OBJTYPE_Ia3					67	//A��͵���
#define		OBJTYPE_Ib1					68	//B��ߵ���
#define		OBJTYPE_Ib2					69	//B���е���
#define		OBJTYPE_Ib3					70	//B��͵���
#define		OBJTYPE_Ic1					71	//C��ߵ���
#define		OBJTYPE_Ic2					72	//C���е���
#define		OBJTYPE_Ic3					73	//C��͵���

#define		OBJTYPE_P1					74	//���й�����
#define		OBJTYPE_P2					75	//���й�����
#define		OBJTYPE_P3					76	//���й�����
#define		OBJTYPE_Pa1					77  //A����й�
#define		OBJTYPE_Pa2					78  //A�����й�
#define		OBJTYPE_Pa3					79  //A����й�
#define		OBJTYPE_Pb1					80  //B����й�
#define		OBJTYPE_Pb2					81  //B�����й�
#define		OBJTYPE_Pb3					82  //B����й�
#define		OBJTYPE_Pc1					83  //C����й�
#define		OBJTYPE_Pc2					84  //C�����й�
#define		OBJTYPE_Pc3					85  //C����й�

#define		OBJTYPE_Q1					86	//���޹�����
#define		OBJTYPE_Q2					87	//���޹�����
#define		OBJTYPE_Q3					88	//���޹�����
#define		OBJTYPE_Qa1					89  //A����޹�
#define		OBJTYPE_Qa2					90  //A�����޹�
#define		OBJTYPE_Qa3					91  //A����޹�
#define		OBJTYPE_Qb1					92  //B����޹�
#define		OBJTYPE_Qb2					93  //B�����޹�
#define		OBJTYPE_Qb3					94  //B����޹�
#define		OBJTYPE_Qc1					95  //C����޹�
#define		OBJTYPE_Qc2					96  //C�����޹�
#define		OBJTYPE_Qc3					97  //C����޹�


#define		OBJTYPE_ANGLE				100	//�Ƕ�
#define		OBJTYPE_HZ					101	//Ƶ��
#define		OBJTYPE_TEMPERATURE			102	//�¶�
#define		OBJTYPE_PRESS				103	//ѹ��
#define		OBJTYPE_WATER				104	//ˮλ
#define		OBJTYPE_FLOW				105	//����	 
#define		OBJTYPE_HUMIDITY			106	//ʪ��
#define		OBJTYPE_DW					107	//��ѹ����λ

#define		OBJTYPE_YC					127	//����ң��

#define		OBJTYPE_V					150	//��ѹ
#define		OBJTYPE_Va					151	//A���ѹ
#define		OBJTYPE_Vb					152	//B���ѹ
#define		OBJTYPE_Vc					153	//C���ѹ
#define		OBJTYPE_Vab					154	//AB�ߵ�ѹ
#define		OBJTYPE_Vbc					155	//BC�ߵ�ѹ
#define		OBJTYPE_Vca					156	//CA�ߵ�ѹ
#define		OBJTYPE_V0					157	//�����ѹ
#define     OBJTYPE_V1					158 //�����ѹ
#define     OBJTYPE_V2					159 //�����ѹ


//������ͱ��붨��
#define		OBJTYPE_PKWH				200	//�й����
#define		OBJTYPE_QKWH			    201	//�޹����
#define		OBJTYPE_PosPKWH				202	//���й����
#define		OBJTYPE_NegPKWH				203	//���й����
#define		OBJTYPE_PosQKWH				204	//���޹����
#define		OBJTYPE_NegQKWH				205	//���޹����
#define		OBJTYPE_RATE				206 //�����ۻ���


//ң�����ͱ��붨��
#define		OBJTYPE_BREAK				300	//����
#define		OBJTYPE_BRK_CAR				301	//С������
#define		OBJTYPE_BRK_BUSLKCAR		302 //ĸ��С������
#define		OBJTYPE_BRK_BUSLINK			303 //ĸ������
#define		OBJTYPE_BRK_BYBUS			304 //��ĸ����
#define		OBJTYPE_BRK_BYBUSLK			305 //��ĸ��ĸ������

#define		OBJTYPE_STATUS				400	//״̬

#define		OBJTYPE_SWITCH				500	//��բ:������ʹ������

#define		OBJTYPE_SW_LINE				501	//��·���뵶
#define		OBJTYPE_SW_B1				502	//Iĸ���뵶
#define		OBJTYPE_SW_B2				503	//IIĸ���뵶
#define		OBJTYPE_SW_B3				504	//IIIĸ���뵶
#define		OBJTYPE_SW_B4				505	//IV ĸ���뵶
#define		OBJTYPE_SW_BYB				506 //��ĸ���뵶
#define		OBJTYPE_SW_TRAN				507 //���䵶բ  
#define		OBJTYPE_SW_STRAN			508 //վ�䵶բ
#define		OBJTYPE_SW_PT				509	//PT��բ
#define		OBJTYPE_SW_BLINK			510	//ĸ����բ

#define		OBJTYPE_SW_L_GRD			541	//��·�ӵص�
#define		OBJTYPE_SW_TRAN_GRD			542	//����ӵص�
#define		OBJTYPE_SW_STRAN_GRD		543	//վ��ӵص�
#define		OBJTYPE_SW_PT_GRD			544	//PT�ӵص�

#define		OBJTYPE_SW_B1_GRD			571	//Iĸ�ӵص�
#define		OBJTYPE_SW_B2_GRD           572	//IIĸ�ӵص�
#define		OBJTYPE_SW_B3_GRD           573	//IIIĸ�ӵص�
#define		OBJTYPE_SW_B4_GRD           574	//IV ĸ�ӵص�
#define		OBJTYPE_SW_BYB_GRD          575 // ��ĸ�ӵص�

#define		OBJTYPE_PROTSIG				600	//����
#define		OBJTYPE_SGZ					601	//�¹���
#define		OBJTYPE_YG					602	//Ԥ���ź�

//�����������ͱ��붨��
#define		OBJTYPE_DIAG_SGZ            4001 //�����¹����ź�/�й���
#define		OBJTYPE_DIAG_NOFAULT        4002 //�޹���
#define		OBJTYPE_DIAG_INST           4003 //˲�����
#define		OBJTYPE_DIAG_UNKNOWN_FAULT  4004 //�������Ͳ���ȷ
#define		OBJTYPE_DIAG_FAULT          4005 //�й���

#define		OBJTYPE_DIAG_S_GND          4011 //����ӵ�
#define		OBJTYPE_DIAG_S_GND_INST     4012 //����˲ʱ�ӵ�
#define		OBJTYPE_DIAG_A_GND          4013 //A��ӵ�
#define		OBJTYPE_DIAG_B_GND          4014 //B��ӵ�
#define		OBJTYPE_DIAG_C_GND          4015 //C��ӵ�
#define		OBJTYPE_DIAG_D_GND          4016 //���ӵ�
#define		OBJTYPE_DIAG_D_GND_INST     4017 //���˲ʱ�ӵ�
#define		OBJTYPE_DIAG_AB_GND         4018 //AB���ӵ�
#define		OBJTYPE_DIAG_BC_GND         4019 //BC���ӵ�
#define		OBJTYPE_DIAG_CA_GND         4020 //4020 CA���ӵ�
#define		OBJTYPE_DIAG_T_GND          4021 //ABC���ӵ�

#define		OBJTYPE_DIAG_D_SHORT        4031 //����·
#define		OBJTYPE_DIAG_D_SHORT_INST   4032 //���˲ʱ��·
#define		OBJTYPE_DIAG_AB_SHORT       4033 //AB����·
#define		OBJTYPE_DIAG_BC_SHORT       4034 //BC����·
#define		OBJTYPE_DIAG_CA_SHORT       4035 //CA����·
#define		OBJTYPE_DIAG_T_SHORT        4036 //ABC�����·
#define		OBJTYPE_DIAG_T_SHORT_INST   4037 //���˲ʱ��·

//FTU��·�������ͱ��붨��
#define		OBJTYPE_REP_SGZ             4101 //�����¹����ź�/�й���
#define		OBJTYPE_REP_NOFAULT         4102 //�޹���
#define		OBJTYPE_REP_INST            4103 //˲�����
#define		OBJTYPE_REP_UNKNOWN_FAULT   4104 //�������Ͳ���ȷ
#define		OBJTYPE_REP_FAULT           4105 //�й���
	
#define		OBJTYPE_REP_S_GND           4111 //����ӵ�
#define		OBJTYPE_REP_S_GND_INST      4112 //����˲ʱ�ӵ�
#define		OBJTYPE_REP_A_GND			4113 //A��ӵ�
#define		OBJTYPE_REP_B_GND			4114 //B��ӵ�
#define		OBJTYPE_REP_C_GND			4115 //C��ӵ�

#define		OBJTYPE_REP_S_SHORT		    4121 //�����·
#define		OBJTYPE_REP_A_SHORT		    4122 //A���·
#define		OBJTYPE_REP_B_SHORT			4123 //B���·
#define		OBJTYPE_REP_C_SHORT			4124 //C���·
#define		OBJTYPE_REP_N_SHORT			4125 //�����·����ֵ��
#define		OBJTYPE_REP_D_SHORT			4126 //����·
#define		OBJTYPE_REP_D_SHORT_INST	4127 //���˲ʱ��·
#define		OBJTYPE_REP_AB_SHORT		4128 //AB����·
#define		OBJTYPE_REP_BC_SHORT		4129 //BC����·
#define		OBJTYPE_REP_CA_SHORT		4130 //CA����·

#define		OBJTYPE_REP_T_SHORT		    4131 //������·
#define		OBJTYPE_REP_T_SHORT_INST	4132 //�����˲ʱ��·

#define		OBJTYPE_REP_S_OVER			4141 //�������
#define		OBJTYPE_REP_S_OVER_INST		4142 //����˲ʱ����
#define		OBJTYPE_REP_A_OVER			4143 //A�����
#define		OBJTYPE_REP_B_OVER			4144 //B�����
#define		OBJTYPE_REP_C_OVER			4145 //C�����
#define		OBJTYPE_REP_N_OVER			4146 //�������

#define		OBJTYPE_REP_D_OVER			4147 //������
#define		OBJTYPE_REP_D_OVER_INST		4148 //���˲ʱ����
#define		OBJTYPE_REP_AB_OVER			4149 //AB������
#define		OBJTYPE_REP_BC_OVER			4150 //BC������
#define		OBJTYPE_REP_CA_OVER			4151 //CA������


//�����жϵĺ궨��
#define     IS_YC_TYPE(a)		        ((a)>=1 && (a)<200)
#define     IS_VOLT_TYPE(a)             ((a)>=OBJTYPE_V && (a)<=OBJTYPE_V2)
#define     IS_PYC_TYPE(a)	        	((a)>=OBJTYPE_P && (a)<=OBJTYPE_Pca)
#define     IS_QYC_TYPE(a)	        	((a)>=OBJTYPE_Q && (a)<=OBJTYPE_Qca)
#define     IS_IYC_TYPE(a)	        	((a)>=OBJTYPE_I && (a)<=OBJTYPE_I2)

#define     IS_KWH_TYPE(a)              ((a)>=200 && (a)<300)

#define     IS_BREAK_TYPE(a)            ((a)>=300 && (a)<400)
#define     IS_STATUS_TYPE(a)           ((a)>=400 && (a)<500)
#define     IS_SWITCH_TYPE(a)           ((a)>=500 && (a)<600)
#define     IS_PROT_TYPE(a)             ((a)>=600 && (a)<700)
#define     IS_DMSSTATUS_TYPE(a)        ((a)>=4000 && (a)<5000)     //add 20021123 ��Ϊ��ͨң�Ŵ���


#define     IS_GLSWITCH_TYPE(a)	        ((a)>=OBJTYPE_SW_LINE && (a)<=OBJTYPE_SW_BLINK)
#define	    IS_GRDSWITCH_TYPE(a)	    ((a)>=OBJTYPE_SW_L_GRD && (a)<=OBJTYPE_SW_PT_GRD)
#define     IS_BUSGRDSWITCH_TYPE(a)     ((a)>=OBJTYPE_SW_B1_GRD && (a)<=OBJTYPE_SW_BYB_GRD)





///////////////////////////////////////////////////
////////////////  ��������     ////////////////////
///////////////////////////////////////////////////


// ���������뺯��
// ���룺devtype �豸���� ,  stname  վ����
//       devname �豸��� ,  objtypename ��������
// �����objname �������
// ���أ�0 ��ȷ�� �������

#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
EncodeObjName(const char devtype,const char *stname,
				  const char *devname,const char  *objtypename,
				  char  *objname);


// ���������뺯��
// ���룺devtype �豸���� ,  stname  վ����
//       devname �豸��� ,  objtype ��������
// �����objname �������
// ���أ�0 ��ȷ�� �������
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
EncodeObjName(const char devtype,const char *stname,
				  const char *devname,const short  objtype,
				  char  *objname);


// ���������뺯��
// ���룺objname  �������
// �����devtype  �豸����(ͬ��)
//		 stname   վ����ϵ����(NULL ��ʾ�����)
//       devname  �豸����(ͬ��)
//       objtypename ��������(ͬ��)
// ���أ�0 ��ȷ�� �������
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
DecodeObjName(char *objname,char *devtype,
				   char *stname,char *devname,char *objtypename); 


// ���������뺯��
// ���룺objname  �������
// �����devtype  �豸����(ͬ��)
//		 stname   վ����ϵ����(NULL ��ʾ�����)
//       devname  �豸����(ͬ��)
//       objtype  ��������(ͬ��)
// ���أ�0 ��ȷ�� �������
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
DecodeObjName(char *objname,char *devtype,
				   char *stname,char *devname,short *objtype); 


// ����������±��뺯��

// ���ݶ����������±���
// ���룺objname      �������
//       objtypename  �µĶ�������
// ���أ�0 ��ȷ�� �������
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
EncodeObjName_ObjType(char *objname,const char *objtypename);


// ���ݶ����������±���
// ���룺objname      �������
//       objtype      �µĶ�������
// ���أ�0 ��ȷ�� �������
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
EncodeObjName_ObjType(char *objname,int objtype);



// ����С�����ص��豸��ű�����Ӧ���غ͵�բ�ı��
// ���룺brkdevname      С�����ص��豸���
// �����brkname         С�����ض�Ӧ���ص��豸���
//       swname          С�����ض�Ӧ��բ���豸���
// ���أ�0 ��ȷ�� �������
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
EncodeCarBreakName(const char *brkdevname,char *brkname,char *swname);


// ����С�����ص��豸��ű�����Ӧ���غ͵�բ�ı��
// ���룺brkswname       С�����ض�Ӧ���ػ�բ���豸���
// �����devname         С�����ص��豸���
// ���أ�0 �������С�����ض�Ӧ���صı��
//       1 �������С�����ض�Ӧ��բ�ı��
//       �������
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
DecodeCarBreakName(const char *brkswname,char *devname);


// ��ȡ�������͵����
// ����: objtypename	����������
//		 classmode		���෽�� ��0---��SCADA�����)
// ���أ����

#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
ObjTypeClassId(const char *objtypename,short *objtype=NULL,int classmode=OBJTPMODE_SCDTAB);

// ��ȡ�������͵����
// ����: objtype	��������
//		 classmode		���෽�� ��0---��SCADA�����)
// ���أ����

#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
ObjTypeClassId(const short objtype,char *objtypename=NULL,int classmode=OBJTPMODE_SCDTAB);

/////////////////////////////////////////////////////////////////////
/////////////////// �빫���豸��Ľӿ� //////////////////////////////
/////////////////////////////////////////////////////////////////////
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
GetDevParaRdbTableName(const char devtype,char *tabname);
/*
/////////////////////////////////////////////////////////////////////
////////////////////  ���ñ������� //////////////////////////////////
/////////////////////////////////////////////////////////////////////
#define  SCDBREAKDR_TABNAME			"ScdSw"
#define  SCDSWITCHDR_TABNAME		"ScdSw"
#define  SCDLINEDR_TABNAME			"ScdLn"
#define  SCDTRANSDR_TABNAME			"ScdTf"
#define  SCDCXDR_TABNAME			"ScdCb"
#define  SCDREACTORDR_TABNAME		"ScdRb"
#define  SCDBUSDR_TABNAME			"ScdBs"
#define  SCDLOADDR_TABNAME			"ScdLd"
#define  SCDWGDR_TABNAME			"ScdGn"
#define  SCDJOINTDR_TABNAME			"ScdJoint"	
#define  SCDBILEIDR_TABNAME			"ScdBilei"		// ��������
#define  SCDCTDR_TABNAME			"ScdCurrent"	// ����������
#define  SCDPTDR_TABNAME			"ScdVolact"		// ��ѹ������
#define  SCDPROTECTDR_TABNAME		"ScdProtect"	// ����װ��
#define  SCDXHXQDR_TABNAME			"ScdXhxq"		// ������Ȧ

#define  EMSSWITCHP_TABNAME		"EMSSwitchP"
#define  EMSLINEP_TABNAME		"EMSLineP"
#define  EMSTRANSHP_TABNAME		"EMSTransP"
#define  EMSCXP_TABNAME			"EMSCXP"
#define  EMSREACTORP_TABNAME	"EMSDkqP"
#define  EMSBUSP_TABNAME		"EMSSBusP"
#define  EMSLOADP_TABNAME		"EMSLoadP"
#define  EMSGENERATORP_TABNAME	"EMSWgP"
#define  EMSJOINTP_TABNAME		"EMSJointP"
#define  EMSBILEIP_TABNAME		"EMSBiLeiP"
#define  EMSCTP_TABNAME			"EMSCurrentP"
#define  EMSPTP_TABNAME			"EMSVolactP"
#define  EMSPROTECTP_TABNAME	"EMSProtectP"
#define  EMSCOILP_TABNAME		"EMSXhxqP"


#define  EMSMSYSD_TABNAME		"EMSMsysD"
#define  EMSSWITCHD_TABNAME		"EMSSwitchD"
#define  EMSLINED_TABNAME		"EMSLineD"
#define  EMSTRANSHD_TABNAME		"EMSTransD"
#define  EMSCXD_TABNAME			"EMSCxD"
#define  EMSREACTORD_TABNAME	"EMSDkqD"
#define  EMSBUSD_TABNAME		"EMSSBusD"
#define  EMSLOADD_TABNAME		"EMSLoadD"
#define  EMSGENERATORD_TABNAME	"EMSWgD"
#define  EMSJOINTD_TABNAME		"EMSJointD"

#define  EMSMSYSDR_TABNAME		"EMSMsysDR"
#define  EMSSWITCHDR_TABNAME	"EMSSwitchDR"
#define  EMSLINEDR_TABNAME		"EMSLineDR"
#define  EMSTRANSHDR_TABNAME	"EMSTransDR"
#define  EMSCXDR_TABNAME		"EMSCxDR"
#define  EMSREACTORDR_TABNAME	"EMSDkqDR"
#define  EMSBUSDR_TABNAME		"EMSSBusDR"
#define  EMSLOADDR_TABNAME		"EMSLoadDR"
#define  EMSGENERATORDR_TABNAME	"EMSWgDR"
#define  EMSJOINTDR_TABNAME		"EMSJointDR"





/////////////////////////////////////////////////////////////////////////////////
///////////////  ������豸����Ľӿ� ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


typedef  struct    {
	char	devtype;			//�豸���ʹ���
	char	substation[SUBSTNAME_LEN];	//վ����ϵ���� 
	char	devname[POWDEVNAME_LEN];		//�豸��ţ��Ѱ���վ����ϵ����
	short	objtype;	//��������
	uchar	devobj_flag;	//2--ȡ�豸����״̬ 1--ָ�豸������ 0--ָ���豸�����Ķ���
}  DEV_OBJECT;
*/


//��һ̬(��̬)	����
#define  FIRST_STATE_SCADA		0 //SCADAʵʱ̬
/*
#define  FIRST_STATE_SE			1 //״̬����
#define  FIRST_STATE_PF			2 //��������
#define  FIRST_STATE_OPF		3 //���ų���
#define  FIRST_STATE_OVR		4 //�޹��Ż�
#define  FIRST_STATE_SC			5 //��·����
#define  FIRST_STATE_CGD		6 //��ȫУ��
#define  FIRST_STATE_PASDM		7 //PAS����

#define  FIRST_STATE_STS		9 //���վ����

#define  FIRST_STATE_OPT		10 //����Ʊ
*/
//�ڶ�̬��С̬������
#define  SECOND_STATE_REAL		0 //ʵʱ̬
/*
#define  SECOND_STATE_S1		1 //�о�̬1
#define  SECOND_STATE_S2		2 //�о�̬2
#define  SECOND_STATE_S3		3 //�о�̬3
#define  SECOND_STATE_S4		4 //�о�̬4
#define  SECOND_STATE_S5		5 //�о�̬5
#define  SECOND_STATE_S6		6 //�о�̬6
#define  SECOND_STATE_S7		7 //�о�̬7
#define  SECOND_STATE_S8		8 //�о�̬8
#define  SECOND_STATE_S9		9 //�о�̬9

#define  SECOND_STATE_OS		50 //����Ʊ��̬
#define  SECOND_STATE_OE		51 //����Ʊĩ̬
*/



typedef  struct    {
	char	devtype;			//�豸���ʹ���
	char	substation[SUBSTNAME_LEN];	//վ����ϵ���� 
	char	devname[POWDEVNAME_LEN];		//�豸��ţ��Ѱ���վ����ϵ����
	short	objtype;	//�������ʹ���
	char	fieldname[RDB_COLNAME_LEN];	//����
	uchar   firststate;		//��һ̬(��̬) 
	uchar   secondstate;	//�ڶ�̬��С̬��0 ʵʱ̬,1~9 Ϊ�о�̬
	uchar	devobj_flag;	//2--ȡSCADA�豸����״̬ 3--ȡPAS�豸����״̬
} RDB_DEV_FIELD;

typedef  struct   {
	uchar			fieldmode;  // 1--RDB_DEV_FIELD��Ч 0--FIELDINFO��Ч
	union  {
		RDB_DEV_FIELD   devfield;
		RDB_FIELD_STRU		dbfield;
	}  fieldinfo;
} RDB_MIX_FIELD;

/////////////////////////////////////////////////////////////////////
////////////// �����豸����ת�����ܵ�ʵʱ����� /////////////////////
/////////////////////////////////////////////////////////////////////


////// �����豸����ṹRDB_DEV_FIELD ת���� ���ݿ����ṹFIELDINFO  ////////

// ����SCADA����
#ifdef RDB_OS_WIN32
int __declspec(dllexport)
#else
int
#endif
DevFieldToDbField_SCADA(const RDB_DEV_FIELD *devfield,RDB_FIELD_STRU *dbfield);


/*
// ����PAS����
#ifdef RDB_OS_WIN32
int __declspec(dllexport)
#else
int
#endif
DevFieldToDbField_PAS(const RDB_DEV_FIELD *devfield,RDB_FIELD_STRU *dbfield);


// ���ݲ���Ʊ����
#ifdef RDB_OS_WIN32
int __declspec(dllexport)
#else
int
#endif
DevFieldToDbField_OPT(const RDB_DEV_FIELD *devfield,RDB_FIELD_STRU *dbfield);

// ����RDB_DEV_FIELD�е�firststate�Զ�����
#ifdef RDB_OS_WIN32
int __declspec(dllexport)
#else
int
#endif
DevFieldToDbField(const RDB_DEV_FIELD *devfield,RDB_FIELD_STRU *dbfield);


////// �����϶���ṹRDB_MIX_FIELD ת���� ���ݿ����ṹFIELDINFO  ////////
#ifdef RDB_OS_WIN32
int __declspec(dllexport)
#else
int
#endif
MixFieldToDbField(int nums,const RDB_MIX_FIELD *mixfield,RDB_FIELD_STRU *dbfield);
*/



#endif
