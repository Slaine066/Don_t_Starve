
// Tool.h : Tool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "MainApp.h"

// CToolApp:
// �� Ŭ������ ������ ���ؼ��� Tool.cpp�� �����Ͻʽÿ�.
//

class CToolApp : public CWinApp
{
public:
	CToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);

	CMainApp*		pMainApp = nullptr;
};

extern CToolApp theApp;
