#if !defined(__IZANAGI_FONT_FONT_RENDERER_H__)
#define __IZANAGI_FONT_FONT_RENDERER_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "izIo.h"
#include "FontDefs.h"

namespace izanagi {
    /**
    * フォントレンダラ
    */
    class CFontRenderer : public CObject {
    public:
        /**
        */
        typedef IZ_UINT (*RenderFontHandler)(graph::CGraphicsDevice* device, IZ_UINT code, const SIntRect& rc, IZ_UINT x, IZ_UINT y, IZ_COLOR color);

        /**
        */
        static CFontRenderer* CreateFontRendererBmp(
            IMemoryAllocator* allocator,
            graph::CGraphicsDevice* device,
            IZ_UINT maxRegisterNum,
            IInputStream* in);

        // TODO
        /**
        */
        static CFontRenderer* CreateFontRendererFT(
            IMemoryAllocator* allocator,
            graph::CGraphicsDevice* device,
            IZ_UINT maxRegisterNum);

    protected:
        enum {
            // 最大表示文字数
            MAX_FONT_NUM = 32768,

            // ソーステクスチャ文字マージン
            CHAR_MARGIN = 1,
        };

    protected:
        CFontRenderer() { m_RenderFontHadler = IZ_NULL; }
        virtual ~CFontRenderer() {}

        NO_COPIABLE(CFontRenderer);

    public:
        // 登録
        virtual IZ_BOOL Register(const void* str) = 0;

        // 描画
        // 登録されていない場合は、描画しない
        virtual void Render(
            const void* str,
            IZ_INT nX, IZ_INT nY,
            IZ_COLOR color = IZ_COLOR_RGBA(255, 255, 255, 255)) = 0;

        // 描画
        // 登録されていない場合は、登録を行う
        virtual IZ_BOOL RenderImmediate(
            void* str,
            IZ_INT nX, IZ_INT nY,
            IZ_COLOR color = IZ_COLOR_RGBA(255, 255, 255, 255)) = 0;

        // ピクセル幅取得
        virtual IZ_UINT GetWidth(void* str) = 0;

        // 登録済みかどうか
        virtual IZ_BOOL IsRegistered(IZ_UINT code) = 0;

        // ピクセル高さ取得
        virtual IZ_UINT GetHeight() const = 0;

        // 文字コード取得
        virtual E_FONT_CHAR_ENCODE GetCharEncode() const = 0;

        // フォント描画ハンドラセット
        void SetFonrRenderHandler(RenderFontHandler* pHandler) { m_RenderFontHadler = pHandler; }

    protected:
        RenderFontHandler* m_RenderFontHadler;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_FONT_FONT_RENDERER_H__)
