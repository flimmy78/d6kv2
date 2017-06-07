#ifndef _dev_def_h
#define _dev_def_h

//////////////// ��վ�����豸�����ȶ��� ////////////////
#define SUBSTNAME_LEN       24		//��վ������
#define POWDEVNAME_LEN      24		//�豸������
#define OBJTYPENAME_LEN     4		//��������������


#define	OBJECTNAME_LEN		24		//����������
#define OBJDESCRIBE_LEN		40		//������������
#define OBJDESCRIBE_LLEN		64		//������������
#define EVENT_LEN          128
//////////////// �豸���Ͷ��� //////////////////////

#define  DEVTYPE_NULL				0   // ��Ч����
#define  DEVTYPE_BREAKER			1   // ����
#define  DEVTYPE_SWITCH				2   // ��բ
#define  DEVTYPE_MELT			    3	// �۶���
#define  DEVTYPE_HANDBREAK		    4	// �ֳ�
#define  DEVTYPE_TRANSFORMER		5   // ����ѹ���������飩
#define  DEVTYPE_PDTRANSFORMER		6   // ����ѹ���������飩
#define  DEVTYPE_CAPACITOR			7   // ������
#define  DEVTYPE_REACTOR			8   // �翹��
#define  DEVTYPE_COIL				9	// ������Ȧ	
#define  DEVTYPE_PT					10	// ��ѹ������
#define  DEVTYPE_CT					11	// ����������
#define  DEVTYPE_ARRESTER			12	// ������
#define  DEVTYPE_GNDSWITCH			13  // �ӵ���
#define  DEVTYPE_LINE    		    14	// �ܿ���--------��·
#define  DEVTYPE_CABLE		        15	// ����----------��·
#define  DEVTYPE_BUS				16  // ĸ��
#define  DEVTYPE_GENERATOR			17  // �����
#define  DEVTYPE_WELD				18  // ����
#define  DEVTYPE_LINKLINE			19  // ������
#define  DEVTYPE_JOINT  			20  // ���� ���ӵ�
#define  DEVTYPE_LOAD				21  // ����
#define  DEVTYPE_PROTECT			22  // ����װ��
#define  DEVTYPE_MLBREAKER			23  // ĸ������
#define  DEVTYPE_ZBR				24  // ���迹֧·ZBR

#define  DEVTYPE_DECK				48  // ��
#define  DEVTYPE_CO					49  // ʡ��
#define  DEVTYPE_DISTRICT			50  // ����
#define  DEVTYPE_OTHERDEV			51  // �����豸
#define  DEVTYPE_SUBSTATION			52  // ��վ
#define  MAXDEVTYPE         		100	// �����豸������ͺ�

//////////////// �豸���������� //////////////////////
#define  DEVTYPENM_BREAKER			'K' // ����
#define  DEVTYPENM_SWITCH			'D' // ��բ
#define  DEVTYPENM_LINE				'X' // �ܿ���--------��·
#define  DEVTYPENM_CABLE			'x' // ����----------��·
#define  DEVTYPENM_BUS				'M' // ĸ��
#define  DEVTYPENM_TRANSFORMER		'B' // ����ѹ��----��ѹ��
#define  DEVTYPENM_PDTRANSFORMER	'b' // ����ѹ��----��ѹ��
#define  DEVTYPENM_GENERATOR		'F' // �����

#define  DEVTYPENM_CAPACITOR		'C' // ������
#define  DEVTYPENM_CT				'I' // ����������
#define  DEVTYPENM_PT				'U' // ��ѹ������
#define  DEVTYPENM_REACTOR			'R' // �翹��
#define  DEVTYPENM_ARRESTER			'L' // ������
#define  DEVTYPENM_PROTECT			'P' // ����װ��
#define  DEVTYPENM_COIL				'Q' // ������Ȧ	
#define  DEVTYPENM_LOAD				'H' // ����
#define  DEVTYPENM_JOINT			'J' // ���ӵ�
#define  DEVTYPENM_MLBREAKER		'l' // ĸ������
#define  DEVTYPENM_MELT				'r' // �۶���
#define  DEVTYPENM_GNDSWITCH		'G' // �ӵ���
#define  DEVTYPENM_LINKLINE			'j' // ������
#define  DEVTYPENM_ZBR				'Z' // ���迹֧·ZBR

#define  DEVTYPENM_DISTRICT			'd' // ����
#define  DEVTYPENM_OTHERDEV			'O' // �����豸
#define  DEVTYPENM_SUBSTATION		'S' // ��վ


///////////////// ���ص�բ������ �� �������� ��ֵ���� /////////

#define  BREAKTYPE_NULL				 0  // ��Ч����
#define  BREAKTYPE_NORMAL_SW		 1  // ��ͨ��բ
#define  BREAKTYPE_BYPASS_SW		 2  // ��·��բ
#define  BREAKTYPE_BUSLINK_SW		 3  // ĸ����բ
#define  BREAKTYPE_GROUND_SW		 4  // �ص�
#define  BREAKTYPE_NORMAL_BK		 5  // ��ͨ����
#define  BREAKTYPE_CAR_BK			 6  // С������
#define  BREAKTYPE_BUSLINK_BK		 7  // ĸ������
#define  BREAKTYPE_BUSLINK_CAR_BK    8  // ĸ��С������
#define  BREAKTYPE_BYBUS_BK			 9  // ��ĸ����
#define  BREAKTYPE_BYBUSLINK_BK		10 // ��ĸ��ĸ������

#define  BREAKTYPE_VIRTUAL_SW		99 // �鵶բ

////////////////������������ �� ���������� ��ֵ���� //////////
#define  CAPA_SHUNT					 0   //����
#define  CAPA_SERIES				 1   //����

///////////////�翹�������� �� �翹������ ��ֵ���� ///////////
#define  REAC_SHUNT					 0   //����
#define  REAC_SERIES			 	 1   //����

///////////////��ѹ�������� �� ��ѹ������ ��ֵ����  ///////////
#define  TRAN_TWO					 1   //������
#define  TRAN_THREE					 2   //������
#define  TRAN_SELF					 3   //��ź��
#define  TRAN_FOUR					 4   //������


///////////////ĸ�߲����� �� ������� ��ֵ����  ///////////////
#define  MXTYPE_GZ					 0  //����ĸ�ߣ�����ĸ��һ��
#define  MXTYPE_BY					 1  //����ĸ�ߣ�����ĸ�߶���
#define  MXTYPE_PL                   2  //��·ĸ�� 
#define  MXTYPE_BYPL                 3  //��·�汸��ĸ�� 
#define  MXTYPE_INVALID			  0xff  //��������


////////////// ����ӳ��� �� Ӧ����� ��ֵ����  ///////////////
#define  MAPYC_PUBLIC				 0  // ����
#define  MAPYC_SCADA				 1  // SCADA
#define  MAPYC_PAS					 2  // PAS



//add for distribution network topo
//��վ����
enum StationType
{
	STATIONTYPE_SUBSTATION = 0,			//���վ
	STATIONTYPE_DISTRIBUTION			//���վ
};

enum BreakerType
{
	BREAKERTYPE_NORMAL = 0,		//��ͨ����
	BREAKERTYPE_CONNECTION,		//���翪��
	BREAKERTYPE_LOAD			//���ɿ���
};

enum SwitchType
{
	SWITCHTYPE_ISOLATOR = 0,	//���뵶բ
	SWITCHTYPE_GROUNDING		//�ӵص�բ
};

//�豸Ͷ��״̬
enum DevServiceStatus
{
	DEVSERVICESTATUS_INSERVICE = 0,		//�豸�ڷ�����
	DEVSERVICESTATUS_OUTSERVICE,		//�豸�˳�����	
	DEVSERVICESTATUS_UNSERVICE			//�豸δͶ������
};

//�豸ʵ������״̬
enum DevRunState
{
	DEVRUNSTATE_POWERCUT = 0,				//ʧ��
	DEVRUNSTATE_POWER_BEGIN = 1,			//���磬���ߺ���ʼ
	DEVRUNSTATE_POWER_END = 4999,			//���磬���ߺŽ���
	DEVRUNSTATE_LOOPNET = 10001,			//����
	DEVRUNSTATE_TRANSFER,					//����ת��
	DEVRUNSTATE_GROUNDING = 10100			//�豸�ӵ�

};

//�豸ʧ��ԭ��
enum LossPowerReason
{
	NORMAL_POWERCUT =0,     //����ͣ��
	FAULT_POWERCUT = 1,		//����ʧ��
	PLANNED_POWERCUT,		//�ƻ�ͣ��
	TEMPORARY_POWERCUT,		//��ʱͣ��
	PROJECT_POWERCUT		//����ͣ��
};

//�豸��ɫ״̬���
enum devColorState
{	
	DEVCOLORSTATE_UNSERVICE = -1,				//�豸δͶ������
	DEVCOLORSTATE_COLORDEFAULT = 0,
	DEVCOLORSTATE_COLORBYFEEDER_BEGIN = 1,	    //�豸�����ߺ���ɫ��ʼ
	DEVCOLORSTATE_COLORBYFEEDER_END = 4999,		//�豸�����ߺ���ɫ����
	DEVCOLORSTATE_COLORBYST_BEGIN = 5000,		//�豸�����վ��ɫ��ʼ
	DEVCOLORSTATE_COLORBYST_END =5999,			//�豸�����վ��ɫ����
	DEVCOLORSTATE_LOOPNET = 10001,				//�豸����
	DEVCOLORSTATE_TRANSFER = 10002,				//�豸����ת��
	DEVCOLORSTATE_GROUNDING = 10100,			//�豸�ӵ�
	DEVCOLORSTATE_NORMAL_POWERCUT = 10200,      //�豸����ͣ��
	DEVCOLORSTATE_FAULT_POWERCUT,		        //�豸����ͣ��
	DEVCOLORSTATE_PLANNED_POWERCUT,				//�豸�ƻ�ͣ��
	DEVCOLORSTATE_TEMPORARY_POWERCUT,			//�豸��ʱͣ��
	DEVCOLORSTATE_PROJECT_POWERCUT,				//�豸����ͣ��
	DEVCOLORSTATE_SUPPLY_AREA = 10300,			//���緶Χ
	DEVCOLORSTATE_SUPPLY_ROUTE ,				//����·��
	DEVCOLORSTATE_OUTSERVICE = 30000			//�豸�˳����У�����
};
//end

#endif
