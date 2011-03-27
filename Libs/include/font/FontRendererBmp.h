#if !defined(__IZANAGI_FONT_FONT_RENDERER_BMP_H__)
#define __IZANAGI_FONT_FONT_RENDERER_BMP_H__

#include "izDefs.h"
#include "FNTFormat.h"
#include "FontRenderer.h"

namespace izanagi {
	class IInputStream;
	class IMemoryAllocator;
	class CGraphicsDevice;
	class CTexture;

	/**
	* フォントレンダラ
	*/
	class CFontRendererBmp : public CFontRenderer {
		friend class CFontRenderer;

	private:
#if 0
		static const E_GRAPH_PIXEL_FMT TEX_FORMAT = E_GRAPH_PIXEL_FMT_RGBA8;
		static const IZ_UINT TEX_BPP = 4;
		static const E_GRAPH_2D_RENDER_OP RENDER_2D_OP = E_GRAPH_2D_RENDER_OP_MODULATE;
#else
		static const E_GRAPH_PIXEL_FMT TEX_FORMAT = E_GRAPH_PIXEL_FMT_A8;
		static const IZ_UINT TEX_BPP = 1;
		static const E_GRAPH_2D_RENDER_OP RENDER_2D_OP = E_GRAPH_2D_RENDER_OP_MODULATE_ALPHA;
#endif

		// 登録情報
		struct SRegInfo;

	private:
		// インスタンス作成
		static CFontRenderer* CreateFontRendererBmp(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IZ_UINT	nMaxRegisterNum,
			IInputStream* in);

	private:
		CFontRendererBmp();
		~CFontRendererBmp();

		NO_COPIABLE(CFontRendererBmp);

	private:
		// 解放
		void InternalRelease();

		// テクスチャ作成
		IZ_BOOL CreateTexture(IInputStream* in);

	public:
		// 登録
		IZ_BOOL Register(void* pStr);

		// 描画
		// 登録されていない場合は、描画しない
		void Render(
			void* pStr,
			IZ_INT nX, IZ_INT nY,
			IZ_COLOR nColor = IZ_COLOR_RGBA(255, 255, 255, 255));

		// 描画
		// 登録されていない場合は、登録を行う
		IZ_BOOL RenderImmediate(
			void* pStr,
			IZ_INT nX, IZ_INT nY,
			IZ_COLOR nColor = IZ_COLOR_RGBA(255, 255, 255, 255));

		// ピクセル幅取得
		IZ_UINT GetWidth(void* pStr);

		// 登録済みかどうか
		IZ_BOOL IsRegistered(IZ_UINT code);

		// ピクセル高さ取得
		IZ_UINT GetHeight() const { return m_sHeader.fontHeight; }

		// 文字コード取得
		E_FONT_CHAR_ENCODE GetCharEncode() const { return m_sHeader.charEncode; }

	private:
		// フォント登録
		SRegInfo* RegisterInternal(IZ_UINT code);

		// フォント登録開始
		IZ_BOOL BeginRegister();

		// フォント登録終了
		void EndRegister();

		// 登録情報取得
		SRegInfo* GetRegInfo(IZ_UINT code);

		// マップ情報取得
		const S_FNT_MAP* GetFontMap(IZ_UINT code);

		// 転送元テクスチャにフォント描画
		void RenderToSrcTex(
			SRegInfo* pRegInfo,
			IZ_UINT code,
			IZ_BOOL bRegister);

		// 転送
		IZ_BOOL UpdateSurface();

		// フォント描画
		void RenderInternal(
			IZ_UINT code,
			CIntPoint* pDstPoint,
			IZ_COLOR nColor);

	private:
		enum {
			HASH_SIZE = 5,
		};

		typedef CStdHash<IZ_UINT, SRegInfo, HASH_SIZE>	CFntHash;
		typedef CFntHash::Item			CFntHashItem;
		typedef CStdList<CFntHashItem>	CFntOrderList;

		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		// ヘッダ
		S_FNT_HEADER m_sHeader;

		// フォントマップ情報
		S_FNT_MAP* m_pMapList;

		// フォントイメージ情報
		S_FNT_IMAGE* m_pImageList;

		// フォントテクスチャ
		CTexture* m_pSrcTex;
		CTexture* m_pDstTex;

		// イメージ情報
		struct {
			IZ_UINT16 x;
			IZ_UINT16 y;

			IZ_UINT8* data;
			IZ_UINT32 pitch;
		} m_sImageInfo;

		// 最大登録可能文字数
		IZ_UINT m_nMaxRegisterNum;

		// 登録数
		IZ_UINT m_nRegNum;

		// 登録情報
		SRegInfo* m_pRegList;

		CFntHash m_FontHash;

		struct {
			IZ_UINT isNeedUpdateSurface	: 1;	// UpdateSurfaceが必要かどうか
		} m_Flags;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_FONT_FONT_RENDERER_BMP_H__)
