#ifndef GYTIMP_FUNC_H_HEADER_INCLUDED
#define GYTIMP_FUNC_H_HEADER_INCLUDED
//################################################
//############���������˵��######################
//################################################

enum
{
	YTTYPE_YKTYPE,//ң�ط�ʽ
	YTTYPE_SETTYPE//��㷽ʽ
};

typedef struct _tagYTReqMail
{
	char	oper[ GDB_DESC_LEN ];//������
	char	devname[ GDB_DESC_LEN * 2 ];//�豸����
	char	yttypedesc[ GDB_DESC_LEN ];//ң����������,���磺4��->5��
	char	graph[ GDB_PATH_LEN ];//ң����������ͼ��
	uint16	yttype;//ң������,��㷽ʽ����ң�ط�ʽ
	uint16	upykno;//��ң�غ�
	uint16	downykno;//��ң�غ�
	uint16	stopykno;//��ͣң�غ�
	uint16	ytno;//ң����
}YTREQMAIL, *PYTREQMAIL;

//��ʼң��
//FUNCSTRUCT tmpstruct;
//int success=sendCMD( GYTF_STARTYT,(uint32)&tmpstruct,0 );
#define GYTF_STARTYT	0x200

//ң�����
// REQSTRUCT ytreq;
//int success=sendCMD( GYTF_YTREQ,(uint32)&ytreq,0 );
#define GYTF_YTREQ	(GYTF_STARTYT+1)

//ң������ظ�
//int success=sendCMD( GYTF_YTREQRET,0,0 );
#define GYTF_YTREQRET	(GYTF_STARTYT+2)

//��ʱ����
//int success=sendCMD( GYTF_TIMEEVENT,0,0 );
#define GYTF_TIMEEVENT	(GYTF_STARTYT+3)

#endif