
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "Tool_Defines.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:
	Tool::OBJECTID Get_ObjectID();
// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	void Get_ClientRect(UINT iRow, UINT iCol, RECT* pRect);
	int Get_TerrainTextureNum();
	int Get_NumVerticesX();
	int Get_NumVerticesZ();
	int Get_TerrainSizeX();
	int Get_TerrainSizeZ();
	float Get_TextureSize();
	bool Get_WallSide_BeChecked();
	HOUSETYPE Get_HouseType();
	DECOTYPE  GetDecoType();
	TRAPTYPE GetTrapType();

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

private:
	CSplitterWnd	m_MainSplitter;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnDestroy();
};


