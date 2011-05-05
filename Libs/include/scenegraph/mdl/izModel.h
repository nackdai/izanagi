#if !defined(__IZANAGI_SCENEGRAPH_IZ_MODEL_H__)
#define __IZANAGI_SCENEGRAPH_IZ_MODEL_H__

#include "izStd.h"
#include "izGraph.h"

#include "scenegraph/msh/MeshInstance.h"
#include "scenegraph/msh/MshRenderHandler.h"
#include "scenegraph/skl/SkeletonInstance.h"

namespace izanagi {
	class IInputStream;

	/**
	 */
	class CModel : public CObject {
	public:
		// Create instance.
		static CModel* CreateModel(
			IMemoryAllocator* pAllocator,
			CMeshInstance* pMesh,
			CSkeletonInstance* pSkl,
			IMshRenderHandler* pRenderHandler);

		static CModel* CreateModel(
			IMemoryAllocator* pAllocator,
			CMesh* pMesh,
			CSkeleton* pSkl,
			IMshRenderHandler* pRenderHandler);

		static CModel* CreateModel(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IInputStream* pIn,
			IMshRenderHandler* pRenderHandler);

	protected:
		CModel();
		~CModel();

		NO_COPIABLE(CModel);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		/**
		 * 描画
		 */
		IZ_BOOL Render();

		/**
		 * 適用されたモーションに基づき更新
		 */
		void Update();

		/**
		 * スケルトン全体にモーション適用
		 */
		void ApplyAnimation(
			IZ_FLOAT fTime,
			CAnimation* pAnm);

		/**
		 * 指定した関節のみにモーション適用
		 */
		void ApplyAnimationByIdx(
			IZ_UINT nJointIdx,
			IZ_FLOAT fTime,
			CAnimation* pAnm);

		/**
		 * 指定した関節のみにモーション適用
		 */
		void ApplyAnimationByName(
			IZ_PCSTR pszJointName,
			IZ_FLOAT fTime,
			CAnimation* pAnm);

		/**
		 * 指定した関節のみにモーション適用
		 */
		void ApplyAnimationByKey(
			IZ_UINT nJointKey,
			IZ_FLOAT fTime,
			CAnimation* pAnm);

		/**
		 * 利用可能なLODレベルの最大を取得
		 */
		IZ_UINT GetMaxLODLevel() const;

		/**
		 * LODレベルを設定する
		 */
		IZ_BOOL SetLODLevel(IZ_UINT level);

	public:
		IZ_UINT GetMeshSetNum()
		{
			IZ_ASSERT(m_pMesh != IZ_NULL);
			return m_pMesh->GetMeshSetNum(m_nCurLODLevel);
		}

		CMeshSetInstance* GetMeshSet(IZ_UINT idx)
		{
			IZ_ASSERT(m_pMesh != IZ_NULL);
			return m_pMesh->GetMeshSet(m_nCurLODLevel, idx);
		}

		CMeshInstance* GetMesh() { return m_pMesh; }
		CSkeletonInstance* GetSkeleton() { return m_pSkeleton; }

		void SetRenderHandler(IMshRenderHandler* pRenderHandler) { SAFE_REPLACE(m_pRenderHandler, pRenderHandler); }

	protected:
		IMemoryAllocator* m_pAllocator;

		CMeshInstance* m_pMesh;
		CSkeletonInstance* m_pSkeleton;

		IMshRenderHandler* m_pRenderHandler;

		IZ_UINT m_nCurLODLevel;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_IZ_MODEL_H__)
