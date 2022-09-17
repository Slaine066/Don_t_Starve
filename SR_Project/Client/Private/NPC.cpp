#include "stdafx.h"
#include "..\Public\NPC.h"
#include "GameInstance.h"


CNPC::CNPC(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CInteractive_Object(pGraphic_Device)
{
}

CNPC::CNPC(const CNPC & rhs)
	:CInteractive_Object(rhs)
{
}

HRESULT CNPC::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNPC::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	//Test
	m_eObjID = OBJID::OBJ_NPC;
	m_eInteract_OBJ_ID = INTERACTOBJ_ID::NPC;

	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);

	Init_Map();

	return S_OK;
}

int CNPC::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Update_Cooltime(fTimeDelta);
	Update_FightMode(fTimeDelta);
	Update_Position(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_iCurrentLevelndex = (LEVEL)CLevel_Manager::Get_Instance()->Get_CurrentLevelIndex();
	
	return OBJ_NOEVENT;
}

void CNPC::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CNPC::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CNPC::Find_Priority()
{
}

_float3 CNPC::Get_Pos(void)
{
	return (m_pTransformCom->Get_State(CTransform::STATE_POSITION));
}

_bool CNPC::Get_TargetDead(void)
{
	if (m_pTarget != nullptr)
	{
		if (m_pTarget->Get_Dead() == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}

}

_bool CNPC::Get_CloseToOwner(void)
{
	if (!m_bOwner)
		return false;

	_float Compare_Range = (m_pOwner->Get_Position().x - Get_Pos().x)*(m_pOwner->Get_Position().x - Get_Pos().x)
		+ (m_pOwner->Get_Position().y - Get_Pos().y)*(m_pOwner->Get_Position().y - Get_Pos().y)
		+ (m_pOwner->Get_Position().z - Get_Pos().z)*(m_pOwner->Get_Position().z - Get_Pos().z);

	if (m_fOwnerRadius > Compare_Range)
	{
		Clear_Activated();
		return true;
	}
	else
	{
		m_pTarget = m_pOwner;
		Clear_Activated();
		return false;
	}

	return _bool();
}


void CNPC::Free()
{
	__super::Free();
}

void CNPC::Attack(_float _fTimeDelta)
{
}

void CNPC::Interrupted(_float _fTimeDelta)
{
}

void CNPC::Make_Interrupt(CPawn * pCauser, _uint _InterruptNum)
{
}

_bool CNPC::Get_Target_Moved(_float _fTimeDelta)
{
	return _bool();
}

_bool CNPC::Set_TargetPos()
{
	if (m_pTarget == nullptr)
		return false;

	m_vTargetPos = m_pTarget->Get_Position();
	return true;
}





CNPC::NPC_STATE CNPC::Find_Activated(void)
{
	for (auto& iter : m_NPCStates)
	{
		if (iter.second == true)
		{
			return iter.first;
		}
	}

	return NPC_STATE::STATE_END;
}

void CNPC::Init_Map(void)
{
	for (_uint i = 0; i < (_uint)CNPC::NPC_STATE::STATE_END; ++i)
	{
		m_NPCStates.emplace((CNPC::NPC_STATE)i, false);
	}
}

void CNPC::Clear_Activated(void)
{
	for (auto& iter : m_NPCStates)
	{
		iter.second = false;
	}
}

void CNPC::Update_Cooltime(_float _fTimeDelta)
{
	if (!m_bCanAttack)
	{
		if (m_fAtk_Cur_CoolTime <= 0.f)
		{
			m_fAtk_Cur_CoolTime = m_fAtk_Max_CoolTime;
			m_bCanAttack = true;
		}
		m_fAtk_Cur_CoolTime -= _fTimeDelta;
	}

	
}

void CNPC::Update_FightMode(_float _fTimeDelta)
{
	if (m_bFightMode && m_pTarget == nullptr)
	{
		m_fModeTime += _fTimeDelta;
		if (m_fModeTime > 2.f)
		{
			m_fModeTime = 0.f;
			m_bFightMode = false;
		}
		else
		{
			m_fModeTime = 0.f;
		}
	}

}
