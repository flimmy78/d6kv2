#ifndef APPDEF_H
#define APPDEF_H

#include "scd/saveflag.h"
#include "sysdef.h"

#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDebug>
#include <QtCore/QMetaType>

#define EPSILON  (1e-5)

#define C2G(str) QString::fromLocal8Bit(str)
#define G2C(qs)  (const char*)(qs).toLocal8Bit()

#define MANUSER  C2G("superdoor")
#define MANPWD   C2G("superdoor")
#define APPNAME  C2G("hiscurve")
#define ORGNAME  C2G("daqo")

#define ONE_CURVE_TABLE_COLUMN 2

#define YCCURVETABLEPREFIX C2G("YC")
#define DDCURVETABLEPREFIX C2G("KWH")
#define YCDAYTABLEPREFIX C2G("DAYYC")
#define DDDAYTABLEPREFIX C2G("DAYKWH")
#define YCMONTABLEPREFIX C2G("MONTHYC")
#define DDMONTABLEPREFIX C2G("MONTHKWH")
#define YCYEARTABLEPREFIX C2G("YEARYC")
#define DDYEARTABLEPREFIX C2G("DYEARKWH")
#define VARIANTCURVETABLEPREFIX C2G("YCCHANGE")
#define TRENDCURVETABLEPREFIX C2G("YCTREND")

// #define YCTYPEDEFINED        C2G("/dbinfo/ң������.txt")
// #define KWHTYPEDEFINED       C2G("/dbinfo/���_����ֵ.txt")
#define DayCycleBeginMinStaticMinute



enum CurveTreeNodeType{RootNode,TerminalNode,YCParaNode,KWHParaNode};
enum CurveTreeType{StationMeasType=1,EnergyMeasType,EmsKindType};

enum CurveDataSrc{YcDataSrc,DdDataSrc};
enum CurveCycleType{DayCycle=1,MonthCycle=2,YearCycle=3,VariantDayCycle = 4,TrendDayCycle = 5};
enum CurveSegmentType{GeneralDate,SegmentDate};
enum CurveSelectorType{SingleSelector=1,MutilSelector=2,CompSelector=3};

//enum CurveDataKindDef{CurrentKind,VoltageKind,WPowerKind,RPowerKind,WKwhKind,RKwpKind,OtherTypeKind};

// #define ARRAY_LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))
// 
// static const QRgb DataCurveColorTable[]=
// {
// 		qRgb(0xFF,0x00,0x00),
// 		qRgb(0x00,0xFF,0x00),
// 		qRgb(0x00,0x00,0xFF),
// 		qRgb(0xFF,0xFF,0x00),
// 		qRgb(0xFF,0x00,0xFF),
// 		qRgb(0x80,0x80,0x80),
// 		qRgb(0x80,0x80,0x00),
// 		qRgb(0x80,0x00,0x80),
// 		qRgb(0x80,0x00,0x00)
// };

struct curvetypepara
{
private:
	int		_type;
	QString _desc;
	QColor  _color;
	QString _unit;
public:
	curvetypepara():_type(0),_desc(""),_color(QColor(Qt::black)),_unit(""){;}
	curvetypepara(int Type,QString desc,QColor color,QString unit)
		:_type(Type),_desc(desc),_color(color),_unit(unit){;}
	int Type() const{return _type;}
	QString Desc() const{return _desc;}
	QColor  Color() const{return _color;}
	QString Unit() const{return _unit;}
};

struct terminalpara
{
	short	no;
	QString	name;
	QString	describe;
	short   zone;
	QString sname;
	bool    beginstatus;
	bool    isstat;
	bool    isused;
};

struct stationviewpara
{
	short	no;
	QString	name;
	short   areano;
};

 struct StationAreaPara
{
	short	id;						//id
	QString	name;	//name
	short	masterid;				//masterAreaId
};

struct ycpara
{
	QString	 name;					//����
	QString	 describe;				//����
	short    zone;					//�����
	QString  devname;				//�豸����
	short    type;					//����
	short	 terminalno;			//�ն˺�
	short    ycno;					//ң�����
	
	float    warnupperlimit;
	float	 warnlowerlimit;
	float    alertupperlimit;
	float    alertlowerlimit;
	
	bool     iscomputer;			//��������־
	short    savetime;				//��������
	short    plantime;              //�ƻ����
	bool     isshow;				//�Ƿ���ʾ
	//
	QString  typeDescribe;
};

struct kwhpara
{
	QString	 name;					//����
	QString	 describe;				//����
	short    zone;					//�����
	QString  devname;				//�豸����
	short    type;					//����
	short	 terminalno;			//�ն˺�
	short    kwhno;					//������
	float    basecode;				//����
    float    scale;					//���
	float	 maxcode;				//����
	float    times;					//����
	short    savetime;				//��������
	short    collectortype;			//�ɼ�����
	ulong    primitivemaxcode;		//ԭʼ����;
	bool     iscomputer;			//��������־
	bool     isintegral;			//���ֱ�־
	bool     issave;				//���̱�־
	bool     isdel;                 //ɾ����־
	short    starttime;			//��ʼ����ʱ��
	bool     startstatus;			//��ʼ״̬
	bool     isstat;				//�Ƿ�ͳ��
	bool     isshow;                //�Ƿ���ʾ
	//
	QString  typeDescribe;
};


struct hisDataValue
{
    QString		name;
    ushort      date;
    ushort      time;
	ushort      ms;
    uchar   	flag;
	union
	{
		uchar		yxdata;
        float       ycdata;
		float		kwhdata;
	} val;
	union
	{
		uchar		 yxdata;
        short		 ycdata;
		uint 		 kwhdata;
	} rawval;
    short	      odate;
    short         otime;
    uchar         oflag;
};

Q_DECLARE_METATYPE(terminalpara*);
Q_DECLARE_METATYPE(StationAreaPara*);
Q_DECLARE_METATYPE(ycpara*);
Q_DECLARE_METATYPE(kwhpara*);

#endif