//#pragma once
//#include "Particle.h"
//
//BEGIN(Client)
//
//class CPSystem : public CGameObject
//{
//public:
//	struct Particle
//	{
//		D3DXVECTOR3 _position; //��ƼŬ�� ��ġ 
//		D3DCOLOR    _color; //��ƼŬ�� �÷�
//		float		_size;
//		static const DWORD FVF;
//	};
//
//	//��ƼŬ�� �Ӽ��� ��� ����ü
//	typedef struct Attribute
//	{
//		D3DXVECTOR3 _position; //���� �����̽� ���� ��ƼŬ ��ġ
//		D3DXVECTOR3 _velocity; //��ƼŬ�� �ӵ�
//		D3DXVECTOR3 _acceleration; //��ƼŬ�� ����
//		float _lifeTime; //��ƼŬ�� �Ҹ��� ������ �����Ǵ� �ð�
//		float _age;  //��ƼŬ�� ����
//		bool _isAlive; //��ƼŬ�� ���� ����
//	}STATEDESC;
//
//private:
//	CPSystem(LPDIRECT3DDEVICE9 pGraphic_Device);
//	CPSystem(const CParticle& rhs);
//	virtual ~CPSystem() = default;
//
//public:
//	virtual HRESULT Initialize_Prototype();
//	virtual HRESULT Initialize(void* pArg);
//	virtual int Tick(_float fTimeDelta);
//	virtual void Late_Tick(_float fTimeDelta);
//	virtual HRESULT Render();
//	HRESULT SetUp_RenderState();
//	HRESULT Release_RenderState();
//
//	virtual void reset();
//	virtual void resetParticle(Attribute* attribute) = 0;
//	virtual void addParticle();
//
//	bool isEmpty() { return _particles.empty(); }
//	bool isDead();
//
//protected:
//	virtual void removeDeadParticles();
//
//protected:
//	IDirect3DDevice9* _device;
//	D3DXVECTOR3		  _origin; //�ý����� ��õ �ý��� ������ ��ƼŬ�� ���۵Ǵ� ��
//	BoundingBox  _boundingBox; //��� ����, ������ ����� ��ƼŬ���� �Ҹ�ȴ�
//	float _emitRate; //�ʴ� ��ƼŬ ����
//	float _size;	 //��ƼŬ�� ũ��
//	CTexture*			m_pTextureCom = nullptr;
//	PDIRECT3DVERTEXBUFFER9		m_pVBuffer = nullptr;
//	_uint			m_iStride = 0;
//	_uint			m_iNumVertices = 0;
//	_uint			m_iNumPrimitive = 0;
//	_ulong			m_dwFVF = 0;
//
//	list<Attribute> _particles; //�Ӽ��� ����Ʈ
//	int				_maxParticles; //�־��� �ð� ���� �ý����� ���� �� �ִ� �ִ� ��ƼŬ�� ��.
//
//	DWORD  _vbSize; // ���ؽ� ���� ������ �� �ִ� ��ƼŬ�� ��
//	DWORD  _vbOffset;
//	DWORD  _vbBatchSize;
//};
//
//END