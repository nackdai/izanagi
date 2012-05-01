#if !defined(__IZANAGI_SCENEGRAPH_IZ_MODEL_H__)
#define __IZANAGI_SCENEGRAPH_IZ_MODEL_H__

#include "izStd.h"
#include "izGraph.h"

#include "scenegraph/msh/MeshInstance.h"
#include "scenegraph/msh/MshRenderHandler.h"
#include "scenegraph/skl/SkeletonInstance.h"

namespace izanagi {
	class IInputStream;
	class IMeshSet;

	/**
	 * モデルオブジェクトのインターフェース
	 */
	class IModel : public CObject {
	protected:
		IModel() {}
		virtual ~IModel() {}

		NO_COPIABLE(IModel);

	public:
		/**
		 * 所属するメッシュセット数を返す
		 */
		virtual IZ_UINT GetMeshSetNum() = 0;

		/**
		 * 指定されたメッシュセットを返す
		 */
		virtual IMeshSet* GetMeshSet(IZ_UINT idx) = 0;
	};

	/** メッシュ + スケルトンで構成されるシンプルなモデルオブジェクト.
	 */
	class CModel : public IModel {
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
		/** 描画.
		 */
		IZ_BOOL Render();

		/** 適用されたモーションに基づき更新.
		 */
		void Update(const SMatrix* mtxL2W = IZ_NULL);

		/** スケルトン全体にモーション適用.
		 */
		void ApplyAnimation(
			IZ_FLOAT fTime,
			IAnimation* pAnm);

		/** 指定した関節のみにモーション適用.
		 */
		void ApplyAnimationByIdx(
			IZ_UINT nJointIdx,
			IZ_FLOAT fTime,
			IAnimation* pAnm);

		/** 利用可能なLODレベルの最大を取得.
		 */
		IZ_UINT GetMaxLODLevel() const;

		/** LODレベルを設定する.
		 */
		IZ_BOOL SetLODLevel(IZ_UINT level);

	public:
		IZ_UINT GetMeshSetNum();

		IMeshSet* GetMeshSet(IZ_UINT idx);

		CMeshInstance* GetMesh();

		CSkeletonInstance* GetSkeleton();

		void SetRenderHandler(IMshRenderHandler* pRenderHandler);

	protected:
		IMemoryAllocator* m_Allocator;

		CMeshInstance* m_pMesh;
		CSkeletonInstance* m_pSkeleton;

		IMshRenderHandler* m_pRenderHandler;

		IZ_UINT m_nCurLODLevel;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_IZ_MODEL_H__)
