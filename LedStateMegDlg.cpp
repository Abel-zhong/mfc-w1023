// LedStateMegDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "W-1023厂测.h"
#include "LedStateMegDlg.h"
#include "afxdialogex.h"


// LedStateMegDlg 对话框

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


// LedStateMegDlg 消息处理程序


void LedStateMegDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void LedStateMegDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL LedStateMegDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (1 == m_nMegFlag) {
		m_staticStateMeg.SetWindowText("LED是否全显红灯？");
	}
	else if (2 == m_nMegFlag) {
		m_staticStateMeg.SetWindowText("LED是否全显绿灯？");
	}
	else if (3 == m_nMegFlag){
		m_staticStateMeg.SetWindowText("LED是否全显蓝灯？");
	}
	else {
		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
