// SDSettingPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "W-1023����.h"
#include "SDSettingPage.h"
#include "afxdialogex.h"
#include "W-1023����Dlg.h"

// CSDSettingPage �Ի���

IMPLEMENT_DYNAMIC(CSDSettingPage, CDialogEx)

CSDSettingPage::CSDSettingPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTING_PAGE, pParent)
{
	m_ccdParent = (CW1023����Dlg *)pParent;
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


// CSDSettingPage ��Ϣ�������


void CSDSettingPage::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AfxMessageBox(_T("ȷ����ť������"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AfxMessageBox(_T("ȡ����ť������"));
	CDialogEx::OnCancel();
}


BOOL CSDSettingPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strTempRead, strTempWrite;

	m_ccdParent->GetSdReadStdVal(strTempRead);
	m_editSDReadMin.SetWindowText(strTempRead);

	m_ccdParent->GetSdWriteStdVal(strTempWrite);
	m_editSDWriteMin.SetWindowText(strTempWrite);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
