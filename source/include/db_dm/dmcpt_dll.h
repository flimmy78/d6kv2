/********************************************************************
file: 
    cpt.c
Notes:
    capture interface
History:
    Date         Who         RefDoc			Memo
    2012-11-06	 xh.wng    					moved	
********************************************************************/
#ifndef _DMCPT_DLL_H
#define _DMCPT_DLL_H

#include "DPI.h"

#define CPT_LANGUAGE_CN			    0	/**<p>����</p>*/ 
#define CPT_LANGUAGE_EN				1	/**<p>Ӣ��</p>*/

#define DM_MAX_CPT_ERROR_INFO_LEN	512
typedef struct cpt_error_info_struct cpt_error_info_t;
struct cpt_error_info_struct
{
	sdint4		error_no;
	sdbyte		error_info[DM_MAX_CPT_ERROR_INFO_LEN];
};

typedef struct cpt_col_def_struct cpt_col_def_t;
struct cpt_col_def_struct 
{
    sdbyte			name[129];		//--�ֶ����ƣ������벶������ֶ�һ��
	sdbyte			col_define[129];		//--�ֶ����ͣ�������ֻ���ڱ��ʽ��Ϊ��ʱָ������Ч����ָ��col_defineʱ��Ҫָ�������Ķ��壬��int��varchar(200)��numeric(10,2)
	sdbyte			expr[1024];		//--�ֶα��ʽ�����Ϊ�մ�����������
};

#define CPT_WITH_API	1

/**
* <p>����˵�������ݲ���Ĵ����ӿ�</p>
* <p>�㷨��������</p>
* <p>�����б�<br><ul>
* <li>con_hdbc, --���룬���ݿ��connection���</li>
* <li>schname,  --���룬�����������ģʽ</li>
* <li>tvname,  --���룬�����/��ͼ��</li>
* <li>wheres,  --���룬��������������������ȷָ����ֵ���ֵ��Ϊ��������'OLD:ID < 10 AND NEW:ID > 10' ��û����������Ϊ"";</li>
* <li>col_defs, --���룬�ֶ�����</li>
* <li>n_cols,  --���룬�����ֶ���</li>
* <li>cdc_id,  --��������������ʶ����Ӧ�仯����</li>
* <li>cdc_blob_id, --���룬��ʶ���������ȣ���������ڵ���129 </li>
* <li>cdc_blob_id, --��������������ʶ����Ӧ�仯���ֶα����</li>
* <li>cdc_blob_id_len,--���룬���ֶα�ʶ���������ȣ���������ڵ���129   </li>
* <li>error_info, --������������������ԭ��</li>
* </ul></p>
* <p>����ֵ: udint4, �Ƿ�ɹ�, {TRUE, FALSE}</p>
*/
DllExport
udint4
dm_create_change_data_capture(
#if CPT_WITH_API
	dhcon con_hdbc,
#else
	void *pidao_connection,               //--���룬IDAO_Connection�ӿ�ָ��
#endif   
   sdbyte*		schname,			
   sdbyte*		tvname,				
   sdbyte*		wheres,				
   cpt_col_def_t*	col_defs,	        
   udint2		n_cols,				
   sdbyte*		cdc_id,				
   udint4		cdc_id_len,		    
   sdbyte*		cdc_blob_id,		
   udint4		cdc_blob_id_len,	    
   cpt_error_info_t* error_info			
);

/**
* <p>����˵�������ݲ����ɾ���ӿ�</p>
* <p>�㷨��������</p>
* <p>�����б�<br><ul>
* <li>con_hdbc, --���룬���ݿ��connection���</li>
* <li>cdc_id,  --���룬���������ʶ����Ӧ�仯����</li>
* <li>error_info, --������������������ԭ��</li>
* </ul></p>
* <p>����ֵ: udint4, �Ƿ�ɹ�, {TRUE, FALSE}</p>
*/
DllExport
udint4
dm_drop_change_data_capture(
#if CPT_WITH_API
	dhcon con_hdbc,
#else
	void *pidao_connection,               //--���룬IDAO_Connection�ӿ�ָ��
#endif
   sdbyte*		cdc_id,
   cpt_error_info_t* error_info
);

/**
* <p>����˵�������ݲ�������Խӿ�</p>
* <p>�㷨��������</p>
* <p>�����б�<br><ul>
* <li>code_id, --���룬���ݲ���ģ���������(Ԥ��)</li>
* <li>lang_id, --���룬���ݲ���ģ����������</li>
* </ul></p>
* <p>����ֵ: dmbool, �Ƿ�ɹ�, {TRUE, FALSE}</p>
*/
DllExport
void
dm_cpt_set_local_code(
    sdint4      code_id, /* ��ʱ��Ч */
    sdint4      lang_id  
);

#endif
