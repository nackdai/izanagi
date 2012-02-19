#if !defined(__IZANAGI_SCENEGRAPH_MSH_MESH_SET_H__)
#define __IZANAGI_SCENEGRAPH_MSH_MESH_SET_H__

#include "MSHFormat.h"

namespace izanagi {
	class CGraphicsDevice;
	class CVertexDeclaration;
	class IInputStream;
	class CPrimitiveSet;
	class CSkeletonInstance;
	class IMshRenderHandler;

	/** メッシュセット.
	 *
	 * マテリアルごとのプリミティブセットの集まり
	 */
	class CMeshSet : public CObject {
		friend class CMesh;
		friend class CMeshGroup;
		friend class CMeshSetInstance;

	private:
		CMeshSet();
		~CMeshSet();

		NO_COPIABLE(CMeshSet);

		void InternalRelease() { delete this; }

	private:
		/** 描画.
		 */
		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			CSkeletonInstance* pSkeleton,
			IMshRenderHandler* pRenderHandler);

		IZ_UINT8* Read(
			IZ_UINT8* pBuf,
			CGraphicsDevice* pDevice,
			IInputStream* pIn);

		CPrimitiveSet* GetPrimSet(IZ_UINT idx);

		inline void GetCenter(SVector& vPos);

		/** マテリアル情報の取得.
		 */
		const S_MSH_MTRL& GetMtrlInfo() const { return m_Info.mtrl; }

		/** プリミティブセット総数を取得.
		 */
		IZ_UINT GetPrimSetNum() const { return m_Info.numSubset; }

		/** 描画に利用する頂点宣言を取得.
		 */
		CVertexDeclaration* GetVD() { return m_VD; }

	private:
		S_MSH_MESH_SET m_Info;

		CVertexDeclaration* m_VD;
		
		CPrimitiveSet** m_pPrimSet;
	};

	// inline *************************************

	void CMeshSet::GetCenter(SVector& vPos)
	{
		vPos.x = m_Info.center[0];
		vPos.y = m_Info.center[1];
		vPos.z = m_Info.center[2];
		vPos.w = 1.0f;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MDL_MESH_H__)
