#include "stdafx.h"
#include "..\Public\Deadcountpont.h"
#include "GameInstance.h"
#include "Inventory.h"


CDeadcountpont::CDeadcountpont(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CDeadcountpont::CDeadcountpont(const CDeadcountpont & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDeadcountpont::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeadcountpont::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	iNumber = (int*)pArg;

	iNum = *iNumber;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);

	m_fSizeX = 35.0f;
	m_fSizeY = 35.0f;
	m_fX = 320.f + (iNum * 35.f);
	m_fY = 680.f;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	return S_OK;
}

int CDeadcountpont::Tick(_float fTimeDelta)
{
	if (m_bcheck == true)
	{
		__super::Tick(fTimeDelta);

		if (m_fY >= 680.f)
			Open_DeadUI(fTimeDelta);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

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
	}
	else
		m_fY = 825.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	return OBJ_NOEVENT;
}

void CDeadcountpont::Late_Tick(_float fTimeDelta)
{
	if (m_bcheck == true)
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

		if (GetTickCount() > m_dwdeadcount + 1000)
		{
			--count;
			CGameInstance* pInstance = CGameInstance::Get_Instance();
			pInstance->PlaySounds(TEXT("deadcountsound.wav"), SOUND_UI, 0.7f);
			m_dwdeadcount = GetTickCount();
		}


		if (iNum == 0)
		{
			texnum = (count % 100) / 10;

			if (texnum <= 0)
				texnum = 30;

		}

		else if (iNum == 1)
			texnum = (count % 10);

		if (nullptr != m_pRendererCom&&m_bcheck)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}

	//set_check(false);
}

HRESULT CDeadcountpont::Render()
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

HRESULT CDeadcountpont::SetUp_Components()
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

HRESULT CDeadcountpont::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CDeadcountpont::Release_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CDeadcountpont * CDeadcountpont::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDeadcountpont*	pInstance = new CDeadcountpont(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CDeadcountpont"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDeadcountpont::Clone(void* pArg)
{
	CDeadcountpont*	pInstance = new CDeadcountpont(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CDeadcountpont"));
		Safe_Release(pInstance);
	}
	CInventory_Manager::Get_Instance()->Get_Deadcountpont_list()->push_back(pInstance);
	return pInstance;
}


void CDeadcountpont::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}