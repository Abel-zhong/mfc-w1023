#pragma once
#include "afxwin.h"


// LedStateMegDlg �Ի���

class LedStateMegDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LedStateMegDlg)

public:
	LedStateMegDlg(CWnd* pParent = NULL);   // ��׼���캯��
	LedStateMegDlg(int nFlag, CWnd* pParent);
	virtual ~LedStateMegDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int m_nMegFlag = 0;
	virtual BOOL OnInitDialog();
	CStatic m_staticStateMeg;
};
