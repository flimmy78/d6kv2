// QusercfgDlg.cpp: implementation of the QusercfgDlg class.
//
//////////////////////////////////////////////////////////////////////
#include "QZonePara.h"
#include "QusercfgDlg.h"
#include "globaldef.h"
#include "uitools/dmessagebox.h"

#include <qmessagebox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <QtDebug>

#include "scd/scdapi.h"
#include "auth.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


QUsercfgDlg::QUsercfgDlg( QWidget* parent, const char* name, Qt::WindowFlags fl )
:QDialog( parent, fl )
{
	ui.setupUi(this);
	m_roltype = ui.m_cbxRoletype;
	
	m_strCurrentPassword = "";

	  // tab order
   setTabOrder( ui.m_editCode, ui.m_editDesc );
   setTabOrder( ui.m_editDesc, ui.m_editPassword );
   setTabOrder( ui.m_editPassword, ui.m_cbxRoletype );
   setTabOrder( ui.m_cbxRoletype, ui.m_editRoledesc );
   setTabOrder( ui.m_editRoledesc, ui.m_rdbtnGrplder );
   setTabOrder( ui.m_rdbtnGrplder, ui.m_btOk );
   setTabOrder( ui.m_btOk, ui.m_btCancel );
   
   ui.m_editPassword->setEchoMode(QLineEdit::Password);

}

QUsercfgDlg::~QUsercfgDlg()
{
}

void QUsercfgDlg::onBtOK()
{

	if (ui.m_editCode->text().isEmpty())
	{	
		DMessageBox::information(NULL,_C2Q("����"), _C2Q("�û�������Ϊ��!") );
		return;
	}
	if (ui.m_editDesc->text().isEmpty())
	{	
		DMessageBox::information(NULL,_C2Q("����"), _C2Q("��������Ϊ��!") );
		return;
	}
	if (ui.m_editPassword->text().isEmpty())
	{	
		DMessageBox::information(NULL,_C2Q("����"), _C2Q("���벻��Ϊ��!") );
		return;
	}
	if (ui.m_editRoledesc->text().isEmpty())
	{	
		DMessageBox::information(NULL,_C2Q("����"), _C2Q("�����������Ϊ��!") );
		return;
	}
	
/*
	//��֤����
	if (!verifyPassword(ui.m_editPassword->text()))
	{
		QMessageBox::information(NULL,_C2Q("����"), _C2Q("������������д��ĸ��Сд��ĸ�����֣��Ҳ�������8λ!") );
		return;
	}
*/

	pUser = new USERPARA;
	pUser->code  = ui.m_editCode->text();
	pUser->desc = ui.m_editDesc->text();
	pUser->password = ui.m_editPassword->text();

	if (m_strCurrentPassword != pUser->password)//�޸�����
	{
		//��֤����,�������ݿ����Ѵ��ڵ��û����벻��֤
		if (!verify_password_auth(_Q2C(ui.m_editPassword->text()) ) )
		{
			QString str = _C2Q(get_error_desc_auth());
			qDebug()<<str<<"\n";

			DMessageBox::information(NULL,_C2Q("����"), str );
		
			return;
		}
		
		char tempBuf[HASH_STRING_LEN + 4];
		int outLen = HASH_STRING_LEN + 4;
		if (0 == convert_md5(_Q2C(pUser->password),tempBuf,outLen) )
		{
			tempBuf[HASH_STRING_LEN]='\0';
			pUser->password = _C2Q(tempBuf);
			qDebug()<<pUser->password<<"\n";
		}
		else
		{
			qDebug()<<"MD5 error!"<<"\n";
			return;
		}
	}
	

//	pUser->roltype  = m_roltype->currentText();
	pUser->roledesc = ui.m_editRoledesc->text();
	if(ui.m_rdbtnGrplder->isChecked ()) pUser->groupleader=1;
	else pUser->groupleader = 0;

	qDebug() << pUser->code << pUser->desc << pUser->password << pUser->roletype << pUser->groupleader <<"\n";
	
	accept();	
}

void QUsercfgDlg::refreshUserCfg(PUSERPARA pModify)
{

		m_strCurrentPassword = pModify->password;

		ui.m_editCode->setText(pModify->code);
		ui.m_editDesc->setText(pModify->desc);

		ui.m_editPassword->setEchoMode(QLineEdit::Password);//QLineEdit::PasswordEchoOnEdit
		ui.m_editPassword->setText(pModify->password);
	
		ui.m_editRoledesc->setText(pModify->roledesc);
		if(pModify->groupleader)
			ui.m_rdbtnGrplder->setChecked(TRUE);
}