#if !defined(__MESH_MANAGER_H__)
#define __MESH_MANAGER_H__

#include <d3dx9.h>

class CMeshManager {
protected:
	static CMeshManager s_cInstance;

public:
	static CMeshManager& GetInstance() { return s_cInstance; }

public:
	enum {
		MESH_SPHERE = 0,
		MESH_TEAPOT,
		MESH_BOX,

		MESH_NUM,
	};

protected:
	CMeshManager();
	~CMeshManager();

public:
	BOOL Create(LPDIRECT3DDEVICE9 pDevice);
	
	void Release();

	void Render(
		UINT type,
		D3DCOLOR color = 0xffffffff);

public:
	BOOL CreateSphere(
		LPDIRECT3DDEVICE9 pDevice,
		float fRadius,
		int nSlice, int nStack);
	BOOL CreateTeapot(LPDIRECT3DDEVICE9 pDevice);
	BOOL CreateBox(
		LPDIRECT3DDEVICE9 pDevice,
		float fWidth, float fHeight, float fDepth);

protected:
	LPD3DXMESH m_pMesh[MESH_NUM];
};

#endif	// #if !defined(__MESH_MANAGER_H__)