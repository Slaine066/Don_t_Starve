#include "stdafx.h"
#include "../Public/House.h"
#include "GameInstance.h"

CHouse::CHouse(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CHouse::CHouse(const CHouse & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHouse::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHouse::Initialize(void* pArg)
{
	ZeroMemory(&m_HouseDesc, sizeof(HOUSEDECS));
	memcpy(&m_HouseDesc, (HOUSEDECS*)pArg, sizeof(HOUSEDECS));

	switch (m_HouseDesc.m_eState)
	{
	case HOUSETYPE::SPIDERHOUSE:
		m_MonsterMaxCount = 4;
		break;
	case HOUSETYPE::BOARONSPAWNER:
		m_MonsterMaxCount = 3;
		break;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(&m_HouseDesc.vInitPosition)))
		return E_FAIL;

	switch (m_HouseDesc.m_eState)
	{
	case HOUSETYPE::BOARONSPAWNER:
	{
		m_pTransformCom->Turn(_float3(1.f, 0.f, 0.f), 1.f);
		m_pTransformCom->Set_Scale(3.f, 3.f, 1.f);
		break;
	}
	case HOUSETYPE::SPIDERHOUSE:
	{
		_float fSize = 3;
		m_pTransformCom->Set_Scale(fSize, fSize, 1.f);
		m_fRadius *= fSize;
		m_fRadius -= 0.4f;
		break;
	}	
	case HOUSETYPE::PIGHOUSE:
	{
		_float fSize = 2;
		m_pTransformCom->Set_Scale(fSize*0.9f, fSize, 1.f);
		m_fRadius *= fSize;
		m_fRadius -= 0.4f;
		break;
	}	
	}

	return S_OK;
}
	

int CHouse::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Tick(fTimeDelta);

	WalkingTerrain();
	Update_Position(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (m_HouseDesc.m_eState == HOUSETYPE::SPIDERHOUSE)
		Spawn_Spider(fTimeDelta);

	return OBJ_NOEVENT;
}

void CHouse::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetUp_BillBoard();

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CHouse::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;

	LEVEL iLevel = (LEVEL)CLevel_Manager::Get_Instance()->Get_CurrentLevelIndex();
	if (iLevel == LEVEL_HUNT && m_HouseDesc.m_eState == PIGHOUSE)
	{
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(1)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(0)))
			return E_FAIL;
	}

	m_pTextureCom->MoveFrame(m_TimerTag);

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHouse::SetUp_Components(void* pArg)
{
	/* For.Com_Texture */
	CTexture::TEXTUREDESC TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(CTexture::TEXTUREDESC));

	switch (m_HouseDesc.m_eState)
	{
	case HOUSETYPE::PIGHOUSE:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Pig_House"), (CComponent**)&m_pTextureCom, &TextureDesc)))
			return E_FAIL;
		break;
	case HOUSETYPE::SPIDERHOUSE:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Spider_House"), (CComponent**)&m_pTextureCom, &TextureDesc)))
			return E_FAIL;
		break;
	case HOUSETYPE::BOARONSPAWNER:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_BOSS, TEXT("Prototype_Component_Texture_Spawner"), (CComponent**)&m_pTextureCom, &TextureDesc)))
			return E_FAIL;
		break;
	}

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	TransformDesc.InitPos = *(_float3*)pArg;

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHouse::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CHouse::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CHouse::SetUp_BillBoard()
{

	if (m_HouseDesc.m_eState == HOUSETYPE::BOARONSPAWNER)
		return;

	_float4x4 ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);	// Get View Matrix
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);		// Get Inverse of View Matrix (World Matrix of Camera)

	_float3 vRight = *(_float3*)&ViewMatrix.m[0][0];
	_float3 vUp = *(_float3*)&ViewMatrix.m[1][0];

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * m_pTransformCom->Get_Scale().x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * m_pTransformCom->Get_Scale().y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);
}

void CHouse::WalkingTerrain()
{
	LEVEL iLevel = (LEVEL)CLevel_Manager::Get_Instance()->Get_CurrentLevelIndex();

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return;
	CVIBuffer_Terrain* pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(iLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"), 0);
	if (nullptr == pVIBuffer_Terrain)
		return;
	CTransform*	pTransform_Terrain = (CTransform*)pGameInstance->Get_Component(iLevel, TEXT("Layer_Terrain"), TEXT("Com_Transform"), 0);

	if (nullptr == pTransform_Terrain)
		return;

	_float3	vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPosition.y = pVIBuffer_Terrain->Compute_Height(vPosition, pTransform_Terrain->Get_WorldMatrix(), m_HouseDesc.m_eState == HOUSETYPE::BOARONSPAWNER ? .01f : m_fRadius);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
}

void CHouse::Spawn_Spider(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	_float3 vTargetPos = pTarget->Get_Position();

	_float m_fDistanceToTarget = D3DXVec3Length(&(Get_Position() - vTargetPos));
	if (m_fDistanceToTarget < 4.f && m_MonsterMaxCount > 0)
	{
		if (m_fSpawnTime < 5.f)
			m_fSpawnTime += fTimeDelta;
		else
		{
			_float3 vPosition = Get_Position();
			vPosition.z -= 0.5f;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Spider_Warrior"), LEVEL_HUNT, TEXT("Layer_Monster"), vPosition)))
				return;

			m_MonsterMaxCount--;
			m_fSpawnTime = 0.f;
		}
	}
}

void CHouse::Spawn_Boaron(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_float3 vSpawnPosition = Get_Position();

	// Spawn Effect
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Spawn_Effect"), LEVEL_BOSS, TEXT("Layer_Effect"), vSpawnPosition)))
		return;
	// Spawn Smoke Effect
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Spawn_Smoke_Effect"), LEVEL_BOSS, TEXT("Layer_Effect"), vSpawnPosition)))
		return;

	// Spawn Adds
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Boaron"), LEVEL_BOSS, TEXT("Layer_Monster"), vSpawnPosition - _float3(.75f, 0.f, 0.f))))
		return;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Boaron"), LEVEL_BOSS, TEXT("Layer_Monster"), vSpawnPosition + _float3(.75f, 0.f, 0.f))))
		return;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Boaron"), LEVEL_BOSS, TEXT("Layer_Monster"), vSpawnPosition + _float3(0.f, 0.f, .75f))))
		return;
}

CHouse* CHouse::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHouse* pInstance = new CHouse(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CHouse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHouse::Clone(void* pArg)
{
	CHouse* pInstance = new CHouse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CHouse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CHouse::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}