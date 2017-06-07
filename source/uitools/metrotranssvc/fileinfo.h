#ifndef FILEINFO_H
#define FILEINFO_H
#include <QListWidget>

#include "db/dbapi.h"
#include "rdb/rdbapi.h"

#include "globaldef.h"


//����������
typedef struct
{
	int	No;									//���
	char	code[CODE_LEN];			//ң�Ŵ���
	unsigned char	state;				//ң��״̬
	int	blockcond_no;					//����������
	int	action_no;						//�������
	unsigned char real_state;	
}TriggerCond;

//����������
typedef struct
{
	int	block_no;							//����������
	int	No;									//���
	char	code[CODE_LEN];			//ң�Ŵ���
	char	table[CODE_LEN];			//ң�ű����
	unsigned char	state;				//ң��״̬
	unsigned char real_state;	
}BlockCond;

//������
typedef struct
{
	int	groupno;							//���
	int	No;									//���
	int	oper_type;						//��������(0:yk   1:yx)
	char	code[CODE_LEN];			//ң�Ŵ���
	char	table[CODE_LEN];			//ң�ű����
	unsigned char	state;				//ң��״̬
	unsigned char real_state;
	char msg[255];
}ActionInfo;


class File_info
{
public:
	int Init();

	int read_triggercond_para();									//��ȡ����������
	int read_blockcond_para(int blockcondno);			//��ȡ����������
	int read_action_para();												//��ȡ������

	int read_action_realstate();		//�������б�ǰʵʱֵ
	int read_blockcond_realstate();		//�������б�ĵ�ǰʵʱֵ

	int get_actionlist_by_actionno(int actionno);		//��ȡ��ǰ���������ĵĿ������У�������������
	File_info();
	virtual ~File_info();

	TriggerCond  *m_faultcond_list;			//���������б�
	BlockCond  *m_blockcond_list;			//���������б�
	ActionInfo  *m_action_list;					//�����б�
	int		m_triggercond_num;		//������������
	int		m_action_num;				//���п�������
	int		m_blockcond_num;			//������������
	ActionInfo  *m_pControlList;			//��ŵĿ�������
	int	m_pControlNum ;
};
#endif