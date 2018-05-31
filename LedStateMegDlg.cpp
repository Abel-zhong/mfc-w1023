// LedStateMegDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "W-1023����.h"
#include "LedStateMegDlg.h"
#include "afxdialogex.h"


// LedStateMegDlg �Ի���

IMPLEMENT_DYNAMIC(LedStateMegDlg, CDialogEx)

LedStateMegDlg::LedStateMegDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

LedStateMegDlg::LedStateMegDlg(int nFlag, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_nMegFlag = nFlag;
}

LedStateMegDlg::~LedStateMegDlg()
{
}

void LedStateMegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LED_STATE, m_staticStateMeg);
}


BEGIN_MESSAGE_MAP(LedStateMegDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &LedStateMegDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &LedStateMegDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// LedStateMegDlg ��Ϣ�������


void LedStateMegDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void LedStateMegDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


BOOL LedStateMegDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (1 == m_nMegFlag) {
		m_staticStateMeg.SetWindowText("LED�Ƿ�ȫ�Ժ�ƣ�");
	}
	else if (2 == m_nMegFlag) {
		m_staticStateMeg.SetWindowText("LED�Ƿ�ȫ���̵ƣ�");
	}
	else if (3 == m_nMegFlag){
		m_staticStateMeg.SetWindowText("LED�Ƿ�ȫ�����ƣ�");
	}
	else {
		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
