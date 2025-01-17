#include "stdafx.h"
#include "..\Public\Wendy.h"
#include "GameInstance.h"
#include "Player.h"
#include "BerryBush.h"
#include "Pig.h"
#include "Bullet.h"
#include "Skill.h"
#include "Inventory.h"
#include "CameraManager.h"
CWendy::CWendy(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CNPC(pGraphic_Device)
{
}

CWendy::CWendy(const CWendy & rhs)
	: CNPC(rhs)
{
}

HRESULT CWendy::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWendy::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	//Test
	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
	m_eObjID = OBJID::OBJ_NPC;
	m_eNPCID = NPCID::NPC_WENDY;

	m_fAtk_Max_CoolTime = 3.f;
	m_fAtk_Cur_CoolTime = m_fAtk_Max_CoolTime;

	m_fSkill_Max_CoolTime = 5.f;
	m_fSkill_Cur_CoolTime = m_fSkill_Max_CoolTime;

	m_fSkillRange = 10.f;
	m_fAtkRange = 10.f;
	m_fOwnerRadius = 10.f;
	m_fMinRange = 5.f;
	m_fDetectRange = 15.f;


	m_tInfo.iMaxHp = 900;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;
	m_tInfo.fDamage = 20.f;

	//Init BehavirvTree
	BehaviorTree = new CBT_NPC(this);

	BehaviorTree->Initialize();

	Change_Texture(TEXT("Com_Texture_Idle_Down"));
	return S_OK;
}

int CWendy::Tick(_float fTimeDelta)
{
	m_iCurrentLevelndex = (LEVEL)CLevel_Manager::Get_Instance()->Get_CurrentLevelIndex();


	
	if ((LEVEL)m_iCurrentLevelndex == LEVEL_GAMEPLAY && m_bDead && m_fReviveTime > 10.f)
	{
		m_bCanTalk = true;
		m_bDead = false;
		Clear_Activated();
		Reset_Target();
	}

	if (m_iCurrentLevelndex != m_iPreLevelIndex)
	{
		if (m_bOwner)
		{
			m_bCanTalk = true;
			_float3 Owner_Pos = static_cast<CPlayer*>(m_pOwner)->Get_Pos();
			Owner_Pos.x -= 3.f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, Owner_Pos);

			MINIMAP		minidesc;
			ZeroMemory(&minidesc, sizeof(MINIMAP));
			minidesc.name = MIN_WENDY;
			minidesc.pointer = this;
			LEVEL CurrentLevelndex = (LEVEL)CLevel_Manager::Get_Instance()->Get_CurrentLevelIndex();
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MiniMap_Icon"), CurrentLevelndex, TEXT("MiniMap_Icon"), &minidesc);
		
		}
		else
		{
			m_bCanTalk = true;
			Clear_Activated();
			Reset_Target();

			if (m_iCurrentLevelndex == LEVEL_GAMEPLAY)
			{
				MINIMAP		minidesc;
				ZeroMemory(&minidesc, sizeof(MINIMAP));
				minidesc.name = MIN_WENDY;
				minidesc.pointer = this;
				LEVEL CurrentLevelndex = (LEVEL)CLevel_Manager::Get_Instance()->Get_CurrentLevelIndex();
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MiniMap_Icon"), CurrentLevelndex, TEXT("MiniMap_Icon"), &minidesc);
			}
		}
		

		m_iPreLevelIndex = m_iCurrentLevelndex;
	}

	if (m_iCurrentLevelndex == LEVEL_LOADING)
		return OBJ_NOEVENT;

	if (m_iCurrentLevelndex != LEVEL_GAMEPLAY && !m_bOwner)
	{
		m_bCanTalk = false;
		return OBJ_NOEVENT;
	}

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Add_CollisionGroup(CCollider_Manager::COLLISION_PLAYER, this);

	__super::Tick(fTimeDelta);


	BehaviorTree->Tick(fTimeDelta);

	Update_Position(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	return OBJ_NOEVENT;
}

void CWendy::Late_Tick(_float fTimeDelta)
{
	if (m_iCurrentLevelndex == LEVEL_LOADING)
		return;

	if (m_iCurrentLevelndex != LEVEL_GAMEPLAY && !m_bOwner)
		return;




	__super::Late_Tick(fTimeDelta);

	Change_Frame(fTimeDelta);

	if (m_bDead)
	{
		return;
	}

	if (m_eCur_Dir == DIR_STATE::DIR_LEFT)
	{
		m_pTransformCom->Set_Scale(-1.f, 1.f, 1.f);
	}
}

HRESULT CWendy::Render()
{
	if (m_iCurrentLevelndex == LEVEL_LOADING)
		return S_OK;

	if (FAILED(__super::Render()))
		return E_FAIL;


#ifdef _DEBUG

	//m_pColliderCom->Render_ColliderBox();
#endif // _DEBUG

	return S_OK;
}

HRESULT CWendy::SetUp_Components(void * pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//m_TimerTag = TEXT("Timer_Grass");
	//if (FAILED(pGameInstance->Add_Timer(m_TimerTag)))
	//return E_FAIL;



	/* For.Com_Texture */
	Texture_Clone();
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_Static"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	CCollider_Cube::COLLRECTDESC CollRectDesc;
	ZeroMemory(&CollRectDesc, sizeof(CCollider_Cube::COLLRECTDESC));
	CollRectDesc.fRadiusY = 0.4f;
	CollRectDesc.fRadiusX = 0.3f;
	CollRectDesc.fRadiusZ = 0.3f;
	CollRectDesc.fOffSetX = 0.f;
	CollRectDesc.fOffSetY = -0.25f;
	CollRectDesc.fOffsetZ = 0.f;

	/* For.Com_Collider_Rect*/
	/*if (FAILED(__super::Add_Components(TEXT("Com_Collider_Rect"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_Rect"), (CComponent**)&m_pColliderCom, &CollRectDesc)))
	return E_FAIL;*/
	if (FAILED(__super::Add_Components(TEXT("Com_Collider_Cube"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"), (CComponent**)&m_pColliderCom, &CollRectDesc)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.f / 1.13f;
	TransformDesc.fRotationPerSec = D3DXToRadian(0.f);
	TransformDesc.InitPos = _float3(40.f, 2.f, 25.f);;



	Safe_Release(pGameInstance);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CWendy::Texture_Clone()
{
	CTexture::TEXTUREDESC TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(CTexture::TEXTUREDESC));
	/*Idle*/
	TextureDesc.m_iStartTex = 0;
	TextureDesc.m_iEndTex = 1;
	TextureDesc.m_fSpeed = 20;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Idle_Down"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Idle_Down"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Idle_Up"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Idle_Up"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Idle_Side"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Idle_Side"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);
	/*Run*/
	TextureDesc.m_iStartTex = 0;
	TextureDesc.m_iEndTex = 27;
	TextureDesc.m_fSpeed = 20;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Run_Down"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Run_Down"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Run_Up"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Run_Up"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Run_Side"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Run_Side"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);
	/*Build*/
	TextureDesc.m_iEndTex = 16;
	TextureDesc.m_fSpeed = 20;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Build_Down"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Build_Down"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Build_Up"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Build_Up"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Build_Side"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Build_Side"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	/*Pickup*/
	TextureDesc.m_iStartTex = 0;
	TextureDesc.m_iEndTex = 5;
	TextureDesc.m_fSpeed = 20;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Pickup_Down"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Pickup_Down"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Pickup_Up"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Pickup_Up"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Pickup_Side"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Pickup_Side"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	/*Give*/
	TextureDesc.m_iStartTex = 0;
	TextureDesc.m_iEndTex = 13;
	TextureDesc.m_fSpeed = 20;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Give_Down"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Give_Down"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Give_Up"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Give_Up"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Give_Side"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Give_Side"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	/*Dance*/
	TextureDesc.m_iEndTex = 31;
	TextureDesc.m_fSpeed = 20;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Dance"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Dance"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	/*Talk*/
	TextureDesc.m_iEndTex = 51;
	TextureDesc.m_fSpeed = 20;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Talk"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Talk"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	/*Recall*/
	TextureDesc.m_iEndTex = 24;
	TextureDesc.m_fSpeed = 20;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Recall"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Recall"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	/*Channel*/
	TextureDesc.m_iEndTex = 52;
	TextureDesc.m_fSpeed = 20;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Channel"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Channel"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	/*Dead*/
	TextureDesc.m_iEndTex = 55;
	TextureDesc.m_fSpeed = 30;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Dead"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Dead"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	/*Hit*/
	TextureDesc.m_iEndTex = 22;
	TextureDesc.m_fSpeed = 30;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Hit"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Wendy_Hit"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;
	m_vecTexture.push_back(m_pTextureCom);

	return S_OK;
}

void CWendy::Change_Frame(_float fTimeDelta)
{
	if (m_eState == NPC_STATE::DEAD)
	{
		m_pTextureCom->MoveFrame(m_TimerTag, false);
	}
	else
	{
		m_pTextureCom->MoveFrame(m_TimerTag);
	}
}

void CWendy::Change_Motion()
{
}

void CWendy::Interact(_uint Damage)
{
	if (m_iTalkCnt == 2 && Damage == 1)
	{
		m_bAccept = true;
		m_iTalkCnt++;
	}
	else if (m_iTalkCnt == 2 && Damage == 2)
	{
		m_bAccept = false;
		m_iTalkCnt++;
	}
	else if (m_iTalkCnt != 2)
	{
		m_iTalkCnt++;
	}

}

HRESULT CWendy::Drop_Items()
{
	return S_OK;
}

void CWendy::Make_Interrupt(CPawn * pCauser, _uint _InterruptNum)
{
	switch (_InterruptNum)
	{
	case 0: // Talk
		Reset_Target();
		m_pTarget = pCauser;
		Safe_AddRef(m_pTarget);
		m_iInterruptNum = _InterruptNum;
		m_bInterrupted = true;
		m_bInteract = true;
		break;
	case 1://FightMode
		if (!m_bFightMode && m_bOwner)
		{
			m_iInterruptNum = _InterruptNum;
			m_bInterrupted = true;
			m_bInteract = true;
		}
		break;
	}


}

_float CWendy::Take_Damage(float fDamage, void * DamageType, CGameObject * DamageCauser)
{
	if (m_bInvincibleMode || m_bFirstCall)
		return 0.f;

	if (!m_bDead && m_tInfo.iCurrentHp <= (_int)fDamage)
	{
		m_fReviveTime = 0.f;
		m_bDead = true;
		m_bCanTalk = false;
	}
	else if (!m_bDead && !m_bHited)
	{
		m_tInfo.iCurrentHp -= (_int)fDamage;
		cout << "WendyHP: " << m_tInfo.iCurrentHp << endl;
		m_bHited = true;
	}
	return fDamage;
}

_bool CWendy::Get_CanSkill(void)
{
	Select_HealTarget(0.f);
	if (m_pHeal_Target && m_bCanSkill)
	{
		return true;
	}
	else if (m_pHeal_Target == nullptr || !m_bCanSkill)
	{
		return false;
	}

	//return m_bCanSkill;

}

_bool CWendy::Get_CanAttack(void)
{
	Select_ReviveTarget();
	if (m_pHeal_Target && m_bCanAttack)
	{
		return true;
	}
	else if (m_pHeal_Target == nullptr || !m_bCanAttack)
	{
		return false;
	}
}

void CWendy::Move(_float _fTimeDelta)
{
	_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_eCur_Dir = Check_Direction();

	m_eState = CNPC::MOVE;

	if (m_ePreState != m_eState
		|| m_ePre_Dir != m_eCur_Dir)
	{
		m_eCur_Dir = Check_Direction();
		switch (m_eCur_Dir)
		{
		case DIR_UP:
			Change_Texture(TEXT("Com_Texture_Run_Up"));
			break;

		case DIR_DOWN:
			Change_Texture(TEXT("Com_Texture_Run_Down"));
			break;

		case DIR_LEFT:
			Change_Texture(TEXT("Com_Texture_Run_Side"));
			break;

		case DIR_RIGHT:
			Change_Texture(TEXT("Com_Texture_Run_Side"));
			break;
		}
		m_ePreState = m_eState;
		m_ePre_Dir = m_eCur_Dir;
	}
	m_pTransformCom->Go_PosTarget(_fTimeDelta, m_vTargetPos, _float3{ 0.f, 0.f, 0.f });

	Play_Sound(_fTimeDelta);

	SetUp_BillBoard();
}

void CWendy::Idle(_float _fTimeDelta)
{
	m_eState = CNPC::IDLE;

	if (m_ePreState != m_eState)
	{
		switch (m_eCur_Dir)
		{
		case DIR_UP:
			Change_Texture(TEXT("Com_Texture_Idle_Up"));
			break;

		case DIR_DOWN:
			Change_Texture(TEXT("Com_Texture_Idle_Down"));
			break;

		case DIR_LEFT:
		case DIR_RIGHT:
			Change_Texture(TEXT("Com_Texture_Idle_Side"));
			break;

		}
		m_ePreState = m_eState;
	}
}

void CWendy::Interaction(_float _fTimedelta)
{
	//Test Only Berry
	if (m_pTarget == nullptr)
		return;
	//나중에 분간.

	Revive_Berry(_fTimedelta);

}
void CWendy::Talk(_float _fTimeDelta)
{
	if (static_cast<CPawn*>(m_pTarget)->Get_ObjID() == OBJID::OBJ_PLAYER)
	{
		Talk_Player(_fTimeDelta);
	}
	else
	{//Monster
		Talk_Friend(_fTimeDelta);
	}
}

void CWendy::Dance(_float _fTimeDelta)
{
	m_fInteractTIme += _fTimeDelta;

	m_eState = CNPC::DANCE;

	if (m_ePreState != m_eState)
	{
		m_bInteract = true;
		cout << "Dance" << endl;
		Change_Texture(TEXT("Com_Texture_Dance"));
		m_ePreState = m_eState;
	}
	if (2.f < m_fInteractTIme)
	{
		cout << "DanceEnd" << endl;
		m_fInteractTIme = 0.f;
		m_bInteract = false;
	}
}

void CWendy::Attack(_float _fTimeDelta)
{
	//m_fInteractTIme += _fTimeDelta;

	m_eState = CNPC::ATTACK;

	if (m_ePreState != m_eState)
	{
		m_bInteract = true;
		Change_Texture(TEXT("Com_Texture_Recall"));
		m_ePreState = m_eState;
		Play_Sound(_fTimeDelta);
	}
	if (m_pTextureCom->Get_Frame().m_iCurrentTex >= m_pTextureCom->Get_Frame().m_iEndTex - 2)
	{
		Create_Bullet(_fTimeDelta);
		m_bInteract = false;
		m_bCanAttack = false;
	}


}

void CWendy::Interrupted(_float _fTimeDelta)
{//for Test. Only TalkInterrupt
	if (m_bInterrupted)
	{

		switch (m_iInterruptNum)
		{
		case 0://TalkMode
			Clear_Activated();
			m_bFirstCall = true;
			m_bArrive = true;
			m_bInteract = true;
			m_bInterrupted = false;
			m_bSelectAct = false;
			m_bFinishInteract = false;
			break;
		case 1: // attackMode
			Clear_Activated();
			m_bFightMode = true;
			m_bInteract = true;
			m_bInterrupted = false;
			m_bSelectAct = false;
			m_bFinishInteract = false;
			break;
		}

	}

}

void CWendy::Skill(_float _fTimeDelta)
{
	m_eState = CNPC::SKILL;

	if (m_ePreState != m_eState)
	{
		m_bInteract = true;
		Change_Texture(TEXT("Com_Texture_Channel"));
		m_ePreState = m_eState;
		Play_Sound(_fTimeDelta);
	}
	if (m_bCanSkill &&m_pTextureCom->Get_Frame().m_iCurrentTex >= m_pTextureCom->Get_Frame().m_iEndTex - 2)
	{
		Create_Heal(_fTimeDelta);
		m_bInteract = false;
		m_bCanSkill = false;
	}
}

_bool CWendy::Hit(_float _fTimeDelta)
{
	m_fInteractTIme += _fTimeDelta;

	m_eState = CNPC::HIT;

	if (m_ePreState != m_eState)
	{
		m_bInteract = true;
		Change_Texture(TEXT("Com_Texture_Hit"));
		m_ePreState = m_eState;
		Play_Sound(_fTimeDelta);
	}

	if (m_pTextureCom->Get_Frame().m_iCurrentTex >= m_pTextureCom->Get_Frame().m_iEndTex - 1)
	{
		Clear_Activated();
		m_bHited = false;
	}
	return true;
}

_bool CWendy::Dead(_float _fTimeDelta)
{
	m_eState = CNPC::DEAD;

	if (m_ePreState != m_eState)
	{
		m_bInteract = true;
		Change_Texture(TEXT("Com_Texture_Dead"));
		m_ePreState = m_eState;
		Play_Sound(_fTimeDelta);
	}
	return true;
}

void CWendy::Select_Target(_float _fTimeDelta)
{
	if (m_iCurrentLevelndex == LEVEL_LOADING)
		return;

	Find_Priority();

	m_bInteract = true;
	m_bArrive = false;
}

void CWendy::Set_RandPos(_float _fTimeDelta)
{// Find Random Patroling Position
	_float fOffsetX = ((_float)rand() / (float)(RAND_MAX)) * m_fPatrolRadius;
	_int bSignX = rand() % 2;
	_float fOffsetZ = ((_float)rand() / (float)(RAND_MAX)) * m_fPatrolRadius;
	_int bSignZ = rand() % 2;
	m_fPatrolPosX = bSignX ? (m_pTransformCom->Get_TransformDesc().InitPos.x + fOffsetX) : (m_pTransformCom->Get_TransformDesc().InitPos.x - fOffsetX);
	m_fPatrolPosZ = bSignZ ? (m_pTransformCom->Get_TransformDesc().InitPos.z + fOffsetZ) : (m_pTransformCom->Get_TransformDesc().InitPos.z - fOffsetZ);

	m_vTargetPos = _float3(m_fPatrolPosX, 0.5f, m_fPatrolPosZ);
	m_bArrive = false;
	m_bSelectAct = false;
}

_bool CWendy::Get_Target_Moved(_float _fTimeDelta, _uint _iTarget)
{
	_float fMinRange = 0.f;
	_float fMiddleRange = 0.f;
	_float Compare_Range = 0.f;
	switch (_iTarget)
	{
	case 0: //Target == Owner
		fMinRange = m_fMinRange;
		fMiddleRange = m_fOwnerRadius;
		m_vTargetPos = static_cast<CPlayer*>(m_pOwner)->Set_PartyPostion(this);
		break;
	case 1:// Basic AttackRange
		fMiddleRange = fMinRange = m_fAtkRange;
		m_vTargetPos = m_pHeal_Target->Get_Position();
		break;
	case 2://SkillRange
		fMiddleRange = fMinRange = m_fSkillRange;

		m_vTargetPos = m_pHeal_Target->Get_Position();
		break;
	default:
		if (m_pTarget)
		{
			m_vTargetPos = m_pTarget->Get_Position();
		}
		fMiddleRange = fMinRange = 0.4f;
		break;
	}


	Compare_Range = (m_vTargetPos.x - Get_Pos().x)*(m_vTargetPos.x - Get_Pos().x)
		+ (m_vTargetPos.z - Get_Pos().z)*(m_vTargetPos.z - Get_Pos().z);

	if (fMinRange > Compare_Range)
	{
		m_bArrive = true;
		Clear_Activated();
		return false;
	}

	if (m_bFightMode)
	{
		if (fMiddleRange < Compare_Range)
		{
			m_bArrive = false;
			Clear_Activated();
		}

	}
	else
	{
		if (fMinRange < Compare_Range)
		{
			m_bArrive = false;
			Clear_Activated();
		}
	}
	return true;
}

_bool CWendy::Detect_Enemy()
{
	Find_Enemy();

	if (m_pTarget == nullptr)
		return false;

	m_bInteract = true;
	return true;
}

void CWendy::Play_Sound(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Is_In_Frustum(Get_Position(), m_fRadius) == false)
		return;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_int iNum = rand() % 4;
	_tchar	szFullPath[MAX_PATH];
	_float fVolume = 0.5f;
	switch (m_eState)
	{
	case CNPC::IDLE:
		break;
	case CNPC::MOVE:
		fVolume = 0.3f;
		if (m_pTextureCom->Get_Frame().m_iCurrentTex == 14
			|| m_pTextureCom->Get_Frame().m_iCurrentTex == 2)
		{
			iNum = rand() % 4;
			if ((LEVEL)m_iCurrentLevelndex == LEVEL_GAMEPLAY)
			{
				wcscpy_s(szFullPath, TEXT("footstep_grass_%d.wav"));
			}
			else
			{
				wcscpy_s(szFullPath, TEXT("DSS_woodlegs_footstep_wood_%d.wav"));
			}
			wsprintf(szFullPath, szFullPath, iNum);
		}
		break;
	case CNPC::HIT:
		////Player Hit
		//wcscpy_s(szFullPath, TEXT("Wendy_hurt_%d.wav"));
		//wsprintf(szFullPath, szFullPath, iNum);
		break;
	case CNPC::DEAD:
		wcscpy_s(szFullPath, TEXT("Wendy_death.wav"));
		break;
	case CNPC::ATTACK:
	case CNPC::SKILL:
		break;
	case CNPC::TALK:
		/*fVolume = 0.4f;
		wcscpy_s(szFullPath, TEXT("Wendy_generic_%d.wav"));
		wsprintf(szFullPath, szFullPath, iNum);*/
		break;
	}

	pGameInstance->PlaySounds(szFullPath, SOUND_WENDY, fVolume);

	Safe_Release(pGameInstance);
}

void CWendy::Revive_Berry(_float _fTimeDelta)
{
	m_eState = CNPC::INTERACT;

	if (m_ePreState != m_eState)
	{
		cout << "Interact" << endl;
		switch (m_eCur_Dir)
		{
		case DIR_UP:
			Change_Texture(TEXT("Com_Texture_Build_Up"));
			break;

		case DIR_DOWN:
			Change_Texture(TEXT("Com_Texture_Build_Down"));
			break;

		case DIR_LEFT:
		case DIR_RIGHT:
			Change_Texture(TEXT("Com_Texture_Build_Side"));
			break;

		}
		m_ePreState = m_eState;
	}

	m_fInteractTIme += _fTimeDelta;

	if (1.5f < m_fInteractTIme)
	{
		dynamic_cast<CInteractive_Object*>(m_pTarget)->Interact(0);
		m_fInteractTIme = 0.f;

		m_bInteract = false;
		m_bSelectAct = true;
		Reset_Target();
	}


}

void CWendy::Talk_Player(_float _fTimeDelta)
{
	CInventory_Manager::Get_Instance()->Get_Talk_list()->front()->Set_WendyTalk(true);

	CInventory_Manager* pinven = CInventory_Manager::Get_Instance();
	pinven->Get_Talk_list()->front()->setcheck(true);

	if (!m_bFirstCall)
	{
		m_bNextAct = false;
		m_bInteract = false;
		m_bArrive = false;
		return;
	}

	m_eState = CNPC::TALK;
	static_cast<CPlayer*>(m_pTarget)->Set_TalkMode(true);
	static_cast<CPlayer*>(m_pTarget)->Set_bOnlyActionKey(true);

	if (m_ePreState != m_eState)
	{
		m_fInteractTIme = 0.f;
		m_bInteract = true;
		Change_Texture(TEXT("Com_Texture_Talk"));
		m_ePreState = m_eState;

	/*	CCameraManager::CAM_STATE eState = CCameraManager::Get_Instance()->Get_CamState();

		if (eState == CCameraManager::CAM_PLAYER)
		{
			CCameraDynamic* pCamera = (CCameraDynamic*)CCameraManager::Get_Instance()->Get_CurrentCamera();
			pCamera->Set_TalkingMode(true);
			pCamera->Set_Target(this);
		}*/
	}


	if (m_iPreTalkCnt != m_iTalkCnt)
	{
		Play_Sound(_fTimeDelta);
		if (!m_bOwner)
		{//IsPartyed
			switch (m_iTalkCnt)
			{
			case 1:
				pinven->Get_Talk_list()->front()->Set_Texnum1(0);
				break;
			case 2:
				pinven->Get_Talk_list()->front()->Set_Texnum1(1);
				break;
			case 3:
				if (m_bAccept)
				{
					pinven->Get_Talk_list()->front()->Set_Texnum1(2);
					m_bNextAct = true;

				}
				else
				{
					pinven->Get_Talk_list()->front()->Set_Texnum1(3);
					m_bNextAct = false;
				}
				break;
			case 4:
				m_bSelectAct = true;
				Clear_Activated();
				static_cast<CPlayer*>(m_pTarget)->Set_TalkMode(false);
				static_cast<CPlayer*>(m_pTarget)->Set_bOnlyActionKey(false);

				/*CCameraManager::CAM_STATE eState = CCameraManager::Get_Instance()->Get_CamState();
				if (eState == CCameraManager::CAM_PLAYER)
				{
					CCameraDynamic* pCamera = (CCameraDynamic*)CCameraManager::Get_Instance()->Get_CurrentCamera();
					pCamera->Set_TalkingMode(false);
				}*/

				m_iTalkCnt = 0;
				m_bInteract = false;
				m_bFirstCall = false;
				if (m_bNextAct)
				{
					m_bOwner = true;
					m_pOwner = static_cast<CPawn*>(m_pTarget);
					static_cast<CPlayer*>(m_pTarget)->Add_Party(TEXT("Wendy"), this);

					CInventory_Manager* pinv = CInventory_Manager::Get_Instance();
					auto k = pinv->Get_Party_list();
					for (auto iter : *k)
					{
						if (iter->Get_check() == false)
						{
							iter->Set_check(true);
							iter->Set_texnum(0);
							break;
						}
					}
				}
				else
				{
					Reset_Target();
				}

				pinven->Get_Talk_list()->front()->setcheck(false);
				CInventory_Manager::Get_Instance()->Get_Talk_list()->front()->Set_WendyTalk(false);
				break;
			}
		}
		else
		{//Solo
			switch (m_iTalkCnt)
			{
			case 1:
				pinven->Get_Talk_list()->front()->Set_Texnum1(4);
				break;
			case 2:
				pinven->Get_Talk_list()->front()->Set_Texnum1(5);
				break;
			case 3:
				if (m_bAccept)
				{
					pinven->Get_Talk_list()->front()->Set_Texnum1(6);
					m_bNextAct = false;
				}
				else
				{
					pinven->Get_Talk_list()->front()->Set_Texnum1(8);
					m_bNextAct = true;
				}
				break;
			case 4:
				m_bSelectAct = true;
				Clear_Activated();
				static_cast<CPlayer*>(m_pTarget)->Set_TalkMode(false);
				static_cast<CPlayer*>(m_pTarget)->Set_bOnlyActionKey(false);

				/*CCameraManager::CAM_STATE eState = CCameraManager::Get_Instance()->Get_CamState();

				if (eState == CCameraManager::CAM_PLAYER)
				{
					CCameraDynamic* pCamera = (CCameraDynamic*)CCameraManager::Get_Instance()->Get_CurrentCamera();
					pCamera->Set_TalkingMode(false);
				}*/

				if (!m_bNextAct)
				{
					static_cast<CPlayer*>(m_pTarget)->Release_Party(TEXT("Wendy"));

					Reset_Target();
					m_bOwner = false;
					m_pOwner = nullptr;

					CInventory_Manager* pinv = CInventory_Manager::Get_Instance();
					auto k = pinv->Get_Party_list();
					for (auto iter : *k)
					{
						if (iter->Get_check() == true && iter->Get_texnum() == 0)
						{
							iter->Set_check(false);
							iter->Set_texnum(3);
							break;
						}
					}
				}
			
				m_iTalkCnt = 0;
				m_bInteract = false;
				m_bFirstCall = false;
				pinven->Get_Talk_list()->front()->setcheck(false);
				CInventory_Manager::Get_Instance()->Get_Talk_list()->front()->Set_WendyTalk(false);
				break;
			}
		}
		m_iPreTalkCnt = m_iTalkCnt;
	}



}

void CWendy::Talk_Friend(_float _fTimeDelta)
{

	m_fInteractTIme += _fTimeDelta;

	m_eState = CNPC::TALK;

	if (m_ePreState != m_eState)
	{
		m_fInteractTIme = 0.f;
		m_bInteract = true;
		m_ePreState = m_eState;
		Change_Texture(TEXT("Com_Texture_Talk"));
		static_cast<CPig*>(m_pTarget)->Interact(_fTimeDelta, 0);
		Play_Sound(_fTimeDelta);
	}

	if (2.f < m_fInteractTIme)
	{
		m_fInteractTIme = 0.f;
		m_bInteract = false;

		_int iRandNum = rand() % 2;
		if (iRandNum == 1)
		{
			static_cast<CPig*>(m_pTarget)->Interact(_fTimeDelta, 2);
			m_bNextAct = false;
		}
		else
		{
			static_cast<CPig*>(m_pTarget)->Interact(_fTimeDelta, 1);
			m_bNextAct = true;
		}
	}


}

void CWendy::Create_Bullet(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CSkill::SKILL_DESC SkillDesc;

	SkillDesc.eDirState = DIR_END;
	SkillDesc.eSkill = CSkill::SKILL_TYPE::REVIVE;
	SkillDesc.vTargetPos = m_pHeal_Target->Get_Position();
	SkillDesc.vPosition = m_pHeal_Target->Get_Position();
	SkillDesc.vScale = static_cast<CMonster*>(m_pHeal_Target)->Get_Scale();
	SkillDesc.pTarget = m_pHeal_Target;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Skill"), m_iCurrentLevelndex, TEXT("Skill"), &SkillDesc)))
		return;
	SkillDesc.eSkill = CSkill::SKILL_TYPE::ICE_BLAST;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Skill"), m_iCurrentLevelndex, TEXT("Skill"), &SkillDesc)))
		return;
}

void CWendy::Create_Heal(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CSkill::SKILL_DESC SkillDesc;
	
	//Select_HealTarget(0.f);

	SkillDesc.eDirState = DIR_END;
	SkillDesc.eSkill = CSkill::SKILL_TYPE::HEAL;
	SkillDesc.vTargetPos =  m_pHeal_Target->Get_Position();
	SkillDesc.vPosition = m_pHeal_Target->Get_Position();
	SkillDesc.vScale = _float3(3.f, 3.f, 1.f);
	SkillDesc.pTarget = m_pHeal_Target;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Skill"), m_iCurrentLevelndex, TEXT("Skill"), &SkillDesc)))
		return;
}

void CWendy::MoveWithOwner(_float _fTimeDelta)
{
	_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (static_cast<CPawn*>(m_pTarget)->Get_ObjID() == OBJ_MONSTER && m_bFightMode)
	{
		m_pTransformCom->Go_PosTarget(_fTimeDelta, m_vTargetPos, _float3{ 0.f, 0.f, 0.f });
		if ((abs(vMyPos.x - m_vTargetPos.x) < 0.3 &&
			abs(vMyPos.z - m_vTargetPos.z) < 0.3))
		{
			m_bArrive = true;
			m_bDirChanged = false;
		}
	}
	else if (m_pTarget == m_pOwner)
	{
		m_pTransformCom->Go_PosTarget(_fTimeDelta, _float3(m_fPatrolPosX, Get_Position().y, m_fPatrolPosZ), _float3{ 0.f, 0.f, 0.f });
		if ((abs(vMyPos.x - m_fPatrolPosX) < 0.3 &&
			abs(vMyPos.z - m_fPatrolPosZ) < 0.3))
		{
			m_bArrive = true;
			m_bDirChanged = false;
		}
	}
}

void CWendy::MoveWithoutOwner(_float _fTimeDelta)
{
	_float3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (m_pTarget)
	{
		m_pTransformCom->Go_PosTarget(_fTimeDelta, m_vTargetPos, _float3{ 0.f, 0.f, 0.f });
		if ((abs(vMyPos.x - m_vTargetPos.x) < 0.3 &&
			abs(vMyPos.z - m_vTargetPos.z) < 0.3))
		{
			m_bArrive = true;
			m_bDirChanged = false;
		}
	}
	else
	{
		m_pTransformCom->Go_PosTarget(_fTimeDelta, _float3(m_fPatrolPosX, Get_Position().y, m_fPatrolPosZ), _float3{ 0.f, 0.f, 0.f });
		if ((abs(vMyPos.x - m_fPatrolPosX) < 0.3 &&
			abs(vMyPos.z - m_fPatrolPosZ) < 0.3))
		{
			m_bArrive = true;
			m_bDirChanged = false;
		}
	}
}

void CWendy::Select_HealTarget(_float _fTimeDelta)
{//for Heal
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	list<CGameObject*> list_Obj = *(pGameInstance->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_NPC")));
	list_Obj.push_back(m_pOwner);

	_uint iIndex = 0;

	_uint MinHP = 0;
	m_pHeal_Target = nullptr;
	for (auto& iter_Obj = list_Obj.begin(); iter_Obj != list_Obj.end();)
	{//NPC
		if (static_cast<CPawn*>(*iter_Obj)->Get_ObjID() == OBJID::OBJ_NPC)
		{
			if ((*iter_Obj)->Get_Dead()
				|| static_cast<CNPC*>(*iter_Obj)->Get_Info().iCurrentHp >= static_cast<CNPC*>(*iter_Obj)->Get_Info().iMaxHp
				|| !(static_cast<CNPC*>(*iter_Obj)->Get_HasOwner()))
			{
				iter_Obj++;
				continue;
			}

			if (m_pHeal_Target == nullptr)
			{
				MinHP = static_cast<CNPC*>(*iter_Obj)->Get_Info().iCurrentHp / static_cast<CNPC*>(*iter_Obj)->Get_Info().iMaxHp;
				m_pHeal_Target = (*iter_Obj);
			}
			else
			{
				if (MinHP > static_cast<CNPC*>(*iter_Obj)->Get_Info().iCurrentHp)
				{
					m_pHeal_Target = (*iter_Obj);
					MinHP = static_cast<CNPC*>(*iter_Obj)->Get_Info().iCurrentHp / static_cast<CNPC*>(*iter_Obj)->Get_Info().iMaxHp;
				}
			}
			iter_Obj++;
			continue;
		}
		else
		{//Player
			if ((*iter_Obj)->Get_Dead() || static_cast<CPlayer*>(*iter_Obj)->Get_Player_Stat().fCurrentHealth >= static_cast<CPlayer*>(*iter_Obj)->Get_Player_Stat().fMaxHealth)
			{
				iter_Obj++;
				continue;
			}

			if (m_pHeal_Target == nullptr)
			{
				MinHP = (_uint)static_cast<CPlayer*>(*iter_Obj)->Get_Player_Stat().fCurrentHealth / (_uint)static_cast<CPlayer*>(*iter_Obj)->Get_Player_Stat().fMaxHealth;
				m_pHeal_Target = (*iter_Obj);
			}
			else if (MinHP > (_uint)static_cast<CPlayer*>(*iter_Obj)->Get_Player_Stat().fCurrentHealth)
			{
				m_pHeal_Target = (*iter_Obj);
				MinHP = (_uint)static_cast<CPlayer*>(*iter_Obj)->Get_Player_Stat().fCurrentHealth / (_uint)static_cast<CPlayer*>(*iter_Obj)->Get_Player_Stat().fMaxHealth;

			}
			iter_Obj++;
			continue;
		}
	}

	Safe_Release(pGameInstance);
}

void CWendy::Select_ReviveTarget(void)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	list<CGameObject*> list_Obj = *(pGameInstance->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_NPC")));
	//list_Obj.push_back(m_pOwner);

	_uint iIndex = 0;

	m_pHeal_Target = nullptr;
	for (auto& iter_Obj = list_Obj.begin(); iter_Obj != list_Obj.end();)
	{//NPC
		if (static_cast<CPawn*>(*iter_Obj)->Get_ObjID() == OBJID::OBJ_NPC)
		{
			if ((*iter_Obj)->Get_Dead() == false)
			{
				iter_Obj++;
				continue;
			}

			if (m_pHeal_Target == nullptr)
			{
				m_pHeal_Target = (*iter_Obj);
				break;
			}
		}
		iter_Obj++;
		continue;
	}

	Safe_Release(pGameInstance);
}

DIR_STATE CWendy::Check_Direction(void)
{
	//내 자신의 Look에서 3번 변환한 값
	_float3 vMyLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float4x4 OriginMat = m_pTransformCom->Get_WorldMatrix();
	_float4x4 RotateMat = OriginMat;
	_float3 vRot45 = *(_float3*)&OriginMat.m[2][0];
	_float3 vRot135 = *(_float3*)&OriginMat.m[2][0];

	//Turn
	D3DXMatrixRotationAxis(&RotateMat, &_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	D3DXVec3TransformNormal(&vRot45, &vRot45, &RotateMat);
	//vRot45 = *(_float3*)&RotateMat.m[2][0];
	D3DXMatrixRotationAxis(&RotateMat, &_float3(0.f, 1.f, 0.f), D3DXToRadian(135.f));
	D3DXVec3TransformNormal(&vRot135, &vRot135, &RotateMat);
	//vRot135 = *(_float3*)&RotateMat.m[2][0];;
	///////
	_float3 vTargetLook = m_vTargetPos - Get_Pos();

	D3DXVec3Normalize(&vRot45, &vRot45);
	D3DXVec3Normalize(&vRot135, &vRot135);
	D3DXVec3Normalize(&vMyLook, &vMyLook);
	D3DXVec3Normalize(&vTargetLook, &vTargetLook);

	//각 내적값들 구하기
	_float fTargetDot = D3DXVec3Dot(&vMyLook, &vRot45);
	fTargetDot = acos(fTargetDot);
	_float fDegree45 = D3DXToDegree(fTargetDot);

	fTargetDot = D3DXVec3Dot(&vMyLook, &vRot135);
	fTargetDot = acos(fTargetDot);
	_float fDegree135 = D3DXToDegree(fTargetDot);

	fTargetDot = D3DXVec3Dot(&vMyLook, &vTargetLook);
	fTargetDot = acos(fTargetDot);
	_float fDegreeTarget = D3DXToDegree(fTargetDot);

	//외적하기
	_float3 vCrossOut;
	//D3DXVec3Cross(&vCrossOut, &vMyLook, &vTargetLook);
	D3DXVec3Cross(&vCrossOut, &vMyLook, &vTargetLook);


	DIR_STATE eDir = DIR_END;
	//Check Left Or Right
	if (vCrossOut.y > 0.f)//Right
	{
		if (fDegreeTarget > 0.f && fDegreeTarget <= 60.f)
		{
			//cout << "y+, UP, Degree: " << fDegreeTarget << endl;
			return DIR_UP;
		}
		else if (fDegreeTarget > 60.f && fDegreeTarget <= 135.f)
		{
			//cout << "y+, Right, Degree: " << fDegreeTarget << endl;
			return DIR_RIGHT;
		}
		else if (fDegreeTarget > 135.f && fDegreeTarget <= 180.f)
		{
			//cout << "y+, Down, Degree: " << fDegreeTarget << endl;
			return DIR_DOWN;
		}
	}
	else//Left
	{
		if (fDegreeTarget > 0.f && fDegreeTarget <= 60.f)
		{
			//cout << "y-, UP, Degree: " << fDegreeTarget << endl;
			return DIR_UP;
		}
		else if (fDegreeTarget > 60.f && fDegreeTarget <= 135.f)
		{
			//cout << "y-, Left, Degree: " << fDegreeTarget << endl;
			return DIR_LEFT;
		}
		else if (fDegreeTarget > 135.f && fDegreeTarget <= 180.f)
		{
			//cout << "y-, Down, Degree: " << fDegreeTarget << endl;
			return DIR_DOWN;
		}
	}

	//cout << "Error " << fDegreeTarget << endl;
	return DIR_DOWN;

}

void CWendy::Find_Priority()
{

	int i = rand() % 4;
	switch (i)
	{
	case 1:
		Find_Friend();
		break;
	case 2:
		Find_Berry();
		break;
	case 0:
	case 3:
	case 4:
		Set_RandPos(0.f);
		break;
	}
}

void CWendy::Find_Friend()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	list<CGameObject*>* list_Obj = pGameInstance->Get_ObjectList(m_iCurrentLevelndex, TEXT("Layer_Monster"));

	if (list_Obj == nullptr)
		return;

	_uint iIndex = 0;
	Reset_Target();
	m_pTarget = nullptr;
	for (auto& iter_Obj = list_Obj->begin(); iter_Obj != list_Obj->end();)
	{
		if ((*iter_Obj) != nullptr && !dynamic_cast<CMonster*>(*iter_Obj)->Get_Aggro()
			&& dynamic_cast<CMonster*>(*iter_Obj)->Get_MonsterID() == CMonster::MONSTER_ID::PIG)
		{
			if (m_pTarget == nullptr)
			{
				m_pTarget = *iter_Obj;
				m_bSelectAct = false;
			}

			_float fCmpDir = (Get_Pos().x - (*iter_Obj)->Get_Position().x)*(Get_Pos().x - (*iter_Obj)->Get_Position().x)
				+ (Get_Pos().y - (*iter_Obj)->Get_Position().y)*(Get_Pos().y - (*iter_Obj)->Get_Position().y)
				+ (Get_Pos().z - (*iter_Obj)->Get_Position().z)*(Get_Pos().z - (*iter_Obj)->Get_Position().z);

			_float fTargetDir = (Get_Pos().x - (m_pTarget)->Get_Position().x)*(Get_Pos().x - (m_pTarget)->Get_Position().x)
				+ (Get_Pos().y - (m_pTarget)->Get_Position().y)*(Get_Pos().y - (m_pTarget)->Get_Position().y)
				+ (Get_Pos().z - (m_pTarget)->Get_Position().z)*(Get_Pos().z - (m_pTarget)->Get_Position().z);

			if (fCmpDir < fTargetDir)
			{
				m_pTarget = *iter_Obj;
			}

			++iIndex;
			iter_Obj++;
		}
		else
		{
			++iIndex;
			iter_Obj++;
			continue;
		}
	}
	Safe_AddRef(m_pTarget);
	Safe_Release(pGameInstance);
}

void CWendy::Find_Enemy()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	list<CGameObject*>* list_Obj = pGameInstance->Get_ObjectList(m_iCurrentLevelndex, TEXT("Layer_Monster"));

	if (list_Obj == nullptr)
		return;

	_uint iIndex = 0;

	Reset_Target();
	m_pTarget = nullptr;
	for (auto& iter_Obj = list_Obj->begin(); iter_Obj != list_Obj->end();)
	{
		if ((*iter_Obj) != nullptr && dynamic_cast<CMonster*>(*iter_Obj)->Get_Aggro())
		{
			_float fCmpDir = (Get_Pos().x - (*iter_Obj)->Get_Position().x)*(Get_Pos().x - (*iter_Obj)->Get_Position().x)
				+ (Get_Pos().z - (*iter_Obj)->Get_Position().z)*(Get_Pos().z - (*iter_Obj)->Get_Position().z);

			if (fCmpDir > m_fDetectRange)
			{
				++iIndex;
				iter_Obj++;
				continue;
			}

			if (m_pTarget == nullptr)
			{
				m_pTarget = *iter_Obj;
				m_bSelectAct = false;
			}

			_float fTargetDir = (Get_Pos().x - (m_pTarget)->Get_Position().x)*(Get_Pos().x - (m_pTarget)->Get_Position().x)
				+ (Get_Pos().z - (m_pTarget)->Get_Position().z)*(Get_Pos().z - (m_pTarget)->Get_Position().z);

			if (fCmpDir < fTargetDir)
			{
				m_pTarget = *iter_Obj;
			}

			++iIndex;
			iter_Obj++;
		}
		else
		{
			++iIndex;
			iter_Obj++;
			continue;
		}
	}

	if (m_pTarget == nullptr)
	{
		m_bFightMode = false;
	}

	Safe_AddRef(m_pTarget);

	Safe_Release(pGameInstance);
}

void CWendy::Find_Berry()
{
	//후에 Primary_queue로 각 레이어들중에서 가장 가까운 객체를 m_pTarget
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	list<CGameObject*>* list_Obj = pGameInstance->Get_ObjectList(m_iCurrentLevelndex, TEXT("Layer_Object"));

	if (list_Obj == nullptr)
		return;

	_uint iIndex = 0;
	Reset_Target();
	m_pTarget = nullptr;
	for (auto& iter_Obj = list_Obj->begin(); iter_Obj != list_Obj->end();)
	{
		if ((*iter_Obj) != nullptr && !dynamic_cast<CInteractive_Object*>(*iter_Obj)->Get_CanInteract()
			&& dynamic_cast<CInteractive_Object*>(*iter_Obj)->Get_InteractName() == INTERACTOBJ_ID::BERRYBUSH)
		{

			if (m_pTarget == nullptr)
			{
				m_pTarget = *iter_Obj;
				m_bSelectAct = false;
			}

			_float fCmpDir = (Get_Pos().x - (*iter_Obj)->Get_Position().x)*(Get_Pos().x - (*iter_Obj)->Get_Position().x)
				+ (Get_Pos().y - (*iter_Obj)->Get_Position().y)*(Get_Pos().y - (*iter_Obj)->Get_Position().y)
				+ (Get_Pos().z - (*iter_Obj)->Get_Position().z)*(Get_Pos().z - (*iter_Obj)->Get_Position().z);

			_float fTargetDir = (Get_Pos().x - (m_pTarget)->Get_Position().x)*(Get_Pos().x - (m_pTarget)->Get_Position().x)
				+ (Get_Pos().y - (m_pTarget)->Get_Position().y)*(Get_Pos().y - (m_pTarget)->Get_Position().y)
				+ (Get_Pos().z - (m_pTarget)->Get_Position().z)*(Get_Pos().z - (m_pTarget)->Get_Position().z);

			if (fCmpDir < fTargetDir)
			{
				m_pTarget = *iter_Obj;
			}

			++iIndex;
			iter_Obj++;
		}
		else
		{
			++iIndex;
			iter_Obj++;
			continue;
		}
	}

	Safe_AddRef(m_pTarget);
	Safe_Release(pGameInstance);

}

void CWendy::Find_Player()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	Reset_Target();

	m_pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	Safe_AddRef(m_pTarget);

	Safe_Release(pGameInstance);
}

CWendy * CWendy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWendy* pInstance = new CWendy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CWendy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWendy::Clone(void * pArg)
{
	CWendy* pInstance = new CWendy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CWendy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWendy::Free()
{
	__super::Free();

	Safe_Release(BehaviorTree);
}
