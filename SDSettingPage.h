#pragma once
#include "afxwin.h"
#include "W-1023����Dlg.h"

// CSDSettingPage �Ի���

class CSDSettingPage : public CDialogEx
{
	DECLARE_DYNAMIC(CSDSettingPage)

public:
	CSDSettingPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSDSettingPage();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// SD������С����
	CEdit m_editSDReadMin;
	// SD��д��С����
	CEdit m_editSDWriteMin;

	CW1023����Dlg *m_ccdParent;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
