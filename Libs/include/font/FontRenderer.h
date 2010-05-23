#if !defined(__URANUS_FONT_FONT_RENDERER_H__)
#define __URANUS_FONT_FONT_RENDERER_H__

#include "unDefs.h"
#include "unStd.h"
#include "FontDefs.h"

namespace uranus {
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
		typedef UN_UINT (*RenderFontHandler)(CGraphicsDevice* pDevice, UN_UINT code, const SIntRect& rc, UN_UINT x, UN_UINT y, UN_COLOR color);

		/**
		*/
		static CFontRenderer* CreateFontRendererBmp(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT	nMaxRegisterNum,
			IInputStream* in);

		// TODO
		/**
		*/
		static CFontRenderer* CreateFontRendererFT(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT	nMaxRegisterNum);

	protected:
		enum {
			// 最大表示文字数
			MAX_FONT_NUM = 32768,

			// ソーステクスチャ文字マージン
			CHAR_MARGIN = 1,
		};

	protected:
		CFontRenderer() { m_pRenderFontHadler = UN_NULL; }
		virtual ~CFontRenderer() {}

		NO_COPIABLE(CFontRenderer);

	public:
		// 登録
		virtual UN_BOOL Register(void* pStr) = 0;

		// 描画
		// 登録されていない場合は、描画しない
		virtual void Render(
			void* pStr,
			UN_INT nX, UN_INT nY,
			UN_COLOR nColor = UN_COLOR_RGBA(255, 255, 255, 255)) = 0;

		// 描画
		// 登録されていない場合は、登録を行う
		virtual UN_BOOL RenderImmediate(
			void* pStr,
			UN_INT nX, UN_INT nY,
			UN_COLOR nColor = UN_COLOR_RGBA(255, 255, 255, 255)) = 0;

		// ピクセル幅取得
		virtual UN_UINT GetWidth(void* pStr) = 0;

		// 登録済みかどうか
		virtual UN_BOOL IsRegistered(UN_UINT code) = 0;

		// ピクセル高さ取得
		virtual UN_UINT GetHeight() const = 0;

		// 文字コード取得
		virtual E_FONT_CHAR_ENCODE GetCharEncode() const = 0;

		// フォント描画ハンドラセット
		void SetFonrRenderHandler(RenderFontHandler* pHandler) { m_pRenderFontHadler = pHandler; }

	protected:
		RenderFontHandler* m_pRenderFontHadler;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_FONT_FONT_RENDERER_H__)
