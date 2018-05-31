#pragma once
#include "afxwin.h"
#include "W-1023厂测Dlg.h"

// CSDSettingPage 对话框

class CSDSettingPage : public CDialogEx
{
	DECLARE_DYNAMIC(CSDSettingPage)

public:
	CSDSettingPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSDSettingPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// SD卡读最小速率
	CEdit m_editSDReadMin;
	// SD卡写最小速率
	CEdit m_editSDWriteMin;

	CW1023厂测Dlg *m_ccdParent;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
