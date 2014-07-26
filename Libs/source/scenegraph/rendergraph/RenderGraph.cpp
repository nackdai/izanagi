#include "scenegraph/rendergraph/RenderGraph.h"
#include "scenegraph/msh/MeshInstance.h"
#include "scenegraph/mdl/izModel.h"
#include "scenegraph/rendergraph/SceneRendererInterface.h"
#include "scenegraph/Camera.h"

using namespace izanagi;

// インスタンス作成
CRenderGraph* CRenderGraph::CreateRenderGraph(
    IMemoryAllocator* pAllocator,
    IZ_UINT nZLevel)
{
    IZ_ASSERT(pAllocator != IZ_NULL);
    IZ_ASSERT(nZLevel > 0);

    size_t instanceSize = sizeof(CRenderGraph);
    size_t listSize = sizeof(CStdSet<IRenderElement>) * nZLevel;

    size_t size = instanceSize + listSize * 2;
    
    IZ_UINT8* buf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, size));
    VRETURN_NULL(buf != IZ_NULL);

    // 先頭位置を覚えておく
    IZ_UINT8* top = buf;

    CRenderGraph* pInstance = new(buf) CRenderGraph;
    {
        buf += instanceSize;

        pInstance->AddRef();
        pInstance->m_Allocator = pAllocator;

        pInstance->m_nZLevvel = nZLevel;

        pInstance->m_Opaque = reinterpret_cast<CStdSet<IRenderElement>*>(buf);
        buf += listSize;

        pInstance->m_Translucent = reinterpret_cast<CStdSet<IRenderElement>*>(buf);
        buf += listSize;

        for (IZ_UINT i = 0; i < nZLevel; i++) {
            pInstance->m_Opaque[i].Init();
            pInstance->m_Translucent[i].Init();
        }
    }

    IZ_ASSERT(CStdUtil::GetPtrDistance(buf, top) == size);
    
    return pInstance;
}

CRenderGraph::CRenderGraph()
{
    m_Allocator = IZ_NULL;

    m_nZLevvel = 0;
    m_Opaque = IZ_NULL;
    m_Translucent = IZ_NULL;
}

CRenderGraph::~CRenderGraph()
{
}

namespace {
    IZ_FLOAT _ComputeZLevel(
        const CCamera& camera,
        const math::SVector4& pos)
    {
        const SCameraParam& cameraParam = camera.GetParam();

        math::SVector4 v;
        math::SMatrix44::Apply(v, pos, cameraParam.mtxW2C);

        IZ_FLOAT w = v.w;
        IZ_FLOAT normalizedZ = v.z;

        // ZLevelが偏らないようにW値を利用してみる
        IZ_FLOAT n = cameraParam.cameraNear;
        IZ_FLOAT f = cameraParam.cameraFar;
        normalizedZ = (w - n) / (f - n);

        // NOTE
        // DirectX ->  0 <= z/w <= 1
        // OpenGL  -> -1 <= z/w <= 1
        // そのため、どちらでも 0 - 1 の範囲に正規化する必要がある
        //normalizedZ = (normalizedZ + 1.0f) * 0.5f;    // For OpenGL

        return normalizedZ;
    }
}   // namespace

// 登録.
IZ_BOOL CRenderGraph::Register(
    const CCamera& camera,
    const math::SVector4& pos,
    IRenderElement* element)
{
    IZ_FLOAT normalizedZ = _ComputeZLevel(camera, pos);
    IZ_UINT level = static_cast<IZ_UINT>(normalizedZ * m_nZLevvel);

    element->SetZ(normalizedZ);

    if (element->IsTranslucent()) {
        // 半透明
        m_Translucent[level].Add(element->GetListItem());
    }
    else {
        m_Opaque[level].Add(element->GetListItem());
    }

    return IZ_TRUE;
}

// モデル登録
IZ_BOOL CRenderGraph::Register(
    const CCamera& camera,
    const math::SVector4& pos,
    IModel* model)
{
    IZ_UINT num = model->GetMeshSetNum();

    for (IZ_UINT i = 0; i < num; i++) {
        // TODO
        Register(
            camera,
            pos,
            model->GetMeshSet(i));
    }

    return IZ_TRUE;
}

// 描画
IZ_BOOL CRenderGraph::Render(
    graph::CGraphicsDevice* device,
    ISceneRenderer* sceneRenderer,
    IMshRenderHandler* renderHandler)
{
    // Opaque
    for (IZ_UINT i = 0; i < m_nZLevvel; i++) {
        VRETURN(
            RenderInternal(
                device, 
                sceneRenderer, 
                renderHandler, 
                m_Opaque[i], 
                ERenderOrder_Forward));
    }

    // Tranlucent
    for (IZ_UINT i = 0; i < m_nZLevvel; i++) {
        VRETURN(
            RenderInternal(
                device, 
                sceneRenderer, 
                renderHandler, 
                m_Translucent[i], 
                ERenderOrder_Reverse));
    }

    return IZ_TRUE;
}

// 描画内部処理
IZ_BOOL CRenderGraph::RenderInternal(
    graph::CGraphicsDevice* device,
    ISceneRenderer* sceneRenderer,
    IMshRenderHandler* renderHandler,
    CStdSet<IRenderElement>& list,
    ERenderOrder order)
{
    // Get top or tail of ZList.
    CStdSet<IRenderElement>::Item* item = (order == ERenderOrder_Forward
                                            ? list.GetTop()
                                            : list.GetTail());

    for (; item != IZ_NULL;) {
        IRenderElement* element = item->GetData();
        IZ_ASSERT(element != IZ_NULL);

        // マテリアル取得
        CMaterial* mtrl = element->GetMaterial();
        VRETURN(mtrl != IZ_NULL);

        // マテリアルに設定されているシェーダ取得
        shader::IShader* shader = mtrl->GetShader();
        VRETURN(shader != IZ_NULL);

        // シーン描画開始
        // TODO
        // テクニック指定
        IZ_UINT passNum = sceneRenderer->BeginRender(device, shader, 0);

        // シェーダが切り替わるときに一連のシェーダの開始位置に戻ることがあるので
        // それを覚えておく
        CStdSet<IRenderElement>::Item* itemPivot = item;

        for (IZ_UINT passCnt = 0; passCnt < passNum; passCnt++) {
            while (item != IZ_NULL) {
                element = item->GetData();

                if (sceneRenderer->IterRender(
                        device, 
                        passCnt, 
                        element,
                        renderHandler))
                {
                    // 描画継続なので、次の要素を取得
                    item = (order == ERenderOrder_Forward
                                ? item->GetNext()
                                : item->GetPrev());
                }
                else {
                    // シェーダが変わる

                    if (passCnt == passNum - 1) {
                        // 全パス描画したので、このシェーダでの描画は終了
                        VRETURN(sceneRenderer->EndRender(device));
                        break;
                    }

                    // ここまでの一連のシェーダの開始位置に戻る
                    item = itemPivot;
                    break;
                }
            }
        }
    }

    return IZ_TRUE;
}

void CRenderGraph::Clear()
{
    for (IZ_UINT i = 0; i < m_nZLevvel; i++) {
        m_Opaque[i].Clear();
        m_Translucent[i].Clear();
    }
}
