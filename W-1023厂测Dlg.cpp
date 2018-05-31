
// W-1023厂测Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "W-1023厂测.h"
#include "W-1023厂测Dlg.h"
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


UINT CW1023厂测Dlg::MainThreadProc(LPVOID lpParam)
{
	CW1023厂测Dlg *pMainDlg;
	CString strSsid;
	CString strPasswd;

	pMainDlg = (CW1023厂测Dlg *)lpParam;
	//pMainDlg->m_bMainThreadLive = TRUE;

	/*点击开始测试将会设置事件，*/
	while (1)
	{
		WaitForSingleObject(pMainDlg->m_ceSNCheckEvent.m_hObject, INFINITE);
		pMainDlg->m_ceSNCheckEvent.ResetEvent();

		if (pMainDlg->m_checkWifiTest.GetCheck() == BST_CHECKED) {
			pMainDlg->m_editSsid.GetWindowText(strSsid);
			if (strSsid.IsEmpty()) {
				pMainDlg->m_editTestState.SetWindowText(_T("SSID设置为空"));
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


UINT CW1023厂测Dlg::SNCheckThreadProc(LPVOID lpParam)
{
	CW1023厂测Dlg *pMainDlg;
	CString strSn;

	pMainDlg = (CW1023厂测Dlg *)lpParam;

	while (1)
	{
		WaitForSingleObject(pMainDlg->m_ceStartEvent.m_hObject, INFINITE);
		pMainDlg->m_ceStartEvent.ResetEvent();
		pMainDlg->ResetMainTestDlg();/*清空前次测试相关信息*/
		pMainDlg->m_editSN.GetWindowText(strSn);
		if (strSn.IsEmpty()) {
			AfxMessageBox(_T("请输入S/N"));
			continue;
		}
		else {
			/*发送SN扫描事件，以此为测试开始的标志*/
			pMainDlg->m_ceSNCheckEvent.SetEvent();
		}

		Sleep(10);
	}

	return 0;
}


// CW1023厂测Dlg 对话框
CW1023厂测Dlg::CW1023厂测Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_W1023_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bUserRoot = FALSE;
	m_bPassEATest = FALSE;
}


void CW1023厂测Dlg::DoDataExchange(CDataExchange* pDX)
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

BEGIN_MESSAGE_MAP(CW1023厂测Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_TESTITEM_COMBO, &CW1023厂测Dlg::OnCbnSelchangeTestitemCombo)
	ON_BN_CLICKED(IDCANCEL, &CW1023厂测Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_START_TEST_BUTTON, &CW1023厂测Dlg::OnBnClickedStartTestButton)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_KEY_FAIL_BUTTON, &CW1023厂测Dlg::OnBnClickedKeyFailButton)
	ON_BN_CLICKED(IDC_SWEEP_SUC_BUTTON, &CW1023厂测Dlg::OnBnClickedSweepSucButton)
	ON_BN_CLICKED(IDC_SWEEP_FAIL_BUTTON, &CW1023厂测Dlg::OnBnClickedSweepFailButton)
	ON_COMMAND(ID_LOGIN, &CW1023厂测Dlg::OnLogin)
	ON_BN_CLICKED(IDC_FILE_CHOSE_BUTTON, &CW1023厂测Dlg::OnBnClickedFileChoseButton)
	ON_COMMAND(ID_SD_ARGUMENT_SETTING, &CW1023厂测Dlg::OnSdArgumentSetting)
	ON_COMMAND(ID_LOG_SETTING, &CW1023厂测Dlg::OnLogSetting)
	ON_COMMAND(ID_SN_PASS_SETTING, &CW1023厂测Dlg::OnSnPassSetting)
	ON_CBN_DROPDOWN(IDC_TESTITEM_COMBO, &CW1023厂测Dlg::OnCbnDropdownTestitemCombo)
	ON_BN_CLICKED(IDC_KNOB_SUCC_BUTTON, &CW1023厂测Dlg::OnBnClickedKnobSuccButton)
	ON_BN_CLICKED(IDC_KNOB_FAIL_BUTTON, &CW1023厂测Dlg::OnBnClickedKnobFailButton)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


BOOL CW1023厂测Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetFocus()->GetDlgCtrlID() == IDC_SN_EDIT)//按下回车，如果当前焦点是在自己期望的控件上
		{
			// 你想做的事，如果按下回车时焦点在你想要的控件上
			OnBnClickedStartTestButton();
		}
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}



// CW1023厂测Dlg 消息处理程序

BOOL CW1023厂测Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	m_cmMenu.LoadMenuA(IDR_MENU1);
	SetMenu(&m_cmMenu);

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	/*1. 添加测试选项的选择*/
	m_comboTestItem.AddString(_T("扫频测试"));
	m_comboTestItem.AddString(_T("功能测试"));
	m_comboTestItem.AddString(_T("电量测试"));

	/*2. 测试选项默认选择扫频测试*/
	GetTestItemConf();
	int nSel = m_comboTestItem.GetCurSel();
	m_comboTestItem.EnableWindow(TRUE);
	ChooseTestItem(nSel);

	/*****改变测试结果的字体****/
	CFont *ptf = m_editTestResult.GetFont();
	LOGFONT lf;
	ptf->GetLogFont(&lf);
	lf.lfHeight = 50;
	lf.lfWidth = 20;
	lf.lfOrientation = 30;
	lf.lfItalic = TRUE;
	lf.lfWeight = FW_BOLD;
	strcpy_s(lf.lfFaceName, _T("隶书"));
	m_fontFont.CreateFontIndirectA(&lf);
	m_editTestResult.SetFont(&m_fontFont);

	/*初始化格式刷*/
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

	/*初始启动固定到其它用户模式，防止更改*/
	OtherUserConf();

	/*3. 设置初始的测试状态和测试结果*/
	ResetMainTestDlg();
	SetTotalTestResult(2);
	/*5. SN处理线程*/
	m_pctSNCheckThread = AfxBeginThread(
		SNCheckThreadProc, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (NULL == m_pctSNCheckThread) {
		AfxMessageBox(_T("创建SN Check线程失败"), MB_OK);
		return FALSE;
	}
	m_pctSNCheckThread->ResumeThread();

#if 1
	/*4. 开处理线程*/
	m_pctMainThread = AfxBeginThread(
		MainThreadProc, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (NULL == m_pctMainThread) {
		AfxMessageBox(_T("创建主测试线程失败"), MB_OK);
		return FALSE;
	}
	m_pctMainThread->ResumeThread();
#endif

	m_pctIprefThread = AfxBeginThread(
		iperfStartProc, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (NULL == m_pctIprefThread) {
		AfxMessageBox(_T("创建Ipref线程失败"), MB_OK);
		return FALSE;
	}
	m_pctIprefThread->ResumeThread();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CW1023厂测Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CW1023厂测Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CW1023厂测Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CW1023厂测Dlg::OnCbnSelchangeTestitemCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel;
	CString strTestItem;

	/*获取测试选项组合框控件的列表框中的选中项索引*/
	nSel = m_comboTestItem.GetCurSel();
	/*获取当前选择的测试项字符串内容*/
	//m_comboTestItem.GetLBText(nSel, strTestItem);
	/*设置当前选项下可以被选择的部分*/
	ChooseTestItem(nSel);
	//m_ceStartEvent.ResetEvent();
}


void CW1023厂测Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel;
	nSel = m_comboTestItem.GetCurSel();
	SaveConfig(nSel);

	CDialogEx::OnCancel();
}


void CW1023厂测Dlg::OnBnClickedStartTestButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSsid;

	TestStartRest();

	if (m_checkWifiTest.GetCheck() == BST_CHECKED) {
		m_editSsid.GetWindowText(strSsid);
		if (strSsid.IsEmpty()) {
			m_editTestState.SetWindowText(_T("SSID设置为空"));
			AfxMessageBox(_T("请输入SSID和密码"));
			TestEndReset();
			return;
		}
	}
	m_ceStartEvent.SetEvent();

	return;
}


void CW1023厂测Dlg::OnBnClickedKeyFailButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nKeyTestFailFlag = TRUE;
}


void CW1023厂测Dlg::OnBnClickedSweepSucButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSweepFlag = TRUE;
	m_nLiveFlag = FALSE;
	MusicStop();
	//m_ceSweepEvent.SetEvent();
}


void CW1023厂测Dlg::OnBnClickedSweepFailButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nLiveFlag = FALSE;
	m_bSweepFlag = FALSE;
	MusicStop();
	//m_ceSweepEvent.SetEvent();
}


void CW1023厂测Dlg::OnBnClickedFileChoseButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilePath;

	TCHAR szFilter[] = _T("wav文件(*.wav)|*.wav|所有文件(*.*)|*.*||");
	//CFileDialog fileDlg(FALSE, _T("wav"), _T("my"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CFileDialog fileDlg(TRUE, _T("wav"), NULL, 0, szFilter, this);

	if (IDOK == fileDlg.DoModal()) {
		strFilePath = fileDlg.GetPathName();
		m_editFileName.SetWindowTextA(strFilePath);
		m_buttonPushFile.SetCheck(TRUE);
	}
}

/**************************Start：颜色处理函数****************************/
HBRUSH CW1023厂测Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	CString strTemp;
	CString strStdMax;
	CString strStdMin;
	
	// TODO:  在此更改 DC 的任何特性
	switch (pWnd->GetDlgCtrlID())//对某一个特定控件进行判断
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
			pDC->SetBkColor(m_redcolor);		// 设置背景色
												//pDC->SetTextColor(m_textcolor);	// 设置文本颜色
			pDC->SetBkMode(TRANSPARENT);		// 设置背景模式为透明，也就是去掉文字的背景		
			hbr = (HBRUSH)m_redbrush;			// 用画刷填满矩形
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
			if (fIsLessThanStd(m_strReadStdVal, strTemp) < 0) {/*小于标准值*/
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
			if(fIsLessThanStd(m_strWriteStdVal, strTemp) < 0){/*小于标准值*/
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
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
/**************************End：颜色处理函数****************************/


/***********************Start:读取文件名字************************/
BOOL CW1023厂测Dlg::PathName2Name(CString &strPcPathName, CString &strName)
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


BOOL CW1023厂测Dlg::GetFileName(CString &strPathName, CString &strName)
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
/***********************End:读取文件名字************************/


/*************Start:测试状态更新系列***************/
//参数及其含义：nSel--选择的测试模式编号   bFlag--初始化时调用标志，初始化状态下会设置所有被测选项为选中状态
void CW1023厂测Dlg::ChooseTestItem(int nSel, BOOL bFlag)
{
	switch (nSel) {
	case 0://扫频测试
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
	case 1://功能测试
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

		/*测试参数部分*/
		m_editMbpsMax.SetReadOnly(FALSE);
		m_editMbpsMin.SetReadOnly(FALSE);
		m_editMbpsMax2.SetReadOnly(FALSE);
		m_editMbpsMin2.SetReadOnly(FALSE);
		m_editRssiMax.SetReadOnly(FALSE);
		m_editRssiMin.SetReadOnly(FALSE);
		m_editPowerTarget.SetReadOnly(TRUE);
		m_editPowerVari.SetReadOnly(TRUE);
		break;
	case 2://电量测试
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

		/*测试参数部分*/
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


void CW1023厂测Dlg::SetSubLedResult(CString &strRet, int nStation)
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
		AfxMessageBox(_T("LED状态显示出错！"), MB_OK);
		break;
	}
}


void CW1023厂测Dlg::SetLedTestResult(BOOL nResult, int nStation, int nNum)
{
	CString strRetInfo;
	CString strStateInfo;

	/*显示LED测试区的状态*/
	strRetInfo.Format(_T("%s"), (nResult ? _T("PASS") : _T("FAIL")));
	SetSubLedResult(strRetInfo, nStation);

	if ((LED_BLUE_TEST == nStation) || (!nResult)) {
		if (nResult) {
			strStateInfo.Format(_T("LED测试通过！"));
			/*显示主功能选择区的测试状态*/
			m_editLedTestResult.SetWindowText(strRetInfo);
			/*测试状态显示栏显示*/
			m_editTestState.SetWindowText(strStateInfo);
		}
		else {
			strStateInfo.Format(_T("LED[%d]测试失败！"), nNum);
			/*显示主功能选择区的测试状态*/
			m_editLedTestResult.SetWindowText(strRetInfo);
			/*测试状态显示栏显示*/
			m_editTestState.SetWindowText(strStateInfo);
		}
	}
}


void CW1023厂测Dlg::SetSubKeyResult(CString &strRet, int nStation)
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
		AfxMessageBox(_T("按键序号显示出错！"), MB_OK);
		break;
	}
}


void CW1023厂测Dlg::SetKeyTestResult(BOOL nResult, int nStation)
{
	CString strRetInfo;
	CString strStateInfo;

	strRetInfo.Format(_T("%s"), (nResult ? _T("PASS") : _T("FAIL")));

	SetSubKeyResult(strRetInfo, nStation);

	if ((KEY6_TEST/*KEY5_TEST*/ == nStation) || (!nResult)) {
		if (nResult) {
			strStateInfo.Format(_T("按键测试通过！"));
			/*显示主功能选择区的测试状态*/
			//m_editKeyTestResult.SetWindowText(strRetInfo);
			/*测试状态显示栏显示*/
			m_editTestState.SetWindowText(strStateInfo);
		}
		else {
			strStateInfo.Format(_T("按键[%d]测试失败！"), nStation);
			/*测试状态显示栏显示*/
			m_editTestState.SetWindowText(strStateInfo);
		}
	}

}


void CW1023厂测Dlg::SetRssiResult(int nVal)
{
	CString strVal;

	strVal.Format(_T("%d"), nVal);
	m_editRssiReal.SetWindowText(strVal);
}


void CW1023厂测Dlg::SetMbpsResult(int nVal)
{
	CString strVal;

	strVal.Format(_T("%d"), nVal);
	m_editMbpsReal.SetWindowText(strVal);
}


void CW1023厂测Dlg::SetWifiTestResult(BOOL nResult)
{
	CString strRet;
	CString strState;

	strRet.Format(_T("%s"), nResult ? _T("PASS") : _T("FAIL"));
	strState.Format(_T("%s"), nResult ? _T("Wifi测试失败") : _T("Wifi测试成功"));
	m_editWifiTestResult.SetWindowText(strRet);
	m_editTestState.SetWindowText(strState);
}


void CW1023厂测Dlg::SetPlayTestResult(BOOL nResult)
{
	CString strRet;
	CString strState;

	strRet.Format(_T("%s"), nResult ? _T("PASS") : _T("FAIL"));
	strState.Format(_T("%s"), nResult ? _T("播放测试失败") : _T("播放测试成功"));
	m_editPlayTestResult.SetWindowText(strRet);
	m_editTestState.SetWindowText(strState);
}


void CW1023厂测Dlg::SetRecordTestResult(BOOL nResult)
{
	CString strRet;
	CString strState;

	strRet.Format(_T("%s"), nResult ? _T("PASS") : _T("FAIL"));
	strState.Format(_T("%s"), nResult ? _T("录音测试失败") : _T("录音测试成功"));
	m_editRecordTestResult.SetWindowText(strRet);
	m_editTestState.SetWindowText(strState);
}


void CW1023厂测Dlg::SetPowerTestResult(BOOL nResult)
{
	CString strRet;
	CString strState;

	strRet.Format(_T("%s"), nResult ? _T("PASS") : _T("FAIL"));
	strState.Format(_T("%s"), nResult ? _T("电量测试成功") : _T("电量测试成功"));
	m_editPowerTestResult.SetWindowText(strRet);
	m_editTestState.SetWindowText(strState);
}


void CW1023厂测Dlg::SetTotalTestResult(int nResult)
{

	if (0 == nResult) {/*测试成功*/
		m_editTestResult.SetWindowText(_T("PASS"));
	}
	else if(1 == nResult){
		m_editTestResult.SetWindowText(_T("FAIL"));
	}
	else{
		m_editTestResult.SetWindowText(_T("N/A"));
	}
}


/*写入测试结果到Log文件中*/
int CW1023厂测Dlg::LoadTestResultToLog(CString *info)
{
	CFile cfFile;
	LPCTSTR s = info->GetBuffer(info->GetLength());
	//BOOL bFlag;

	if(!cfFile.Open(m_strLogPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)){
		AfxMessageBox(_T("不能打开文件：W-1023厂测软件测试记录.txt！"));
		return -1;
	}

	//if (FALSE == bFlag) {/*确保文件被写入的格式是Unicode格式，中文不乱码*/
	//	WORD unicode = 0xFEFF; 
	//	cfFile.Write(&unicode, 2);  
	//}

	cfFile.SeekToEnd();
	cfFile.Write(_T("\r\n"), sizeof(_T("\r\n")));
	cfFile.Write(s, info->GetLength()*sizeof(TCHAR));
	cfFile.Close();

	return 0;
}


/*清空所有设置信息*/
void CW1023厂测Dlg::ResetMainTestDlg()
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


void CW1023厂测Dlg::TestEndReset()
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


void CW1023厂测Dlg::TestStartRest()
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


void CW1023厂测Dlg::UpdataTestInfo(CString &info)
{
	/*写入测试结果到LOG中*/
	LoadTestResultToLog(&info);
	/*写入测试结果到测试界面的状态中*/
	//AppendTestStateInfo(info);
}
/*************End:测试状态更新系列***************/



/*******************************Start:测试指令函数系列*********************************/
BOOL CW1023厂测Dlg::iAdbExist(BOOL bFlag)
{
	CString strTemp;
	CAdbshell casShell;

#if 0
	if (TRUE == m_bAdbShellLive) {
		if (bFlag == TRUE)
			AfxMessageBox(_T("adb实例已在运行中！"), MB_OK);
		return -1;
	}
	m_bAdbShellLive = TRUE;
#endif

	if (FALSE == casShell.Start())
	//if (FALSE == m_casShell.Start())
	{
		//adb shell执行错误，返回
		if (bFlag == TRUE)
			AfxMessageBox(_T("adb进程启动失败，内存不足！"), MB_OK);
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
			AfxMessageBox(_T("adb错误，请检查USB连接！"), MB_OK);
		return FALSE;
	}

	return TRUE;
}

/*Adb shell命令返回的值是int型*/
int CW1023厂测Dlg::iExecuteAdbCmd(CString &strCmd, BOOL bFlag)
{
	CString strTemp;
	CAdbshell casShell;

#if 0
	if (TRUE == m_bAdbShellLive) {
		if (bFlag == TRUE)
			AfxMessageBox(_T("adb实例已在运行中！"), MB_OK);
		return -1;
	}
	m_bAdbShellLive = TRUE;
#endif
	
	if (FALSE == casShell.Start())
	//if (FALSE == m_casShell.Start())
	{
		//adb shell执行错误，返回
		if(bFlag == TRUE)
			AfxMessageBox(_T("adb进程启动失败，内存不足！"), MB_OK);	
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

	/*用于没有返回值的情况，返回的字符和传入的命令是一样的*/
	if (0 == strTemp.Compare("")) {
		return -1;
	}
	else if (0 == strTemp.Compare(_T("error: no devices/emulators found"))) {
		if (bFlag == TRUE)
			AfxMessageBox(_T("adb错误，请检查USB连接！"), MB_OK);
		return 1;
	}
	else {
		return 0;
	}
}


int CW1023厂测Dlg::iExecuteAdbCmd(CString &strCmd, CString &strRet, BOOL bFlag)
{
	//CString strTemp;
	//CString strRet;
	CAdbshell casShell;

	/*清空buff中遗留数据*/
	strRet.Empty();
	
	//if (FALSE == m_casShell.Start())
	if (FALSE == casShell.Start())
	{
		//adb shell执行错误，返回
		if (bFlag == TRUE)
			AfxMessageBox(_T("adb进程启动失败，内存不足！"), MB_OK);
		//m_bAdbShellLive = FALSE;
		return -1;
	}

	casShell.RunCmd(strCmd);
	casShell.Stop();
	strRet = casShell.GetOutput();

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);
	

	if (0 == strRet.Compare("")) {
		//AfxMessageBox(_T("adb返回结果为空"));
		return -1;
	}
	else if (0 == strRet.Compare(_T("error: no devices/emulators found"))) {
		if (bFlag == TRUE)
			AfxMessageBox(_T("adb错误，请检查USB连接！"), MB_OK);
		strRet = _T("");
		return 1;
	}
	else {
		return 0;
	}
}


int CW1023厂测Dlg::iExecuteShellCmd(CString &strCmd, CString &strRet, BOOL bFlag)
{
	//CString strTemp;
	CCmdshell cmdShell;

	/*清空buff中遗留数据*/
	strRet.Empty();
	if (FALSE == cmdShell.Start(strCmd)) {
		if(TRUE == bFlag)
			AfxMessageBox(_T("进程开始失败！"), MB_OK);
		return -1;
	}

	strRet = cmdShell.GetOutput();
	strRet.ReleaseBuffer();

	//测试用
	//AfxMessageBox(strRet, MB_OK);

	return 0;
}


int CW1023厂测Dlg::iExecuteShellCmdWithTimeout(CString &strCmd, int nTimeout, BOOL bFlag)
{
	//CString strTemp;
	CCmdshell cmdShell;


	if (FALSE == cmdShell.StartWithoutTimeout(strCmd, nTimeout)) {
		if (TRUE == bFlag)
			AfxMessageBox(_T("进程开始失败！"), MB_OK);
		return -1;
	}

	return 0;
}



int CW1023厂测Dlg::FactoryStop()
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
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
		return 1;
	}

	//strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);
	return 0;
}


int CW1023厂测Dlg::SDReadTest()
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
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


int CW1023厂测Dlg::SDWriteTest()
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
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


int CW1023厂测Dlg::WifiConnnect()
{
	CString strCmd;
	CString strRet;
	CString strSsid;
	CString strPasswd;
	int nRet;


	/*获取SSID和Passwd*/
	m_editSsid.GetWindowText(strSsid);
	m_editPasswd.GetWindowText(strPasswd);

	strCmd.Format(_T("%s ATE_WIFI_CONNECT %s %s\n"), FACTORY_LOC, strSsid, strPasswd);
	
	nRet = iExecuteAdbCmd(strCmd, strRet);
	if(nRet < 0){
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


int CW1023厂测Dlg::WifiDisconnect()
{
	CString strCmd;
	CString strRet;
	int nRet;
	//CString strSuccInfo(_T("wifi断开连接成功！"));
	//CString strErrInfo(_T("wifi断开连接失败！"));

	strCmd.Format(_T("%s ATE_WIFI_DISCONNECT\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


//返回值及其含义：  1--ADB出错返回；   0--正常返回；   -1--播放失败
int CW1023厂测Dlg::MusicPlay(CString &filePath)
{
	CString strCmd;
	CString strRet;
	int nRet;

	strCmd.Format(_T("%s ATE_PLAY_FILE %s\n"), FACTORY_LOC, filePath);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		//AfxMessageBox(_T("adb命令执行失败"));
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
		return 1;
	}

	strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		AfxMessageBox(_T("adb结果解析失败"));
		return -1;
	}

	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	return 0;
}


int CW1023厂测Dlg::MusicStop()
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
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


int CW1023厂测Dlg::MicRecordStart(CString &filePath)
{
	CString strCmd;
	CString strRet;
	int nRet;
	//CString strSuccInfo(_T("录音测试开始成功！"));
	//CString strErrInfo(_T("录音测试开始失败！"));

	strCmd.Format(_T("%s ATE_RECORD_START %s\n"), FACTORY_LOC, filePath);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


int CW1023厂测Dlg::MicRecordStop()
{
	CString strCmd;
	CString strRet;
	int nRet;
	//CString strSuccInfo(_T("录音测试停止成功！"));
	//CString strErrInfo(_T("录音测试停止失败！"));

	strCmd.Format(_T("%s ATE_RECORD_STOP\n"), FACTORY_LOC);

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


int CW1023厂测Dlg::RememberTestInfo(CString &strStation, BOOL bFlag)
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


int CW1023厂测Dlg::DisplayTestInfo(CString &strStation)
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


//进入厂测模式
int CW1023厂测Dlg::EnterFactory()
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


/*恢复出厂设置*/
int CW1023厂测Dlg::OutFactory()
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
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


int CW1023厂测Dlg::FactoryTestSuccess()
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
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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
/*恢复出厂设置*/
int CW1023厂测Dlg::FactorySet()
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
		AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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

int CW1023厂测Dlg::GetCurPower(CString &strPower)
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
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


int CW1023厂测Dlg::PushFile(CString &strPath)
{
	int nRet;
	int nPos;
	CString strCmd;
	CString strRet;

	strCmd.Format(_T("adb push \"%s\" /mnt/SDCARD"), strPath);

	//AfxMessageBox(strCmd);
	CString strMsg;

	strMsg.Format(_T("文件-%s-不存在，请检查文件名"), strPath);
	
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
		AfxMessageBox(_T("未发现ADB设备"));
		return -1;
	}

	return 0;
}


int CW1023厂测Dlg::PushFile(CString &strPath, CString &strDest)
{
	int nRet;
	int nPos;
	CString strCmd;
	CString strRet;

	strCmd.Format(_T("adb push \"%s\" %s"), strPath, strDest);

	//AfxMessageBox(strCmd);
	CString strMsg;

	strMsg.Format(_T("文件-%s-不存在，请检查文件名"), strPath);

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
		AfxMessageBox(_T("未发现ADB设备"));
		return -1;
	}

	return 0;
}


int CW1023厂测Dlg::PullFile(CString &strPath)
{
	CString strCmd;
	CString strRet;
	int nPos;

	strCmd.Format(_T("adb pull %s ."), strPath);
	/*未完成*/
	iExecuteShellCmd(strCmd, strRet);

	nPos = strRet.Find(_T("fail"));
	if (nPos != -1) {
		return -1;
	}
	nPos = strRet.Find(_T("error: device not found"));
	if (nPos != -1) {
		AfxMessageBox(_T("未发现ADB设备"));
		return -1;
	}

	return 0;
}


int CW1023厂测Dlg::ChmodFactory()
{
	CString strCmd;
	CString strRet;
	int nRet;

	strCmd.Format(_T("chmod ugo+x %s\n"),  FACTORY_LOC);
	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet > 0) {
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
		return -1;
	}

	//strRet.ReleaseBuffer();
	//AfxMessageBox(strRet, MB_OK);

	return 0;
}

int CW1023厂测Dlg::GetDutIp(CString &strBuf)
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
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


int CW1023厂测Dlg::DutConnectionCheck()
{
	CString strCmd;

	strCmd.Format(_T("%s ATE_DUT_PING\n"), FACTORY_LOC);

	return iExecuteAdbCmd(strCmd);
}


int CW1023厂测Dlg::IprefRecvTest()
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
		AfxMessageBox(_T("创建Ipref接收线程失败"), MB_OK);
		return -1;
	}
	pThread->ResumeThread();
#endif

	nRet = iExecuteAdbCmd(strCmd, strRet);
	if (nRet < 0) {
		return -1;
	}
	else if (nRet > 0) {
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
		return 1;
	}
	
	/*检测返回值*/
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


UINT CW1023厂测Dlg::iprefRecvProc(LPVOID lpParam)
{
	CString strPcCmd;
	CString strRet;
	CW1023厂测Dlg *p;
	int nRet;
	int nPos;

	p = (CW1023厂测Dlg *)lpParam;
	Sleep(5000);
	strPcCmd.Format(_T("iperf3 -c %s -t 15 -p 8888 -i 5"), p->m_strIp);
	//strPcCmd.Format(_T("iperf3 -c 192.168.123.123 -t 15 -p 8888 -i 5"));
	nRet = p->iExecuteShellCmd(strPcCmd, strRet);
	if (nRet != 0) {
		AfxMessageBox(_T("启动ipref客服端失败"), MB_OK);
	}
	//AfxMessageBox(strRet);

	strRet.ReleaseBuffer();
	nPos = strRet.Find(_T("iperf3: error"));
	if (-1 != nPos) {
		return -1;/*出错*/
	}

	return 0;
}


int CW1023厂测Dlg::IprefSendTest(int nPort)
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
		AfxMessageBox(_T("创建Ipref发送线程失败"), MB_OK);
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
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
		return 1;
	}

	/*检测返回值*/
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


UINT CW1023厂测Dlg::iprefSendProc(LPVOID lpParam)
{
	int nRet;
	CW1023厂测Dlg *p;
	CString strRet;
	CString strPcCmd(_T("iperf3 -s -p 8888 -i 5 -1"));
	//CString strPcCmd(_T("iperf3 -s -p 8888 -i 5"));

	p = (CW1023厂测Dlg *)lpParam;

	nRet = p->iExecuteShellCmd(strPcCmd, strRet);
	if (nRet != 0) {
		AfxMessageBox(_T("启动ipref服务端失败"), MB_OK);
	}

	//AfxMessageBox(strRet);
	return 0;
}


int CW1023厂测Dlg::LedTestSubBlue()
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
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


int CW1023厂测Dlg::LedTestSubGreen()
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
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


int CW1023厂测Dlg::LedTestSubRed()
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
		//AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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


int CW1023厂测Dlg::SubKeyTest()
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
		AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
		return -1;
	}

	Json::Reader jsReader;
	Json::Value jsValue;
	std::string str = CString(strRet);
	if (!jsReader.parse(str, jsValue))
	{
		return -1;
	}

	/*检测按键返回是否OK*/
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


int CW1023厂测Dlg::NightKeyTest()
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
		AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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

	/*检测按键返回是否OK*/
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


int CW1023厂测Dlg::ResetKeyTest()
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
		AfxMessageBox(_T("ADB出错，请检查USB连接！"), MB_OK);
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

	/*检测按键返回是否OK*/
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


int CW1023厂测Dlg::WifiRssiTest()
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

	/*检测按键返回是否OK*/
	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	val = _T(jsValue["codeMsg"].asCString());
	if (0 == val.Compare(_T("RESULT_FILE_NOT_EXIST"))) {
		m_editRssiReal.SetWindowText(_T(""));
		AfxMessageBox(_T("测试WIFI强度请插入SD卡"));
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


int CW1023厂测Dlg::GetFirmwareVer()
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

	/*检测按键返回是否OK*/
	CString val = _T(jsValue["result"].asCString());
	if (0 != val.Compare(_T("OK"))) {
		return -1;
	}

	val = _T(jsValue["codeMsg"].asCString());
	m_editSoftwareVersion.SetWindowText(val);

	return 0;
}


void CW1023厂测Dlg::PowerDischarge()
{
	CString strCmd;

	CString strPcCmd;

	strPcCmd.Format(_T("%s ATE_DISCHARGE"), FACTORY_LOC);
	
	/*需要执行PC端指令*/
	iExecuteAdbCmd(strPcCmd);

	return;
}


int CW1023厂测Dlg::AdbSampleTest()
{
	CString strCmd;
	CString strRet;
	CString strSuccInfo(_T("AdbSampleTest执行成功！"));
	CString strErrInfo(_T("AdbSampleTest执行失败！"));

	//strCmd.Format(_T("cd bin"));
	CString strPcCmd(_T("iperf3 -c IP -t 10 -p 35000"));
	//CString strPcCmd(_T("iperf3 -s -t 10 -p 35000"));
	/*需要执行PC端指令*/
	iExecuteShellCmd(strPcCmd, strRet);
	//return iExecuteAdbCmd(strCmd, strSuccInfo, strErrInfo);
	return 0;
}

/*******************************End:测试指令函数系列*********************************/



/***************************Start:测试项函数***************************/
//返回值及其意义   1:被终止测试； 0:所有测试完成，正常退出； -1:测试出错； -2：异常退出
int CW1023厂测Dlg::LedTest()
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
	if (0 != nRet) {/*测试失败*/
		m_editLedRed.SetWindowText(_T("FAIL"));
		m_editTestState.SetWindowText(_T("LED红灯测试失败"));
		goto fail;
	}
		
	nResponse = msg1.DoModal();
	if (IDOK != nResponse) {
		m_editLedRed.SetWindowText(_T("FAIL"));
		m_editTestState.SetWindowText(_T("LED红灯测试失败"));
		goto fail;
	}
	m_editLedRed.SetWindowText(_T("PASS"));
		
	m_editLedGreen.SetWindowText(_T("..."));
	nRet = LedTestSubGreen();
	if (0 != nRet) {/*测试失败*/
		m_editLedGreen.SetWindowText(_T("FAIL"));
		m_editTestState.SetWindowText(_T("LED绿灯测试失败"));
		goto fail;
	}
	nResponse = msg2.DoModal();
	if (IDOK != nResponse) {
		m_editLedGreen.SetWindowText(_T("FAIL"));
		m_editTestState.SetWindowText(_T("LED绿灯测试失败"));
		goto fail;
	}
	m_editLedGreen.SetWindowText(_T("PASS"));
		
	m_editLedBlue.SetWindowText(_T("..."));
	nRet = LedTestSubBlue();
	if (0 != nRet) {/*测试失败*/
		m_editLedBlue.SetWindowText(_T("FAIL"));
		m_editTestState.SetWindowText(_T("LED蓝灯测试失败"));
		goto fail;
	}
	nResponse = msg3.DoModal();
	if (IDOK != nResponse) {
		m_editLedBlue.SetWindowText(_T("FAIL"));
		m_editTestState.SetWindowText(_T("LED蓝灯测试失败"));
		goto fail;
	}
	m_editLedBlue.SetWindowText(_T("PASS"));

	m_editLedTestResult.SetWindowText(_T("PASS"));
	return 0;
fail:
	m_editLedTestResult.SetWindowText(_T("FAIL"));
	return -1;
}


void CW1023厂测Dlg::KeyPrepareTestDisplay()
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
		AfxMessageBox(_T("按键准备测试显示动作失败！"));
		break;
	}
}


//返回值及其意义   1:被终止测试； 0:所有测试完成，测试正常； -1:所有测试完成，测试失败；
int CW1023厂测Dlg::KeyTest()
{
	int nRet;
	//int nStation;
	CString msg;
	m_nStation = KEY_TEST;

	KeyPrepareTestDisplay();
	m_editTestState.SetWindowText(_T("正在进行按键测试..."));
	AfxMessageBox(_T("请依次按键‘消息’，‘上一\n曲’，‘语音’，‘下一曲’，‘菜\n单’，‘夜灯’,‘Reset’键，如\n某一按键不能自动检测通\n过，请按'按键功能不良'键"));
	m_nKeyTestFailFlag = FALSE;
	m_buttonKeyTest.EnableWindow(TRUE);
	/*检测USB连接是否OK，后边不再提示*/
	//iAdbExist();

	while (m_nLiveFlag && (m_nKeyTestFailFlag == FALSE))
	{
		++m_nStation;
		KeyPrepareTestDisplay();
		nRet = SubKeyTest();
		msg.Format(_T("%d"), nRet);
		//AfxMessageBox(msg);
		if ((-1 == nRet) || (0 == nRet)) {/*失败不做任何动作直接重复*/
			SetKeyTestResult(FALSE, m_nStation);
			goto fail;
		}
		if (nRet != (m_nStation - KEY_TEST)) {/*获取键值不对*/
			--m_nStation;
			if (m_nKeyTestFailFlag == FALSE) {
				AfxMessageBox(_T("所按键不对，请重按"));
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

	//AfxMessageBox(_T("调试标志1"));

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
	m_editTestState.SetWindowText(_T("按键测试成功..."));
	m_buttonKeyTest.EnableWindow(FALSE);
	return 0;
fail:
	m_editKeyTestResult.SetWindowText(_T("FAIL"));
	m_editTestState.SetWindowText(_T("按键测试失败..."));
	m_buttonKeyTest.EnableWindow(FALSE);
	return -1;
}


int CW1023厂测Dlg::WifiTest()
{
	int nRet;
	int nCycel;
	CString strIp;
	m_nStation = WIFI_TEST;

	/*清空m_strIp中的内容*/
	m_strIp.Empty();

	/*wifi测试结果显示测试中*/
	m_editWifiTestResult.SetWindowText(_T("..."));
	m_editTestState.SetWindowText(_T("正在连接网络..."));

	/*1. 连接wifi*/
	for (nCycel = 0; nCycel < 3; nCycel++) {
		nRet = WifiConnnect();
		if (nRet != 0) {/*wifi连接失败*/
			continue;
		}
		else {
			break;
		}
	}
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("WIFI连接失败..."));
		m_editWifiTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}
	else {
		m_editTestState.SetWindowText(_T("网络连接成功..."));
	}
#if 0
	//Sleep(1500);/*延时防止获取IP失败*/
	/*获取设备IP*/
	m_editTestState.SetWindowText(_T("正在获取IP..."));
	nRet = GetDutIp(m_strIp);
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("获取设备IP失败..."));
		m_editWifiTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}
#endif
	/*4.获取PC机的IP*/
	CString strHostName;
	if (FALSE == GetHostIPaddr(strHostName)) {
		m_editTestState.SetWindowText(_T("获取本机IP失败..."));
		AfxMessageBox(_T("获取本机IP失败,请检查你的网络"));
		return -1;
	}

	/*2. 测试wifi的强度*/
	//m_editTestState.SetWindowText(m_strIp);
	m_editTestState.SetWindowText(_T("正在测试Wifi强度..."));
	nRet = WifiRssiTest();
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("测试WIFI强度失败..."));
		m_editWifiTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}

	/*3. 测试wifi的吞吐量*/
	m_editTestState.SetWindowText(_T("正在测试Wifi吞吐量..."));

	nRet = IprefRecvTest();
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("测试WIFI吞吐量-发送-失败..."));
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
		m_editTestState.SetWindowText(_T("测试WIFI吞吐量-接收-失败..."));
		m_editWifiTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}

sendPass:
	m_editWifiTestResult.SetWindowText(_T("PASS"));
	return 0;

}


int CW1023厂测Dlg::PowerTest()
{
	int nRet;
	CString strPower;
	CString strStdMax, strStdMin;
	BOOL bFlag = FALSE;  

	m_editCurrPower.SetWindowText(_T("..."));

	nRet = GetCurPower(strPower);
	if (nRet != 0) {
		goto fail;/*测试失败*/
	}

	m_editCurrPower.SetWindowText(strPower);
	m_editPowerTarget.SetWindowText(strStdMax);
	m_editPowerVari.SetWindowText(strStdMin);
	if(fIsLessThanStd(strStdMin, strPower) < 0){
		AfxMessageBox(_T("电量不足，请充电"));
		goto fail;
	}

	if(fIsLessThanStd(strStdMax, strPower) > 0){
		AfxMessageBox(_T("电量过多，将自动播放耗电"));
		goto fail;
	}


	m_editTestState.SetWindowText(_T("电量测试成功..."));
	m_editPowerTestResult.SetWindowText(_T("PASS"));
	return 0;
fail:
	m_editTestState.SetWindowText(_T("电量测试失败..."));
	m_editPowerTestResult.SetWindowText(_T("FAIL"));
	return -1;

}


int CW1023厂测Dlg::KnodPowerOffTest()
{
	m_editTestState.SetWindowText(_T("等待判定关机键是否有效..."));

	m_ceKnobEvent.ResetEvent();
	m_bKnobFlag = TRUE;
	m_buttonKnobSucc.EnableWindow(TRUE);
	m_buttonKnobFail.EnableWindow(TRUE);

	AfxMessageBox(_T("请旋转旋钮关机，观察是否\n正常，并在【旋钮判定区】\n选择按键上传结果"));

	WaitForSingleObject(m_ceKnobEvent.m_hObject, INFINITE);
	m_ceKnobEvent.ResetEvent();
	
	if (FALSE == m_bKnobFlag) {/*判定关机键失效*/
		goto fail;
	}


	m_buttonKnobSucc.EnableWindow(FALSE);
	m_buttonKnobFail.EnableWindow(FALSE);
	m_editTestState.SetWindowText(_T("关机键被判定有效"));
	return 0;

fail:
	m_buttonKnobSucc.EnableWindow(FALSE);
	m_buttonKnobFail.EnableWindow(FALSE);
	m_editTestState.SetWindowText(_T("关机键被判定无效"));
	return -1;
}


int CW1023厂测Dlg::FunctionPlayTest()
{
	int n = 0;
	int nRet;
	CString strFilePcPath;
	CString strFileDutPath;
	CString strFileName;
	CString strMsg, strErrMsg;

	//m_nStation = SWEEPING_PLAY_TEST;
	m_editPlayTestResult.SetWindowText(_T("..."));

	/*1. 获取文件路径及文件名*/
	if (FALSE == GetFileName(strFilePcPath, strFileName)) {
		strFilePcPath = _T("sin.wav");
		nRet = PushFile(strFilePcPath);
		if (nRet != 0) {
			m_editTestState.SetWindowText(_T("推送音频文件到设备失败..."));
			//AfxMessageBox(_T("推送文件到DUT设备失败"));
			m_editPlayTestResult.SetWindowText(_T("FAIL"));
			return -1;
		}
		strFileDutPath.Format(_T("/mnt/SDCARD/sin.wav "));
		strMsg.Format(_T("正在播放音频文件(sin.wav)..."));
		strErrMsg.Format(_T("播放音频文件(sin.wav)失败..."));
	}
	else {

		m_editTestState.SetWindowText(_T("正在推送文件到DUT..."));
		/*3. 推送文件到DUT*/
		nRet = PushFile(strFilePcPath);
		if (nRet != 0) {
			m_editTestState.SetWindowText(_T("推送音频文件到设备失败..."));
			//AfxMessageBox(_T("推送文件到DUT设备失败"));
			m_editPlayTestResult.SetWindowText(_T("FAIL"));
			return -1;
		}

		/*4. 构建播放的文件名*/
		strFileDutPath.Format(_T("/mnt/SDCARD/%s"), strFileName);
		strMsg.Format(_T("正在播放音频文件(%s)..."), strFileName);
		strErrMsg.Format(_T("播放音频文件(%s)失败..."), strFileName);
	}

	/*5. 播放*/
	m_editTestState.SetWindowText(strMsg);

	nRet = MusicPlay(strFileDutPath);
	if (nRet != 0) {
		m_editTestState.SetWindowText(strErrMsg);
		goto fail;
	}


	m_editTestState.SetWindowText(_T("播放测试成功..."));
	m_editPlayTestResult.SetWindowText(_T("PASS"));
	return 0;

fail:
	m_editTestState.SetWindowText(_T("播放测试失败..."));
	m_editPlayTestResult.SetWindowText(_T("FAIL"));
	return -1;
}


int CW1023厂测Dlg::SweepingPlayTest()
{
	int n = 0;
	int nRet;
	CString strFilePcPath;
	CString strFileDutPath;
	CString strFileName;
	CString strMsg, strErrMsg;

	m_nStation = SWEEPING_PLAY_TEST;
	m_editPlayTestResult.SetWindowText(_T("..."));
	

	/*1. 获取文件路径及文件名*/
	if (FALSE == GetFileName(strFilePcPath, strFileName)) {
		strFilePcPath = _T("sin.wav");
		nRet = PushFile(strFilePcPath);
		if (nRet != 0) {
			m_editTestState.SetWindowText(_T("推送音频文件到设备失败..."));
			//AfxMessageBox(_T("推送文件到DUT设备失败"));
			m_editPlayTestResult.SetWindowText(_T("FAIL"));
			return -1;
		}
		strFileDutPath.Format(_T("/mnt/SDCARD/sin.wav "));
		strMsg.Format(_T("正在播放音频文件(sin.wav)..."));
		strErrMsg.Format(_T("播放音频文件(sin.wav)失败..."));
	}
	else {

		m_editTestState.SetWindowText(_T("正在推送文件到DUT..."));
		/*3. 推送文件到DUT*/
		nRet = PushFile(strFilePcPath);
		if (nRet != 0) {
			m_editTestState.SetWindowText(_T("推送音频文件到设备失败..."));
			//AfxMessageBox(_T("推送文件到DUT设备失败"));
			m_editPlayTestResult.SetWindowText(_T("FAIL"));
			return -1;
		}

		/*4. 构建播放的文件名*/
		strFileDutPath.Format(_T("/mnt/SDCARD/%s"), strFileName);
		strMsg.Format(_T("正在播放音频文件(%s)..."), strFileName);
		strErrMsg.Format(_T("播放音频文件(%s)失败..."), strFileName);
	}

	m_buttonSweepPass.EnableWindow(TRUE);
	m_buttonSweepFail.EnableWindow(TRUE);
	m_bSweepFlag = TRUE;
	/*5. 播放*/
	m_editTestState.SetWindowText(strMsg);
	while (m_nLiveFlag) {
		nRet = MusicPlay(strFileDutPath);
		if (FALSE == m_nLiveFlag) {
			break;
		}
		if (nRet < 0) {
			AfxMessageBox(_T("播放失败"));
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
	m_editTestState.SetWindowText(_T("播放测试成功..."));
	m_editPlayTestResult.SetWindowText(_T("PASS"));
	return 0;

fail:
	m_buttonSweepPass.EnableWindow(FALSE);
	m_buttonSweepFail.EnableWindow(FALSE);
	m_editTestState.SetWindowText(_T("播放测试失败..."));
	m_editPlayTestResult.SetWindowText(_T("FAIL"));
	return -1;
}


int CW1023厂测Dlg::SDWRRatesTest()
{
	int nRet;

	m_nStation = SD_WR_RATE_TEST;

	m_editSDTestResult.SetWindowText(_T("..."));

	m_editSdWriteVal.SetWindowText(_T("..."));
	m_editTestState.SetWindowText(_T("正在测试SD卡写速率..."));
	nRet = SDWriteTest();
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("测试SD卡写速率失败..."));
		m_editSDTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}
	else if (nRet > 0) {
		//goto writeTest;
		m_editTestState.SetWindowText(_T("测试SD卡写速率失败..."));
		m_editSDTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}

	m_editSdReadVal.SetWindowText(_T("..."));
	m_editTestState.SetWindowText(_T("正在测试SD卡读速率..."));
	nRet = SDReadTest();
	if (nRet < 0) {
		m_editTestState.SetWindowText(_T("测试SD卡读速率失败..."));
		m_editSDTestResult.SetWindowText(_T("FAIL"));
		return -1;/*测试不通过*/
	}
	else if (nRet > 0) {
		//goto readTest;
		m_editTestState.SetWindowText(_T("测试SD卡读速率失败..."));
		m_editSDTestResult.SetWindowText(_T("FAIL"));
		return -1;
	}

	m_editTestState.SetWindowText(_T("测试SD卡读速率成功..."));
	m_editSDTestResult.SetWindowText(_T("PASS"));
	return 0;
}


BOOL CW1023厂测Dlg::PrepareTest()
{
	int nRet;
	CString strFactory;
	CString strDest;

#if 1
	m_editTestState.SetWindowText(_T("正在推送厂测文件..."));
	/*1. 进入厂测模式，实际上是推送factory文件到SD中*/
	strFactory.Format(_T("factory"));
	strDest.Format(_T("/mnt/SDCARD"));
	nRet = PushFile(strFactory, strDest);
	if (nRet < 0) {
		m_editTestState.SetWindowText(_T("推送厂测文件失败..."));
		m_editTestResult.SetWindowText(_T("FAIL"));
		return FALSE;
	}

	m_editTestState.SetWindowText(_T("正在修改厂测文件权限..."));
	nRet = ChmodFactory();
	if (nRet < 0) {
		m_editTestState.SetWindowText(_T("修改厂测文件权限失败..."));
		m_editTestResult.SetWindowText(_T("FAIL"));
		return FALSE;
	}
#endif

#if 0  
	m_editTestState.SetWindowText(_T("正在初始化厂测模式..."));
	nRet = FactorySet();
	if (nRet < 0) {
		m_editTestState.SetWindowText(_T("初始化厂测模式失败..."));
		m_editTestResult.SetWindowText(_T("FAIL"));
		return FALSE;
	}
#endif

	m_editTestState.SetWindowText(_T("正在进入厂测模式..."));
	nRet = EnterFactory();
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("进入厂测模式失败..."));
		//m_editTestResult.SetWindowText(_T("FAIL"));
		return FALSE;
	}

	m_editTestState.SetWindowText(_T("正在获取软件版本..."));
	nRet = GetFirmwareVer();
	if (nRet != 0) {
		m_editTestState.SetWindowText(_T("获取软件版本失败..."));
		//m_editTestResult.SetWindowText(_T("FAIL"));
		return FALSE;
	}

	return TRUE;
}


/***************************End:测试项函数***************************/
BOOL CW1023厂测Dlg::FunctionTest()
{
	int nRet;
	BOOL bFlag = TRUE;
	CString strTemp;

	m_editTestResult.SetWindowText(_T("..."));

	strTemp.Format(FUNC_STATION);
	m_strResult.SetStation(strTemp);

	/*Check SN*/
	bFlag = SNCheck();
	if (FALSE == bFlag) {/*SN Check不通过*/
		//AfxMessageBox(_T("S/N检测不通过"));
		m_editTestResult.SetWindowText(_T("FAIL"));
		LogTestResult(FALSE);
		goto end;
	}

	//AfxMessageBox(_T("S/N检测通过"));

	/*厂测初始化等动作*/
	if (FALSE == PrepareTest()) {
		goto fail;
	}

	/*1. 按键测试*/
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

	/*2. Led测试*/
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

	/*3. wifi测试*/
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

	/*4. TF卡测试--如果有*/
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
	

	/*5. 播放测试*/
	if (m_checkPlayTest.GetCheck() == BST_CHECKED)
	{
		AfxMessageBox(_T("将进行播放测试，请同时\n左旋和右旋音量旋钮，检\n查其是否能正常调节音量，\n并在【播放测试判定区】\n选择按键上传结果"));

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
			m_editTestState.SetWindowText(_T("播放测试判定失败..."));
			goto fail;
		}
	}
	else {
		bFlag = FALSE;
	}

	m_editTestState.SetWindowText(_T("播放测试判定通过..."));
	
	nRet = KnodPowerOffTest();
	if (nRet != 0) {
		goto fail;
	}

pass:
	if (TRUE == bFlag) {
		/*5. 上传测试结果*/
		m_editTestState.SetWindowText(_T("正在上传测试结果到MES系统..."));
		if (FALSE == UploadTestResult()) {
			//m_editTestState.SetWindowText(_T("上传测试结果到MES系统失败..."));
			AfxMessageBox(_T("上传测试结果失败"));
			m_editTestResult.SetWindowText(_T("FAIL"));
			LogTestResult(FALSE);
			goto end;
		}

		/*6. 正在过站*/
		m_editTestState.SetWindowText(_T("正在过站..."));
		if (FALSE == SNSave()) {
			//m_editTestState.SetWindowText(_T("过站失败..."));
			AfxMessageBox(_T("过站失败"));
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
	/*5. 上传测试结果*/
	//m_editTestState.SetWindowText(_T("正在上传测试结果到MES系统..."));
	if (FALSE == UploadTestResult()) {
		//m_editTestState.SetWindowText(_T("上传测试结果到MES系统失败..."));
		AfxMessageBox(_T("上传测试结果失败"));
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


BOOL CW1023厂测Dlg::SweepingTest()
{
	int nRet;
	CString strStation;
	CString strTemp;
	BOOL bFlag = TRUE;

	m_editTestResult.SetWindowText(_T("..."));

	strTemp.Format(SWEEP_STATION);
	m_strResult.SetStation(strTemp);

	bFlag = SNCheck();
	if (FALSE == bFlag) {/*SN Check不通过*/
		//AfxMessageBox(_T("S/N检测不通过"));
		m_editTestResult.SetWindowText(_T("FAIL"));
		LogTestResult(FALSE);
		goto end;
	}

	//AfxMessageBox(_T("S/N检测通过"));

	/*厂测初始化等动作*/
	if (FALSE == PrepareTest()) {
		goto fail;
	}

#if 1
	/*2. 获取上一站(老化测试)测试结果*/
	strStation.Format(BURN_STATION);
	nRet = DisplayTestInfo(strStation);
	if (nRet > 0) {
		AfxMessageBox(_T("获取老化测试结果失败，请检查USB连接及其它"));
		goto fail;
	}
	else if (nRet < 0) {
		AfxMessageBox(_T("老化测试未通过，请重测！"));
		goto fail;
	}
#endif

	/*3. 播放测试*/
	if (m_checkPlayTest.GetCheck() == BST_CHECKED)
	{
		AfxMessageBox(_T("将进行扫频播放测试，请在\n‘播放测试判定区’选择按键\n上传结果"));
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

	//m_editTestState.SetWindowText(_T("等待用户输入测试结果..."));
	//AfxMessageBox(_T("请输入测试结果"));
	/*4. 等待用户选择输入是通过还是Fail*/
	//WaitForSingleObject(m_ceSweepEvent.m_hObject, INFINITE);
	//m_ceSweepEvent.ResetEvent();
	if (m_bSweepFlag) {
		m_editTestState.SetWindowText(_T("扫频测试判定通过..."));
		goto pass;
	}
	else {
		m_editTestState.SetWindowText(_T("扫频测试判定失败..."));
		goto fail;
	}


pass:
	if (bFlag) {
		/*5. 上传测试结果*/
		m_editTestState.SetWindowText(_T("正在上传测试结果到MES系统..."));
		if (FALSE == UploadTestResult()) {
			//m_editTestState.SetWindowText(_T("上传测试结果到MES系统失败..."));
			AfxMessageBox(_T("上传测试结果失败"));
			m_editTestResult.SetWindowText(_T("FAIL"));
			LogTestResult(FALSE);
			goto end;
		}

		/*6. 正在过站*/
		m_editTestState.SetWindowText(_T("正在过站..."));
		if (FALSE == SNSave()) {
			//m_editTestState.SetWindowText(_T("过站失败..."));
			AfxMessageBox(_T("过站失败"));
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
	/*5. 上传测试结果*/
	if (FALSE == UploadTestResult()) {
		//m_editTestState.SetWindowText(_T("上传测试结果到MES系统失败..."));
		AfxMessageBox(_T("上传测试结果失败"));
	}
	m_editTestResult.SetWindowText(_T("FAIL"));
	LogTestResult(FALSE);
	goto end;

end:/*测试完成则返回TRUE，未完成则返回FALSE*/
	return TRUE;
}


BOOL CW1023厂测Dlg::OutFactoryTest()
{
	int nRet;
	CString strTemp;
	BOOL bFlag = TRUE;

	m_editTestResult.SetWindowText(_T("..."));


	strTemp.Format(PACK_STATION);
	m_strResult.SetStation(strTemp);

	bFlag = SNCheck();
	if (FALSE == bFlag) {/*SN Check不通过*/
		//AfxMessageBox(_T("S/N检测不通过"));
		m_editTestResult.SetWindowText(_T("FAIL"));
		LogTestResult(FALSE);
		goto end;
	}

	if (TRUE == m_bPassEATest) {/*补丁，解决进入用户模式的板子在上传MES系统失败后不能再测试的问题*/
		strTemp.Format(_T("PASS"));
		m_strResult.SetPower(strTemp);
		goto pass;
	}

	/*厂测初始化等动作*/
	if (FALSE == PrepareTest()) {
		goto fail;
	}

#if 1
	m_editTestState.SetWindowText(_T("正在获取当前电量..."));
	/*3. 电量测试*/
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
		/*4. 出厂模式*/
		m_editTestState.SetWindowText(_T("正在恢复出厂设置..."));
		nRet = OutFactory();
		if (nRet != 0) {
			m_editTestState.SetWindowText(_T("恢复出厂设置失败"));
			goto fail;
		}

		m_editTestState.SetWindowText(_T("厂测成功，正在记录..."));
		nRet = FactoryTestSuccess();
		if (nRet != 0) {
			m_editTestState.SetWindowText(_T("厂测结果记录失败"));
			goto fail;
		}
	}
pass:
	if (bFlag) {
		/*5. 上传测试结果*/
		m_editTestState.SetWindowText(_T("正在上传测试结果到MES系统..."));
		if (FALSE == UploadTestResult()) {
			//m_editTestState.SetWindowText(_T("上传测试结果到MES系统失败..."));
			AfxMessageBox(_T("上传测试结果失败"));
			m_editTestResult.SetWindowText(_T("FAIL"));
			LogTestResult(FALSE);
			goto end;
		}

		/*6. 正在过站*/
		m_editTestState.SetWindowText(_T("正在过站..."));
		if (FALSE == SNSave()) {
			//m_editTestState.SetWindowText(_T("过站失败..."));
			AfxMessageBox(_T("过站失败"));
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
	/*5. 上传测试结果*/
	//m_editTestState.SetWindowText(_T("正在上传测试结果到MES系统..."));
	if (FALSE == UploadTestResult()) {
		//m_editTestState.SetWindowText(_T("上传测试结果到MES系统失败..."));
		AfxMessageBox(_T("记录测试结果失败"));
	}

	m_editTestResult.SetWindowText(_T("FAIL"));
	LogTestResult(FALSE);
	PowerDischarge();
	goto end;


end:/*测试完成则返回TRUE，未完成则返回FALSE*/
	return TRUE;
}


BOOL CW1023厂测Dlg::MainTestLoop()
{
	int nSel;
	CString strFactory;

	CString strSn;
	CString strStation;

	m_editTestResult.SetWindowText(_T("..."));

	m_editSN.GetWindowText(strSn);
	m_strResult.SetSN(strSn);

	
	/*2. 检测当前处于哪个测试模式下*/
	nSel = m_comboTestItem.GetCurSel();

	switch (nSel) {
		case 0:/*扫频模式*/
			SweepingTest();
			break;
		case 1:/*功能测试*/
			FunctionTest();
			break;
		case 2:/*电量测试*/
			OutFactoryTest();
			break;
	}

	return TRUE;
}


BOOL CW1023厂测Dlg::KillProcessFromName(CString strProcessName)
{
	//创建进程快照(TH32CS_SNAPPROCESS表示创建所有进程的快照)    
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//PROCESSENTRY32进程快照的结构体    
	PROCESSENTRY32 pe;
	
	//实例化后使用Process32First获取第一个快照的进程前必做的初始化操作    
	pe.dwSize = sizeof(PROCESSENTRY32);

	//下面的IF效果同:    
	//if(hProcessSnap == INVALID_HANDLE_VALUE)   无效的句柄    
	if (!Process32First(hSnapShot, &pe))
	{
		 return FALSE;
	}
	
	//将字符串转换为小写    
	strProcessName.MakeLower();
	
	//如果句柄有效  则一直获取下一个句柄循环下去    
	while (Process32Next(hSnapShot, &pe))
	{
	
		//pe.szExeFile获取当前进程的可执行文件名称    
		CString scTmp = pe.szExeFile;
	
		//将可执行文件名称所有英文字母修改为小写    
		scTmp.MakeLower();
	
		//比较当前进程的可执行文件名称和传递进来的文件名称是否相同    
		//相同的话Compare返回0    
		if (!scTmp.Compare(strProcessName))
		{
			//从快照进程中获取该进程的PID(即任务管理器中的PID)    
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


BOOL CW1023厂测Dlg::GetHostIPaddr(CString &sLocalName)
{
	
	char szLocalName[256];//定义  
	WSADATA wsaData;//定义套接字存储变量  
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)
	{
		if (gethostname(szLocalName, sizeof(szLocalName)) != 0) {//产生错误 
			m_strHostIp = _T("");
			WSACleanup();
			//return GetLastError();
			return FALSE;
		}
		else{
			sLocalName = szLocalName;//用sLocalName变量存储获得的计算机名称  
			struct hostent FAR * lpHostEnt = gethostbyname(sLocalName);
			if (lpHostEnt == NULL) {//错误
				m_strHostIp = _T("");
				WSACleanup();
				//return GetLastError();
				return FALSE;
			}
			LPSTR lpAddr = lpHostEnt->h_addr_list[0];//获取IP地址  
			if (lpAddr)
			{
				struct in_addr inAddr;
				memmove(&inAddr, lpAddr, 4);
				m_strHostIp = inet_ntoa(inAddr);//转换为标准格式  
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
	 WSACleanup();//清空套接字初始化变量  
	 //return 0
	 return TRUE;
}


void CW1023厂测Dlg::SaveConfig(int nFlag)
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
		AfxMessageBox(_T("配置文件-config.ini-不存在"));
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



void CW1023厂测Dlg::SaveLogConfig()
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
		AfxMessageBox(_T("配置文件-config.ini-不存在"));
		return;
	}

	WritePrivateProfileString("HEAD SETTING", "LOG_LOCA", m_strLogPath, config_file_path);

	return;
}


void CW1023厂测Dlg::SaveSdTestConfig()
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
		AfxMessageBox(_T("配置文件-config.ini-不存在"));
		return;
	}

	WritePrivateProfileString("FUNCTION SETTING", "SD_READ", m_strReadStdVal, config_file_path);
	WritePrivateProfileString("FUNCTION SETTING", "SD_WRITE", m_strWriteStdVal, config_file_path);

	return;
}


void CW1023厂测Dlg::LoadConfig(int nFlag)
{
	CStdioFile config_file;
	CString config_file_path = "./config.ini";
	CFileFind finder;
	int nTemp;
	CString strTemp;

	BOOL bFlag = finder.FindFile(config_file_path);
	if (FALSE == bFlag) {
		AfxMessageBox(_T("配置文件-config.ini-不存在"));
		return;
	}

	switch (nFlag) {
		case 0:   /*扫频测试*/
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

		case 1:   /*功能测试*/
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

		case 2:   /*电量测试*/
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

void CW1023厂测Dlg::GetTestItemConf()
{
	CStdioFile config_file;
	CString config_file_path = "./config.ini";
	CFileFind finder;
	int nTemp;

	BOOL bFlag = finder.FindFile(config_file_path);
	if (FALSE == bFlag) {
		AfxMessageBox(_T("配置文件-config.ini-不存在"));
		return;
	}	

	nTemp = GetPrivateProfileInt("HEAD SETTING", "TEST_ITEM", 0, config_file_path);
	m_comboTestItem.SetCurSel(nTemp);

	/*顺便获取Log文件的存储位置*/
	GetPrivateProfileString("HEAD SETTING", "LOG_LOCA", "D://w1023-Log.txt", m_strLogPath.GetBuffer(MAX_PATH), MAX_PATH, config_file_path);
}


float CW1023厂测Dlg::TTOF(CString &strString)
{
	return (float)_ttof(strString);
}


#if 1
BOOL CW1023厂测Dlg::LogTestResult(BOOL bFlag)
{
	CString strSn;
	CString strStation;
	CString strTotal;
	CString strTemp;

	/*写入S/N*/
	m_strResult.GetSN(strSn);
	m_strResult.GetStation(strStation);
	//m_strResult.GetTotal(strTotal);
	//if ((0 != strTotal.CompareNoCase(_T("Pass"))) && (0 != strTotal.CompareNoCase(_T("Fail")))) {
	//	return TRUE;/*此种情况不上传*/
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


BOOL CW1023厂测Dlg::UploadItem(int nFlag)
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
		case TEST_ITEM_WIFI:	/*wifi测试*/
			strItem.Format(_T("Wifi"));
			m_strResult.GetWifi(strVal);
			break;
		case TEST_ITEM_LED:	/*Led测试*/
			strItem.Format(_T("Led"));
			m_strResult.GetLed(strVal);
			break;
		case TEST_ITEM_KEY:	/*Key测试*/
			strItem.Format(_T("Key"));
			m_strResult.GetKey(strVal);
			break;
		case TEST_ITEM_PLAY:	/*Play测试*/
			strItem.Format(_T("Play"));
			m_strResult.GetPlay(strVal);
			break;
		case TEST_ITEM_POWER:	/*Power测试*/
			strItem.Format(_T("Power"));
			m_strResult.GetPower(strVal);
			break;
		case TEST_ITEM_CARD:	/*SD测试*/
			strItem.Format(_T("SD"));
			m_strResult.GetCard(strVal);
			break;
		//case TEST_ITEM_TOTAL:	/*Total测试*/
		//	strItem.Format(_T("Total"));
		//	m_strResult.GetTotal(strVal);
		//	break;
		default:	
			break;
	}

	resultLoadReq.testitem = strItem.GetBuffer(0);
	resultLoadReq.testresult = strVal.GetBuffer(0);
	
	if ((0 != strVal.CompareNoCase(_T("FAIL"))) || (0 == strVal.CompareNoCase(_T("PASS")))) {
		/*除了Pass或者Fail外，其它不应该上传*/
		return TRUE;
	}

	timep = time(&timeq);
	timeq += 8 * 60 * 60;
	resultLoadReq.testtime = timeq;

	CString strTemp;
	wchar_t *p;

	if (SOAP_OK == ClientSoap.SfcTestResult_USCOREUpload_(&resultLoadReq, resultLoadRer))
	{
		// 函数与系统通信成功
		strResult = resultLoadRer.SfcTestResult_USCOREUploadResult->anyType[0];
		p = MulityByteToWideChar(resultLoadRer.SfcTestResult_USCOREUploadResult->anyType[1]);
		strReson = p;
		//strReport.Format("SSN测试结果上传：%s", strReson);
		if (strResult.CompareNoCase("PASS") == 0)
		{
			// 系统判定上传结果成功					
			return TRUE;
		}
		else
		{
			// 系统判定上传结果失败
			strTemp.Format(_T("上传失败：%s"), strReson);
			AfxMessageBox(strTemp);
			return FALSE;
		}

		delete p;
	}
	else
	{
		// 函数与系统通信失败
		AfxMessageBox(_T("测试结果上传--通信失败"));
		return FALSE;
	}

}


BOOL CW1023厂测Dlg::UploadTestResult()
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


BOOL CW1023厂测Dlg::SNCheck()
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
		// 校验SN连接端口成功
		strResult = checkSsnRer.CheckSSN_USCORENEWResult->anyType[0];
		if (strResult.CompareNoCase("PASS") == 0)
		{
			//pStatus->SetWindowTextA("SN校验成功！");
			//AfxMessageBox(_T("SN Check成功"));
			return TRUE;
		}
		else
		{
			// 校验SN失败，给出失败原因，退出上传MES系统
			strReson = MulityByteToWideChar(checkSsnRer.CheckSSN_USCORENEWResult->anyType[1]);
			//strReport.Format("SFC校验SN失败：%s", strReson);
			//AfxMessageBox(strReport);
			if (-1 != strReson.Find(_T("ASSEMBLY1"))) {
				AfxMessageBox(_T("需测组装1站"));
			}
			else if (-1 != strReson.Find(_T("ASSEMBLY2"))) {
				AfxMessageBox(_T("需测组装2站"));
			}
			else if (-1 != strReson.Find(_T("SWEEP"))) {
				AfxMessageBox(_T("需测扫频站"));
			}
			else if (-1 != strReson.Find(_T("FUNCTION TEST"))) {
				AfxMessageBox(_T("需测功能测试站"));
			}
			else if (-1 != strReson.Find(_T("CURVE TEST"))) {
				AfxMessageBox(_T("需测SoundCheck站"));
			}
			else if (-1 != strReson.Find(_T("E/A-TEST"))) {
				AfxMessageBox(_T("需测电量测试站"));
			}
			else if (-1 != strReson.Find(_T("E/A-TEST"))) {
				AfxMessageBox(_T("需测外观检测站"));
			}
			else if (-1 != strReson.Find(_T("SCAN CONUTRY"))) {
				AfxMessageBox(_T("需测SCAN CONUTRY站"));
			}
			else if (-1 != strReson.Find(_T("PACKCARTON"))) {
				AfxMessageBox(_T("需测包装站"));
			}
			else if (-1 != strReson.Find(_T("PALLETIZATION"))) {
				AfxMessageBox(_T("需测货盘站"));
			}
			else if (-1 != strReson.Find(_T("connect failed in tcp_connect"))) {
				AfxMessageBox(_T("请检查网络连接"));
			}
			else {
				AfxMessageBox(_T("SN检测不通过"));
			}
			return FALSE;
		}
	}
	else
	{
		// 校验SN失败：通信失败
		std::string fault;
		fault = ClientSoap.soap_fault_detail();
		CString strTemp;
		strTemp = fault.c_str();
		//strTemp.Format("校验SN失败：通信失败：%d", nErrorCode);
		AfxMessageBox(strTemp);
		//pStatus->SetWindowTextA(strTemp);
		//EventReset.SetEvent();
		return FALSE;
	}
}


BOOL CW1023厂测Dlg::SNSave()
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
		// 过站函数与系统通信成功
		CString strResult = saveSsnRer.SaveSSN_USCORENEWResult->anyType[0];
		if (strResult.CompareNoCase("PASS") == 0)
		{
			// 过站成功，一切OK了
			strReson = MulityByteToWideChar(saveSsnRer.SaveSSN_USCORENEWResult->anyType[1]);
			strReport.Format("SSN过站成功：%s", strReson);
			//AfxMessageBox(strReport, MB_OK);
			//m_staStatus.SetWindowText("log文件解析成功，上传MES系统成功！");
			return  TRUE;
		}
		else
		{
			// 过站失败，给出失败原因，退出上传MES系统
			strReson = MulityByteToWideChar(saveSsnRer.SaveSSN_USCORENEWResult->anyType[1]);
			strReport.Format("SSN过站失败：%s", strReson);
			AfxMessageBox(strReport, MB_OK);
			//m_staStatus.SetWindowText("上传MES系统：过站失败！");
			return FALSE;
		}
	}
	else
	{
		// 这表示过站函数与系统通信失败
		AfxMessageBox("过站失败：通信失败！", MB_OK);
		//m_staStatus.SetWindowText("上传MES系统：通信失败！");
		return FALSE;
	}
}



char FirstDriveFromMask(ULONG unitmask) //獲取盤符
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


BOOL CW1023厂测Dlg::CreateAdbEnableFile(CString &strPath)
{
	CString strFile;
	CFile cfFile;
	//LPCTSTR s = info->GetBuffer(info->GetLength());
	BOOL bFlag;

	//bFlag = PathFileExists(_T("E:\\W-1023厂测软件测试记录.txt"));

	strFile.Format(_T("%s:jl_enter_factory_mode_file_2018_4_23"), strPath);
	bFlag = PathFileExists(strFile);
	if (bFlag == TRUE) {
		return TRUE;
	}

	//AfxMessageBox(_T("已经在U盘创建文件ljl_enter_factory_mode_file_2018_4_23，请确认"));

	if (!cfFile.Open(strFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)) {
		return FALSE;
	}

	cfFile.Close();

	return TRUE;
}


#if 0
LRESULT CW1023厂测Dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString decDriver;

	switch (message)
	{
		case WM_DEVICECHANGE:
		{
			PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)(lParam);
			switch (wParam)
			{
				case DBT_DEVICEARRIVAL://设备检测结束，并且可以使用
				{
					if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {//逻辑卷
						PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
						switch (lpdbv->dbcv_flags) {
							case 0://U盘
							{
								//CString decDriver;
								decDriver = FirstDriveFromMask(lpdbv->dbcv_unitmask);
								if (FALSE == CreateAdbEnableFile(decDriver)) {
									//AfxMessageBox("U盘写入进厂测文件失败");
								}
							}
							break;
							case DBTF_MEDIA://光盘
								break;
						}
					}
				}

				case DBT_DEVICEREMOVECOMPLETE://设备卸载或者拔出
				{
					if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)//逻辑卷
					{
						PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
						switch (lpdbv->dbcv_flags)
						{
						case 0:                //U盘a
						{
							//CString decDriver;
							decDriver = FirstDriveFromMask(lpdbv->dbcv_unitmask);
						}
						break;
						case DBTF_MEDIA:    //光盘
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
void CW1023厂测Dlg::TermMainThread()
{
	if (TRUE == m_bMainThreadLive) {
		TerminateThread(m_pctMainThread->m_hThread, 0);
		CloseHandle(m_pctMainThread->m_hThread);
	}
}
#endif

void CW1023厂测Dlg::ButtonInit()
{
	/*按键fail消息没有被置位，表示没有fail*/
	m_nKeyTestFailFlag = FALSE;
	

	/*扫频测试循环标志位置位，会循环*/
	m_nLiveFlag = TRUE;
	m_bSweepFlag = TRUE;

	m_buttonSweepPass.EnableWindow(FALSE);
	m_buttonSweepFail.EnableWindow(FALSE);

	m_buttonKeyTest.EnableWindow(FALSE);

	m_buttonKnobSucc.EnableWindow(FALSE);
	m_buttonKnobFail.EnableWindow(FALSE);
}

void CW1023厂测Dlg::OnLogin()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox(_T("登录"));
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


void CW1023厂测Dlg::OtherUserConf()
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

	/*参数部分*/
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


void CW1023厂测Dlg::RootUserConf()
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




void CW1023厂测Dlg::OnSdArgumentSetting()
{
	// TODO: 在此添加命令处理程序代码
	
	INT_PTR nResponse;
	CSDSettingPage setPage(this);

	nResponse = setPage.DoModal();
	if (IDOK == nResponse) {
		SaveSdTestConfig();
	}

}


void CW1023厂测Dlg::SetSdReadStdVal(CString &strVal)
{
	m_strReadStdVal = strVal;
}


void CW1023厂测Dlg::SetSdWriteStdVal(CString &strVal)
{
	m_strWriteStdVal = strVal;
}


void CW1023厂测Dlg::GetSdReadStdVal(CString &strTemp)
{
	strTemp = m_strReadStdVal;
}

void CW1023厂测Dlg::GetSdWriteStdVal(CString &strTemp)
{
	strTemp = m_strWriteStdVal;
}


void CW1023厂测Dlg::GetLogPath(CString &strTemp)
{
	strTemp = m_strLogPath;
}

void CW1023厂测Dlg::SetLogPath(CString &strVal)
{
	m_strLogPath = strVal;
}


/******************结果比较函数，比较结果不计算单位*******************/
int CW1023厂测Dlg::fIsLessThanStd(CString &strStd, CString &strVal)
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


int CW1023厂测Dlg::fIsLessThanStd(float fStd, float fVal)
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


int CW1023厂测Dlg::fIsLessThanStd(CString &strStd, float fVal)
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


/******************结果比较函数，比较结果不计算单位*******************/
BOOL CW1023厂测Dlg::fIsInStd(CString &strStdMax, CString &strStdMin, CString &strVal)
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


BOOL CW1023厂测Dlg::MbpsTestJudge(CString &strStd, CString &strVal)
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

void CW1023厂测Dlg::OnLogSetting()
{
	// TODO: 在此添加命令处理程序代码

	INT_PTR nResponse;
	CLogConfigPage logPage(this);

	nResponse = logPage.DoModal();
	if (IDOK == nResponse) {
		SaveLogConfig();
	}
}


void CW1023厂测Dlg::OnSnPassSetting()
{
	int nRet;
	// TODO: 在此添加命令处理程序代码
	nRet = AfxMessageBox(_T("直接过MES系统E/A\n测试站，请注意仍要\n勾选上传MES系统"), MB_OKCANCEL|MB_ICONEXCLAMATION);
	if (nRet != IDOK) {
		m_bPassEATest = FALSE;
	}
	else {
		m_bPassEATest = TRUE;
		m_comboTestItem.SetCurSel(2);
		ChooseTestItem(2);
	}
}


void CW1023厂测Dlg::OnCbnDropdownTestitemCombo()
{
	int nSel;
	// TODO: 在此添加控件通知处理程序代码
	nSel = m_comboTestItem.GetCurSel();
	SaveConfig(nSel);
}


void CW1023厂测Dlg::OnBnClickedKnobSuccButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bKnobFlag = TRUE;
	m_ceKnobEvent.SetEvent();
}


void CW1023厂测Dlg::OnBnClickedKnobFailButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bKnobFlag = FALSE;
	m_ceKnobEvent.SetEvent();
}


UINT CW1023厂测Dlg::iperfStartProc(LPVOID lpParam)
{
	CW1023厂测Dlg *p;

	p = (CW1023厂测Dlg *)lpParam;

	p->StrarIperf3();

	AfxMessageBox(_T("iperf线程已经退出"));

	return 0;
}

void CW1023厂测Dlg::StrarIperf3()
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
		AfxMessageBox(_T("启动Iperf3进程失败"));
		//int error = GetLastError();
	}
}

void CW1023厂测Dlg::StopIperf3()
{
	TerminateProcess(m_ProcessInfo.hProcess, 0);
}



BOOL CW1023厂测Dlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	StopIperf3();
	return CDialogEx::DestroyWindow();
}
