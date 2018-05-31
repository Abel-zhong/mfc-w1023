
// W-1023厂测Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "AdbShell.h"
#include "CmdShell.h"

#define RSSI_MAX_VAL		100
#define RSSI_MIN_VAL		0
#define MBPS_MAX_VAL		100
#define MBPS_MIN_VAL		0

typedef enum __CurTestStation
{
	START_TEST = 0,
	WIFI_TEST, 
	/*******LED灯测试********/
	LED_TEST,
	LED_RED_TEST,
	LED_GREEN_TEST,
	LED_BLUE_TEST,
	/*******LED灯测试********/
	/********按键测试********/
	KEY_TEST,
	KEY1_TEST,
	KEY2_TEST,
	KEY3_TEST,
	KEY4_TEST,
	KEY5_TEST,
	KEY6_TEST,
	/********按键测试********/
	SD_WR_RATE_TEST,
	SWEEPING_PLAY_TEST,
}CurTestStation;


typedef enum __TestItem
{
	TEST_ITEM_WIFI = 0,
	TEST_ITEM_KEY,
	TEST_ITEM_LED,
	TEST_ITEM_PLAY,
	TEST_ITEM_RECORD,
	TEST_ITEM_POWER,
	TEST_ITEM_CARD,
	TEST_ITEM_TOTAL,
}TestItem;


class TestResult {

private:
	CString m_strStation = _T("");
	CString m_strSN = _T("");
	CString m_strPlay = _T("");
	CString m_strRecord = _T("");
	CString m_strKey = _T("");
	CString m_strLed = _T("");
	CString m_strWifi = _T("");
	CString m_strCard = _T("");
	CString m_strPower = _T("");
	CString m_strTotal = _T("");
public:
	TestResult();
	//~TestResult();
	void SetStation(CString &strStation);
	void SetSN(CString &strSN);
	void SetPlay(CString &strPlay);
	void SetRecord(CString &strRecord);
	void SetKey(CString &strKey);
	void SetLed(CString &strLed);
	void SetWifi(CString &strWifi);
	void SetCard(CString &strCard);
	void SetPower(CString &strPower);
	void SetTotal(CString &strTotal);

	void GetStation(CString &strStation);
	void GetSN(CString &strSN);
	void GetPlay(CString &strPlay);
	void GetRecord(CString &strRecord);
	void GetKey(CString &strKey);
	void GetLed(CString &strLed);
	void GetWifi(CString &strWifi);
	void GetCard(CString &strCard);
	void GetPower(CString &strPower);
	void GetTotal(CString &strTotal);

	void Empty();
	void ReleaseBuffer();
};


// CW1023厂测Dlg 对话框
class CW1023厂测Dlg : public CDialogEx
{
// 构造
public:
	CW1023厂测Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_W1023_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeTestitemCombo();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedStartTestButton();
	afx_msg void OnBnClickedKeyFailButton();
	afx_msg void OnBnClickedSweepSucButton();
	afx_msg void OnBnClickedSweepFailButton();
	afx_msg void OnBnClickedFileChoseButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CBrush m_greenbrush, m_redbrush, m_yellowbrush, m_bluebrush;
	COLORREF m_greencolor, m_redcolor, m_textcolor, m_yellowcolor, m_bluecolor;
	// 测试工站选择项
	CComboBox m_comboTestItem;
	/*测试界面显示项*/
	CEdit m_editTestResult;
	CEdit m_editTestState;
	CEdit m_editSN;
	CEdit m_editSsid;
	CEdit m_editPasswd;
	// Wifi测试结果显示
	CEdit m_editWifiTestResult;
	// LED测试结果显示
	CEdit m_editLedTestResult;
	// 按键测试结果显示
	CEdit m_editKeyTestResult;
	// 播放测试结果显示
	CEdit m_editPlayTestResult;
	// 录音测试结果显示
	CEdit m_editRecordTestResult;
	// 电量检测结果显示
	CEdit m_editPowerTestResult;
	// 版本显示
	CEdit m_editSoftwareVersion;
	// SD卡测试结果
	CEdit m_editSDTestResult;
	// 开始测试按钮
	CButton m_buttonStartTest;
	// 案件测试不良按钮
	CButton m_buttonKeyTest;
	// 测试信息上传MES系统选择控件
	CButton m_checkUploadMes;
	// wifi测试选择控件
	CButton m_checkWifiTest;
	// 按键测试选择控件
	CButton m_checkKeyTest;
	// 播放测试是否选择控件
	CButton m_checkPlayTest;
	// Led测试选择控件
	CButton m_checkLedTest;
	// 录音测试选择控件
	CButton m_checkRecordTest;
	// 电量测试选择控件
	CButton m_checkPowerTest;
	// SD卡测试选择控件
	CButton m_checkSdCardTest;
	CEdit m_editRssiMin;
	CEdit m_editRssiMax;
	CEdit m_editRssiReal;
	CEdit m_editMbpsMin;
	CEdit m_editMbpsMax;
	CEdit m_editMbpsReal;
	// MBPS接受最小值
	CEdit m_editMbpsMin2;
	// MBPS接收最大值
	CEdit m_editMbpsMax2;
	// MBPS测试接收值
	CEdit m_editMbpsReal2;
	CEdit m_editKey1State;
	CEdit m_editKey2State;
	CEdit m_editKey3State;
	CEdit m_editKey4State;
	CEdit m_editKey5State;
	CEdit m_editKey6State;
	CEdit m_editResetKeyState;
	CEdit m_editLedBlue;
	CEdit m_editLedGreen;
	CEdit m_editLedRed;

	CEdit m_editSdReadVal;
	CEdit m_editSdWriteVal;
	CEdit m_editPowerTarget;/*改成最大电量*/
	CEdit m_editPowerVari;/*改成最小电量*/
	CEdit m_editCurrPower;

	/*播放测试的文件名*/
	CEdit m_editFileName;
	/*文件推送标志*/
	CButton m_buttonPushFile;

	CButton m_buttonSweepPass;
	CButton m_buttonSweepFail;
	CEvent m_ceSweepEvent;
	BOOL m_bSweepFlag = FALSE;

	/*当前测试工站，值来自于enum CurTestStation结构*/
	int m_nStation;

	/*处理主线程相应结构*/
	CEvent m_ceStartEvent;
	CWinThread *m_pctMainThread;
	CEvent m_ceMainEvent;

	CWinThread *m_pctSNCheckThread;
	CEvent m_ceSNCheckEvent;

	/*Iperf3服务端线程*/
	CWinThread *m_pctIprefThread;

	/*用于告知程序中子循环退出的标志*/
	BOOL m_nLiveFlag = FALSE;

	/*按键判定Fail后设置的标志*/
	BOOL m_nKeyTestFailFlag = FALSE;

	/*测试Led测试程序是否还存在*/
	BOOL m_nLedTestThreadLive = FALSE;


	/*用于改变字体颜色的变量*/
	CFont m_fontFont;
	CFont m_buttonFont;

	/*保存当前测试设备IP*/
	CString m_strIp;
	/*保存当前测试机器的IP*/
	CString m_strHostIp;

	/*结果保存*/
	TestResult m_strResult;

	/*菜单项*/
	CMenu m_cmMenu;
	BOOL m_bUserRoot = FALSE;
	BOOL m_bPassEATest = FALSE;

	// 选择要推送的音频文件
	CButton m_buttonFileChoose;

	//SD卡读写速率设置值
	CString m_strReadStdVal;
	CString m_strWriteStdVal;

	//Log保存路径
	CString m_strLogPath;

	// 旋钮功能正常键	// 旋钮功能正常键
	CButton m_buttonKnobSucc;
	// 旋钮功能异常按键
	CButton m_buttonKnobFail;
	CEvent m_ceKnobEvent;
	BOOL m_bKnobFlag = FALSE;

	PROCESS_INFORMATION m_ProcessInfo;

public:
	/*显示系列函数*/
	BOOL PathName2Name(CString &strPathName, CString &strName);
	BOOL GetFileName(CString &strPathName, CString &strName);
	void ChooseTestItem(int nSel, BOOL bFlag = FALSE);
	void SetSubLedResult(CString &strRet, int nStation);
	void SetLedTestResult(BOOL nResult, int nStation, int nNum);
	void SetSubKeyResult(CString &strRet, int nStation);
	void SetKeyTestResult(BOOL nResult, int nStation);
	void SetRssiResult(int nVal);
	void SetMbpsResult(int nVal);
	void SetWifiTestResult(BOOL nResult);
	void SetPlayTestResult(BOOL nResult);
	void SetRecordTestResult(BOOL nResult);
	void SetPowerTestResult(BOOL nResult);

	void SetTotalTestResult(int nResult);
	int LoadTestResultToLog(CString *info);
	void TestEndReset();
	void TestStartRest();
	void ResetMainTestDlg();
	void UpdataTestInfo(CString &info);

	BOOL iAdbExist(BOOL bFlag = TRUE);
	int iExecuteAdbCmd(CString &strCmd, BOOL bFlag = TRUE);
	int iExecuteAdbCmd(CString &strCmd, CString &strRet, BOOL bFlag = TRUE);
	int iExecuteShellCmd(CString &strCmd, CString &strRet, BOOL bFlag = TRUE);
	int iExecuteShellCmdWithTimeout(CString &strCmd, int nTimeout, BOOL bFlag = TRUE);

	/*分离测试动作*/
	int FactoryStop();
	int SDReadTest();
	int SDWriteTest();
	int LedTestSubRed();
	int LedTestSubBlue();
	int LedTestSubGreen();
	int SubKeyTest();
	int NightKeyTest();
	int ResetKeyTest();
	int WifiConnnect();
	int WifiDisconnect();
	int MusicPlay(CString &filePath);
	int MusicStop();
	int MicRecordStart(CString &filePath);
	int MicRecordStop();
	int RememberTestInfo(CString &strStation, BOOL bFlag = TRUE);
	int DisplayTestInfo(CString &strStation);
	int EnterFactory();
	int OutFactory();
	int FactorySet();
	int GetCurPower(CString &strPower);
	int PushFile(CString &strPath);
	int PushFile(CString &strPath, CString &strDest);
	int PullFile(CString &strPath);
	int ChmodFactory();
	int GetDutIp(CString &strBuf);
	int DutConnectionCheck();
	int IprefRecvTest();
	int IprefSendTest(int nPort);
	int WifiRssiTest();
	int GetFirmwareVer();
	int FactoryTestSuccess();
	void PowerDischarge();
	int AdbSampleTest();
	
	//测试项相关
	int LedTest();
	void KeyPrepareTestDisplay();
	int KeyTest();
	int WifiTest();
	int PowerTest();
	int KnodPowerOffTest();
	int SweepingPlayTest();
	int FunctionPlayTest();
	int SDWRRatesTest();
	int PrepareTest();

	BOOL FunctionTest();
	BOOL SweepingTest();
	BOOL OutFactoryTest();
	BOOL MainTestLoop();

	static UINT MainThreadProc(LPVOID lpParam);
	static UINT SNCheckThreadProc(LPVOID lpParam);
	static UINT iprefRecvProc(LPVOID lpParam);
	static UINT iprefSendProc(LPVOID lpParam);

	BOOL KillProcessFromName(CString strProcessName);
	BOOL GetHostIPaddr(CString &sLocalName);

	void SaveConfig(int nFlag);
	void SaveLogConfig();
	void SaveSdTestConfig();
	void LoadConfig(int nFlag);
	void GetTestItemConf();
	float TTOF(CString &strString);

	BOOL CreateAdbEnableFile(CString &strPath);

#if 1
	BOOL UploadItem(int nFlag);
	BOOL UploadTestResult();
	BOOL SNCheck();
	BOOL SNSave();
	BOOL LogTestResult(BOOL bFlag);
#endif

	void ButtonInit();

	void SetSdReadStdVal(CString &strVal);
	void SetSdWriteStdVal(CString &strVal);
	void GetSdReadStdVal(CString &strTemp);
	void GetSdWriteStdVal(CString &strTemp);
	void GetLogPath(CString &strTemp);
	void SetLogPath(CString &strVal);

	int fIsLessThanStd(CString &strStd, CString &strVal);
	int fIsLessThanStd(float fStd, float fVal);
	int fIsLessThanStd(CString &strStd, float fVal);
	BOOL fIsInStd(CString &strStdMax, CString &strStdMin, CString &strVal);
	BOOL MbpsTestJudge(CString &strStd, CString &strVal);

	static UINT iperfStartProc(LPVOID lpParam);
	void StrarIperf3();
	void StopIperf3();
	//void TermMainThread();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLogin();

	void OtherUserConf();
	void RootUserConf();

	afx_msg void OnSdArgumentSetting();
	afx_msg void OnLogSetting();
	afx_msg void OnSnPassSetting();
	afx_msg void OnCbnDropdownTestitemCombo();

	afx_msg void OnBnClickedKnobSuccButton();
	afx_msg void OnBnClickedKnobFailButton();
	virtual BOOL DestroyWindow();
};





