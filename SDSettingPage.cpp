// SDSettingPage.cpp : 实现文件
//

#include "stdafx.h"
#include "W-1023厂测.h"
#include "SDSettingPage.h"
#include "afxdialogex.h"
#include "W-1023厂测Dlg.h"

// CSDSettingPage 对话框

IMPLEMENT_DYNAMIC(CSDSettingPage, CDialogEx)

CSDSettingPage::CSDSettingPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTING_PAGE, pParent)
{
	m_ccdParent = (CW1023厂测Dlg *)pParent;
}

CSDSettingPage::~CSDSettingPage()
{
}

void CSDSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SD_READ_MIN_EDIT, m_editSDReadMin);
	DDX_Control(pDX, IDC_SD_WRITE_MIN_EDIT, m_editSDWriteMin);
}


BEGIN_MESSAGE_MAP(CSDSettingPage, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSDSettingPage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSDSettingPage::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSDSettingPage 消息处理程序


void CSDSettingPage::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("确定按钮被按下"));
	CString strReadSetting;
	CString strWriteSetting;

	m_editSDReadMin.GetWindowText(strReadSetting);
	m_editSDWriteMin.GetWindowText(strWriteSetting);

	if (FALSE == strReadSetting.IsEmpty()) {
		m_ccdParent->SetSdReadStdVal(strReadSetting);
	}

	if (FALSE == strWriteSetting.IsEmpty()) {
		m_ccdParent->SetSdWriteStdVal(strWriteSetting);
	}

	CDialogEx::OnOK();
}


void CSDSettingPage::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("取消按钮被按下"));
	CDialogEx::OnCancel();
}


BOOL CSDSettingPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strTempRead, strTempWrite;

	m_ccdParent->GetSdReadStdVal(strTempRead);
	m_editSDReadMin.SetWindowText(strTempRead);

	m_ccdParent->GetSdWriteStdVal(strTempWrite);
	m_editSDWriteMin.SetWindowText(strTempWrite);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
