#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CCollider;
END

BEGIN(Client)

class CBullet : public CGameObject
{
public:
	CBullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual int Tick(_float fTimeDelta)override;
	virtual void Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render() override;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private://Init
	HRESULT Init_Data(void);
	HRESULT Texture_Clone(void);
private://Passive
	void Excute(_float fTimeDelta);
	void AttackCheck(_float _fTimeDelta);
private:/*Act Player`s Bullets*/
	void Red_Smoke(_float _fTimeDelta);
	void Bomb(void);
	//ApplyDamage
	virtual void	Apply_Damage(_float Damage, CGameObject* DamagedObj, void* AttackType = nullptr);
	virtual void	Apply_Damage_Multi(_float fDamage, vector<CGameObject*>& vecDamagedObj, void* AttackType = nullptr);

	//Campare Terrain& Bullets
	_bool Compare_Terrain(void);
private:/*for Texture*/
	HRESULT Render_TextureState();
	HRESULT Change_Texture(const _tchar* LayerTag);
private:
	void SetUp_BillBoard();
private:/*For Components*/
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;

private: //values		
	//WEAPON_TYPE				m_eWeaponType = WEAPON_TYPE::WEAPON_HAND;
	const _tchar*			m_TimerTag = TEXT("");
	_bool					m_bIsAttacked = false;

	BULLETDATA				m_tBulletData;
	_float					m_fAccDeadTimer = 0.f;
	_float					m_fDamageTimger = 0.f; //jisokDamage

	_float					m_fDamage= 20.f;
	//Test
	_float					m_fTime = 0.f;
	_float					m_fPower = 2.f;
	_float					m_MaxTime;
	_float					m_AddTime;

public:
	static CBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual CGameObject* Clone_Load(const _tchar* VIBufferTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

END