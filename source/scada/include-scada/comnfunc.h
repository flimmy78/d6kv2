/********************************************************************
*																	*
*					���ÿ⺯��	gepeng 2002									*
*																	*
*	01. ð������(CommSort).											*
*	02. ��������(CommQSort).										*
*	03. ���ַ�����(CommBinFind).									*
*	04. ע��λ��DS3100\DLL\Ŀ¼�µĿؼ�(CommRegisterOcx)			*
*	05. ����LRCУ����(����żУ��)(ComLRC)							*
*																	*
********************************************************************/
#if !defined(_025_3343030_COMMFUNC)
#define _025_3343030_COMMFUNC

#if !defined(COMFACE_DLL)
#define _COMFACE_PORT _declspec(dllimport)
#else
#define _COMFACE_PORT _declspec(dllexport)
#endif


/////////////////////////////////////////////////////////////////////
//ð�����򣬽������������������顣
//pObj�����׵�ַ, objNum�����С
//���ô˺��������ز����� "<", ">"
template <class T>
inline void CommSort(T * pObj, int objNum)
{
	int i,j,k,m;
	T obj;
	k = 0; m = objNum-1;
	if (pObj==NULL || objNum <= 0)
		return;
	while(k<m)
	{
		//��ǰ����ɨ�����У��Ƚ�������Ŀ�Ĵ�С
		//���������򽻻���ʹ����߻������е����
		j = m-1; m = 0;
		for(i=k;i<=j;i++)
		{
			if(pObj[i] > pObj[i+1])
			{	
				obj = pObj[i];
				pObj[i] = pObj[i+1];
				pObj[i+1] = obj;
				m=i;
			}
		}
		//�Ӻ�ǰɨ��ʣ�µ����У��Ƚ�������Ŀ�Ĵ�С
		//���������򽻻���ʹ��С�߻������е���ǰ
		j=k+1;k=0;
		for(i=m; i>= j; i--)
		{
			if(pObj[i-1] > pObj[i])
			{
				obj = pObj[i];
				pObj[i] = pObj[i-1];
				pObj[i-1] = obj;
				k=i;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////
//�������������������飬�����ǰ��ð�������ٶ�Ҫ�졣�ʺϱȽϴ������
//pObj�����׵�ַ, objNum�����С
//���ô˺������ṹ��������, �����ز����� ">", ">=", "<", "<="
template <class T>
inline void CommQSort(T * pObj, int objNum)
{
	int i0, *i, m;
	T *s;
	i=&i0;
	if(objNum>10)//�������10���ָ��������
	{
		CommSplit(pObj, objNum, i);//�ָ��
		m=i0;
		CommQSort(pObj, m);
		s = pObj + (i0 + 1);
		m = objNum - (i0 + 1);
		CommQSort(s, m);
	}
	else		//����ֱ����ð�ݷ�����
		CommSort(pObj, objNum);
}

template <class T>
inline void CommSplit(T * p, int n, int *m)
{
	int i, j, k, l;
	T t;
	i = 0; j = n-1;
	k = (i+j)/2;
	if ((p[i] >= p[j]) && (p[j] >= p[k])) l = j;
	else if ((p[i] >= p[k]) && (p[k] >= p[j])) l = k;
	else l = i;
	t = p[l]; p[l] = p[i];
	while(i!=j)
	{
		while((i<j)&&(p[j]>=t))j=j-1;
		if(i<j)
		{
			p[i]=p[j];
			i=i+1;
			while((i<j)&&(p[i]<=t)) i=i+1;
			if(i<j)
			{
				p[j]=p[i];
				j=j-1;
			}
		}
	}
	p[i] = t;
	*m = i;
}

///////////////////////////////////////////////////////////
//���ַ���ָ�������������е�λ��
//objָ������, pObj��������, objNum�����С
//����ֵ < 0 ����δ�ҵ�, ���򷵻ض����������е�λ��
template <class T>
inline int CommBinFind(T obj, T * pObj, int objNum)
{
	int	wObjNum = objNum, wTempNum = 0,
		wMatchNo = -1, wObjNo = 0;
	if (pObj == NULL || objNum < 1) 
		return -1;
	do
	{
		wTempNum = wObjNum;

		int wTempNo = wObjNo + wObjNum/2;
		T tmpObj = pObj[wTempNo];
		if (tmpObj == obj)
		{
			wMatchNo = wTempNo;
		}
		else if (tmpObj > obj)
		{
			wTempNum = wTempNo - wObjNo;
		}
		else if (obj < tmpObj)
		{
			wTempNum = wObjNum - (wTempNo - wObjNo);
			wObjNo = wTempNo;
		}

		if (wTempNum == wObjNum) break;
		wObjNum = wTempNum;
	}while (wObjNum > 0);

	return wMatchNo;
}

/////////////////////////////////////////////////////////////////////
//ע��λ��DS3100\DLL\Ŀ¼�µĿؼ�
//������OcxName �ؼ���
//����ֵ��< 0 ���ɹ� ����ɹ� 
_COMFACE_PORT int CommRegisterOcx(char *OcxName);

////////////////////////////////////////////////////////////////////
// ����LRCУ����(����żУ��)		
// BufLen:���Ĵ�С
// Buf:��������
// ����ֵ:һ�ֽ�У����
_COMFACE_PORT BYTE CommLRC(int BufLen, BYTE * Buf);

#endif