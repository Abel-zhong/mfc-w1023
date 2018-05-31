
// W-1023����Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "W-1023����.h"
#include "W-1023����Dlg.h"
#include "afxdialogex.h"
#include "CmdShell.h"
#include "json.h"
//#include "KillProcess.h"
#include "tlhelp32.h"
#include <atlstr.h>
#include "SFCWebServiceSoap.nsmap"
#include "soapSFCWebServiceSoapProxy.h"
#include <dbt.h>
#include <winioctl.h> 
#include "LedStateMegDlg.h"
#include "LoginDlg.h"
#include "SDSettingPage.h"
#include "LogConfigPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const char *SWEEP_STATION = _T("SWEEP");
const char *FUNC_STATION = _T("FUNCTION TEST");
const char *PACK_STATION = _T("E/A-TEST");
const char *BURN_STATION = _T("BurnTest");
//const int INT_MBPS_MIN = 15;
const CString RESULT_PASS = _T("PASS");
const char *FACTORY_LOC = _T("/mnt/SDCARD/factory");
//const char *FACTORY_LOC = _T("factory");

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


TestResult::TestResult()
{
}

void TestResult::SetStation(CString &strStation)
{
	m_strStation = strStation;
}

void TestResult::SetSN(CString &strSN)
{
	m_strSN = strSN;
}

void TestResult::SetPlay(CString &strPlay)
{
	m_strPlay = strPlay;
}

void TestResult::SetRecord(CString &strRecord)
{
	m_strRecord = strRecord;
}

void TestResult::SetKey(CString &strKey)
{
	m_strKey = strKey;
}

void TestResult::SetLed(CString &strLed)
{
	m_strLed = strLed;
}

void TestResult::SetWifi(CString &strWifi)
{
	m_strWifi = strWifi;
}

void TestResult::SetCard(CString &strCard)
{
	m_strCard = strCard;
}

void TestResult::SetPower(CString &strPower)
{
	m_strPower = strPower;
}

void TestResult::SetTotal(CString &strTotal)
{
	m_strTotal = strTotal;
}


void TestResult::GetStation(CString &strStation)
{
	strStation = m_strStation;
}

void TestResult::GetSN(CString &strSN)
{
	strSN = m_strSN;
}

void TestResult::GetPlay(CString &strPlay)
{
	strPlay = m_strPlay;
}

void TestResult::GetRecord(CString &strRecord)
{
	strRecord = m_strRecord;
}

void TestResult::GetKey(CString &strKey)
{
	strKey = m_strKey;
}

void TestResult::GetLed(CString &strLed)
{
	strLed = m_strLed;
}

void TestResult::GetWifi(CString &strWifi)
{
	strWifi = m_strWifi;
}

void TestResult::GetCard(CString &strCard)
{
	strCard = m_strCard;
}

void TestResult::GetPower(CString &strPower)
{
	strPower = m_strPower;
}

void TestResult::GetTotal(CString &strTotal)
{
	strTotal = m_strTotal;
}

void TestResult::Empty()
{
	m_strStation.Empty();
	m_strSN.Empty();
	m_strPlay.Empty();
	m_strRecord.Empty();
	m_strKey.Empty();
	m_strLed.Empty();
	m_strWifi.Empty();
	m_strCard.Empty();
	m_strPower.Empty();
	m_strTotal.Empty();
}

void TestResult::ReleaseBuffer()
{
	m_strStation.ReleaseBuffer();
	m_strSN.ReleaseBuffer();
	m_strPlay.ReleaseBuffer();
	m_strRecord.ReleaseBuffer();
	m_strKey.ReleaseBuffer();
	m_strLed.ReleaseBuffer();
	m_strWifi.ReleaseBuffer();
	m_strCard.ReleaseBuffer();
	m_strPower.ReleaseBuffer();
	m_strTotal.ReleaseBuffer();
}


UINT CW1023����Dlg::MainThreadProc(LPVOID lpParam)
{
	CW1023����Dlg *pMainDlg;
	CString strSsid;
	CString strPasswd;

	pMainDlg = (CW1023����Dlg *)lpParam;
	//pMainDlg->m_bMainThreadLive = TRUE;

	/*�����ʼ���Խ��������¼���*/
	while (1)
	{
		WaitForSingleObject(pMainDlg->m_ceSNCheckEvent.m_hObject, INFINITE);
		pMainDlg->m_ceSNCheckEvent.ResetEvent();

		if (pMainDlg->m_checkWifiTest.GetCheck() == BST_CHECKED) {
			pMainDlg->m_editSsid.GetWindowText(strSsid);
			if (strSsid.IsEmpty()) {
				pMainDlg->m_editTestState.SetWindowText(_T("SSID����Ϊ��"));
				continue;
			}
		}
		pMainDlg->m_strResult.Empty();

		pMainDlg->GetDlgItem(IDC_START_TEST_BUTTON)->EnableWindow(FALSE);
		pMainDlg->m_cmMenu.EnableMenuItem(ID_SD_ARGUMENT_SETTING, TRUE);
		pMainDlg->m_cmMenu.EnableMenuItem(ID_LOG_SETTING, TRUE);
		pMainDlg->m_cmMenu.EnableMenuItem(ID_SN_PASS_SETTING, TRUE);
		pMainDlg->m_cmMenu.EnableMenuItem(ID_LOGIN, TRUE);

		pMainDlg->MainTestLoop();

		pMainDlg->GetDlgItem(IDC_START_TEST_BUTTON)->EnableWindow(TRUE);

		pMainDlg->TestEndReset();

		pMainDlg->GotoDlgCtrl(pMainDlg->GetDlgItem(IDC_SN_EDIT));
	}

	return 0;
}


UINT CW1023����Dlg::SNCheckThreadProc(LPVOID lpParam)
{
	CW1023����Dlg *pMainDlg;
	CString strSn;

	pMainDlg = (CW1023����Dlg *)lpParam;

	while (1)
	{
		WaitForSingleObject(pMainDlg->m_ceStartEvent.m_hObject, INFINITE);
		pMainDlg->m_ceStartEvent.ResetEvent();
		pMainDlg->ResetMainTestDlg();/*���ǰ�β��������Ϣ*/
		pMainDlg->m_editSN.GetWindowText(strSn);
		if (strSn.IsEmpty()) {
			AfxMessageBox(_T("������S/N"));
			continue;
		}
		else {
			/*����SNɨ���¼����Դ�Ϊ���Կ�ʼ�ı�־*/
			pMainDlg->m_ceSNCheckEvent.SetEvent();
		}

		Sleep(10);
	}

	return 0;
}


// CW1023����Dlg �Ի���
CW1023����Dlg::CW1023����Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_W1023_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bUserRoot = FALSE;
	m_bPassEATest = FALSE;
}


void CW1023����Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TESTITEM_COMBO, m_comboTestItem);
	DDX_Control(pDX, IDC_RESULT_EDIT, m_editTestResult);
	DDX_Control(pDX, IDC_TEST_STATE_EDIT, m_editTestState);
	DDX_Control(pDX, IDC_SSID_EDIT, m_editSsid);
	DDX_Control(pDX, IDC_PASSWD_EDIT, m_editPasswd);
	DDX_Control(pDX, IDC_SN_EDIT, m_editSN);
	DDX_Control(pDX, IDC_WIFI_TEST_EDIT, m_editWifiTestResult);
	DDX_Control(pDX, IDC_LED_TEST_EDIT, m_editLedTestResult);
	DDX_Control(pDX, IDC_KEY_TEST_EDIT, m_editKeyTestResult);
	DDX_Control(pDX, IDC_PLAY_TEST_EDIT, m_editPlayTestResult);
	DDX_Control(pDX, IDC_RECORD_TEST_EDIT, m_editRecordTestResult);
	DDX_Control(pDX, IDC_POWER_TEST_EDIT, m_editPowerTestResult);
	DDX_Control(pDX, IDC_SOFTWARE_VERSION_EDIT, m_editSoftwareVersion);
	DDX_Control(pDX, IDC_RSS_MIN_EDIT, m_editRssiMin);
	DDX_Control(pDX, IDC_RSS_MAX_EDIT, m_editRssiMax);
	DDX_Control(pDX, IDC_RSS_REAL_EDIT, m_editRssiReal);
	DDX_Control(pDX, IDC_MBPS_MIN_EDIT, m_editMbpsMin);
	DDX_Control(pDX, IDC_MBPS_MAX_EDIT, m_editMbpsMax);
	DDX_Control(pDX, IDC_MBPS_REAL_EDIT, m_editMbpsReal);
	DDX_Control(pDX, IDC_KEY1_EDIT, m_editKey1State);
	DDX_Control(pDX, IDC_KEY2_EDIT, m_editKey2State);
	DDX_Control(pDX, IDC_KEY3_EDIT, m_editKey3State);
	DDX_Control(pDX, IDC_KEY4_EDIT, m_editKey4State);
	DDX_Control(pDX, IDC_KEY5_EDIT, m_editKey5State);
	DDX_Control(pDX, IDC_KEY6_EDIT, m_editKey6State);
	DDX_Control(pDX, IDC_LED_BLUE_EDIT, m_editLedBlue);
	DDX_Control(pDX, IDC_LED_GREEN_EDIT, m_editLedGreen);
	DDX_Control(pDX, IDC_LED_RED_EDIT, m_editLedRed);
	DDX_Control(pDX, IDC_START_TEST_BUTTON, m_buttonStartTest);
	DDX_Control(pDX, IDC_KEY_FAIL_BUTTON, m_buttonKeyTest);
	DDX_Control(pDX, IDC_UPLOAD_MES_CHECK, m_checkUploadMes);
	DDX_Control(pDX, IDC_WIFI_TEST_CHECK, m_checkWifiTest);
	DDX_Control(pDX, IDC_KEY_TEST_CHECK, m_checkKeyTest);
	DDX_Control(pDX, IDC_PLAY_TEST_CHECK, m_checkPlayTest);
	DDX_Control(pDX, IDC_LED_TEST_CHECK, m_checkLedTest);
	DDX_Control(pDX, IDC_RECORD_TEST_CHECK, m_checkRecordTest);
	DDX_Control(pDX, IDC_POWER_TEST_CHECK, m_checkPowerTest);
	DDX_Control(pDX, IDC_MBPS_MIN_EDIT2, m_editMbpsMin2);
	DDX_Control(pDX, IDC_MBPS_MAX_EDIT2, m_editMbpsMax2);
	DDX_Control(pDX, IDC_MBPS_REAL_EDIT2, m_editMbpsReal2);
	DDX_Control(pDX, IDC_SD_TEST_CHECK, m_checkSdCardTest);
	DDX_Control(pDX, IDC_SD_TEST_EDIT, m_editSDTestResult);
	DDX_Control(pDX, IDC_SD_READ_EDIT, m_editSdReadVal);
	DDX_Control(pDX, IDC_SD_WRITE_EDIT, m_editSdWriteVal);
	DDX_Control(pDX, IDC_POWER_TARGET_EDIT, m_editPowerTarget);
	DDX_Control(pDX, IDC_POWER_VICE_EDIT, m_editPowerVari);
	DDX_Control(pDX, IDC_CURR_POWER_EDIT, m_editCurrPower);
	DDX_Control(pDX, IDC_FILE_NAME_EDIT, m_editFileName);
	DDX_Control(pDX, IDC_FILE_PUSH_CHECK, m_buttonPushFile);
	DDX_Control(pDX, IDC_SWEEP_SUC_BUTTON, m_buttonSweepPass);
	DDX_Control(pDX, IDC_SWEEP_FAIL_BUTTON, m_buttonSweepFail);
	DDX_Control(pDX, IDC_FILE_CHOSE_BUTTON, m_buttonFileChoose);
	DDX_Control(pDX, IDC_KEY_RESET_EDIT, m_editResetKeyState);
	DDX_Control(pDX, IDC_KNOB_SUCC_BUTTON, m_buttonKnobSucc);
	DDX_Control(pDX, IDC_KNOB_FAIL_BUTTON, m_buttonKnobFail);
}

BEGIN_MESSAGE_MAP(CW1023����Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_TESTITEM_COMBO, &CW1023����Dlg::OnCbnSelchangeTestitemCombo)
	ON_BN_CLICKED(IDCANCEL, &CW1023����Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_START_TEST_BUTTON, &CW1023����Dlg::OnBnClickedStartTestButton)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_KEY_FAIL_BUTTON, &CW1023����Dlg::OnBnClickedKeyFailButton)
	ON_BN_CLICKED(IDC_SWEEP_SUC_BUTTON, &CW1023����Dlg::OnBnClickedSweepSucButton)
	ON_BN_CLICKED(IDC_SWEEP_FAIL_BUTTON, &CW1023����Dlg::OnBnClickedSweepFailButton)
	ON_COMMAND(ID_LOGIN, &CW1023����Dlg::OnLogin)
	ON_BN_CLICKED(IDC_FILE_CHOSE_BUTTON, &CW1023����Dlg::OnBnClickedFileChoseButton)
	ON_COMMAND(ID_SD_ARGUMENT_SETTING, &CW1023����Dlg::OnSdArgumentSetting)
	ON_COMMAND(ID_LOG_SETTING, &CW1023����Dlg::OnLogSetting)
	ON_COMMAND(ID_SN_PASS_SETTING, &CW1023����Dlg::OnSnPassSetting)
	ON_CBN_DROPDOWN(IDC_TESTITEM_COMBO, &CW1023����Dlg::OnCbnDropdownTestitemCombo)
	ON_BN_CLICKED(IDC_KNOB_SUCC_BUTTON, &CW1023����Dlg::OnBnClickedKnobSuccButton)
	ON_BN_CLICKED(IDC_KNOB_FAIL_BUTTON, &CW1023����Dlg::OnBnClickedKnobFailButton)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


BOOL CW1023����Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetFocus()->GetDlgCtrlID() == IDC_SN_EDIT)//���»س��������ǰ���������Լ������Ŀؼ���
		{
			// ���������£�������»س�ʱ����������Ҫ�Ŀؼ���
			OnBnClickedStartTestButton();
		}
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}



// CW1023����Dlg ��Ϣ�������

BOOL CW1023����Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	m_cmMenu.LoadMenuA(IDR_MENU1);
	SetMenu(&m_cmMenu);

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	/*1. ��Ӳ���ѡ���ѡ��*/
	m_comboTestItem.AddString(_T("ɨƵ����"));
	m_comboTestItem.AddString(_T("���ܲ���"));
	m_comboTestItem.AddString(_T("��������"));

	/*2. ����ѡ��Ĭ��ѡ��ɨƵ����*/
	GetTestItemConf();
	int nSel = m_comboTestItem.GetCurSel();
	m_comboTestItem.EnableWindow(TRUE);
	ChooseTestItem(nSel);

	/*****�ı���Խ��������****/
	CFont *ptf = m_editTestResult.GetFont();
	LOGFONT lf;
	ptf->GetLogFont(&lf);
	lf.lfHeight = 50;
	lf.lfWidth = 20;
	lf.lfOrientation = 30;
	lf.lfItalic = TRUE;
	lf.lfWeight = FW_BOLD;
	strcpy_s(lf.lfFaceName, _T("����"));
	m_fontFont.CreateFontIndirectA(&lf);
	m_editTestResult.SetFont(&m_fontFont);

	/*��ʼ����ʽˢ*/
	m_redcolor = RGB(250, 137, 107);
	m_greencolor = RGB(116, 241, 122);
	m_yellowcolor = RGB(255, 204, 0);
	m_bluecolor = RGB(51, 204, 204);
	m_textcolor = RGB(0, 0, 0);
	m_greenbrush.CreateSolidBrush(m_greencolor);
	m_redbrush.CreateSolidBrush(m_redcolor);
	m_yellowbrush.CreateSolidBrush(m_yellowcolor);
	m_bluebrush.CreateSolidBrush(m_bluecolor);

	ButtonInit();

	/*��ʼ�����̶��������û�ģʽ����ֹ����*/
	OtherUserConf();

	/*3. ���ó�ʼ�Ĳ���״̬�Ͳ��Խ��*/
	ResetMainTestDlg();
	SetTotalTestResult(2);
	/*5. SN�����߳�*/
	m_pctSNCheckThread = AfxBeginThread(
		SNCheckThreadProc, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (NULL == m_pctSNCheckThread) {
		AfxMessageBox(_T("����SN Check�߳�ʧ��"), MB_OK);
		return FALSE;
	}
	m_pctSNCheckThread->ResumeThread();

#if 1
	/*4. �������߳�*/
	m_pctMainThread = AfxBeginThread(
		MainThreadProc, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (NULL == m_pctMainThread) {
		AfxMessageBox(_T("�����������߳�ʧ��"), MB_OK);
		return FALSE;
	}
	m_pctMainThread->ResumeThread();
#endif

	m_pctIprefThread = AfxBeginThread(
		iperfStartProc, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (NULL == m_pctIprefThread) {
		AfxMessageBox(_T("����Ipref�߳�ʧ��"), MB_OK);
		return FALSE;
	}
	m_pctIprefThread->ResumeThread();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CW1023����Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CW1023����Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CW1023����Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CW1023����Dlg::OnCbnSelchangeTestitemCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel;
	CString strTestItem;

	/*��ȡ����ѡ����Ͽ�ؼ����б���е�ѡ��������*/
	nSel = m_comboTestItem.GetCurSel();
	/*��ȡ��ǰѡ��Ĳ������ַ�������*/
	//m_comboTestItem.GetLBText(nSel, strTestItem);
	/*���õ�ǰѡ���¿��Ա�ѡ��Ĳ���*/
	ChooseTestItem(nSel);
	//m_ceStartEvent.ResetEvent();
}


void CW1023����Dlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel;
	nSel = m_comboTestItem.GetCurSel();
	SaveConfig(nSel);

	CDialogEx::OnCancel();
}


void CW1023����Dlg::OnBnClickedStartTestButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strSsid;

	TestStartRest();

	if (m_checkWifiTest.GetCheck() == BST_CHECKED) {
		m_editSsid.GetWindowText(strSsid);
		if (strSsid.IsEmpty()) {
			m_editTestState.SetWindowText(_T("SSID����Ϊ��"));
			AfxMessageBox(_T("������SSID������"));
			TestEndReset();
			return;
		}
	}
	m_ceStartEvent.SetEvent();

	return;
}


void CW1023����Dlg::OnBnClickedKeyFailButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nKeyTestFailFlag = TRUE;
}


void CW1023����Dlg::OnBnClickedSweepSucButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bSweepFlag = TRUE;
	m_nLiveFlag = FALSE;
	MusicStop();
	//m_ceSweepEvent.SetEvent();
}


void CW1023����Dlg::OnBnClickedSweepFailButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nLiveFlag = FALSE;
	m_bSweepFlag = FALSE;
	MusicStop();
	//m_ceSweepEvent.SetEvent();
}


void CW1023����Dlg::OnBnClickedFileChoseButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;

	TCHAR szFilter[] = _T("wav�ļ�(*.wav)|*.wav|�����ļ�(*.*)|*.*||");
	//CFileDialog fileDlg(FALSE, _T("wav"), _T("my"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CFileDialog fileDlg(TRUE, _T("wav"), NULL, 0, szFilter, this);

	if (IDOK == fileDlg.DoModal()) {
		strFilePath = fileDlg.GetPathName();
		m_editFileName.SetWindowTextA(strFilePath);
		m_buttonPushFile.SetCheck(TRUE);
	}
}

/**************************Start����ɫ������****************************/
HBRUSH CW1023����Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	CString strTemp;
	CString strStdMax;
	CString strStdMin;
	
	// TODO:  �ڴ˸��� DC ���κ�����
	switch (pWnd->GetDlgCtrlID())//��ĳһ���ض��ؼ������ж�
	{
	case IDC_RESULT_EDIT:
	case IDC_LED_BLUE_EDIT:
	case IDC_LED_RED_EDIT:
	case IDC_LED_GREEN_EDIT:
	case IDC_WIFI_TEST_EDIT:
	case IDC_LED_TEST_EDIT:
	case IDC_KEY_TEST_EDIT:
	case IDC_PLAY_TEST_EDIT:
	case IDC_RECORD_TEST_EDIT:
	case IDC_POWER_TEST_EDIT:
	case IDC_KEY1_EDIT:
	case IDC_KEY2_EDIT:
	case IDC_KEY3_EDIT:
	case IDC_KEY4_EDIT:
	case IDC_KEY5_EDIT:
	case IDC_KEY6_EDIT:
	case IDC_SD_TEST_EDIT:
	case IDC_KEY_RESET_EDIT:
		GetDlgItemText(pWnd->GetDlgCtrlID(), strTemp);
		if (0 == strTemp.CompareNoCase(_T("FAIL")))
		{
			pDC->SetBkColor(m_redcolor);		// ���ñ���ɫ
												//pDC->SetTextColor(m_textcolor);	// �����ı���ɫ
			pDC->SetBkMode(TRANSPARENT);		// ���ñ���ģʽΪ͸����Ҳ����ȥ�����ֵı���		
			hbr = (HBRUSH)m_redbrush;			// �û�ˢ��������
		}
		else if (0 == strTemp.CompareNoCase(_T("PASS")))
		{
			pDC->SetBkColor(m_greencolor);
			//pDC->SetTextColor(m_textcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_greenbrush;
		}
		else if (0 == strTemp.CompareNoCase(_T("...")))
		{
			pDC->SetBkColor(m_bluecolor);
			//pDC->SetTextColor(m_textcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_bluebrush;
		}
		else {
			pDC->SetBkColor(m_yellowcolor);
			//pDC->SetTextColor(m_textcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_yellowbrush;
		}
		break;

	case IDC_SOFTWARE_VERSION_EDIT:
		GetDlgItemText(pWnd->GetDlgCtrlID(), strTemp);
		if (0 == strTemp.CompareNoCase(_T("N/A"))) {
			pDC->SetBkColor(m_yellowcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_yellowbrush;
		}
		else if (0 == strTemp.CompareNoCase(_T("...")))
		{
			pDC->SetBkColor(m_bluecolor);
			//pDC->SetTextColor(m_textcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_bluebrush;
		}
		else if (0 != strTemp.CompareNoCase(_T(""))) {
			pDC->SetBkColor(m_greencolor);
			//pDC->SetTextColor(m_textcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_greenbrush;
		}
		else {
			pDC->SetBkColor(m_redcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_redbrush;
		}
		break;
	
	case IDC_RSS_REAL_EDIT:
		GetDlgItemText(pWnd->GetDlgCtrlID(), strTemp);
		m_editRssiMin.GetWindowText(strStdMin);
		m_editRssiMax.GetWindowText(strStdMax);
		if (0 == strTemp.CompareNoCase(_T("N/A"))) {
			pDC->SetBkColor(m_yellowcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_yellowbrush;
		}
		else if (0 == strTemp.CompareNoCase(_T("...")))
		{
			pDC->SetBkColor(m_bluecolor);
			//pDC->SetTextColor(m_textcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_bluebrush;
		}
		else {
			if(!fIsInStd(strStdMax, strStdMin, strTemp)){
				pDC->SetBkColor(m_redcolor);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_redbrush;
			}
			else {
				pDC->SetBkColor(m_greencolor);
				//pDC->SetTextColor(m_textcolor);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_greenbrush;
			}
		}
		break;

	case IDC_MBPS_REAL_EDIT:
		GetDlgItemText(pWnd->GetDlgCtrlID(), strTemp);
		m_editMbpsMin.GetWindowText(strStdMin);
		//m_editMbpsMax.GetWindowText(strStdMax);
		if (0 == strTemp.CompareNoCase(_T("N/A"))) {
			pDC->SetBkColor(m_yellowcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_yellowbrush;
		}
		else if (0 == strTemp.CompareNoCase(_T("...")))
		{
			pDC->SetBkColor(m_bluecolor);
			//pDC->SetTextColor(m_textcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_bluebrush;
		}
		else {
			if (FALSE == MbpsTestJudge(strStdMin, strTemp)) {
				pDC->SetBkColor(m_redcolor);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_redbrush;
			}
			else {
				pDC->SetBkColor(m_greencolor);
				//pDC->SetTextColor(m_textcolor);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_greenbrush;
			}
		}
		break;

	case IDC_MBPS_REAL_EDIT2:
		GetDlgItemText(pWnd->GetDlgCtrlID(), strTemp);
		m_editMbpsMin2.GetWindowText(strStdMin);
		if (0 == strTemp.CompareNoCase(_T("N/A"))) {
			pDC->SetBkColor(m_yellowcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_yellowbrush;
		}
		else if (0 == strTemp.CompareNoCase(_T("...")))
		{
			pDC->SetBkColor(m_bluecolor);
			//pDC->SetTextColor(m_textcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_bluebrush;
		}
		else {

			if (FALSE == MbpsTestJudge(strStdMin, strTemp))  {
				pDC->SetBkColor(m_redcolor);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_redbrush;
			}
			else {
				pDC->SetBkColor(m_greencolor);
				//pDC->SetTextColor(m_textcolor);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_greenbrush;
			}
		}
		break;
	case IDC_SD_READ_EDIT:
		GetDlgItemText(pWnd->GetDlgCtrlID(), strTemp);
		if (0 == strTemp.CompareNoCase(_T("N/A"))) {
			pDC->SetBkColor(m_yellowcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_yellowbrush;
		}
		else if (0 == strTemp.CompareNoCase(_T("...")))
		{
			pDC->SetBkColor(m_bluecolor);
			//pDC->SetTextColor(m_textcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_bluebrush;
		}
		else {
			//f = TTOF(strTemp);
			//if (f < (float)(0.2)) {
			if (fIsLessThanStd(m_strReadStdVal, strTemp) < 0) {/*С�ڱ�׼ֵ*/
				pDC->SetBkColor(m_redcolor);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_redbrush;
			}
			else {
				pDC->SetBkColor(m_greencolor);
				//pDC->SetTextColor(m_textcolor);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_greenbrush;
			}
		}
		break;
	case IDC_SD_WRITE_EDIT:
		GetDlgItemText(pWnd->GetDlgCtrlID(), strTemp);
		if (0 == strTemp.CompareNoCase(_T("N/A"))) {
			pDC->SetBkColor(m_yellowcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_yellowbrush;
		}
		else if (0 == strTemp.CompareNoCase(_T("...")))
		{
			pDC->SetBkColor(m_bluecolor);
			//pDC->SetTextColor(m_textcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_bluebrush;
		}
		else {
			//f = TTOF(strTemp);
			//if (f < (float)(0.2)) {
			if(fIsLessThanStd(m_strWriteStdVal, strTemp) < 0){/*С�ڱ�׼ֵ*/
				pDC->SetBkColor(m_redcolor);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_redbrush;
			}
			else {
				pDC->SetBkColor(m_greencolor);
				//pDC->SetTextColor(m_textcolor);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_greenbrush;
			}
		}
		break;
	case IDC_CURR_POWER_EDIT:
		GetDlgItemText(pWnd->GetDlgCtrlID(), strTemp);
		m_editPowerTarget.GetWindowText(strStdMax);
		m_editPowerVari.GetWindowText(strStdMin);
		if (0 == strTemp.CompareNoCase(_T("N/A"))) {
			pDC->SetBkColor(m_yellowcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_yellowbrush;
		}
		else if (0 == strTemp.CompareNoCase(_T("...")))
		{
			pDC->SetBkColor(m_bluecolor);
			//pDC->SetTextColor(m_textcolor);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_bluebrush;
		}
		else {
			//f = TTOF(strTemp);
			//if ((f < (float)(60))|| (f > (float)(80))) {
			if(!fIsInStd(strStdMax, strStdMin, strTemp)){
				pDC->SetBkColor(m_redcolor);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_redbrush;
			}
			else {
				pDC->SetBkColor(m_greencolor);
				//pDC->SetTextColor(m_textcolor);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)m_greenbrush;
			}
		}
		break;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
/**************************End����ɫ������****************************/


/***********************Start:��ȡ�ļ�����************************/
BOOL CW1023����Dlg::PathName2Name(CString &strPcPathName, CString &strName)
{
	int nPos1;

	if (TRUE == strPcPathName.IsEmpty()) {
		return FALSE;
	}

	nPos1 = strPcPathName.ReverseFind('\\');
	if (nPos1 != -1) {
		strName = strPcPathName.Right(strPcPathName.GetLength() - nPos1 - 1);
		//AfxMessageBox(strName);
	}
	else {
		strName = strPcPathName;
		return TRUE;
	}

	return TRUE;
}


BOOL CW1023����Dlg::GetFileName(CString &strPathName, CString &strName)
{
	if (m_buttonPushFile.GetCheck() == BST_CHECKED) {
		m_editFileName.GetWindowText(strPathName);
		if (strPathName.IsEmpty()) {
			return FALSE;
		}
		//AfxMessageBox(strPathName);
		if (FALSE == PathName2Name(strPathName, strName)) {
			return FALSE;
		}
	
	}
	else {
		strPathName = _T("");
		return FALSE;
	}
	return TRUE;
}
/***********************End:��ȡ�ļ�����************************/


/*************Start:����״̬����ϵ��***************/
//�������京�壺nSel--ѡ��Ĳ���ģʽ���   bFlag--��ʼ��ʱ���ñ�־����ʼ��״̬�»��������б���ѡ��Ϊѡ��״̬
void CW1023����Dlg::ChooseTestItem(int nSel, BOOL bFlag)
{
	switch (nSel) {
	case 0://ɨƵ����
		m_checkKeyTest.EnableWindow(FALSE);
		m_checkLedTest.EnableWindow(FALSE);
		m_checkWifiTest.EnableWindow(FALSE);
		m_checkPlayTest.EnableWindow(TRUE);
		m_checkRecordTest.EnableWindow(FALSE);
		m_checkPowerTest.EnableWindow(FALSE);
		m_checkSdCardTest.EnableWindow(FALSE);
		m_buttonPushFile.EnableWindow(TRUE);
		m_editFileName.SetReadOnly(FALSE);
		m_editSsid.SetReadOnly(TRUE);
		m_editPasswd.SetReadOnly(TRUE);
		m_buttonFileChoose.EnableWindow(TRUE);

		m_editPowerTarget.SetReadOnly(TRUE);
		m_editPowerVari.SetReadOnly(TRUE);
		m_editMbpsMax.SetReadOnly(TRUE);
		m_editMbpsMin.SetReadOnly(TRUE);
		m_editMbpsMax2.SetReadOnly(TRUE);
		m_editMbpsMin2.SetReadOnly(TRUE);
		m_editRssiMax.SetReadOnly(TRUE);
		m_editRssiMin.SetReadOnly(TRUE);
		break;
	case 1://���ܲ���
		m_checkKeyTest.EnableWindow(TRUE);
		m_checkLedTest.EnableWindow(TRUE);
		m_checkWifiTest.EnableWindow(TRUE);
		m_checkSdCardTest.EnableWindow(TRUE);
		m_checkPowerTest.EnableWindow(FALSE);
		m_checkPlayTest.EnableWindow(TRUE);
		m_checkRecordTest.EnableWindow(FALSE);
		m_buttonPushFile.EnableWindow(TRUE);
		m_editFileName.SetReadOnly(FALSE);
		m_editSsid.SetReadOnly(FALSE);
		m_editPasswd.SetReadOnly(FALSE);
		m_buttonFileChoose.EnableWindow(TRUE);

		/*���Բ�������*/
		m_editMbpsMax.SetReadOnly(FALSE);
		m_editMbpsMin.SetReadOnly(FALSE);
		m_editMbpsMax2.SetReadOnly(FALSE);
		m_editMbpsMin2.SetReadOnly(FALSE);
		m_editRssiMax.SetReadOnly(FALSE);
		m_editRssiMin.SetReadOnly(FALSE);
		m_editPowerTarget.SetReadOnly(TRUE);
		m_editPowerVari.SetReadOnly(TRUE);
		break;
	case 2://��������
		m_checkKeyTest.EnableWindow(FALSE);
		m_checkLedTest.EnableWindow(FALSE);
		m_checkWifiTest.EnableWindow(FALSE);
		m_checkPlayTest.EnableWindow(FALSE);
		m_checkRecordTest.EnableWindow(FALSE);
		m_checkPowerTest.EnableWindow(TRUE);
		m_checkSdCardTest.EnableWindow(FALSE);
		m_buttonPushFile.EnableWindow(FALSE);
		m_editFileName.SetReadOnly(TRUE);
		m_editSsid.SetReadOnly(TRUE);
		m_editPasswd.SetReadOnly(TRUE);
		m_buttonFileChoose.EnableWindow(FALSE);

		/*���Բ�������*/
		m_editPowerTarget.SetReadOnly(FALSE);
		m_editPowerVari.SetReadOnly(FALSE);
		m_editMbpsMax.SetReadOnly(TRUE);
		m_editMbpsMin.SetReadOnly(TRUE);
		m_editMbpsMax2.SetReadOnly(TRUE);
		m_editMbpsMin2.SetReadOnly(TRUE);
		m_editRssiMax.SetReadOnly(TRUE);
		m_editRssiMin.SetReadOnly(TRUE);
		break;
	}
	if(FALSE == bFlag)
		LoadConfig(nSel);
}


void CW1023����Dlg::SetSubLedResult(CString &strRet, int nStation)
{
	switch (nStation)
	{
	case LED_RED_TEST:
		m_editLedRed.SetWindowText(strRet);
		break;
	case LED_GREEN_TEST:
		m_editLedGreen.SetWindowText(strRet);
		break;
	case LED_BLUE_TEST:
		m_editLedBlue.SetWindowText(strRet);
		break;
	default:
		AfxMessageBox(_T("LED״̬��ʾ����"), MB_OK);
		break;
	}
}


void CW1023����Dlg::SetLedTestResult(BOOL nResult, int nStation, int nNum)
{
	CString strRetInfo;
	CString strStateInfo;

	/*��ʾLED��������״̬*/
	strRetInfo.Format(_T("%s"), (nResult ? _T("PASS") : _T("FAIL")));
	SetSubLedResult(strRetInfo, nStation);

	if ((LED_BLUE_TEST == nStation) || (!nResult)) {
		if (nResult) {
			strStateInfo.Format(_T("LED����ͨ����"));
			/*��ʾ������ѡ�����Ĳ���״̬*/
			m_editLedTestResult.SetWindowText(strRetInfo);
			/*����״̬��ʾ����ʾ*/
			m_editTestState.SetWindowText(strStateInfo);
		}
		else {
			strStateInfo.Format(_T("LED[%d]����ʧ�ܣ�"), nNum);
			/*��ʾ������ѡ�����Ĳ���״̬*/
			m_editLedTestResult.SetWindowText(strRetInfo);
			/*����״̬��ʾ����ʾ*/
			m_editTestState.SetWindowText(strStateInfo);
		}
	}
}


void CW1023����Dlg::SetSubKeyResult(CString &strRet, int nStation)
{
	switch (nStation) {
	case KEY1_TEST:
		m_editKey1State.SetWindowText(strRet);
		break;
	case KEY2_TEST:
		m_editKey2State.SetWindowText(strRet);
		break;
	case KEY3_TEST:
		m_editKey3State.SetWindowText(strRet);
		break;
	case KEY4_TEST:
		m_editKey4State.SetWindowText(strRet);
		break;
	case KEY5_TEST:
		m_editKey5State.SetWindowText(strRet);
		break;
	case KEY6_TEST:
		m_editKey6State.SetWindowText(strRet);
		break;
	default:
		AfxMessageBox(_T("���������ʾ����"), MB_OK);
		break;
	}
}


void CW1023����Dlg::SetKeyTestResult(BOOL nResult, int nStation)
{
	CString strRetInfo;
	CString strStateInfo;

	strRetInfo.Format(_T("%s"), (nResult ? _T("PASS") : _T("FAIL")));

	SetSubKeyResult(strRetInfo, nStation);

	if ((KEY6_TEST/*KEY5_TEST*/ == nStation) || (!nResult)) {
		if (nResult) {
			strStateInfo.Format(_T("��������ͨ����"));
			/*��ʾ������ѡ�����Ĳ���״̬*/
			//m_editKeyTestResult.SetWindowText(strRetInfo);
			/*����״̬��ʾ����ʾ*/
			m_editTestState.SetWindowText(strStateInfo);
		}
		else {
			strStateInfo.Format(_T("����[%d]����ʧ�ܣ�"), nStation);
			/*����״̬��ʾ����ʾ*/
			m_editTestState.SetWindowText(strStateInfo);
		}
	}

}


void CW1023����Dlg::SetRssiResult(int nVal)
{
	CString strVal;

	strVal.Format(_T("%d"), nVal);
	m_editRssiReal.SetWindowText(strVal);
}


void CW1023����Dlg::SetMbpsResult(int nVal)
{
	CString strVal;

	strVal.Format(_T("%d"), nVal);
	m_editMbpsReal.SetWindowText(strVal);
}


void CW1023����Dlg::SetWifiTestResult(BOOL nResult)
{
	CString strRet;
	CString strState;

	strRet.Format(_T("%s"), nResult ? _T("PASS") : _T("FAIL"));
	strState.Format(_T("%s"), nResult ? _T("Wifi����ʧ��") : _T("Wifi���Գɹ�"));
	m_editWifiTestResult.SetWindowText(strRet);
	m_editTestState.SetWindowText(strState);
}


void CW1023����Dlg::SetPlayTestResult(BOOL nResult)
{
	CString strRet;
	CString strState;

	strRet.Format(_T("%s"), nResult ? _T("PASS") : _T("FAIL"));
	strState.Format(_T("%s"), nResult ? _T("���Ų���ʧ��") : _T("���Ų��Գɹ�"));
	m_editPlayTestResult.SetWindowText(strRet);
	m_editTestState.SetWindowText(strState);
}


void CW1023����Dlg::SetRecordTestResult(BOOL nResult)
{
	CString strRet;
	CString strState;

	strRet.Format(_T("%s"), nResult ? _T("PASS") : _T("FAIL"));
	strState.Format(_T("%s"), nResult ? _T("¼������ʧ��") : _T("¼�����Գɹ�"));
	m_editRecordTestResult.SetWindowText(strRet);
	m_editTestState.SetWindowText(strState);
}


void CW1023����Dlg::SetPowerTestResult(BOOL nResult)
{
	CString strRet;
	CString strState;

	strRet.Format(_T("%s"), nResult ? _T("PASS") : _T("FAIL"));
	strState.Format(_T("%s"), nResult ? _T("�������Գɹ�") : _T("�������Գɹ�"));
	m_editPowerTestResult.SetWindowText(strRet);
	m_editTestState.SetWindowText(strState);
}


void CW1023����Dlg::SetTotalTestResult(int nResult)
{

	if (0 == nResult) {/*���Գɹ�*/
		m_editTestResult.SetWindowText(_T("PASS"));
	}
	else if(1 == nResult){
		m_editTestResult.SetWindowText(_T("FAIL"));
	}
	else{
		m_editTestResult.SetWindowText(_T("N/A"));
	}
}


/*д����Խ����Log�ļ���*/
int CW1023����Dlg::LoadTestResultToLog(CString *info)
{
	CFile cfFile;
	LPCTSTR s = info->GetBuffer(info->GetLength());
	//BOOL bFlag;

	if(!cfFile.Open(m_strLogPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)){
		AfxMessageBox(_T("���ܴ��ļ���W-1023����������Լ�¼.txt��"));
		return -1;
	}

	//if (FALSE == bFlag) {/*ȷ���ļ���д��ĸ�ʽ��Unicode��ʽ�����Ĳ�����*/
	//	WORD unicode = 0xFEFF; 
	//	cfFile.Write(&unicode, 2);  
	//}

	cfFile.SeekToEnd();
	cfFile.Write(_T("\r\n"), sizeof(_T("\r\n")));
	cfFile.Write(s, info->GetLength()*sizeof(TCHAR));
	cfFile.Close();

	return 0;
}


/*�������������Ϣ*/
void CW1023����Dlg::ResetMainTestDlg()
{
	m_editTestState.SetWindowText(_T(""));
	m_editTestResult.SetWindowText(_T("N/A"));
	m_editWifiTestResult.SetWindowText(_T("N/A"));
	m_editLedTestResult.SetWindowText(_T("N/A"));
	m_editKeyTestResult.SetWindowText(_T("N/A"));
	m_editPlayTestResult.SetWindowText(_T("N/A"));
	m_editRecordTestResult.SetWindowText(_T("N/A"));
	m_editPowerTestResult.SetWindowText(_T("N/A"));
	m_editSoftwareVersion.SetWindowText(_T("N/A"));
	//m_editRssiMin.SetWindowText(_T("-85 dBm"));
	//m_editRssiMax.SetWindowText(_T("0 dBm"));
	m_editRssiReal.SetWindowText(_T("N/A"));
	//m_editMbpsMin.SetWindowText(_T("15 Mbit"));
	//m_editMbpsMax.SetWindowText(_T("20 Mbit"));
	//m_editMbpsMin2.SetWindowText(_T("15 Mbit"));
	//m_editMbpsMax2.SetWindowText(_T("20 Mbit"));
	m_editMbpsReal.SetWindowText(_T("N/A"));
	m_editMbpsReal2.SetWindowText(_T("N/A"));
	m_editKey1State.SetWindowText(_T("N/A"));
	m_editKey2State.SetWindowText(_T("N/A"));
	m_editKey3State.SetWindowText(_T("N/A"));
	m_editKey4State.SetWindowText(_T("N/A"));
	m_editKey5State.SetWindowText(_T("N/A"));
	m_editKey6State.SetWindowText(_T("N/A"));
	m_editResetKeyState.SetWindowText(_T("N/A"));
	m_editLedBlue.SetWindowText(_T("N/A"));
	m_editLedRed.SetWindowText(_T("N/A"));
	m_editLedGreen.SetWindowText(_T("N/A"));
	m_editSDTestResult.SetWindowText(_T("N/A"));
	m_editSdReadVal.SetWindowText(_T("N/A"));
	m_editSdWriteVal.SetWindowText(_T("N/A"));
	m_editCurrPower.SetWindowText(_T("N/A"));
	//m_editPowerTarget.SetWindowText(_T("70"));
	//m_editPowerVari.SetWindowText(_T("10"));
}


void CW1023����Dlg::TestEndReset()
{
	int nSel;
	m_editPasswd.SetReadOnly(FALSE);
	m_editSsid.SetReadOnly(FALSE);
	m_checkUploadMes.EnableWindow(TRUE);
	nSel = m_comboTestItem.GetCurSel();
	ChooseTestItem(nSel, TRUE);
	m_nLiveFlag = FALSE;
	m_comboTestItem.EnableWindow(TRUE);
	if (TRUE == m_bUserRoot) { 
		m_cmMenu.EnableMenuItem(ID_SD_ARGUMENT_SETTING, FALSE);
		m_cmMenu.EnableMenuItem(ID_LOG_SETTING, FALSE);
		m_cmMenu.EnableMenuItem(ID_SN_PASS_SETTING, FALSE);
	}
	m_cmMenu.EnableMenuItem(ID_LOGIN, FALSE);

	ButtonInit();

	m_ceStartEvent.ResetEvent();
}


void CW1023����Dlg::TestStartRest()
{
	m_nLiveFlag = TRUE;
	m_nKeyTestFailFlag = FALSE;
	m_editPasswd.SetReadOnly(TRUE);
	m_editSsid.SetReadOnly(TRUE);
	m_editFileName.SetReadOnly(TRUE);
	m_checkUploadMes.EnableWindow(FALSE);
	m_comboTestItem.EnableWindow(FALSE);
	m_checkKeyTest.EnableWindow(FALSE);
	m_checkLedTest.EnableWindow(FALSE);
	m_checkWifiTest.EnableWindow(FALSE);
	m_checkPowerTest.EnableWindow(FALSE);
	m_checkPlayTest.EnableWindow(FALSE);
	m_checkRecordTest.EnableWindow(FALSE);
	m_checkSdCardTest.EnableWindow(FALSE);

}


void CW1023����Dlg::UpdataTestInfo(CString &info)
{
	/*д����Խ����LOG��*/
	LoadTestResultToLog(&info);
	/*д����Խ�������Խ����״̬��*/
	//AppendTestStateInfo(info);
}
/*************End:����״̬����ϵ��***************/



/*******************************Start:����ָ���ϵ��*********************************/
BOOL CW1023����Dlg::iAdbExist(BOOL bFlag)
{
	CString strTemp;
	CAdbshell casShell;

#if 0
	if (TRUE == m_bAdbShellLive) {
		if (bFlag == TRUE)
			AfxMessageBox(_T("adbʵ�����������У�"), MB_OK);
		return -1;
	}
	m_bAdbShellLive = TRUE;
#endif

	if (FALSE == casShell.Start())
	//if (FALSE == m_casShell.Start())
	{
		//adb shellִ�д��󣬷���
		if (bFlag == TRUE)
			AfxMessageBox(_T("adb��������ʧ�ܣ��ڴ治�㣡"), MB_OK);
		//m_bAdbShellLive = FALSE;
		return FALSE;
	}
	casShell.Stop();
	strTemp = casShell.GetOutput();
	//m_casShell.Stop();
	//strTemp = m_casShell.GetOutput();
	strTemp.ReleaseBuffer();

	//m_bAdbShellLive = FALSE;

	if (0 == strTemp.Compare(_T("error: no devices/emulators found"))) {
		if (bFlag == TRUE)
			AfxMessageBox(_T("adb��������USB���ӣ�"), MB_OK);
		return FALSE;
	}

	return TRUE;
}

/*Adb shell����ص�ֵ��int��*/
int CW1023����Dlg::iExecuteAdbCmd(CString &strCmd, BOOL bFlag)
{
	CString strTemp;
	CAdbshell casShell;

#if 0
	if (TRUE == m_bAdbShellLive) {
		if (bFlag == TRUE)
			AfxMessageBox(_T("adbʵ�����������У�"), MB_OK);
		return -1;
	}
	m_bAdbShellLive = TRUE;
#endif
	
	if (FALSE == casShell.Start())
	//if (FALSE == m_casShell.Start())
	{
		//adb shellִ�д��󣬷���
		if(bFlag == TRUE)
			AfxMessageBox(_T("adb��������ʧ�ܣ��ڴ治�㣡"), MB_OK);	
		//m_bAdbShellLive = FALSE;
		return -1;
	}

	casShell.RunCmd(strCmd);
	casShell.Stop();
	strTemp = casShell.GetOutput();
	//m_casShell.RunCmd(strCmd);
	//m_casShell.Stop();
	//strTemp = m_casShell.GetOutput();
	strTemp.ReleaseBuffer();
	//AfxMessageBox(strTemp, MB_OK);
	//m_bAdbShellLive = FALSE;

	/*����û�з���ֵ����������ص��ַ��ʹ����������һ����*/
	if (0 == strTemp.Compare("")) {
		return -1;
	}
	else if (0 == strTemp.Compare(_T("error: no devices/emulators found"))) {
		if (bFlag == TRUE)
			AfxMessageBox(_T("adb��������USB���ӣ�"), MB_OK);
		return 1;
	}
	else {
		return 0;
	}
}


int CW1023����Dlg::iExecuteAdbCmd(CString &strCmd, CString &strRet, BOOL bFlag)
{
	//CString strTemp;
	//CString strRet;
	CAdbshell casShell;

	/*���buff����������*/
	strRet.Empty();
	
	//if (FALSE == m_casShell.Start())
	if (FALSE == casShell.Start())
	{
		//adb shellִ�д��󣬷���
		if (bFlag == TRUE)
			AfxMessageBox(_T("adb��������ʧ�ܣ��ڴ治�㣡"), MB_OK);
		//m_bAdbShellLive = FALSE;
		return -1;
	}

	casShell.RunCmd(strCmd);
	casShell.Stop();
	strRet = casShell.GetOutput();

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);
	

	if (0 == strRet.Compare("")) {
		//AfxMessageBox(_T("adb���ؽ��Ϊ��"));
		return -1;
	}
	else if (0 == strRet.Compare(_T("error: no devices/emulators found"))) {
		if (bFlag == TRUE)
			AfxMessageBox(_T("adb��������USB���ӣ�"), MB_OK);
		strRet = _T("");
		return 1;
	}
	else {
		return 0;
	}
}


int CW1023����Dlg::iExecuteShellCmd(CString &strCmd, CString &strRet, BOOL bFlag)
{
	//CString strTemp;
	CCmdshell cmdShell;

	/*���buff����������*/
	strRet.Empty();
	if (FALSE == cmdShell.Start(strCmd)) {
		if(TRUE == bFlag)
			AfxMessageBox(_T("���̿�ʼʧ�ܣ�"), MB_OK);
		return -1;
	}

	strRet = cmdShell.GetOutput();
	strRet.ReleaseBuffer();

	//������
	//AfxMessageBox(strRet, MB_OK);

	return 0;
}


int CW1023����Dlg::iExecuteShellCmdWithTimeout(CString &strCmd, int nTimeout, BOOL bFlag)
{
	//CString strTemp;
	CCmdshell cmdShell;


	if (FALSE == cmdShell.StartWithoutTimeout(strCmd, nTimeout)) {
		if (TRUE == bFlag)
			AfxMessageBox(_T("���̿�ʼʧ�ܣ�"), MB_OK);
		return -1;
	}

	return 0;
}



int CW1023����Dlg::FactoryStop()
{
	CString strCmd;
	CString strRet;
	int nRet;

	strCmd.Format(_T("%s ATE_EXIT_FACTORY\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	//strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);
	return 0;
}


int CW1023����Dlg::SDReadTest()
{
	CString strCmd;
	CString strRet;
	int nRet;

	strCmd.Format(_T("%s ATE_SD_RD_SPEED_TEST\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	val = _T(jsValue["codeMsg"].asCString());

	m_editSdReadVal.SetWindowText(val);

	//float f = TTOF(val);
	//if (f < (float)(0.2)) {
	if (fIsLessThanStd(m_strReadStdVal, val) < 0) {
		return -1;
	}

	return 0;
}


int CW1023����Dlg::SDWriteTest()
{
	CString strCmd;
	CString strRet;
	CString strStdMax, strStdMin;
	int nRet;

	strCmd.Format(_T("%s ATE_SD_WR_SPEED_TEST\n"), FACTORY_LOC);
	strCmd.ReleaseBuffer();
	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	val = _T(jsValue["codeMsg"].asCString());

	m_editSdWriteVal.SetWindowText(val);


	//float f = TTOF(val);
	//if (f < (float)(0.2)) {
	if (fIsLessThanStd(m_strWriteStdVal, val) < 0) {
		return -1;
	}

	return 0;
}


int CW1023����Dlg::WifiConnnect()
{
	CString strCmd;
	CString strRet;
	CString strSsid;
	CString strPasswd;
	int nRet;


	/*��ȡSSID��Passwd*/
	m_editSsid.GetWindowText(strSsid);
	m_editPasswd.GetWindowText(strPasswd);

	strCmd.Format(_T("%s ATE_WIFI_CONNECT %s %s\n"), FACTORY_LOC, strSsid, strPasswd);
	
	nRet = iExecuteAdbCmd(strCmd, strRet);
	if(nRet < 0){
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}
	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}
	
	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))){
		return -1;
	}

	m_strIp.Empty();
	m_strIp = _T(jsValue["codeMsg"].asCString());
	
	return 0;
}


int CW1023����Dlg::WifiDisconnect()
{
	CString strCmd;
	CString strRet;
	int nRet;
	//CString strSuccInfo(_T("wifi�Ͽ����ӳɹ���"));
	//CString strErrInfo(_T("wifi�Ͽ�����ʧ�ܣ�"));

	strCmd.Format(_T("%s ATE_WIFI_DISCONNECT\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	return 0;
}


//����ֵ���京�壺  1--ADB�����أ�   0--�������أ�   -1--����ʧ��
int CW1023����Dlg::MusicPlay(CString &filePath)
{
	CString strCmd;
	CString strRet;
	int nRet;

	strCmd.Format(_T("%s ATE_PLAY_FILE %s\n"), FACTORY_LOC, filePath);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		//AfxMessageBox(_T("adb����ִ��ʧ��"));
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		AfxMessageBox(_T("adb�������ʧ��"));
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	return 0;
}


int CW1023����Dlg::MusicStop()
{
	CString strCmd;
	CString strRet;
	int nRet;

	strCmd.Format(_T("%s ATE_PLAY_STOP\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}
	return 0;
}


int CW1023����Dlg::MicRecordStart(CString &filePath)
{
	CString strCmd;
	CString strRet;
	int nRet;
	//CString strSuccInfo(_T("¼�����Կ�ʼ�ɹ���"));
	//CString strErrInfo(_T("¼�����Կ�ʼʧ�ܣ�"));

	strCmd.Format(_T("%s ATE_RECORD_START %s\n"), FACTORY_LOC, filePath);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	return 0;
}


int CW1023����Dlg::MicRecordStop()
{
	CString strCmd;
	CString strRet;
	int nRet;
	//CString strSuccInfo(_T("¼������ֹͣ�ɹ���"));
	//CString strErrInfo(_T("¼������ֹͣʧ�ܣ�"));

	strCmd.Format(_T("%s ATE_RECORD_STOP\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	return 0;
}


int CW1023����Dlg::RememberTestInfo(CString &strStation, BOOL bFlag)
{
	int nRet;
	CString strCmd;
	CString strRet;

	strCmd.Format(_T("%s ATE_SET_STA_RESULT %s %d\n"), FACTORY_LOC, strStation, (bFlag ? 0 : 1));

	nRet = iExecuteAdbCmd(strCmd);
	if (nRet != 0) {
		return FALSE;
	}
	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return FALSE;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return FALSE;
	}

	return TRUE;
}


int CW1023����Dlg::DisplayTestInfo(CString &strStation)
{
	int nRet;
	CString strCmd;
	CString strRet;

	//strCmd.Format(_T("factory ATE_GET_STA_RESULT %s\n"), strStation);
	strCmd.Format(_T("%s ATE_GET_SELF_TEST_RESULT\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet != 0) {
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return 1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	//val = _T(jsValue["codeMsg"].asCString());
	//if (0 != val.Compare(_T("0"))) {
	//	return -1;
	//}

	return 0;
}


//���볧��ģʽ
int CW1023����Dlg::EnterFactory()
{
	int nRet;
	CString strCmd;
	CString strRet;

	strCmd.Format(_T("%s ATE_ENTER_FACTORY\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet != 0) {
		return -1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	return 0;
}


/*�ָ���������*/
int CW1023����Dlg::OutFactory()
{
	int nRet;
	CString strRet;
	CString strCmd;

	strCmd.Format(_T("%s ATE_ENTER_USER_MODE\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	return 0;
}


int CW1023����Dlg::FactoryTestSuccess()
{
	int nRet;
	CString strRet;
	CString strCmd;
	CString val, strCode;

	strCmd.Format(_T("%s ATE_SET_FACTORY_SUCCESS\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}
	
	strCode = _T(jsValue["code"].asCString());

	val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	return 0;
}


#if 0
/*�ָ���������*/
int CW1023����Dlg::FactorySet()
{
	int nRet;
	CString strRet;
	CString strCmd;
	
	strCmd.Format(_T("%s ATE_ENTER_FACTORY\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	return 0;
}
#endif

int CW1023����Dlg::GetCurPower(CString &strPower)
{
	int nRet;
	CString strRet;
	CString strCmd;

	strCmd.Format(_T("%s ATE_GET_POWER\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	strPower = _T(jsValue["codeMsg"].asCString());

	//m_editCurrPower.SetWindowText(strBuf);

	return 0;
}


int CW1023����Dlg::PushFile(CString &strPath)
{
	int nRet;
	int nPos;
	CString strCmd;
	CString strRet;

	strCmd.Format(_T("adb push \"%s\" /mnt/SDCARD"), strPath);

	//AfxMessageBox(strCmd);
	CString strMsg;

	strMsg.Format(_T("�ļ�-%s-�����ڣ������ļ���"), strPath);
	
	if (FALSE == PathFileExists(strPath)) {
		AfxMessageBox(_T(strMsg));
		return -1;
	}

	nRet = iExecuteShellCmd(strCmd, strRet);
	if (nRet) {
		return -1;
	}

	nPos = strRet.Find(_T("fail"));
	if (nPos != -1) {
		return -1;
	}
	nPos = strRet.Find(_T("error: device not found"));
	if (nPos != -1) {
		AfxMessageBox(_T("δ����ADB�豸"));
		return -1;
	}

	return 0;
}


int CW1023����Dlg::PushFile(CString &strPath, CString &strDest)
{
	int nRet;
	int nPos;
	CString strCmd;
	CString strRet;

	strCmd.Format(_T("adb push \"%s\" %s"), strPath, strDest);

	//AfxMessageBox(strCmd);
	CString strMsg;

	strMsg.Format(_T("�ļ�-%s-�����ڣ������ļ���"), strPath);

	if (FALSE == PathFileExists(strPath)) {
		AfxMessageBox(_T(strMsg));
		return -1;
	}

	nRet = iExecuteShellCmd(strCmd, strRet);
	if (nRet) {
		return -1;
	}

	nPos = strRet.Find(_T("fail"));
	if (nPos != -1) {
		return -1;
	}
	nPos = strRet.Find(_T("error: device not found"));
	if (nPos != -1) {
		AfxMessageBox(_T("δ����ADB�豸"));
		return -1;
	}

	return 0;
}


int CW1023����Dlg::PullFile(CString &strPath)
{
	CString strCmd;
	CString strRet;
	int nPos;

	strCmd.Format(_T("adb pull %s ."), strPath);
	/*δ���*/
	iExecuteShellCmd(strCmd, strRet);

	nPos = strRet.Find(_T("fail"));
	if (nPos != -1) {
		return -1;
	}
	nPos = strRet.Find(_T("error: device not found"));
	if (nPos != -1) {
		AfxMessageBox(_T("δ����ADB�豸"));
		return -1;
	}

	return 0;
}


int CW1023����Dlg::ChmodFactory()
{
	CString strCmd;
	CString strRet;
	int nRet;

	strCmd.Format(_T("chmod ugo+x %s\n"),  FACTORY_LOC);
	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return -1;
	}

	//strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	return 0;
}

int CW1023����Dlg::GetDutIp(CString &strBuf)
{
	int nRet;

	CString strRet;
	CString strCmd;

	strCmd.Format(_T("%s ATE_WIFI_IP\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	strBuf = _T(jsValue["codeMsg"].asCString());

	return 0;
}


int CW1023����Dlg::DutConnectionCheck()
{
	CString strCmd;

	strCmd.Format(_T("%s ATE_DUT_PING\n"), FACTORY_LOC);

	return iExecuteAdbCmd(strCmd);
}


int CW1023����Dlg::IprefRecvTest()
{
	int nRet;
	CString strRet;
	CString strCmd;
	CString strStd;

	//strCmd.Format(_T("%s ATE_RX_COMMAND -s -p 8888 -i 5 -1\n"), FACTORY_LOC);
	strCmd.Format(_T("%s ATE_RX_COMMAND -R -c %s -p 8888 -i 5 -t 15\n"), FACTORY_LOC, m_strHostIp);

	m_editMbpsReal.SetWindowText(_T("..."));

#if 0
	CWinThread *pThread;

	pThread = AfxBeginThread(
		iprefRecvProc, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (NULL == pThread) {
		AfxMessageBox(_T("����Ipref�����߳�ʧ��"), MB_OK);
		return -1;
	}
	pThread->ResumeThread();
#endif

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}
	
	/*��ⷵ��ֵ*/
	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	val = _T(jsValue["codeMsg"].asCString());
	m_editMbpsReal.SetWindowText(val);

	m_editMbpsMin2.GetWindowText(strStd);
	if (FALSE == MbpsTestJudge(strStd, val)) {
		return -1;
	}

	return 0;
}


UINT CW1023����Dlg::iprefRecvProc(LPVOID lpParam)
{
	CString strPcCmd;
	CString strRet;
	CW1023����Dlg *p;
	int nRet;
	int nPos;

	p = (CW1023����Dlg *)lpParam;
	Sleep(5000);
	strPcCmd.Format(_T("iperf3 -c %s -t 15 -p 8888 -i 5"), p->m_strIp);
	//strPcCmd.Format(_T("iperf3 -c 192.168.123.123 -t 15 -p 8888 -i 5"));
	nRet = p->iExecuteShellCmd(strPcCmd, strRet);
	if (nRet != 0) {
		AfxMessageBox(_T("����ipref�ͷ���ʧ��"), MB_OK);
	}
	//AfxMessageBox(strRet);

	strRet.ReleaseBuffer();
	nPos = strRet.Find(_T("iperf3: error"));
	if (-1 != nPos) {
		return -1;/*����*/
	}

	return 0;
}


int CW1023����Dlg::IprefSendTest(int nPort)
{
	int nRet;
	CString strRet;
	CString strCmd;
	CString strStd;
	CString strVal;


	m_editMbpsReal2.SetWindowText(_T("..."));

#if 0
	CWinThread *pThread;

	pThread = AfxBeginThread(
		iprefSendProc, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (NULL == pThread) {
		AfxMessageBox(_T("����Ipref�����߳�ʧ��"), MB_OK);
		return -1;
	}
	pThread->ResumeThread();
	Sleep(2000);
#endif

	strCmd.Format(_T("%s ATE_TX_COMMAND -c %s -p %d -i 5 -t 15\n"), FACTORY_LOC, m_strHostIp, nPort);
	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return 1;
	}

	/*��ⷵ��ֵ*/
	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	strVal = _T(jsValue["result"].asCString());
	if (0 != strVal.Compare(_T("OK"))) {
		return -1;
	}

	strVal = _T(jsValue["codeMsg"].asCString());
	m_editMbpsReal2.SetWindowText(strVal);
	m_editMbpsMin2.GetWindowText(strStd);

	if (FALSE == MbpsTestJudge(strStd, strVal)) {
		return -1;
	}

	return 0;
}


UINT CW1023����Dlg::iprefSendProc(LPVOID lpParam)
{
	int nRet;
	CW1023����Dlg *p;
	CString strRet;
	CString strPcCmd(_T("iperf3 -s -p 8888 -i 5 -1"));
	//CString strPcCmd(_T("iperf3 -s -p 8888 -i 5"));

	p = (CW1023����Dlg *)lpParam;

	nRet = p->iExecuteShellCmd(strPcCmd, strRet);
	if (nRet != 0) {
		AfxMessageBox(_T("����ipref�����ʧ��"), MB_OK);
	}

	//AfxMessageBox(strRet);
	return 0;
}


int CW1023����Dlg::LedTestSubBlue()
{
	int nRet;
	CString strRet;
	CString strCmd;

	strCmd.Format(_T("%s ATE_LED_ALL_BLUE\n"), FACTORY_LOC);
	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return -1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	return 0;
}


int CW1023����Dlg::LedTestSubGreen()
{
	int nRet;
	CString strRet;
	CString strCmd;

	strCmd.Format(_T("%s ATE_LED_ALL_GREEN\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return -1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}
	return 0;
}


int CW1023����Dlg::LedTestSubRed()
{
	int nRet;
	CString strRet;
	CString strCmd;

	strCmd.Format(_T("%s ATE_LED_ALL_RED\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return -1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	return 0;
}


int CW1023����Dlg::SubKeyTest()
{
	int nRet;
	CString strRet;
	CString strCmd;

	strCmd.Format(_T("%s ATE_GET_KEY_PRESS\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet, FALSE);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return -1;
	}

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	/*��ⰴ�������Ƿ�OK*/
	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	val = _T(jsValue["codeMsg"].asCString());
	//AfxMessageBox(val);
	if (0 == val.Compare(_T("4"))) {
		return 1;
	}
	else if (0 == val.Compare(_T("7"))) {
		return 2;
	}
	else if (0 == val.Compare(_T("13"))) {
		return 3;
	}
	else if (0 == val.Compare(_T("6"))) {
		return 4;
	}
	else if (0 == val.Compare(_T("5"))) {
		return 5;
	}
	else {
		return -1;
	}

}


int CW1023����Dlg::NightKeyTest()
{
	int nRet;
	CString strRet;
	CString strCmd;

	strCmd.Format(_T("%s ATE_GET_NIGHT_KEY_PRESS\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet, FALSE);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return -1;
	}

	//AfxMessageBox(strRet);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	/*��ⰴ�������Ƿ�OK*/
	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	val = _T(jsValue["codeMsg"].asCString());
	if (0 != val.Compare(_T("RESULT_NIGHT_KEY_IS_PRESS"))) {
		return -1;
	}

	return 0;
}


int CW1023����Dlg::ResetKeyTest()
{
	int nRet;
	CString strRet;
	CString strCmd;

	strCmd.Format(_T("%s ATE_GET_RESET_KEY_EVENT\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		AfxMessageBox(_T("ADB��������USB���ӣ�"), MB_OK);
		return -1;
	}

	//AfxMessageBox(strRet);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	/*��ⰴ�������Ƿ�OK*/
	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	val = _T(jsValue["codeMsg"].asCString());
	if (0 != val.Compare(_T("RESULT_NIGHT_KEY_IS_PRESS"))) {
		return -1;
	}

	return 0;
}


int CW1023����Dlg::WifiRssiTest()
{
	int nRet;
	CString strRet;
	CString strCmd;
	CString strStdMax, strStdMin;

	strCmd.Format(_T("%s ATE_WIFI_GET_SIGNAL_STRENGTH\n"), FACTORY_LOC);

	m_editRssiReal.SetWindowText(_T("..."));

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		return -1;
	}

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	/*��ⰴ�������Ƿ�OK*/
	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	val = _T(jsValue["codeMsg"].asCString());
	if (0 == val.Compare(_T("RESULT_FILE_NOT_EXIST"))) {
		m_editRssiReal.SetWindowText(_T(""));
		AfxMessageBox(_T("����WIFIǿ�������SD��"));
		return -1;
	}

	m_editRssiReal.SetWindowText(val);
	m_editRssiMin.GetWindowText(strStdMin);
	m_editRssiMax.GetWindowText(strStdMax);

	if(FALSE == fIsInStd(strStdMax, strStdMin, val)){
		return -1;
	}

	return 0;
}


int CW1023����Dlg::GetFirmwareVer()
{
	int nRet;
	CString strRet;
	CString strCmd;

	strCmd.Format(_T("%s ATE_GET_SOFTWARE_VERSION\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		return -1;
	}

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	/*��ⰴ�������Ƿ�OK*/
	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	val = _T(jsValue["codeMsg"].asCString());
	m_editSoftwareVersion.SetWindowText(val);

	return 0;
}


void CW1023����Dlg::PowerDischarge()
{
	CString strCmd;

	CString strPcCmd;

	strPcCmd.Format(_T("%s ATE_DISCHARGE"), FACTORY_LOC);
	
	/*��Ҫִ��PC��ָ��*/
	iExecuteAdbCmd(strPcCmd);

	return;
}


int CW1023����Dlg::AdbSampleTest()
{
	CString strCmd;
	CString strRet;
	CString strSuccInfo(_T("AdbSampleTestִ�гɹ���"));
	CString strErrInfo(_T("AdbSampleTestִ��ʧ�ܣ�"));

	//strCmd.Format(_T("cd bin"));
	CString strPcCmd(_T("iperf3 -c IP -t 10 -p 35000"));
	//CString strPcCmd(_T("iperf3 -s -t 10 -p 35000"));
	/*��Ҫִ��PC��ָ��*/
	iExecuteShellCmd(strPcCmd, strRet);
	//return iExecuteAdbCmd(strCmd, strSuccInfo, strErrInfo);
	return 0;
}

/*******************************End:����ָ���ϵ��*********************************/



/***************************Start:�������***************************/
//����ֵ��������   1:����ֹ���ԣ� 0:���в�����ɣ������˳��� -1:���Գ��� -2���쳣�˳�
int CW1023����Dlg::LedTest()
{
	int nRet;
	BOOL nRetu = FALSE;
	LedStateMegDlg msg1(1, this);
	LedStateMegDlg msg2(2, this);
	LedStateMegDlg msg3(3, this);
	INT_PTR nResponse;

	m_editLedTestResult.SetWindowText(_T("..."));

	m_editLedRed.SetWindowText(_T("..."));
	nRet = LedTestSubRed();
	if (0 != nRet) {/*����ʧ��*/
		m_editLedRed.SetWindowText(_T("FAIL"));
		m_editTestState.SetWindowText(_T("LED��Ʋ���ʧ��"));
		goto fail;
	}
		
	nResponse = msg1.DoModal();
	if (IDOK != nResponse) {
		m_editLedRed.SetWindowText(_T("FAIL"));
		m_editTestState.SetWindowText(_T("LED��Ʋ���ʧ��"));
		goto fail;
	}
	m_editLedRed.SetWindowText(_T("PASS"));
		
	m_editLedGreen.SetWindowText(_T("..."));
	nRet = LedTestSubGreen();
	if (0 != nRet) {/*����ʧ��*/
		m_editLedGreen.SetWindowText(_T("FAIL"));
		m_editTestState.SetWindowText(_T("LED�̵Ʋ���ʧ��"));
		goto fail;
	}
	nResponse = msg2.DoModal();
	if (IDOK != nResponse) {
		m_editLedGreen.SetWindowText(_T("FAIL"));
		m_editTestState.SetWindowText(_T("LED�̵Ʋ���ʧ��"));
		goto fail;
	}
	m_editLedGreen.SetWindowText(_T("PASS"));
		
	m_editLedBlue.SetWindowText(_T("..."));
	nRet = LedTestSubBlue();
	if (0 != nRet) {/*����ʧ��*/
		m_editLedBlue.SetWindowText(_T("FAIL"));
		m_editTestState.SetWindowText(_T("LED���Ʋ���ʧ��"));
		goto fail;
	}
	nResponse = msg3.DoModal();
	if (IDOK != nResponse) {
		m_editLedBlue.SetWindowText(_T("FAIL"));
		m_editTestState.SetWindowText(_T("LED���Ʋ���ʧ��"));
		goto fail;
	}
	m_editLedBlue.SetWindowText(_T("PASS"));

	m_editLedTestResult.SetWindowText(_T("PASS"));
	return 0;
fail:
	m_editLedTestResult.SetWindowText(_T("FAIL"));
	return -1;
}


void CW1023����Dlg::KeyPrepareTestDisplay()
{
	switch (m_nStation) {
	case KEY_TEST:
		m_editKeyTestResult.SetWindowText(_T("..."));
		break;
	case KEY1_TEST:
		m_editKey1State.SetWindowText(_T("..."));
		break;
	case KEY2_TEST:
		m_editKey2State.SetWindowText(_T("..."));
		break;
	case KEY3_TEST:
		m_editKey3State.SetWindowText(_T("..."));
		break;
	case KEY4_TEST:
		m_editKey4State.SetWindowText(_T("..."));
		break;
	case KEY5_TEST:
		m_editKey5State.SetWindowText(_T("..."));
		break;
	case KEY6_TEST:
		m_editKey6State.SetWindowText(_T("..."));
		break;
	default:
		AfxMessageBox(_T("����׼��������ʾ����ʧ�ܣ�"));
		break;
	}
}


//����ֵ��������   1:����ֹ���ԣ� 0:���в�����ɣ����������� -1:���в�����ɣ�����ʧ�ܣ�
int CW1023����Dlg::KeyTest()
{
	int nRet;
	//int nStation;
	CString msg;
	m_nStation = KEY_TEST;

	KeyPrepareTestDisplay();
	m_editTestState.SetWindowText(_T("���ڽ��а�������..."));
	AfxMessageBox(_T("�����ΰ�������Ϣ��������һ\n��������������������һ����������\n��������ҹ�ơ�,��Reset��������\nĳһ���������Զ����ͨ\n�����밴'�������ܲ���'��"));
	m_nKeyTestFailFlag = FALSE;
	m_buttonKeyTest.EnableWindow(TRUE);
	/*���USB�����Ƿ�OK����߲�����ʾ*/
	//iAdbExist();

	while (m_nLiveFlag && (m_nKeyTestFailFlag == FALSE))
	{
		++m_nStation;
		KeyPrepareTestDisplay();
		nRet = SubKeyTest();
		msg.Format(_T("%d"), nRet);
		//AfxMessageBox(msg);
		if ((-1 == nRet) || (0 == nRet)) {/*ʧ�ܲ����κζ���ֱ���ظ�*/
			SetKeyTestResult(FALSE, m_nStation);
			goto fail;
		}
		if (nRet != (m_nStation - KEY_TEST)) {/*��ȡ��ֵ����*/
			--m_nStation;
			if (m_nKeyTestFailFlag == FALSE) {
				AfxMessageBox(_T("���������ԣ����ذ�"));
			}
			continue;
		}
		else {
			//nStation = m_nStation;
			SetKeyTestResult(TRUE, m_nStation);
			if (m_nStation == /*KEY6_TEST*/KEY5_TEST) {
				break;
			}
		}
	}

	//AfxMessageBox(_T("���Ա�־1"));

	++m_nStation;
	if (TRUE == m_nKeyTestFailFlag) {
		SetKeyTestResult(FALSE, m_nStation);
		goto fail;
	}


	KeyPrepareTestDisplay();
	//++m_nStation;
	nRet = NightKeyTest();
	if (nRet != 0) {
		SetKeyTestResult(FALSE, m_nStation);
		goto fail;
	}
	SetKeyTestResult(TRUE, m_nStation);

	m_editResetKeyState.SetWindowText(_T("..."));
	nRet = ResetKeyTest();
	if (nRet != 0) {
		m_editResetKeyState.SetWindowText(_T("FAIL"));
		goto fail;
	}
	m_editResetKeyState.SetWindowText(_T("PASS"));


	m_editKeyTestResult.SetWindowText(_T("PASS"));
	m_editTestState.SetWindowText(_T("�������Գɹ�..."));
	m_buttonKeyTest.EnableWindow(FALSE);
	return 0;
fail:
	m_editKeyTestResult.SetWindowText(_T("FAIL"));
	m_editTestState.SetWindowText(_T("��������ʧ��..."));
	m_buttonKeyTest.EnableWindow(FALSE);
	return -1;
}


int CW1023����Dlg::WifiTest()
{
	int nRet;
	int nCycel;
	CString strIp;
	m_nStation = WIFI_TEST;

	/*���m_strIp�е�����*/
	m_strIp.Empty();

	/*wifi���Խ����ʾ������*/
	m_editWifiTestResult.SetWindowText(_T("..."));
	m_editTestState.SetWindowText(_T("������������..."));

	/*1. ����wifi*/
	for (nCycel = 0; nCycel < 3; nCycel++) {
		nRet = WifiConnnect();
		if (nRet != 0) {/*wifi����ʧ��*/
			continue;
		}
		else {
			break;
		}
	}
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("WIFI����ʧ��..."));
		m_editWifiTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}
	else {
		m_editTestState.SetWindowText(_T("�������ӳɹ�..."));
	}
#if 0
	//Sleep(1500);/*��ʱ��ֹ��ȡIPʧ��*/
	/*��ȡ�豸IP*/
	m_editTestState.SetWindowText(_T("���ڻ�ȡIP..."));
	nRet = GetDutIp(m_strIp);
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("��ȡ�豸IPʧ��..."));
		m_editWifiTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}
#endif
	/*4.��ȡPC����IP*/
	CString strHostName;
	if (FALSE == GetHostIPaddr(strHostName)) {
		m_editTestState.SetWindowText(_T("��ȡ����IPʧ��..."));
		AfxMessageBox(_T("��ȡ����IPʧ��,�����������"));
		return -1;
	}

	/*2. ����wifi��ǿ��*/
	//m_editTestState.SetWindowText(m_strIp);
	m_editTestState.SetWindowText(_T("���ڲ���Wifiǿ��..."));
	nRet = WifiRssiTest();
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("����WIFIǿ��ʧ��..."));
		m_editWifiTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}

	/*3. ����wifi��������*/
	m_editTestState.SetWindowText(_T("���ڲ���Wifi������..."));

	nRet = IprefRecvTest();
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("����WIFI������-����-ʧ��..."));
		m_editWifiTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}

	nRet = IprefSendTest(8888);
	if (nRet == 0) {
		goto sendPass;
	}

	nRet = IprefSendTest(1234);
	if (nRet == 0) {
		goto sendPass;
	}

	nRet = IprefSendTest(4321);
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("����WIFI������-����-ʧ��..."));
		m_editWifiTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}

sendPass:
	m_editWifiTestResult.SetWindowText(_T("PASS"));
	return 0;

}


int CW1023����Dlg::PowerTest()
{
	int nRet;
	CString strPower;
	CString strStdMax, strStdMin;
	BOOL bFlag = FALSE;  

	m_editCurrPower.SetWindowText(_T("..."));

	nRet = GetCurPower(strPower);
	if (nRet != 0) {
		goto fail;/*����ʧ��*/
	}

	m_editCurrPower.SetWindowText(strPower);
	m_editPowerTarget.SetWindowText(strStdMax);
	m_editPowerVari.SetWindowText(strStdMin);
	if(fIsLessThanStd(strStdMin, strPower) < 0){
		AfxMessageBox(_T("�������㣬����"));
		goto fail;
	}

	if(fIsLessThanStd(strStdMax, strPower) > 0){
		AfxMessageBox(_T("�������࣬���Զ����źĵ�"));
		goto fail;
	}


	m_editTestState.SetWindowText(_T("�������Գɹ�..."));
	m_editPowerTestResult.SetWindowText(_T("PASS"));
	return 0;
fail:
	m_editTestState.SetWindowText(_T("��������ʧ��..."));
	m_editPowerTestResult.SetWindowText(_T("FAIL"));
	return -1;

}


int CW1023����Dlg::KnodPowerOffTest()
{
	m_editTestState.SetWindowText(_T("�ȴ��ж��ػ����Ƿ���Ч..."));

	m_ceKnobEvent.ResetEvent();
	m_bKnobFlag = TRUE;
	m_buttonKnobSucc.EnableWindow(TRUE);
	m_buttonKnobFail.EnableWindow(TRUE);

	AfxMessageBox(_T("����ת��ť�ػ����۲��Ƿ�\n���������ڡ���ť�ж�����\nѡ�񰴼��ϴ����"));

	WaitForSingleObject(m_ceKnobEvent.m_hObject, INFINITE);
	m_ceKnobEvent.ResetEvent();
	
	if (FALSE == m_bKnobFlag) {/*�ж��ػ���ʧЧ*/
		goto fail;
	}


	m_buttonKnobSucc.EnableWindow(FALSE);
	m_buttonKnobFail.EnableWindow(FALSE);
	m_editTestState.SetWindowText(_T("�ػ������ж���Ч"));
	return 0;

fail:
	m_buttonKnobSucc.EnableWindow(FALSE);
	m_buttonKnobFail.EnableWindow(FALSE);
	m_editTestState.SetWindowText(_T("�ػ������ж���Ч"));
	return -1;
}


int CW1023����Dlg::FunctionPlayTest()
{
	int n = 0;
	int nRet;
	CString strFilePcPath;
	CString strFileDutPath;
	CString strFileName;
	CString strMsg, strErrMsg;

	//m_nStation = SWEEPING_PLAY_TEST;
	m_editPlayTestResult.SetWindowText(_T("..."));

	/*1. ��ȡ�ļ�·�����ļ���*/
	if (FALSE == GetFileName(strFilePcPath, strFileName)) {
		strFilePcPath = _T("sin.wav");
		nRet = PushFile(strFilePcPath);
		if (nRet != 0) {
			m_editTestState.SetWindowText(_T("������Ƶ�ļ����豸ʧ��..."));
			//AfxMessageBox(_T("�����ļ���DUT�豸ʧ��"));
			m_editPlayTestResult.SetWindowText(_T("FAIL"));
			return -1;
		}
		strFileDutPath.Format(_T("/mnt/SDCARD/sin.wav "));
		strMsg.Format(_T("���ڲ�����Ƶ�ļ�(sin.wav)..."));
		strErrMsg.Format(_T("������Ƶ�ļ�(sin.wav)ʧ��..."));
	}
	else {

		m_editTestState.SetWindowText(_T("���������ļ���DUT..."));
		/*3. �����ļ���DUT*/
		nRet = PushFile(strFilePcPath);
		if (nRet != 0) {
			m_editTestState.SetWindowText(_T("������Ƶ�ļ����豸ʧ��..."));
			//AfxMessageBox(_T("�����ļ���DUT�豸ʧ��"));
			m_editPlayTestResult.SetWindowText(_T("FAIL"));
			return -1;
		}

		/*4. �������ŵ��ļ���*/
		strFileDutPath.Format(_T("/mnt/SDCARD/%s"), strFileName);
		strMsg.Format(_T("���ڲ�����Ƶ�ļ�(%s)..."), strFileName);
		strErrMsg.Format(_T("������Ƶ�ļ�(%s)ʧ��..."), strFileName);
	}

	/*5. ����*/
	m_editTestState.SetWindowText(strMsg);

	nRet = MusicPlay(strFileDutPath);
	if (nRet != 0) {
		m_editTestState.SetWindowText(strErrMsg);
		goto fail;
	}


	m_editTestState.SetWindowText(_T("���Ų��Գɹ�..."));
	m_editPlayTestResult.SetWindowText(_T("PASS"));
	return 0;

fail:
	m_editTestState.SetWindowText(_T("���Ų���ʧ��..."));
	m_editPlayTestResult.SetWindowText(_T("FAIL"));
	return -1;
}


int CW1023����Dlg::SweepingPlayTest()
{
	int n = 0;
	int nRet;
	CString strFilePcPath;
	CString strFileDutPath;
	CString strFileName;
	CString strMsg, strErrMsg;

	m_nStation = SWEEPING_PLAY_TEST;
	m_editPlayTestResult.SetWindowText(_T("..."));
	

	/*1. ��ȡ�ļ�·�����ļ���*/
	if (FALSE == GetFileName(strFilePcPath, strFileName)) {
		strFilePcPath = _T("sin.wav");
		nRet = PushFile(strFilePcPath);
		if (nRet != 0) {
			m_editTestState.SetWindowText(_T("������Ƶ�ļ����豸ʧ��..."));
			//AfxMessageBox(_T("�����ļ���DUT�豸ʧ��"));
			m_editPlayTestResult.SetWindowText(_T("FAIL"));
			return -1;
		}
		strFileDutPath.Format(_T("/mnt/SDCARD/sin.wav "));
		strMsg.Format(_T("���ڲ�����Ƶ�ļ�(sin.wav)..."));
		strErrMsg.Format(_T("������Ƶ�ļ�(sin.wav)ʧ��..."));
	}
	else {

		m_editTestState.SetWindowText(_T("���������ļ���DUT..."));
		/*3. �����ļ���DUT*/
		nRet = PushFile(strFilePcPath);
		if (nRet != 0) {
			m_editTestState.SetWindowText(_T("������Ƶ�ļ����豸ʧ��..."));
			//AfxMessageBox(_T("�����ļ���DUT�豸ʧ��"));
			m_editPlayTestResult.SetWindowText(_T("FAIL"));
			return -1;
		}

		/*4. �������ŵ��ļ���*/
		strFileDutPath.Format(_T("/mnt/SDCARD/%s"), strFileName);
		strMsg.Format(_T("���ڲ�����Ƶ�ļ�(%s)..."), strFileName);
		strErrMsg.Format(_T("������Ƶ�ļ�(%s)ʧ��..."), strFileName);
	}

	m_buttonSweepPass.EnableWindow(TRUE);
	m_buttonSweepFail.EnableWindow(TRUE);
	m_bSweepFlag = TRUE;
	/*5. ����*/
	m_editTestState.SetWindowText(strMsg);
	while (m_nLiveFlag) {
		nRet = MusicPlay(strFileDutPath);
		if (FALSE == m_nLiveFlag) {
			break;
		}
		if (nRet < 0) {
			AfxMessageBox(_T("����ʧ��"));
			m_editTestState.SetWindowText(strErrMsg);
			goto fail;
		}
		if (nRet > 0) {
			continue;
		}

	}

	if (m_bSweepFlag == TRUE) {
		goto pass;
	}
	else {
		goto fail;
	}

	

pass:
	m_buttonSweepPass.EnableWindow(FALSE);
	m_buttonSweepFail.EnableWindow(FALSE);
	m_editTestState.SetWindowText(_T("���Ų��Գɹ�..."));
	m_editPlayTestResult.SetWindowText(_T("PASS"));
	return 0;

fail:
	m_buttonSweepPass.EnableWindow(FALSE);
	m_buttonSweepFail.EnableWindow(FALSE);
	m_editTestState.SetWindowText(_T("���Ų���ʧ��..."));
	m_editPlayTestResult.SetWindowText(_T("FAIL"));
	return -1;
}


int CW1023����Dlg::SDWRRatesTest()
{
	int nRet;

	m_nStation = SD_WR_RATE_TEST;

	m_editSDTestResult.SetWindowText(_T("..."));

	m_editSdWriteVal.SetWindowText(_T("..."));
	m_editTestState.SetWindowText(_T("���ڲ���SD��д����..."));
	nRet = SDWriteTest();
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("����SD��д����ʧ��..."));
		m_editSDTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}
	else if (nRet > 0) {
		//goto writeTest;
		m_editTestState.SetWindowText(_T("����SD��д����ʧ��..."));
		m_editSDTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}

	m_editSdReadVal.SetWindowText(_T("..."));
	m_editTestState.SetWindowText(_T("���ڲ���SD��������..."));
	nRet = SDReadTest();
	if (nRet < 0) {
		m_editTestState.SetWindowText(_T("����SD��������ʧ��..."));
		m_editSDTestResult.SetWindowText(_T("FAIL"));
		return -1;/*���Բ�ͨ��*/
	}
	else if (nRet > 0) {
		//goto readTest;
		m_editTestState.SetWindowText(_T("����SD��������ʧ��..."));
		m_editSDTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}

	m_editTestState.SetWindowText(_T("����SD�������ʳɹ�..."));
	m_editSDTestResult.SetWindowText(_T("PASS"));
	return 0;
}


BOOL CW1023����Dlg::PrepareTest()
{
	int nRet;
	CString strFactory;
	CString strDest;

#if 1
	m_editTestState.SetWindowText(_T("�������ͳ����ļ�..."));
	/*1. ���볧��ģʽ��ʵ����������factory�ļ���SD��*/
	strFactory.Format(_T("factory"));
	strDest.Format(_T("/mnt/SDCARD"));
	nRet = PushFile(strFactory, strDest);
	if (nRet < 0) {
		m_editTestState.SetWindowText(_T("���ͳ����ļ�ʧ��..."));
		m_editTestResult.SetWindowText(_T("FAIL"));
		return FALSE;
	}

	m_editTestState.SetWindowText(_T("�����޸ĳ����ļ�Ȩ��..."));
	nRet = ChmodFactory();
	if (nRet < 0) {
		m_editTestState.SetWindowText(_T("�޸ĳ����ļ�Ȩ��ʧ��..."));
		m_editTestResult.SetWindowText(_T("FAIL"));
		return FALSE;
	}
#endif

#if 0  
	m_editTestState.SetWindowText(_T("���ڳ�ʼ������ģʽ..."));
	nRet = FactorySet();
	if (nRet < 0) {
		m_editTestState.SetWindowText(_T("��ʼ������ģʽʧ��..."));
		m_editTestResult.SetWindowText(_T("FAIL"));
		return FALSE;
	}
#endif

	m_editTestState.SetWindowText(_T("���ڽ��볧��ģʽ..."));
	nRet = EnterFactory();
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("���볧��ģʽʧ��..."));
		//m_editTestResult.SetWindowText(_T("FAIL"));
		return FALSE;
	}

	m_editTestState.SetWindowText(_T("���ڻ�ȡ����汾..."));
	nRet = GetFirmwareVer();
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("��ȡ����汾ʧ��..."));
		//m_editTestResult.SetWindowText(_T("FAIL"));
		return FALSE;
	}

	return TRUE;
}


/***************************End:�������***************************/
BOOL CW1023����Dlg::FunctionTest()
{
	int nRet;
	BOOL bFlag = TRUE;
	CString strTemp;

	m_editTestResult.SetWindowText(_T("..."));

	strTemp.Format(FUNC_STATION);
	m_strResult.SetStation(strTemp);

	/*Check SN*/
	bFlag = SNCheck();
	if (FALSE == bFlag) {/*SN Check��ͨ��*/
		//AfxMessageBox(_T("S/N��ⲻͨ��"));
		m_editTestResult.SetWindowText(_T("FAIL"));
		LogTestResult(FALSE);
		goto end;
	}

	//AfxMessageBox(_T("S/N���ͨ��"));

	/*�����ʼ���ȶ���*/
	if (FALSE == PrepareTest()) {
		goto fail;
	}

	/*1. ��������*/
	if (m_checkKeyTest.GetCheck() == BST_CHECKED) {
		nRet = KeyTest();
		if (nRet < 0) {
			strTemp.Format(_T("FAIL"));
			m_strResult.SetKey(strTemp);
			goto fail;
		}
		else if(nRet > 0) {
			strTemp.Format(_T("Out"));
			m_strResult.SetKey(strTemp);
			goto out;
		}
		else {
			strTemp.Format(_T("PASS"));
			m_strResult.SetKey(strTemp);
		}
	}
	else {
		bFlag = FALSE;
	}

	/*2. Led����*/
	if (m_checkLedTest.GetCheck() == BST_CHECKED) {
		nRet = LedTest();
		if (nRet < 0) {
			strTemp.Format(_T("FAIL"));
			m_strResult.SetLed(strTemp);
			goto fail;
		}
		else if(nRet > 0) {
			strTemp.Format(_T("Out"));
			m_strResult.SetLed(strTemp);
			goto out;
		}
		else {
			strTemp.Format(_T("PASS"));
			m_strResult.SetLed(strTemp);
		}
	}
	else {
		bFlag = FALSE;
	}

	/*3. wifi����*/
	if (m_checkWifiTest.GetCheck() == BST_CHECKED) {
		nRet = WifiTest();
		if (nRet < 0) {
			strTemp.Format(_T("FAIL"));
			m_strResult.SetWifi(strTemp);
			goto fail;
		}
		else if(nRet > 0) {
			strTemp.Format(_T("Out"));
			m_strResult.SetWifi(strTemp);
			goto out;
		}
		else {
			strTemp.Format(_T("PASS"));
			m_strResult.SetWifi(strTemp);
		}
	}
	else {
		bFlag = FALSE;
	}

	/*4. TF������--�����*/
	if (m_checkSdCardTest.GetCheck() == BST_CHECKED) {
		nRet = SDWRRatesTest();
		if (nRet < 0) {
			//strTemp.Format(_T("Fail"));
			//m_strResult.SetCard(strTemp);
			goto fail;
		}
		else if(nRet > 0) {
			//strTemp.Format(_T("Out"));
			//m_strResult.SetCard(strTemp);
			goto out;
		}
		else {
			//strTemp.Format(_T("Pass"));
			//m_strResult.SetCard(strTemp);
		}
	}
	else {
		bFlag = FALSE;
	}
	

	/*5. ���Ų���*/
	if (m_checkPlayTest.GetCheck() == BST_CHECKED)
	{
		AfxMessageBox(_T("�����в��Ų��ԣ���ͬʱ\n����������������ť����\n�����Ƿ�����������������\n���ڡ����Ų����ж�����\nѡ�񰴼��ϴ����"));

		nRet = SweepingPlayTest();
		if (nRet < 0) {
			strTemp.Format(_T("FAIL"));
			m_strResult.SetPlay(strTemp);
			goto fail;
		}
		else {
			strTemp.Format(_T("PASS"));
			m_strResult.SetPlay(strTemp);
		}

		if (FALSE == m_bSweepFlag) {
			m_editTestState.SetWindowText(_T("���Ų����ж�ʧ��..."));
			goto fail;
		}
	}
	else {
		bFlag = FALSE;
	}

	m_editTestState.SetWindowText(_T("���Ų����ж�ͨ��..."));
	
	nRet = KnodPowerOffTest();
	if (nRet != 0) {
		goto fail;
	}

pass:
	if (TRUE == bFlag) {
		/*5. �ϴ����Խ��*/
		m_editTestState.SetWindowText(_T("�����ϴ����Խ����MESϵͳ..."));
		if (FALSE == UploadTestResult()) {
			//m_editTestState.SetWindowText(_T("�ϴ����Խ����MESϵͳʧ��..."));
			AfxMessageBox(_T("�ϴ����Խ��ʧ��"));
			m_editTestResult.SetWindowText(_T("FAIL"));
			LogTestResult(FALSE);
			goto end;
		}

		/*6. ���ڹ�վ*/
		m_editTestState.SetWindowText(_T("���ڹ�վ..."));
		if (FALSE == SNSave()) {
			//m_editTestState.SetWindowText(_T("��վʧ��..."));
			AfxMessageBox(_T("��վʧ��"));
			m_editTestResult.SetWindowText(_T("FAIL"));
			LogTestResult(FALSE);
			goto end;
		}
		m_editTestState.SetWindowText(_T(""));
		m_editTestResult.SetWindowText(_T("PASS"));
		LogTestResult(TRUE);
		goto end;
	}
	else {
		m_editTestResult.SetWindowText(_T("N/A"));
		goto end;
	}

fail:
	/*5. �ϴ����Խ��*/
	//m_editTestState.SetWindowText(_T("�����ϴ����Խ����MESϵͳ..."));
	if (FALSE == UploadTestResult()) {
		//m_editTestState.SetWindowText(_T("�ϴ����Խ����MESϵͳʧ��..."));
		AfxMessageBox(_T("�ϴ����Խ��ʧ��"));
	}
	m_editTestResult.SetWindowText(_T("FAIL"));
	LogTestResult(FALSE);
	goto end;

out:
	m_editTestState.SetWindowText(_T(""));
	m_editTestResult.SetWindowText(_T("N/A"));
	return FALSE;

end:
	return TRUE;
}


BOOL CW1023����Dlg::SweepingTest()
{
	int nRet;
	CString strStation;
	CString strTemp;
	BOOL bFlag = TRUE;

	m_editTestResult.SetWindowText(_T("..."));

	strTemp.Format(SWEEP_STATION);
	m_strResult.SetStation(strTemp);

	bFlag = SNCheck();
	if (FALSE == bFlag) {/*SN Check��ͨ��*/
		//AfxMessageBox(_T("S/N��ⲻͨ��"));
		m_editTestResult.SetWindowText(_T("FAIL"));
		LogTestResult(FALSE);
		goto end;
	}

	//AfxMessageBox(_T("S/N���ͨ��"));

	/*�����ʼ���ȶ���*/
	if (FALSE == PrepareTest()) {
		goto fail;
	}

#if 1
	/*2. ��ȡ��һվ(�ϻ�����)���Խ��*/
	strStation.Format(BURN_STATION);
	nRet = DisplayTestInfo(strStation);
	if (nRet > 0) {
		AfxMessageBox(_T("��ȡ�ϻ����Խ��ʧ�ܣ�����USB���Ӽ�����"));
		goto fail;
	}
	else if (nRet < 0) {
		AfxMessageBox(_T("�ϻ�����δͨ�������ز⣡"));
		goto fail;
	}
#endif

	/*3. ���Ų���*/
	if (m_checkPlayTest.GetCheck() == BST_CHECKED)
	{
		AfxMessageBox(_T("������ɨƵ���Ų��ԣ�����\n�����Ų����ж�����ѡ�񰴼�\n�ϴ����"));
		nRet = SweepingPlayTest();
		if (nRet < 0) {
			strTemp.Format(_T("FAIL"));
			m_strResult.SetPlay(strTemp);
			goto fail;
		}
		else {
			strTemp.Format(_T("PASS"));
			m_strResult.SetPlay(strTemp);
		}
	}
	else {
		bFlag = FALSE;
	}

	//m_editTestState.SetWindowText(_T("�ȴ��û�������Խ��..."));
	//AfxMessageBox(_T("��������Խ��"));
	/*4. �ȴ��û�ѡ��������ͨ������Fail*/
	//WaitForSingleObject(m_ceSweepEvent.m_hObject, INFINITE);
	//m_ceSweepEvent.ResetEvent();
	if (m_bSweepFlag) {
		m_editTestState.SetWindowText(_T("ɨƵ�����ж�ͨ��..."));
		goto pass;
	}
	else {
		m_editTestState.SetWindowText(_T("ɨƵ�����ж�ʧ��..."));
		goto fail;
	}


pass:
	if (bFlag) {
		/*5. �ϴ����Խ��*/
		m_editTestState.SetWindowText(_T("�����ϴ����Խ����MESϵͳ..."));
		if (FALSE == UploadTestResult()) {
			//m_editTestState.SetWindowText(_T("�ϴ����Խ����MESϵͳʧ��..."));
			AfxMessageBox(_T("�ϴ����Խ��ʧ��"));
			m_editTestResult.SetWindowText(_T("FAIL"));
			LogTestResult(FALSE);
			goto end;
		}

		/*6. ���ڹ�վ*/
		m_editTestState.SetWindowText(_T("���ڹ�վ..."));
		if (FALSE == SNSave()) {
			//m_editTestState.SetWindowText(_T("��վʧ��..."));
			AfxMessageBox(_T("��վʧ��"));
			m_editTestResult.SetWindowText(_T("FAIL"));
			LogTestResult(FALSE);
			goto end;
		}

		m_editTestState.SetWindowText(_T(""));
		m_editTestResult.SetWindowText(_T("PASS"));
		LogTestResult(TRUE);
		goto end;
	}
	else {
		m_editTestResult.SetWindowText(_T("N/A"));
		goto end;
	}

fail:
	/*5. �ϴ����Խ��*/
	if (FALSE == UploadTestResult()) {
		//m_editTestState.SetWindowText(_T("�ϴ����Խ����MESϵͳʧ��..."));
		AfxMessageBox(_T("�ϴ����Խ��ʧ��"));
	}
	m_editTestResult.SetWindowText(_T("FAIL"));
	LogTestResult(FALSE);
	goto end;

end:/*��������򷵻�TRUE��δ����򷵻�FALSE*/
	return TRUE;
}


BOOL CW1023����Dlg::OutFactoryTest()
{
	int nRet;
	CString strTemp;
	BOOL bFlag = TRUE;

	m_editTestResult.SetWindowText(_T("..."));


	strTemp.Format(PACK_STATION);
	m_strResult.SetStation(strTemp);

	bFlag = SNCheck();
	if (FALSE == bFlag) {/*SN Check��ͨ��*/
		//AfxMessageBox(_T("S/N��ⲻͨ��"));
		m_editTestResult.SetWindowText(_T("FAIL"));
		LogTestResult(FALSE);
		goto end;
	}

	if (TRUE == m_bPassEATest) {/*��������������û�ģʽ�İ������ϴ�MESϵͳʧ�ܺ����ٲ��Ե�����*/
		strTemp.Format(_T("PASS"));
		m_strResult.SetPower(strTemp);
		goto pass;
	}

	/*�����ʼ���ȶ���*/
	if (FALSE == PrepareTest()) {
		goto fail;
	}

#if 1
	m_editTestState.SetWindowText(_T("���ڻ�ȡ��ǰ����..."));
	/*3. ��������*/
	if (m_checkPowerTest.GetCheck() == BST_CHECKED)
	{
		nRet = PowerTest();
		if (nRet < 0) {
			strTemp.Format(_T("FAIL"));
			m_strResult.SetPower(strTemp);
			goto fail;
		}
		else {
			strTemp.Format(_T("PASS"));
			m_strResult.SetPower(strTemp);
		}
	}
	else {
		bFlag = FALSE;
	}
#endif

	if (m_checkUploadMes.GetCheck() == BST_CHECKED) {
		/*4. ����ģʽ*/
		m_editTestState.SetWindowText(_T("���ڻָ���������..."));
		nRet = OutFactory();
		if (nRet != 0) {
			m_editTestState.SetWindowText(_T("�ָ���������ʧ��"));
			goto fail;
		}

		m_editTestState.SetWindowText(_T("����ɹ������ڼ�¼..."));
		nRet = FactoryTestSuccess();
		if (nRet != 0) {
			m_editTestState.SetWindowText(_T("��������¼ʧ��"));
			goto fail;
		}
	}
pass:
	if (bFlag) {
		/*5. �ϴ����Խ��*/
		m_editTestState.SetWindowText(_T("�����ϴ����Խ����MESϵͳ..."));
		if (FALSE == UploadTestResult()) {
			//m_editTestState.SetWindowText(_T("�ϴ����Խ����MESϵͳʧ��..."));
			AfxMessageBox(_T("�ϴ����Խ��ʧ��"));
			m_editTestResult.SetWindowText(_T("FAIL"));
			LogTestResult(FALSE);
			goto end;
		}

		/*6. ���ڹ�վ*/
		m_editTestState.SetWindowText(_T("���ڹ�վ..."));
		if (FALSE == SNSave()) {
			//m_editTestState.SetWindowText(_T("��վʧ��..."));
			AfxMessageBox(_T("��վʧ��"));
			m_editTestResult.SetWindowText(_T("FAIL"));
			LogTestResult(FALSE);
			goto end;
		}
		m_editTestState.SetWindowText(_T(""));
		m_editTestResult.SetWindowText(_T("PASS"));
		LogTestResult(TRUE);
		goto end;
	}
	else {
		goto end;
	}

fail:
	/*5. �ϴ����Խ��*/
	//m_editTestState.SetWindowText(_T("�����ϴ����Խ����MESϵͳ..."));
	if (FALSE == UploadTestResult()) {
		//m_editTestState.SetWindowText(_T("�ϴ����Խ����MESϵͳʧ��..."));
		AfxMessageBox(_T("��¼���Խ��ʧ��"));
	}

	m_editTestResult.SetWindowText(_T("FAIL"));
	LogTestResult(FALSE);
	PowerDischarge();
	goto end;


end:/*��������򷵻�TRUE��δ����򷵻�FALSE*/
	return TRUE;
}


BOOL CW1023����Dlg::MainTestLoop()
{
	int nSel;
	CString strFactory;

	CString strSn;
	CString strStation;

	m_editTestResult.SetWindowText(_T("..."));

	m_editSN.GetWindowText(strSn);
	m_strResult.SetSN(strSn);

	
	/*2. ��⵱ǰ�����ĸ�����ģʽ��*/
	nSel = m_comboTestItem.GetCurSel();

	switch (nSel) {
		case 0:/*ɨƵģʽ*/
			SweepingTest();
			break;
		case 1:/*���ܲ���*/
			FunctionTest();
			break;
		case 2:/*��������*/
			OutFactoryTest();
			break;
	}

	return TRUE;
}


BOOL CW1023����Dlg::KillProcessFromName(CString strProcessName)
{
	//�������̿���(TH32CS_SNAPPROCESS��ʾ�������н��̵Ŀ���)    
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//PROCESSENTRY32���̿��յĽṹ��    
	PROCESSENTRY32 pe;
	
	//ʵ������ʹ��Process32First��ȡ��һ�����յĽ���ǰ�����ĳ�ʼ������    
	pe.dwSize = sizeof(PROCESSENTRY32);

	//�����IFЧ��ͬ:    
	//if(hProcessSnap == INVALID_HANDLE_VALUE)   ��Ч�ľ��    
	if (!Process32First(hSnapShot, &pe))
	{
		 return FALSE;
	}
	
	//���ַ���ת��ΪСд    
	strProcessName.MakeLower();
	
	//��������Ч  ��һֱ��ȡ��һ�����ѭ����ȥ    
	while (Process32Next(hSnapShot, &pe))
	{
	
		//pe.szExeFile��ȡ��ǰ���̵Ŀ�ִ���ļ�����    
		CString scTmp = pe.szExeFile;
	
		//����ִ���ļ���������Ӣ����ĸ�޸�ΪСд    
		scTmp.MakeLower();
	
		//�Ƚϵ�ǰ���̵Ŀ�ִ���ļ����ƺʹ��ݽ������ļ������Ƿ���ͬ    
		//��ͬ�Ļ�Compare����0    
		if (!scTmp.Compare(strProcessName))
		{
			//�ӿ��ս����л�ȡ�ý��̵�PID(������������е�PID)    
			DWORD dwProcessID = pe.th32ProcessID;
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
			TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
			scTmp.ReleaseBuffer();
			strProcessName.ReleaseBuffer();
			return TRUE;
		}
		scTmp.ReleaseBuffer();
	}
	strProcessName.ReleaseBuffer();
	return FALSE;
}


BOOL CW1023����Dlg::GetHostIPaddr(CString &sLocalName)
{
	
	char szLocalName[256];//����  
	WSADATA wsaData;//�����׽��ִ洢����  
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)
	{
		if (gethostname(szLocalName, sizeof(szLocalName)) != 0) {//�������� 
			m_strHostIp = _T("");
			WSACleanup();
			//return GetLastError();
			return FALSE;
		}
		else{
			sLocalName = szLocalName;//��sLocalName�����洢��õļ��������  
			struct hostent FAR * lpHostEnt = gethostbyname(sLocalName);
			if (lpHostEnt == NULL) {//����
				m_strHostIp = _T("");
				WSACleanup();
				//return GetLastError();
				return FALSE;
			}
			LPSTR lpAddr = lpHostEnt->h_addr_list[0];//��ȡIP��ַ  
			if (lpAddr)
			{
				struct in_addr inAddr;
				memmove(&inAddr, lpAddr, 4);
				m_strHostIp = inet_ntoa(inAddr);//ת��Ϊ��׼��ʽ  
				if (m_strHostIp.IsEmpty())
				{
					m_strHostIp = _T("");
					WSACleanup();
					//return 1;
					return FALSE;
				}
			}
		}
	}
	 WSACleanup();//����׽��ֳ�ʼ������  
	 //return 0
	 return TRUE;
}


void CW1023����Dlg::SaveConfig(int nFlag)
{
	CStdioFile config_file;
	CString config_file_path = "./config.ini";
	CFileFind finder;
	CString strSsid;
	CString strPasswd;
	CString strFileName;
	CString strItem;
	CString strTemp;

	BOOL bFlag = finder.FindFile(config_file_path);
	if (FALSE == bFlag){
		AfxMessageBox(_T("�����ļ�-config.ini-������"));
		return;
	}

	switch (nFlag) {
		case 0:
			if (m_checkPlayTest.GetCheck() == BST_CHECKED) {
				WritePrivateProfileString("SWEEP SETTING", "PLAY_TEST", "1", config_file_path);
			}
			else {
				WritePrivateProfileString("SWEEP SETTING", "PLAY_TEST", "0", config_file_path);
			}

			if (m_checkUploadMes.GetCheck() == BST_CHECKED) {
				WritePrivateProfileString("SWEEP SETTING", "MES_PUSH", "1", config_file_path);
			}
			else {
				WritePrivateProfileString("SWEEP SETTING", "MES_PUSH", "0", config_file_path);
			}

			if (m_buttonPushFile.GetCheck() == BST_CHECKED) {
				WritePrivateProfileString("SWEEP SETTING", "FILE_PUSH", "1", config_file_path);
			}
			else {
				WritePrivateProfileString("SWEEP SETTING", "FILE_PUSH", "0", config_file_path);
			}

			m_editFileName.GetWindowText(strFileName);
			WritePrivateProfileString("SWEEP SETTING", "FILE_NAME", strFileName, config_file_path);
			break;
		case 1:
			if (m_checkLedTest.GetCheck() == BST_CHECKED) {
				WritePrivateProfileString("FUNCTION SETTING", "LED_TEST", "1", config_file_path);
			}
			else {
				WritePrivateProfileString("FUNCTION SETTING", "LED_TEST", "0", config_file_path);
			}

			if (m_checkKeyTest.GetCheck() == BST_CHECKED) {
				WritePrivateProfileString("FUNCTION SETTING", "KEY_TEST", "1", config_file_path);
			}
			else {
				WritePrivateProfileString("FUNCTION SETTING", "KEY_TEST", "0", config_file_path);
			}

			if (m_checkWifiTest.GetCheck() == BST_CHECKED) {
				WritePrivateProfileString("FUNCTION SETTING", "WIFI_TEST", "1", config_file_path);
			}
			else {
				WritePrivateProfileString("FUNCTION SETTING", "WIFI_TEST", "0", config_file_path);
			}

			if (m_checkSdCardTest.GetCheck() == BST_CHECKED) {
				WritePrivateProfileString("FUNCTION SETTING", "SD_TEST", "1", config_file_path);
			}
			else {
				WritePrivateProfileString("FUNCTION SETTING", "SD_TEST", "0", config_file_path);
			}

			if (m_checkPlayTest.GetCheck() == BST_CHECKED) {
				WritePrivateProfileString("FUNCTION SETTING", "PLAY_TEST", "1", config_file_path);
			}
			else {
				WritePrivateProfileString("FUNCTION SETTING", "PLAY_TEST", "0", config_file_path);
			}

			if (m_checkUploadMes.GetCheck() == BST_CHECKED) {
				WritePrivateProfileString("FUNCTION SETTING", "MES_PUSH", "1", config_file_path);
			}
			else {
				WritePrivateProfileString("FUNCTION SETTING", "MES_PUSH", "0", config_file_path);
			}

			if (m_buttonPushFile.GetCheck() == BST_CHECKED) {
				WritePrivateProfileString("FUNCTION SETTING", "FILE_PUSH", "1", config_file_path);
			}
			else {
				WritePrivateProfileString("FUNCTION SETTING", "FILE_PUSH", "0", config_file_path);
			}

			m_editFileName.GetWindowText(strFileName);
			WritePrivateProfileString("FUNCTION SETTING", "FILE_NAME", strFileName, config_file_path);

			m_editSsid.GetWindowText(strSsid);
			WritePrivateProfileString("FUNCTION SETTING", "WIFI_SSID", strSsid, config_file_path);

			m_editPasswd.GetWindowText(strPasswd);
			WritePrivateProfileString("FUNCTION SETTING", "WIFI_PASSWD", strPasswd, config_file_path);

			m_editRssiMin.GetWindowText(strTemp);
			WritePrivateProfileString("FUNCTION SETTING", "WIFI_RSSIMIN", strTemp, config_file_path);

			m_editRssiMax.GetWindowText(strTemp);
			WritePrivateProfileString("FUNCTION SETTING", "WIFI_RSSIMAX", strTemp, config_file_path);

			m_editMbpsMin.GetWindowText(strTemp);
			WritePrivateProfileString("FUNCTION SETTING", "WIFI_MBPSMIN", strTemp, config_file_path);

			m_editMbpsMax.GetWindowText(strTemp);
			WritePrivateProfileString("FUNCTION SETTING", "WIFI_MBPSMAX", strTemp, config_file_path);

			m_editMbpsMin2.GetWindowText(strTemp);
			WritePrivateProfileString("FUNCTION SETTING", "WIFI_MBPSMIN2", strTemp, config_file_path);

			m_editMbpsMax2.GetWindowText(strTemp);
			WritePrivateProfileString("FUNCTION SETTING", "WIFI_MBPSMXA2", strTemp, config_file_path);

			WritePrivateProfileString("FUNCTION SETTING", "SD_READ", m_strReadStdVal, config_file_path);
			WritePrivateProfileString("FUNCTION SETTING", "SD_WRITE", m_strWriteStdVal, config_file_path);

			break;
		case 2:
			if (m_checkPowerTest.GetCheck() == BST_CHECKED) {
				WritePrivateProfileString("PACKAGE SETTING", "POWER_TEST", "1", config_file_path);
			}
			else {
				WritePrivateProfileString("PACKAGE SETTING", "POWER_TEST", "0", config_file_path);
			}

			if (m_checkUploadMes.GetCheck() == BST_CHECKED) {
				WritePrivateProfileString("PACKAGE SETTING", "MES_PUSH", "1", config_file_path);
			}
			else {
				WritePrivateProfileString("PACKAGE SETTING", "MES_PUSH", "0", config_file_path);
			}

			m_editPowerTarget.GetWindowText(strTemp);
			WritePrivateProfileString("PACKAGE SETTING", "POWER_MAX", strTemp, config_file_path);

			m_editPowerVari.GetWindowText(strTemp);
			WritePrivateProfileString("PACKAGE SETTING", "POWER_MIN", strTemp, config_file_path);
			break;
	}
	
	strItem.Format(_T("%d"), nFlag);
	WritePrivateProfileString("HEAD SETTING", "TEST_ITEM", strItem, config_file_path);
	WritePrivateProfileString("HEAD SETTING", "LOG_LOCA", m_strLogPath, config_file_path);

	return;
}



void CW1023����Dlg::SaveLogConfig()
{
	CStdioFile config_file;
	CString config_file_path = "./config.ini";
	CFileFind finder;
	CString strSsid;
	CString strPasswd;
	CString strFileName;
	CString strItem;
	CString strTemp;

	BOOL bFlag = finder.FindFile(config_file_path);
	if (FALSE == bFlag) {
		AfxMessageBox(_T("�����ļ�-config.ini-������"));
		return;
	}

	WritePrivateProfileString("HEAD SETTING", "LOG_LOCA", m_strLogPath, config_file_path);

	return;
}


void CW1023����Dlg::SaveSdTestConfig()
{
	CStdioFile config_file;
	CString config_file_path = "./config.ini";
	CFileFind finder;
	CString strSsid;
	CString strPasswd;
	CString strFileName;
	CString strItem;
	CString strTemp;

	BOOL bFlag = finder.FindFile(config_file_path);
	if (FALSE == bFlag) {
		AfxMessageBox(_T("�����ļ�-config.ini-������"));
		return;
	}

	WritePrivateProfileString("FUNCTION SETTING", "SD_READ", m_strReadStdVal, config_file_path);
	WritePrivateProfileString("FUNCTION SETTING", "SD_WRITE", m_strWriteStdVal, config_file_path);

	return;
}


void CW1023����Dlg::LoadConfig(int nFlag)
{
	CStdioFile config_file;
	CString config_file_path = "./config.ini";
	CFileFind finder;
	int nTemp;
	CString strTemp;

	BOOL bFlag = finder.FindFile(config_file_path);
	if (FALSE == bFlag) {
		AfxMessageBox(_T("�����ļ�-config.ini-������"));
		return;
	}

	switch (nFlag) {
		case 0:   /*ɨƵ����*/
			nTemp = GetPrivateProfileInt("SWEEP SETTING", "PLAY_TEST", 1, config_file_path);
			if (0 == nTemp) {
				m_checkPlayTest.SetCheck(FALSE);
			}
			else {
				m_checkPlayTest.SetCheck(TRUE);
			}

			nTemp = GetPrivateProfileInt("SWEEP SETTING", "MES_PUSH", 1, config_file_path);
			if (0 == nTemp) {
				m_checkUploadMes.SetCheck(FALSE);
			}
			else {
				m_checkUploadMes.SetCheck(TRUE);
			}

			nTemp = GetPrivateProfileInt("SWEEP SETTING", "FILE_PUSH", 1, config_file_path);
			if (0 == nTemp) {
				m_buttonPushFile.SetCheck(FALSE);
			}
			else {
				m_buttonPushFile.SetCheck(TRUE);
			}
	
			GetPrivateProfileString("SWEEP SETTING", "FILE_NAME", "test.wav", strTemp.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
			m_editFileName.SetWindowText(strTemp);

			m_checkWifiTest.SetCheck(FALSE);
			m_checkLedTest.SetCheck(FALSE);
			m_checkKeyTest.SetCheck(FALSE);
			m_checkRecordTest.SetCheck(FALSE);
			m_checkPowerTest.SetCheck(FALSE);
			m_checkSdCardTest.SetCheck(FALSE);
			m_editSsid.SetWindowText(_T(""));
			m_editPasswd.SetWindowText(_T(""));
			break;

		case 1:   /*���ܲ���*/
			nTemp = GetPrivateProfileInt("FUNCTION SETTING", "LED_TEST", 1, config_file_path);
			if (0 == nTemp) {
				m_checkLedTest.SetCheck(FALSE);
			}
			else {
				m_checkLedTest.SetCheck(TRUE);
			}

			nTemp = GetPrivateProfileInt("FUNCTION SETTING", "KEY_TEST", 1, config_file_path);
			if (0 == nTemp) {
				m_checkKeyTest.SetCheck(FALSE);
			}
			else {
				m_checkKeyTest.SetCheck(TRUE);
			}

			nTemp = GetPrivateProfileInt("FUNCTION SETTING", "WIFI_TEST", 1, config_file_path);
			if (0 == nTemp) {
				m_checkWifiTest.SetCheck(FALSE);
			}
			else {
				m_checkWifiTest.SetCheck(TRUE);
			}

			nTemp = GetPrivateProfileInt("FUNCTION SETTING", "SD_TEST", 1, config_file_path);
			if (0 == nTemp) {
				m_checkSdCardTest.SetCheck(FALSE);
			}
			else {
				m_checkSdCardTest.SetCheck(TRUE);
			}

			nTemp = GetPrivateProfileInt("FUNCTION SETTING", "PLAY_TEST", 1, config_file_path);
			if (0 == nTemp) {
				m_checkPlayTest.SetCheck(FALSE);
			}
			else {
				m_checkPlayTest.SetCheck(TRUE);
			}

			nTemp = GetPrivateProfileInt("FUNCTION SETTING", "FILE_PUSH", 1, config_file_path);
			if (0 == nTemp) {
				m_buttonPushFile.SetCheck(FALSE);
			}
			else {
				m_buttonPushFile.SetCheck(TRUE);
			}

			nTemp = GetPrivateProfileInt("FUNCTION SETTING", "MES_PUSH", 1, config_file_path);
			if (0 == nTemp) {
				m_checkUploadMes.SetCheck(FALSE);
			}
			else {
				m_checkUploadMes.SetCheck(TRUE);
			}

			GetPrivateProfileString("FUNCTION SETTING", "WIFI_SSID", "SoftwareE_VPN", strTemp.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
			m_editSsid.SetWindowText(strTemp);

			GetPrivateProfileString("FUNCTION SETTING", "WIFI_PASSWD", "rangangwei#", strTemp.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
			m_editPasswd.SetWindowText(strTemp);

			GetPrivateProfileString("FUNCTION SETTING", "WIFI_RSSIMIN", "-85", strTemp.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
			m_editRssiMin.SetWindowText(strTemp);

			GetPrivateProfileString("FUNCTION SETTING", "WIFI_RSSIMAX", "0", strTemp.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
			m_editRssiMax.SetWindowText(strTemp);

			GetPrivateProfileString("FUNCTION SETTING", "WIFI_MBPSMIN", "1", strTemp.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
			m_editMbpsMin.SetWindowText(strTemp);

			GetPrivateProfileString("FUNCTION SETTING", "WIFI_MBPSMAX", "50", strTemp.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
			m_editMbpsMax.SetWindowText(strTemp);

			GetPrivateProfileString("FUNCTION SETTING", "WIFI_MBPSMIN2", "1", strTemp.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
			m_editMbpsMin2.SetWindowText(strTemp);

			GetPrivateProfileString("FUNCTION SETTING", "WIFI_MBPSMXA2", "50", strTemp.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
			m_editMbpsMax2.SetWindowText(strTemp);

			GetPrivateProfileString("FUNCTION SETTING", "FILE_NAME", "test.wav", strTemp.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
			m_editFileName.SetWindowText(strTemp);

			GetPrivateProfileString("FUNCTION SETTING", "SD_READ", "0.625", m_strReadStdVal.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);

			GetPrivateProfileString("FUNCTION SETTING", "SD_WRITE", "0.625", m_strWriteStdVal.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);


			m_checkPowerTest.SetCheck(FALSE);
			m_checkRecordTest.SetCheck(FALSE);
			//m_buttonPushFile.SetCheck(FALSE);
			//m_editFileName.SetWindowText(_T(""));

			break;

		case 2:   /*��������*/
			nTemp = GetPrivateProfileInt("PACKAGE SETTING", "POWER_TEST", 1, config_file_path);
			if (0 == nTemp) {
				m_checkPowerTest.SetCheck(FALSE);
			}
			else {
				m_checkPowerTest.SetCheck(TRUE);
			}

			nTemp = GetPrivateProfileInt("PACKAGE SETTING", "MES_PUSH", 1, config_file_path);
			if (0 == nTemp) {
				m_checkUploadMes.SetCheck(FALSE);
			}
			else {
				m_checkUploadMes.SetCheck(TRUE);
			}

			GetPrivateProfileString("PACKAGE SETTING", "POWER_MAX", "80", strTemp.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
			m_editPowerTarget.SetWindowText(strTemp);

			GetPrivateProfileString("PACKAGE SETTING", "POWER_MIN", "60", strTemp.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
			m_editPowerVari.SetWindowText(strTemp);

			m_checkWifiTest.SetCheck(FALSE);
			m_checkLedTest.SetCheck(FALSE);
			m_checkKeyTest.SetCheck(FALSE);
			m_checkRecordTest.SetCheck(FALSE);
			m_checkPlayTest.SetCheck(FALSE);
			m_buttonPushFile.SetCheck(FALSE);
			m_checkSdCardTest.SetCheck(FALSE);
			m_editSsid.SetWindowText(_T(""));
			m_editPasswd.SetWindowText(_T(""));
			m_editFileName.SetWindowText(_T(""));
			break;
		default:
			break;
	}

	return;
}

void CW1023����Dlg::GetTestItemConf()
{
	CStdioFile config_file;
	CString config_file_path = "./config.ini";
	CFileFind finder;
	int nTemp;

	BOOL bFlag = finder.FindFile(config_file_path);
	if (FALSE == bFlag) {
		AfxMessageBox(_T("�����ļ�-config.ini-������"));
		return;
	}	

	nTemp = GetPrivateProfileInt("HEAD SETTING", "TEST_ITEM", 0, config_file_path);
	m_comboTestItem.SetCurSel(nTemp);

	/*˳���ȡLog�ļ��Ĵ洢λ��*/
	GetPrivateProfileString("HEAD SETTING", "LOG_LOCA", "D://w1023-Log.txt", m_strLogPath.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
}


float CW1023����Dlg::TTOF(CString &strString)
{
	return (float)_ttof(strString);
}


#if 1
BOOL CW1023����Dlg::LogTestResult(BOOL bFlag)
{
	CString strSn;
	CString strStation;
	CString strTotal;
	CString strTemp;

	/*д��S/N*/
	m_strResult.GetSN(strSn);
	m_strResult.GetStation(strStation);
	//m_strResult.GetTotal(strTotal);
	//if ((0 != strTotal.CompareNoCase(_T("Pass"))) && (0 != strTotal.CompareNoCase(_T("Fail")))) {
	//	return TRUE;/*����������ϴ�*/
	//}
	strTemp.Format(_T("........Start........"));
	if (0 != LoadTestResultToLog(&strTemp)) {
		return FALSE;
	}

	strTemp.Format(_T("S/N:[%s]"), strSn);
	if (0 != LoadTestResultToLog(&strTemp)) {
		return FALSE;
	}

	strTemp.Format(_T("Sta:[%s]"), strStation);
	if (0 != LoadTestResultToLog(&strTemp)) {
		return FALSE;
	}

	if (bFlag) {
		strTemp.Format(_T("Ret:[%s]"), _T("PASS"));
	}
	else {
		strTemp.Format(_T("Ret:[%s]"), _T("FAIL"));
	}
	//strTemp.Format(_T("Ret:[%s]"), strTotal);
	if (0 != LoadTestResultToLog(&strTemp)) {
		return FALSE;
	}

	strTemp.Format(_T("........End.........."));
	if (0 != LoadTestResultToLog(&strTemp)) {
		return FALSE;
	}

	return TRUE;
}
#endif



wchar_t* MulityByteToWideChar(char *str)
{
	DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wchar_t *pwText = new wchar_t[dwNum];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, pwText, dwNum);
	return pwText;
}


BOOL CW1023����Dlg::UploadItem(int nFlag)
{
	CString strSn;
	CString strStation;
	CString strVal;
	CString strItem;
	CString strResult;
	CString strReson;
	time_t timep, timeq;
	SFCWebServiceSoapProxy ClientSoap;
	_ns1__SfcTestResult_USCOREUpload resultLoadReq;
	_ns1__SfcTestResult_USCOREUploadResponse resultLoadRer;

	m_strResult.GetSN(strSn);
	m_strResult.GetStation(strStation);
	resultLoadReq.strSSN = strSn.GetBuffer(0);
	resultLoadReq.strEventPoint = strStation.GetBuffer(0);

	switch (nFlag) 
	{
		case TEST_ITEM_WIFI:	/*wifi����*/
			strItem.Format(_T("Wifi"));
			m_strResult.GetWifi(strVal);
			break;
		case TEST_ITEM_LED:	/*Led����*/
			strItem.Format(_T("Led"));
			m_strResult.GetLed(strVal);
			break;
		case TEST_ITEM_KEY:	/*Key����*/
			strItem.Format(_T("Key"));
			m_strResult.GetKey(strVal);
			break;
		case TEST_ITEM_PLAY:	/*Play����*/
			strItem.Format(_T("Play"));
			m_strResult.GetPlay(strVal);
			break;
		case TEST_ITEM_POWER:	/*Power����*/
			strItem.Format(_T("Power"));
			m_strResult.GetPower(strVal);
			break;
		case TEST_ITEM_CARD:	/*SD����*/
			strItem.Format(_T("SD"));
			m_strResult.GetCard(strVal);
			break;
		//case TEST_ITEM_TOTAL:	/*Total����*/
		//	strItem.Format(_T("Total"));
		//	m_strResult.GetTotal(strVal);
		//	break;
		default:	
			break;
	}

	resultLoadReq.testitem = strItem.GetBuffer(0);
	resultLoadReq.testresult = strVal.GetBuffer(0);
	
	if ((0 != strVal.CompareNoCase(_T("FAIL"))) || (0 == strVal.CompareNoCase(_T("PASS")))) {
		/*����Pass����Fail�⣬������Ӧ���ϴ�*/
		return TRUE;
	}

	timep = time(&timeq);
	timeq += 8 * 60 * 60;
	resultLoadReq.testtime = timeq;

	CString strTemp;
	wchar_t *p;

	if (SOAP_OK == ClientSoap.SfcTestResult_USCOREUpload_(&resultLoadReq, resultLoadRer))
	{
		// ������ϵͳͨ�ųɹ�
		strResult = resultLoadRer.SfcTestResult_USCOREUploadResult->anyType[0];
		p = MulityByteToWideChar(resultLoadRer.SfcTestResult_USCOREUploadResult->anyType[1]);
		strReson = p;
		//strReport.Format("SSN���Խ���ϴ���%s", strReson);
		if (strResult.CompareNoCase("PASS") == 0)
		{
			// ϵͳ�ж��ϴ�����ɹ�					
			return TRUE;
		}
		else
		{
			// ϵͳ�ж��ϴ����ʧ��
			strTemp.Format(_T("�ϴ�ʧ�ܣ�%s"), strReson);
			AfxMessageBox(strTemp);
			return FALSE;
		}

		delete p;
	}
	else
	{
		// ������ϵͳͨ��ʧ��
		AfxMessageBox(_T("���Խ���ϴ�--ͨ��ʧ��"));
		return FALSE;
	}

}


BOOL CW1023����Dlg::UploadTestResult()
{
	CString strStation;

	if (m_checkUploadMes.GetCheck() != BST_CHECKED) {
		return TRUE;
	}

	m_strResult.GetStation(strStation);

	if (0 == strStation.CompareNoCase(SWEEP_STATION)) {
		if (FALSE == UploadItem(TEST_ITEM_PLAY)) {
			goto fail;
		}
		//if (FALSE == UploadItem(TEST_ITEM_TOTAL)) {
		//	goto fail;
		//}
	}
	else if (0 == strStation.CompareNoCase(PACK_STATION)){
		if (FALSE == UploadItem(TEST_ITEM_POWER)) {
			goto fail;
		}
		//if (FALSE == UploadItem(TEST_ITEM_TOTAL)) {
		//	goto fail;
		//}
	}
	else if (0 == strStation.CompareNoCase(FUNC_STATION)) {
		if (FALSE == UploadItem(TEST_ITEM_WIFI)) {
			goto fail;
		}
		if (FALSE == UploadItem(TEST_ITEM_KEY)) {
			goto fail;
		}
		if (FALSE == UploadItem(TEST_ITEM_LED)) {
			goto fail;
		}
		if (FALSE == UploadItem(TEST_ITEM_CARD)) {
			goto fail;
		}
		if (FALSE == UploadItem(TEST_ITEM_PLAY)) {
			goto fail;
		}
		//if (FALSE == UploadItem(TEST_ITEM_TOTAL)) {
		//	goto fail;
		//}
	}
	else {
		goto succ;
	}

succ:
	return TRUE;
fail:
	return FALSE;
}


BOOL CW1023����Dlg::SNCheck()
{
	int nErrCode;
	CString strResult;
	CString strReson;
	CString strReport;
	CString strStation;
	CString strSn;
	SFCWebServiceSoapProxy ClientSoap;
	_ns1__CheckSSN_USCORENEW checkSsnReq;
	_ns1__CheckSSN_USCORENEWResponse checkSsnRer;

	if (m_checkUploadMes.GetCheck() != BST_CHECKED) {
		return TRUE;
	}

	m_strResult.GetStation(strStation);
	m_strResult.GetSN(strSn);

	checkSsnReq.strSN = strSn.GetBuffer(0);
	checkSsnReq.station = strStation.GetBuffer(0);

	nErrCode = ClientSoap.CheckSSN_USCORENEW(&checkSsnReq, checkSsnRer);
	if (SOAP_OK == nErrCode)
	{
		// У��SN���Ӷ˿ڳɹ�
		strResult = checkSsnRer.CheckSSN_USCORENEWResult->anyType[0];
		if (strResult.CompareNoCase("PASS") == 0)
		{
			//pStatus->SetWindowTextA("SNУ��ɹ���");
			//AfxMessageBox(_T("SN Check�ɹ�"));
			return TRUE;
		}
		else
		{
			// У��SNʧ�ܣ�����ʧ��ԭ���˳��ϴ�MESϵͳ
			strReson = MulityByteToWideChar(checkSsnRer.CheckSSN_USCORENEWResult->anyType[1]);
			//strReport.Format("SFCУ��SNʧ�ܣ�%s", strReson);
			//AfxMessageBox(strReport);
			if (-1 != strReson.Find(_T("ASSEMBLY1"))) {
				AfxMessageBox(_T("�����װ1վ"));
			}
			else if (-1 != strReson.Find(_T("ASSEMBLY2"))) {
				AfxMessageBox(_T("�����װ2վ"));
			}
			else if (-1 != strReson.Find(_T("SWEEP"))) {
				AfxMessageBox(_T("���ɨƵվ"));
			}
			else if (-1 != strReson.Find(_T("FUNCTION TEST"))) {
				AfxMessageBox(_T("��⹦�ܲ���վ"));
			}
			else if (-1 != strReson.Find(_T("CURVE TEST"))) {
				AfxMessageBox(_T("���SoundCheckվ"));
			}
			else if (-1 != strReson.Find(_T("E/A-TEST"))) {
				AfxMessageBox(_T("����������վ"));
			}
			else if (-1 != strReson.Find(_T("E/A-TEST"))) {
				AfxMessageBox(_T("�����ۼ��վ"));
			}
			else if (-1 != strReson.Find(_T("SCAN CONUTRY"))) {
				AfxMessageBox(_T("���SCAN CONUTRYվ"));
			}
			else if (-1 != strReson.Find(_T("PACKCARTON"))) {
				AfxMessageBox(_T("����װվ"));
			}
			else if (-1 != strReson.Find(_T("PALLETIZATION"))) {
				AfxMessageBox(_T("������վ"));
			}
			else if (-1 != strReson.Find(_T("connect failed in tcp_connect"))) {
				AfxMessageBox(_T("������������"));
			}
			else {
				AfxMessageBox(_T("SN��ⲻͨ��"));
			}
			return FALSE;
		}
	}
	else
	{
		// У��SNʧ�ܣ�ͨ��ʧ��
		std::string fault;
		fault = ClientSoap.soap_fault_detail();
		CString strTemp;
		strTemp = fault.c_str();
		//strTemp.Format("У��SNʧ�ܣ�ͨ��ʧ�ܣ�%d", nErrorCode);
		AfxMessageBox(strTemp);
		//pStatus->SetWindowTextA(strTemp);
		//EventReset.SetEvent();
		return FALSE;
	}
}


BOOL CW1023����Dlg::SNSave()
{
	CString strStation;
	CString strSn;
	CString strReson;
	CString strReport;
	SFCWebServiceSoapProxy ClientSoap;
	_ns1__SfcTestResult_USCOREUpload resultLoadReq;
	_ns1__SfcTestResult_USCOREUploadResponse resultLoadRer;

	_ns1__SaveSSN_USCORENEW saveSsnReq;
	_ns1__SaveSSN_USCORENEWResponse saveSsnRer;

	if (m_checkUploadMes.GetCheck() != BST_CHECKED) {
		return TRUE;
	}

	m_strResult.GetStation(strStation);
	m_strResult.GetSN(strSn);

	saveSsnReq.strEventPoint = strStation.GetBuffer(0);
	saveSsnReq.strSSN = strSn.GetBuffer(0);
	saveSsnReq.strScanner = "SZSFC1";
	saveSsnReq.strIspass = "PASS";
	saveSsnReq.strFailcode = "";

	if (SOAP_OK == ClientSoap.SaveSSN_USCORENEW_(&saveSsnReq, saveSsnRer))
	{
		// ��վ������ϵͳͨ�ųɹ�
		CString strResult = saveSsnRer.SaveSSN_USCORENEWResult->anyType[0];
		if (strResult.CompareNoCase("PASS") == 0)
		{
			// ��վ�ɹ���һ��OK��
			strReson = MulityByteToWideChar(saveSsnRer.SaveSSN_USCORENEWResult->anyType[1]);
			strReport.Format("SSN��վ�ɹ���%s", strReson);
			//AfxMessageBox(strReport, MB_OK);
			//m_staStatus.SetWindowText("log�ļ������ɹ����ϴ�MESϵͳ�ɹ���");
			return  TRUE;
		}
		else
		{
			// ��վʧ�ܣ�����ʧ��ԭ���˳��ϴ�MESϵͳ
			strReson = MulityByteToWideChar(saveSsnRer.SaveSSN_USCORENEWResult->anyType[1]);
			strReport.Format("SSN��վʧ�ܣ�%s", strReson);
			AfxMessageBox(strReport, MB_OK);
			//m_staStatus.SetWindowText("�ϴ�MESϵͳ����վʧ�ܣ�");
			return FALSE;
		}
	}
	else
	{
		// ���ʾ��վ������ϵͳͨ��ʧ��
		AfxMessageBox("��վʧ�ܣ�ͨ��ʧ�ܣ�", MB_OK);
		//m_staStatus.SetWindowText("�ϴ�MESϵͳ��ͨ��ʧ�ܣ�");
		return FALSE;
	}
}



char FirstDriveFromMask(ULONG unitmask) //�@ȡ�P��
{
	char i;
	for (i = 0; i < 26; ++i)
	{
		if (unitmask & 0x1)
			break;
		unitmask = unitmask >> 1;
	}
	return (i + 'A');
}


BOOL CW1023����Dlg::CreateAdbEnableFile(CString &strPath)
{
	CString strFile;
	CFile cfFile;
	//LPCTSTR s = info->GetBuffer(info->GetLength());
	BOOL bFlag;

	//bFlag = PathFileExists(_T("E:\\W-1023����������Լ�¼.txt"));

	strFile.Format(_T("%s:jl_enter_factory_mode_file_2018_4_23"), strPath);
	bFlag = PathFileExists(strFile);
	if (bFlag == TRUE) {
		return TRUE;
	}

	//AfxMessageBox(_T("�Ѿ���U�̴����ļ�ljl_enter_factory_mode_file_2018_4_23����ȷ��"));

	if (!cfFile.Open(strFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)) {
		return FALSE;
	}

	cfFile.Close();

	return TRUE;
}


#if 0
LRESULT CW1023����Dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	CString decDriver;

	switch (message)
	{
		case WM_DEVICECHANGE:
		{
			PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)(lParam);
			switch (wParam)
			{
				case DBT_DEVICEARRIVAL://�豸�����������ҿ���ʹ��
				{
					if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {//�߼���
						PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
						switch (lpdbv->dbcv_flags) {
							case 0://U��
							{
								//CString decDriver;
								decDriver = FirstDriveFromMask(lpdbv->dbcv_unitmask);
								if (FALSE == CreateAdbEnableFile(decDriver)) {
									//AfxMessageBox("U��д��������ļ�ʧ��");
								}
							}
							break;
							case DBTF_MEDIA://����
								break;
						}
					}
				}

				case DBT_DEVICEREMOVECOMPLETE://�豸ж�ػ��߰γ�
				{
					if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)//�߼���
					{
						PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
						switch (lpdbv->dbcv_flags)
						{
						case 0:                //U��a
						{
							//CString decDriver;
							decDriver = FirstDriveFromMask(lpdbv->dbcv_unitmask);
						}
						break;
						case DBTF_MEDIA:    //����
							break;
						}
					}
				}
				break;
			}

		}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}
#endif





#if 0
void CW1023����Dlg::TermMainThread()
{
	if (TRUE == m_bMainThreadLive) {
		TerminateThread(m_pctMainThread->m_hThread, 0);
		CloseHandle(m_pctMainThread->m_hThread);
	}
}
#endif

void CW1023����Dlg::ButtonInit()
{
	/*����fail��Ϣû�б���λ����ʾû��fail*/
	m_nKeyTestFailFlag = FALSE;
	

	/*ɨƵ����ѭ����־λ��λ����ѭ��*/
	m_nLiveFlag = TRUE;
	m_bSweepFlag = TRUE;

	m_buttonSweepPass.EnableWindow(FALSE);
	m_buttonSweepFail.EnableWindow(FALSE);

	m_buttonKeyTest.EnableWindow(FALSE);

	m_buttonKnobSucc.EnableWindow(FALSE);
	m_buttonKnobFail.EnableWindow(FALSE);
}

void CW1023����Dlg::OnLogin()
{
	// TODO: �ڴ���������������
	//AfxMessageBox(_T("��¼"));
	CLoginDlg cLogin(this);
	INT_PTR nResponse;

	//CString a(_T("abel"));

	//AfxMessageBox(a);

	nResponse = cLogin.DoModal();
	if (IDOK != nResponse) {
		m_bUserRoot = FALSE;
		m_bPassEATest = FALSE;
		OtherUserConf();
	}
	else {
		m_bUserRoot = TRUE;
		RootUserConf();
	}
}


void CW1023����Dlg::OtherUserConf()
{
	int nSel;

	//m_cmMenu.EnableMenuItem(ID_WIFI_SETTING, TRUE);
	m_cmMenu.EnableMenuItem(ID_SD_ARGUMENT_SETTING, TRUE);
	m_cmMenu.EnableMenuItem(ID_LOG_SETTING, TRUE);
	m_cmMenu.EnableMenuItem(ID_SN_PASS_SETTING, TRUE);

	m_comboTestItem.EnableWindow(FALSE);

	m_checkKeyTest.EnableWindow(FALSE);
	m_checkLedTest.EnableWindow(FALSE);
	m_checkWifiTest.EnableWindow(FALSE);
	m_checkPlayTest.EnableWindow(FALSE);
	m_checkRecordTest.EnableWindow(FALSE);
	m_checkPowerTest.EnableWindow(FALSE);
	m_checkSdCardTest.EnableWindow(FALSE);
	m_buttonPushFile.EnableWindow(FALSE);
	m_buttonFileChoose.EnableWindow(FALSE);
	m_checkUploadMes.EnableWindow(FALSE);
	m_editFileName.SetReadOnly(TRUE);
	m_editSsid.SetReadOnly(TRUE);
	m_editPasswd.SetReadOnly(TRUE);

	/*��������*/
	m_editMbpsMax.SetReadOnly(TRUE);
	m_editMbpsMin.SetReadOnly(TRUE);
	m_editMbpsMax2.SetReadOnly(TRUE);
	m_editMbpsMin2.SetReadOnly(TRUE);
	m_editRssiMax.SetReadOnly(TRUE);
	m_editRssiMin.SetReadOnly(TRUE);
	m_editPowerTarget.SetReadOnly(TRUE);
	m_editPowerVari.SetReadOnly(TRUE);

	nSel = m_comboTestItem.GetCurSel();
	SaveConfig(nSel);
}


void CW1023����Dlg::RootUserConf()
{
	int nSel;

	//m_cmMenu.EnableMenuItem(ID_WIFI_SETTING, FALSE);
	m_cmMenu.EnableMenuItem(ID_SD_ARGUMENT_SETTING, FALSE);
	m_cmMenu.EnableMenuItem(ID_LOG_SETTING, FALSE);
	m_cmMenu.EnableMenuItem(ID_SN_PASS_SETTING, FALSE);

	nSel = m_comboTestItem.GetCurSel();
	m_comboTestItem.EnableWindow(TRUE);
	m_checkUploadMes.EnableWindow(TRUE);
	ChooseTestItem(nSel, TRUE);
}




void CW1023����Dlg::OnSdArgumentSetting()
{
	// TODO: �ڴ���������������
	
	INT_PTR nResponse;
	CSDSettingPage setPage(this);

	nResponse = setPage.DoModal();
	if (IDOK == nResponse) {
		SaveSdTestConfig();
	}

}


void CW1023����Dlg::SetSdReadStdVal(CString &strVal)
{
	m_strReadStdVal = strVal;
}


void CW1023����Dlg::SetSdWriteStdVal(CString &strVal)
{
	m_strWriteStdVal = strVal;
}


void CW1023����Dlg::GetSdReadStdVal(CString &strTemp)
{
	strTemp = m_strReadStdVal;
}

void CW1023����Dlg::GetSdWriteStdVal(CString &strTemp)
{
	strTemp = m_strWriteStdVal;
}


void CW1023����Dlg::GetLogPath(CString &strTemp)
{
	strTemp = m_strLogPath;
}

void CW1023����Dlg::SetLogPath(CString &strVal)
{
	m_strLogPath = strVal;
}


/******************����ȽϺ������ȽϽ�������㵥λ*******************/
int CW1023����Dlg::fIsLessThanStd(CString &strStd, CString &strVal)
{
	float fStd, fVal;

	fStd = TTOF(strStd);
	fVal = TTOF(strVal);

	if (fVal < fStd) {
		return -1;
	}
	else if(fVal > fStd){
		return 1;
	}
	else {
		return 0;
	}
}


int CW1023����Dlg::fIsLessThanStd(float fStd, float fVal)
{
	if (fVal < fStd) {
		return -1;
	}
	else if (fVal > fStd) {
		return 1;
	}
	else {
		return 0;
	}
}


int CW1023����Dlg::fIsLessThanStd(CString &strStd, float fVal)
{
	float fStd;

	fStd = TTOF(strStd);
	if (fVal < fStd) {
		return -1;
	}
	else if (fVal > fStd) {
		return 1;
	}
	else {
		return 0;
	}
}


/******************����ȽϺ������ȽϽ�������㵥λ*******************/
BOOL CW1023����Dlg::fIsInStd(CString &strStdMax, CString &strStdMin, CString &strVal)
{
	int nRet;

	nRet = fIsLessThanStd(strStdMax, strVal);
	if (nRet > 0) {
		return FALSE;
	}

	nRet = fIsLessThanStd(strStdMin, strVal);
	if (-1 == nRet) {
		return FALSE;
	}

	return TRUE;
}


BOOL CW1023����Dlg::MbpsTestJudge(CString &strStd, CString &strVal)
{
	int nPos;
	float fVal;

	fVal = TTOF(strVal);

	nPos = strVal.Find(_T("Mbit"));
	if (-1 != nPos) {
		goto judge;
	}

	nPos = strVal.Find(_T("Kbit"));
	if (-1 != nPos) {
		fVal = fVal / 1000;
		goto judge;
	}

	nPos = strVal.Find(_T("Gbit"));
	if (-1 != nPos) {
		fVal = fVal * 1000;
		goto judge;
	}

	return FALSE;

judge:
	nPos = fIsLessThanStd(strStd, fVal);
	if (nPos < 0) {
		return FALSE;
	}

	return TRUE;
}

void CW1023����Dlg::OnLogSetting()
{
	// TODO: �ڴ���������������

	INT_PTR nResponse;
	CLogConfigPage logPage(this);

	nResponse = logPage.DoModal();
	if (IDOK == nResponse) {
		SaveLogConfig();
	}
}


void CW1023����Dlg::OnSnPassSetting()
{
	int nRet;
	// TODO: �ڴ���������������
	nRet = AfxMessageBox(_T("ֱ�ӹ�MESϵͳE/A\n����վ����ע����Ҫ\n��ѡ�ϴ�MESϵͳ"), MB_OKCANCEL|MB_ICONEXCLAMATION);
	if (nRet != IDOK) {
		m_bPassEATest = FALSE;
	}
	else {
		m_bPassEATest = TRUE;
		m_comboTestItem.SetCurSel(2);
		ChooseTestItem(2);
	}
}


void CW1023����Dlg::OnCbnDropdownTestitemCombo()
{
	int nSel;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	nSel = m_comboTestItem.GetCurSel();
	SaveConfig(nSel);
}


void CW1023����Dlg::OnBnClickedKnobSuccButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bKnobFlag = TRUE;
	m_ceKnobEvent.SetEvent();
}


void CW1023����Dlg::OnBnClickedKnobFailButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bKnobFlag = FALSE;
	m_ceKnobEvent.SetEvent();
}


UINT CW1023����Dlg::iperfStartProc(LPVOID lpParam)
{
	CW1023����Dlg *p;

	p = (CW1023����Dlg *)lpParam;

	p->StrarIperf3();

	AfxMessageBox(_T("iperf�߳��Ѿ��˳�"));

	return 0;
}

void CW1023����Dlg::StrarIperf3()
{
	STARTUPINFO StartupInfo;

	StartupInfo.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&StartupInfo);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	StartupInfo.wShowWindow = SW_HIDE;

	if (CreateProcess(NULL, _T("iperf3 -s -p 8888 -i 5"), NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &m_ProcessInfo)) {
		WaitForSingleObject(m_ProcessInfo.hProcess, INFINITE);
		CloseHandle(m_ProcessInfo.hThread);
		CloseHandle(m_ProcessInfo.hProcess);
	}
	else{
		AfxMessageBox(_T("����Iperf3����ʧ��"));
		//int error = GetLastError();
	}
}

void CW1023����Dlg::StopIperf3()
{
	TerminateProcess(m_ProcessInfo.hProcess, 0);
}



BOOL CW1023����Dlg::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	StopIperf3();
	return CDialogEx::DestroyWindow();
}
