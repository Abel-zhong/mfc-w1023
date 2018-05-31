#pragma once
#include "afxwin.h"
#include "W-1023厂测Dlg.h"

// CLogConfigPage 对话框

class CLogConfigPage : public CDialogEx
{
	DECLARE_DYNAMIC(CLogConfigPage)

public:
	CLogConfigPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogConfigPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGCONF_PAGE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoglocChoseButton();
	CEdit m_editLogFilePath;
	CW1023厂测Dlg *m_ccdParent;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

};
