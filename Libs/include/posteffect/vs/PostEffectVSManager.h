#if !defined(__IZANAGI_POSTEFFECT_VS_MANAGER_H__)
#define __IZANAGI_POSTEFFECT_VS_MANAGER_H__

#include "izStd.h"
#include "izIO.h"
#include "../PostEffectDefs.h"
#include "PostEffectVS.h"

namespace izanagi {
    /**
    * 頂点シェーダマネージャ
    */
    class CPostEffectVSManager {
        friend class CPostEffectSystem;
        friend class CPostEffectShader;
        friend class CPostEffectVS;

    private:
        inline CPostEffectVSManager();
        inline ~CPostEffectVSManager();

        NO_COPIABLE(CPostEffectVSManager);

    private:
        // 頂点シェーダ作成
        IZ_BOOL CreateVS(
            IZ_UINT nVsProgramNum,
            IMemoryAllocator* pAllocator,
            graph::CGraphicsDevice* pDevice,
            IZ_UINT8* pReadBuffer,
            IInputStream* in);

        // 頂点シェーダ作成
        IZ_BOOL CreateVS(
            E_POSTEFFECT_VTX_SHADER type,
            IMemoryAllocator* pAllocator,
            graph::CGraphicsDevice* pDevice,
            IZ_UINT8* pProgram);

        // 解放
        void Release();

        // レンダラ初期化
        IZ_BOOL InitRenderer(graph::CGraphicsDevice* pDevice);

        // 頂点バッファにデータセット
        IZ_BOOL SetVertexBuffer();

    private:
        // 頂点シェーダ取得
        inline CPostEffectVS* GetVS(E_POSTEFFECT_VTX_SHADER type);

    private:
        enum {
            VERTEX_NUM = 4,     // 頂点数
            PRIM_NUM = 2,       // 面数
        };

        struct CUSTOM_FVF{
            math::SVector vecPos;
            IZ_FLOAT uv[2];
        };

    private:
        CPostEffectVS* m_pVS[E_POSTEFFECT_VTX_SHADER_NUM];

        // 頂点バッファ
        graph::CVertexBuffer* m_pVertexBuffer;

        // 頂点宣言オブジェクト
        graph::CVertexDeclaration* m_pVertexDecl;
    };

    // inline *******************************

    // コンストラクタ
    CPostEffectVSManager::CPostEffectVSManager()
    {
        FILL_ZERO(m_pVS, sizeof(m_pVS));

        m_pVertexBuffer = IZ_NULL;
        m_pVertexDecl = IZ_NULL;
    }

    // デストラクタ
    CPostEffectVSManager::~CPostEffectVSManager()
    {
        Release();
    }

    // 頂点シェーダ取得
    CPostEffectVS* CPostEffectVSManager::GetVS(E_POSTEFFECT_VTX_SHADER type)
    {
        IZ_ASSERT(type <E_POSTEFFECT_VTX_SHADER_NUM);

        return m_pVS[type];
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_POSTEFFECT_VS_MANAGER_H__)
