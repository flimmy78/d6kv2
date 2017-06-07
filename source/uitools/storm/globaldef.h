#ifndef GLOBALDEF_H_
#define GLOBALDEF_H_

#include <QString>

#define  _Q2C(str) (str.toAscii().data())
#define _C2Q(str) (QString::fromLocal8Bit(str))

const int IP_LEN = 32;
const int FILENAME_LEN = 256;
const int BUF_LEN = 4096;
const int READ_LEN = 512;
const int FIXMSG_LEN = 6;
const int STARTCODE = 0x68;
const int YCBASE = 462;
const int YCBAND = 40;
const int YCBEGIN = 0x4001;
const int YXBEGIN = 0x0001;
const int YCNUMPERFRAME = 100;
const int YXNUMPERFRAME = 100;
const int YCNUMPERSTORMFRAME = 32;
const int YXNUMPERSTORMFRAME = 64;
const int SOENUMBERSTORMFRAME = 18;
const int YCTYPE = 0x15;
const int YXTYPE = 0x01;
const int SOETYPE = 0x1E;

enum SENDCMD {SENDCHG, SENDNORMAL};

typedef struct  
{
	int clientNum;		//�ͻ�������
	int clientYxNum;	//ÿ���ͻ��˵�ң������
	int clientYcNum;	//ÿ���ͻ��˵�ң������
	int changeYcNum;	//ÿ���ͻ���ÿ��ң�ű�λ����
	int changeYxNum;	//ÿ���ͻ���ÿ��ң��仯����
	int sendGap;		//���ͼ��
	int startPort;		//�׸��ͻ��˵Ķ˿�
	int	countLimit;		//���ʹ���
}SETTINGS;

typedef struct
{
	int listenSock;	
	int linkSock;
	char ip[IP_LEN];

	int linkSock2;
	char ip2[IP_LEN];

	int clientChgYxCount;	//ÿ���ͻ��˷��͵ı�λң������
	int clientChgYcCount;	//ÿ���ͻ��˷��͵ı仯ң������
	bool readyFlag;			//ÿ���ͻ���׼�����ܷ籩�ı�־
	bool stormFlag;			//ÿ���ͻ��˷��ͷ籩�ı�־
	int sendCount;			//�ܷ��ʹ���
}CLIENTSTAT;

typedef struct
{
	ushort		 aMs;
	uchar   	 aSec;
	uchar 		 aMin;
	uchar		 aHour;
	uchar		 aDay;
	uchar   	 aMon;
	uchar   	 aYear;          //start 1900
}FETIME;
#endif