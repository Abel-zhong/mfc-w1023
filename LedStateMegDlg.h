#pragma once
#include "afxwin.h"


// LedStateMegDlg 对话框

class LedStateMegDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LedStateMegDlg)

public:
	LedStateMegDlg(CWnd* pParent = NULL);   // 标准构造函数
	LedStateMegDlg(int nFlag, CWnd* pParent);
	virtual ~LedStateMegDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int m_nMegFlag = 0;
	virtual BOOL OnInitDialog();
	CStatic m_staticStateMeg;
};
