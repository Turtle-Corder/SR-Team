#pragma once

#ifndef __BUNDLE_TERRAIN_H__
#define __BUNDLE_TERRAIN_H__

USING(Engine)
BEGIN(Client)


class CCubeTerrain;
class CTerrainBundle final : public CGameObject
{
private:
	explicit CTerrainBundle(LPDIRECT3DDEVICE9 _pDevice);
	explicit CTerrainBundle(const CTerrainBundle& _rOther);
	virtual ~CTerrainBundle() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;


	//Get 높이 길이 인터벌 타일인포
	//Set
private:
	HRESULT Add_Component();


public:
	static CTerrainBundle* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;


private:
	//TerrainCube 인포
	vector<TERRAININFO> m_TerrianList[15];

};

END

#endif

