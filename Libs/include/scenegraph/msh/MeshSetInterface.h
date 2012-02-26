#if !defined(__IZANAGI_SCENEGRAPH_MESH_SET_INTERFACE_H__)
#define __IZANAGI_SCENEGRAPH_MESH_SET_INTERFACE_H__

#include "izStd.h"
#include "MSHFormat.h"
#include "scenegraph/rendergraph/RenderElement.h"

namespace izanagi {
	class CMaterial;
	class CSkeletonInstance;

	/** メッシュセットインターフェース.
	 *
	 * ジオメトリソーティングの最小単位
	 * メッシュセットとは、マテリアルごとのプリミティブセットの集まり
	 */
	class IMeshSet : public IRenderElement {
	protected:
		IMeshSet() {}
		virtual ~IMeshSet() {}

		NO_COPIABLE(IMeshSet);

		virtual void InternalRelease() { delete this; }

	protected:
		/** マテリアル情報取得.
		 */
		virtual const S_MSH_MTRL& GetMaterialInfo() = 0;

		/** 描画に利用するマテリアルを設定.
		 */
		virtual void SetMaterial(CMaterial* pMtrl) = 0;

		/** 描画に利用するスケルトンを設定.
		 */
		virtual void SetSkeleton(CSkeletonInstance* pSkl) = 0;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MESH_SET_INTERFACE_H__)
