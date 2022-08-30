#include "stdafx.h"
#include "..\Public\ParticleSystem.h"
#include "GameInstance.h"

CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CParticleSystem::CParticleSystem(const CParticle & rhs)
	: CGameObject(rhs)
{
}

HRESULT CParticleSystem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleSystem::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_StateDesc, pArg, sizeof(STATEDESC));

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(Ready_ParticleSystem()))
		return E_FAIL;

	if (FAILED(Ready_VIBuffer()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_StateDesc.vPosition);
	m_bStart = false;

	return S_OK;
}

int CParticleSystem::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	/* ��ƼŬ ����Ʈ���� ���� ��ƼŬ���� �����*/
	if (FAILED(Kill_DeadParticles()))
		return E_FAIL;

	m_dCurrentDuration += fTimeDelta;
	/*���� ���� �ð��� DeadDuration �Ⱓ���� ������ ��ƼŬ�� ��ġ�� �������ش�*/
	if (m_StateDesc.dDuration > m_dCurrentDuration)
	{
		switch (m_StateDesc.eType)
		{
		case Client::CParticleSystem::PARTICLE_LEAF:
			//�Լ� �߰�
			break;
		case Client::CParticleSystem::PARTICLE_END:
			break;
		default:
			break;
		}
	}

	/* Ȥ�� ��� ��ƼŬ�� �׾��ٸ� �ٷ� Dead ó��*/
	if (m_bStart && Check_AllParticleDead())
	{
		m_bDead = true;
		return OBJ_DEAD;
	}

	//��ƼŬ �ý����� ����� ���� (��ȿ���������� ��å���� �˷��� ����)
	/*1. ��� ��ƼŬ�� �����Ѵ�.*/
	if (FAILED(Update_Particles(fTimeDelta)))
		return E_FAIL;

	/*2. ��� ��� �ִ� ��ƼŬ���� ���۷� �����Ѵ� */
	if (FAILED(Update_VIBuffer()))
		return E_FAIL;
	
	/*3. ���ؽ� ���۸� �׸���. �̰� �����κп���*/
	return OBJ_NOEVENT;
}

void CParticleSystem::Late_Tick(_float fTimeDelta)
{
}

HRESULT CParticleSystem::Render()
{
	return E_NOTIMPL;
}

HRESULT CParticleSystem::SetUp_Components(void * pArg)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For Com_Texture */
	CTexture::TEXTUREDESC		TextureDesc;
	TextureDesc.m_iStartTex = 0;
	TextureDesc.m_iEndTex = 32;
	TextureDesc.m_fSpeed = 60;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pig_RUN_DOWN"), (CComponent**)&m_pTextureCom, &TextureDesc)))
		return E_FAIL;

	/* For.Transform*/
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticleSystem::Ready_ParticleSystem()
{
	m_ParticleArray = new PARTICLE_INFO[m_StateDesc.iMaxParticleCount];
	ZeroMemory(m_ParticleArray, sizeof(PARTICLE_INFO) * m_StateDesc.iMaxParticleCount);
	if (nullptr == m_ParticleArray)
		return E_FAIL;

	for (int i = 0; i < m_StateDesc.iMaxParticleCount; ++i)
	{
		m_ParticleArray[i].bActive = false;
	}

	m_iCurrentParticleCount = 0;
	m_dCurrentSpawnTime = 0;
	return S_OK;
}

HRESULT CParticleSystem::Ready_VIBuffer()
{
	return E_NOTIMPL;
}

HRESULT CParticleSystem::Update_Particles(_float fTimeDelta)
{
	return E_NOTIMPL;
}

HRESULT CParticleSystem::Update_VIBuffer()
{
	return E_NOTIMPL;
}

HRESULT CParticleSystem::Kill_DeadParticles()
{
	for (int i = 0; i < m_StateDesc.iMaxParticleCount; ++i)
	{
		/* ��ƼŬ�� �ൿ�ϰ� �ְ�,  �� Life Time�� ��ƼŬ�� Dead LifeTime���� Ŭ ���*/
		if ((true == m_ParticleArray[i].bActive) && (m_StateDesc.dParticleLifeTime <= m_ParticleArray[i].dCurrentLifeTime))
		{
			m_ParticleArray[i].bActive = false; /*��ƼŬ�� �ൿ�� �����*/
			--m_iCurrentParticleCount; /*���� ��� �ִ� ��ƼŬ�� ���� �����*/

			for (int j = i; j < m_StateDesc.iMaxParticleCount - 1; ++j) /*����ִ� ��ƼŬ�� ������ŭ*/
			{
				/* ��� �ִ� �ֵ��� ��ĭ�� ������ ���� ���� �ϱ�*/
				/* �� �ڿ� �ִ� �ֵ��� ������ ��� �ִ� ������ �ƴϴϲ� �Ű� x*/
				m_ParticleArray[j].bActive = m_ParticleArray[j + 1].bActive;
				m_ParticleArray[j].vPosition = m_ParticleArray[j + 1].vPosition;
				m_ParticleArray[j].vVelocity = m_ParticleArray[j + 1].vVelocity;
				m_ParticleArray[j].dCurrentLifeTime = m_ParticleArray[j + 1].dCurrentLifeTime;
			}
		}
	}

	return S_OK;
}

_bool CParticleSystem::Check_AllParticleDead()
{
	for (int i = 0; i < m_StateDesc.iMaxParticleCount; ++i)
	{
		/* �ϳ��� ����ִٸ� False��ȯ*/
		if (m_ParticleArray[i].bActive)
			return false;
	}
	return true;
}

CParticleSystem * CParticleSystem::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return nullptr;
}

CGameObject * CParticleSystem::Clone(void * pArg)
{
	return nullptr;
}

void CParticleSystem::Free()
{
}
