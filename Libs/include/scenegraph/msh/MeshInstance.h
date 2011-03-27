#if !defined(__IZANAGI_SCENEGRAPH_MESH_INSTANCE_H__)
#define __IZANAGI_SCENEGRAPH_MESH_INSTANCE_H__

#include "izStd.h"
#include "MSHFormat.h"

class CMeshGroupInstance;

namespace izanagi {
	class IMemoryAllocator;
	class CGraphicsDevice;
	class CMesh;
	class CMeshSet;
	class CPrimitiveSet;
	class CSkeletonInstance;
	class IMshRenderHandler;
	class CMaterial;

	////////////////////////////////////////////
	/**
	*/
	class CMeshSetInstance : public CPlacementNew {
		friend class CMeshGroupInstance;

	private:
		static CMeshSetInstance* CreateMeshSetInstance(
			IZ_UINT8** pBuf,
			CMeshSet* pMeshSet);

		static inline size_t ComputeBytes(CMeshSet* pMeshSet);

	private:
		inline CMeshSetInstance();
		inline ~CMeshSetInstance();

		NO_COPIABLE(CMeshSetInstance);

	private:
		IZ_UINT8* Init(
			IZ_UINT8* pBuf,
			CMeshSet* pMeshSet);

	public:
		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			IMshRenderHandler* pRenderHandler);

		void SetSkeleton(CSkeletonInstance* pSkl);

		void SetMaterial(CMaterial* pMtrl);
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
	*/
	class CMeshInstance : public CObject {
	public:
		static CMeshInstance* CreateMeshInstance(
			IMemoryAllocator* pAllocator,
			CMesh* pMesh);

	private:
		CMeshInstance();
		~CMeshInstance();

		NO_COPIABLE(CMeshInstance);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		IZ_BOOL Render(
			CSkeletonInstance* pSkl,
			IMshRenderHandler* pRenderHandler);

		void ApplySkeleton(CSkeletonInstance* pSkl);

		IZ_UINT GetMeshSetNum() const;

		CMeshSetInstance* GetMeshSet(IZ_UINT idx);

	private:
		IMemoryAllocator* m_pAllocator;

		CMesh* m_pBody;

		IZ_UINT m_nGroupNum;
		CMeshGroupInstance** m_pGroups;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MESH_INSTANCE_H__)
