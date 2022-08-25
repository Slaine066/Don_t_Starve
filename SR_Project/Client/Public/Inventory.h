#pragma once

#include "Client_Defines.h"

#include "Base.h"
#include "MainInventory_front.h"

/*  ������ ���ӿ�����Ʈ���� ��Ƽ� �����Ѵ�. : ����ڰ� ������ ���� ���ؿ� ����. */


class CInventory_Manager final : public CBase
{
	DECLARE_SINGLETON(CInventory_Manager)
private:
	CInventory_Manager();
	virtual ~CInventory_Manager() = default;

public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_GameObject( _uint iLevelIndex,  void* pArg = nullptr);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

	list<CMainInventory_front*>* Get_Inven_list(void) { return& m_MainInventorylist; }

public:
	//class CGameObject* Find_Objects(_uint iLevelIndex, const _tchar* pLayerTag);

private: /* ���� �����ϰ� ���� ��� �׷��� ���� �������� ��ü�� �߰��Ѵ�. */
	_uint										m_iNumLevels = 0;
	/*map<const _tchar*, class CLayer*>*			m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>	LAYERS;*/
	list<CMainInventory_front*> m_MainInventorylist;//[INVEN_END]

	typedef list<CMainInventory_front*> INVENLIST ; 

//private:
	//class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	//class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);


public:
	virtual void Free() override;
};