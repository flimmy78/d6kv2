#include "funutils.h"
// #include "constutils.h"
#include "globaldef.h"


double time_to_float(const QString& dataTime)
{
	return 0.0;
}

// �ı����������¸�ʽ:
//11,���ܱ��ѹ��׷�֧C�����,C,CXDL,A,0.0029130094,0.000000,0.000000,-32767,32767
// ������ȡ��index����֮ǰ��һ���ַ���, index is based on 0;
QString cut_char(const QString& cStr, int index, char sub)
{
	int i(0);
	QString str1(cStr);
	
	while (i < index)
	{
		str1 = str1.right(str1.length() - str1.indexOf(sub) - 1); i++;
		if ((str1.indexOf(sub) == -1) && (i < index)) str1 = "";
	}
	int n = str1.indexOf(sub);
	if (n >= 0) str1 = str1.left(n);
	return str1.trimmed();
}

int cut_char_to_int(const QString& cStr, int index)
{
	QString s = cut_char(cStr, index);
	return s.toInt();
}

double cut_char_to_float(const QString& cStr, int index)
{
	QString s = cut_char(cStr, index);
	return s.toFloat();
}

// ȥ��·��������ļ���
QString extract_file_name(const QString& fileName)
{
	int n = last_delimiter("\\", fileName);
	return fileName.right(fileName.length() - n);
}

// �ҳ�s��Delimiters�е�һ���ַ���s�г��ֵ����λ��
int last_delimiter(QString Delimiters, const QString& s)
{
	int nResult = s.length() - 1;
	while (nResult > 0)
	{
		QChar ch(s[nResult]);
		if (Delimiters.indexOf(ch) == -1)
			nResult--;
		else break;
	}
	return nResult;
}

/***********************************************************
����  �ܡ������ļ�����׺
����  ����FileName-�ļ��� Extension-��׺
/***********************************************************/
QString change_file_ext(const QString& fileName, QString ext)
{
	QString PathDelim = "\\";
	QString DriveDelim = ":";
	QString EndDelim = "\0";
	QString DotDelim = ".";

	int i = last_delimiter("." + PathDelim + DriveDelim, fileName);
	QString s = fileName[i];
	if (i == 0 || s != DotDelim)
		i = 1000;
	return fileName.left(i) + ext;
}

const QString PossiblePhases[3][4] = 
{("a", "b", "c", "n"),  ("A", "B", "C", "N"), ("��", "��", "��", "����")};

QString possible_phase(const QString& s)
{
	const QChar phase1[] = {'a', 'b', 'c', 'n'};
	const QChar phase2[] = {'A', 'B', 'C', 'N'};
	const QString phase3[] = {"��", "��", "��", "����"};
	
	for (int i = 0; i < 3; i++)
	{
		if (s.indexOf(phase1[i]) >= 0)
			return phase1[i];
		if (s.indexOf(phase2[i]) >= 0)
			return phase2[i];
		if (s.indexOf(phase3[i]) >= 0)
			return phase3[i];
	}
	return "";
}

// ���Value�е�Bitλ��ֵ
// 0: False; 1: True
bool is_bit_on(ushort value, int bit)
{
	return ((value & (1 << bit)) != 0);
}

void get_range(int& m1, int& m2)
{
	int n;
	if (m1 > m2)
	{
		n = m2; m2 = m1; m1 = n;
	}
}

// ��ʽ������
QString format_complex(const Complex& data)
{
	QString str;
	if (data.Im >= 0)
		str.sprintf("%5.3f+%5.3fj", data.Re, data.Im);
	else
		str.sprintf("%5.3f%5.3fj", data.Re, data.Im);
	return str;
}

// ��ʽ������ - '%5.3f��%5.3f
QString format_vector(const Complex& data)
{
	QString strRe,strIm;
	strRe.sprintf("%5.3f", data.Re);
	strIm.sprintf("%5.3f",data.Im);
	return strRe+QObject::tr("��")+strIm;
}

QString switch_state(bool state)
{
	return DIGINFOSTR[1 + (int)state];
}

// ��ø�ʽ���ַ���
QString get_format_str()
{
// 	return _C2Q("%5.3f");
	return _C2Q("%1");
}

// ������·��
QString relative_path(const QString& path)
{
	int n = last_delimiter("/", path);
	return path.right(path.length() - n - 1);
}

