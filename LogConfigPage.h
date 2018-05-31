#pragma once
#include "afxwin.h"
#include "W-1023����Dlg.h"

// CLogConfigPage �Ի���

class CLogConfigPage : public CDialogEx
{
	DECLARE_DYNAMIC(CLogConfigPage)

public:
	CLogConfigPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogConfigPage();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGCONF_PAGE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoglocChoseButton();
	CEdit m_editLogFilePath;
	CW1023����Dlg *m_ccdParent;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

};
