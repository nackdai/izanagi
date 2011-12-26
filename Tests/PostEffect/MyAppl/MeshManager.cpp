#include "MeshManager.h"

#include "izDefs.h"

CMeshManager CMeshManager::s_cInstance;

CMeshManager::CMeshManager()
{
	ZeroMemory(m_pMesh, sizeof(m_pMesh));
}

CMeshManager::~CMeshManager()
{
}

BOOL CMeshManager::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (! CreateSphere(
			pDevice,
			3.0f,
			50, 50))
	{
		Release();
		return FALSE;
	}

	if (! CreateTeapot(pDevice)) {
		Release();
		return FALSE;
	}

	if (! CreateBox(
			pDevice,
			1.0f, 1.0f, 1.0f))
	{
		Release();
		return FALSE;
	}

	return TRUE;
}

BOOL CMeshManager::CreateSphere(
	LPDIRECT3DDEVICE9 pDevice,
	float fRadius,
	int nSlice, int nStack)
{
	HRESULT hr = D3DXCreateSphere(
					pDevice,
					fRadius,
					nSlice, nStack,
					&m_pMesh[MESH_SPHERE],
					NULL);
	
	BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	return ret;
}

BOOL CMeshManager::CreateTeapot(LPDIRECT3DDEVICE9 pDevice)
{
	HRESULT hr = D3DXCreateTeapot(
					pDevice,
					&m_pMesh[MESH_TEAPOT],
					NULL);
	
	BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	return ret;
}

BOOL CMeshManager::CreateBox(
	LPDIRECT3DDEVICE9 pDevice,
	float fWidth, float fHeight, float fDepth)
{
	HRESULT hr = D3DXCreateBox(
					pDevice,
					fWidth, fHeight, fDepth,
					&m_pMesh[MESH_BOX],
					NULL);

	BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	return ret;
}

void CMeshManager::Release()
{
	for (int i = 0; i < MESH_NUM; i++) {
		SAFE_RELEASE(m_pMesh[i]);
	}
}

void CMeshManager::Render(
	UINT type,
	D3DCOLOR color/*= 0xffffffff*/)
{
	IZ_ASSERT(type < MESH_NUM);
	IZ_ASSERT(m_pMesh[type] != NULL);

	D3DMATERIAL9 sMaterial;
	ZeroMemory(&sMaterial, sizeof(D3DMATERIAL9));

	sMaterial.Diffuse.a = 1.0f;

	//sMaterial.Emissive.a = (float)((color >> 24) & 0xff) / 255.0f;
	sMaterial.Emissive.a = 1.0f;
	sMaterial.Emissive.r = (float)((color >> 16) & 0xff) / 255.0f;
	sMaterial.Emissive.g = (float)((color >> 8) & 0xff) / 255.0f;
	sMaterial.Emissive.b = (float)(color & 0xff) / 255.0f;

	LPDIRECT3DDEVICE9 pDevice = NULL;
	m_pMesh[type]->GetDevice(&pDevice);
	IZ_ASSERT(pDevice != NULL);

	pDevice->SetMaterial(&sMaterial);
	SAFE_RELEASE(pDevice);

	m_pMesh[type]->DrawSubset(0);
}
