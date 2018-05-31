// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "W-1023厂测.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg 对话框

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


// CLoginDlg 消息处理程序

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLoginDlg::OnBnClickedOk()
{
	CString strName;
	CString strPasswd;

	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
