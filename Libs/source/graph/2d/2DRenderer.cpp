#include "graph/2d/2DRenderer.h"
#include "graph/2d/2DShader.h"
#include "graph/VertexBuffer.h"
#include "graph/IndexBuffer.h"
#include "graph/VertexDeclaration.h"
#include "graph/GraphicsDevice.h"
#include "graph/BaseTexture.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    C2DRenderer* C2DRenderer::Create2DRenderer(
        CGraphicsDevice* device,
        IMemoryAllocator* allocator)
    {
        IZ_ASSERT(device != IZ_NULL);
        IZ_ASSERT(allocator != IZ_NULL);

        IZ_BOOL result = IZ_FALSE;
        IZ_UINT8* buf = IZ_NULL;
        C2DRenderer* instance = IZ_NULL;

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(C2DRenderer));
        if (!(result = (buf != IZ_NULL))) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        instance = new (buf)C2DRenderer;
        if (!(result = (instance != IZ_NULL))) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        {
            instance->m_Allocator = allocator;
            instance->AddRef();

            // 初期化
            result = instance->Init(device);
            if (!result) {
                IZ_ASSERT(IZ_FALSE);
                goto __EXIT__;
            }
        }

    __EXIT__:
        if (!result) {
            if (instance != IZ_NULL) {
                SAFE_RELEASE(instance);
            }
            else if (buf != IZ_NULL) {
                allocator->Free(buf);
            }
        }

        return instance;
    }

    // コンストラクタ
    C2DRenderer::C2DRenderer()
    {
        m_Allocator = IZ_NULL;

        m_pVB = IZ_NULL;
        m_pIB = IZ_NULL;
        m_pVD = IZ_NULL;

        m_pShader = IZ_NULL;

        m_nPrimType = PRIM_TYPE_SPRITE;

        m_nCurIdx = 0;
        m_nCurPrimNum = 0;
    }

    // デストラクタ
    C2DRenderer::~C2DRenderer()
    {
        SAFE_RELEASE(m_pVB);
        SAFE_RELEASE(m_pIB);
        SAFE_RELEASE(m_pVD);
        SAFE_RELEASE(m_pShader);
    }

    // 初期化
    IZ_BOOL C2DRenderer::Init(CGraphicsDevice* device)
    {
        IZ_ASSERT(device != IZ_NULL);

        // 頂点バッファ
        {
            m_pVB = device->CreateVertexBuffer(
                        VTX_STRIDE,
                        MAX_VERTEX_NUM,
                        E_GRAPH_RSC_USAGE_DYNAMIC);
            VRETURN(m_pVB != IZ_NULL);

            // かなり無茶するが・・・
            // m_pVBの分のデバイスの参照カウントを減らす
            device->Release();
        }

        // インデックスバッファ
        {
            m_pIB = device->CreateIndexBuffer(
                        MAX_INDEX_NUM,
                        E_GRAPH_INDEX_BUFFER_FMT_INDEX16,   // TODO とりあえず・・・
                        E_GRAPH_RSC_USAGE_DYNAMIC);
            VRETURN(m_pIB != IZ_NULL);

            // かなり無茶するが・・・
            // m_pIBの分のデバイスの参照カウントを減らす
            device->Release();
        }

        static const SVertexElement VtxElement[] = {
            {0,  0, E_GRAPH_VTX_DECL_TYPE_FLOAT4, E_GRAPH_VTX_DECL_USAGE_POSITION, 0},  // 座標
            {0, 16, E_GRAPH_VTX_DECL_TYPE_COLOR,  E_GRAPH_VTX_DECL_USAGE_COLOR,    0},  // 頂点カラー
            {0, 20, E_GRAPH_VTX_DECL_TYPE_FLOAT2, E_GRAPH_VTX_DECL_USAGE_TEXCOORD, 0},  // テクスチャ座標
        };

        {
            m_pVD = device->CreateVertexDeclaration(VtxElement, COUNTOF(VtxElement));
            VRETURN(m_pVD != IZ_NULL);
        }

        // シェーダ
        m_pShader = C2DShader::Create2DShader(
                        m_Allocator,
                        device);
        VRETURN(m_pShader != IZ_NULL);

        return IZ_TRUE;
    }

    // フレーム開始時に呼ぶこと
    void C2DRenderer::BeginFrame()
    {
        m_sVBInfo.Clear();
        m_sIBInfo.Clear();

        m_nCurIdx = 0;
        m_nCurPrimNum = 0;
    }

    // 描画開始
    IZ_BOOL C2DRenderer::BeginDraw()
    {
        m_sVBInfo.buf_ptr = IZ_NULL;
        m_sIBInfo.buf_ptr = IZ_NULL;

        m_bIsLock = IZ_FALSE;

        return IZ_TRUE;
    }

    // 描画終了
    IZ_BOOL C2DRenderer::EndDraw(CGraphicsDevice* device)
    {
        // といってもフラッシュするだけ・・・
        IZ_BOOL ret = Flush(device);

        return ret;
    }

    namespace {
        // 矩形の頂点数取得
        IZ_UINT GetRectVtxNum(IZ_UINT nRectNum)
        {
            return nRectNum << 2;
        }

        // 矩形を描画するために必要なプリミティブ数計算
        IZ_UINT ComputeRectRealPrimNum(IZ_UINT nRectNum)
        {
            // プリミティブ数
            // NOTE
            // １矩形 = ２三角形(プリミティブ)
            // 矩形と矩形の間をつなぐ見えない三角形は４つ存在する
            // ex) ２矩形 -> 0123 34 4567
            //               012-123-[233-334-344-445]-456-567 -> 2 * 2 + 4
            IZ_UINT nPrimNum = nRectNum * 2 + (nRectNum - 1) * 4;

            return nPrimNum;
        }

        // 矩形描画時の最小インデックス値計算
        IZ_INT ComputeRectMinIdx(
            IZ_UINT nIdx,
            IZ_UINT nRectNum)
        {
            // 最小インデックス値
            // NOTE
            // ある矩形のインデックスについて考える
            // n, n+1, n+2, n+3 となり、m_ushNowSetIndex = n+3 となっている
            // よって、-3することで矩形の先頭のインデックスになる。
            // さらに、矩形の先頭のインデックスは４の倍数になる。
            // よって、(描画する矩形数 - 1) x 4 だけマイナスすると最小インデックス値になる
            // -1 しているのは、描画する矩形数が１のときは、マイナスする必要がないので。
            IZ_INT nMinIdx = nIdx - 3 - 4 * (nRectNum - 1);

            return nMinIdx;
        }

        // 線の頂点数取得
        IZ_UINT GetLineVtxNum(IZ_UINT nLineNum)
        {
            return nLineNum << 1;
        }

        // 線を描画するために必要なプリミティブ数計算
        IZ_UINT ComputeLineRealPrimNum(IZ_UINT nLineNum)
        {
            return nLineNum;
        }

        // 矩形描画時の最小インデックス値計算
        IZ_INT ComputeLineMinIdx(
            IZ_UINT nIdx,
            IZ_UINT nLineNum)
        {
            IZ_INT nMinIdx = nIdx - 2 * nLineNum;
            return nMinIdx;
        }

        // 頂点数取得
        IZ_UINT (*GetVtxNum[])(IZ_UINT) = {
            GetRectVtxNum,
            GetRectVtxNum,
            GetLineVtxNum,
        };

        // 描画するために必要なプリミティブ数計算
        IZ_UINT (*ComputeRealPrimNum[])(IZ_UINT) = {
            ComputeRectRealPrimNum,
            ComputeRectRealPrimNum,
            ComputeLineRealPrimNum,
        };

        // 描画時の最小インデックス値計算
        IZ_INT (*ComputeMinIdx[])(IZ_UINT, IZ_UINT) = {
            ComputeRectMinIdx,
            ComputeRectMinIdx,
            ComputeLineMinIdx,
        };

        E_GRAPH_PRIM_TYPE PrimType[] = {
            E_GRAPH_PRIM_TYPE_TRIANGLESTRIP,
            E_GRAPH_PRIM_TYPE_TRIANGLESTRIP,
            E_GRAPH_PRIM_TYPE_LINESTRIP,
        };
    }   // namespace

    // 描画コマンドをフラッシュ
    IZ_BOOL C2DRenderer::Flush(CGraphicsDevice* device)
    {
        IZ_C_ASSERT(COUNTOF(GetVtxNum) == PRIM_TYPE_NUM);
        IZ_C_ASSERT(COUNTOF(ComputeRealPrimNum) == PRIM_TYPE_NUM);
        IZ_C_ASSERT(COUNTOF(ComputeMinIdx) == PRIM_TYPE_NUM);
        IZ_C_ASSERT(COUNTOF(PrimType) == PRIM_TYPE_NUM);

        // アンロック
        Unlock();

        if (m_nCurPrimNum == 0) {
            // 何も登録していない
            return IZ_TRUE;
        }

        {
            // インデックスバッファのセット
            device->SetIndexBuffer(m_pIB);

            // 頂点宣言のセット
            device->SetVertexDeclaration(m_pVD);

            // 頂点バッファのセット
            device->SetVertexBuffer(
                0,
                0,
                m_pVB->GetStride(),
                m_pVB);

            // シェーダのセット
            m_pShader->SetShader(device);

            // シェーダパラメータのセット
            m_pShader->SetShaderParams(device);
        }

        // 頂点数
        IZ_UINT vtxNum = (*GetVtxNum[m_nPrimType])(m_nCurPrimNum);

        // プリミティブ数
        IZ_UINT nPrimNum = (*ComputeRealPrimNum[m_nPrimType])(m_nCurPrimNum);

        // 最小インデックス値
        IZ_INT nMinIdx = (*ComputeMinIdx[m_nPrimType])(m_nCurIdx, m_nCurPrimNum);
        nMinIdx = (nMinIdx < 0 ? 0 : nMinIdx);  // 一応・・・

        IZ_UINT vtxOffset = m_sVBInfo.offset / m_pVB->GetStride();
        IZ_UINT idxOffset = m_sIBInfo.offset / m_pIB->GetStride();

        // NOTE
        // インデックスは必ず０から始まる

        // 描画
        IZ_BOOL ret = device->DrawIndexedPrimitive(
                        PrimType[m_nPrimType],
                        vtxOffset,   // BaseIdx
                        vtxNum, // VtxNum
                        idxOffset,  // StartIdx
                        nPrimNum);  // PrimCnt

        m_nCurIdx = 0;
        m_nCurPrimNum = 0;

        return ret;
    }

    // スプライト描画
    IZ_BOOL C2DRenderer::DrawSprite(
        CGraphicsDevice* device,
        const CFloatRect& rcSrc,
        const CIntRect& rcDst,
        const IZ_COLOR color/*= IZ_COLOR_RGBA(255, 255, 255, 255)*/)
    {
        // 描画準備
        VRETURN(PrepareDraw(
                    device,
                    PRIM_TYPE_SPRITE));

        // ロック
        VRETURN(Lock());

        // UV座標矩形
        CFloatRect rcUV(rcSrc);
        {
            // テクセルの中心をサンプリングするようにする
            CBaseTexture* pTex = device->GetTexture(0);
            IZ_FLOAT fTexWidth = (IZ_FLOAT)pTex->GetWidth();
            IZ_FLOAT fTexHeight = (IZ_FLOAT)pTex->GetHeight();
        
            rcUV.left += 0.5f / fTexWidth;
            rcUV.top += 0.5f / fTexHeight;
            rcUV.right += 0.5f / fTexWidth;
            rcUV.bottom += 0.5f / fTexHeight;
        }
    
        // 頂点データセット
        SetVtx(
            rcUV, rcDst,
            color);

        // インデックスデータセット
        SetIdx();

        m_nCurPrimNum++;

        return IZ_TRUE;
    }

    // スプライト描画
    IZ_BOOL C2DRenderer::DrawSpriteEx(
        CGraphicsDevice* device,
        const CIntRect& rcSrc,
        const CIntRect& rcDst,
        const IZ_COLOR color/*= IZ_COLOR_RGBA(255, 255, 255, 255)*/)
    {
        // 描画準備
        VRETURN(PrepareDraw(
                    device,
                    PRIM_TYPE_SPRITE));

        // ロック
        VRETURN(Lock());

        // UV座標矩形
        CFloatRect rcUV;
        {
            IZ_FLOAT fTexLeft = (IZ_FLOAT)(rcSrc.left);
            IZ_FLOAT fTexTop = (IZ_FLOAT)(rcSrc.top);
            IZ_FLOAT fTexRight = (IZ_FLOAT)(rcSrc.right());
            IZ_FLOAT fTexBottom = (IZ_FLOAT)(rcSrc.bottom());

            {
                // テクセルの中心をサンプリングするようにする
                fTexLeft += 0.5f;
                fTexTop += 0.5f;
                fTexRight += 0.5f;
                fTexBottom += 0.5f;

                CBaseTexture* pTex = device->GetTexture(0);
                IZ_FLOAT fTexWidth = (IZ_FLOAT)pTex->GetWidth();
                IZ_FLOAT fTexHeight = (IZ_FLOAT)pTex->GetHeight();
            
                rcUV.left = fTexLeft / fTexWidth;
                rcUV.top = fTexTop / fTexHeight;
                rcUV.right = fTexRight / fTexWidth;
                rcUV.bottom = fTexBottom / fTexHeight;
            }
        }
    
        // 頂点データセット
        SetVtx(
            rcUV, rcDst,
            color);

        // インデックスデータセット
        SetIdx();

        ++m_nCurPrimNum;

        return IZ_TRUE;
    }

    // 矩形描画
    IZ_BOOL C2DRenderer::DrawRect(
        CGraphicsDevice* device,
        const CIntRect& rcDst,
        const IZ_COLOR color)
    {
        // 元の値を覚えておく
        E_GRAPH_2D_RENDER_OP nPrevOp = GetRenderOp();

        // テクスチャを張らないので強制的に
        VRETURN(SetRenderOp(
                    device,
                    E_GRAPH_2D_RENDER_OP_VTX));

        // 描画準備
        VRETURN(PrepareDraw(
                    device,
                    PRIM_TYPE_RECT));

        // ロック
        VRETURN(Lock());
    
        // 頂点データセット
        SetVtx(
            CFloatRect(),   // ダミー
            rcDst,
            color);

        // インデックスデータセット
        SetIdx();

        ++m_nCurPrimNum;

        // 元に戻す
        VRETURN(SetRenderOp(
                    device,
                    nPrevOp));

        return IZ_TRUE;
    }

    // 線描画
    IZ_BOOL C2DRenderer::DrawLine(
        CGraphicsDevice* device,
        const CIntPoint& ptStart,
        const CIntPoint& ptGoal,
        const IZ_COLOR color)
    {
        // 元の値を覚えておく
        E_GRAPH_2D_RENDER_OP nPrevOp = GetRenderOp();

        // テクスチャを張らないので強制的に
        VRETURN(SetRenderOp(
                    device,
                    E_GRAPH_2D_RENDER_OP_VTX));

        // 描画準備
        VRETURN(PrepareDraw(
                    device,
                    PRIM_TYPE_LINE));

        // ロック
        VRETURN(Lock());
    
        // 頂点データセット
        {
            CUSTOM_VERTEX* v = (CUSTOM_VERTEX*)m_sVBInfo.buf_ptr;

            // 頂点色
            v[0].color = v[1].color = v[2].color = v[3].color = color;

            // 頂点位置
            v[0].x = (IZ_FLOAT)ptStart.x;
            v[0].y = (IZ_FLOAT)ptStart.y;

            v[1].x = (IZ_FLOAT)ptGoal.x;
            v[1].y = (IZ_FLOAT)ptGoal.y;

            v[0].z = v[1].z = 0.5f; // 適当
            v[0].w = v[1].w = 1.0f;

            // １ライン２頂点なので
            m_sVBInfo.buf_ptr = (IZ_UINT8*)m_sVBInfo.buf_ptr + 2 * m_pVB->GetStride();
            m_sVBInfo.num += 2;
        }

        // インデックスデータセット
        {
            IZ_ASSERT(m_pIB->GetFormat() == E_GRAPH_INDEX_BUFFER_FMT_INDEX16);

            IZ_UINT16* pIdx = (IZ_UINT16*)m_sIBInfo.buf_ptr;

            *pIdx++ = m_nCurIdx++;
            *pIdx++ = m_nCurIdx++;

            m_sIBInfo.buf_ptr = pIdx;
            m_sIBInfo.num += 2;
        }

        ++m_nCurPrimNum;

        // 元に戻す
        VRETURN(SetRenderOp(
                    device,
                    nPrevOp));

        return IZ_TRUE;
    }

    // 描画設定セット
    IZ_BOOL C2DRenderer::SetRenderOp(
        CGraphicsDevice* device,
        E_GRAPH_2D_RENDER_OP nOp)
    {
        IZ_ASSERT(m_pShader != IZ_NULL);

        // 前と異なるときはフラッシュする
        if (m_pShader->GetRenderOp() != nOp) {
            VRETURN(Flush(device));
        }

        m_pShader->SetRenderOp(device, nOp);

        return IZ_TRUE;
    }

    // ユーザー定義のシェーダをセット
    void C2DRenderer::SetUserDefsShader(
        CVertexShader* vs,
        CPixelShader* ps)
    {
        IZ_ASSERT(m_pShader != IZ_NULL);
        m_pShader->SetUserDefsShader(vs, ps);
    }

    // 描画モードがユーザー定義の時のみ2D用のシェーダープログラムを取得
    CShaderProgram* C2DRenderer::Get2DShaderProgramIfRenderOpIsUserDefs()
    {
        IZ_ASSERT(m_pShader != IZ_NULL);

        if (m_pShader->GetRenderOp() == E_GRAPH_2D_RENDER_OP_USER_DEFS) {
            return m_pShader->GetShaderProgram();
        }

        return IZ_NULL;
    }

    // 描画準備
    IZ_BOOL C2DRenderer::PrepareDraw(
        CGraphicsDevice* device,
        PRIM_TYPE nPrimType)
    {
        if (m_nPrimType != nPrimType) {
            // プリミティブタイプが違うときはフラッシュする
            VRETURN(Flush(device));
        }

        m_nPrimType = nPrimType;

        // 頂点バッファ
        {
            // １プリミティブあたりの頂点数
            IZ_UINT nVtxNumPerPrim = 0;
            switch (nPrimType) {
            case PRIM_TYPE_SPRITE:
            case PRIM_TYPE_RECT:
                nVtxNumPerPrim = 4;
                break;
            case PRIM_TYPE_LINE:
                nVtxNumPerPrim = 2;
                break;
            default:
                IZ_ASSERT(IZ_FALSE);
                break;
            };

            IZ_UINT size = m_sVBInfo.num * m_pVB->GetStride();
            size += nVtxNumPerPrim * m_pVB->GetStride();

            if (size > m_pVB->GetSize()) {
                IZ_ASSERT(IZ_FALSE);    // 一応

                Flush(device);
                m_sVBInfo.Clear();

                m_sVBInfo.next_lock_discard = IZ_TRUE;
            }
        }

        // インデックスバッファ
        {
            // １プリミティブあたりのインデックス数
            IZ_UINT nIdxNumPerPrim = 0;
            switch (nPrimType) {
            case PRIM_TYPE_SPRITE:
            case PRIM_TYPE_RECT:
                nIdxNumPerPrim = (m_nCurIdx == 0 ? 4 : 6);
                break;
            case PRIM_TYPE_LINE:
                nIdxNumPerPrim = 2;
                break;
            default:
                IZ_ASSERT(IZ_FALSE);
                break;
            };

            IZ_UINT size = m_sIBInfo.num * m_pIB->GetStride();
            size += nIdxNumPerPrim * m_pIB->GetStride();

            if (size > m_pIB->GetSize()) {
                IZ_ASSERT(IZ_FALSE);    // 一応

                Flush(device);
                m_sIBInfo.Clear();

                m_sIBInfo.next_lock_discard = IZ_TRUE;
            }
        }

        return IZ_TRUE;
    }

    // ロック
    IZ_BOOL C2DRenderer::Lock()
    {
        if (!IsLock()) {
            // ロックフラグ切り替え
            ToggleIsLock();

            // 頂点バッファ
            {
                m_sVBInfo.offset = m_sVBInfo.num * m_pVB->GetStride();

                // ロックサイズ
                IZ_UINT nLockSize = (m_sVBInfo.next_lock_discard    // DISCARDでロックするかどうか
                                        ? 0
                                        : m_pVB->GetSize() - m_sVBInfo.offset); // 残りロック可能サイズ

                IZ_BOOL result = m_pVB->Lock(
                                    m_sVBInfo.offset,
                                    nLockSize,
                                    (void**)&m_sVBInfo.buf_ptr,
                                    IZ_FALSE,
                                    m_sVBInfo.next_lock_discard);
                m_sVBInfo.next_lock_discard = IZ_FALSE;
                VRETURN(result);
            }

            // インデックスバッファ
            {
                m_sIBInfo.offset = m_sIBInfo.num * m_pIB->GetStride();

                // ロックサイズ
                IZ_UINT nLockSize = (m_sIBInfo.next_lock_discard    // DISCARDでロックするかどうか
                                        ? 0
                                        : m_pIB->GetSize() - m_sIBInfo.offset); // 残りロック可能サイズ

                IZ_BOOL result = m_pIB->Lock(
                                    m_sIBInfo.offset,
                                    nLockSize,
                                    (void**)&m_sIBInfo.buf_ptr,
                                    IZ_FALSE,
                                    m_sIBInfo.next_lock_discard);
                m_sIBInfo.next_lock_discard = IZ_FALSE;
                VRETURN(result);
            }
        }

        return IZ_TRUE;
    }

    // アンロック
    void C2DRenderer::Unlock()
    {
        if (IsLock()) {
            // ロックされている
            ToggleIsLock();

            m_pVB->Unlock();
            m_pIB->Unlock();

            m_sVBInfo.buf_ptr = IZ_NULL;
            m_sIBInfo.buf_ptr = IZ_NULL;
        }
    }

    // 頂点データセット
    void C2DRenderer::SetVtx(
        const CFloatRect& rcSrc,
        const CIntRect& rcDst,
        const IZ_COLOR color)
    {
        CUSTOM_VERTEX* v = (CUSTOM_VERTEX*)m_sVBInfo.buf_ptr;

        // 頂点色
        v[0].color = v[1].color = v[2].color = v[3].color = color;

        // 転送先矩形
        {
            IZ_FLOAT fLeft = (IZ_FLOAT)(rcDst.left);
            IZ_FLOAT fTop = (IZ_FLOAT)(rcDst.top);
            IZ_FLOAT fRight = (IZ_FLOAT)(rcDst.right());
            IZ_FLOAT fBottom = (IZ_FLOAT)(rcDst.bottom());

            v[0].x = fLeft;
            v[0].y = fTop;

    #ifdef IZ_COORD_LEFT_HAND
            v[1].x = fRight;
            v[1].y = fTop;

            v[2].x = fLeft;
            v[2].y = fBottom;
    #else
            v[1].x = fLeft;
            v[1].y = fBottom;

            v[2].x = fRight;
            v[2].y = fTop;
    #endif

            v[3].x = fRight;
            v[3].y = fBottom;

            v[0].z = v[1].z = v[2].z = v[3].z = 0.5f;   // 適当・・・
            v[0].w = v[1].w = v[2].w = v[3].w = 1.0f;
        }

        v[0].u = rcSrc.left;
        v[0].v = rcSrc.top;

    #ifdef IZ_COORD_LEFT_HAND
        v[1].u = rcSrc.right;
        v[1].v = rcSrc.top;

        v[2].u = rcSrc.left;
        v[2].v = rcSrc.bottom;
    #else
        v[1].u = rcSrc.left;
        v[1].v = rcSrc.bottom;

        v[2].u = rcSrc.right;
        v[2].v = rcSrc.top;
    #endif

        v[3].u = rcSrc.right;
        v[3].v = rcSrc.bottom;

        // １矩形４頂点なので
        m_sVBInfo.buf_ptr = (IZ_UINT8*)m_sVBInfo.buf_ptr + 4 * m_pVB->GetStride();
        m_sVBInfo.num += 4;
    }

    // インデックスデータセット
    void C2DRenderer::SetIdx()
    {
        // NOTE
        // 0123 34 4567・・・
        // 012-123-[233-334-344-445]-456-567-・・・
        // 233 のようにインデックスが二つ連続する場合は、見えない三角形ができる

        IZ_ASSERT(m_pIB->GetFormat() == E_GRAPH_INDEX_BUFFER_FMT_INDEX16);

        IZ_UINT16* pIdx = (IZ_UINT16*)m_sIBInfo.buf_ptr;

        if (m_nCurIdx == 0) {
            // 最初は 0, 1, 2, 3 固定
            *pIdx++ = 0;
            *pIdx++ = 1;
            *pIdx++ = 2;
            *pIdx++ = 3;

            m_nCurIdx = 3;

            m_sIBInfo.buf_ptr = pIdx;
            m_sIBInfo.num += 4;
        }
        else {
            // 矩形と矩形の間の見えない三角形を作るインデックス
            *pIdx++ = m_nCurIdx++;
            *pIdx++ = m_nCurIdx;

            // 次の矩形
            *pIdx++ = m_nCurIdx++;
            *pIdx++ = m_nCurIdx++;
            *pIdx++ = m_nCurIdx++;
            *pIdx++ = m_nCurIdx;

            m_sIBInfo.buf_ptr = pIdx;
            m_sIBInfo.num += 6;
        }
    }

    // リソースリセット
    void C2DRenderer::DisableResource()
    {
        IZ_ASSERT(m_pVB != IZ_NULL);
        IZ_ASSERT(m_pIB != IZ_NULL);

        m_pVB->Disable();
        m_pIB->Disable();
    }

    // リセット
    void C2DRenderer::RestoreResource()
    {
        IZ_ASSERT(m_pVB != IZ_NULL);
        IZ_ASSERT(m_pIB != IZ_NULL);

        m_pVB->Restore();
        m_pIB->Restore();
    }
}   // namespace graph
}   // namespace izanagi
