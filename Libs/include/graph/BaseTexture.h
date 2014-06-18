#if !defined(__IZANAGI_GRAPH_BASE_TEXTURE_H__)
#define __IZANAGI_GRAPH_BASE_TEXTURE_H__

#include "izDefs.h"
#include "izStd.h"
#include "graph/GraphResource.h"
#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
    /** This class is base of texture classes.
     */
    class CBaseTexture : public CGraphResource
    {
    protected:
        CBaseTexture()
        {
            m_Allocator = IZ_NULL;
            FILL_ZERO(&m_TexInfo, sizeof(m_TexInfo));
        }

        virtual ~CBaseTexture() {};

    public:
        /** Get texture width.
         *
         * @param [in] level mipmap level.
         * @return texture width, in pixels, by mipmap level you set.
         */
        IZ_UINT GetWidth(IZ_UINT level = 0) const
        {
            IZ_ASSERT(level < m_TexInfo.level);
            IZ_UINT ret = (m_TexInfo.width >> level);
            return ret;
        }

        /** Get texture height.
         *
         * @param [in] level mipmap level.
         * @return texture height, in pixels, by mipmap level you set.
         */
        IZ_UINT GetHeight(IZ_UINT level = 0) const
        {
            IZ_ASSERT(level < m_TexInfo.level);
            IZ_UINT ret = (m_TexInfo.height >> level);
            return ret;
        }

        /** Get texture pixel format.
         *
         * @return texture pixel format.
         */
        E_GRAPH_PIXEL_FMT GetPixelFormat() const
        {
            return m_TexInfo.fmt;
        }

        /** Get number of mipmap levels in this texture.
         *
         * @return number of mipmap levels.
         */
        IZ_UINT GetMipMapNum() const
        {
            return m_TexInfo.level;
        }

        /** Set filter of texture sampler state.
         *
         * @param [in] minFilter filter for minimize.
         * @param [in] magFilter filter for magnifier.
         * @param [in] mipFilter filter for mipmap.
         */
        virtual void SetFilter(
            E_GRAPH_TEX_FILTER minFilter,
            E_GRAPH_TEX_FILTER magFilter,
            E_GRAPH_TEX_FILTER mipFilter)
        {
            m_State.minFilter = minFilter;
            m_State.magFilter = magFilter;
            m_State.mipFilter = mipFilter;
        }

        /** Set addressing of texture sampler state.
         *
         * @param [in] addressU addressing for texture U coordinate.
         * @param [in] addressU addressing for texture V coordinate.
         */
        virtual void SetAddress(
            E_GRAPH_TEX_ADDRESS addressU,
            E_GRAPH_TEX_ADDRESS addressV)
        {
            m_State.addressU = addressU;
            m_State.addressV = addressV;
        }

        /** Get texture sampler state.
         *
         * @return reference to  sampler state.
         */
        const S_SAMPLER_STATE& GetState() const
        {
            return m_State;
        }

        /** Get if this texture is render target.
         *
         * @return If true, this texture is render target. If false, not.
         */
        IZ_BOOL IsRenderTarget() const
        {
            return m_TexInfo.is_rendertarget;
        }

        IZ_BOOL IsDynamic() const
        {
            return m_TexInfo.is_dynamic;
        }
        
        IZ_BOOL IsOnSysMem() const
        {
            return m_TexInfo.is_on_sysmem;
        }

        IZ_BOOL IsOnVram() const
        {
            return m_TexInfo.is_on_vram;
        }

        E_GRAPH_RSC_USAGE GetRscType() const
        {
            return m_TexInfo.usage;
        }

        /** Get texture type.
         *
         * @return texture type.
         */
        PURE_VIRTUAL(E_GRAPH_TEX_TYPE GetTexType());

        /** Get platform specified texture handle.
         *
         * @return platform specified texture handle.
         */
        PURE_VIRTUAL(TEX_HANDLE GetTexHandle());

    protected:
        // テクスチャ情報
        struct {
            IZ_UINT16 width;
            IZ_UINT16 height;
            
            IZ_UINT8  level;
            IZ_UINT8  depth;

            struct {
                IZ_UINT16 is_dynamic        : 1;    // 動的テクスチャかどうか
                IZ_UINT16 is_on_sysmem      : 1;    // システムメモリ上のみにあるかどうか
                IZ_UINT16 is_on_vram        : 1;    // VRAM上のみにあるかどうか
                IZ_UINT16 is_rendertarget   : 1;    // レンダーターゲットかどうか
            };

            E_GRAPH_PIXEL_FMT fmt;

            E_GRAPH_RSC_USAGE usage;
        } m_TexInfo;        

        // ステート
        S_SAMPLER_STATE m_State;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_BASE_TEXTURE_H__)
