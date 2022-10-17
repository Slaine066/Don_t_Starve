
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"
#include "ToolView.h"
#include "MyForm.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

HWND g_Main_hWnd;
HINSTANCE g_Main_hInst;

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

Tool::OBJECTID CMainFrame::Get_ObjectID()
{
	CMyForm* pFormView = dynamic_cast<CMyForm*>(m_MainSplitter.GetPane(0, 1));
	if (nullptr == pFormView)
		return Tool::OBJECTID();

	return pFormView->Get_ObjectID();
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
void CMainFrame::Get_ClientRect(UINT iRow, UINT iCol, RECT * pRect)
{
	CWnd* pView = m_MainSplitter.GetPane(iRow, iCol);
	if (nullptr == pView)
		return;

	pView->GetClientRect(pRect);
}


int CMainFrame::Get_TerrainTextureNum()
{
	CMyForm* pFormView = dynamic_cast<CMyForm*>(m_MainSplitter.GetPane(0, 1));
	if (nullptr == pFormView)
		return -1;

	return pFormView->Get_TerrainTexture();
}
int CMainFrame::Get_NumVerticesX()
{
	CMyForm* pFormView = dynamic_cast<CMyForm*>(m_MainSplitter.GetPane(0, 1));
	if (nullptr == pFormView)
		return -1;

	return pFormView->Get_NumVerticesX();
}
int CMainFrame::Get_NumVerticesZ()
{
	CMyForm* pFormView = dynamic_cast<CMyForm*>(m_MainSplitter.GetPane(0, 1));
	if (nullptr == pFormView)
		return -1;

	return pFormView->Get_NumVerticesZ();
}
int CMainFrame::Get_TerrainSizeX()
{
	CMyForm* pFormView = dynamic_cast<CMyForm*>(m_MainSplitter.GetPane(0, 1));
	if (nullptr == pFormView)
		return -1;

	return pFormView->Get_TerrainSizeX();
}
int CMainFrame::Get_TerrainSizeZ()
{
	CMyForm* pFormView = dynamic_cast<CMyForm*>(m_MainSplitter.GetPane(0, 1));
	if (nullptr == pFormView)
		return -1;

	return pFormView->Get_TerrainSizeZ();
}
float CMainFrame::Get_TextureSize()
{
	CMyForm* pFormView = dynamic_cast<CMyForm*>(m_MainSplitter.GetPane(0, 1));
	if (nullptr == pFormView)
		return -1;

	return pFormView->Get_TextureSize();
}
bool CMainFrame::Get_WallSide_BeChecked()
{
	CMyForm* pFormView = dynamic_cast<CMyForm*>(m_MainSplitter.GetPane(0, 1));
	if (nullptr == pFormView)
		return false;

	return pFormView->Get_WallSide_BeChecked();
}
HOUSETYPE CMainFrame::Get_HouseType()
{
	CMyForm* pFormView = dynamic_cast<CMyForm*>(m_MainSplitter.GetPane(0, 1));
	if (nullptr == pFormView)
		return HOUSE_END;

	return pFormView->GetHouseType();
}
DECOTYPE CMainFrame::GetDecoType()
{
	CMyForm* pFormView = dynamic_cast<CMyForm*>(m_MainSplitter.GetPane(0, 1));
	if (nullptr == pFormView)
		return DECO_END;

	return pFormView->GetDecoType();
}
TRAPTYPE CMainFrame::GetTrapType()
{
	CMyForm* pFormView = dynamic_cast<CMyForm*>(m_MainSplitter.GetPane(0, 1));
	if (nullptr == pFormView)
		return TRAP_MAX;

	return pFormView->GetTrapType();
}
#endif //_DEBUG


// CMainFrame �޽��� ó����



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	g_Main_hWnd = GetSafeHwnd();
	g_Main_hInst = AfxGetInstanceHandle();

	m_MainSplitter.CreateStatic(this, 1, 2);

	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CToolView), CSize(g_iToolViewCX, g_iToolViewCY), pContext);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyForm), CSize(g_iSideViewCX, g_iSideViewCY), pContext);

	m_MainSplitter.SetColumnInfo(0, 1080, 500);
	/*m_pMainView = (CToolView*)m_MainSplitter.GetPane(0, 1);
	m_pMyFormView = (CMyForm*)m_MainSplitter.GetPane(0, 0);*/

	return TRUE;
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	m_MainSplitter.DestroyWindow();
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
