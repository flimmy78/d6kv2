#include <math.h>
#include <memory.h>

#include "mathutils.h"

// nType-�㷨����: 0-ȫ�� 1-�벨 2-���ȫ�� 3-��ְ벨

/*=========================================== 
	          ��Ƿ�ֵ����     
   ������AReal��ʵ���� AImage���鲿   	 
   ���أ�AReal��ģֵ��AIamge�����(0-2PI)
============================================*/   
void CalAngle(double& AReal, 
			  double& AImag)
{
	double Re = AReal;
	double Im = AImag;
	double Angle;

	Angle = atan2(Im, Re);
	AReal = pow(Re * Re + Im * Im, 0.5) / 1.414;
	if (Re>0.001 && Im>=0) 
		AImag = Angle;                // Area 1
	else if (Re>0.001 && Im<0)
		AImag = 2 * PI + Angle;       // Area 4
	else if (Re<-0.001 && Im>=0.0)
		AImag = Angle;		          // Area 2
	else if (Re<-0.001 && Im<0.0)
		AImag = 2 * PI + Angle;       // Area 3
	else if (fabs(Re)<0.001 && Im>0.01)
		AImag = PI / 2.0;             // +Y
	else if (fabs(Re)<=0.001 && Im<-0.01)
		AImag = 3.0 * PI / 2.0;       // -Y
	else
		AImag = 0.0;
}

// ��Ƶ��
int DecFreq(int nFreq)
{
	return nFreq / 101 + 1;
}

/*=========================================== 
	    ʵ�����鲿�ֽ⺯��(һ)     
   ������pSam����������,
   	     pRe��ʵ��, pIm���鲿
		 ָ�������С-nSum�� bHarm-����г����־
		 �㷨��ǰһ�ܲ���ǰ��һ���ܲ�   
============================================*/   
void Decompose(double* pSam,    
			   double* pRe, 
			   double* pIm, 
		       int nSum, 
			   int nSample,
			   int nHarm,
			   int nType, 
			   bool bHarm)
{
	if (nSample <= 0) return;
	if (pSam == 0 || pRe == 0 || pIm == 0) return;

	int i, j;
	double Re(0.0), Im(0.0);

	int nDec(1);
	if (! bHarm) nDec = DecFreq(nSample);	// �򻯼���Ƶ��

	int nCount(nSample);
	if (nType == 1 || nType == 3)		// �벨
		nCount /= 2;

	int nSum1 = nCount / nDec;
	int n = nSample / nDec;
	double Ang;
	if (nType == 0 || nType == 1)		// �ǲ��
	{
		for (i = 0; i < nSum; i ++)
		{
			if (i < nCount) continue;
			Re = 0; Im = 0;
			int k = i;
			for (j = 0; j < nSum1; j ++)
			{
				Ang = nHarm * j * 2.0 * PI / n;
				Re = Re + pSam[k] * cos(Ang) * 2.0 / nSum1;
				Im = Im + pSam[k] * sin(Ang) * 2.0 / nSum1;
				k -= nDec;
			}
			pRe[i] = Re;
			pIm[i] = Im;
		}
	} 
	else if (nType == 2 || nType == 3)	// ���
	{
		for (i = 0; i < nSum; i ++)
		{
			if (i < nCount) continue;
			Re = 0; Im = 0;
			double A = 1.0 / (2.0 * sin((nHarm * 360.0 / n / 2.0) * PI / 180.0));
			int k = i;
			for (j = 0; j < nSum1; j ++)
			{
				Ang = nHarm * j * 2.0 * PI / n;
				Re = Re + A * (pSam[k] - pSam[k-1]) * cos(Ang) * 2.0 / nSum1;
				Im = Im + A * (pSam[k] - pSam[k-1]) * sin(Ang) * 2.0 / nSum1;
				k -= nDec;
			}
			pRe[i] = Re;
			pIm[i] = Im;
		}
	}

	for (i = 0; i < nCount; i ++)      // ����ǰһ���ܲ��Ŀ�����
	{
		pRe[i] = pRe[i+nCount];
		pIm[i] = pIm[i+nCount];
	}
}

/*=========================================== 
	    ʵ�����鲿�ֽ⺯��(��)     
   ������pSam������һ���ܲ�������,
   	     pRe��ʵ��, pIm���鲿
		 nType-�㷨����
   ȱʡ��nHarm = 1; nType = 0; bHarm = False
============================================*/   
void CycleDecompose(double* pSam, 
					double& fRe, 
					double& fIm, 
					int nSample, 
					int nHarm,
					int nType,
					bool bHarm)
{
	if (nSample <= 0 || pSam == 0) return;

	int i;
	double Re(0.0), Im(0.0);


	int nDec(1);
	if (bHarm) nDec = DecFreq(nSample);	// �򻯼���Ƶ��

	int nCount(nSample);
	if (nType == 1 || nType == 3)		// �벨
		nCount /= 2;

	int nSum = nCount / nDec;
	int n = nSample / nDec;
	double Ang;
	if (nType == 0 || nType == 1)		// �ǲ��
	{
		int k = nSample - 1;
		for (i = 0; i < nSum; i ++)
		{
			Ang = nHarm * i * 2.0 * PI / n;
			Re = Re + pSam[k] * cos(Ang) * 2.0 / nSum;
			Im = Im + pSam[k] * sin(Ang) * 2.0 / nSum;
			k -= nDec;
		}
	}
	else if (nType == 2 || nType == 3)	// ���
	{
		double A = 1.0 / (2.0 * sin((nHarm * 360.0 / n / 2.0) * PI / 180.0));
//		double A = 3.83;
		int k = nSample - 1;
		for (i = 0; i < nSum; i ++)
		{
			Ang = nHarm * i * 2.0 * PI / n;
			Re = Re + A * (pSam[k] - pSam[k-1]) * cos(Ang) * 2.0 / nSum;
			Im = Im + A * (pSam[k] - pSam[k-1]) * sin(Ang) * 2.0 / nSum;
			k -= nDec;
		}
	}
	fRe = Re;
	fIm = Im;
}

/*=========================================
	 ������������� --- A��B��C����   
   ������pA, pB, pC����������ָ��
         pSeq����������ָ��  
   	     nType��0:���� 1:���� 2:����
===============================================*/
void CalSequence(double* pA, 
				   double* pB, 
				   double* pC, 
		           double* pSeq, 
				   int nSum, 
				   int nSample,
				   int nType)
{
	if (pA == 0 || pB == 0 || 
		pC == 0 || pSeq == 0 || 
		nSample <= 0) return;
	
	int i, n;
	double fVal;
    
	n = int(nSample / 3);
	for (i = 0; i < nSum; i ++)
	{
		fVal = 0.00;
		switch(nType)
		{
		case 0:    // ����
			fVal = (pA[i] + pB[i] + pC[i]) / 3.0;
			break;
		case 1:   
			if (i <= (nSum - 2 * n -1))
				fVal = (pA[i] + pB[i+n] + pC[i+2*n]) / 3.0;
			break;
		case 2:    // ����
			if (i >= 2*n)
				fVal = (pA[i] + pB[i-n] + pC[i-2*n]) / 3.0;
			break;
		default:
			break;
		}
		pSeq[i] = fVal;
	}
/*	if (nType == 1)
	{
		for (i = nSum-2*n; i < nSum; i++)
		{
			if (i-nSample >= 0)
				pSeq[i] = pSeq[i-nSample];
			else
				pSeq[i] = 0;
		}
	}
	else if (nType == 2)
	{
		for (i = 0; i < 2*n; i++)
		{
			if (i+nSample < nSum) 
				pSeq[i] = pSeq[i+nSample];
			else
				pSeq[i] = 0;
		}
	}*/
}

/*=========================================
  	       �������ʷ�������
  ������pIA, pIB, pIC��ABC�����
        pUA, pUB, pUC��ABC���ѹ
        pPQ2������ָ��
============================================*/
void CalRealPower2(double* pIA, 
				   double* pIB, 
				   double* pIC,    
				   double* pUA, 
				   double* pUB, 
				   double* pUC,
				   double* pPQ2, 
				   int nSum, 
				   int nSample)
{
	if (pIA == 0 || pIC == 0 || 
		pUA == 0 || pUC == 0 || pPQ2 == 0) return;
	bool AC_I_Type, AC_U_Type;
	double *pSeqI, *pSeqU;

	if (pIB == 0)
		 AC_I_Type = true;
	else AC_I_Type = false;
	if (pUB == 0)
		 AC_U_Type = true;
	else AC_U_Type = false;

	pSeqI = new double[nSum];
	pSeqU = new double[nSum];

	if (! AC_I_Type)
	{
		CalSequence(pIA, pIB, pIC,
			        pSeqI, 
					nSum, nSample,
					2);
	}
	else
	{
	}
	if (! AC_U_Type)
	{
		CalSequence(pUA, pUB, pUC,
			        pSeqU,
				    nSum, nSample,
					2);
	}
	else
	{
	}
	
	memset(pPQ2, sizeof(double) * nSum, 0);
	double uRe, uIm, iRe, iIm;
	for (int i = 0; i < nSum; i++)
	{
		if (i >= nSum - nSample) break;
		uRe = 0; uIm = 0;
		iRe = 0; iIm = 0;
		for (int j = 0; j < nSample; j++)
		{
			double fVal = j * 2.0 * PI / (nSample * 1.0);
			double fCos = cos(fVal) * 2.0 / nSample;
			double fSin = sin(fVal) * 2.0 / nSample;
			int n = i + j;
			uRe += pSeqU[n] * fCos;
			uIm += pSeqU[n] * fSin;
			iRe += pSeqI[n] * fCos;
			iIm += pSeqI[n] * fSin;
		}
		pPQ2[i] = 3 * ((uRe * iRe + uIm * iIm) * cos(1.309) 
					 + (uIm * iRe - uRe * iIm) * sin(1.309));
	}

	delete []pSeqI; pSeqI = 0;
	delete []pSeqU; pSeqU = 0;
}

/*=========================================
  	      �й����޹���������
  ������pIA, pIB, pIC��ABC�����
        pUA, pUB, pUC��ABC���ѹ
        pPQ2������ָ�� nType��0:P 1:Q
============================================*/
void CalPower(double *pSamIA, 
				double *pSamIB, 
				double *pSamIC,
				double *pSamUA, 
				double *pSamUB, 
				double *pSamUC,
				double *pVal, 
				int nSum, 
				int nSample,
				int nType)    //nType:0P,1Q
{
	if (pSamIA == 0 || pSamIB == 0 || pSamIC == 0 || 
		pSamUA == 0 || pSamUB == 0 || pSamUC == 0 ||
		pVal == 0) return;

	int i(0), j(0), k(0), nT(0);

	for(i = 0; i < nSum; i ++) 
		pVal[i] = 0.0;

	if(nSample <= 0 || nSum < nSample) return;

	double fT(0.0);
	double Ua, Ub, Uc, Ia, Ib, Ic;

	nT = nSample / 4;//90����λ, ����ÿ�ܲ����������ܱ�4����������£�ƫ�Ƶ���λ�Ϳ��ܲ�׼ȷ������6����Ϊ60�ȡ�

	switch(nType)
	{
	case 0://P
		//һ�������ڵ�ƽ��ֵ
		for(i = nSample; i < nSum; i ++)
		{
			fT = 0;
			for(j = 0; j < nSample; j ++)
			{
				k = i - nSample + j;
				Ua = pSamUA[k];
				Ub = pSamUB[k];
				Uc = pSamUC[k];
    			Ia = pSamIA[k];
				Ib = pSamIB[k];
				Ic = pSamIC[k];
				fT += (Ua*Ia + Ub*Ib + Uc*Ic); 
       		}
			fT = fT / nSample;
			pVal[i] = fT;
			fT = fabs(fT);
		}
		for(i = 0; i < nSample; i ++)
		{
			pVal[i] = pVal[nSample];
		}
		break;
	case 1://Q
		//������ǰ����90�ȼ���
		for(i = nSample; i < nSum-nT; i ++)
		{
			for(j = 0; j < nSample; j ++)
			{
				k = i - nSample + j;
				Ua = pSamUA[k];//i-Sample_Point+j
				Ub = pSamUB[k];
				Uc = pSamUC[k];
				k = i - nSample + nT + j;
    			Ia = pSamIA[k];//i-Sample_Point-N+j
				Ib = pSamIB[k];
				Ic = pSamIC[k];
				fT += (Ua*Ia + Ub*Ib + Uc*Ic); 
       		}
			fT = fT / nSample;
			pVal[i] = fT;
			fT = fabs(fT);
  		}
		for(i = 0; i < nSample; i ++)
		{
			pVal[i] = pVal[nSample+nT];
			pVal[nSum-i-1] = pVal[nSum-nSample-1];
		}
		break;
	default:
		break;
	}
}
/*=========================================
  	      Ƶ�ʷ�������
  ������pSam������ϳ���
        pFreq��Ƶ��ָ��
		fSamFrep������Ƶ��, fBaseFreq����׼Ƶ��
============================================*/
void CalFrequency(double *pSam,
				  double *pFreq,
				  int nSum, 
				  int nSample)
{
	int i;
	int nEnd = nSum - nSample / 4;
	for (i = 0; i < nSum; i++)
	{
		double *p, *p1, *p2;
		p = new double[nSample];
		p1 = new double[nSample];
		p2 = new double[nSample];

		memset(p, nSample * sizeof(double), 0);
        memset(p1, nSample * sizeof(double), 0);
		memset(p2, nSample * sizeof(double), 0);

		int nPos, j;
		for (j = 2; j < nSample; j++)
		{
			nPos = i + j;
			if (i > nSample)
				nPos -= nSample;
			p1[j] = pSam[nPos] + SQRT3 * pSam[nPos-1] + pSam[nPos-2];
			p2[j] = p1[j] + p1[j-2];
			p[j] = p2[j] - p2[j-1] + p2[j-2];
		}

		double fCount(0);
		int nCount(0);
		for (j = 2; j < nSample/2; j++)
		{
			if (fabs(p[j+nSample/4]) < 1) continue;
			double ft, ft0, ff;
			ft0 = 1 / PI * (p[j]+p[j+nSample/2]) / p[j+nSample/4];
			if (fabs(ft0) > 0.12) continue;
			ft = ft0 + ft0 * PI * PI / 24 * ft0 * ft0 * ft0;
			ff = (1 + ft) * 50;
			fCount += ff;
			nCount++;
		}
		if (nCount == 0)
			pFreq[i] = 50;
		else
			pFreq[i] = 100 - fCount / nCount;

		delete []p;
		delete []p1;
		delete []p2;
	}
	// ���1/4���ܲ����㲻׼ȷ
	if (nEnd < 0) return;
	for (i = nEnd; i < nSum; i++)
		pFreq[i] = pFreq[i-nSample];
}

/***************************************
		ƽ��ֵ���㺯��
/***************************************/
double CalAverage(double* pSam, 
				  int nSample)
{
	double nSum(0);
	for (int i = 0; i < nSample; i++)
		nSum += pSam[i];		// ����
	return nSum / nSample;		// ȡƽ��ֵ 	
}

//*********************************************
//		 ����ӵ��迹�������㺯��
// ������pV: ��ѹ
//       pIA, pIB, pIC������
//		 nType-�㷨���� nPhase-���
//       k(����)=((k1*k1*KR+KX)+j*(K1*KX-K1*KR)) / (K1*K1+1)
//       K1 := ctg(ps1)---ps1: �����迹��
//       Z = U/(I+K*I0)
//       Line: ��·����
// �����pRe������
//       pIm���翹
//*********************************************
void CalSingleZ(double* pV,
				double* pIA,
				double* pIB,
				double* pIC,
				double* pRe,
				double* pIm,
				const LINEPARA Line,
				int nSum,
				int nSample,
				int nPhase,
				int nType)
{
	double *pIre0, *pIim0, *pIre, *pIim, *pVre, *pVim, *pI0;
	pIre0 = new double[nSum];
	pIim0 = new double[nSum];
	pIre = new double[nSum];
	pIim = new double[nSum];
	pVre = new double[nSum];
	pVim = new double[nSum];
	pI0 = new double[nSum];
	memset(pIre0, nSum * sizeof(double), 0);
	memset(pIim0, nSum * sizeof(double), 0);
	memset(pIre, nSum * sizeof(double), 0);
	memset(pIim, nSum * sizeof(double), 0);
	memset(pVre, nSum * sizeof(double), 0);
	memset(pVim, nSum * sizeof(double), 0);
	memset(pI0, nSum * sizeof(double), 0);

	double *pI;
	switch(nPhase)
	{
	case 0:	pI = pIA;
	case 1: pI = pIB;
	case 2: pI = pIC;
	}
	Decompose(pV, pVre, pVim, nSum, nSample, 1, nType); 
	Decompose(pI, pIre, pIim, nSum, nSample, 1, nType);

	// �ֽ���������
	CalSequence(pIA, pIB, pIC, pI0, nSum, nSample, 0);
	Decompose(pI0, pIre0, pIim0, nSum, nSample, 1, nType);

	// ��K---���򲹳�ϵ��
	double k1 = 1 / tan(Line.fPs1 * PI / 180);
	double Re = (k1 * k1 * Line.fKR + Line.fKX) / (1 + k1 * k1);
	double Im = (k1 * Line.fKX - k1 * Line.fKR) / (1 + k1 * k1);
	for (int i = 0; i < nSum; i++) 
	{
		pRe[i] = 0.0;
		pIm[i] = 0.0;
		if (pIre[i] * pIre[i] + pIim[i] * pIim[i] > 1e-6) 
		{
			double UR = pVre[i];
			double UI = pVim[i];
			double IR = pIre[i] + Re * pIre0[i] - Im * pIim0[i];
			double II = pIim[i] + Re * pIim0[i] - Im * pIre0[i];
			double f1 = sqrt(UR * UR + UI * UI) / sqrt(IR * IR + II * II);
			double f2 = atan2(UI, UR) - atan2(II, IR);
			pRe[i] = f1 * cos(f2);
			pIm[i] = f1 * sin(f2);
		}
	}
	delete []pIre0; pIre0 = 0;
	delete []pIim0; pIim0 = 0;
	delete []pIre; pIre = 0;
	delete []pIim; pIim = 0;
	delete []pVre; pVre = 0;
	delete []pVim; pVim = 0;	
	delete []pI0; pI0 = 0;	
}

/***************************************
		ͻ�������㺯��
   ������pIn-��������ָ�� 
         pOut-�������ָ��
         nSum-�ܵ��� 
		 nChange-ͻ��������
/***************************************/
void CalBreak(double* pIn, 
			  double* pOut,
			  int nSum,
			  int nChange)
{
	if (nChange < 0 || nChange > nSum) return;
	memset(pOut, nSum * sizeof(double), 0);
	for (int i = nChange; i < nSum; i ++)
		pOut[i] = pIn[i] - pIn[i-nChange];
}

/*=========================================== 
	    �迹���㸨������
   ������pVol����ѹ, pCur������,
         pRe���迹ʵ���� pIm���迹�鲿
===========================================*/    
void CalZ(double* pVol, 
		  double* pCur,    
	      double* pRe, 
		  double* pIm, 
	      int nSum, 
		  int nSample, 
		  int nType) 
{
	if (pVol == 0 || pCur == 0 || 
		pRe == 0 || pIm == 0) return;

	double fVal, fAng;
	double *pReI, *pImI, *pReU, *pImU;

	pReI = new double[nSum];
	pImI = new double[nSum];
	pReU = new double[nSum];
	pImU = new double[nSum];

	Decompose(pCur,
			  pReI, 
			  pImI,
			  nSum, 
			  nSample,
			  1,
			  nType);
    Decompose(pVol,
 		      pReU, 
 			  pImU,
 			  nSum, 
			  nSample,
			  nType);

	for (int i = 0; i < nSum; i ++)
	{
		pRe[i] = 0.0;
		pIm[i] = 0.0;
		if ((pReI[i] * pReI[i] + pImI[i] * pImI[i]) > 1E-6)
		{
			fVal = sqrt(pReU[i] * pReU[i] + pImU[i] * pImU[i]) 
				 / sqrt(pReI[i] * pReI[i] + pImI[i] * pImI[i]);
			fAng = atan2(pImU[i], pReU[i]) - atan2(pImI[i], pReI[i]);
			pRe[i] = fVal * cos(fAng);
			pIm[i] = fAng * sin(fAng);
		}		
	}
	delete []pReI; pReI = 0;
	delete []pImI; pImI = 0;
	delete []pReU; pReU = 0;
	delete []pImU; pImU = 0;
}

//*********************************************
//		 ����ӵ��迹�������㺯��(����)
// ������pV: ��ѹ
//       pIA, pIB, pIC������
//		 nType-�㷨���� nPhase-���
//       k(����)=((k1*k1*KR+KX)+j*(K1*KX-K1*KR)) / (K1*K1+1)
//       K1 := ctg(ps1)---ps1: �����迹��
//       Z = U/(I+K*I0)
//       Line: ��·����
// �����Re������
//       Im���翹
//*********************************************
void CalCycleSingleZ(double* pV,
				double* pIA,
				double* pIB,
				double* pIC,
				double& Re,
				double& Im,
				const LINEPARA Line,
				int nSample,
				int nPhase,
				int nType)
{
	double *pI0;
	pI0 = new double[nSample];
	memset(pI0, nSample * sizeof(double), 0);

	double *pI;
	switch(nPhase)
	{
	case 0:	pI = pIA;
	case 1: pI = pIB;
	case 2: pI = pIC;
	}
	double U_Re, U_Im, I_Re, I_Im, I_Re0, I_Im0;
	CycleDecompose(pV, U_Re, U_Im, nSample);
	CycleDecompose(pI, I_Re, I_Im, nSample);
	// �ֽ���������
	CalSequence(pIA, pIB, pIC, pI0, nSample, nSample, 0);
	CycleDecompose(pI0, I_Re0, I_Im0, nSample);

	// ��K---���򲹳�ϵ��
	double k1 = 1 / tan(Line.fPs1 * PI / 180);
	double k_Re = (k1 * k1 * Line.fKR + Line.fKX) / (1 + k1 * k1);
	double k_Im = (k1 * Line.fKX - k1 * Line.fKR) / (1 + k1 * k1);
	Re = 0.0;
	Im = 0.0;
	if (I_Re * I_Re + I_Im * I_Im > 1e-6)
	{
		double IR = I_Re + k_Re * I_Re0 - k_Im * I_Im0;
		double II = I_Im + k_Re * I_Im0 - k_Im * I_Re0;
		double f1 = sqrt(U_Re * U_Re + U_Im * U_Im) / sqrt(IR * IR + II * II);
		double f2 = atan2(U_Im, U_Re) - atan2(II, IR);
		Re = f1 * cos(f2);
		Im = f1 * sin(f2);
	}
	delete []pI0; pI0 = 0;	
}

/*=========================================== 
	    �迹���㸨������(����)
   ������pVol����ѹ, pCur������,
         Re���迹ʵ���� Im���迹�鲿
===========================================*/    
void CalCycleZ(double* pVol, 
			   double* pCur,    
			   double& Re, 
			   double& Im, 
			   int nSample, 
			   int nType) 
{
	if (pVol == 0 || pCur == 0) return;

	double fVal, fAng;
	double U_Re, U_Im, I_Re, I_Im;
	CycleDecompose(pVol, U_Re, U_Im, nSample);
	CycleDecompose(pCur, I_Re, I_Im, nSample);

	Re = 0;
	Im = 0;
	if (I_Re * I_Re + I_Im * I_Im > 1e-6)
	{
		fVal = sqrt(U_Re * U_Re + U_Im * U_Im) 
			 / sqrt(I_Re * I_Re + I_Im * I_Im);	
		fAng = atan2(U_Im, U_Re) - atan2(I_Im, I_Re);
		Re = fVal * cos(fAng);
		Im = fVal * sin(fAng);
	}
}

/*=========================================
	 ������������� --- A��C����   
   ������pA, pC����������ָ��
         pSeq����������ָ��  
   	     nType��0:���� 1:���� 2:����
===============================================*/
void CalSequence2(double* pA, 
				  double* pC, 
				  double* pSeq, 
				  int nSum, 
				  int nSample,
				  int nType)
{
	if (pA == 0 || pC == 0 || 
		pSeq == 0 || nSample <= 0) return;
	int n = nSample / 6;
	double f = sqrt((double)3) / 3.0;
	for (int i = 0; i < nSum; i++)
	{
		double ff = 0;
		switch (nType)
		{
		case 0:
			ff = (pA[i] + pC[i]) * f;
			break;
		case 1:
			if (i >= n) 
				ff = (pA[i] + pC[i-n]) * f;
			break;
		case 2:
			if (i < nSum - n)
				ff = (pA[i] + pC[i+n]) * f;
			break;
		}
		pSeq[i] = ff;
	}
}