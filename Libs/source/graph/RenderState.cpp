#include "graph/RenderState.h"

namespace izanagi
{
namespace graph
{
    // コンストラクタ
    CRenderState::CRenderState()
    {
        m_IsSavedRS = IZ_FALSE;
    }

    // デストラクタ
    CRenderState::~CRenderState()
    {
    }

    // 現在のレンダーステートを保存
    IZ_BOOL CRenderState::Save()
    {
        // 保持中でないこと
        IZ_ASSERT(!m_IsSavedRS);

        IZ_BOOL ret = IZ_FALSE;

        if (!m_IsSavedRS) {
            memcpy(&m_SaveRS.dwRS, this->dwRS, sizeof(m_SaveRS.dwRS));

            m_SaveRS.stencilParams = this->stencilParams;

            m_IsSavedRS = IZ_TRUE;
            ret = IZ_TRUE;
        }

        return ret;
    }

    // 保存したレンダーステートを元に戻す
    IZ_BOOL CRenderState::Load(CGraphicsDevice* pDevice)
    {
        // 保持中であること
        IZ_ASSERT(m_IsSavedRS);

        IZ_BOOL ret = IZ_FALSE;

        if (m_IsSavedRS) {
            for (IZ_UINT i = 0; i < E_GRAPH_RS_NUM; ++i) {
                SetRenderState(
                    pDevice,
                    static_cast<E_GRAPH_RENDER_STATE>(i),
                    m_SaveRS.dwRS[i]);
            }

            //SetStencilFunc(pDevice, stencilParams.func, stencilParams.ref, stencilParams.mask);
            //SetStencilOp(pDevice, stencilParams.opPass, stencilParams.opZFail, stencilParams.opFail);

            m_IsSavedRS = IZ_FALSE;
            ret = IZ_TRUE;
        }

        return ret;
    }
}   // namespace graph
}   // namespace izanagi
