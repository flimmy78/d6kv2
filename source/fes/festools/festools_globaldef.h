/**
@file festools_globaldef.h
@brief ȫ�ֶ���

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/
#ifndef FESTOOLS_GLOABLDEF_H_
#define FESTOOLS_GLOABLDEF_H_

#define _Q2C(str) ((const char *)((str).toLocal8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))

const int PORT_NO = 9005 ;
const int RECV_LEN = 1024 ;

enum USER_ROLE
{
	ROLE_MODEL_TYPE = Qt::UserRole + 10 ,

	TYPE_CITY ,
	TYPE_STATION ,
	TYPE_TERMINAL ,
	TYPE_CHANNAL ,
};

typedef struct _StationAreaPara
{
	short	id ;							//id
	char		name[ 64 ] ;			//name
	short	masterid ;				//masterAreaId
}STATIONAREAPARA,*PSTATIONAREAPARA;

typedef struct _TerminalViewPara
{
	short	terminalno ;				//�ն����
	char		terminalname[ 24 ] ;	//�ն˴���
	char		terminaldesc[ 64 ] ;	//�ն�����
	char		stationname[ 24 ] ;		//վ����ϵ����
	char		stationdesc[ 40 ] ;		//վ����ϵ����
	short	areaid ;
	short	masterid ;
	char		areaname[ 64 ] ;
}TERMINALVIEWPARA,*PTERMINALVIEWPARA;

typedef struct _ChannelViewPara
{
	short	channelno ;					//ͨ����
	char		channelname[ 24 ] ;	//ͨ������
	char		channeldesc[ 64 ] ;		//ͨ������
	short	terminalno ;				//�ն����
	char		terminalname[ 24 ] ;	//�ն˴���
	char		terminaldesc[ 64 ] ;	//�ն�����
	char		stationname[ 24 ] ;		//վ����ϵ����
	char		stationdesc[ 40 ] ;		//վ����ϵ����
	short	areaid ;
	short	masterid ;
	char		areaname[ 64 ] ;
}CHANNELVIEWPARA,*PCHANNELVIEWPARA;

typedef struct rtdata
{
	int no;
	QString desc;
	int isnegate;
	int iscomputer;
}RTDATA, *PRTDATA;

#endif