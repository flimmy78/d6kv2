#ifndef DE_PUB_H
#define DE_PUB_H

#ifdef __cplusplus
extern "C" {
#endif

typedef   signed short     sdint2;
typedef unsigned short     udint2;

typedef   signed int       sdint4;                   
typedef unsigned int       udint4; 

#ifdef WIN32
typedef __int64            sdint8;
typedef unsigned __int64   udint8;
#else
typedef long long int          sdeint8;
typedef unsigned long long int udeint8;
#endif

typedef signed char         sdbyte;
typedef unsigned char       udbyte;

typedef double              ddouble;

#define NSTR_LOCAL_LEN  48

typedef struct de_str_struct de_str;
struct de_str_struct
{
    udint4            len;    /*�ַ�������*/
    udbyte*           data;   /*�ַ���ָ��*/
    udbyte            local_space[NSTR_LOCAL_LEN];  /*local_space�洢���ַ�����'\0'������ʵ��ֻ�ܴ洢47���ַ�*/
};


typedef struct de_data_struct de_data;
struct de_data_struct
{
	udint4 null_flag; /*�����Ƿ�Ϊ�գ�1��ʾ�ǿգ�0��ʾ��*/
	union	
    {
		sdint4   v_int;
		ddouble  v_double;
		de_str   v_str;
	}data;
};


typedef struct de_args_struct de_args;
struct de_args_struct
{
	udint4        n_args;      /*��������*/
    de_data*      args;        /*�����б�*/
};

/**
 * <p>����˵�������ص�arg_id������Ϊ����ֵ</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> args�������б���Ϣ��������Ϊ��</li>
   <li> arg_id��ȡ��ָ��id�Ĳ���ֵ����ʼֵΪ0��������Ϊ��</li>
* </ul></p>
 * <p>����ֵ ����arg_id����������ֵ</p>
 */
sdint4 
de_get_int(
    de_args* args, 
    udint4   arg_id
);

/**
 * <p>����˵�������ص�arg_id������Ϊdouble����ֵ</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> args�������б���Ϣ��������Ϊ��</li>
   <li> arg_id��ȡ��ָ��id�Ĳ���ֵ����ʼֵΪ0��������Ϊ��</li>
* </ul></p>
 * <p>����ֵ ����arg_id������double����ֵ</p>
 */
ddouble 
de_get_double(
    de_args*  args, 
    udint4    arg_id
);

/**
 * <p>����˵�������ص�arg_id�����ĵ��ַ�������</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> args�������б���Ϣ��������Ϊ��</li>
   <li> arg_id��ȡ��ָ��id�Ĳ���ֵ����ʼֵΪ0��������Ϊ��</li>
* </ul></p>
 * <p>����ֵ ����arg_id�������ַ��������ؽ���ԡ�\0����β</p>
 */
udbyte* 
de_get_str(
    de_args* args, 
    udint4   arg_id
);

/**
 * <p>����˵�������ص�arg_id�������ַ���</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> args�������б���Ϣ��������Ϊ��</li>
   <li> arg_id��ȡ��ָ��id�Ĳ���ֵ����ʼֵΪ0��������Ϊ��</li>
	<li> alen�������ַ����ĳ���</li>
* </ul></p>
 * <p>����ֵ ����arg_id�������ַ��������ؽ�����ԡ�\0����β</p>
 */
udbyte* 
de_get_str_with_len(
    de_args*    args, 
    udint4      arg_id, 
    udint4*     alen
);

/**
 * <p>����˵�������õ�arg_id����������ֵ</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> args�������б���Ϣ��������Ϊ��</li>
   <li> arg_id������ָ��id�Ĳ���ֵ����ʼֵΪ0��������Ϊ��</li>
	<li> ret����arg_id�����µ�����ֵ </li>
* </ul></p>
 * <p>����ֵ ����</p>
 */
void 
de_set_int(
    de_args*    args, 
    udint4      arg_id, 
    sdint4      ret
);

/**
 * <p>����˵�������õ�arg_id������double����ֵ</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> args�������б���Ϣ��������Ϊ��</li>
   <li> arg_id������ָ��id�Ĳ���ֵ����ʼֵΪ0��������Ϊ��</li>
	<li> ret����arg_id�����µ�double����ֵ </li>
* </ul></p>
 * <p>����ֵ ����</p>
 */
void 
de_set_double(
    de_args* args, 
    udint4   arg_id, 
    ddouble  ret
);

/**
 * <p>����˵�������õ�arg_id�������ַ��� </p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> args�������б���Ϣ��������Ϊ��</li>
   <li> arg_id������ָ��id�Ĳ���ֵ����ʼֵΪ0��������Ϊ��</li>
	<li> ret����arg_id�����µ��ַ��������ַ����ԡ�\0����β</li>
* </ul></p>
 * <p>����ֵ ����</p>
 */
void de_set_str(
    de_args*    args, 
    udint4      arg_id, 
    udbyte*     ret
);

/**
 * <p>����˵�������õ�arg_id�������ַ��� </p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> args�������б���Ϣ��������Ϊ��</li>
   <li> arg_id������ָ��id�Ĳ���ֵ����ʼֵΪ0��������Ϊ��</li>
<li> alen�����ַ����ĳ���</li>
	<li> ret����arg_id�����µ��ַ��������ַ�����Ҫ���ԡ�\0����β</li>
* </ul></p>
 * <p>����ֵ ����</p>
 */
void 
de_set_str_with_len(
    de_args*    args, 
    udint4      arg_id, 
    udbyte*     ret, 
    udint4      alen
);

/**
 * <p>����˵�������ص�arg_id����������ֵ</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> ret����Ҫ���ص�����ֵ</li>
* </ul></p>
 * <p>����ֵ ������</p>
 */
de_data 
de_return_int(
    sdint4 ret
);

/**
 * <p>����˵�������ص�arg_id������double����ֵ</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> ret����Ҫ���ص�double����ֵ</li>
* </ul></p>
 * <p>����ֵ ��double����</p>
 */
de_data 
de_return_double(
    ddouble ret
);

/**
 * <p>����˵�������ص�arg_id�������ַ���</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> ret����Ҫ���ص��ַ��������ַ����ԡ�\0����β</li>
* </ul></p>
 * <p>����ֵ ���ַ��� </p>
 */
de_data
de_return_str(
    udbyte* ret
);

/**
 * <p>����˵�������ص�arg_id�������ַ���</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> ret����Ҫ���ص��ַ��������ַ�����Ҫ���ԡ�\0����β</li>
  <li> alen���ַ�������</li>
* </ul></p>
 * <p>����ֵ ���ַ��� </p>
 */
de_data 
de_return_str_with_len(
    udbyte*    ret, 
    udint4     alen
);

/**
 * <p>����˵�����ͷſռ�</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
   <li> str��ָ����Ҫ�ͷ��ַ��ռ��ָ��</li>
* </ul></p>
 * <p>����ֵ ���� </p>
 */
void 
de_str_free(
    sdbyte* str
);

/**
 * <p>����˵�����жϲ����Ƿ�Ϊ��</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
      <li> args�������б���Ϣ��������Ϊ��</li>
      <li> arg_id������ָ��id�Ĳ���ֵ����ʼֵΪ0��������Ϊ��</li>
* </ul></p>
 * <p>����ֵ ���������� </p>
 */
udint4 
de_is_null(
    de_args*  args, 
    udint4    arg_id
);

/**
 * <p>����˵�������ò���Ϊ��</p>
 * <p>�㷨��������</p>
 * <p>�����б�<br><ul>
      <li> args�������б���Ϣ��������Ϊ��</li>
      <li> arg_id������ָ��id�Ĳ���ֵ����ʼֵΪ0��������Ϊ��</li>
* </ul></p>
 * <p>����ֵ ���� </p>
 */
void  
de_set_null(
    de_args*    args, 
    udint4      arg_id
);

/**
 * <p>����˵�������ؿ�ֵ</p>
 * <p>�㷨��������</p>
 * <p>�����б���<br><ul>
* </ul></p>
 * <p>����ֵ ���� </p>
 */
de_data 
de_return_null();

#ifdef __cplusplus
}
#endif

#endif

