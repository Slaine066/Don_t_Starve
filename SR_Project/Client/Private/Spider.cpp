#include "stdafx.h"
#include "..\Public\Spider.h"
#include "GameInstance.h"
#include "Player.h"
#include "Inventory.h"
#include "Item.h"

CSpider::CSpider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster(pGraphic_Device)
{
	ZeroMemory(&m_tInfo, sizeof(OBJINFO));
}

CSpider::CSpider(const CSpider & rhs)
	: CMonster(rhs)
{
}

HRESULT CSpider::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpider::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_Scale(1.2f, 1.f, 1.f);

	m_tInfo.iMaxHp = 100;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;
	
	return S_OK;
}

int CSpider::Tick(_float fTimeDelta)
{
	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;

	// A.I.
	AI_Behaviour(fTimeDelta);

	Update_Position(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	return OBJ_NOEVENT;
}

void CSpider::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Change_Motion();
	Change_Frame();
}

HRESULT CSpider::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpider::SetUp_Components(void* pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Spider"))))
		return E_FAIL;
	m_TimerTag = TEXT("Timer_Spider");

	Safe_Release(pGameInstance);

	/* For.Com_Texture */
	Texture_Clone();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Collider*/
	if (FAILED(__super::Add_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.InitPos = *(_float3*)pArg;

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpider::Texture_Clone()
{
	CTexture::TEXTUREDESC TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(CTexture::TEXTUREDESC));

	TextureDesc.m_iStartTex = 0;
	TextureDesc.m_fSpeed = 60;

	TextureDesc.m_iEndTex = 0;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_IDLE"), LEVEL_HUNT, TEXT("Prototype_Component_Texture_Spider_Idle"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 17;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_MOVE_UP"), LEVEL_HUNT, TEXT("Prototype_Component_Texture_Spider_Move_Up"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 17;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_MOVE_DOWN"), LEVEL_HUNT, TEXT("Prototype_Component_Texture_Spider_Move_Down"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 16;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_MOVE_SIDE"), LEVEL_HUNT, TEXT("Prototype_Component_Texture_Spider_Move_Side"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 31;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_ATTACK_UP"), LEVEL_HUNT, TEXT("Prototype_Component_Texture_Spider_Attack_Up"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 33;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_ATTACK_DOWN"), LEVEL_HUNT, TEXT("Prototype_Component_Texture_Spider_Attack_Down"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 43;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_ATTACK_SIDE"), LEVEL_HUNT, TEXT("Prototype_Component_Texture_Spider_Attack_Side"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 31;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_TAUNT"), LEVEL_HUNT, TEXT("Prototype_Component_Texture_Spider_Taunt"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 16;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_HIT"), LEVEL_HUNT, TEXT("Prototype_Component_Texture_Spider_Hit"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 27;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_DIE"), LEVEL_HUNT, TEXT("Prototype_Component_Texture_Spider_Die"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	return S_OK;
}

void CSpider::Change_Frame()
{
	switch (m_eState)
	{
	case STATE::IDLE:
		m_pTextureCom->MoveFrame(m_TimerTag);
		break;
	case STATE::MOVE:
		if (m_eDir == DIR_STATE::DIR_LEFT)
			m_pTransformCom->Set_Scale(-1.2f, 1.f, 1.f);
		else
			m_pTransformCom->Set_Scale(1.2f, 1.f, 1.f);

		m_pTextureCom->MoveFrame(m_TimerTag);
		break;
	case STATE::ATTACK:
		if (m_eDir == DIR_STATE::DIR_LEFT)
			m_pTransformCom->Set_Scale(-1.2f, 1.f, 1.f);
		else
			m_pTransformCom->Set_Scale(1.2f, 1.f, 1.f);

		if ((m_pTextureCom->MoveFrame(m_TimerTag, false)) == true)
		{
			m_bIsAttacking = false;
			m_dwAttackTime = GetTickCount();
		}
		else if (m_pTextureCom->Get_Frame().m_iCurrentTex == 23)
		{
			BULLETDATA BulletData;
			ZeroMemory(&BulletData, sizeof(BulletData));
			BulletData.vPosition = Get_Position();
			BulletData.vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			BulletData.eDirState = m_eDir;

			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Bullet"), LEVEL_GAMEPLAY, TEXT("Bullet"), &BulletData)))
				return;
		}
		break;
	case STATE::TAUNT:
		if (m_eDir == DIR_STATE::DIR_LEFT)
			m_pTransformCom->Set_Scale(-1.2f, 1.f, 1.f);
		else
			m_pTransformCom->Set_Scale(1.2f, 1.f, 1.f);

		if ((m_pTextureCom->MoveFrame(m_TimerTag, false)) == true)
			m_eState = STATE::IDLE;
		break;
	case STATE::HIT:
		if (m_eDir == DIR_STATE::DIR_LEFT)
			m_pTransformCom->Set_Scale(-1.2f, 1.f, 1.f);
		else
			m_pTransformCom->Set_Scale(1.2f, 1.f, 1.f);

		if ((m_pTextureCom->MoveFrame(m_TimerTag, false) == true))
			m_bHit = false;
		break;
	case STATE::DIE:
		if (m_pTextureCom->Get_Frame().m_iCurrentTex == 9)
			Drop_Items();

		m_pTextureCom->MoveFrame(m_TimerTag, false);
		break;
	}
}

void CSpider::Change_Motion()
{
	if (m_eState != m_ePreState || m_eDir != m_ePreDir)
	{
		switch (m_eState)
		{
		case STATE::IDLE:
			Change_Texture(TEXT("Com_Texture_IDLE"));
			break;
		case STATE::MOVE:
			switch (m_eDir)
			{
			case DIR_STATE::DIR_UP:
				Change_Texture(TEXT("Com_Texture_MOVE_UP"));
				break;
			case DIR_STATE::DIR_DOWN:
				Change_Texture(TEXT("Com_Texture_MOVE_DOWN"));
				break;
			case DIR_STATE::DIR_RIGHT:
			case DIR_STATE::DIR_LEFT:
				Change_Texture(TEXT("Com_Texture_MOVE_SIDE"));
				break;
			}

			if (m_eDir != m_ePreDir)
				m_ePreDir = m_eDir;
			break;
		case STATE::ATTACK:
			switch (m_eDir)
			{
			case DIR_STATE::DIR_UP:
				Change_Texture(TEXT("Com_Texture_ATTACK_UP"));
				break;
			case DIR_STATE::DIR_DOWN:
				Change_Texture(TEXT("Com_Texture_ATTACK_DOWN"));
				break;
			case DIR_STATE::DIR_RIGHT:
			case DIR_STATE::DIR_LEFT:
				Change_Texture(TEXT("Com_Texture_ATTACK_SIDE"));
				break;
			}

			if (m_eDir != m_ePreDir)
				m_ePreDir = m_eDir;
			break;
		case STATE::TAUNT:
			Change_Texture(TEXT("Com_Texture_TAUNT"));
			break;
		case STATE::HIT:
			Change_Texture(TEXT("Com_Texture_HIT"));
			break;
		case STATE::DIE:
			Change_Texture(TEXT("Com_Texture_DIE"));
			break;
		}

		if (m_eState != m_ePreState)
			m_ePreState = m_eState;
	}
}

void CSpider::AI_Behaviour(_float fTimeDelta)
{
	if (m_bHit)
		m_eState = STATE::HIT;

	// Get Target and Aggro Radius
	Find_Target();

	// Check for Target, AggroRadius
	if (m_pTarget && m_bAggro)
	{
		// If in AttackRadius > Attack
		if (m_fDistanceToTarget < m_fAttackRadius)
		{
			if (!m_bIsAttacking && GetTickCount() > m_dwAttackTime + 1500)
			{
				m_eState = STATE::ATTACK;
				m_bIsAttacking = true;
			}
			else if (!m_bIsAttacking)
				m_eState = STATE::IDLE;
		}
		// If NOT in AttackRadius > Follow Target
		else
			Follow_Target(fTimeDelta);
	}
}

void CSpider::Find_Target()
{
	if (!m_bIsAttacking && !m_bHit && !m_bDead)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_Player"));

		Safe_Release(pGameInstance);

		if (pTarget)
		{
			_float3 vTargetPos = pTarget->Get_Position();
			m_fDistanceToTarget = sqrt(pow(Get_Position().x - vTargetPos.x, 2) + pow(Get_Position().y - vTargetPos.y, 2) + pow(Get_Position().z - vTargetPos.z, 2));

			if (m_fDistanceToTarget < m_fAggroRadius || m_bAggro)
				m_bAggro = true;

			m_pTarget = pTarget;
		}
		else
			m_pTarget = nullptr;
	}
	else
		m_pTarget = nullptr;
}

void CSpider::Follow_Target(_float fTimeDelta)
{
	// Set State 
	m_eState = STATE::MOVE;

	_float3 fTargetPos = m_pTarget->Get_Position();

	// Set Direction
	_float fX = fTargetPos.x - Get_Position().x;
	_float fZ = fTargetPos.z - Get_Position().z;

	// Move Horizontally
	if (abs(fX) > abs(fZ))
		if (fX > 0)
			m_eDir = DIR_STATE::DIR_RIGHT;
		else
			m_eDir = DIR_STATE::DIR_LEFT;
	// Move Vertically
	else
		if (fZ > 0)
			m_eDir = DIR_STATE::DIR_UP;
		else
			m_eDir = DIR_STATE::DIR_DOWN;

	m_pTransformCom->Go_PosTarget(fTimeDelta * .1f, fTargetPos, _float3(0, 0, 0));

	m_bIsAttacking = false;
}

_float CSpider::Take_Damage(float fDamage, void * DamageType, CGameObject * DamageCauser)
{
	_float fDmg = __super::Take_Damage(fDamage, DamageType, DamageCauser);
	
	if (fDmg > 0)
	{
		if (!m_bDead)
			m_bHit = true;

		m_bIsAttacking = false;
		m_dwAttackTime = GetTickCount();
	}

	return fDmg;
}

HRESULT CSpider::Drop_Items()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CItem::ITEMDESC ItemDesc;
	ZeroMemory(&ItemDesc, sizeof(CItem::ITEMDESC));

	// Random Position Drop based on Object Position
	_float fOffsetX = ((_float)rand() / (float)(RAND_MAX)) * .5f;
	_int bSignX = rand() % 2;
	_float fOffsetZ = ((_float)rand() / (float)(RAND_MAX)) * .5f;
	_int bSignZ = rand() % 2;
	_float fPosX = bSignX ? (Get_Position().x + fOffsetX) : (Get_Position().x - fOffsetX);
	_float fPosZ = bSignZ ? (Get_Position().z + fOffsetZ) : (Get_Position().z - fOffsetZ);

	ItemDesc.fPosition = _float3(fPosX, Get_Position().y, fPosZ);
	ItemDesc.pTextureComponent = TEXT("Com_Texture_Spider_Meat");
	ItemDesc.pTexturePrototype = TEXT("Prototype_Component_Texture_Equipment_front");
	ItemDesc.eItemName = ITEMNAME::ITEMNAME_SPIDERMEAT;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_GAMEPLAY, TEXT("Layer_Object"), &ItemDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

_bool CSpider::IsDead()
{
	if (m_bDead && m_eState == STATE::DIE && GetTickCount() > m_dwDeathTime + 1500)
		return true;
	else if (m_bDead && m_eState != STATE::DIE)
	{
		m_dwDeathTime = GetTickCount();
		m_eState = STATE::DIE;
	}
	
	return false;
}

CSpider* CSpider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpider* pInstance = new CSpider(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CSpider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpider::Clone(void* pArg)
{
	CSpider* pInstance = new CSpider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CSpider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpider::Clone_Load(const _tchar * VIBufferTag, void * pArg)
{
	return nullptr;
}

void CSpider::Free()
{
	__super::Free();
}