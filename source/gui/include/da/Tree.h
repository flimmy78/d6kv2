//���ṹ��ͨ��ģ��
//���еĽڵ�����"PROPERTY",������Ӧ�������������ԣ�Ψһ��
//									written by xiaowq 2004-09-29
////////////////////////////////////////////////////////////////////////////
#ifndef _TREETREE_H_
#define _TREETREE_H_

template <class PROPERTY,int MAX_CHILD>
class CTree
{
	struct _NODE
	{
		PROPERTY propterty;
		struct _NODE* pchild[MAX_CHILD];
	}
public:
	CTree(){
		pRoot = NULL;
	}
	~CTree(){
	}
	int Add(PROPERTY father,PROPERTY self);
public:
	struct _NODE* pRoot;
};

template <class PROPERTY,int MAX_CHILD>
int CTree<PROPERTY,MAX_CHILD>::Add(PROPERTY father,PROPERTY self)
{
	struct _NODE* pNew = new struct _NODE;
	pNew->propterty = self;
	memset(pNew->pchild,0,MAX_CHILD*sizeof(struct _NODE*));
	if(pRoot == NULL)
		pRoot = pNew;
	else
	{

	}
	return 0;
}

#endif