#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CMainInventory_front final : public CGameObject
{
private:
	CMainInventory_front(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMainInventory_front(const CMainInventory_front& rhs);
	virtual ~CMainInventory_front() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual int Tick(_float fTimeDelta)override;
	virtual void Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render() override;

private: /* For.Components */
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	_float4x4				m_ProjMatrix;
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float                  m_pfX, m_pfY;
	_float3                 pos;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CMainInventory_front* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


	bool get_check() { return m_bcheck; }
	bool get_pontcheck() { return m_bpontcheck; }
	bool get_check_bag() { return m_bcheck_bag; }
	void set_check(bool tof) { m_bcheck = tof; }
	void set_check_bag(bool tof) { m_bcheck_bag = tof; }
	void plus_itemcount() { ++item_number; }
	void plus_itemcount2( _uint num) { item_number+= num; }
	void minus_itemcount() { --item_number; }
	void minus_material(_uint minus) { item_number -= minus; }
	void set_itemcount(_uint count) { item_number = count; }
	void set_texnum(ITEMNAME texnumber) { texnum = texnumber; }
	ITEMNAME get_texnum() { return texnum; }
	_uint get_item_number() { return item_number; }
	void Use_item(ITEMNAME item);
	int get_iNum() { return iNum; }
	ITEMID get_itemtype() { return m_itemtype; }
	_float3 get_pos() { return pos; }
	bool get_big() { return big; }
	void set_big(bool tof) { big = tof; }

	bool get_crash() { return crash; }
	void set_small(bool tof) { smaller = tof; }
	void set_crash(bool tof) { crash = tof; }
	void set_bbig(bool tof) { bbig = tof; }

	bool get_bbig() { return bbig; }
	bool get_small() { return smaller; }

	void bigbig(void);

private:
	int* iNumber = nullptr;
	int iNum = 100;
	ITEMNAME texnum = ITEMNAME_END;
	ITEMID m_itemtype = ITEM_END;
	_uint item_number = 0;//�����۰���
	bool m_bcheck = true;
	bool m_bpontcheck = true;
	bool m_bcheck_bag = false;  
	bool big = false;
	bool crash = false;
	bool smaller = false;
	bool bbig = false;
	_uint whatnum = 30;

};

END