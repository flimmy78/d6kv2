#ifndef PROTOCOL_CDT92_H_
#define PROTOCOL_CDT92_H_
#include <QList>
#include <QListIterator>
#include "msgview_string.h"

static QString cdt_header =  _C2Q("(ͬ����6�ֽ�)");
static QString cdt_header_error =  _C2Q("(ͬ���ִ���)");
static QString cdt_crc_error =  _C2Q("(У�������)");
static QString cdt_len_error =  _C2Q("(���ĳ��ȴ���)");

//static QString remind_space = " ";
//static QString remind_leftkuohao = "(";
//static QString remind_rightkuohao = ")";

static QString cdt_control_field =  _C2Q("(�����ֽ�)");
static QString cdt_frame_sort =  _C2Q("֡���:");
static QString cdt_frame_len =  _C2Q("��Ϣ����:");
static QString cdt_source_addr =  _C2Q("Դվ��ַ:");
static QString cdt_target_addr =  _C2Q("Ŀ��վ��ַ:");
static QString cdt_frame_crc =  _C2Q("У����");

//֡���
static QString cdt_frame_a =  _C2Q("��Ҫң��A֡");
static QString cdt_frame_b =  _C2Q("��Ҫң��B֡");
static QString cdt_frame_c =  _C2Q("һ��ң��C֡");
static QString cdt_frame_di =  _C2Q("ң��״̬");
static QString cdt_frame_ps =  _C2Q("�����������ֵ");
static QString cdt_frame_soe =  _C2Q("SOE��Ϣ");
static QString cdt_frame_ctrl_sel =  _C2Q("ң��ѡ��");
static QString cdt_frame_ctrl_exe =  _C2Q("ң��ִ��");
static QString cdt_frame_ctrl_can =  _C2Q("ң�س���");
static QString cdt_frame_ctrl_con =  _C2Q("ң�ط�У");
static QString cdt_frame_updown_sel =  _C2Q("����ѡ��");
static QString cdt_frame_updown_exe =  _C2Q("����ִ��");
static QString cdt_frame_updown_cle =  _C2Q("��������");
static QString cdt_frame_idi_set =  _C2Q("�趨����");
static QString cdt_frame_clock_set =  _C2Q("����ʱ��");
static QString cdt_frame_clock_con =  _C2Q("����ʱ��У��ֵ");
static QString cdt_frame_clock_sum =  _C2Q("�ٻ���վʱ��");
static QString cdt_frame_reset =  _C2Q("��������");
static QString cdt_frame_brod =  _C2Q("�㲥����");


//������
static QString cdt_func_yx_begin =  _C2Q("ң����ʼ���:");
static QString cdt_func_yc_begin =  _C2Q("ң����ʼ���:");
static QString cdt_func_yk_begin =  _C2Q("ң����ʼ���:");
static QString cdt_func_ym_begin =  _C2Q("ң����ʼ���:");
static QString cdt_func_soe_begin =  _C2Q("soe��ʼ���:");

static QString cdt_func_yx_value =  _C2Q("ң��ֵ");
static QString cdt_func_yc_value =  _C2Q("ң��");
static QString cdt_func_yk_value =  _C2Q("ң��ֵ:");
static QString cdt_func_ym_value =  _C2Q("ң��ֵ:");
static QString cdt_func_soe_value =  _C2Q("soe:");

static QString cdt_func_fre =  _C2Q("Ƶ��");
static QString cdt_func_reset =  _C2Q("��������");
static QString cdt_func_brod =  _C2Q("�㲥����");

static QString cdt_func_ctrl_sel =  _C2Q("ң��ѡ������");
static QString cdt_func_ctrl_con =  _C2Q("ң�ط�У������");
static QString cdt_func_ctrl_exe =  _C2Q("ң��ִ�й�����");
static QString cdt_func_ctrl_cle =  _C2Q("ң�س���������");

static QString cdt_func_updown_sel =  _C2Q("����ѡ��");
static QString cdt_func_updown_con =  _C2Q("������У");
static QString cdt_func_updown_exe =  _C2Q("����ִ��");
static QString cdt_func_updown_cle =  _C2Q("��������");

static QString cdt_func_idi_set =  _C2Q("��������");
static QString cdt_func_child_idi =  _C2Q("��վ״̬��Ϣ");
static QString cdt_func_clock_con =  _C2Q("����ʱ��У��ֵ");

//static char cdt_temp_buf[1024];
static BYTE cdt_complete_buf[1024];
//static char* cdt_complete_buf;
static bool cdt_frame_finish = false;
static bool cdt_frame_first = true;
//static int cdt_len_full = 0;
static QList <BYTE> cdt_frame_list;   //CDT��֡�������


//֡������
enum CDT_TYPE
{

	CDT_FRAME_A = 0x61,    //��Ҫң�⣬A֡
	CDT_FRAME_B = 0xC2,    //��Ҫң�⣬B֡
	CDT_FRAME_C = 0xB3,    //һ��ң�⣬C֡
	CDT_FRAME_DI = 0xF4,   //ң��״̬
	CDT_FRAME_PS = 0x85,   //�����������ֵ
	CDT_FRAME_SOE = 0x26,  //ʱ��˳���¼

    CDT_FRAME_CTRL_SEL = 0x61,  //ң��ѡ��
    CDT_FRAME_CTRL_EXE = 0xC2,  //ң��ִ��
    CDT_FRAME_CTRL_CAN = 0xB3,  //ң�س���
	CDT_FRAME_CTRL_CON  = 0X9E,      //ң�ط�У

	CDT_FRAME_UPDOWN_SEL = 0xF4,  //����ѡ��
	CDT_FRAME_UPDOWN_EXE = 0x85,  //����ִ��
	CDT_FRAME_UPDOWN_CAN = 0x26,  //��������

	CDT_FRAME_IDI_SET = 0x57,//�趨����

	CDT_FRAME_CLOCK_SET = 0x7A,  //����ʱ��
	CDT_FRAME_CLOCK_CON = 0x0B,  //����ʱ��У��ֵ

	CDT_FRAME_CLOCK_SUM = 0x4C,   //�ٻ���վʱ��

	CDT_FRAME_RESET = 0x3D,      //��������
	CDT_FRAME_BROD  = 0x9E       //�㲥����

};

//������
enum CDT_FUNC
{
	//00H-7FH    ң�� 256
	//F0H-FFH    ң�� 512
    //80H-81H    SOE 4096
	//82H-83H    ����
	//84H-85H    ��վʱ�ӷ��� 1
	//86H-89H    �ܼ�ң�� 8
	//8DH-92H    ˮλ 6
	//93H-9FH    ����
	//A0H-DFH    �����������ֵ 64
	//EEH-EFH    ����ʱ�ӣ����У�1


	CDT_FUNC_FRE = 0x8A,        //Ƶ�� 2
	CDT_FUNC_RESET = 0x8B,      //����������У� 16
	CDT_FUNC_BROD = 0x8C,       //�㲥������У� 16

	CDT_FUNC_CTRL_SEL = 0xE0,   //ң��ѡ�����У� 256
	CDT_FUNC_CTRL_CON = 0xE1,   //ң�ط�У�����У� 256
	CDT_FUNC_CTRL_EXE = 0xE2,   //ң��ִ�У����У� 256
	CDT_FUNC_CTRL_CAN = 0xE3,   //ң�س��������У� 256
	
	CDT_FUNC_UPDOWN_SEL = 0xE4,  //����ѡ�����У� 256
	CDT_FUNC_UPDOWN_CON = 0xE5,  //������У�����У� 256
	CDT_FUNC_UPDOWN_EXE = 0xE6,  //����ִ�У����У� 256
	CDT_FUNC_UPDOWN_CAN = 0xE7,  //�������������У� 256

	CDT_FUNC_IDI_SET = 0xE8,    //����������У� 256
	CDT_FUNC_CHILD_IDI = 0xEC,  //��վ״̬��Ϣ 1
	CDT_FUNC_CLOCK_CON = 0xED   //����ʱ��У��ֵ�����У� 1
};

#endif