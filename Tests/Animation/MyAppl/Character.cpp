#include "Character.h"
#include "Common.h"

#include "izIo.h"

CCharacter* CCharacter::sInstance = IZ_NULL;

CCharacter* CCharacter::CreateCharacter(
	izanagi::IMemoryAllocator* allocator,
	izanagi::CGraphicsDevice* device)
{
	IZ_ASSERT(allocator != IZ_NULL);
	IZ_ASSERT(device != IZ_NULL);

	if (sInstance != IZ_NULL) {
		return sInstance;
	}

	IZ_BOOL result = IZ_FALSE;

	void* buf = ALLOC_ZERO(allocator, sizeof(CCharacter));
	VRETURN_NULL(buf);

	sInstance = new(buf) CCharacter;
	{
		sInstance->AddRef();

		sInstance->m_Allocator = allocator;
		SAFE_REPLACE(sInstance->m_Device, device);

		result = sInstance->Init();
	}

	if (!result) {
		SAFE_RELEASE(sInstance);
	}
	return sInstance;
}

CCharacter::CCharacter()
{
	m_Allocator = IZ_NULL;
	m_Device = IZ_NULL;

	m_pMdl = IZ_NULL;
	m_pMsh = IZ_NULL;
	m_pSkl = IZ_NULL;

	m_pTex[0] = IZ_NULL;
	m_pTex[1] = IZ_NULL;

	m_pMtrl[0] = IZ_NULL;
	m_pMtrl[1] = IZ_NULL;
}

CCharacter::~CCharacter()
{
	SAFE_RELEASE(m_pMdl);
	SAFE_RELEASE(m_pMsh);
	SAFE_RELEASE(m_pSkl);
	SAFE_RELEASE(m_pTex[0]);
	SAFE_RELEASE(m_pTex[1]);
	SAFE_RELEASE(m_pMtrl[0]);
	SAFE_RELEASE(m_pMtrl[1]);

	SAFE_RELEASE(m_Device);
}

void CCharacter::InternalRelease()
{
	delete this;
	if (m_Allocator) {
		FREE(m_Allocator, this);
	}

	sInstance = IZ_NULL;
}

IZ_BOOL CCharacter::Init()
{
	izanagi::CFileInputStream input;

	// Mesh
	{
		VRETURN(input.Open("data/00.msh"));

		m_pMsh = izanagi::CMesh::CreateMesh(
					m_Allocator,
					m_Device,
					&input);
		IZ_ASSERT(m_pMsh != IZ_NULL);
		input.Finalize();
	}

	// Skeleton
	{
		VRETURN(input.Open("data/00.skl"));

		m_pSkl = izanagi::CSkeleton::CreateSkeleton(m_Allocator, &input);
		IZ_ASSERT(m_pSkl != IZ_NULL);
		input.Finalize();
	}

	// Material
	{
		VRETURN(input.Open("data/00_0.mtrl"));
		m_pMtrl[0] = izanagi::CMaterial::CreateMaterial(m_Allocator, &input);
		IZ_ASSERT(m_pMtrl[0] != IZ_NULL);
		input.Finalize();

		VRETURN(input.Open("data/00_1.mtrl"));
		m_pMtrl[1] = izanagi::CMaterial::CreateMaterial(m_Allocator, &input);
		IZ_ASSERT(m_pMtrl[1] != IZ_NULL);
		input.Finalize();
	}

	// Model
	m_pMdl = izanagi::CModel::CreateModel(
				m_Allocator,
				m_pMsh,
				m_pSkl,
				IZ_NULL);
	IZ_ASSERT(m_pMdl != IZ_NULL);

	// テクスチャ
	{
		m_pTex[0] = m_Device->CreateTextureFromFile("data/1P_C.dds", izanagi::E_GRAPH_PIXEL_FMT_RGBA8);
		IZ_ASSERT(m_pTex[0] != IZ_NULL);

		m_pTex[1] = m_Device->CreateTextureFromFile("data/Face_C.dds", izanagi::E_GRAPH_PIXEL_FMT_RGBA8);
		IZ_ASSERT(m_pTex[1] != IZ_NULL);
	}

	return IZ_TRUE;
}

void CCharacter::Update(
	IZ_FLOAT time,
	izanagi::IAnimation* animation)
{
	if (animation != IZ_NULL) {
		m_pMdl->ApplyAnimation(time, animation);
	}

	m_pMdl->Update();
}

void CCharacter::SetShader(izanagi::IShader* shader)
{
	IZ_ASSERT(shader != IZ_NULL);

	if (m_pMtrl[0]->GetShader() == IZ_NULL) {
		m_pMtrl[0]->SetName("Material_0");
		m_pMtrl[0]->SetTexture("Face_C.dds", m_pTex[1]);
		m_pMtrl[0]->SetShader(shader);

		m_pMdl->GetMesh()->SetMaterial(0, m_pMtrl[0]);
	}

	if (m_pMtrl[1]->GetShader() == IZ_NULL) {
		m_pMtrl[1]->SetName("Material_1");
		m_pMtrl[1]->SetTexture("1P_C.dds", m_pTex[0]);
		m_pMtrl[1]->SetShader(shader);

		m_pMdl->GetMesh()->SetMaterial(0, m_pMtrl[1]);
	}
}
