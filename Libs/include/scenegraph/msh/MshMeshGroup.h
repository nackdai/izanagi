#if !defined(__IZANAGI_SCENEGRAPH_MESH_GROUP_H__)
#define __IZANAGI_SCENEGRAPH_MESH_GROUP_H__

#include "MSHFormat.h"

namespace izanagi {
	class graph::CGraphicsDevice;
	class graph::CVertexBuffer;
	class IInputStream;
	class CMeshSet;
	class IMshRenderHandler;
	class CSkeletonInstance;

	/** メッシュグループ.
	 *
	 * LODのレベルごとのメッシュセットの集まり
	 */
	class CMeshGroup : public CObject {
		friend class CMesh;

	private:
		CMeshGroup();
		~CMeshGroup();

		NO_COPIABLE(CMeshGroup);

		void InternalRelease() { delete this; }

	public:
		/** メッシュセットの総数を取得.
		 */
		IZ_UINT GetMeshSetNum() { return m_Info.numMeshSet; }

		/** メッシュセットを取得.
		 */
		CMeshSet* GetMeshSet(IZ_UINT idx);

		/** メッシュグループ全体を描画.
		 */
		IZ_BOOL Render(
			graph::CGraphicsDevice* pDevice,
			CSkeletonInstance* pSkl,
			IMshRenderHandler* pRenderHandler);

	private:
		IZ_UINT8* Read(
			IZ_UINT8* pBuf,
			graph::CGraphicsDevice* pDevice,
			IInputStream* pIn);

		IZ_BOOL ReadVertices(
			graph::CGraphicsDevice* pDevice,
			IInputStream* pIn);

	private:
		S_MSH_MESH_GROUP m_Info;

		graph::CVertexBuffer** m_pVB;
		CMeshSet** m_pMeshSet;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MESH_GROUP_H__)
