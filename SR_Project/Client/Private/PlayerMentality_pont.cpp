#include "stdafx.h"
#include "..\Public\PlayerMentality_pont.h"
#include "GameInstance.h"
#include "Inventory.h"
#include "Player.h"



CPlayerMentality_pont::CPlayerMentality_pont(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPlayerMentality_pont::CPlayerMentality_pont(const CPlayerMentality_pont & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayerMentality_pont::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerMentality_pont::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	iNumber = (int*)pArg;

	iNum = *iNumber;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);

	m_fSizeX = 20.0f;
	m_fSizeY = 20.0f;
	m_fX = 1115.f + (iNum * 15.f);
	m_fY = 190.f;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	return S_OK;
}

int CPlayerMentality_pont::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//RECT		rcRect;
	//SetRect(&rcRect, (int)(m_fX - m_fSizeX * 0.5f), (int)(m_fY - m_fSizeY * 0.5f), (int)(m_fX + m_fSizeX * 0.5f), (int)(m_fY + m_fSizeY * 0.5f));

	//POINT		ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);

	//if (PtInRect(&rcRect, ptMouse))
	//{

	//	m_fSizeX = 35.f;
	//	m_fSizeY = 35.f;
	//	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//	//set_check(true);
	//}




	//if()
	return OBJ_NOEVENT;
}

void CPlayerMentality_pont::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	//RECT		rcRect;
	//SetRect(&rcRect, (int)(m_fX - m_fSizeX * 0.5f), (int)(m_fY - m_fSizeY * 0.5f), (int)(m_fX + m_fSizeX * 0.5f), (int)(m_fY + m_fSizeY * 0.5f));

	//POINT		ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);

	//if (!PtInRect(&rcRect, ptMouse))
	//{
	//	m_fSizeX = 20;
	//	m_fSizeY = 20;
	//	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);


	//	//ERR_MSG(L"�浹");
	//}
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_uint playermental = (_uint)(dynamic_cast<CPlayer*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")))->Get_Player_Stat().fCurrentMental);

	if (playermental< 100 && iNum < 1)
	{
		m_bcheck = false;
	}
	else if (playermental < 10 && iNum < 2)
	{
		m_bcheck = false;
	}
	else
		m_bcheck = true;

	

	if (nullptr != m_pRendererCom)//&&m_bcheck)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	//set_check(false);
}

HRESULT CPlayerMentality_pont::Render()
{
	
	if (m_bcheck == true)
	{
		if (FAILED(__super::Render()))
			return E_FAIL;

		if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
			return E_FAIL;

		_float4x4		ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);

		m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(texnum)))
			return E_FAIL;

		if (FAILED(SetUp_RenderState()))
			return E_FAIL;

		m_pVIBufferCom->Render();

		if (FAILED(Release_RenderState()))
			return E_FAIL;


	}
	
	



	return S_OK;
}

HRESULT CPlayerMentality_pont::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HpPont"), (CComponent**)&m_pTextureCom)))
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

HRESULT CPlayerMentality_pont::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CPlayerMentality_pont::Release_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CPlayerMentality_pont * CPlayerMentality_pont::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayerMentality_pont*	pInstance = new CPlayerMentality_pont(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPlayerMentality_pont"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayerMentality_pont::Clone(void* pArg)
{
	CPlayerMentality_pont*	pInstance = new CPlayerMentality_pont(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPlayerMentality_pont"));
		Safe_Release(pInstance);
	}
	CInventory_Manager::Get_Instance()->Get_playermentality_Pont_list()->push_back(pInstance);
	return pInstance;
}
void CPlayerMentality_pont::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}