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

#endif