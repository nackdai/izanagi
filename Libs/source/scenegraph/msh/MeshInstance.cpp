#include "scenegraph/msh/MeshInstance.h"
#include "scenegraph/msh/izMesh.h"
#include "scenegraph/msh/MshMeshSet.h"
#include "scenegraph/msh/MshRenderHandler.h"
#include "scenegraph/msh/MshMeshGroup.h"
#include "scenegraph/msh/MshPrimitiveSet.h"
#include "scenegraph/msh/MeshSetInstance.h"

#include "scenegraph/skl/SkeletonInstance.h"
#include "scenegraph/mtrl/izMaterial.h"


using namespace izanagi;

///////////////////////////////////////////////////

namespace izanagi {
	// メッシュグループインスタンス
	// メッシュグループとは、LODのレベルごとのメッシュセットの集まり
	class CMeshGroupInstance : public CPlacementNew {
	public:
		// インスタンス作成
		static CMeshGroupInstance* CreateMeshGroupInstances(
			IZ_UINT8** pBuf,
			CMeshGroup* pGroup);

		// インスタンス作成に必要なバイト数を計算
		// CMesh の持つメッシュグループ全てについて計算する
		static inline size_t ComputeBytes(CMesh* pMesh);

	public:
		inline CMeshGroupInstance();
		inline ~CMeshGroupInstance();

		NO_COPIABLE(CMeshGroupInstance);

	private:
		// 初期化
		IZ_UINT8* Init(
			IZ_UINT8* pBuf,
			CMeshGroup* pGroup);

	public:
		IZ_UINT GetMeshSetNum() { return m_nSetNum; }

		// メッシュセットインスタンスを取得
		inline CMeshSetInstance* GetMeshSet(IZ_UINT idx);

		// 描画
		inline IZ_BOOL Render(
			graph::CGraphicsDevice* pDevice,
			IMshRenderHandler* pRenderHandler);

		// スケルトンをセットする
		inline void SetSkeleton(CSkeletonInstance* pSkl);

		IZ_UINT GetMeshSetNum() const { return m_nSetNum; }

	private:
		CMeshGroup* m_pBody;

		IZ_UINT m_nSetNum;
		CMeshSetInstance** m_pMeshSet;
	};

	// インスタンス作成
	CMeshGroupInstance* CMeshGroupInstance::CreateMeshGroupInstances(
		IZ_UINT8** pBuf,
		CMeshGroup* pGroup)
	{
		IZ_ASSERT(pBuf != IZ_NULL);
		IZ_ASSERT(pGroup != IZ_NULL);

		IZ_UINT8*& buf = *pBuf;

		CMeshGroupInstance* pInstance = new(buf) CMeshGroupInstance;
		buf += sizeof(CMeshGroupInstance);
		
		buf = pInstance->Init(buf, pGroup);
		IZ_BOOL result = (buf != IZ_NULL);

		if (!result) {
			SAFE_DELETE(pInstance);
		}
		return pInstance;
	}

	// インスタンス作成に必要なバイト数を計算
	// CMesh の持つメッシュグループ全てについて計算する
	size_t CMeshGroupInstance::ComputeBytes(CMesh* pMesh)
	{
		IZ_ASSERT(pMesh != IZ_NULL);

		size_t ret = 0;

		// メッシュグループ数
		IZ_UINT nGroupNum = pMesh->GetMeshGroupNum();

		for (IZ_UINT i = 0; i < nGroupNum; i++) {
			ret += sizeof(CMeshGroupInstance*);
			ret += sizeof(CMeshGroupInstance);

			CMeshGroup* pGroup = pMesh->GetMeshGroup(i);

			// メッシュグループの持つメッシュセット数
			IZ_UINT nMeshSetNum = pGroup->GetMeshSetNum();

			for (IZ_UINT n = 0; n < nMeshSetNum; n++) {
				ret += sizeof(CMeshSetInstance*);
				ret += CMeshSetInstance::ComputeBytes(pGroup->GetMeshSet(n));
			}
		}

		return ret;
	}

	// コンストラクタ
	CMeshGroupInstance::CMeshGroupInstance()
	{
		m_pBody = IZ_NULL;

		m_nSetNum = 0;
		m_pMeshSet = IZ_NULL;
	}

	// デストラクタ
	CMeshGroupInstance::~CMeshGroupInstance()
	{
		SAFE_RELEASE(m_pBody);

		for (IZ_UINT i = 0; i < m_nSetNum; i++) {
			delete m_pMeshSet[i];
		}
	}

	// 初期化
	IZ_UINT8* CMeshGroupInstance::Init(
		IZ_UINT8* pBuf,
		CMeshGroup* pGroup)
	{
		IZ_ASSERT(pBuf != IZ_NULL);
		IZ_ASSERT(pGroup != IZ_NULL);

		// メッシュグループ本体
		SAFE_REPLACE(m_pBody, pGroup);

		m_nSetNum = m_pBody->GetMeshSetNum();
		if (m_nSetNum == 0) {
			// メッシュセットが存在しないので、ここまで
			return pBuf;
		}

		m_pMeshSet = reinterpret_cast<CMeshSetInstance**>(pBuf);
		pBuf += sizeof(CMeshSetInstance*) * m_nSetNum;

		for (IZ_UINT i = 0; i < m_nSetNum; i++) {
			// メッシュセットインスタンス作成
			m_pMeshSet[i] = CMeshSetInstance::CreateMeshSetInstance(
								&pBuf,
								pGroup->GetMeshSet(i));
			if (m_pMeshSet[i] == IZ_NULL) {
				pBuf = IZ_NULL;
				break;
			}
		}
		
		return pBuf;
	}

	// メッシュセットインスタンスを取得
	CMeshSetInstance* CMeshGroupInstance::GetMeshSet(IZ_UINT idx)
	{
		IZ_ASSERT(idx < m_nSetNum);
		return m_pMeshSet[idx];
	}

	// 描画
	IZ_BOOL CMeshGroupInstance::Render(
		graph::CGraphicsDevice* pDevice,
		IMshRenderHandler* pRenderHandler)
	{
		for (IZ_UINT i = 0; i < m_nSetNum; ++i) {
			VRETURN(m_pMeshSet[i]->Render(pDevice, pRenderHandler));
		}

		return IZ_TRUE;
	}

	// スケルトンをセットする
	void CMeshGroupInstance::SetSkeleton(CSkeletonInstance* pSkl)
	{
		for (IZ_UINT i = 0; i < m_nSetNum; ++i) {
			m_pMeshSet[i]->SetSkeleton(pSkl);
		}
	}
}	// namespace izanagi

// インスタンス作成
CMeshInstance* CMeshInstance::CreateMeshInstance(
	IMemoryAllocator* pAllocator,
	CMesh* pMesh)
{
	// Compute allocated size.
	size_t nSize = sizeof(CMeshInstance);
	nSize += CMeshGroupInstance::ComputeBytes(pMesh);

	// Allocate buffer.
	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	// Remenber buffer's top.
	IZ_UINT8* pTop = pBuf;

	IZ_BOOL result = IZ_TRUE;

	CMeshInstance* pInstance = new(pBuf) CMeshInstance;
	pBuf += sizeof(CMeshInstance);
	{
		pInstance->AddRef();
		pInstance->m_Allocator = pAllocator;

		SAFE_REPLACE(pInstance->m_pBody, pMesh);

		// メッシュグループ数（LOD数）
		IZ_UINT nGroupNum = pMesh->GetMeshGroupNum();

		// メッシュグループバッファ
		pInstance->m_pGroups = reinterpret_cast<CMeshGroupInstance**>(pBuf);
		pBuf += sizeof(CMeshGroupInstance*) * nGroupNum;

		pInstance->m_nGroupNum = nGroupNum;

		// メッシュグループの本データからインスタンスを作成
		for (IZ_UINT i = 0; i < nGroupNum; i++) {
			CMeshGroup* pGroup = pMesh->GetMeshGroup(i);

			pInstance->m_pGroups[i] = CMeshGroupInstance::CreateMeshGroupInstances(&pBuf, pGroup);

			result = (pInstance->m_pGroups[i] != IZ_NULL);
			IZ_ASSERT(result);
			if (!result) {
				break;
			}
		}
	}

	// Check distance of buffer.
	//IZ_ASSERT(CStdUtil::GetPtrDistance(pTop, pBuf) == nSize);

	if (!result) {
		SAFE_RELEASE(pInstance);
	}

	return pInstance;
}

// コンストラクタ
CMeshInstance::CMeshInstance()
{
	m_Allocator = IZ_NULL;
	m_pBody = IZ_NULL;

	m_nGroupNum = 0;
	m_pGroups = IZ_NULL;
}

// デストラクタ
CMeshInstance::~CMeshInstance()
{
	SAFE_RELEASE(m_pBody);

	for (IZ_UINT i = 0; i < m_nGroupNum; i++) {
		SAFE_DELETE(m_pGroups[i]);
	}
}

// 描画
IZ_BOOL CMeshInstance::Render(
	IZ_UINT level,
	CSkeletonInstance* pSkl,
	IMshRenderHandler* pRenderHandler)
{
	IZ_ASSERT(m_pBody != IZ_NULL);
	IZ_ASSERT(m_pGroups != IZ_NULL);
	IZ_ASSERT(level < m_nGroupNum);

	graph::CGraphicsDevice* pDevice = m_pBody->GetGraphicsDevice();
	IZ_ASSERT(pDevice != IZ_NULL);

	// スケルトン設定
	m_pGroups[level]->SetSkeleton(pSkl);

	// 描画
	IZ_BOOL ret = m_pGroups[level]->Render(pDevice, pRenderHandler);

	return ret;
}

// スケルトンデータの適用
void CMeshInstance::ApplySkeleton(
	IZ_UINT level,
	CSkeletonInstance* pSkl)
{
	IZ_ASSERT(m_pGroups != IZ_NULL);
	IZ_ASSERT(level < m_nGroupNum);

	m_pGroups[level]->SetSkeleton(pSkl);
}

// 描画に利用するマテリアルを設定
IZ_BOOL CMeshInstance::SetMaterial(
	IZ_UINT level,
	CMaterial* pMtrl)
{
	IZ_ASSERT(level < m_nGroupNum);
	IZ_ASSERT(m_pGroups != IZ_NULL);

	IZ_BOOL ret = IZ_FALSE;

	IZ_UINT nMtrlKey = pMtrl->GetKey();

	IZ_UINT nMeshSetNum = m_pGroups[level]->GetMeshSetNum();

	// メッシュセットにマテリアルを設定する
	for (IZ_UINT i = 0; i < nMeshSetNum; i++) {
		CMeshSetInstance* pMshSet = m_pGroups[level]->GetMeshSet(i);
		if (pMshSet->GetMaterialInfo().nameKey == nMtrlKey) {
			pMshSet->SetMaterial(pMtrl);

			// １個でも登録できたので
			ret = IZ_TRUE;
		}
	}

	return ret;
}

IZ_UINT CMeshInstance::GetMeshSetNum(IZ_UINT level) const
{
	IZ_ASSERT(m_pGroups != IZ_NULL);
	IZ_ASSERT(level < m_nGroupNum);

	IZ_UINT ret = m_pGroups[level]->GetMeshSetNum();
	return ret;
}

CMeshSetInstance* CMeshInstance::GetMeshSet(IZ_UINT level, IZ_UINT idx)
{
	IZ_ASSERT(m_pGroups != IZ_NULL);
	IZ_ASSERT(idx < GetMeshSetNum(level));

	CMeshSetInstance* ret = m_pGroups[level]->GetMeshSet(idx);
	return ret;
}
