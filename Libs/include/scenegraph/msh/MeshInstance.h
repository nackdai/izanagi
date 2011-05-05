#if !defined(__IZANAGI_SCENEGRAPH_MESH_INSTANCE_H__)
#define __IZANAGI_SCENEGRAPH_MESH_INSTANCE_H__

#include "izStd.h"
#include "MSHFormat.h"

namespace izanagi {
	class IMemoryAllocator;
	class CGraphicsDevice;
	class CMesh;
	class CMeshSet;
	class CPrimitiveSet;
	class CSkeletonInstance;
	class IMshRenderHandler;
	class CMaterial;
	class CMeshGroupInstance;

	////////////////////////////////////////////

	/**
	 * メッシュセットインスタンス
	 *
	 * メッシュセットとは、マテリアルごとのプリミティブセットの集まり
	 */
	class CMeshSetInstance : public CPlacementNew {
		friend class CMeshGroupInstance;
		friend class CGeometrySorter;
		friend class CScene;

	private:
		// インスタンス作成
		static CMeshSetInstance* CreateMeshSetInstance(
			IZ_UINT8** pBuf,
			CMeshSet* pMeshSet);

		// インスタンス作成に必要なバイト数を計算
		static inline size_t ComputeBytes(CMeshSet* pMeshSet);

	private:
		inline CMeshSetInstance();
		inline ~CMeshSetInstance();

		NO_COPIABLE(CMeshSetInstance);

	private:
		// 初期化
		IZ_UINT8* Init(
			IZ_UINT8* pBuf,
			CMeshSet* pMeshSet);

		// 描画
		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			IMshRenderHandler* pRenderHandler);

		// 描画に利用するスケルトンを設定
		void SetSkeleton(CSkeletonInstance* pSkl);

		// 描画に利用するマテリアルを設定
		void SetMaterial(CMaterial* pMtrl);

		// 設定されているマテリアルを取得
		CMaterial* GetMaterial();

		const S_MSH_MTRL& GetMaterialInfo();

		CStdList<CMeshSetInstance>::Item* GetListItem() { return &m_ListItem; }

		// TODO
		IZ_BOOL IsTranslucent() const { return IZ_FALSE; }

		// TODO
		void SetZ(IZ_FLOAT z) { m_fZ = z; }
		IZ_FLOAT GetZ() const { return m_fZ; }

	private:
		CMeshSet* m_pBody;

		IZ_UINT m_nPrimNum;
		CPrimitiveSet** m_pPrims;

		CSkeletonInstance* m_pSkl;

		CMaterial* m_pMtrl;

		IZ_FLOAT m_fZ;

		CStdList<CMeshSetInstance>::Item m_ListItem;
	};

	////////////////////////////////////////////

	/**
	 * メッシュデータインスタンス
	 */
	class CMeshInstance : public CObject {
	public:
		/**
		 * インスタンス作成
		 */
		static CMeshInstance* CreateMeshInstance(
			IMemoryAllocator* pAllocator,
			CMesh* pMesh);

	private:
		CMeshInstance();
		~CMeshInstance();

		NO_COPIABLE(CMeshInstance);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		/**
		 * 描画
		 */
		IZ_BOOL Render(
			IZ_UINT level,
			CSkeletonInstance* pSkl,
			IMshRenderHandler* pRenderHandler);

		/**
		 * スケルトンデータの適用
		 */
		void ApplySkeleton(
			IZ_UINT level,
			CSkeletonInstance* pSkl);

		/**
		 * 描画に利用するマテリアルを設定
		 */
		void SetMaterial(
			IZ_UINT level,
			CMaterial* pMtrl);

		IZ_UINT GetMeshGroupNum() const { return m_nGroupNum; }

		IZ_UINT GetMeshSetNum(IZ_UINT level) const;

		CMeshSetInstance* GetMeshSet(IZ_UINT level, IZ_UINT idx);

	private:
		IMemoryAllocator* m_pAllocator;

		CMesh* m_pBody;

		IZ_UINT m_nGroupNum;
		CMeshGroupInstance** m_pGroups;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MESH_INSTANCE_H__)
