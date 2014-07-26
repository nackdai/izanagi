#if !defined(__IZ_SAMPLE_SAMPLE_MODEL_RENDER_HANDLER_H__)
#define __IZ_SAMPLE_SAMPLE_MODEL_RENDER_HANDLER_H__

#include <izDefs.h>
#include <izStd.h>
#include <izGraph.h>
#include <izSceneGraph.h>

namespace izanagi {
namespace sample {
    // ジョイントマトリクスを描画時に設定するためのハンドラ
    class CSampleMdlRenderHandler : public izanagi::IMshRenderHandler {
        // NOTE
        // IMshRenderHandlerが持つインスタンス作成用メソッドに対して
        // コンストラクタを見せるために必要
        friend class izanagi::IMshRenderHandler;

    private:
        CSampleMdlRenderHandler()
        {
            m_pShader = IZ_NULL;
        }

        ~CSampleMdlRenderHandler()
        {
            SAFE_RELEASE(m_pShader);
        }

    public:
        virtual void BeginRenderMesh();
        virtual void EndRenderMesh();

        virtual void SetJointMatrix(
            IZ_UINT nIdx,
            const izanagi::math::SMatrix44& mtx);

        virtual void CommitChanges(izanagi::graph::CGraphicsDevice* device);

    public:
        void SetShader(izanagi::shader::IShader* pShader)
        {
            SAFE_REPLACE(m_pShader, pShader);
        }

        izanagi::shader::IShader* GetShader() { return m_pShader; }

    private:
        izanagi::shader::IShader* m_pShader;

        IZ_UINT m_nCnt;
        izanagi::math::SMatrix44 m_Mtx[48];

        izanagi::shader::IZ_SHADER_HANDLE m_Handle;
    };
}
}

#endif    // #if !defined(__IZ_SAMPLE_SAMPLE_MODEL_RENDER_HANDLER_H__)
