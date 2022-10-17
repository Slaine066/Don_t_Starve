
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif
#include "MainFrm.h"
#include "MainApp.h"
#include "ToolDoc.h"
#include "ToolView.h"

#include "GameInstance.h"
#include "Vertex.h"
#include "Tool_Defines.h"
#include "House.h"
#include "WoodWall.h"
#include "DecoObject.h"
#include "Trap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWnd;
HINSTANCE g_hInst;


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
	: m_pGameInstance (CGameInstance::Get_Instance())
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	Safe_AddRef(m_pGameInstance);
}

CToolView::~CToolView()
{

}

#pragma region ���ʿ�
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

HRESULT CToolView::Ready_Window_Size()
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;

	RECT rcMainFrame, rcToolVeiw, rcSideView;

	pMainFrame->GetWindowRect(&rcMainFrame);
	GetClientRect(&rcToolVeiw);
	pMainFrame->Get_ClientRect(0, 1, &rcSideView);

	UINT iFrameGapWidth = (rcMainFrame.right - rcMainFrame.left) - (rcToolVeiw.right + rcSideView.right);
	UINT iFrameGapHeight = (rcMainFrame.bottom - rcMainFrame.top) - rcToolVeiw.bottom;

	if (FALSE == pMainFrame->SetWindowPos(nullptr, 0, 0, g_iToolViewCX + g_iSideViewCX + iFrameGapWidth, g_iToolViewCY + iFrameGapHeight, SWP_NOMOVE))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolView::Ready_Layer_Terrain(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;

	CVIBuffer_Terrain::TERRAINDESC		TerrainDesc;

	CVertex* pVertex = (CVertex*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Vertex"));
	_float3 TerrainPos;
	if (false == pVertex->Picking(&TerrainPos))
		return E_FAIL;

	TerrainDesc.m_iPosVerticesX = (_uint)TerrainPos.x;
	TerrainDesc.m_iPosVerticesZ = (_uint)TerrainPos.z;
	TerrainDesc.m_fSizeX = (_float)pMainFrame->Get_TerrainSizeX();
	TerrainDesc.m_fSizeZ = (_float)pMainFrame->Get_TerrainSizeZ();
	TerrainDesc.m_fTextureSize = pMainFrame->Get_TextureSize();
	TerrainDesc.m_iNumVerticesX = pMainFrame->Get_NumVerticesX();
	TerrainDesc.m_iNumVerticesZ = pMainFrame->Get_NumVerticesZ();
	TerrainDesc.m_iTextureNum = pMainFrame->Get_TerrainTextureNum();


	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerTerrain(LayerTag, &TerrainDesc);

	Safe_Release(pMainApp);

	return S_OK;
}

HRESULT CToolView::Ready_Layer_Grass(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;


	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
		return E_FAIL;

	TerrainPos.y += 0.5;
	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerGrass(LayerTag, &TerrainPos);

	Safe_Release(pMainApp);
	return S_OK;
}

HRESULT CToolView::Ready_Layer_Rock(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;


	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
		return E_FAIL;

	TerrainPos.y += 0.5;
	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerRock(LayerTag, &TerrainPos);

	Safe_Release(pMainApp);
	return S_OK;
	return S_OK;
}

HRESULT CToolView::Ready_Layer_Tree(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;


	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
		return E_FAIL;

	TerrainPos.y += 0.5;
	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerTree(LayerTag, &TerrainPos);

	Safe_Release(pMainApp);
	return S_OK;
}

HRESULT CToolView::Ready_Layer_BerryBush(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;

	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
	return E_FAIL;

	TerrainPos.y += 0.5;
	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerBerryBush(LayerTag, &TerrainPos);

	Safe_Release(pMainApp);
	return S_OK;
}

HRESULT CToolView::Ready_Layer_House(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;

	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
		return E_FAIL;
	TerrainPos.y += 0.5;

	CHouse::HOUSEDECS HouseDesc;
	HouseDesc.m_eState = CHouse::HOUSETYPE(pMainFrame->Get_HouseType());
	HouseDesc.vInitPosition = TerrainPos;


	
	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerHouse(LayerTag, &HouseDesc);

	Safe_Release(pMainApp);
	
	return S_OK;
}

HRESULT CToolView::Ready_Layer_Pig(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;

	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
		return E_FAIL;

	TerrainPos.y += 0.8;
	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerPig(LayerTag, &TerrainPos);

	Safe_Release(pMainApp);
	return S_OK;
}

HRESULT CToolView::Ready_Layer_Spider(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;

	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
		return E_FAIL;

	TerrainPos.y += 0.5;
	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerSpider(LayerTag, &TerrainPos);

	Safe_Release(pMainApp);
	return S_OK;
}

HRESULT CToolView::Ready_Layer_Wall(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;

	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	
	
	CWoodWall::WALLDESC WallDesc;
	

	if (false == pTerrain->Picking(&WallDesc.vecPosition))
		return E_FAIL;
	WallDesc.vecPosition.y += 1.f;
	WallDesc.etype = CWoodWall::WALL_ROCK;

	if (pMainFrame->Get_WallSide_BeChecked())
		WallDesc.eDir = CWoodWall::SIDE;
	else
		WallDesc.eDir = CWoodWall::FRONT;


	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerWall(LayerTag, &WallDesc);

	Safe_Release(pMainApp);
	return S_OK;
}

HRESULT CToolView::Ready_Layer_Carrot(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;


	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
		return E_FAIL;

	TerrainPos.y += 0.5;
	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerCarrot(LayerTag, &TerrainPos);

	Safe_Release(pMainApp);
	return S_OK;
}

HRESULT CToolView::Ready_Layer_DecoObject(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;

	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
		return E_FAIL;
	TerrainPos.y += 0.01;

	CDecoObject::DECODECS DecoDesc;
	DecoDesc.m_eState = CDecoObject::DECOTYPE(pMainFrame->GetDecoType());
	DecoDesc.vInitPosition = TerrainPos;

	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerDeco(LayerTag, &DecoDesc);

	Safe_Release(pMainApp);

	return S_OK;
}

HRESULT CToolView::Ready_Layer_Dirt(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;


	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
		return E_FAIL;

	TerrainPos.y += 0.5;
	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerDirt(LayerTag, &TerrainPos);

	Safe_Release(pMainApp);
	return S_OK;
}

HRESULT CToolView::Ready_Layer_Trap(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;

	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
		return E_FAIL;
	TerrainPos.y += 0.01;

	CTrap::TRAPDESC DecoDesc;
	DecoDesc.eType = CTrap::TRAPTYPE(pMainFrame->GetTrapType());
	DecoDesc.vInitPosition = TerrainPos;

	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerTrap(LayerTag, &DecoDesc);

	Safe_Release(pMainApp);

	return S_OK;
}

HRESULT CToolView::Ready_Layer_Card(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;


	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
		return E_FAIL;

	TerrainPos.y += 0.5;
	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerCard(LayerTag, &TerrainPos);

	Safe_Release(pMainApp);
	return S_OK;
}

HRESULT CToolView::Ready_Layer_Egg(const _tchar * LayerTag)
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;


	CTerrain* pTerrain = (CTerrain*)m_pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Terrain"));
	_float3 TerrainPos;
	if (false == pTerrain->Picking(&TerrainPos))
		return E_FAIL;

	TerrainPos.y += 0.5;
	CMainApp* pMainApp = CMainApp::Get_Instance();
	Safe_AddRef(pMainApp);
	pMainApp->Add_LayerEgg(LayerTag, &TerrainPos);

	Safe_Release(pMainApp);
	return S_OK;
}


CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


#pragma endregion ���ʿ�

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	g_hWnd = GetSafeHwnd();
	g_hInst = AfxGetInstanceHandle();
	CMainApp::Get_Instance()->Initialize(g_hWnd, g_hInst);

	if (FAILED(Ready_Window_Size()))
		return;
}

// CToolView �׸���

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

}




void CToolView::OnDestroy()
{
	
	CView::OnDestroy();
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Safe_Release(m_pTerrain);
	Safe_Release(m_pGameInstance);
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return;

	Tool::OBJECTID eObjectID = pMainFrame->Get_ObjectID();

	switch (eObjectID)
	{
	case Tool::OBJ_TERRAIN:
		if (FAILED(Ready_Layer_Terrain(L"Layer_Terrain")))
			return;
		break;
	case  Tool::OBJ_ROCK:
		if (FAILED(Ready_Layer_Rock(L"Layer_Rock")))
			return;
		break;
	case Tool::OBJ_BERRYBUSH:
		if (FAILED(Ready_Layer_BerryBush(L"Layer_BerryBush")))
			return;
		break;
	case Tool::OBJ_TREE:
		if (FAILED(Ready_Layer_Tree(L"Layer_Tree")))
			return;
		break;
	case Tool::OBJ_GRASS:
		if (FAILED(Ready_Layer_Grass(L"Layer_Grass")))
			return;
		break;
	case Tool::OBJ_HOUSE:
		if (FAILED(Ready_Layer_House(L"Layer_House")))
			return;
		break;
	case Tool::OBJ_PIG:
		if (FAILED(Ready_Layer_Pig(L"Layer_Pig")))
			return;
		break;
	case Tool::OBJ_SPIDER:
		if (FAILED(Ready_Layer_Spider(L"Layer_Spider")))
			return;
		break;
	case Tool::OBJ_WALL:
		if (FAILED(Ready_Layer_Wall(L"Layer_Wall")))
			return;
		break;
	case Tool::OBJ_CARROT:
		if (FAILED(Ready_Layer_Carrot(L"Layer_Carrot")))
			return;
		break;
	case Tool::OBJ_DECO:
		if (FAILED(Ready_Layer_DecoObject(L"Layer_Deco")))
			return;
		break;
	case Tool::OBJ_DIRT:
		if (FAILED(Ready_Layer_Dirt(L"Layer_Dirt")))
			return;
		break;
	case Tool::OBJ_TRAP:
		if (FAILED(Ready_Layer_Trap(L"Layer_Trap")))
			return;
		break;
	case Tool::OBJ_CARNIVAL:
		if (FAILED(Ready_Layer_Card(L"Layer_Card")))
			return;
		break;
	case Tool::OBJ_EGG:
		if (FAILED(Ready_Layer_Egg(L"Layer_Egg")))
			return;
		break;
	}

	CView::OnLButtonDown(nFlags, point);
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	
	


	CView::OnMouseMove(nFlags, point);
}


void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	// Z : �ֱ� ���� ����
	if ('Z' == nChar)
		if (FAILED(Erase_CurrentTerrain()))
			return;
	
	if ('Q' == nChar)
		if (FAILED(Rotate_Object_Plus()))
			return;

	if ('E' == nChar)
		if (FAILED(Rotate_Object_Minus()))
			return;

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

HRESULT CToolView::Erase_CurrentTerrain()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);


	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;

	Tool::OBJECTID eObjectID = pMainFrame->Get_ObjectID();
	list<CGameObject*>* plistClone = nullptr;

	switch (eObjectID)
	{
	case Tool::OBJ_TERRAIN:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Terrain");
		break;
	case  Tool::OBJ_ROCK:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Rock");
		break;
	case Tool::OBJ_BERRYBUSH:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_BerryBush");
		break;
	case Tool::OBJ_TREE:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Tree");
		break;
	case Tool::OBJ_GRASS:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Grass");
		break;
	case Tool::OBJ_HOUSE:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_House");
		break;
	case Tool::OBJ_PIG:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Pig");
		break;
	case Tool::OBJ_SPIDER:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Spider");		
		break;
	case Tool::OBJ_WALL:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Wall");
		break;
	case Tool::OBJ_CARROT:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Carrot");
		break;
	case Tool::OBJ_DECO:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Deco");
		break;
	case Tool::OBJ_DIRT:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Dirt");
		break;
	case Tool::OBJ_TRAP:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Trap");
		break;
	case Tool::OBJ_CARNIVAL:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Card");
		break;
	case Tool::OBJ_EGG:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Egg");
		break;
	}

	if (nullptr == plistClone || true == plistClone->empty())
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	auto iter = --plistClone->end();
	Safe_Release(*iter);

	plistClone->erase(iter);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CToolView::Rotate_Object_Plus()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);


	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;

	Tool::OBJECTID eObjectID = pMainFrame->Get_ObjectID();
	list<CGameObject*>* plistClone = nullptr;

	switch (eObjectID)
	{
	case Tool::OBJ_DECO:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Deco");
		break;
	}

	if (nullptr == plistClone || true == plistClone->empty())
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	auto iter = --plistClone->end();
	CDecoObject::DECOTYPE eType =   dynamic_cast<CDecoObject*>(*iter)->Get_Desc().m_eState;
	if (eType == CDecoObject::DECOTYPE::FLOORFIRE)
		dynamic_cast<CDecoObject*>(*iter)->Add_Rotate();

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CToolView::Rotate_Object_Minus()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);


	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == pMainFrame)
		return E_FAIL;

	Tool::OBJECTID eObjectID = pMainFrame->Get_ObjectID();
	list<CGameObject*>* plistClone = nullptr;

	switch (eObjectID)
	{
	case Tool::OBJ_DECO:
		plistClone = pGameInstance->Get_ObjectList(LEVEL_STATIC, L"Layer_Deco");
		break;
	}

	if (nullptr == plistClone || true == plistClone->empty())
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	auto iter = --plistClone->end();
	CDecoObject::DECOTYPE eType = dynamic_cast<CDecoObject*>(*iter)->Get_Desc().m_eState;
	if (eType == CDecoObject::DECOTYPE::FLOORFIRE)
		dynamic_cast<CDecoObject*>(*iter)->Minus_Rotate();

	Safe_Release(pGameInstance);

	return S_OK;
}
