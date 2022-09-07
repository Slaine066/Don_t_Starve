#include "stdafx.h"
#include "..\Public\Pig.h"
#include "GameInstance.h"
#include "Player.h"
#include "Inventory.h"
#include "Item.h"

CPig::CPig(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster(pGraphic_Device)
{
	ZeroMemory(&m_tInfo, sizeof(OBJINFO));
}

CPig::CPig(const CPig & rhs)
	: CMonster(rhs)
{
}

HRESULT CPig::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPig::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_Scale(2.f, 2.f, 1.f);

	m_tInfo.iMaxHp = 100;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;
	m_fAttackRadius = .8f;

	return S_OK;
}

int CPig::Tick(_float fTimeDelta)
{
	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;

	// A.I.
	AI_Behaviour(fTimeDelta);

	Update_Position(m_pTransformCom->Get_State(CTransform::STATE_POSITION));



	
	return OBJ_NOEVENT;
}

void CPig::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Change_Motion();
	Change_Frame();


	m_pColliderCom->Update_ColliderBox(m_pTransformCom->Get_WorldMatrix());
}

HRESULT CPig::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render_ColliderBox();
#endif // _DEBUG

	return S_OK;
}

HRESULT CPig::SetUp_Components(void* pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/*if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Pig"))))
		return E_FAIL;
	m_TimerTag = TEXT("Timer_Pig");*/

	Safe_Release(pGameInstance);

	/* For.Com_Texture */
	Texture_Clone();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Collider*/
	CCollider_Rect::COLLRECTDESC CollRectDesc;
	ZeroMemory(&CollRectDesc, sizeof(CCollider_Rect::COLLRECTDESC));
	CollRectDesc.fRadiusY = 0.35f;
	CollRectDesc.fRadiusX = 0.3f;
	CollRectDesc.fOffSetX = 0.f;
	CollRectDesc.fOffSetY = -0.25f;

	/* For.Com_Collider_Rect*/
	if (FAILED(__super::Add_Components(TEXT("Com_Collider_Rect"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_Rect"), (CComponent**)&m_pColliderCom, &CollRectDesc)))
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

	SetUp_DebugComponents(pArg);

	return S_OK;
}

HRESULT CPig::Texture_Clone()
{
	CTexture::TEXTUREDESC TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(CTexture::TEXTUREDESC));

	TextureDesc.m_iStartTex = 0;
	TextureDesc.m_fSpeed = 60;

	TextureDesc.m_iEndTex = 17;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_IDLE_UP"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Idle_Up"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 17;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_IDLE_DOWN"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Idle_Down"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 17;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_IDLE_SIDE"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Idle_Side"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 20;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_WALK_UP"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Walk_Up"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 20;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_WALK_DOWN"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Walk_Down"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 20;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_WALK_SIDE"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Walk_Side"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 16;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_RUN_UP"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Run_Up"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 16;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_RUN_DOWN"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Run_Down"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 16;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_RUN_SIDE"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Run_Side"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 30;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_ATTACK_UP"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Attack_Up"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 32;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_ATTACK_DOWN"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Attack_Down"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 36;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_ATTACK_SIDE"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Attack_Side"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 14;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_HIT"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Hit"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	TextureDesc.m_iEndTex = 31;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_DIE"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_Death"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	return S_OK;
}

void CPig::Change_Frame()
{
	switch (m_eState)
	{
	case STATE::IDLE:
		if (m_eDir == DIR_STATE::DIR_LEFT)
			m_pTransformCom->Set_Scale(-2.f, 2.f, 1.f);
		else
			m_pTransformCom->Set_Scale(2.f, 2.f, 1.f);

		m_pTextureCom->MoveFrame(m_TimerTag);
		break;
	case STATE::WALK:
		if (m_eDir == DIR_STATE::DIR_LEFT)
			m_pTransformCom->Set_Scale(-2.f, 2.f, 1.f);
		else
			m_pTransformCom->Set_Scale(2.f, 2.f, 1.f);

		m_pTextureCom->MoveFrame(m_TimerTag);
		break;
	case STATE::RUN:
		if (m_eDir == DIR_STATE::DIR_LEFT)
			m_pTransformCom->Set_Scale(-2.f, 2.f, 1.f);
		else
			m_pTransformCom->Set_Scale(2.f, 2.f, 1.f);

		m_pTextureCom->MoveFrame(m_TimerTag);
		break;
	case STATE::ATTACK:
		if (m_eDir == DIR_STATE::DIR_LEFT)
			m_pTransformCom->Set_Scale(-2.f, 2.f, 1.f);
		else
			m_pTransformCom->Set_Scale(2.f, 2.f, 1.f);

		if ((m_pTextureCom->MoveFrame(m_TimerTag, false)) == true)
		{
			m_bIsAttacking = false;
			m_dwAttackTime = GetTickCount();
		}
		else if (m_pTextureCom->Get_Frame().m_iCurrentTex == 14)
		{
			BULLETDATA BulletData;
			ZeroMemory(&BulletData, sizeof(BulletData));

			BulletData.vPosition = m_pColliderCom->Get_CollRectDesc().StateMatrix.m[3];
			BulletData.eDirState = m_eDir;
			BulletData.fOffsetSide = .5f;
			BulletData.fOffsetUp = .25f;
			BulletData.fOffsetDown = .5f;

			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Bullet"), LEVEL_GAMEPLAY, TEXT("Bullet"), &BulletData)))
				return;
		}
		break;
		break;
	case STATE::HIT:
		if (m_eDir == DIR_STATE::DIR_LEFT)
			m_pTransformCom->Set_Scale(-2.f, 2.f, 1.f);
		else
			m_pTransformCom->Set_Scale(2.f, 2.f, 1.f);

		if ((m_pTextureCom->MoveFrame(m_TimerTag, false) == true))
			m_bHit = false;
		break;
	case STATE::DIE:
		if (m_pTextureCom->Get_Frame().m_iCurrentTex == 17)
			Drop_Items();

		m_pTextureCom->MoveFrame(m_TimerTag, false);
		break;
	}
}

void CPig::Change_Motion()
{
	if (m_eState != m_ePreState || m_eDir != m_ePreDir)
	{
		switch (m_eState)
		{
		case STATE::IDLE:
			switch (m_eDir)
			{
			case DIR_STATE::DIR_UP:
				Change_Texture(TEXT("Com_Texture_IDLE_UP"));
				break;
			case DIR_STATE::DIR_DOWN:
				Change_Texture(TEXT("Com_Texture_IDLE_DOWN"));
				break;
			case DIR_STATE::DIR_RIGHT:
			case DIR_STATE::DIR_LEFT:
				Change_Texture(TEXT("Com_Texture_IDLE_SIDE"));
				break;
			}

			if (m_eDir != m_ePreDir)
				m_ePreDir = m_eDir;
			break;
		case STATE::WALK:
			switch (m_eDir)
			{
			case DIR_STATE::DIR_UP:
				Change_Texture(TEXT("Com_Texture_WALK_UP"));
				break;
			case DIR_STATE::DIR_DOWN:
				Change_Texture(TEXT("Com_Texture_WALK_DOWN"));
				break;
			case DIR_STATE::DIR_RIGHT:
			case DIR_STATE::DIR_LEFT:
				Change_Texture(TEXT("Com_Texture_WALK_SIDE"));
				break;
			}

			if (m_eDir != m_ePreDir)
				m_ePreDir = m_eDir;
			break;
		case STATE::RUN:
			switch (m_eDir)
			{
			case DIR_STATE::DIR_UP:
				Change_Texture(TEXT("Com_Texture_RUN_UP"));
				break;
			case DIR_STATE::DIR_DOWN:
				Change_Texture(TEXT("Com_Texture_RUN_DOWN"));
				break;
			case DIR_STATE::DIR_RIGHT:
			case DIR_STATE::DIR_LEFT:
				Change_Texture(TEXT("Com_Texture_RUN_SIDE"));
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

void CPig::AI_Behaviour(_float fTimeDelta)
{
	if (m_bHit)
		m_eState = STATE::HIT;

	// Check for Target, AggroRadius
	if (m_bAggro)
	{
		Find_Target();
		if (m_pTarget)
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
	else
		Patrol(fTimeDelta);
}

void CPig::Patrol(_float fTimeDelta)
{
	// Switch between Idle and Walk (based on time)
	if (m_eState == STATE::IDLE)
	{
		if (GetTickCount() > m_dwIdleTime + (rand() % 3000)*(rand()%2+1) + 3000)
		{
			m_eState = STATE::WALK;
			m_dwWalkTime = GetTickCount();

			// Find Random Patroling Position
			_float fOffsetX = ((_float)rand() / (float)(RAND_MAX)) * m_fPatrolRadius;
			_int bSignX = rand() % 2;
			_float fOffsetZ = ((_float)rand() / (float)(RAND_MAX)) * m_fPatrolRadius;
			_int bSignZ = rand() % 2;
			m_fPatrolPosX = bSignX ? (m_pTransformCom->Get_TransformDesc().InitPos.x + fOffsetX) : (m_pTransformCom->Get_TransformDesc().InitPos.x - fOffsetX);
			m_fPatrolPosZ = bSignZ ? (m_pTransformCom->Get_TransformDesc().InitPos.z + fOffsetZ) : (m_pTransformCom->Get_TransformDesc().InitPos.z - fOffsetZ);
		}
	}
	else if (m_eState == STATE::WALK)
	{
		if (GetTickCount() > m_dwWalkTime + (rand() % 3000)*(rand() % 2 + 1) + 1500)
		{
			m_eState = STATE::IDLE;
			m_dwIdleTime = GetTickCount();
		}
	}

	// Movement
	if (m_eState == STATE::WALK)
	{
		// Adjust PatrolPosition Y
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		if (!pGameInstance)
			return;
		CVIBuffer_Terrain* pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"), 0);
		if (!pVIBuffer_Terrain)
			return;
		CTransform*	pTransform_Terrain = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), TEXT("Com_Transform"), 0);
		if (!pTransform_Terrain)
			return;

		_float3 vPatrolPosition = { m_fPatrolPosX, Get_Position().y, m_fPatrolPosZ };
		_float3 vScale = m_pTransformCom->Get_Scale();

		vPatrolPosition.y = pVIBuffer_Terrain->Compute_Height(vPatrolPosition, pTransform_Terrain->Get_WorldMatrix(), (1 * vScale.y / 2));

		// Change Direction
		_float fX = vPatrolPosition.x - Get_Position().x;
		_float fZ = vPatrolPosition.z - Get_Position().z;

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

		m_pTransformCom->Go_PosTarget(fTimeDelta * .1f, _float3(m_fPatrolPosX, Get_Position().y, m_fPatrolPosZ), _float3{ 0.f, 0.f, 0.f });
	}
}

void CPig::Find_Target()
{
	if (!m_bIsAttacking && !m_bHit && !m_bDead)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));

		Safe_Release(pGameInstance);

		if (pTarget)
		{
			_float3 vTargetPos = pTarget->Get_Position();
			m_fDistanceToTarget = sqrt(pow(Get_Position().x - vTargetPos.x, 2) + pow(Get_Position().y - vTargetPos.y, 2) + pow(Get_Position().z - vTargetPos.z, 2));
			m_pTarget = pTarget;
		}
		else
			m_pTarget = nullptr;
	}
	else
		m_pTarget = nullptr;
}

void CPig::Follow_Target(_float fTimeDelta)
{
	// Set State 
	m_eState = STATE::RUN;

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

	m_pTransformCom->Go_PosTarget(fTimeDelta * .2f, fTargetPos, _float3(0, 0, 0));

	m_bIsAttacking = false;
}

_float CPig::Take_Damage(float fDamage, void * DamageType, CGameObject * DamageCauser)
{
	_float fDmg = __super::Take_Damage(fDamage, DamageType, DamageCauser);

	if (fDmg > 0)
	{
		if (!m_bDead)
			m_bHit = true;
		
		m_bAggro = true;
		m_bIsAttacking = false;
		m_dwAttackTime = GetTickCount();

		return fDmg;
	}

	return 0.f;
}

HRESULT CPig::Drop_Items()
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
	ItemDesc.pTextureComponent = TEXT("Com_Texture_Pig_Tail");
	ItemDesc.pTexturePrototype = TEXT("Prototype_Component_Texture_Equipment_front");
	ItemDesc.eItemName = ITEMNAME::ITEMNAME_PIGTAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_GAMEPLAY, TEXT("Layer_Object"), &ItemDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

_bool CPig::IsDead()
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

CPig* CPig::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPig* pInstance = new CPig(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPig"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPig::Clone(void* pArg)
{
	CPig* pInstance = new CPig(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPig"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CPig::Free()
{
	__super::Free();

	//Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

	for (auto& iter : m_vecTexture)
		Safe_Release(iter);

	m_vecTexture.clear();
}
