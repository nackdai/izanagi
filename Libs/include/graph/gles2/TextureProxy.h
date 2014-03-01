#if !defined(__IZANAGI_GRAPH_TEXTURE_PROXY_H__)
#define __IZANAGI_GRAPH_TEXTURE_PROXY_H__

#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;

    template <typename _BASE>
    class CTextureProxy : public _BASE
    {
        friend class CGraphicsDeviceGLES2;

    protected:
        CTextureProxy()
        {
            for (IZ_UINT i = 0; i < E_GRAPH_SAMPLER_STATE_TYPE_NUM; i++) {
                m_IsDirty[i] = IZ_TRUE;
            }
        }
        virtual ~CTextureProxy() {}

    public:
        // フィルタセット
        virtual void SetFilter(
            E_GRAPH_TEX_FILTER minFilter,
            E_GRAPH_TEX_FILTER magFilter,
            E_GRAPH_TEX_FILTER mipFilter)
        {
            // MinFilter
            if (!m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_MINFILTER]) {
                m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_MINFILTER] = (m_State.minFilter != minFilter);
                if (m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_MINFILTER]) {
                    m_State.minFilter = minFilter;
                }
            }

            // MagFilter
            if (!m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_MAGFILTER]) {
                m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_MAGFILTER] = (m_State.magFilter != magFilter);
                if (m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_MAGFILTER]) {
                    m_State.magFilter = magFilter;
                }
            }
            
            // MipFilter
            if (!m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_MIPFILTER]) {
                m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_MIPFILTER] = (m_State.mipFilter != mipFilter);
                if (m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_MIPFILTER]) {
                    m_State.mipFilter = mipFilter;
                }
            }
        }

        // アドレッシングセット
        virtual void SetAddress(
            E_GRAPH_TEX_ADDRESS addressU,
            E_GRAPH_TEX_ADDRESS addressV)
        {
            // AddressU
            if (!m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSU]) {
                m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSU] = (m_State.addressU != addressU);
                if (m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSU]) {
                    m_State.addressU = addressU;
                }
            }

            // AddressV
            if (!m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSV]) {
                m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSV] = (m_State.addressV != addressV);
                if (m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSV]) {
                    m_State.addressV = addressV;
                    
                }
            }
        }

    public:
        void ClearDirty()
        {
            for (IZ_UINT i = 0; i < E_GRAPH_SAMPLER_STATE_TYPE_NUM; i++) {
                m_IsDirty[i] = IZ_FALSE;
            }
        }

        IZ_BOOL IsDirty(E_GRAPH_SAMPLER_STATE_TYPE type)
        {
            return m_IsDirty[type];
        }

    private:
        IZ_BOOL m_IsDirty[E_GRAPH_SAMPLER_STATE_TYPE_NUM];
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_TEXTURE_PROXY_H__)
