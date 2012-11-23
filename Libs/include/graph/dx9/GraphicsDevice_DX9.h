#if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_DX9_H__)
#define __IZANAGI_GRPAH_GRAPHICS_DEVICE_DX9_H__

#include "izD3DDefs.h"
#include "graph/GraphicsDevice.h"
#include "graph/RenderState.h"
#include "graph/internal/ParamValueConverter.h"

namespace izanagi
{
namespace graph
{
    class CTextureDX9;
    class CVertexBufferDX9;
    class CIndexBufferDX9;

	// グラフィックスデバイス
	class CGraphicsDeviceDX9 : public CGraphicsDevice
    {
        friend class CGraphicsDevice;
        friend class CTextureDX9;
        friend class CVertexBufferDX9;
        friend class CIndexBufferDX9;

	private:
		CGraphicsDeviceDX9();
		virtual ~CGraphicsDeviceDX9();

    private:
        void ClearRenderState();

	public:
		// メモリからテクスチャ作成
		virtual CTexture* CreateTextureFromMemory(
			void* pData,
			IZ_UINT nDataSize,
			E_GRAPH_PIXEL_FMT fmt);

		// テクスチャ作成
		virtual CTexture* CreateTexture(
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			IZ_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			E_GRAPH_RSC_TYPE nRscType);

		// キューブテクスチャ作成
		virtual CCubeTexture* CreateCubeTexture(
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			IZ_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			IZ_BOOL bIsDynamic);

		// レンダーターゲット作成
		virtual CTexture* CreateRenderTarget(
			IZ_UINT nWidth, IZ_UINT nHeight,
			E_GRAPH_PIXEL_FMT fmt);

		// 深度・ステンシルサーフェス作成
		virtual CSurface* CreateDepthStencilSurface(
			IZ_UINT width, 
			IZ_UINT height,
			E_GRAPH_PIXEL_FMT fmt);
		
		// システムメモリ上にテクスチャ作成
		virtual CTexture* CreateTextureOnSysMem(
			IZ_UINT nWidth, IZ_UINT nHeight,
			E_GRAPH_PIXEL_FMT fmt,
			IZ_UINT nMipLevel);

		// 頂点バッファ作成
		virtual CVertexBuffer* CreateVertexBuffer(
			IZ_UINT nStride,
			IZ_UINT nVtxNum,
			E_GRAPH_RSC_TYPE nCreateType);

		// インデックスバッファ作成
		virtual CIndexBuffer* CreateIndexBuffer(
			IZ_UINT nIdxNum,
			E_GRAPH_INDEX_BUFFER_FMT fmt,
			E_GRAPH_RSC_TYPE nCreateType);

		// 頂点シェーダ作成
		virtual CVertexShader* CreateVertexShader(const void* pProgram);

		// ピクセルシェーダ作成
		virtual CPixelShader* CreatePixelShader(const void* pProgram);

		// 頂点宣言作成
		virtual CVertexDeclaration* CreateVertexDeclaration(const SVertexElement* pElem, IZ_UINT nNum);

	private:
		// リソース挿入
		template <class _T>
		_T* InsertResource(_T* p, _T* pListTop);

		// リソース削除
		template <class _T>
		_T* RemoveResource(_T* p, _T* pListTop);

        template <class _T>
		void DisableResource(_T* pList);

		template <class _T>
		void RestoreResource(_T* pList);

		void RemoveTexture(CTextureDX9* p);
		void RemoveVertexBuffer(CVertexBufferDX9* p);
		void RemoveIndexBuffer(CIndexBufferDX9* p);

        // 本体作成
		IZ_BOOL CreateBody(const SGraphicsDeviceInitParams& sParams);

		// リセット
		IZ_BOOL ResetInternal(const SGraphicsDeviceInitParams& sParams);

	public:
		// リセット
		virtual IZ_BOOL Reset(const void* initialParam);
		
		// 描画開始
		virtual IZ_BOOL BeginRender(
			IZ_DWORD nClearFlags,
			IZ_COLOR nClearColor,
			IZ_FLOAT fClearZ,
			IZ_DWORD nClearStencil);

		// 描画終了
		virtual void EndRender();

		// クリア
		virtual void Clear(
			IZ_DWORD nClearFlags,
			IZ_COLOR nClearColor,
			IZ_FLOAT fClearZ,
			IZ_DWORD nClearStencil);

		virtual IZ_BOOL BeginScene(
			CSurface** pRT,
			IZ_UINT nCount,
			CSurface* pDepth,
			IZ_DWORD nClearFlags,
			IZ_COLOR nClearColor = 0,
			IZ_FLOAT fClearZ = 1.0f,
			IZ_DWORD nClearStencil = 0);

		// シーン描画終了
		virtual void EndScene(IZ_UINT flag = 0xffffffff);

		// 同期
		virtual IZ_BOOL Present();

	public:
		// 頂点バッファセット
		virtual IZ_BOOL SetVertexBuffer(
			IZ_UINT nStreamIdx,
			IZ_UINT nOffsetByte,
			IZ_UINT nStride,
			CVertexBuffer* pVB);

		// インデックスバッファセット
		virtual IZ_BOOL SetIndexBuffer(CIndexBuffer* pIB);

        // 頂点シェーダセット
		virtual IZ_BOOL SetVertexShader(CVertexShader* pVS);

		// ピクセルシェーダセット
		virtual IZ_BOOL SetPixelShader(CPixelShader* pPS);

		// 頂点宣言セット
		virtual IZ_BOOL SetVertexDeclaration(CVertexDeclaration* pVD);

		// インデックスバッファ描画
		virtual IZ_BOOL DrawIndexedPrimitive(
			E_GRAPH_PRIM_TYPE prim_type,
			IZ_UINT nBaseIdx,
			IZ_UINT nMinIdx,
			IZ_UINT nVtxNum,
			IZ_UINT nStartIdx,
			IZ_UINT nPrimCnt);

		// インデックスバッファなし描画
		virtual IZ_BOOL DrawPrimitive(
			E_GRAPH_PRIM_TYPE prim_type,
			IZ_UINT nStartIdx,
			IZ_UINT nPrimCnt);		

	public:
		// ビューポートセット
		virtual IZ_BOOL SetViewport(const SViewport& vp);

		// デフォルトのレンダーステートを設定
		virtual void SetDefaultRenderState();

		virtual void SetRenderState(const S_RENDER_STATE& sRS);

	private:
		// テクスチャステージステート設定
	    template <typename _TS, typename _T>
	    void SetTextureStageState(
		    IZ_DWORD stage,
		    _TS nTSType,
		    _T& old_val, _T new_val)
	    {
		    if (m_Flags.is_force_set_state) {
			    m_Device->SetTextureStageState(stage, nTSType, new_val);
			    old_val = new_val;
		    }
		    else {
			    if (old_val != new_val) {
				    m_Device->SetTextureStageState(stage, nTSType, new_val);
				    old_val = new_val;
			    }
		    }
	    }

	    // サンプラステート設定
	    template <typename _SS, typename _T>
	    void SetSamplerStateAddr(
		    IZ_DWORD stage,
		    _SS nSSType,
		    _T& old_val, _T new_val)
	    {
		    if (m_Flags.is_force_set_state) {
			    m_Device->SetSamplerState(
				    stage, 
				    IZ_GET_TARGET_SAMPLER_STATE_TYPE(nSSType),
				    IZ_GET_TARGET_TEX_ADDR(new_val));

			    old_val = new_val;
		    }
		    else {
			    if (old_val != new_val) {
				    m_Device->SetSamplerState(
					    stage, 
					    IZ_GET_TARGET_SAMPLER_STATE_TYPE(nSSType), 
					    IZ_GET_TARGET_TEX_ADDR(new_val));

				    old_val = new_val;
			    }
		    }
	    }

	    // サンプラステート設定
	    template <typename _SS, typename _T>
	    void SetSamplerStateFilter(
		    IZ_DWORD stage,
		    _SS nSSType,
		    _T& old_val, _T new_val)
	    {
		    if (m_Flags.is_force_set_state) {
			    m_Device->SetSamplerState(
				    stage, 
				    IZ_GET_TARGET_SAMPLER_STATE_TYPE(nSSType),
				    IZ_GET_TARGET_TEX_FILTER(new_val));

			    old_val = new_val;
		    }
		    else {
			    if (old_val != new_val) {
				    m_Device->SetSamplerState(
					    stage, 
					    IZ_GET_TARGET_SAMPLER_STATE_TYPE(nSSType), 
					    IZ_GET_TARGET_TEX_FILTER(new_val));

				    old_val = new_val;
			    }
		    }
	    }

	public:
		// リセット用コールバックセット
		virtual void SetResetCallBack(GraphicsDeviceResetCallBack pCallBack)
        {
            m_ResetCallBack = pCallBack;
	    }


		// デバイスロスト用コールバックセット
		virtual void SetLostDeviceCallBack(GraphicsDeviceLostDeviceCallBack pCallBack)
        {
		    m_LostDeviceCallBack = pCallBack;
	    }

        virtual IZ_UINT GetBackBufferWidth() const
        {
            return m_PresentParameters.BackBufferWidth;
        }

        virtual IZ_UINT GetBackBufferHeight() const
        {
            return m_PresentParameters.BackBufferHeight;
        }

        virtual void* GetPlatformInterface() { return GetRawInterface(); }

        // ディスプレイモード取得
		const D3DDISPLAYMODE& GetDisplayMode() const
        {
            return m_DisplayMode;
        }

		// プレゼントパラメータ取得
		const D3DPRESENT_PARAMETERS& GetPresentParam() const
        {
            return m_PresentParameters;
        }

		// リフレッシュレート取得
		IZ_UINT GetRefreshRate() const
        {
            return m_DisplayMode.RefreshRate;
        }

    private:
        virtual IZ_BOOL SetTextureInternal(IZ_UINT nStage, CBaseTexture* pTex);
        virtual void SetRenderTargetInternal(CSurface** pSurface, IZ_UINT num);

		// 深度・ステンシルセット
		virtual void SetDepthStencil(CSurface* pSurface);

	public:
		virtual D3D_DEVICE* GetRawInterface() { return m_Device; }

	private:
		D3D_INST* m_D3D;

		// 本体
		D3D_DEVICE* m_Device;

		D3DDISPLAYMODE m_DisplayMode;
		D3DPRESENT_PARAMETERS m_PresentParameters;
		HWND m_hFocusWindow;

		// リセット対策用
		CTextureDX9* m_ResetTexture;
		CVertexBufferDX9* m_ResetVB;
		CIndexBufferDX9* m_ResetIB;

		// リセット用コールバック
		GraphicsDeviceResetCallBack m_ResetCallBack;

		// デバイスロスト用コールバック
		GraphicsDeviceLostDeviceCallBack m_LostDeviceCallBack;
	};
}   // namespace graph
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_DX9_H__)
