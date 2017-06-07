#ifndef  __saveflag_h
#define  __saveflag_h

//���̱�־����
#define  HISDATA_REAL           1       //ʵʱֵ
#define  HISDATA_LOCALSET       2       //�����˹�����
#define  HISDATA_REMOTESET      3       //Զ���˹�����
#define  HISDATA_DEADVAL        4       //����
#define  HISDATA_TERMINALERROR  5       //�ն˹���
//#define  HISDATA_FTUERROR       6       //FTU����
#define  HISDATA_NETERROR       7       //�������
#define  HISDATA_OVERHIGHTLMT   8       //ң����Խ��
#define  HISDATA_OVERLOWLMT     9       //ң����Խ��
#define  HISDATA_OVERVOLUP      10      //��ѹԽ���ϸ�����
#define  HISDATA_OVERVOLDOWN    11      //��ѹԽ���ϸ�����
#define  HISDATA_MANUPDATE	    12      //�˹��޸�
#define  HISDATA_MANINSERT	    13      //�˹�����

#define	 HISDATA_PLAN						51		//�ƻ�ֵ
#define  HISDATA_KWHTABCODE			52		//��ȱ���-Modify Plan
#define  HISDATA_KWHRAWDATA			53		//���ԭʼ����


#define  HISDATA_REALMAX        55      //���ֵ

#define  HISDATA_REALMIN        56      //��Сֵ
#define  HISDATA_REALHGL        57      //�ϸ���

#define  HISDATA_AVERAGE        58      //ƽ��ֵ
#define  HISDATA_PQRATE         59      //������

#define  HISDATA_MAXTIME        60      //���ʱ ��ʱδ��
#define  HISDATA_MINTIME        61      //��Сʱ

#define  HISDATA_OVHHCOUNT      62      //Խ�����޴���
#define  HISDATA_OVHCOUNT       63      //Խ���޴���
#define  HISDATA_DOWNLCOUNT     64      //Խ���޴���
#define  HISDATA_DOWNLLCOUNT    65      //Խ�����޴���
#define  HISDATA_OVHHTIME       66      //Խ������ʱ��
#define  HISDATA_OVHTIME        67      //Խ����ʱ��
#define  HISDATA_DOWNLTIME		68      //Խ����ʱ��
#define  HISDATA_DOWNLLTIME		69      //Խ������ʱ��
#define  HISDATA_DAYKWHSD1      70      //1ʱ���յ���(ƽ)
#define  HISDATA_DAYKWHSD2      71      //2ʱ���յ���(��)
#define  HISDATA_DAYKWHSD3      72      //3ʱ���յ���(��)
#define  HISDATA_DAYKWHSD4      73      //4ʱ���յ���(��)
#define  HISDATA_DAYKWHSD5      74      //5ʱ���յ���(����)
#define  HISDATA_DAYKWHSD6      75      //6ʱ���յ���

#define  HISDATA_DAYKWHTTL      79      //���ۼƵ���

#define  HISDATA_DAYDEADTIME    80      //������ʱ��
#define  HISDATA_DAYSTOPTIME    81      //��ͣ��ʱ��

//ע�⣺ 83---90  ��PASʹ��
#define  HISDATA_FORECAST		83		//����Ԥ������
	
//����������ʷͳ�ƴ������ı�־
#define  HISDATA_DAYS_MAX        150      //��ͳ�����ֵ
#define  HISDATA_DAYS_MIN        151      //��ͳ����Сֵ
#define  HISDATA_DAYS_HGL        152      //��ͳ�ƺϸ���
#define  HISDATA_DAYS_AVERAGE    153      //��ͳ��ƽ��ֵ
#define  HISDATA_DAYS_PQRATE     154      //��ͳ�Ƹ�����
#define  HISDATA_DAYS_MAXTIME    155      //��ͳ�����ʱ ��ʱδ��
#define  HISDATA_DAYS_MINTIME    156      //��ͳ����Сʱ

// 157��160�Ǹ��ݴ��̵���ң��������е�������ֵ����ͳ��
#define  HISDATA_DAYS_OVHHCOUNT       157 //��ͳ��Խ�����޵�������ʱδ�ã�
#define  HISDATA_DAYS_OVHCOUNT        158 //��ͳ��Խ���޵���
#define  HISDATA_DAYS_DOWNLCOUNT      159 //��ͳ��Խ���޵���
#define  HISDATA_DAYS_DOWNLLCOUNT     160 //��ͳ��Խ�����޵�������ʱδ�ã�
#define  HISDATA_DAYS_OVHHTIME        161 //��ͳ��Խ������ʱ�䣨��ʱδ�ã�
#define  HISDATA_DAYS_OVHTIME         162 //��ͳ��Խ����ʱ��
#define  HISDATA_DAYS_DOWNLTIME		  163 //��ͳ��Խ����ʱ��
#define  HISDATA_DAYS_DOWNLLTIME	  164 //��ͳ��Խ������ʱ�䣨��ʱδ�ã�

// 165��169�Ǹ��ݵ�ѹ�Ĵ��̵����ѹ��ֵ���е�������ֵ����ͳ��
#define  HISDATA_DAYS_VQOHCOUNT       165 //�յ�ѹ����Խ���޵���
#define  HISDATA_DAYS_VQOLCOUNT       166 //�յ�ѹ����Խ���޵���
#define  HISDATA_DAYS_VQOHTIME        167 //�յ�ѹ����Խ����ʱ��
#define  HISDATA_DAYS_VQOLTIME        168 //�յ�ѹ����Խ����ʱ��
#define  HISDATA_DAYS_VQHGL           169 //�յ�ѹ�����ϸ���
#define	 HISDATA_DAYS_JLDL            170 //�ս�������
#define  HISDATA_DAYS_KFDL            171 //�տ۷�����
#define  HISDATA_DAYS_JFDL			  172 //�ս�������
#define  HISDATA_DAYS_ZQL			  180 //�ռƻ�׼ȷ��

#define  HISDATA_UPPERTDDATA	      200 // �ƻ�����
#define  HISDATA_LOWERTDDATA	      201 // �ƻ�����

///////// �豸����ͳ�Ʊ����豸���͵Ķ��� ////////////////

#define HISDEV_BREAK             300     //�����������
#define HISDEV_TERMINAL          10000   //RTU�������
#define HISDEV_CHAN              10002   //ͨ���������
#define HISDEV_ANALOG_OL         10003   //ģ����Խ�����
#define HISDEV_HOST				 10004   //�����������




#endif
