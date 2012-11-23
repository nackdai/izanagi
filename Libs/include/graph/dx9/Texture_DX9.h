#if !defined(__IZANAGI_GRAPH_TEXTURE_DX9_H__)
#define __IZANAGI_GRAPH_TEXTURE_DX9_H__

#include "graph/Texture.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceDX9;
    class CSurfaceDX9;

	class CTextureDX9 : public CTexture
    {
        friend class CGraphicsDeviceDX9;

	private:
		// ファイルからテクスチャ作成
		static CTextureDX9* CreateTextureFromFile(
			CGraphicsDeviceDX9* device,
			IMemoryAllocator* allocator,
			IZ_PCSTR path,
			E_GRAPH_PIXEL_FMT fmt);

		// データからテクスチャ作成
		static CTextureDX9* CreateTextureFromMemory(
			CGraphicsDeviceDX9* device,
			IMemoryAllocator* allocator,
			const void* data,
			IZ_UINT dataSize,
			E_GRAPH_PIXEL_FMT fmt);

		// テクスチャ作成
		static CTextureDX9* CreateTexture(
			CGraphicsDeviceDX9* device,
			IMemoryAllocator* allocator,
			IZ_UINT width,
			IZ_UINT height,
			IZ_UINT mipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			E_GRAPH_RSC_TYPE rscType,
			IZ_BOOL isOnSysMem = IZ_FALSE);

		// レンダーターゲット作成
		static CTextureDX9* CreateRenderTarget(
			CGraphicsDeviceDX9* device,
			IMemoryAllocator* allocator,
			IZ_UINT width,
			IZ_UINT height,
			E_GRAPH_PIXEL_FMT fmt);

	private:
		inline CTextureDX9();
		virtual inline ~CTextureDX9();

	private:
		template <typename _T, typename _Func>
		static CTextureDX9* CreateBody_From(
			CGraphicsDeviceDX9* device,
			IMemoryAllocator* allocator,
			_T tInput,
			IZ_UINT inputSize,
			_Func& func,
			E_GRAPH_PIXEL_FMT fmt);

		// 本体作成（テクスチャ）
		IZ_BOOL CreateBody_Texture(
			IZ_UINT width,
			IZ_UINT height,
			IZ_UINT mipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			E_GRAPH_RSC_TYPE createType,
			IZ_BOOL isOnSysMem);

		// 本体作成（レンダーターゲット）
		IZ_BOOL CreateBody_RenderTarget(
			IZ_UINT width,
			IZ_UINT height,
			E_GRAPH_PIXEL_FMT fmt,
			IZ_UINT mipLevel);

		// テクスチャ情報取得
		void GetTextureInfo();

		// サーフェス作成
		IZ_BOOL CreateSurface();

	public:
		// ロック
		virtual IZ_UINT Lock(
			UINT level,
			void** data,
			IZ_BOOL isReadOnly,
			IZ_BOOL isDiscard = IZ_FALSE);

		// アンロック
		virtual IZ_BOOL Unlock(IZ_UINT level);

    public:
        virtual IZ_BOOL IsPrepared() const;

        virtual IZ_BOOL Disable();

        virtual IZ_BOOL Restore();

	private:
        // サーフェス取得
		virtual CSurface* GetSurface(IZ_UINT idx);

		virtual TEX_HANDLE GetTexHandle()
        {
            return m_Texture;
        }

	public:
		D3D_TEXTURE* GetRawInterface() { return m_Texture; }

	private:
        CGraphicsDeviceDX9* m_Device;

		// 本体
		D3D_TEXTURE* m_Texture;

        // サーフェス
		CSurfaceDX9** m_Surface;

		CTextureDX9* m_Next;
	};
}   // namespace graph
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_TEXTURE_DX9_H__)
