
// W-1023����.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CW1023����App: 
// �йش����ʵ�֣������ W-1023����.cpp
//

class CW1023����App : public CWinApp
{
public:
	CW1023����App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CW1023����App theApp;