#include "CNetModulaData.h"
#include "string.h"
#include "stdio.h"
#include "Protocol_IEC104.h"
#include "Protocol_CDT92.h"
#include "msgview_string.h"
#include "Protocol_process.h"
#include "msgviewwnd.h"
//#include "vld.h"

#include <QtCore/QCoreApplication>
#include <QString>
#include <QLatin1String>
#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QList>
#include <QListIterator>
#include <QMutableListIterator>


CNetModulaData::CNetModulaData()
{
	m_pData = NULL;
	//m_exData = NULL;
	m_pKeyFlag = NULL;
	m_nLength = 0;
	m_nType = -1;
	m_exchange = "\0";

	cdt_frame_count = 0;
	protocol_type = "";   //0703���
	//checkbox_type = 0;
}

CNetModulaData::~CNetModulaData()
{
	if(m_pData)
		delete[] m_pData;

	if(m_pKeyFlag)
		delete[] m_pKeyFlag;


}

//���Ĵ洢
bool CNetModulaData::SetData(char* pData, int nLength)
{
	if(m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
		m_nLength = 0;
	}



	if(m_pKeyFlag)
	{
		delete[] m_pKeyFlag;
		m_pKeyFlag = NULL;
	}

	m_pData = new char[nLength];
	if(!m_pData)
	{
		return false;
	}

	//m_exData = new char[10240];
	//if(!m_exData)
	//{
	//	return false;
	//}

	m_pKeyFlag = new uchar[nLength];
	if(!m_pKeyFlag)
	{
		delete[] m_pData;
		m_pData = NULL;
		return false;
	}
	
	//���ݴ洢��������m_pData������
	memcpy(m_pData, pData, nLength); 
	m_nLength = nLength;




 
	//���ݹ�Լ����ѡ�������Լ
	if (protocol_type == "iec104" )   //104��Լ
	{
//		analyze_iec104_asc(m_pData, m_nLength);
	}

	if (protocol_type == "CDT" )    //CDT��Լ
	{			
		cdt_rebuild(m_pData, m_nLength, m_nType);
	}



	
	//char�洢��QString��
	//for (int i = 0; i < GetexDataLength(); i++)
	//{	
	//	char tmp[5];
	//	sprintf(tmp, "%02X", (unsigned char)*m_exData);


	//	m_exchange += tmp;
	//	//m_exchange += " ";
	//	m_exData++;
	//}

	//m_pData = m_tmp_pData;

	//for (int i = 0; i < m_nLength; i++)
	//{    
	//	bool ok;
	//	QString m_tmp = QString::number((unsigned char)*m_pData,16); 


	//	//�����ַ�Ϊ0x00�����ַ���Ϊ00
	//	if (m_tmp.toInt(&ok,10) == 0)
	//	{
	//		m_tmp = ("00");
	//	}
	//	//�����ַ�Ϊ0x04,0x07�ȣ�����0
	//	if ((m_tmp.toInt(&ok,10) < 10) && (m_tmp.toInt(&ok,10) > 0))
	//	{
	//		m_tmp = m_tmp.insert(0, "0");
	//	}

	//	m_exchange.append(m_tmp);
	//	m_exchange.append(" ");
	//	m_pData++;
	//	m_tmp.clear();
	//}




	memset(m_pKeyFlag, 0, nLength*sizeof(uchar));

	return true;
}
//
//bool CNetModulaData::Exchange(char* m_pData)
//{
//	QString m_exchange = QString(QLatin1String(m_pData));
//}

void CNetModulaData::analyze_iec104_asc( char* m_pData, int nLength)
{
     char temp[2];   //ԭʼchar����
     int pos = 0;   //  pos/2���ֽ���
     int cur_position = 0;  //
     int cnt = 0;  //֡��Ŀ����������
     memset(temp,0,2);
	 int i;
	 int frame_length;  //frame_length + 2	
	 int frame_len = 0;
	 int ms = 0;
	 bool Is_wrong = false;
	 int byte_num = 0;
	 byte_num = nLength;



	 int sendnum = 0;
	 int recvnum = 0;

	// m_exchange = new QString;   //0717




	 while (cur_position < nLength/2) // pos/2���ֽ���
	 {
		 memset(temp,0,2);


	 //����ͷ����
	 if((BYTE)m_pData[cur_position] != FIRST)  
	 {
		 frame_length = (BYTE)m_pData[cur_position+1];
		 for (cur_position = 0; cur_position < frame_length + 2; cur_position++)
		 {
			 sprintf(temp, "%02X", (BYTE)m_pData[cur_position]);
			 m_exchange += temp;
			 m_exchange += remind_space;
		 }
		 m_exchange += remind_first_error; 
		 cur_position = 0;
		 continue;
	 }

	 if ((byte_num -cur_position)%6!=0)
	 {
		 if(((BYTE)m_pData[cur_position + 7] & 0x7f) == 0)
		 {
			 sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
			 m_exchange += temp;
			 m_exchange += remind_first_error;
			 cur_position++;
			 continue;
		 }
		 else if((((BYTE)m_pData[cur_position + 1] - 10) % ((BYTE)m_pData[cur_position + 7] & 0x7f)) != 0 
			 && (((BYTE)m_pData[cur_position + 1] - 13) % ((BYTE)m_pData[cur_position + 7] & 0x7f)) != 0 )
		 {

			 sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
			 m_exchange += temp;
			 m_exchange += remind_first_error;
			 cur_position++;
			 continue;
		 }
	 }


	 //���ĳ��Ȳ���
	 if((BYTE)m_pData[cur_position+1] < S_LEN)  
	 {
		 frame_length = (BYTE)m_pData[cur_position+1];
		 for (cur_position = 0; cur_position < frame_length + 2; cur_position++)
		 {
			 sprintf(temp, "%02X", (BYTE)m_pData[cur_position]);
			 m_exchange += temp;
			 m_exchange += remind_space;
		 }
		 m_exchange += remind_second_error; 
		 cur_position = 0;
		continue;
	 }

	 if (((BYTE)m_pData[(BYTE)m_pData[cur_position+1] + cur_position + 2] != FIRST  //����ͷ
		 &&(BYTE)m_pData[cur_position+1] + cur_position + 2 < byte_num)             //���ĳ���
		 || (BYTE)m_pData[cur_position+1] + cur_position + 2 > byte_num)            //���ĳ���
	 {
		 int tem_position = cur_position + 1;
		 Is_wrong = true;
		 /*Ѱ����һ֡���ĵı�ͷλ��*/

		 while(tem_position < byte_num/2)
		 {

			 if((BYTE)m_pData[tem_position] == FIRST)
			 {
				 if(((BYTE)m_pData[tem_position + 7] & 0x7f) != 0)
				 {
					 if((((BYTE)m_pData[tem_position + 1] - 10) % ((BYTE)m_pData[tem_position + 7] & 0x7f)) == 0 
						 || (((BYTE)m_pData[tem_position + 1] - 13) % ((BYTE)m_pData[tem_position + 7] & 0x7f)) == 0)
					 {
						 break;
					 }
				 }
			 }
			 tem_position++;
		 }
		 frame_len = tem_position - cur_position;
	 }

	 if(Is_wrong)
	 {
		 if(frame_len < 12)
		 {
			 cnt++;
			 cur_position+=frame_len;
			 continue;
		 }
	 }


	 /*****************************�������Ĵ�����********************************/


	 //����ͷ
     sprintf(temp, "%02X", (BYTE)m_pData[cur_position]);
	 m_exchange += temp;
	 m_exchange += remind_first;


	 //���ݳ���
	 sprintf(temp, "%02X", (BYTE)m_pData[cur_position+1]);
	 m_exchange += temp;
	 m_exchange += remind_len;
	 sprintf(temp,"%d",(BYTE)m_pData[cur_position+1]);
	 m_exchange += temp;
	 m_exchange += remind_rightkuohao;
	 m_exchange += remind_space;

	 apdu_104=(P104_APDU*)&m_pData[cur_position];
	 cur_position += 2;   //�Ƶ������򲿷�


	 
	 //������,�ж�֡����
     switch (m_pData[cur_position]&0x03)
	 {
	  case 0x00:   //I֡ 68 16 0e 00 04 00 01 03 03 00 02 00 01 00 00 01 03 00 00 01 02 00 00 01 
	  case 0x02:
		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 0E
		 m_exchange += temp;
		 m_exchange += remind_second_type_i;

		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //00
		 m_exchange += temp;
		 m_exchange += remind_leftkuohao;
		 m_exchange += remind_send_no;
		 sendnum = (apdu_104->apci.type_i.ns.num&0x00ff)>>1;
		 sendnum += apdu_104->apci.type_i.ns.num&0xff00;     //0E 00 �������к�:7
		 sprintf(temp,"%d",sendnum);
		 m_exchange += temp;
		 m_exchange += remind_rightkuohao;
		 m_exchange += remind_space;

		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //04
		 m_exchange += temp;
		 m_exchange += remind_space;
		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //00
		 m_exchange += temp;
		 m_exchange += remind_leftkuohao;
		 m_exchange += remind_recv_no;
		 recvnum = (apdu_104->apci.type_i.nr.num&0x00ff)>>1;
		 recvnum += apdu_104->apci.type_i.nr.num&0xff00;
		 sprintf(temp,"%d",recvnum);   //04 00 �������к� 2
		 m_exchange += temp;
		 m_exchange += remind_rightkuohao;
		 m_exchange += remind_space;
		 break;

	 case 0x01:   //S֡ 68 04 01 00 02 00
		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 04
		 m_exchange += temp;
		 m_exchange += remind_second_type_s;   //S֡

		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 01
		 m_exchange += temp;
		 m_exchange += remind_control_s; 

		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 02
		 m_exchange += temp;
		 m_exchange += remind_space;

		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00
		 m_exchange += temp;
		 m_exchange += remind_leftkuohao;
		 m_exchange += remind_recv_no;
		 recvnum = (apdu_104->apci.type_i.nr.num&0x00ff)>>1;
		 recvnum += apdu_104->apci.type_i.nr.num&0xff00;
		 sprintf(temp,"%d",recvnum);   //02 00 �������к� 1
		 m_exchange += temp;
		 m_exchange += remind_rightkuohao;
		 m_exchange += remind_space;
		 return;
	 case 0x03:  //U֡ 68 04 07 00 00 00
		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 07
		 m_exchange += temp;
		 m_exchange += remind_second_type_u;   //U֡
		

		 switch(apdu_104->apci.type_u.u_type) //�����������ж�
		 {
		 case STARTDT_ACT:  //��վ��������
			 m_exchange += remind_start_act;
			 break;
		 case STARTDT_CON:  //��վȷ�ϼ���
			 m_exchange += remind_start_con;
			 break;
		 case STOPDT_ACT:
			 m_exchange += remind_stop_act;
			 break;
		 case STOPDT_CON:
			 m_exchange += remind_stop_con;
			 break;
		 case TESTFR_ACT:
			 m_exchange += remind_test_act;
			 break;
		 case TESTFR_CON:
			 m_exchange += remind_test_con;
			 break;
		 default:
			 break;
		 }
         
		  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00
		  m_exchange += temp;
		  m_exchange += remind_space;

		  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00
		  m_exchange += temp;
		  m_exchange += remind_space;

		  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00
		  m_exchange += temp;
		  m_exchange += remind_control_u;
		  return;
	 default:
		 break;

	 }

	 /*****************************************ASDU����**********************************************/
	 //68 1E 04 00 02 00 03 05 14 00 01 00 01 00 00 02 06 00 00 02 0A 00 00 01 0B 00 00 02 0C 00 00 01
	 //ASDU->TYPE
	 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]); //���ͱ�ʶ 03
	 m_exchange += temp;
	 m_exchange += remind_leftkuohao;
	 m_exchange += remind_type;
	 sprintf(temp,"%d",apdu_104->asdu.type);
	 m_exchange += temp;
	 m_exchange += remind_space;

	 switch(apdu_104->asdu.type)
	 {
	 case SP_NA_1:
		 m_exchange += remind_no_time_single_signal;
		 break;
	 case DP_NA_3:
		  m_exchange += remind_no_time_double_signal;
		 break;
	 case ME_NA_9:
		  m_exchange += remind_measuring;
		 break;
	 case ME_ND_21:
		 m_exchange += remind_no_time_measuring;
		 break;
	 case SP_TB_30:
		 m_exchange += remind_time_single_signal;
		 break;
	 case DP_TB_31:
		 m_exchange += remind_time_double_signal;
		 break;
	 case ME_TD_34:
		 m_exchange += remind_time_quality_measuring;
		 break;
	 case SC_NA_45:
		 m_exchange += remind_single_command;
		 break;
	 case DC_NA_46:
		 m_exchange += remind_double_command;
		 break;
	 case SE_NA_48:
		 m_exchange += remind_no_time_control_measuring;
		 break;
	 case SC_NA_58:
		 m_exchange += remind_time_single_command;
		 break;
	 case DC_NA_59:
		 m_exchange += remind_time_double_command;
		 break;
	 case SE_TA_61:
		 m_exchange += remind_time_control_measuring;
		 break;
	 case IC_NA_100:
		 m_exchange += remind_total_call;
		 break;
	 case CI_NA_101:
		 m_exchange += remind_energy_call;
		 break;
	 case CS_NA_103:
		 m_exchange += remind_clock_sync;
		 break;
	 case M_EI_NA_70:
		 m_exchange += remind_initialization;
		 break;
	 default:
		 m_exchange += _C2Q("δ֪��������");
		 break;
	 }

	 m_exchange += remind_rightkuohao;
	 m_exchange += remind_space;

	 //ASDU->VSQ
	 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]); //���ͱ�ʶ 05
	 m_exchange += temp;
	 m_exchange += remind_leftkuohao;
	 m_exchange += remind_num;
	 sprintf(temp,"%d ",(apdu_104->asdu.vsq)&0x7F);
	 m_exchange += temp;
	 m_exchange += remind_space;

	 if ((apdu_104->asdu.vsq&0x80) == 0)
	 {
		 m_exchange += remind_nonsequenced;
	 }
	 else
	 {
		  m_exchange += remind_sequenced;
	 }

	 m_exchange += remind_rightkuohao;
	 m_exchange += remind_space;

     //����ԭ��
     sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //14
	 m_exchange += temp;
	 m_exchange += remind_space;

	 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //00
	 m_exchange += temp;
	 m_exchange += remind_leftkuohao;
	 m_exchange += remind_transmit_reason;
	 sprintf(temp,"%d ",apdu_104->asdu.cot);    //cot

	 switch(apdu_104->asdu.cot&0x00ff)
	 {
	 case COT_SPONT:  //ͻ��
		 m_exchange += remind_transmit_reason_burst_3;
		 break;
	 case COT_ACT:   //����
		 m_exchange += remind_transmit_reason_activation_6;
		 break;
	 case COT_ACTCON:    //����ȷ��
		 m_exchange += remind_transmit_reason_activation_confirm_7;
		 break;
	 case COT_DEACT:   //ֹͣ����
		 m_exchange += remind_transmit_reason_activation_stop_8;
		 break;
	 case COT_DEACTCON:  //ֹͣ����ȷ��
		 m_exchange += remind_transmit_reason_activation_stop_confirm_9;
		 break;
	 case COT_ACTTERM:   //������ֹ
		 m_exchange += remind_transmit_reason_activation_teminate_10;
		 break; 
	 case COT_INRO:   //��Ӧվ�ٻ�
		 m_exchange += remind_transmit_reason_respond_station_call_20;
		 break;
	 case COT_UNKOWN_TYPE:
		 break;
	 case COT_UNKOWN_COT:  
		 break;
	 case COT_UNKOWN_ADDR: 
		 break;
	 case COT_UNKOWN_INDEX:
		 break;
	 case ACTIVATION:
		 m_exchange += remind_transmit_reason_activation_6;
		 break;
	 case ACTIVATION_CONFIRM:  
		 m_exchange += remind_transmit_reason_activation_confirm_7;
		 break;
	 default:
		 if ((apdu_104->asdu.cot&0x00ff)>20 && (apdu_104->asdu.cot&0x00ff)<37)
		 {
			 m_exchange += _C2Q("��Ӧ��");
			 sprintf(temp,"%d",(apdu_104->asdu.cot&0x00ff)-20);
			 m_exchange += temp;
			 m_exchange += _C2Q("���ٻ�");
		 }
		 else
		 {
			 m_exchange += _C2Q("δ֪ԭ��");
		 }
		 break;
	 }

	 m_exchange += remind_rightkuohao;
	 m_exchange += remind_space;

	 //�ն˵�ַ
	 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 02
	 m_exchange += temp;
	 m_exchange += remind_space;

	 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00
	 m_exchange += temp;
	 m_exchange += remind_leftkuohao;
     m_exchange += remind_transmit_addr;
	 sprintf(temp,"%d",apdu_104->asdu.addr);
	 m_exchange += temp;
     m_exchange += remind_rightkuohao;
	 m_exchange += remind_space;

	 //���ݲ���
	 int info_addr = 0;
	 int info_num = apdu_104->asdu.vsq&0x7F;  //��Ϣ����
	 int unit_byte = 0;
	 int des_len = 12;

	 /*���ݱ������ͽ��в�ͬ�Ĵ���*/
	 switch(apdu_104->asdu.type)
	 {
		case SP_NA_1://����ң��

			if(Is_wrong)
			{
				unit_byte = 4;
				if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				{
					info_num = (frame_len - des_len)/unit_byte;
				}

			}

			//ASDU DATA����
			//01 00 00 01
			//03 00 00 01
			//02 00 00 01
		if ((apdu_104->asdu.vsq&0x80) == 0)//�ж��Ƿ�Ϊ˳����Ϣ��ַ����Ϣһ��˳���ŷ�
		{

			for(int i = 0; i < info_num; i++)
			{
				info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 01
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00 
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_yx_addr;
				sprintf(temp,"%d",info_addr);
				m_exchange += temp;    //��Ϣ��ַ
				m_exchange += remind_rightkuohao;   
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //��Ϣ����Ϣ 01
				m_exchange += temp;
				m_exchange += remind_leftkuohao;

				m_exchange += remind_siq;
				if((m_pData[cur_position]&0x01) == 0x00)
				{
					m_exchange += remind_open;   //ң�ŷ�λ
				}
				else
				{
					m_exchange += remind_close;   //ң�ź�λ
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x10) == 0x00)
				{
					m_exchange += remind_unblock;     //����
				}
				else
				{
					m_exchange += remind_block;  //����
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x20) == 0x00)
				{
					m_exchange += remind_no_replace;  //δȡ��
				}
				else
				{
					m_exchange += remind_replace;  //ȡ��
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x40) == 0x00)
				{ 
					m_exchange += remind_current_value;  //��ǰֵ
				}
				else
				{
					m_exchange += remind_no_current_value;  //�ǵ�ǰֵ
				}

				m_exchange += remind_space;

				if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
				{
					m_exchange += remind_valid;   //��Ч
				}
				else
				{
					m_exchange += remind_invalid;  //��Ч
				}
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;
			}

		}    //LINE 711

		else    //ֻ����Ϣ��ʼ��ַ����Ϣ˳���ŷ�
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;  

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //01
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //00 
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //00
			m_exchange += temp;
			m_exchange += remind_space;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yx_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;    //��Ϣ��ַ
			m_exchange += remind_rightkuohao;   
			m_exchange += remind_space;

			if(Is_wrong)
			{
				unit_byte = 1;
				des_len += 3;
				if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				{
					info_num = (frame_len - des_len)/unit_byte;
				}
			}

			for(int i = 0; i < info_num; i++)
			{

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //��Ϣ����Ϣ 01
				m_exchange += temp;
				m_exchange += remind_leftkuohao;

				m_exchange += remind_siq;
				if((m_pData[cur_position]&0x01) == 0x00)
				{
					m_exchange += remind_open;   //ң�ŷ�λ
				}
				else
				{
					m_exchange += remind_close;   //ң�ź�λ
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x10) == 0x00)
				{
					m_exchange += remind_unblock;     //����
				}
				else
				{
					m_exchange += remind_block;  //����
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x20) == 0x00)
				{
					m_exchange += remind_no_replace;  //δȡ��
				}
				else
				{
					m_exchange += remind_replace;  //ȡ��
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x40) == 0x00)
				{ 
					m_exchange += remind_current_value;  //��ǰֵ
				}
				else
				{
					m_exchange += remind_no_current_value;  //�ǵ�ǰֵ
				}

				m_exchange += remind_space;

				if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
				{
					m_exchange += remind_valid;   //��Ч
				}
				else
				{
					m_exchange += remind_invalid;  //��Ч
				}
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
			    cur_position++;
			}

		}
		break;

	case DP_NA_3://˫��ң��
		//68 1E 04 00 02 00 03 05 14 00 01 00 01 00 00 02 06 00 00 02 0A 00 00 01 0B 00 00 02 0C 00 00 01
	    if ((apdu_104->asdu.vsq&0x80) == 0)
		{	
				if(Is_wrong)
				{
					unit_byte = 4;
					if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
					{
						info_num = (frame_len - des_len)/unit_byte;
					}
				}

				for(int i = 0; i < info_num; i++)
				{
					info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //01
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //00
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //00 ����3λΪ��ַ
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_yx_addr;
					sprintf(temp,"%d",info_addr);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_diq;

					switch(m_pData[cur_position]&0x03) 
					{
					case 0:
						m_exchange += remind_uncertainty_double;
						break;
					case 1:
						m_exchange += remind_open_double;
						break;
					case 2:
						m_exchange += remind_close_double;
						break;
					case 3:
						m_exchange += remind_uncertainty_double;
						break;
					default:
						break;
					}

					m_exchange += remind_space;

					if ((m_pData[cur_position]&0x10) == 0x00)
					{
						m_exchange += remind_unblock;
					}
					else
					{
						m_exchange += remind_block;
					}

					m_exchange += remind_space;

					if ((m_pData[cur_position]&0x20) == 0x00)
					{
						m_exchange += remind_no_replace;
					}
					else
					{
						m_exchange += remind_replace;
					}

					m_exchange += remind_space;

					if ((m_pData[cur_position]&0x40) == 0x00)
					{
						m_exchange += remind_current_value;
					}
					else
					{
						m_exchange += remind_no_current_value;
					}

					m_exchange += remind_space;

					if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
					{
						m_exchange += remind_valid;
					}
					else
					{
						m_exchange += remind_invalid;
					}

					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;
					cur_position++;
				}
		 }

		 else
		 {
			 info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;

			 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			 m_exchange += temp;
			 m_exchange += remind_space;

			 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			 m_exchange += temp;
			 m_exchange += remind_space;

			 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			 m_exchange += temp;
			 m_exchange += remind_space;
			 m_exchange += remind_leftkuohao;
			 m_exchange += remind_yx_addr;
			 sprintf(temp,"%d",info_addr);
			 m_exchange += temp;    //��Ϣ��ַ
			 m_exchange += remind_rightkuohao;   
			 m_exchange += remind_space;


			 if(Is_wrong)
			 {
				 unit_byte = 1;
				 des_len += 3;
				 if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				 {
					 info_num = (frame_len - des_len)/unit_byte;
				 }
			 }

			 for(int i = 0; i < info_num; i++)
			 {
				 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				 m_exchange += temp;
				 m_exchange += remind_leftkuohao;
				 m_exchange += remind_diq;

				 switch(m_pData[cur_position]&0x03) 
					{
				 case 0:
					 m_exchange += remind_uncertainty_double;
					 break;
				 case 1:
					 m_exchange += remind_open_double;
					 break;
				 case 2:
					 m_exchange += remind_close_double;
					 break;
				 case 3:
					 m_exchange += remind_uncertainty_double;
					 break;
				 default:
					 break;
					}

				 m_exchange += remind_space;

				 if ((m_pData[cur_position]&0x10) == 0x00)
					{
						m_exchange += remind_unblock;
					}
				 else
					{
						m_exchange += remind_block;
					}

				 m_exchange += remind_space;

				 if ((m_pData[cur_position]&0x20) == 0x00)
					{
						m_exchange += remind_no_replace;
					}
				 else
					{
						m_exchange += remind_replace;
					}

				 m_exchange += remind_space;

				 if ((m_pData[cur_position]&0x40) == 0x00)
					{
						m_exchange += remind_current_value;
					}
				 else
					{
						m_exchange += remind_no_current_value;
					}

				 m_exchange += remind_space;

				 if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
					{
						m_exchange += remind_valid;
					}
				 else
					{
						m_exchange += remind_invalid;
					}

				 m_exchange += remind_rightkuohao;
				 m_exchange += remind_space;
				 cur_position++;
			 }
		}	
		break;

	case ME_NA_9://����ֵ����һ��ֵ

		if ((apdu_104->asdu.vsq&0x80) == 0)
	  {
		  if(Is_wrong)
		  {
			  unit_byte = 6;
			  if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			  {
				  info_num = (frame_len - des_len)/unit_byte;
			  }
		  }

		  for(int i = 0; i < info_num; i++)
		  {
			  info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

			  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //��ַ1�ֽ�
			  m_exchange += temp;
			  m_exchange += remind_space;

			  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ2�ֽ�
			  m_exchange += temp;
			  m_exchange += remind_space;

			  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ3�ֽ�
			  m_exchange += temp;
			  m_exchange += remind_leftkuohao;
			  m_exchange += remind_yc_addr;
			  sprintf(temp, "%d", info_addr);
			  m_exchange += temp;
			  m_exchange += remind_rightkuohao;
			  m_exchange += remind_space;                   

			  int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);

			  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			  m_exchange += temp;                                     //ң�������ֽ�1
			  m_exchange += remind_space;

			  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			  m_exchange += temp;                                     //ң�������ֽ�2 + ����
			  m_exchange += remind_leftkuohao;
			  m_exchange += remind_me_na;
			  sprintf(temp,"%d",mea_data);   //ң��ֵ
			  m_exchange += temp;
			  m_exchange += remind_rightkuohao;
			  m_exchange += remind_space;

			  sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);     //ң��Ʒ������
			  m_exchange += temp; 
			  m_exchange += remind_leftkuohao;
			  m_exchange += remind_qds;

			  if((m_pData[cur_position]&0x01) == 0)
			  {
				  m_exchange += remind_no_overflow;
			  }
			  else
			  {
				  m_exchange += remind_overflow;
			  }

			  m_exchange += remind_space;

			  if((m_pData[cur_position]&0x10) == 0)
			  {
				  m_exchange += remind_unblock;
			  }
			  else
			  {
				  m_exchange += remind_block;
			  }

			  m_exchange += remind_space;

			  if((m_pData[cur_position]&0x20) == 0)
			  {
				  m_exchange += remind_no_replace;
			  }
			  else
			  {
				  m_exchange += remind_replace;
			  }

			  m_exchange += remind_space;

			  if((m_pData[cur_position]&0x40) == 0)
			  {
				  m_exchange += remind_current_value;
			  }
			  else
			  {
				  m_exchange += remind_no_current_value;
			  }

			  m_exchange += remind_space;

			  if(((BYTE)m_pData[cur_position++]&0x80) == 0)   //++ �Ƶ���һ�����ݣ����ֽ�����+һ�ֽ�Ʒ��
			  {
				  m_exchange += remind_valid;
			  }
			  else
			  {
				  m_exchange += remind_invalid;
			  }	

			  m_exchange += remind_rightkuohao;
			  m_exchange += remind_space;
		  }

	  }

	else
	{
		info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //��ַ1�ֽ�
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //��ַ2�ֽ�
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ3�ֽ�
		m_exchange += temp;
		m_exchange += remind_leftkuohao;
		m_exchange += info_addr;
		sprintf(temp, "%d", info_addr);
		m_exchange += temp;
		m_exchange += remind_rightkuohao;
		m_exchange += remind_space; 


		if(Is_wrong)
		{
			unit_byte = 3;
			des_len += 3;

			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

	for(int i = 0; i < info_num; i++)
	{
			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //ң�������ֽ�1
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //ң�������ֽ�2 + ����
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Ʒ������
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_qds;

			if((m_pData[cur_position]&0x01) == 0)
			{
				m_exchange += remind_no_overflow;
			}
			else
			{
				m_exchange += remind_overflow;
			}

			if((m_pData[cur_position]&0x10) == 0)
			{
				m_exchange += remind_unblock;
			}
			else
			{
				m_exchange += remind_block;
			}
			if((m_pData[cur_position]&0x20) == 0)
			{
				m_exchange += remind_no_replace;
			}
			else
			{
				m_exchange += remind_replace;
			}
			if((m_pData[cur_position]&0x40) == 0)
			{
				m_exchange +=  remind_current_value;
			}
			else
			{
				m_exchange += remind_no_current_value;
			}
			  if(((BYTE)m_pData[cur_position++]&0x80) == 0)    //ԭʼ
			{
				m_exchange += remind_valid;
			}
			else
			{
				m_exchange += remind_invalid;
			}

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

		}
	}
				
		  break;

case ME_ND_21://����Ʒ��ң��
	if ((apdu_104->asdu.vsq&0x80) == 0)
	{
		if(Is_wrong)
		{
			unit_byte = 5;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ3�ֽڽ���
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yc_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;


			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //ң���ֽ�1
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //ң���ֽ�2
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
		}

	}
	else
	{
		info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ3�ֽ�
		m_exchange += temp;
		m_exchange += remind_leftkuohao;
		m_exchange += remind_yc_addr;
		sprintf(temp,"%d",info_addr);
		m_exchange += temp;
		m_exchange += remind_rightkuohao;
		m_exchange += remind_space;

		if(Is_wrong)
		{
			unit_byte = 2;
			des_len += 3;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

		}
	}
	break;

	case SP_TB_30://��ʱ�굥��ң��,SOE
		if ((apdu_104->asdu.vsq&0x80) == 0)
		{
			if(Is_wrong)
			{
				unit_byte = 11;
				if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				{
					info_num = (frame_len - des_len)/unit_byte;
				}
			}

			for(int i = 0; i < info_num; i++)
			{
				info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;   //��Ϣ���ַ3�ֽ�

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //��ַ�ֽ�1
				m_exchange += temp;
			    m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ�ֽ�2
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ�ֽ�3
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_yx_addr;
				sprintf(temp,"%d",info_addr);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��Ϣ�ֽ�
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_siq;

				if((m_pData[cur_position]&0x01) == 0x00)
				{
					m_exchange += remind_open;
				}
				else
				{
					m_exchange += remind_close;
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x10) == 0x00)
				{
					m_exchange += remind_unblock;
				}
				else
				{
					m_exchange += remind_block;
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x20) == 0x00)
				{
					m_exchange += remind_no_replace;
				}
				else
				{
					m_exchange += remind_replace;
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x40) == 0x00)
				{
					m_exchange += remind_current_value;
				}
				else
				{
					m_exchange += remind_no_current_value;
				}

				m_exchange += remind_space;

				if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
				{
					m_exchange += remind_valid;
				}
				else
				{
					m_exchange += remind_invalid;
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//SOEʱ�䲿�ִ���
				ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ�
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_ms;
				sprintf(temp,"%d",ms);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_minute;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Сʱ
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_hour;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_day;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_month;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_year;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

			}

		}
		else
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //��ַ�ֽ�1
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //��ַ�ֽ�2
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ�ֽ�3
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yx_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			if(Is_wrong)
			{
				unit_byte = 8;
				des_len += 3;
				if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				{
					info_num = (frame_len - des_len)/unit_byte;
				}
			}

			for(int i = 0; i < info_num; i++)
			{
				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��Ϣ�ֽ�
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_siq;

				if((m_pData[cur_position]&0x01) == 0x00)
				{
					m_exchange += remind_open;
				}
				else
				{
					m_exchange += remind_close;
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x10) == 0x00)
				{
					m_exchange += remind_unblock;
				}
				else
				{
					m_exchange += remind_block;
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x20) == 0x00)
				{
					m_exchange += remind_no_replace;
				}
				else
				{
					m_exchange += remind_replace;
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x40) == 0x00)
				{
					m_exchange += remind_current_value;
				}
				else
				{
					m_exchange += remind_no_current_value;
				}

				m_exchange += remind_space;

				if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
				{
					m_exchange += remind_valid;
				}
				else
				{
					m_exchange += remind_invalid;
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//SOEʱ�䲿�ִ���
				ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ�
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_ms;
				sprintf(temp,"%d",ms);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_minute;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Сʱ
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_hour;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_day;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_month;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_year;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
			}
		}
		break;


			case DP_TB_31://��ʱ��˫��ң��,SOE
				if ((apdu_104->asdu.vsq&0x80) == 0)
				{
					if(Is_wrong)
					{
						unit_byte = 11;
						if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
						{
							info_num = (frame_len - des_len)/unit_byte;
						}
					}

					for(int i = 0; i < info_num; i++)
					{
						info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;   //��Ϣ���ַ3�ֽ�

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //��ַ�ֽ�1
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ�ֽ�2
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ�ֽ�3
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_yx_addr;
						sprintf(temp,"%d",info_addr);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��Ϣ�ֽ�
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_siq;

						switch(m_pData[cur_position]&0x03) 
						{
						case 0:
							m_exchange += remind_uncertainty_double;
							break;
						case 1:
							m_exchange += remind_open_double;
							break;
						case 2:
							m_exchange += remind_close_double;
							break;
						case 3:
							m_exchange += remind_uncertainty_double;
							break;
						default:
							break;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x10) == 0x00)
						{
							m_exchange += remind_unblock;
						}
						else
						{
							m_exchange += remind_block;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x20) == 0x00)
						{
							m_exchange += remind_no_replace;
						}
						else
						{
							m_exchange += remind_replace;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x40) == 0x00)
						{
							m_exchange += remind_current_value;
						}
						else
						{
							m_exchange += remind_no_current_value;
						}

						m_exchange += remind_space;

						if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
						{
							m_exchange += remind_valid;
						}
						else
						{
							m_exchange += remind_invalid;
						}

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						//SOEʱ�䲿�ִ���
						ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ�
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_ms;
						sprintf(temp,"%d",ms);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_minute;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Сʱ
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_hour;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_day;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_month;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_year;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
					}

				}
				else
				{
					info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //��ַ�ֽ�1
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //��ַ�ֽ�2
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ�ֽ�3
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_yx_addr;
					sprintf(temp,"%d",info_addr);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					if(Is_wrong)
					{
						unit_byte = 8;
						des_len += 3;
						if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
						{
							info_num = (frame_len - des_len)/unit_byte;
						}
					}

					for(int i = 0; i < (apdu_104->asdu.vsq&0x7F); i++)
					{
						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��Ϣ�ֽ�
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_siq;

						switch(m_pData[cur_position]&0x03) 
						{
						case 0:
							m_exchange += remind_uncertainty_double;
							break;
						case 1:
							m_exchange += remind_open_double;
							break;
						case 2:
							m_exchange += remind_close_double;
							break;
						case 3:
							m_exchange += remind_uncertainty_double;
							break;
						default:
							break;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x10) == 0x00)
						{
							m_exchange += remind_unblock;
						}
						else
						{
							m_exchange += remind_unblock;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x20) == 0x00)
						{
							m_exchange += remind_no_replace;
						}
						else
						{
							m_exchange += remind_replace;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x40) == 0x00)
						{
							m_exchange += remind_current_value;
						}
						else
						{
							m_exchange += remind_no_current_value;
						}

						m_exchange += remind_space;

						if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
						{
							m_exchange += remind_valid;
						}
						else
						{
							m_exchange += remind_invalid;
						}


						m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//SOEʱ�䲿�ִ���
				ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ�
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_ms;
				sprintf(temp,"%d",ms);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_minute;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Сʱ
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_hour;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_day;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_month;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_year;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
					}
				}
				break;

case ME_TD_34://��ʱ��Ĳ���ֵ����һ��ֵ
	if ((apdu_104->asdu.vsq&0x80) == 0)
	{
		if(Is_wrong)
		{
			unit_byte = 13;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;
			
			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ�ֽ�1
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ�ֽ�2
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ�ֽ�3+����
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yc_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
			
			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);   //ң������2�ֽ�

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;


			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Ʒ������
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_qds;


			if((m_pData[cur_position]&0x01) == 0)
			{
				m_exchange += remind_no_overflow;
			}
			else
			{
				m_exchange += remind_overflow;
			}

			m_exchange += remind_space;

			if((m_pData[cur_position]&0x10) == 0)
			{
				m_exchange += remind_unblock;
			}
			else
			{
				m_exchange += remind_block;
			}

			m_exchange += remind_space;

			if((m_pData[cur_position]&0x20) == 0)
			{
				m_exchange += remind_no_replace;
			}
			else
			{
				m_exchange += remind_replace;
			}

			m_exchange += remind_space;

			if((m_pData[cur_position]&0x40) == 0)
			{
				m_exchange += remind_current_value;
			}
			else
			{
				m_exchange += remind_no_current_value;
			}

			m_exchange += remind_space;

			if(((BYTE)m_pData[cur_position++]&0x80) == 0)
			{
				m_exchange += remind_valid;
			}
			else
			{
				m_exchange += remind_invalid;
			}	

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			//SOEʱ�䲿�ִ���
			ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ�
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_ms;
			sprintf(temp,"%d",ms);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_minute;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Сʱ
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_hour;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_day;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_month;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_year;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
		}

	}
	else
	{
		info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ�ֽ�1
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ�ֽ�2
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ�ֽ�3+����
		m_exchange += temp;
		m_exchange += remind_leftkuohao;
		m_exchange += remind_yc_addr;
		sprintf(temp,"%d",info_addr);
		m_exchange += temp;
		m_exchange += remind_rightkuohao;
		m_exchange += remind_space;

		if(Is_wrong)
		{
			unit_byte = 10;
			des_len += 3;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);   //ң������2�ֽ�

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			if((m_pData[cur_position]&0x01) == 0)
			{
				m_exchange += remind_no_overflow;
			}
			else
			{
				m_exchange += remind_overflow;
			}

			m_exchange += remind_space;

			if((m_pData[cur_position]&0x10) == 0)
			{
				m_exchange += remind_unblock;
			}
			else
			{
				m_exchange += remind_block;
			}

			m_exchange += remind_space;

			if((m_pData[cur_position]&0x20) == 0)
			{
				m_exchange += remind_no_replace;
			}
			else
			{
				m_exchange += remind_replace;
			}

			m_exchange += remind_space;

			if((m_pData[cur_position]&0x40) == 0)
			{
				m_exchange += remind_current_value;
			}
			else
			{
				m_exchange += remind_no_current_value;
			}

			m_exchange += remind_space;

			if(((BYTE)m_pData[cur_position++]&0x80) == 0)
			{
				m_exchange += remind_valid;
			}
			else
			{
				m_exchange += remind_invalid;
			}	

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			//SOEʱ�䲿�ִ���
			ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ�
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_ms;
			sprintf(temp,"%d",ms);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_minute;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Сʱ
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_hour;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_day;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_month;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_year;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
		}
	}
	break;


	//ң������
			case SC_NA_45://����ң��
				if ((apdu_104->asdu.vsq&0x80) == 0)
				{
					if(Is_wrong)
					{
						unit_byte = 4;
						if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
						{
							info_num = (frame_len - des_len)/unit_byte;
						}
					}

					for(int i = 0; i < info_num; i++)
					{
						info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ3�ֽ�
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_yk_addr;
						sprintf(temp,"%d",info_addr);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //ң������
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_sco;

						if((m_pData[cur_position]&0x01) == 0)
						{
							m_exchange += remind_open_ctl;
						}
						else
						{
							m_exchange += remind_close_ctl;
						}

						m_exchange += remind_space;

						int bit_tem = (m_pData[cur_position]&0x7C);
						bit_tem>>=2;

						switch(bit_tem)
						{
						case 0:
							m_exchange += remind_no_other;
							break;
						case 1:
							m_exchange += remind_short_pulse;
							break;
						case 2:
							m_exchange += remind_long_pulse;
							break;
						case 3:
							m_exchange += remind_sustained_output;
							break;
						default:
							m_exchange += remind_reserve;
							break;
						}

						m_exchange += remind_space;

						if (((BYTE)m_pData[cur_position]&0x80) == 0)
						{
							m_exchange += remind_execute;
						}
						else
						{
							m_exchange += remind_select;
						}

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;
					}

				}
				else
				{
					info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ3�ֽ�
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_yk_addr;
					sprintf(temp,"%d",info_addr);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;


					if(Is_wrong)
					{
						unit_byte = 1;
						des_len += 3;
						if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
						{
							info_num = (frame_len - des_len)/unit_byte;
						}
					}

					for(int i = 0; i < info_num; i++)
					{
						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //ң������
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_sco;

						int bit_tem = (m_pData[cur_position]&0x7C);
						bit_tem>>=2;

						switch(bit_tem)
						{
						case 0:
							m_exchange += remind_no_other;
							break;
						case 1:
							m_exchange += remind_short_pulse;
							break;
						case 2:
							m_exchange += remind_long_pulse;
							break;
						case 3:
							m_exchange += remind_sustained_output;
							break;
						default:
							m_exchange += remind_reserve;
							break;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x80) == 0)
						{
							m_exchange += remind_execute;
						}
						else
						{
							m_exchange += remind_select;
						}

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;
					}
				}
				break;


	case DC_NA_46://˫��ң��
		if ((apdu_104->asdu.vsq&0x80) == 0)
		{
			if(Is_wrong)
			{
				unit_byte = 4;
				if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				{
					info_num = (frame_len - des_len)/unit_byte;
				}
			}

			for(int i = 0; i < info_num; i++)
			{
				info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;  //��ַ3�ֽ�

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
			    m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_yk_addr;
				sprintf(temp,"%d",info_addr);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_dco;

				switch(m_pData[cur_position]&0x03)
				{
				case 0:
					m_exchange += remind_no_allowed;
					break;
				case 1:
					m_exchange += remind_open_ctl;
					break;
				case 2:
					m_exchange += remind_close_ctl;
					break;
				case 3:
					m_exchange += remind_no_allowed;
					break;
				default:
					m_exchange += remind_reserve;
					break;

				}

				m_exchange += remind_space;

				int bit_tem = m_pData[cur_position]&0x7C;
				bit_tem>>=2;

				switch(bit_tem)
				{
				case 0:
					m_exchange += remind_no_other;
					break;
				case 1:
					m_exchange += remind_short_pulse;
					break;
				case 2:
					m_exchange += remind_long_pulse;
					break;
				case 3:
					m_exchange += remind_sustained_output;
					break;
				default:
					m_exchange += remind_reserve;
					break;
				}

				m_exchange += remind_space;


				if ((m_pData[cur_position]&0x80) == 0)
				{
					m_exchange += remind_execute;
				}
				else
				{
					m_exchange += remind_select;
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;
			}

		}
		else
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;  //��ַ3�ֽ�

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yk_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			if(Is_wrong)
			{
				unit_byte = 1;
				des_len += 3;
				if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				{
					info_num = (frame_len - des_len)/unit_byte;
				}
			}

			for(int i = 0; i < info_num; i++)
			{
				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_dco;

				switch(m_pData[cur_position]&0x03)
				{
				case 0:
					m_exchange += remind_no_allowed;
					break;
				case 1:
					m_exchange += remind_open_ctl;
					break;
				case 2:
					m_exchange += remind_close_ctl;
					break;
				case 3:
					m_exchange += remind_no_allowed;
					break;
				default:
					m_exchange += remind_reserve;
					break;

				}

				m_exchange += remind_space;

				int bit_tem = m_pData[cur_position]&0x7C;
				bit_tem>>=2;

				switch(bit_tem)
				{
				case 0:
					m_exchange += remind_no_other;
					break;
				case 1:
					m_exchange += remind_short_pulse;
					break;
				case 2:
					m_exchange += remind_long_pulse;
					break;
				case 3:
					m_exchange += remind_sustained_output;
					break;
				default:
					m_exchange += remind_reserve;
					break;
				}

				m_exchange += remind_space;


				if ((m_pData[cur_position]&0x80) == 0)
				{
					m_exchange += remind_execute;
				}
				else
				{
					m_exchange += remind_select;
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;
			}
		}
		break;

case SE_NA_48://��������һ��ֵ

	//68 10 00 00 02 00 30 01 07 00 01 00 00 00 00 34 12 80 
	if ((apdu_104->asdu.vsq&0x80) == 0)
	{
		if(Is_wrong)
		{
			unit_byte = 6;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //��ַ��Ϣ3�ֽ�
		    m_exchange += temp; 
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yc_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //�趨ң��2�ֽ�
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_qos;

			if ((m_pData[cur_position]&0x7F) == 0)
			{
				m_exchange += _C2Q("δ��");
			}
			
			//m_exchange += remind_space;

			else if ((m_pData[cur_position]&0x7F) <= 15)
			{
				m_exchange += _C2Q("���������ݷ�Χ");
			}

			//m_exchange += remind_space;

			else if ((m_pData[cur_position]&0x7F) > 15 &&(m_pData[cur_position]&0x7F) < 32 )
			{
				m_exchange += _C2Q("������ר�÷�Χ");
			}

			//m_exchange += remind_space;

			else
			{
				m_exchange += _C2Q("δ֪");
			}

			m_exchange += remind_space;

			if(((BYTE)m_pData[cur_position]&0x80) == 0)
			{
				m_exchange += remind_execute;
			}
			else
			{
				m_exchange += remind_select;
			}

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
			cur_position++;
		}

	}
	else
	{
		info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //��ַ��Ϣ3�ֽ�
		m_exchange += temp; 
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
		m_exchange += temp;
		m_exchange += remind_leftkuohao;
		m_exchange += remind_yc_addr;
		sprintf(temp,"%d",info_addr);
		m_exchange += temp;
		m_exchange += remind_rightkuohao;
		m_exchange += remind_space;

		if(Is_wrong)
		{
			unit_byte = 3;
			des_len += 3;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //�趨ң��2�ֽ�
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_qos;

			if ((m_pData[cur_position]&0x7F) == 0)
			{
				m_exchange += _C2Q("δ��");
			}

			//m_exchange += remind_space;

			else if ((m_pData[cur_position]&0x7F) <= 15)
			{
				m_exchange += _C2Q("���������ݷ�Χ");
			}

			//m_exchange += remind_space;

			else if ((m_pData[cur_position]&0x7F) > 15 &&(m_pData[cur_position]&0x7F) < 32 )
			{
				m_exchange += _C2Q("������ר�÷�Χ");
			}

			//m_exchange += remind_space;

			else
			{
				m_exchange += _C2Q("δ֪");
			}

			m_exchange += remind_space;

			if(((BYTE)m_pData[cur_position]&0x80) == 0)
			{
				m_exchange += remind_execute;
			}
			else
			{
				m_exchange += remind_select;
			}

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
			cur_position++;
		}
	}			
	break;


		case SC_NA_58://��ʱ��ĵ�����
			if ((apdu_104->asdu.vsq&0x80) == 0)
			{
				if(Is_wrong)
				{
					unit_byte = 11;
					if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
					{
						info_num = (frame_len - des_len)/unit_byte;
					}
				}

				for(int i = 0; i < info_num; i++)
				{
					info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ3�ֽ�
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ3�ֽ�
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ3�ֽ�
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_yk_addr;
					sprintf(temp,"%d",info_addr);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //ң������
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_sco;

					if((m_pData[cur_position]&0x01) == 0x00)
					{
						m_exchange == remind_open_ctl;
					}
					else
					{
						m_exchange += remind_close_ctl;
					}

					m_exchange += remind_space;

					int bit_tem = m_pData[cur_position]&0x7C;
					bit_tem>>=2;

					switch(bit_tem)
					{
					case 0:
						m_exchange += remind_no_other;
						break;
					case 1:
						m_exchange += remind_short_pulse;
						break;
					case 2:
						m_exchange += remind_long_pulse;
						break;
					case 3:
						m_exchange += remind_sustained_output;
						break;
					default:
						m_exchange += remind_reserve;
						break;
					}

					m_exchange += remind_space;

					if (((BYTE)m_pData[cur_position]&0x80) == 0)
					{
						m_exchange += remind_execute;
					}
					else
					{
						m_exchange += remind_select;
					}
                    
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;
					cur_position++;

					ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ�
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_ms;
					sprintf(temp,"%d",ms);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_minute;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Сʱ
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_hour;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_day;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_month;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_year;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;
				}

			}
			else
			{
				info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ3�ֽ�
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ3�ֽ�
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //��ַ3�ֽ�
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_yk_addr;
				sprintf(temp,"%d",info_addr);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				if(Is_wrong)
				{
					unit_byte = 8;
					des_len += 3;
					if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
					{
						info_num = (frame_len - des_len)/unit_byte;
					}
				}

				for(int i = 0; i < info_num; i++)
				{
					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //ң������
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_sco;

					if((m_pData[cur_position]&0x01) == 0x00)
					{
						m_exchange == remind_open_ctl;
					}
					else
					{
						m_exchange += remind_close_ctl;
					}

					m_exchange += remind_space;

					int bit_tem = m_pData[cur_position]&0x7C;
					bit_tem>>=2;

					switch(bit_tem)
					{
					case 0:
						m_exchange += remind_no_other;
						break;
					case 1:
						m_exchange += remind_short_pulse;
						break;
					case 2:
						m_exchange += remind_long_pulse;
						break;
					case 3:
						m_exchange += remind_sustained_output;
						break;
					default:
						m_exchange += remind_reserve;
						break;
					}

					m_exchange += remind_space;

					if (((BYTE)m_pData[cur_position]&0x80) == 0)
					{
						m_exchange += remind_execute;
					}
					else
					{
						m_exchange += remind_select;
					}

					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;
					cur_position++;

					ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ�
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_ms;
					sprintf(temp,"%d",ms);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_minute;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Сʱ
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_hour;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_day;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_month;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_year;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;
				}
			}
			break;


			case DC_NA_59://��ʱ���˫����
				if ((apdu_104->asdu.vsq&0x80) == 0)
				{
					if(Is_wrong)
					{
						unit_byte = 11;
						if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
						{
							info_num = (frame_len - des_len)/unit_byte;
						}
					}

					for(int i = 0; i < info_num; i++)
					{
						info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //��ַ3�ֽ�
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_yk_addr;
						sprintf(temp,"%d",info_addr);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //ң������
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_dco;

						switch(m_pData[cur_position]&0x03)
						{
						case 0:
							m_exchange += remind_no_allowed;
							break;
						case 1:
							m_exchange += remind_open_ctl;
							break;
						case 2:
							m_exchange += remind_close_ctl;
							break;
						case 3:
							m_exchange += remind_no_allowed;
							break;
						default:
							m_exchange += remind_reserve;
							break;
						}

						m_exchange += remind_space;

						int bit_tem = m_pData[cur_position]&0x7C;
						bit_tem>>=2;

						switch(bit_tem)
						{
						case 0:
							m_exchange += remind_no_other;
							break;
						case 1:
							m_exchange += remind_short_pulse;
							break;
						case 2:
							m_exchange += remind_long_pulse;
							break;
						case 3:
							m_exchange += remind_sustained_output;
							break;
						default:
							m_exchange += remind_reserve;
							break;

						}

						m_exchange += remind_space;

						if (((BYTE)m_pData[cur_position]&0x80) == 0)
						{
							m_exchange += remind_execute;
						}
						else
						{
							m_exchange += remind_select;
						}

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ�
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_ms;
						sprintf(temp,"%d",ms);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_minute;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Сʱ
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_hour;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_day;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_month;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_year;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
					}

				}
				else
				{	
				   info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //��ַ3�ֽ�
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_yk_addr;
					sprintf(temp,"%d",info_addr);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					if(Is_wrong)
					{
						unit_byte = 8;
						des_len += 3;
						if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
						{
							info_num = (frame_len - des_len)/unit_byte;
						}
					}

					for(int i = 0; i < info_num; i++)
					{
						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //ң������
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_dco;

						switch(m_pData[cur_position]&0x03)
						{
						case 0:
							m_exchange += remind_no_allowed;
							break;
						case 1:
							m_exchange += remind_open_ctl;
							break;
						case 2:
							m_exchange += remind_close_ctl;
							break;
						case 3:
							m_exchange += remind_no_allowed;
							break;
						default:
							m_exchange += remind_reserve;
							break;
						}

						m_exchange += remind_space;

						int bit_tem = m_pData[cur_position]&0x7C;
						bit_tem>>=2;

						switch(bit_tem)
						{
						case 0:
							m_exchange += remind_no_other;
							break;
						case 1:
							m_exchange += remind_short_pulse;
							break;
						case 2:
							m_exchange += remind_long_pulse;
							break;
						case 3:
							m_exchange += remind_sustained_output;
							break;
						default:
							m_exchange += remind_reserve;
							break;
						}

						m_exchange += remind_space;

						if (((BYTE)m_pData[cur_position]&0x80) == 0)
						{
							m_exchange += remind_execute;
						}
						else
						{
							m_exchange += remind_select;
						}


						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;


						ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ�
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_ms;
						sprintf(temp,"%d",ms);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_minute;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Сʱ
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_hour;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_day;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_month;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_year;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
					}
				}
				break;

case SE_TA_61://��ʱ�����������һ��ֵ
	if ((apdu_104->asdu.vsq&0x80) == 0)
	{
		if(Is_wrong)
		{
			unit_byte = 13;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //��ַ3�ֽ�
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yc_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);   

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //ң��2�ֽ�
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_qos;

			if ((m_pData[cur_position]&0x7F) == 0)
			{
				m_exchange += _C2Q("δ��");
			}

			else if ((m_pData[cur_position]&0x7F) <= 15)
			{
				m_exchange += _C2Q("���������ݷ�Χ");
			}

			else if ((m_pData[cur_position]&0x7F) > 15 &&(m_pData[cur_position]&0x7F) < 32 )
			{
				m_exchange += _C2Q("������ר�÷�Χ");
			}

			else
			{
				m_exchange += _C2Q("δ֪");
			}

			m_exchange += remind_space;

			if(((BYTE)m_pData[cur_position]&0x80) == 0)
			{
				m_exchange += remind_execute;
			}

			else
			{
				m_exchange += remind_select;
			}

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
			cur_position++;


			ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ�
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_ms;
			sprintf(temp,"%d",ms);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_minute;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Сʱ
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_hour;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_day;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_month;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_year;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
		}

	}
	else
	{
		info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //��ַ3�ֽ�
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
		m_exchange += temp;
		m_exchange += remind_leftkuohao;
		m_exchange += remind_yc_addr;
		sprintf(temp,"%d",info_addr);
		m_exchange += temp;
		m_exchange += remind_rightkuohao;
		m_exchange += remind_space;

		if(Is_wrong)
		{
			unit_byte = 10;
			des_len += 3;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);   

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //ң��2�ֽ�
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_qos;

			if ((m_pData[cur_position]&0x7F) == 0)
			{
				m_exchange += _C2Q("δ��");
			}

			else if ((m_pData[cur_position]&0x7F) <= 15)
			{
				m_exchange += _C2Q("���������ݷ�Χ");
			}

			else if ((m_pData[cur_position]&0x7F) > 15 &&(m_pData[cur_position]&0x7F) < 32 )
			{
				m_exchange += _C2Q("������ר�÷�Χ");
			}

			else
			{
				m_exchange += _C2Q("δ֪");
			}

			m_exchange += remind_space;

			if(((BYTE)m_pData[cur_position]&0x80) == 0)
			{
				m_exchange += remind_execute;
			}
			else
			{
				m_exchange += remind_select;
			}

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
			cur_position++;

			ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ�
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_ms;
			sprintf(temp,"%d",ms);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //����
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_minute;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //Сʱ
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_hour;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_day;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_month;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //��
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_year;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
		}
	}			
	break;

			case IC_NA_100://���ٻ�����

				if (Is_wrong)
				{
					continue;
				}

				//info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3);
	
				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��Ϣ���ַ3�ֽ�
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_info_addr;
				sprintf(temp,"%d",apdu_104->asdu.cs.index_no);   //���ٻ���ַ
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //�ٻ��޶���QOI
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_call_limit;
				sprintf(temp,"%d",apdu_104->asdu.cs.qoi);
				m_exchange += temp;
				m_exchange += remind_space;

				if(apdu_104->asdu.cs.qoi == 20)
				{
					m_exchange += remind_respond_station_call_20;
				}
				else
				{
					m_exchange += _C2Q("��Ӧ��");
					sprintf(temp,"%d",apdu_104->asdu.cs.qoi-20);
					m_exchange += temp;
					m_exchange += _C2Q("���ٻ�");
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				break;

			case CI_NA_101://���������ٻ�
				if (Is_wrong)
				{
					continue;
				}
				info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 25600;  

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��Ϣ���ַ3�ֽ�
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_ym_addr;
				sprintf(temp,"%d",info_addr);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //QCC
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_energy_limit;
				m_exchange += remind_space;

				switch(m_pData[cur_position]&0xC0) 
				{
				case 0x40:
					m_exchange += remind_energy_fre_no_reset;
					break;
				case 0x80:
					m_exchange += remind_energy_fre_reset;
					break;
				case 0xC0:
					m_exchange += remind_energy_counter_reset;
					break;
				default:
					break;
				}

				m_exchange += remind_space;

				switch(m_pData[cur_position]&0x3F) 
				{
				case 1:
					m_exchange += remind_energy_call_1;
					break;
				case 2:
					m_exchange += remind_energy_call_2;
					break;
				case 3:
					m_exchange += remind_energy_call_3;
					break;
				case 4:
					m_exchange += remind_energy_call_4;
					break;
				case 5:
					m_exchange += remind_energy_call_5;
					break;
				default:
					break;
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;
				break;

			case CS_NA_103://ʱ��ͬ������

				if (Is_wrong)
				{
					continue;
				}

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��Ϣ���ַ3�ֽ�
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_info_addr;
				sprintf(temp,"%d",apdu_104->asdu.sts.index_no);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //������ֽ�
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������ֽ� + ����
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_ms;
				sprintf(temp,"%d",ms);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //����
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_minute;
				sprintf(temp,"%d",apdu_104->asdu.sts.cp56time.minute);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //Сʱ
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_hour;
				sprintf(temp,"%d",apdu_104->asdu.sts.cp56time.hour);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_day;
				sprintf(temp,"%d",apdu_104->asdu.sts.cp56time.day);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //��
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_month;
				sprintf(temp,"%d",apdu_104->asdu.sts.cp56time.month);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;


				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_year;
				sprintf(temp,"%d",apdu_104->asdu.sts.cp56time.year);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				break;



			case M_EI_NA_70://��ʼ��

				if (Is_wrong)
				{
					continue;
				}

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ3�ֽ�
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_info_addr;
				sprintf(temp,"%d",apdu_104->asdu.sts.index_no);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_coi;

				switch(m_pData[cur_position]&0x7F)
				{
				case 0:
					m_exchange += remind_coi_0;
					break;
				case 1:
					m_exchange += remind_coi_1;
					break;
				case 2:
					m_exchange += remind_coi_2;
					break;
				default:
					m_exchange += remind_coi_other;
					break;
				}

				m_exchange += remind_space;

				if (((BYTE)m_pData[cur_position++]&0x80) == 0)
				{
					m_exchange += remind_coi_unchanged;
				}
				else
				{
					m_exchange += remind_coi_changed;
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				break;


	  //case:
		 // break;

	 default:   //ʣ�����ʹ�ӡ���ַ���
		 int left_num = apdu_104->apci.apdu_len - 10 ;  

		 for (i = 0; i < left_num; i++)
		 {
			 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //��ַ3�ֽ�
			 m_exchange += temp;
			 m_exchange += remind_space;
		 }
		 break;




	 }  //while
}  //function





     
     


	
	// if((BYTE)m_pData[cur_position]  == 0x43 || (BYTE)m_pData[cur_position] == 0x07)  //���ĳ��Ȳ���
	// {

	//	 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //������
	//	 m_exchange += temp;
	//	 m_exchange += remind_control_u;
	// }

 //      sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]); 
	//   m_exchange += temp;
	//   m_exchange += remind_space;

	//   sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]); 
	//   m_exchange += temp;
 //      m_exchange += remind_space;

	//   sprintf(temp,"%02X",(BYTE)m_pData[cur_position]); 
	//   m_exchange += temp;
 

}

//CDT���Ķ�֡����
//void CNetModulaData::cdt_rebuild( char* pData, int nLength, int nType)
//{
//	int pos = 0;   //  pos/2���ֽ���
//	int cur_position = 0;  //
//	int cnt = 0;  //֡��Ŀ����������
//	//memset(temp,0,2);
//	int i;
//	int frame_length;  //frame_length + 2	
//
//	int ms = 0;
//	bool Is_wrong = false;
//	int byte_num = 0;
//	byte_num = nLength;
//
//	int frame_len = nLength;
//
//	//��һ֡��������ȡEB90���ֽ�������
//	if (cdt_frame_first == true)
//	{
//		for (i = 0; i < nLength; i++)
//		{
//			//�жϵ�һ֡�����Ƿ���CDT����ͷ���֣��еĻ����Ƶ�������
//			if (((BYTE)pData[i] == 0xEB) && ((BYTE)pData[i+1] == 0x90)
//				&& ((BYTE)pData[i+2] == 0xEB) && ((BYTE)pData[i+3] == 0x90)
//				&& ((BYTE)pData[i+4] == 0xEB) && ((BYTE)pData[i+5] == 0x90))
//			{
//				cdt_len_full = (BYTE)pData[i+8] * 6 + 12;   //����֡����
//				
//				//��ȥǰ�����ı��ģ�ʣ�µĸպ���һ֡������CDT֡
//				if (nLength - i == cdt_len_full)   
//				{
//					memcpy(cdt_complete_buf,&pData[i],cdt_len_full);
//					cdt_frame_finish = true;
//					if (cdt_frame_finish == true)
//					{
//						analyze_cdt_asc((char*)cdt_complete_buf, cdt_len_full, nType);
//					}
//					memset(cdt_complete_buf, 0, cdt_len_full);
//				}
//
//				//������ǰ����౨�ģ�������һ֡������CDT֡���������������CDT֡
//				if(nLength - i > cdt_len_full)
//				{
//					memcpy(cdt_complete_buf,&pData[i],cdt_len_full);
//					cdt_frame_finish = true;
//					if (cdt_frame_finish == true)
//					{
//						analyze_cdt_asc((char*)cdt_complete_buf, cdt_len_full, nType);
//					}
//					memset(cdt_complete_buf, 0, cdt_len_full);
//
//				}
//				
//			}
//
//		}
//		cdt_frame_first = false;   //��һ֡�����꣬����ͷ֡�����뻺��������
//		return;
//
//	}
//
//	if (cdt_frame_first == false)
//	{
//
//		for (i = 0; i < nLength; i++)
//		{
//			//�жϵ�һ֡�����Ƿ���CDT����ͷ���֣��еĻ����Ƶ�������
//			if (((BYTE)pData[i] == 0xEB) && ((BYTE)pData[i+1] == 0x90)
//				&& ((BYTE)pData[i+2] == 0xEB) && ((BYTE)pData[i+3] == 0x90)
//				&& ((BYTE)pData[i+4] == 0xEB) && ((BYTE)pData[i+5] == 0x90))
//			{
//				cdt_len_full = (BYTE)pData[i+8] * 6 + 12;   //֡����
//				if (nLength - i == cdt_len_full)   
//				{
//					memcpy(cdt_complete_buf,&pData[i],cdt_len_full);
//					cdt_frame_finish = true;
//					if (cdt_frame_finish == true)
//					{
//						analyze_cdt_asc((char*)cdt_complete_buf, cdt_len_full, nType);
//					}
//				}
//				memset(cdt_complete_buf, 0, cdt_len_full);
//			}
//
//		}
//		return;
//	}
//
//	//sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]); 
//	//for (i = 0; i < nLength; i++)
//	//{
//	//	if (((BYTE)pData[i] == 0xEB) && ((BYTE)pData[i+1] == 0x90)
//	//		&& ((BYTE)pData[i+2] == 0xEB) && ((BYTE)pData[i+3] == 0x90)
//	//		&& ((BYTE)pData[i+4] == 0xEB) && ((BYTE)pData[i+5] == 0x90))
//	//	{
//	//		cdt_len_full = (BYTE)pData[i+8] * 6 + 12;   //֡����
//	//		//��ȥ����ͷ��ʣ�²���������һ֡������CDT����,pmaģ��
//	//		if (nLength - i == cdt_len_full)   
//	//		{
//	//			memcpy(cdt_complete_buf,&pData[i],cdt_len_full);
//	//			cdt_frame_finish = true;
//	//			return;
//	//		}
//
//	//		//�����б���ͷ�����Ǳ��Ĳ���������һ֡Ҫ�����洢��������
//
//	//		//һ֡���İ�������������ͷ
//
//	//		//��һ֡�������ñ���
//
//
//	//		memcpy(cdt_complete_buf,&pData[i],(BYTE)pData[i]);
//	//		break;
//	//	}
//	//	
//
//}
	
void CNetModulaData::cdt_rebuild( char* pData, int nLength, int nType)
{
		int pos = 0;   //  pos/2���ֽ���
		int cur_position = 0;  //
		int cnt = 0;  //֡��Ŀ����������
		//memset(temp,0,2);
		int i,j;
		int frame_length;  //frame_length + 2	
	
		int ms = 0;
		bool Is_wrong = false;
		int byte_num = 0;
		byte_num = nLength;
		int cdt_frame_length = 0;
		int cdt_len_full = 0;
		int cdt_frame_list_len;
	
		int frame_len = nLength;
		int cdt_frame_append;
		int cdt_begin_position;

		for (i = 0; i < nLength; i++)
		{
			cdt_frame_list << (BYTE)pData[i];		
		}

		cdt_frame_list_len = cdt_frame_list.size();

		if (!cdt_frame_list.isEmpty())
		{
			/*for (i = 0; i < cdt_frame_list_len; i++)
			{*/
			cdt_begin_position = cdt_frame_list.indexOf(0xEB);
			if ((cdt_begin_position >= 0) && (cdt_begin_position + 5 <= cdt_frame_list_len))
			{
				if ((cdt_frame_list.at(cdt_begin_position) == 0xEB) && (cdt_frame_list.at(cdt_begin_position+1) == 0x90)
					&& (cdt_frame_list.at(cdt_begin_position+2) == 0xEB) && (cdt_frame_list.at(cdt_begin_position+3) == 0x90)
					&& (cdt_frame_list.at(cdt_begin_position+4) == 0xEB) && (cdt_frame_list.at(cdt_begin_position+5) == 0x90))
				{
					cdt_frame_append = cdt_begin_position;  //����ͷ����λ��
					j = cdt_begin_position;


					cdt_len_full = cdt_frame_list.at(cdt_begin_position+8) * 6 + 12;   //����֡����
					if (cdt_frame_list_len - cdt_begin_position >= cdt_len_full)
					{
						for (i = 0; i < cdt_len_full; i++,j++)
						{
							cdt_complete_buf[i] = cdt_frame_list.at(j);
						}

						//analyze_cdt_asc((BYTE*)cdt_complete_buf, cdt_len_full, nType);
						analyze_cdt_asc(cdt_complete_buf, cdt_len_full, nType);
						memset(cdt_complete_buf, 0, sizeof(cdt_complete_buf));



						//һ֡����
						if (cdt_frame_list_len == cdt_len_full)
						{
							for (i = 0; i < cdt_frame_list_len; i++)
							{
								//cdt_frame_list.removeAt(i);
								cdt_frame_list.removeFirst();
							}
							return;
						}

						if (cdt_frame_list_len > cdt_len_full)
						{
							if (cdt_frame_list_len - cdt_frame_append -cdt_len_full == 0)
							{
								for (i = 0; i < cdt_frame_list_len; i++)
								{
									cdt_frame_list.removeFirst();
								}
								return;

							}
							if (cdt_frame_list_len - cdt_frame_append -cdt_len_full > 0)
							{
								for (i = 0; i < cdt_frame_append + cdt_len_full; i++)
								{
									cdt_frame_list.removeFirst();
								}
								return;
							}


						}

						if (cdt_frame_list_len < cdt_len_full)
						{
							cdt_untotal_frame(nType);
							return;
						}
						//
					}
				}
				cdt_untotal_frame(nType);
			}
			else
				cdt_untotal_frame(nType);
			    return;

			
			//}   //for

		}
		return;


}
















void CNetModulaData::analyze_cdt_asc( BYTE* pData, int nLength, int nType)
{
	char temp[2];   //ԭʼchar����
	int pos = 0;   //  pos/2���ֽ���
	int cur_position = 0;  //
	int cnt = 0;  //֡��Ŀ����������
	memset(temp,0,2);
	int i;
	int frame_length;  //frame_length + 2	
	int frame_len = 0;
	int ms = 0;
	bool Is_wrong = false;
	int byte_num = 0;
	byte_num = nLength;
	int sendnum = 0;
	int recvnum = 0;

	
	int cdt_type = 0;   //֡���
	int cdt_signal_length = 0;  //��Ϣ����
	int cdt_signal_begin = 0;  //��Ϣ��ʼλ
	int cdt_signal_seq = 0;
	int cdt_func = 0;   //������

	int cdt_di_value = 0;
	int cdt_mea_value = 0;

	//m_exchange = new QString;   //0717

	while (cur_position < nLength/2) // pos/2���ֽ���
	{
		memset(temp,0,10);

		//����ͷ����
		//if((BYTE)m_pData[cur_position] != 0xEB || (BYTE)m_pData[cur_position+1] != 0x90 ||
		//	(BYTE)m_pData[cur_position+2] != 0xEB || (BYTE)m_pData[cur_position+3] != 0x90 ||
		//	(BYTE)m_pData[cur_position+4] != 0xEB || (BYTE)m_pData[cur_position+5] != 0x90)  
		//{
		//	frame_length = (BYTE)m_pData[cur_position+8] * 6 + 12;
		//	for (cur_position = 0; cur_position < nLength; cur_position++)
		//	{
		//		sprintf(temp, "%02X", (BYTE)m_pData[cur_position]);
		//		m_exchange += temp;
		//		m_exchange += remind_space;
		//	}
		//	m_exchange += cdt_header_error; 
		//	cur_position = 0;
		//	//continue;
		//	return;
		//}

		//���ĳ��Ȳ���
		//if(((BYTE)m_pData[cur_position+8] * 6 + 12) != nLength)  
		//{
		//	for (cur_position = 0; cur_position < nLength; cur_position++)
		// {
		//	 sprintf(temp, "%02X", (BYTE)m_pData[cur_position]);
		//	 m_exchange += temp;
		//	 m_exchange += remind_space;
		// }
		//	m_exchange += cdt_len_error; 
		//	cur_position = 0;
		//	//continue;
		//	return;
		//}

		/*****************************�������Ĵ�����********************************/


		//����ͷ6�ֽ�
		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //EB
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //90
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //EB
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //90
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //EB
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //90
		m_exchange += temp;
		m_exchange += cdt_header;
		m_exchange += remind_space;

		m_exchange += "*";    //����ָ���,0711
		cdt_frame_count++;

		//������1�ֽ�
		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //71
		m_exchange += temp;
		m_exchange += cdt_control_field;
		m_exchange += remind_space;


		//֡���
		cdt_type = (BYTE)pData[cur_position];              //֡���
		sprintf(temp, "%02X", (BYTE)pData[cur_position]);  
		m_exchange += temp;
		m_exchange += remind_leftkuohao;
		m_exchange += cdt_frame_sort;

	

		if (nType == 1)   //RECV
		{
			switch (cdt_type)
			{
			case CDT_FRAME_A:
				m_exchange += cdt_frame_a;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //�ƶ�����Ϣ����

				//��Ϣ����
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Դվ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Ŀ��վ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//У����
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				m_exchange += "*";    //����ָ���,0711
				cdt_frame_count++;
				cur_position++;

				//��Ϣ����
				
				for (i = 0; i < cdt_signal_length; i++)
				{
					cdt_signal_begin = (BYTE)pData[cur_position];   //��Ϣ��ʼλ
					
					if ((cdt_signal_begin >= 0x00) && (cdt_signal_begin  <= 0x7F))  //ң�⹦���뷶Χ
					{
						//ң�⹦���루��ʼ�ţ�
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_yc_begin;
						cdt_signal_seq = 2 * (BYTE)pData[cur_position] + 1;    //ң�����
						sprintf(temp,"%d",cdt_signal_seq);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;
						

						cdt_mea_value = sghextoint_cdt((BYTE*)&pData[cur_position], 2);
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //ң�����ֽ�,��һ��
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("ֵ:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						cdt_mea_value = sghextoint_cdt((BYTE*)&pData[cur_position], 2);
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //ң�����ֽ�,�ڶ���
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("ֵ:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //У����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //����ָ���,0711
						cdt_frame_count++;
						cur_position++;
					}

				}
				break;

			case CDT_FRAME_B:
				m_exchange += cdt_frame_b;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //�ƶ�����Ϣ����

				//��Ϣ����
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Դվ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Ŀ��վ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//У����
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				m_exchange += "*";    //����ָ���,0711
				cdt_frame_count++;
				cur_position++;

				//��Ϣ����

				for (i = 0; i < cdt_signal_length; i++)
				{
					cdt_signal_begin = (BYTE)pData[cur_position];   //��Ϣ��ʼλ

					if ((cdt_signal_begin >= 0x00) && (cdt_signal_begin  <= 0x7F))  //ң�⹦���뷶Χ
					{
						//ң�⹦���루��ʼ�ţ�
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_yc_begin;
						cdt_signal_seq = 2 * (BYTE)pData[cur_position] + 1;    //ң�����
						sprintf(temp,"%d",cdt_signal_seq);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;


						cdt_mea_value = sghextoint_cdt((BYTE*)&pData[cur_position], 2);
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //ң�����ֽ�,��һ��
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("ֵ:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						cdt_mea_value = sghextoint_cdt((BYTE*)&pData[cur_position], 2);
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //ң�����ֽ�,�ڶ���
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("ֵ:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //У����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //����ָ���,0711
						cdt_frame_count++;
						cur_position++;
					}

				}
				break;

			case CDT_FRAME_C:
				m_exchange += cdt_frame_c;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //�ƶ�����Ϣ����

				//��Ϣ����
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Դվ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Ŀ��վ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//У����
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				m_exchange += "*";    //����ָ���,0711
				cdt_frame_count++;
				cur_position++;

				//��Ϣ����

				for (i = 0; i < cdt_signal_length; i++)
				{
					cdt_signal_begin = (BYTE)pData[cur_position];   //��Ϣ��ʼλ

					if ((cdt_signal_begin >= 0x00) && (cdt_signal_begin  <= 0x7F))  //ң�⹦���뷶Χ
					{
						//ң�⹦���루��ʼ�ţ�
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_yc_begin;
						cdt_signal_seq = 2 * (BYTE)pData[cur_position] + 1;    //ң�����
						sprintf(temp,"%d",cdt_signal_seq);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;


						cdt_mea_value = sghextoint_cdt((BYTE*)&pData[cur_position], 2);
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //ң�����ֽ�,��һ��
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("ֵ:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						cdt_mea_value = sghextoint_cdt((BYTE*)&pData[cur_position], 2);
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //ң�����ֽ�,�ڶ���
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("ֵ:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //У����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //����ָ���,0711
						cdt_frame_count++;
						cur_position++;
					}

				}
				break;

			case CDT_FRAME_DI:
				m_exchange += cdt_frame_di;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //�ƶ�����Ϣ����


				//��Ϣ����
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Դվ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Ŀ��վ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//У����
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;


				m_exchange += "*";    //����ָ���,0711
				cdt_frame_count++;

				//��Ϣ����

				for (i = 0; i < cdt_signal_length; i++)
				{
					cdt_signal_begin = (BYTE)pData[cur_position];   //��Ϣ��ʼλ

					if ((cdt_signal_begin >= 0xF0) && (cdt_signal_begin  <= 0xFF))  //ң�Ź����뷶Χ
					{
						//ң�Ź����루��ʼ�ţ�
						//sprintf(temp, "%02X", (BYTE)m_pData[cur_position]);
						//m_exchange += temp;
						//m_exchange += remind_leftkuohao;
						//m_exchange += cdt_func_yx_begin;
						//cdt_signal_seq = 32 * ((BYTE)m_pData[cur_position] - 240) + 1;    //ң�����
						//sprintf(temp,"%d",cdt_signal_seq);
						//m_exchange += temp;
						//m_exchange += remind_rightkuohao;
						//m_exchange += remind_space;
						//cur_position++;

						//ң�Ź�����
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;

						switch (cdt_signal_begin)
						{
						case 0xF0:
							m_exchange += _C2Q("ң��1-32");
							break;
						case 0xF1:
							m_exchange += _C2Q("ң��33-64");
							break;
						case 0xF2:
							m_exchange += _C2Q("ң��65-96");
							break;
						case 0xF3:
							m_exchange += _C2Q("ң��97-128");
							break;
						case 0xF4:
							m_exchange += _C2Q("ң��129-160");
							break;
						case 0xF5:
							m_exchange += _C2Q("ң��161-192");
							break;
						case 0xF6:
							m_exchange += _C2Q("ң��193-224");
							break;
						case 0xF7:
							m_exchange += _C2Q("ң��225-256");
							break;
						case 0xF8:
							m_exchange += _C2Q("ң��257-288");
							break;
						case 0xF9:
							m_exchange += _C2Q("ң��289-320");
							break;
						case 0xFA:
							m_exchange += _C2Q("ң��321-352");
							break;
						case 0xFB:
							m_exchange += _C2Q("ң��353-384");
							break;
						case 0xFC:
							m_exchange += _C2Q("ң��385-416");
							break;
						case 0xFD:
							m_exchange += _C2Q("ң��417-448");
							break;
						case 0xFE:
							m_exchange += _C2Q("ң��449-480");
							break;
						case 0xFF:
							m_exchange += _C2Q("ң��481-512");
							break;
						default:
							break;
						}

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;


						cdt_di_value = (BYTE)pData[cur_position];
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //ң�����ֽ�1
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						
						if ((cdt_di_value & 0x01) == 0x01)  //ң��1
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x02) == 0x02)  //ң��2
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x04) == 0x04)  //ң��3
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x08) == 0x08)  //ң��4
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x10) == 0x10)  //ң��5
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x20) == 0x20)  //ң��6
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x40) == 0x40)  //ң��7
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x80) == 0x80)  //ң��8
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");
						
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						cdt_di_value = (BYTE)pData[cur_position];
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //ң�����ֽ�2
						m_exchange += temp;
						m_exchange += remind_leftkuohao;

						if ((cdt_di_value & 0x01) == 0x01)  //ң��1
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x02) == 0x02)  //ң��2
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x04) == 0x04)  //ң��3
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x08) == 0x08)  //ң��4
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x10) == 0x10)  //ң��5
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x20) == 0x20)  //ң��6
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x40) == 0x40)  //ң��7
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x80) == 0x80)  //ң��8
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						cdt_di_value = (BYTE)pData[cur_position];
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //ң�����ֽ�3
						m_exchange += temp;
						m_exchange += remind_leftkuohao;

						if ((cdt_di_value & 0x01) == 0x01)  //ң��1
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x02) == 0x02)  //ң��2
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x04) == 0x04)  //ң��3
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x08) == 0x08)  //ң��4
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x10) == 0x10)  //ң��5
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x20) == 0x20)  //ң��6
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x40) == 0x40)  //ң��7
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x80) == 0x80)  //ң��8
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						cdt_di_value = (BYTE)pData[cur_position];
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //ң�����ֽ�4
						m_exchange += temp;
						m_exchange += remind_leftkuohao;

						if ((cdt_di_value & 0x01) == 0x01)  //ң��1
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x02) == 0x02)  //ң��2
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x04) == 0x04)  //ң��3
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x08) == 0x08)  //ң��4
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x10) == 0x10)  //ң��5
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x20) == 0x20)  //ң��6
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x40) == 0x40)  //ң��7
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						if ((cdt_di_value & 0x80) == 0x80)  //ң��8
						{
							m_exchange += _C2Q("��");
						}
						else
							m_exchange += _C2Q("��");

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //У����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //����ָ���,0711
						cdt_frame_count++;
						cur_position++;
					}

				}
			
				break;
			case CDT_FRAME_PS:
				m_exchange += cdt_frame_ps;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //�ƶ�����Ϣ����

				//��Ϣ����
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Դվ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Ŀ��վ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//У����
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				m_exchange += "*";    //����ָ���,0711
				cdt_frame_count++;
				cur_position++;

				//��Ϣ����

				for (i = 0; i < cdt_signal_length; i++)
				{
					cdt_signal_begin = (BYTE)pData[cur_position];   //��Ϣ��ʼλ

					if ((cdt_signal_begin >= 0xA0) && (cdt_signal_begin  <= 0xDF))  //ң�������뷶Χ
					{
						//ң�������루��ʼ�ţ�
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_ym_begin;
						cdt_signal_seq = (BYTE)pData[cur_position] - 159;    //ң����ţ�-0xa0 + 1
						sprintf(temp,"%d",cdt_signal_seq);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;



						if (((BYTE)pData[cur_position + 3] & 0x20) == 0x00)   //�������룬b29 == 0
						{
							cdt_mea_value = usghextoint((BYTE*)&pData[cur_position], 3); 
						}

						if (((BYTE)pData[cur_position + 3] & 0x20) == 0x20)   //BCD�룬b29 == 1
						{
							cdt_mea_value = ((BYTE)pData[cur_position] & 0x0F) * 0.01
								             + (((BYTE)pData[cur_position] & 0xF0) >> 4) * 0.1
											 +((BYTE)pData[cur_position+1] & 0x0F) * 1
											 + (((BYTE)pData[cur_position+1] & 0xF0) >> 4) * 10
											 +((BYTE)pData[cur_position+2] & 0x0F) * 100
											 + (((BYTE)pData[cur_position+2] & 0xF0) >> 4) * 1000; 
						}
						 
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);    //����4�ֽ�
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);    //����4�ֽ�
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);    //����4�ֽ�
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);    //����4�ֽ�
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("ֵ:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //У����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //����ָ���,0711
						cdt_frame_count++;
						cur_position++;
					}

				}				

				break;
			case CDT_FRAME_SOE:
				break;
			case CDT_FRAME_CTRL_CON:
				break;
			default:
				break;
			}
		}

		if (nType == 2)   //SEND
		{
			switch (cdt_type)
			{
			case CDT_FRAME_CTRL_SEL:
				m_exchange += cdt_frame_ctrl_sel;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //�ƶ�����Ϣ����


				//��Ϣ����
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Դվ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Ŀ��վ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//У����
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;


				m_exchange += "*";    //����ָ���,0711
				cdt_frame_count++;


				//E0 CC 06 CC 06 F8
				//E0 CC 06 CC 06 F8
				//E0 CC 06 CC 06 F8
				for (i = 0; i < cdt_signal_length; i++)
				{
					if ( (BYTE)pData[cur_position] == 0xE0)  //ң��ѡ��
					{
						
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //E0
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_ctrl_sel;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //CC
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						if((BYTE)pData[cur_position] == 0xCC)
						{
							m_exchange += _C2Q("�غ�");
						}
						if((BYTE)pData[cur_position] == 0x33)
						{
							m_exchange += _C2Q("�ط�");

						}
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //06
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						sprintf(temp, "%d", (BYTE)pData[cur_position]); 
						m_exchange += _C2Q("ң�غ�:");
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //CC
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						if((BYTE)pData[cur_position] == 0xCC)
						{
							m_exchange += _C2Q("�غ�");
						}
						if((BYTE)pData[cur_position] == 0x33)
						{
							m_exchange += _C2Q("�ط�");

						}
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //06
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						sprintf(temp, "%d", (BYTE)pData[cur_position]); 
						m_exchange += _C2Q("ң�غ�:");
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //У����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //����ָ���,0711
						cdt_frame_count++;
						cur_position++;					
					}

				}
			
				
				break;
			case CDT_FRAME_CTRL_EXE:
				m_exchange += cdt_frame_ctrl_exe;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //�ƶ�����Ϣ����


				//��Ϣ����
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Դվ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Ŀ��վ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//У����
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;


				m_exchange += "*";    //����ָ���,0711
				cdt_frame_count++;


				//E2 AA 06 AA 06 F8
				//E2 AA 06 AA 06 F8
				//E2 AA 06 AA 06 F8
				for (i = 0; i < cdt_signal_length; i++)
				{
					if ( (BYTE)pData[cur_position] == 0xE2)  //ң��ִ��
					{

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //E2
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_ctrl_exe;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //AA
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						if((BYTE)pData[cur_position] == 0xAA)
						{
							m_exchange += _C2Q("ִ��");
						}
					
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //06
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						sprintf(temp, "%d", (BYTE)pData[cur_position]); 
						m_exchange += _C2Q("ң�غ�:");
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //AA
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						if((BYTE)pData[cur_position] == 0xAA)
						{
							m_exchange += _C2Q("ִ��");
						}
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //06
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						sprintf(temp, "%d", (BYTE)pData[cur_position]); 
						m_exchange += _C2Q("ң�غ�:");
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //У����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //����ָ���,0711
						cdt_frame_count++;
						cur_position++;					
					}

				}


				break;
			case CDT_FRAME_CTRL_CAN:
				m_exchange += cdt_frame_ctrl_can;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //�ƶ�����Ϣ����


				//��Ϣ����
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Դվ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//Ŀ��վ��ַ
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//У����
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;


				m_exchange += "*";    //����ָ���,0711
				cdt_frame_count++;


				//E3 55 06 55 06 F2
				//E3 55 06 55 06 F2
				//E3 55 06 55 06 F2
				for (i = 0; i < cdt_signal_length; i++)
				{
					if ( (BYTE)pData[cur_position] == 0xE3)  //ң�س���
					{

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //E3
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_ctrl_cle;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //55
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						if((BYTE)pData[cur_position] == 0x55)
						{
							m_exchange += _C2Q("����");
						}

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //06
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						sprintf(temp, "%d", (BYTE)pData[cur_position]); 
						m_exchange += _C2Q("ң�غ�:");
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //AA
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						if((BYTE)pData[cur_position] == 0x55)
						{
							m_exchange += _C2Q("����");
						}
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //06
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						sprintf(temp, "%d", (BYTE)pData[cur_position]); 
						m_exchange += _C2Q("ң�غ�:");
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //У����
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //����ָ���,0711
						cdt_frame_count++;
						cur_position++;					
					}

				}
				break;
			case CDT_FRAME_UPDOWN_SEL:	
				break;
			case CDT_FRAME_UPDOWN_EXE:	
				break;
			case CDT_FRAME_UPDOWN_CAN:	
				break;
			case CDT_FRAME_IDI_SET:	
				break;
			case CDT_FRAME_CLOCK_SET:	
				break;
			case CDT_FRAME_CLOCK_CON:	
				break;
			case CDT_FRAME_CLOCK_SUM:	
				break;
			case CDT_FRAME_RESET:	
				break;
			case CDT_FRAME_BROD:	
				break;
			default:
				break;
			
			}
		}


	}



}



// bool CNetModulaData::MatchKeyWord(CNetKeyWord* pKeyWord)
// {
// 	QList<int> listMatchIndex;
// 
// 	if(pKeyWord->m_ListSection.isEmpty())
// 		return false;
// 
// 	bool bMatchFixed = false;
// 	int nCurrentIndex = 0;
// 	QListIterator<CNetKeySection*> findIterator(pKeyWord->m_ListSection);
// 	findIterator.toFront();
// 	while(findIterator.hasNext())
// 	{
// 		if(nCurrentIndex >= m_nLength)
// 			return false;
// 
// 		CNetKeySection* pSetction = findIterator.next();
// 		if(pSetction)
// 		{
// 			switch(pSetction->GetType())
// 			{
// 			case KEYSECTION_DATA:
// 				{
// 					if(bMatchFixed)
// 					{
// 						if(pSetction->GetKey() != (uchar)(m_pData[nCurrentIndex]))
// 							return false;
// 						listMatchIndex.push_back(nCurrentIndex);
// 						nCurrentIndex++;
// 					}
// 					else
// 					{
// 						while(nCurrentIndex < m_nLength)
// 						{
// 							if(pSetction->GetKey() == (uchar)(m_pData[nCurrentIndex]))
// 								break;
// 							nCurrentIndex++;
// 						}
// 						if(nCurrentIndex >= m_nLength)
// 							return false;
// 						listMatchIndex.push_back(nCurrentIndex);
// 						nCurrentIndex++;
// 					}
// 					bMatchFixed = true;
// 				}
// 				break;
// 
// 			case KEYSECTION_ANYONE:
// 				bMatchFixed = true;
// 				nCurrentIndex++;
// 				break;
// 
// 			case KEYSECTION_ANYMORE:
// 				bMatchFixed = false;
// 				break;
// 
// 			default:
// 				return false;
// 				break;
// 			}
// 		}
// 	}
// 
// 	if(listMatchIndex.isEmpty())
// 		return false;
// 
// 	QListIterator<int> setIterator(listMatchIndex);
// 	setIterator.toFront();
// 	while(setIterator.hasNext())
// 	{
// 		int nSetIndex = setIterator.next();
// 		if((nSetIndex >= 0) && (nSetIndex < m_nLength))
// 			m_pKeyFlag[nSetIndex] = 1;
// 	}
// 	return true;
// }
