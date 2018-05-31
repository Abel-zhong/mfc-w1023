
// W-1023����Dlg.h : ͷ�ļ�
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
	/*******LED�Ʋ���********/
	LED_TEST,
	LED_RED_TEST,
	LED_GREEN_TEST,
	LED_BLUE_TEST,
	/*******LED�Ʋ���********/
	/********��������********/
	KEY_TEST,
	KEY1_TEST,
	KEY2_TEST,
	KEY3_TEST,
	KEY4_TEST,
	KEY5_TEST,
	KEY6_TEST,
	/********��������********/
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


// CW1023����Dlg �Ի���
class CW1023����Dlg : public CDialogEx
{
// ����
public:
	CW1023����Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_W1023_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	// ���Թ�վѡ����
	CComboBox m_comboTestItem;
	/*���Խ�����ʾ��*/
	CEdit m_editTestResult;
	CEdit m_editTestState;
	CEdit m_editSN;
	CEdit m_editSsid;
	CEdit m_editPasswd;
	// Wifi���Խ����ʾ
	CEdit m_editWifiTestResult;
	// LED���Խ����ʾ
	CEdit m_editLedTestResult;
	// �������Խ����ʾ
	CEdit m_editKeyTestResult;
	// ���Ų��Խ����ʾ
	CEdit m_editPlayTestResult;
	// ¼�����Խ����ʾ
	CEdit m_editRecordTestResult;
	// �����������ʾ
	CEdit m_editPowerTestResult;
	// �汾��ʾ
	CEdit m_editSoftwareVersion;
	// SD�����Խ��
	CEdit m_editSDTestResult;
	// ��ʼ���԰�ť
	CButton m_buttonStartTest;
	// �������Բ�����ť
	CButton m_buttonKeyTest;
	// ������Ϣ�ϴ�MESϵͳѡ��ؼ�
	CButton m_checkUploadMes;
	// wifi����ѡ��ؼ�
	CButton m_checkWifiTest;
	// ��������ѡ��ؼ�
	CButton m_checkKeyTest;
	// ���Ų����Ƿ�ѡ��ؼ�
	CButton m_checkPlayTest;
	// Led����ѡ��ؼ�
	CButton m_checkLedTest;
	// ¼������ѡ��ؼ�
	CButton m_checkRecordTest;
	// ��������ѡ��ؼ�
	CButton m_checkPowerTest;
	// SD������ѡ��ؼ�
	CButton m_checkSdCardTest;
	CEdit m_editRssiMin;
	CEdit m_editRssiMax;
	CEdit m_editRssiReal;
	CEdit m_editMbpsMin;
	CEdit m_editMbpsMax;
	CEdit m_editMbpsReal;
	// MBPS������Сֵ
	CEdit m_editMbpsMin2;
	// MBPS�������ֵ
	CEdit m_editMbpsMax2;
	// MBPS���Խ���ֵ
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
	CEdit m_editPowerTarget;/*�ĳ�������*/
	CEdit m_editPowerVari;/*�ĳ���С����*/
	CEdit m_editCurrPower;

	/*���Ų��Ե��ļ���*/
	CEdit m_editFileName;
	/*�ļ����ͱ�־*/
	CButton m_buttonPushFile;

	CButton m_buttonSweepPass;
	CButton m_buttonSweepFail;
	CEvent m_ceSweepEvent;
	BOOL m_bSweepFlag = FALSE;

	/*��ǰ���Թ�վ��ֵ������enum CurTestStation�ṹ*/
	int m_nStation;

	/*�������߳���Ӧ�ṹ*/
	CEvent m_ceStartEvent;
	CWinThread *m_pctMainThread;
	CEvent m_ceMainEvent;

	CWinThread *m_pctSNCheckThread;
	CEvent m_ceSNCheckEvent;

	/*Iperf3������߳�*/
	CWinThread *m_pctIprefThread;

	/*���ڸ�֪��������ѭ���˳��ı�־*/
	BOOL m_nLiveFlag = FALSE;

	/*�����ж�Fail�����õı�־*/
	BOOL m_nKeyTestFailFlag = FALSE;

	/*����Led���Գ����Ƿ񻹴���*/
	BOOL m_nLedTestThreadLive = FALSE;


	/*���ڸı�������ɫ�ı���*/
	CFont m_fontFont;
	CFont m_buttonFont;

	/*���浱ǰ�����豸IP*/
	CString m_strIp;
	/*���浱ǰ���Ի�����IP*/
	CString m_strHostIp;

	/*�������*/
	TestResult m_strResult;

	/*�˵���*/
	CMenu m_cmMenu;
	BOOL m_bUserRoot = FALSE;
	BOOL m_bPassEATest = FALSE;

	// ѡ��Ҫ���͵���Ƶ�ļ�
	CButton m_buttonFileChoose;

	//SD����д��������ֵ
	CString m_strReadStdVal;
	CString m_strWriteStdVal;

	//Log����·��
	CString m_strLogPath;

	// ��ť����������	// ��ť����������
	CButton m_buttonKnobSucc;
	// ��ť�����쳣����
	CButton m_buttonKnobFail;
	CEvent m_ceKnobEvent;
	BOOL m_bKnobFlag = FALSE;

	PROCESS_INFORMATION m_ProcessInfo;

public:
	/*��ʾϵ�к���*/
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

	/*������Զ���*/
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
	
	//���������
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





