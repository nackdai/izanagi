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
	 * メッシュセットインターフェース
	 *
	 * ジオメトリソーティングの最小単位
	 * メッシュセットとは、マテリアルごとのプリミティブセットの集まり
	 */
	class IMeshSet : public CPlacementNew {
		friend class CGeometrySorter;
		friend class CScene;

	protected:
		IMeshSet();
		virtual ~IMeshSet();

		NO_COPIABLE(IMeshSet);

	protected:
		/**
		 * マテリアル情報取得
		 */
		virtual const S_MSH_MTRL& GetMaterialInfo() = 0;

		/**
		 * 描画に利用するマテリアルを設定
		 */
		void SetMaterial(CMaterial* pMtrl);

		/**
		 * 設定されているマテリアルを取得
		 */
		CMaterial* GetMaterial();

		/**
		 * 描画
		 */
		virtual IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			IMshRenderHandler* pRenderHandler) = 0;

		/**
		 * 描画に利用するスケルトンを設定
		 */
		virtual void SetSkeleton(CSkeletonInstance* pSkl)
		{
			// Nothing is done.
		}

		// TODO
		IZ_BOOL IsTranslucent() const { return IZ_FALSE; }

	private:
		// GeometrySorter用
#if 0
		CStdList<IMeshSet>::Item* GetListItem() { return &m_ListItem; }
#else
		CStdSet<IMeshSet>::Item* GetListItem() { return &m_ListItem; }
	public:
		IZ_BOOL operator>(IMeshSet& rhs)
		{
			return m_fZ > rhs.m_fZ;
		}
	private:
#endif

		// TODO
		void SetZ(IZ_FLOAT z) { m_fZ = z; }
		IZ_FLOAT GetZ() const { return m_fZ; }

	protected:
		CMaterial* m_pMtrl;

#if 0
		CStdList<IMeshSet>::Item m_ListItem;
#else
		CStdSet<IMeshSet>::Item m_ListItem;
#endif

		IZ_FLOAT m_fZ;
	};

	////////////////////////////////////////////

	/**
	 * メッシュセットインスタンス
	 *
	 * メッシュセットとは、マテリアルごとのプリミティブセットの集まり
	 */
	class CMeshSetInstance : public IMeshSet {
		friend class CMeshInstance;
		friend class CMeshGroupInstance;

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

	private:
		// 初期化
		IZ_UINT8* Init(
			IZ_UINT8* pBuf,
			CMeshSet* pMeshSet);

		// マテリアル情報取得
		const S_MSH_MTRL& GetMaterialInfo();

		// 描画
		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			IMshRenderHandler* pRenderHandler);

		// 描画に利用するスケルトンを設定
		void SetSkeleton(CSkeletonInstance* pSkl);

	private:
		CMeshSet* m_pBody;

		IZ_UINT m_nPrimNum;
		CPrimitiveSet** m_pPrims;

		CSkeletonInstance* m_pSkl;
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
		IZ_BOOL SetMaterial(
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
