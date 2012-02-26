#include "scenegraph/msh/MeshSetInstance.h"
#include "scenegraph/msh/MshMeshSet.h"

#include "scenegraph/msh/MshRenderHandler.h"
#include "scenegraph/msh/MshPrimitiveSet.h"
#include "scenegraph/skl/SkeletonInstance.h"
#include "scenegraph/mtrl/izMaterial.h"

using namespace izanagi;

// インスタンス作成
CMeshSetInstance* CMeshSetInstance::CreateMeshSetInstance(
	IZ_UINT8** pBuf,
	CMeshSet* pMeshSet)
{
	IZ_ASSERT(pBuf != IZ_NULL);
	IZ_ASSERT(pMeshSet != IZ_NULL);

	IZ_UINT8*& buf = *pBuf;

	CMeshSetInstance* pInstance = new(buf) CMeshSetInstance;
	buf += sizeof(CMeshSetInstance);

	SAFE_REPLACE(pInstance->m_pBody, pMeshSet);

	return pInstance;
}

// インスタンス作成に必要なバイト数を計算
size_t CMeshSetInstance::ComputeBytes(CMeshSet* pMeshSet)
{
	IZ_ASSERT(pMeshSet != IZ_NULL);

	size_t ret = sizeof(CMeshSetInstance);

	return ret;
}

// コンストラクタ
CMeshSetInstance::CMeshSetInstance()
{
	m_pBody = IZ_NULL;
	m_pSkl = IZ_NULL;
	m_Mtrl = IZ_NULL;
}

// デストラクタ
CMeshSetInstance::~CMeshSetInstance()
{
	SAFE_RELEASE(m_pBody);
	SAFE_RELEASE(m_pSkl);
	SAFE_RELEASE(m_Mtrl);
}

// マテリアル情報取得
const S_MSH_MTRL& CMeshSetInstance::GetMaterialInfo()
{
	IZ_ASSERT(m_pBody != IZ_NULL);
	return m_pBody->GetMtrlInfo();
}

// 描画に利用するマテリアルを設定.
void CMeshSetInstance::SetMaterial(CMaterial* pMtrl)
{
	SAFE_REPLACE(m_Mtrl, pMtrl);
}

// 設定されているマテリアルを取得.
CMaterial* CMeshSetInstance::GetMaterial()
{
	return m_Mtrl;
}

// 描画に利用するスケルトンを設定.
void CMeshSetInstance::SetSkeleton(CSkeletonInstance* pSkl)
{
	SAFE_REPLACE(m_pSkl, pSkl);
}

// 描画
IZ_BOOL CMeshSetInstance::Render(
	CGraphicsDevice* device,
	void* renderHandler)
{
	IZ_ASSERT(m_pBody != IZ_NULL);

	IZ_BOOL ret = m_pBody->Render(
					device,
					m_pSkl,
					(IMshRenderHandler*)renderHandler);
	return ret;
}

// 半透明かどうかを取得.
IZ_BOOL CMeshSetInstance::IsTranslucent() const
{
	IZ_ASSERT(m_Mtrl != IZ_NULL);

	// マテリアル自体が半透明設定されているかどうか
	IZ_BOOL isTranslucent = (m_Mtrl->GetAlpha() == IZ_UINT8_MAX);

	// 半透明テクスチャの有無をチェック
	IZ_BOOL isTransTex = IZ_FALSE;
	{
		IZ_UINT texNum = m_Mtrl->GetTexNum();
		for (IZ_UINT i = 0; i < texNum; i++) {
			isTransTex = (m_Mtrl->GetTexInfoByIdx(i)->type.isTranslucent);
			if (isTransTex) {
				break;
			}
		}
	}

	return (isTranslucent || isTransTex);
}
