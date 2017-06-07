#ifndef GYKIMP_FUNC_H_HEADER_INCLUDED
#define GYKIMP_FUNC_H_HEADER_INCLUDED
//################################################
//############���������˵��######################
//################################################


typedef struct _tagYKReqMail
{
	char	oper[ GDB_DESC_LEN ];//������
	char	devname[ GDB_DESC_LEN * 2 ];//�豸����
	char	yktype[ GDB_DESC_LEN ];//���ַ��������Ĳ������ͣ�����:�ɷֵ���
	char	graph[ GDB_PATH_LEN ];//ң���豸����ͼ��
	uint16	ykno;//ң�غ�
	uint16	yxno;//ң�ź�
}YKREQMAIL, *PYKREQMAIL;

typedef struct _tagYKAQYSMail
{
	uint32	MailMark;			//4
	char	HostName0[ GDB_CODE_LEN ];	//+24=28
	char	HostName1[ GDB_CODE_LEN ];	//+24=52
	char	Operator[ GDB_CODE_LEN ];		//+24=76
	char	ObjCode[ GDB_CODE_LEN ];		//+24=100
	char	ObjName[ GDB_DESC_LEN ];		//+40=140
	uint32	OperateType;		//+4=144
	uint32	Permission;			//+
	char	Info[ 256 ];		//+300=444
}YKAQYSMAIL,*PYKAQYSMAIL;

//��ʼң��
//FUNCSTRUCT tmpstruct;
//int success=sendCMD( GYKF_STARTYK,(uint32)��tmpstruct,0 );
#define GYKF_STARTYK	0x100

//ң�����
// REQSTRUCT ykreqstruct;
//int success=sendCMD( GYKF_YKREQ,(uint32)&ykreqstruct,0 );
#define GYKF_YKREQ		(GYKF_STARTYK+1)

//ң������ظ�
//int success=sendCMD( GYKF_YKREQ,0,0 );
#define GYKF_YKREQRET	(GYKF_STARTYK+2)

//��ʱ����
//int success=sendCMD( GYKF_TIMEEVENT,0,0 );
#define GYKF_TIMEEVENT	(GYKF_STARTYK+3)

//����ң��
#define GYKF_SERIAL_YK	(0x01)

//���б༭
#define GYKF_SERIAL_EDIT	(0x02)

//������ж���
#define GYKF_SERIAL_ADDOBJECT (0x03)

//�ر����б༭����
#define GYKF_SERIAL_CLOSEEDITWND (0x04)

//ң�ذ�ȫԼ��
#define GYKF_AQYS			(0x05)

//��ȫԼ����֤����
#define GYKF_AQYSRET		(0x06)

//��/�ϻ�ң��
#define GYKF_Ring_YK			(0x07)

//��/�ϻ�ң�ر༭
#define GYKF_Ring_EDIT		(0x08)

//��ӽ�/�ϻ�����
#define GYKF_Ring_ADDOBJECT (0x09)

//�رս�/�ϻ��༭����
#define GYKF_Ring_CLOSEEDITWND (0x0a)

//Ⱥ��
#define GYKF_GROUP_YK			(0x0b)

//Ⱥ�ر༭
#define GYKF_GROUP_EDIT		(0x0c)

//���Ⱥ�ض���
#define GYKF_GROUP_ADDOBJECT (0x0d)

//�ر�Ⱥ�ر༭����
#define GYKF_GROUP_CLOSEEDITWND (0x0e)

#endif