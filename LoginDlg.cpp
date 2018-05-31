// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "W-1023����.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGIN_DIALOG, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USER_NAME_EDIT, m_editName);
	DDX_Control(pDX, IDC_USER_PWD_EDIT, m_editPasswd);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CLoginDlg::OnBnClickedOk()
{
	CString strName;
	CString strPasswd;

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_editName.GetWindowText(strName);
	m_editPasswd.GetWindowText(strPasswd);

	//AfxMessageBox(strName);
	//AfxMessageBox(strPasswd);

	if (0 == strName.Compare(_T("root"))) {
		if (0 == strPasswd.Compare(_T("passwd"))) {
			CDialogEx::OnOK();
			return;
		}
		else {
			CDialogEx::OnCancel();
			return;
		}
	}

	CDialogEx::OnCancel();
	return;
}


void CLoginDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
