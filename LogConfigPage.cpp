// LogConfigPage.cpp : 实现文件
//

#include "stdafx.h"
#include "W-1023厂测.h"
#include "LogConfigPage.h"
#include "afxdialogex.h"


// CLogConfigPage 对话框

IMPLEMENT_DYNAMIC(CLogConfigPage, CDialogEx)

CLogConfigPage::CLogConfigPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGCONF_PAGE_DIALOG, pParent)
{
	m_ccdParent = (CW1023厂测Dlg *)pParent;
}

CLogConfigPage::~CLogConfigPage()
{
}

void CLogConfigPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_LOCATION_EDIT, m_editLogFilePath);
}


BEGIN_MESSAGE_MAP(CLogConfigPage, CDialogEx)
	ON_BN_CLICKED(IDC_LOGLOC_CHOSE_BUTTON, &CLogConfigPage::OnBnClickedLoglocChoseButton)
	ON_BN_CLICKED(IDOK, &CLogConfigPage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLogConfigPage::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLogConfigPage 消息处理程序

void CLogConfigPage::OnBnClickedLoglocChoseButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilePath;

	TCHAR szFilter[] = _T("txt文件(*.tx)|*.txt|所有文件(*.*)|*.*||");

	CFileDialog fileDlg(FALSE, _T("txt"), _T("w1023-Log"), OFN_HIDEREADONLY, szFilter, this);

	if (IDOK == fileDlg.DoModal()) {
		strFilePath = fileDlg.GetPathName();
		m_editLogFilePath.SetWindowText(strFilePath);
	}
}


void CLogConfigPage::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilePath;

	m_editLogFilePath.GetWindowText(strFilePath);

	if (FALSE == strFilePath.IsEmpty()) {
		m_ccdParent->SetLogPath(strFilePath);
	}

	CDialogEx::OnOK();
}


void CLogConfigPage::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CLogConfigPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	CString strTemp;

	m_ccdParent->GetLogPath(strTemp);
	m_editLogFilePath.SetWindowText(strTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
