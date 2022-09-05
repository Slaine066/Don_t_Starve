#pragma once
#include "Particle.h"

BEGIN(Client)

// ��ƼŬ ���Ű� ���÷���, �Ҹ�, �������� �����ϴ� ������ �Ѵ� 
class CParticleSystem final : public CGameObject
{
public:
	enum PARTICLE_TYPE { PARTICLE_ROCK, PARTICLE_LEAF, PARTICLE_END };


public:
	typedef struct tagStateDesc
	{
		PARTICLE_TYPE	eType;
		// Particle TextureInfo
		LEVEL			eTextureScene;
		const _tchar*		pTextureKey;
		_uint				iTextureNum;

		// System CreateInfo
		_bool				bActive;
		_double			dDuration;

		// Particle Info
		_float2				vParticleScale;
		_float3				vPosition;
		_float3				vVelocity;
		_float				fVelocityDeviation;


		// Particles Info
		_int				iMaxParticleCount;
		_double			dParticleLifeTime;

		// System SpawnInfo
		_double			dSpawnTime;
		_float				fParticlePerSecond;
		_float3				vParticleDeviation; // ��ƼŬ ����
	}STATEDESC;  /* �������� ���� ��ƼŬ ����*/


	typedef struct tagParticleInfo
	{
		// Particle Info
		_bool			bActive;

		_float3			vVelocity;
		_float3			vPosition;

		_double		dCurrentLifeTime;
	}PARTICLE_INFO; /*���� ��ƼŬ ����*/

private:
	CParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleSystem(const CParticle& rhs);
	virtual ~CParticleSystem() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT SetUp_Components(void* pArg = nullptr);
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT Ready_ParticleSystem();
	HRESULT Ready_VIBuffer();
	HRESULT Update_Particles(_float fTimeDelta);
	HRESULT Update_VIBuffer();
	HRESULT Render_VIBuffer();
	
public:
	HRESULT Kill_DeadParticles();
	

public:
	HRESULT Particle_Rock_Mining(_float fTimeDelta);
	HRESULT Particle_Leaf_Moving(_float fTimeDelta);


public:
	_bool Check_AllParticleDead();
	_float Compute_ViewZ(_float3 WorldPos);
	void SetUp_BillBoard();

private:
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;

	STATEDESC		m_StateDesc;
	PARTICLE_TYPE	m_eType = PARTICLE_END;

private:
	_bool		m_bStart = false;
	_double		m_dCurrentDuration = 0;
	_double		m_dCurrentSpawnTime = 0;
	_int		m_iCurrentParticleCount = 0;

private:
	PARTICLE_INFO*			m_ParticleArray = nullptr;		// ��ƼŬ�� ������
	VTXTEX*					m_Vertices = nullptr;			// �������� ������

protected: // For.VertexBuffer
	LPDIRECT3DVERTEXBUFFER9		m_pVBuffer = nullptr;
	_uint			m_iStride = 0;
	_uint			m_iNumVertices = 0;
	_uint			m_iNumPrimitive = 0;
	_ulong			m_dwFVF = 0;

public:
	static CParticleSystem* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END