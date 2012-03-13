#if !defined(__IZANAGI_FONT_FONT_RENDERER_H__)
#define __IZANAGI_FONT_FONT_RENDERER_H__

#include "izDefs.h"
#include "izStd.h"
#include "FontDefs.h"

namespace izanagi {
	class IMemoryAllocator;
	class CGraphicsDevice;
	class IInputStream;

	/**
	* フォントレンダラ
	*/
	class CFontRenderer : public CObject {
	public:
		/**
		*/
		typedef IZ_UINT (*RenderFontHandler)(CGraphicsDevice* pDevice, IZ_UINT code, const SIntRect& rc, IZ_UINT x, IZ_UINT y, IZ_COLOR color);

		/**
		*/
		static CFontRenderer* CreateFontRendererBmp(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IZ_UINT	nMaxRegisterNum,
			IInputStream* in);

		// TODO
		/**
		*/
		static CFontRenderer* CreateFontRendererFT(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IZ_UINT	nMaxRegisterNum);

	protected:
		enum {
			// 最大表示文字数
			MAX_FONT_NUM = 32768,

			// ソーステクスチャ文字マージン
			CHAR_MARGIN = 1,
		};

	protected:
		CFontRenderer() { m_pRenderFontHadler = IZ_NULL; }
		virtual ~CFontRenderer() {}

		NO_COPIABLE(CFontRenderer);

	public:
		// 登録
		virtual IZ_BOOL Register(const void* pStr) = 0;

		// 描画
		// 登録されていない場合は、描画しない
		virtual void Render(
			const void* pStr,
			IZ_INT nX, IZ_INT nY,
			IZ_COLOR nColor = IZ_COLOR_RGBA(255, 255, 255, 255)) = 0;

		// 描画
		// 登録されていない場合は、登録を行う
		virtual IZ_BOOL RenderImmediate(
			void* pStr,
			IZ_INT nX, IZ_INT nY,
			IZ_COLOR nColor = IZ_COLOR_RGBA(255, 255, 255, 255)) = 0;

		// ピクセル幅取得
		virtual IZ_UINT GetWidth(void* pStr) = 0;

		// 登録済みかどうか
		virtual IZ_BOOL IsRegistered(IZ_UINT code) = 0;

		// ピクセル高さ取得
		virtual IZ_UINT GetHeight() const = 0;

		// 文字コード取得
		virtual E_FONT_CHAR_ENCODE GetCharEncode() const = 0;

		// フォント描画ハンドラセット
		void SetFonrRenderHandler(RenderFontHandler* pHandler) { m_pRenderFontHadler = pHandler; }

	protected:
		RenderFontHandler* m_pRenderFontHadler;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_FONT_FONT_RENDERER_H__)
