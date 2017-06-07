#include "viewintf.h"
#include "optionsimpl.h"
#include "dataserver.h"

extern Propertys* g_Propertys;

void Data_server::GetThreePhase1(int a, int b, int c, int pos, Complex* data)
{
}

bool Data_server::CheckDelta(int pos, double delta, int& pos1)
{
	return false;
}

bool Data_server::CheckLimit1(const Complex& c, bool b)
{
	return false;
}

bool Data_server::CheckLimit2(int c, int pos, bool b)
{
	return false;
}

void Data_server::GetDleta(double* p, int count, int delta)
{
}

void Data_server::Mul(double* p, int count, double f)
{
}

void Data_server::Move1(const double* src, double* dst, int count, int delta)

{
}

Complex Data_server::Factor120()
{
	return Complex(-0.5, 0.8660254);
}

Complex Data_server::Factor240()
{
	return Complex(-0.5, -0.8660254);
}

// ��ֵ+��� =>> ʵ��+�鲿
Complex Data_server::VectorToComplex(const Complex& c)
{
	double ang = c.Im * PI / 180.0;
	return Complex(c.Re * cos(ang), c.Re * sin(ang));
}

// ��ֵ+���(����)
Complex Data_server::GetVector(int channelNo, int pos)
{
	Complex c = GetHarmValue(channelNo, pos, 1);
	CalAngle(c.Re, c.Im);
	return c;
}

// ��ֵ+���(�Ƕ�)
Complex Data_server::GetVector1(int channelNo, int pos)
{
	Complex c = GetVector(channelNo, pos);
	c.Im *= 180.0 / PI;
	return c;
}

// ����ʵ�鲿
Complex Data_server::GetComplex(int channelNo, int pos)
{
	return GetHarmValue(channelNo, pos, 1);
}

// г������
Complex Data_server::GetHarmValue(int channelNo, int pos, int harm)
{
	if (DataObject() == NULL)
		return Complex(0, 0);

	bool bHarm = harm > 1;
	int cycle = DataObject()->cycle_count(pos);
	double* p = DataObject()->get_cycle_data(channelNo, pos);
	double re, im;
	CycleDecompose(p, re, im, cycle, harm, g_Propertys->Filter(), bHarm);
	return Complex(re, im);
}

// ���г���������ʽ
Complex Data_server::GetHarmVector(int channelNo, int pos, int harm)
{
	Complex c = GetHarmValue(channelNo, pos, harm);
	CalAngle(c.Re, c.Im);
	c.Im *= 180.0 / PI;
	return c;
}

// ���ֱ��
Complex Data_server::GetDirect(int channelNo, int pos)
{
	int cycle = DataObject()->cycle_count(pos);
	double* p = DataObject()->get_cycle_data(channelNo, pos);
	Complex c;
	c.Re = CalAverage(p, cycle);
	return c;
}

// ���ƽ��ֵ
Complex Data_server::GetAverage(int channelNo, int pos, int count)
{	
	if (DataObject() == NULL)
		return Complex(0, 0);

	double* p =	DataObject()->get_analog_data(channelNo, pos, count);
	Complex c;
	c.Re = CalAverage(p, count);
	return c;
}

void Data_server::GetThreePhase(int a, int b, int c, int pos, double cycle, Complex* data)
{
}

// ����������
void Data_server::GetDual(int count, const double* a, const double* b, double* ab)
{
	for (int i = 0; i < count; i++)
		ab[i] = a[i] - b[i];
}

Complex Data_server::GetSinglePhaseZ(int u, int ia, int ib, int ic, 
									 int phase, int pos, LINEPARA line)
{
	return Complex(0, 0);
}

Complex Data_server::GetInterPhaseZ(int u1, int u2, int i1, int i2, int pos)
{
	return Complex(0, 0);
}

Complex Data_server::GetSequenceZ(int ua, int ub, int uc, 
								  int ia, int ib, int ic, int seqtype, int pos)
{
	return Complex(0, 0);
}

Complex Data_server::GetAngle(int u1, int u2, int i, int pos)
{
	return Complex(0, 0);
}

Complex Data_server::GetAngle1(int ua, int ub, int uc, 
					   int ia, int ib, int ic, int seqtype, int pos)
{
	return Complex(0, 0);
}

Complex Data_server::GetAngle2(int ua, int ub, int uc, 
							   int ia, int ib, int ic, int seqtype, int pos, double delta)
{
	return Complex(0, 0);
}

Complex Data_server::GetAngle3(int u1, int u2, int i1, int i2, int pos)
{
	return Complex(0, 0);
}

Complex Data_server::GetAngle4(int u1, int u2, int i1, int i2, int pos, double delta)
{
	return Complex(0, 0);
}

Complex Data_server::GetAngle5(int u, int i, int pos)
{
	return Complex(0, 0);
}

Complex Data_server::GetAngle6(int u, int i, int pos, double delta)
{
	return Complex(0, 0);
}

Complex Data_server::GetSeq(const Complex& a, const Complex& b, const Complex& c, int seqtype)
{
	return Complex(0, 0);
}

Complex Data_server::GetSeq1(int a, int b, int c, int pos, int seqtype)
{
	return Complex(0, 0);
}

// ����г������
double* Data_server::CreateHarm(int channel, int harm)
{
	if (DataObject() == NULL)
		return NULL;

	int count = DataObject()->data_count();
	double* pHarm = new double[count];
	int samCount = DataObject()->sample_count();
	SAMPLE* pSample = new SAMPLE[samCount];
	DataObject()->get_full_sample(pSample);

	for (int i = 0; i < samCount; i++)		// ��ʱ�ν��м���
	{
		double* pData = DataObject()->get_analog_data(channel, 0, count);
		pData += pSample[i].begin;
		count = pSample[i].count * sizeof(double);
		double* pRe = new double[count];
		double* pIm = new double[count];
		Decompose(pData, 
				  pRe, pIm, 
				  pSample[i].count, 
				  (int)pSample[i].frequency,
				  harm,
				  g_Propertys->Filter(),	// �㷨����
				  true);
		double *p, *p1, *p2, *p3;
		p = pHarm;
		p += pSample[i].begin;
		for (int j = 0; j < pSample[i].count; j ++)
		{
			p3 = p;	  p3 += j;
			p1 = pRe; p1 += j;
			p2 = pIm; p2 += j;
			double fval = *p1;
			double fang = *p2;
			CalAngle(fval, fang);
			*p3 = fval * cos(fang) * 1.414;
		}
		delete pRe;
		delete pIm;
	}
	return pHarm;
}

// Ƶ�ʼ��� 
double Data_server::GetFreq(int channel, int pos)
{
	int cycle = DataObject()->cycle_count(pos);
	double* p = DataObject()->get_cycle_data(channel, pos);
	double freq = 0;
	CalFrequency(p, &freq, 1, cycle);
	return freq;
}


