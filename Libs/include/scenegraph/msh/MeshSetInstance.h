#if !defined(__IZANAGI_SCENEGRAPH_MESH_SET_INSTANCE_H__)
#define __IZANAGI_SCENEGRAPH_MESH_SET_INSTANCE_H__

#include "izStd.h"
#include "MeshSetInterface.h"

namespace izanagi {
    class graph::CGraphicsDevice;
    class CMeshSet;
    class CSkeletonInstance;
    class CMaterial;

    /** メッシュセットインスタンス.
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
        static size_t ComputeBytes(CMeshSet* pMeshSet);

    private:
        CMeshSetInstance();
        virtual ~CMeshSetInstance();        

    private:
        // マテリアル情報取得
        virtual const S_MSH_MTRL& GetMaterialInfo();

        // 描画に利用するマテリアルを設定.
        virtual void SetMaterial(CMaterial* pMtrl);

        // 設定されているマテリアルを取得.
        virtual CMaterial* GetMaterial();

        // 描画に利用するスケルトンを設定.
        virtual void SetSkeleton(CSkeletonInstance* pSkl);

        // 描画
        virtual IZ_BOOL Render(
            graph::CGraphicsDevice* device,
            void* renderHandler);

        // 半透明かどうかを取得.
        virtual IZ_BOOL IsTranslucent() const;

    private:
        CMeshSet* m_pBody;
        CSkeletonInstance* m_pSkl;

        CMaterial* m_Mtrl;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_MESH_SET_INSTANCE_H__)
