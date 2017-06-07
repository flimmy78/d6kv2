#ifndef GLOBALDEF_H
#define GLOBALDEF_H
#include <QString>
#include <QColor>

#define  _Q2C(str) (str.toAscii().data())
#define _C2Q( str ) ( QString::fromLocal8Bit( str ) )


const QString PHASES[] = {"A", "B", "C", "N", "None"};
const QColor COLORTABS[] = 
{Qt::yellow, Qt::green, Qt::red, Qt::cyan, Qt::white, Qt::gray};
const QString NEWFILENAME = _C2Q("�½������ļ�");

// �α����
const QColor TIMELINECOLORS[] =
{Qt::yellow, Qt::gray, Qt::red};
const QString TIMELINECAPTION[] =
{_C2Q("�α�"), _C2Q("���ʱ�����"),  _C2Q("��t���")};
const QString TIMELINESTR[] =
{ _C2Q("�����α�͡�t���"), _C2Q( "�����α�͡�t���")};
const QString AUTOSTRING[] =
{ _C2Q("�Զ���ʾ"),  _C2Q("ֹͣ��ʾ")};
const QString MATHSTR =  _C2Q("�㷨: ");
const QString ARITHMETICSTRING[] =
{ _C2Q("ȫ�ܸ���"),  _C2Q("���ܸ���"),  _C2Q("��֣�ȫ�ܸ���"),  _C2Q("��֣����ܸ���")};

const QString AVERAGESTR =  _C2Q("���ü���ƽ��ֵ�ĵ���: ");

const QString ANAINFOSTR[] =
{ _C2Q("˲ʱֵ"),
_C2Q("��������"),
_C2Q("����ʵ�鲿"),
_C2Q("ƽ��ֵ"),
_C2Q("Ƶ��"),
_C2Q("ֱ��"),
_C2Q("%1��г��"),
_C2Q("��"),
_C2Q("j"),
_C2Q("ѡ����..."),
_C2Q("������..."),
_C2Q("ɾ����"),
_C2Q("����ʸ��ͼ")};

const QString DIGINFOSTR[] =
{_C2Q("״̬%1"),
_C2Q("��"),
_C2Q("��"),
_C2Q("�α��׼"),
_C2Q("�����ޱ�λͨ��")};

const QString ADDVECTORSTR = _C2Q("���ʸ��ͨ��");
const QString NOVECTORSTR = _C2Q("�޻�׼ʸ��");
const QString ADDHARMSTR = _C2Q("���г��ͨ��");
const QString HARMONESTR = _C2Q("����");

#endif