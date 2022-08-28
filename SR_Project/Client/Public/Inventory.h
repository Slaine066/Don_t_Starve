#pragma once

#include "Client_Defines.h"

#include "Base.h"
#include "MainInventory_front.h"
#include "Equipment_front.h"
#include "Pont.h"
#include "Playerhp_pont.h"
/*  ������ ���ӿ�����Ʈ���� ��Ƽ� �����Ѵ�. : ����ڰ� ������ ���� ���ؿ� ����. */


class CInventory_Manager final : public CBase
{
	DECLARE_SINGLETON(CInventory_Manager)
private:
	CInventory_Manager();
	virtual ~CInventory_Manager() = default;

public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_GameObject(_uint iLevelIndex, void* pArg = nullptr);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

	list<CMainInventory_front*>* Get_Inven_list(void) { return&m_MainInventorylist; }
	list<CPont*>* Get_Pont_list(void) { return &m_Pontlist; }
	list<CEquipment_front*>* Get_Equipment_list(void) { return&m_Equipmentlist; }
	list<CPlayerhp_pont*>* Get_playerhp_Pont_list(void) { return &m_Hppontlist; }

	void sethp(_uint hp) { m_ihp = hp; }



public:
	class CGameObject* Find_Objects(_uint iLevelIndex, const _tchar* pLayerTag);

private: /* ���� �����ϰ� ���� ��� �׷��� ���� �������� ��ü�� �߰��Ѵ�. */
	_uint										m_iNumLevels = 0;
	/*map<const _tchar*, class CLayer*>*			m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>	LAYERS;*/
	list<CMainInventory_front*> m_MainInventorylist;//[INVEN_END]
	list<CEquipment_front*> m_Equipmentlist;
	list<CPont*> m_Pontlist;
	list<CPlayerhp_pont*> m_Hppontlist;

	typedef list<CMainInventory_front*> INVENLIST;

	_uint m_ihp = 7;

	//private:
	//class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	//class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);


public:
	virtual void Free() override;
};