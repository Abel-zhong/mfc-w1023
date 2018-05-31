// LogConfigPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "W-1023����.h"
#include "LogConfigPage.h"
#include "afxdialogex.h"


// CLogConfigPage �Ի���

IMPLEMENT_DYNAMIC(CLogConfigPage, CDialogEx)

CLogConfigPage::CLogConfigPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGCONF_PAGE_DIALOG, pParent)
{
	m_ccdParent = (CW1023����Dlg *)pParent;
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


// CLogConfigPage ��Ϣ�������

void CLogConfigPage::OnBnClickedLoglocChoseButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;

	TCHAR szFilter[] = _T("txt�ļ�(*.tx)|*.txt|�����ļ�(*.*)|*.*||");

	CFileDialog fileDlg(FALSE, _T("txt"), _T("w1023-Log"), OFN_HIDEREADONLY, szFilter, this);

	if (IDOK == fileDlg.DoModal()) {
		strFilePath = fileDlg.GetPathName();
		m_editLogFilePath.SetWindowText(strFilePath);
	}
}


void CLogConfigPage::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;

	m_editLogFilePath.GetWindowText(strFilePath);

	if (FALSE == strFilePath.IsEmpty()) {
		m_ccdParent->SetLogPath(strFilePath);
	}

	CDialogEx::OnOK();
}


void CLogConfigPage::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


BOOL CLogConfigPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strTemp;

	m_ccdParent->GetLogPath(strTemp);
	m_editLogFilePath.SetWindowText(strTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
