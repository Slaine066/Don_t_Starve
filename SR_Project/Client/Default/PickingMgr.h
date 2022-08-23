#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CTerrain;
class CPickingMgr : public CBase
{
	DECLARE_SINGLETON(CPickingMgr)
private:
	explicit CPickingMgr();
	virtual ~CPickingMgr() = default;

public:
	HRESULT	Ready_PickingMgr(PDIRECT3DDEVICE9 pGraphic_Device);

public:
	void	Add_PickingGroup(CGameObject* pGameObject);
	void	Out_PickingGroup(CGameObject* pGameObject);

public:
	//���ӿ�����Ʈ�� ��ŷ�Ѵ�.
	CGameObject*	Picking(POINT& pt);
	//vOrigin ���κ��� ���� ����� ������Ʈ�� ��ȯ�Ѵ�. Self�� �����Ѵ�.
	CGameObject*	Get_Closest_Object(_float3 vOrigin, CGameObject* pSelf = nullptr);
public:
	_bool TerrainPicking();

	void			Clear_PickingMgr();

private:
	CTerrain*			m_pTerrain = nullptr;
	list<CGameObject*>	m_GameObjects;
	
public:
	virtual void Free() override;
};
END
