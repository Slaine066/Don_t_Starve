#include "stdafx.h"
#include "..\Public\Craftmain_back.h"
#include "GameInstance.h"
#include "Inventory.h"

CCraftmain_back::CCraftmain_back(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CCraftmain_back::CCraftmain_back(const CCraftmain_back & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCraftmain_back::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCraftmain_back::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	iNumber = (int*)pArg;

	iNum = *iNumber;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);

	m_fSizeX = 55.0f;
	m_fSizeY = 55.0f;
	m_fX = 230.f + (iNum * 65.f);
	m_fY = 0.f;


	m_firstx = m_fX;
	m_firsty = m_fY;
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));



	return S_OK;
}

int CCraftmain_back::Tick(_float fTimeDelta)
{
	if (m_bonof == false)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);

	if (m_makewhat == MAKE_AXE || m_makewhat == MAKE_HAMBAT || m_makewhat == MAKE_FENCE || m_makewhat == MAKE_ROPE)
		m_fY = 155.f;
	else if (m_makewhat == MAKE_PICK || m_makewhat == MAKE_SHOTTER || m_makewhat == MAKE_POT || m_makewhat == MAKE_COAL)
		m_fY = 205.f;
	else if (m_makewhat == MAKE_STAFF || m_makewhat == MAKE_TENT || m_makewhat == MAKE_TORCH)
		m_fY = 255.f;
	else if (m_makewhat == MAKE_ARMOR)
		m_fY = 305.f;
	else if (m_makewhat == MAKE_HELMET)
		m_fY = 355.f;

	if (m_makewhat == MAKE_ROPE || m_makewhat == MAKE_COAL || m_makewhat == MAKE_TORCH)
	{
		//if(iNum == 0)
		//	setcolor();
		m_fX = 262.5f;
		if (iNum == 1)
		{
			m_bonof = false;
			return OBJ_NOEVENT;
		}

	}


	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	RECT		rcRect;
	SetRect(&rcRect, (int)(m_fX - m_fSizeX * 0.5f), (int)(m_fY - m_fSizeY * 0.5f), (int)(m_fX + m_fSizeX * 0.5f), (int)(m_fY + m_fSizeY * 0.5f));

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	//if (m_fY <= 155.f)
	//Open_Craft(fTimeDelta);

	//if (m_fX <= 100)
	//	Open_Weapontool(fTimeDelta);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));


	/*if (PtInRect(&rcRect, ptMouse))
	{
	m_fSizeX = 55.f;
	m_fSizeY = 55.f;
	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	backtexnum = 0;


	}*/

	setcolor();


	return OBJ_NOEVENT;
}

void CCraftmain_back::Late_Tick(_float fTimeDelta)
{

	if (m_bonof == true)
	{

		__super::Late_Tick(fTimeDelta);
		RECT		rcRect;
		SetRect(&rcRect, (int)(m_fX - m_fSizeX * 0.5f), (int)(m_fY - m_fSizeY * 0.5f), (int)(m_fX + m_fSizeX * 0.5f), (int)(m_fY + m_fSizeY * 0.5f));

		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);



		if (nullptr != m_pRendererCom &&m_bonof == true)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}


}

HRESULT CCraftmain_back::Render()
{

	if (m_bonof == false)
		return OBJ_NOEVENT;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;

	_float4x4		ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (FAILED(m_pTextureCom->Bind_OnGraphicDev(backtexnum)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return S_OK;
}

void CCraftmain_back::setcolor()
{

	CInventory_Manager*         pInventory_Manager = CInventory_Manager::Get_Instance();
	//Safe_AddRef(pInventory_Manager);


	auto pinven = pInventory_Manager->Get_Inven_list();


	if (m_makewhat == MAKE_AXE || m_makewhat == MAKE_PICK)
	{
		for (auto iter = pinven->begin(); iter != pinven->end(); ++iter)
		{
			if (iNum == 0)
			{
				if ((*iter)->get_texnum() == ITEMNAME_WOOD && (*iter)->get_item_number() >= 1) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}

			else if (iNum == 1)
			{
				if ((*iter)->get_texnum() == ITEMNAME_ROCK2 && (*iter)->get_item_number() >= 1) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}


		}
	}

	else if (m_makewhat == MAKE_ARMOR || m_makewhat == MAKE_HELMET)
	{
		for (auto iter = pinven->begin(); iter != pinven->end(); ++iter)
		{
			if (iNum == 0)
			{
				if ((*iter)->get_texnum() == ITEMNAME_WOOD && (*iter)->get_item_number() >= 1) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}

			else if (iNum == 1)
			{
				if ((*iter)->get_texnum() == ITEMNAME_ROCK2 && (*iter)->get_item_number() >= 3) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}


		}
	}

	else if (m_makewhat == MAKE_HAMBAT)
	{
		for (auto iter = pinven->begin(); iter != pinven->end(); ++iter)
		{
			if (iNum == 0)
			{
				if ((*iter)->get_texnum() == ITEMNAME_WOOD && (*iter)->get_item_number() >= 1) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}

			else if (iNum == 1)
			{
				if ((*iter)->get_texnum() == ITEMNAME_MEAT && (*iter)->get_item_number() >= 3) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}


		}
	}

	else if (m_makewhat == MAKE_SHOTTER)
	{
		for (auto iter = pinven->begin(); iter != pinven->end(); ++iter)
		{
			if (iNum == 0)
			{
				if ((*iter)->get_texnum() == ITEMNAME_GRASS && (*iter)->get_item_number() >= 2) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}

			else if (iNum == 1)
			{
				if ((*iter)->get_texnum() == ITEMNAME_ROPE && (*iter)->get_item_number() >= 2) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}


		}
	}

	else if (m_makewhat == MAKE_STAFF)
	{
		for (auto iter = pinven->begin(); iter != pinven->end(); ++iter)
		{
			if (iNum == 0)
			{
				if ((*iter)->get_texnum() == ITEMNAME_WOOD && (*iter)->get_item_number() >= 2) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}

			else if (iNum == 1)
			{
				if ((*iter)->get_texnum() == ITEMNAME_GOLD && (*iter)->get_item_number() >= 2) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}


		}
	}

	else if (m_makewhat == MAKE_FENCE)
	{
		for (auto iter = pinven->begin(); iter != pinven->end(); ++iter)
		{
			if (iNum == 0)
			{
				if ((*iter)->get_texnum() == ITEMNAME_WOOD && (*iter)->get_item_number() >= 1) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}

			else if (iNum == 1)
			{
				if ((*iter)->get_texnum() == ITEMNAME_ROPE && (*iter)->get_item_number() >= 1) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}


		}
	}

	else if (m_makewhat == MAKE_POT)
	{
		for (auto iter = pinven->begin(); iter != pinven->end(); ++iter)
		{
			if (iNum == 0)
			{
				if ((*iter)->get_texnum() == ITEMNAME_ROCK2 && (*iter)->get_item_number() >= 1) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}

			else if (iNum == 1)
			{
				if ((*iter)->get_texnum() == ITEMNAME_PIGTAIL && (*iter)->get_item_number() >= 1) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}


		}
	}

	else if (m_makewhat == MAKE_TENT)
	{
		for (auto iter = pinven->begin(); iter != pinven->end(); ++iter)
		{
			if (iNum == 0)
			{
				if ((*iter)->get_texnum() == ITEMNAME_GRASS && (*iter)->get_item_number() >= 2) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}

			else if (iNum == 1)
			{
				if ((*iter)->get_texnum() == ITEMNAME_ROPE && (*iter)->get_item_number() >= 1) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					break;
				}
				else
					backtexnum = 1;
			}


		}
	}
	else if (m_makewhat == MAKE_ROPE)
	{
		for (auto iter = pinven->begin(); iter != pinven->end(); ++iter)
		{
			if (iNum == 0)
			{
				if ((*iter)->get_texnum() == ITEMNAME_GRASS && (*iter)->get_item_number() >= 1) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					return;
				}

			}


		}
		backtexnum = 1;
	}
	else if (m_makewhat == MAKE_COAL)
	{
		for (auto iter = pinven->begin(); iter != pinven->end(); ++iter)
		{
			if (iNum == 0)
			{
				if ((*iter)->get_texnum() == ITEMNAME_ROCK2 && (*iter)->get_item_number() >= 1) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					return;
				}

			}


		}
		backtexnum = 1;
	}
	else if (m_makewhat == MAKE_TORCH)
	{
		for (auto iter = pinven->begin(); iter != pinven->end(); ++iter)
		{
			if (iNum == 0)
			{
				if ((*iter)->get_texnum() == ITEMNAME_WOOD && (*iter)->get_item_number() >= 1) // 있으면 초록! 업승면 빨강! 근데.. 계속돌아서 
				{
					backtexnum = 0;
					return;
				}

			}


		}
		backtexnum = 1;
	}





	//Safe_Release(pInventory_Manager);
}

HRESULT CCraftmain_back::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Craftmain_back"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCraftmain_back::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT CCraftmain_back::Release_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

CCraftmain_back * CCraftmain_back::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCraftmain_back*	pInstance = new CCraftmain_back(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCraftmain_back"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCraftmain_back::Clone(void* pArg)
{
	CCraftmain_back*	pInstance = new CCraftmain_back(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCraftmain_back"));
		Safe_Release(pInstance);
	}

	CInventory_Manager::Get_Instance()->Get_Craftmainback_list()->push_back(pInstance);

	return pInstance;
}


void CCraftmain_back::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}

