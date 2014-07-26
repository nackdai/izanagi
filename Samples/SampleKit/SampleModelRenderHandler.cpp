#include "SampleModelRenderHandler.h"

namespace izanagi {
namespace sample {
    void CSampleMdlRenderHandler::BeginRenderMesh()
    {
        m_nCnt = 0;

        izanagi::math::SMatrix44::SetUnit(m_Mtx[0]);
        izanagi::math::SMatrix44::SetUnit(m_Mtx[1]);
        izanagi::math::SMatrix44::SetUnit(m_Mtx[2]);
        izanagi::math::SMatrix44::SetUnit(m_Mtx[3]);

        m_Handle = 0;
    }

    void CSampleMdlRenderHandler::EndRenderMesh()
    {
    }

    void CSampleMdlRenderHandler::SetJointMatrix(
        IZ_UINT nIdx,
        const izanagi::math::SMatrix44& mtx)
    {
        izanagi::math::SMatrix44::Copy(m_Mtx[m_nCnt], mtx);
        m_nCnt++;
    }

    void CSampleMdlRenderHandler::CommitChanges(izanagi::graph::CGraphicsDevice* device)
    {
        if (m_Handle == 0) {
            m_Handle = m_pShader->GetParameterByName("vJointMatrix");
            //IZ_ASSERT(m_Handle > 0);
        }

        if (m_Handle > 0) {
            m_pShader->SetParamValue(
                m_Handle,
                m_Mtx,
                sizeof(izanagi::math::SMatrix44) * m_nCnt);
        }

        m_pShader->CommitChanges(device);
    }
}   // namespace izanagi
}   // namespace sample
