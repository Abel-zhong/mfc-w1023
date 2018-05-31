#include "StdAfx.h"  
#include "CmdShell.h"


CCmdshell::CCmdshell(void)
{
}

CCmdshell::~CCmdshell(void)
{
	if (m_nFlag) {
		TerminateProcess(m_pInfo.hProcess, 0);
		CloseHandle(m_hWritePipe);
		CloseHandle(m_hReadPipe);
		CloseHandle(m_pInfo.hThread);
		CloseHandle(m_pInfo.hProcess);
		m_nFlag = FALSE;
	}
}

//启动adb shell，如果成功则进入#状态，可以使用RunCmd执行命令  
BOOL CCmdshell::Start(CString &strCmd)
{
	SECURITY_ATTRIBUTES sat;
	STARTUPINFO startupinfo;
	//PROCESS_INFORMATION pinfo;
	CString strTemp;

	m_strOutput.Empty();

	strTemp.Format(_T("%s"), strCmd);
	sat.nLength = sizeof(SECURITY_ATTRIBUTES);
	sat.bInheritHandle = TRUE;
	sat.lpSecurityDescriptor = NULL;
	if (CreatePipe(&m_hReadPipe, &m_hWritePipe, &sat, NULL) == FALSE) {
		return FALSE;
	}
	//if (CreatePipe(&m_hReadPipe2, &m_hWritePipe2, &sat, NULL) == FALSE) {
		//return FALSE;
	//}
	startupinfo.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&startupinfo);
	startupinfo.hStdError = m_hWritePipe;
	startupinfo.hStdOutput = m_hWritePipe;
	startupinfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupinfo.wShowWindow = SW_HIDE;
	if (CreateProcess(NULL, (LPSTR)(LPCTSTR)strTemp, NULL, NULL, TRUE, 0, NULL, NULL, &startupinfo, &m_pInfo) == FALSE) {
		//DWORD dwError = GetLastError();  
		return FALSE;
	}
	m_nFlag = TRUE;
	CloseHandle(m_hWritePipe);
	//获取管道信息  
	CHAR buffer[1024];
	DWORD    byteRead;
	while (TRUE)
	{
		RtlZeroMemory(buffer, _countof(buffer));
		if (ReadFile(m_hReadPipe, buffer, _countof(buffer), &byteRead, NULL) == FALSE)
		{
			m_strOutput += buffer;
			m_nFlag = FALSE;
			break;
		}
		m_strOutput += buffer;
    }
	//Sleep(10);

    CloseHandle(m_hReadPipe);
	CloseHandle(m_pInfo.hThread);
	CloseHandle(m_pInfo.hProcess);
	m_strOutput.ReleaseBuffer();

	return TRUE;
}


//启动adb shell，如果成功则进入#状态，可以使用RunCmd执行命令  

BOOL CCmdshell::StartWithoutTimeout(CString &strCmd, int timeout)
{
	SECURITY_ATTRIBUTES sat;
	STARTUPINFO startupinfo;
	//PROCESS_INFORMATION pinfo;
	CString strTemp;

	m_strOutput.Empty();

	strTemp.Format(_T("%s"), strCmd);
	sat.nLength = sizeof(SECURITY_ATTRIBUTES);
	sat.bInheritHandle = TRUE;
	sat.lpSecurityDescriptor = NULL;
	if (CreatePipe(&m_hReadPipe, &m_hWritePipe, &sat, NULL) == FALSE) {
		return FALSE;
	}

	startupinfo.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&startupinfo);
	startupinfo.hStdError = m_hWritePipe;
	startupinfo.hStdOutput = m_hWritePipe;
	startupinfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupinfo.wShowWindow = SW_HIDE;
	if (CreateProcess(NULL, (LPSTR)(LPCTSTR)strTemp, NULL, NULL, TRUE, 0, NULL, NULL, &startupinfo, &m_pInfo) == FALSE) {
		//DWORD dwError = GetLastError();  
		return FALSE;
	}
	m_nFlag = TRUE;

	Sleep(timeout * 1000);
	m_nFlag = FALSE;
	TerminateProcess(m_pInfo.hProcess, 0);
	CloseHandle(m_hWritePipe);
	CloseHandle(m_hReadPipe);
	CloseHandle(m_pInfo.hThread);
	CloseHandle(m_pInfo.hProcess);

	return TRUE;
}



//读取输出结果,调用前请务必调用Stop先结束  
CString CCmdshell::GetOutput()
{
	int nPos1 = 0;

	nPos1 = m_strOutput.Find(_T("Usage:"));
	if (nPos1 >= 0) {
		m_strOutput = m_strOutput.Left(m_strOutput.Find(_T("Usage:")) + 6);
	}
	//m_strOutput = m_strOutput.Right(m_strOutput.GetLength() - 1 - m_strOutput.Find(_T("\r\n")));


	while ((m_strOutput.Right(1) == '\r') || (m_strOutput.Right(1) == '\n') || (m_strOutput.Right(1) == ' '))
	{
		m_strOutput = m_strOutput.Left(m_strOutput.GetLength() - 1);
	}

	while ((m_strOutput.Right(1) == '\r') || (m_strOutput.Right(1) == '\n') || (m_strOutput.Right(1) == ' '))
	{
		m_strOutput = m_strOutput.Right(m_strOutput.GetLength() - 1);
	}

	return m_strOutput;
}

