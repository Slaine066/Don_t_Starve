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
			Particle_Leaf_Moveing(fTimeDelta);
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
	/*1. ��� ��ƼŬ�� �����Ѵ�.(�� ��ƼŬ���� �ð��� ��ġ ������Ʈ)*/
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
	if (nullptr == m_pRendererCom)
		return;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CParticleSystem::Render()
{
	if (nullptr == m_pVBuffer)
		return E_FAIL;

	if (FAILED(Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticleSystem::Render_VIBuffer()
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pVBuffer)
		return E_FAIL;

	m_pGraphic_Device->SetStreamSource(0, m_pVBuffer, 0, m_iStride);
	m_pGraphic_Device->SetFVF(m_dwFVF);
	m_pGraphic_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_iNumPrimitive);

	return S_OK;
}

HRESULT CParticleSystem::Particle_Leaf_Moveing(_float fTimeDelta)
{
	m_bStart = true;
	_bool emitParticle = false;
	_bool found = false;
	float positionX, positionY, positionZ, red, green, blue;
	int index, i, j;

	m_dCurrentSpawnTime += fTimeDelta;
	emitParticle = false;

	if (m_dCurrentSpawnTime > (m_StateDesc.dSpawnTime / m_StateDesc.fParticlePerSecond))
	{
		m_dCurrentSpawnTime = 0.0f;
		emitParticle = true;
	}

	if ((emitParticle == true) && (m_iCurrentParticleCount < (m_StateDesc.iMaxParticleCount - 1)))
	{
		m_iCurrentParticleCount++;

		positionX = (((float)rand() - (float)rand()) / RAND_MAX) * m_StateDesc.vParticleDeviation.x;
		positionY = (((float)rand() - (float)rand()) / RAND_MAX) * m_StateDesc.vParticleDeviation.y;
		positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * m_StateDesc.vParticleDeviation.z;
		_float3 vPosition = _float3(positionX, positionY, positionZ);

		_float3 vVelocity = m_StateDesc.vVelocity * (((float)rand() - (float)rand()) / RAND_MAX) * m_StateDesc.fVelocityDeviation;

		red = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		green = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

		_float4 vColor = _float4(red, green, blue, 1.f);

		index = 0;
		found = false;
		while (!found)
		{
			if ((m_ParticleArray[index].bActive == false) || Compute_ViewZ(m_ParticleArray[index].vPosition) < Compute_ViewZ(vPosition))
			{
				found = true;
			}
			else
			{
				index++;
			}
		}

		i = m_iCurrentParticleCount;
		j = i - 1;

		while (i != index)
		{
			m_ParticleArray[i].bActive = m_ParticleArray[j].bActive;
			m_ParticleArray[i].vPosition = m_ParticleArray[j].vPosition;
			m_ParticleArray[i].vVelocity = m_ParticleArray[j].vVelocity;
			m_ParticleArray[i].vColor = m_ParticleArray[j].vColor;
			m_ParticleArray[i].dCurrentLifeTime = m_ParticleArray[j].dCurrentLifeTime;
			i--;
			j--;
		}

		m_ParticleArray[i].bActive = true;
		m_ParticleArray[i].vPosition = vPosition;
		m_ParticleArray[i].vVelocity = vVelocity;
		m_ParticleArray[i].vColor = vColor;
		m_ParticleArray[i].dCurrentLifeTime = 0;
	}

	return S_OK;
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
	for (int i = 0; i < m_StateDesc.iMaxParticleCount; ++i)
	{
		if (m_ParticleArray[i].bActive)
		{
			// �� ��ƼŬ���� �ð��� ��ġ ������Ʈ
			switch (m_StateDesc.eType)
			{
			case Client::CParticleSystem::PARTICLE_LEAF:
				m_ParticleArray[i].dCurrentLifeTime += fTimeDelta;
				m_ParticleArray[i].vPosition += m_ParticleArray[i].vVelocity;
				break;
			case Client::CParticleSystem::PARTICLE_END:
				break;
			default:
				break;
			}
		}
	}
	return S_OK;
}

HRESULT CParticleSystem::Update_VIBuffer()
{
	ZeroMemory(m_Vertices, sizeof(VTXTEX) * m_iNumVertices);

	// For.Vertices
	VTXTEX*		pVertices = nullptr;
	int index = 0;

	m_pVBuffer->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);

	_float4x4 ViewMatrix, InvViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&InvViewMatrix, nullptr, &ViewMatrix);

	//�������� ��ġ�� ���� �޸� �ϰ�
	ZeroMemory((_float3*)InvViewMatrix.m[3], sizeof(_float3));

	//��ƼŬ ������ŭ ������ ��ǥ���� ������ǥ�� �ٲ��ش�.
	for (_int i = 0; i < m_StateDesc.iMaxParticleCount; ++i)
	{
		/// ������ �� �ﰢ��
		// Left Top
		_float3 LocalPos = _float3(-0.5f, 0.5f, 0);
		D3DXVec3TransformCoord(&LocalPos, &LocalPos, &InvViewMatrix);
		pVertices[index].vPosition = m_ParticleArray[i].vPosition + LocalPos;
		pVertices[index].vTexture = _float2(0.f, 0.f);
		m_Vertices[index] = pVertices[index];
		++index;

		// Right Top
		LocalPos = _float3(0.5f, 0.5f, 0);
		D3DXVec3TransformCoord(&LocalPos, &LocalPos, &InvViewMatrix);
		pVertices[index].vPosition = m_ParticleArray[i].vPosition + LocalPos;
		pVertices[index].vTexture = _float2(1.f, 0.f);
		m_Vertices[index] = pVertices[index];
		++index;

		// Right Bottom
		LocalPos = _float3(0.5f, -0.5f, 0);
		D3DXVec3TransformCoord(&LocalPos, &LocalPos, &InvViewMatrix);
		pVertices[index].vPosition = m_ParticleArray[i].vPosition + LocalPos;
		pVertices[index].vTexture = _float2(1.f, 1.f);
		m_Vertices[index] = pVertices[index];
		++index;

		/// ���� �Ʒ� �ﰢ��
		// Left Top
		LocalPos = _float3(-0.5f, 0.5f, 0);
		D3DXVec3TransformCoord(&LocalPos, &LocalPos, &InvViewMatrix);
		pVertices[index].vPosition = m_ParticleArray[i].vPosition + LocalPos;
		pVertices[index].vTexture = _float2(0.f, 0.f);
		m_Vertices[index] = pVertices[index];
		++index;

		// Right Bottom
		LocalPos = _float3(0.5f, -0.5f, 0);
		D3DXVec3TransformCoord(&LocalPos, &LocalPos, &InvViewMatrix);
		pVertices[index].vPosition = m_ParticleArray[i].vPosition + LocalPos;
		pVertices[index].vTexture = _float2(1.f, 1.f);
		m_Vertices[index] = pVertices[index];
		++index;

		// Left Bottom
		LocalPos = _float3(-0.5f, -0.5f, 0);
		D3DXVec3TransformCoord(&LocalPos, &LocalPos, &InvViewMatrix);
		pVertices[index].vPosition = m_ParticleArray[i].vPosition + LocalPos;
		pVertices[index].vTexture = _float2(0.f, 1.f);
		m_Vertices[index] = pVertices[index];
		++index;
	}

	m_pVBuffer->Unlock();

	return S_OK;
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
