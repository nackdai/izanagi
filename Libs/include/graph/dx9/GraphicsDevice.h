#if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_H__)
#define __IZANAGI_GRPAH_GRAPHICS_DEVICE_H__

#include "izDefs.h"
#include "izD3DDefs.h"
#include "izStd.h"
#include "RenderState.h"
#include "../internal/RenderTargetManager.h"
#include "../internal/ParamValueConverter.h"
#include "graph/GraphDefs.h"

namespace izanagi {
	class CBaseTexture;
	class CTexture;
	class CCubeTexture;
	class CVertexBuffer;
	class CIndexBuffer;
	class C2DRenderer;
	class CVertexShader;
	class CPixelShader;
	class CVertexDeclaration;
	struct SVertexElement;

	/**
	* 初期化パラメータ
	*/
	struct SGraphicsDeviceInitParams {
		HWND hFocusWindow;
		HWND hDeviceWindow;

		IZ_UINT BackBufferWidth;
		IZ_UINT BackBufferHeight;
		IZ_BOOL Windowed;
		D3DMULTISAMPLE_TYPE MultiSampleType;

		IZ_UINT Adapter;
		D3DDEVTYPE DeviceType;
		IZ_DWORD BehaviorFlags;

		D3DFORMAT DepthStencilFormat;

		IZ_UINT PresentationInterval;
	};

	/**
	* グラフィックスデバイス
	*/
	class CGraphicsDevice : public CObject {
		friend class CTexture;
		friend class CVertexBuffer;
		friend class CIndexBuffer;

	private:
		static CStandardMemoryAllocator s_cDeviceAllocator;
		static CGraphicsDevice* s_pInstance;

	public:
		// インスタンス作成
		static CGraphicsDevice* CreateGrapicsDevice(
			IZ_UINT nBufSize,
			void* pDeviceBuffer);

		// グラフィックスデバイス用メモリアロケータダンプ
		static inline void Dump();

	private:
		CGraphicsDevice();
		~CGraphicsDevice();

		CGraphicsDevice(const CGraphicsDevice& rhs);
		const CGraphicsDevice& operator=(const CGraphicsDevice& rhs);

	private:
		// 解放
		void InternalRelease();

	public:
		// ファイルからテクスチャ作成
		CTexture* CreateTextureFromFile(
			IZ_PCSTR lpszPathName,
			E_GRAPH_PIXEL_FMT fmt = E_GRAPH_PIXEL_FMT_NUM);

		// メモリからテクスチャ作成
		CTexture* CreateTextureFromMemory(
			void* pData,
			IZ_UINT nDataSize,
			E_GRAPH_PIXEL_FMT fmt);

		// テクスチャ作成
		CTexture* CreateTexture(
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			IZ_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			E_GRAPH_RSC_TYPE nRscType);

		// ファイルからキューブテクスチャ作成
		CCubeTexture* CreateCubeTextureFromFile(
			IZ_PCSTR lpszPathName,
			E_GRAPH_PIXEL_FMT fmt = E_GRAPH_PIXEL_FMT_NUM);

		// キューブテクスチャ作成
		CCubeTexture* CreateCubeTexture(
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			IZ_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			IZ_BOOL bIsDynamic);

		// レンダーターゲット作成
		CTexture* CreateRenderTarget(
			IZ_UINT nWidth, IZ_UINT nHeight,
			E_GRAPH_PIXEL_FMT fmt);
		
		// システムメモリ上にテクスチャ作成
		CTexture* CreateTextureOnSysMem(
			IZ_UINT nWidth, IZ_UINT nHeight,
			E_GRAPH_PIXEL_FMT fmt,
			IZ_UINT nMipLevel);

		// 頂点バッファ作成
		CVertexBuffer* CreateVertexBuffer(
			IZ_UINT nStride,
			IZ_UINT nVtxNum,
			E_GRAPH_RSC_TYPE nCreateType);

		// インデックスバッファ作成
		CIndexBuffer* CreateIndexBuffer(
			IZ_UINT nIdxNum,
			E_GRAPH_INDEX_BUFFER_FMT fmt,
			E_GRAPH_RSC_TYPE nCreateType);

		// 頂点シェーダ作成
		CVertexShader* CreateVertexShader(const void* pProgram);

		// ピクセルシェーダ作成
		CPixelShader* CreatePixelShader(const void* pProgram);

		// 頂点宣言作成
		CVertexDeclaration* CreateVertexDeclaration(const D3D_VTX_ELEMENT* pElem);
		CVertexDeclaration* CreateVertexDeclaration(const SVertexElement* pElem, IZ_UINT nNum);

	private:
		// リソース挿入
		template <class _T>
		_T* InsertResource(_T* p, _T* pListTop);

		// リソース削除
		template <class _T>
		_T* RemoveResource(_T* p, _T* pListTop);

		void RemoveTexture(CTexture* p);
		void RemoveVertexBuffer(CVertexBuffer* p);
		void RemoveIndexBuffer(CIndexBuffer* p);

	public:
		// リセット
		IZ_BOOL Reset(const SGraphicsDeviceInitParams& sParams);

	private:
		// 本体作成
		IZ_BOOL CreateBody(const SGraphicsDeviceInitParams& sParams);

		// リセット
		IZ_BOOL ResetInternal(const SGraphicsDeviceInitParams& sParams);

	private:
		template <class _T>
		inline void ReleaseResource(_T* pList);

		template <class _T>
		inline void ResetResource(_T* pList);

	public:
		// 描画開始
		IZ_BOOL BeginRender(
			IZ_DWORD nClearFlags,
			IZ_COLOR nClearColor,
			IZ_FLOAT fClearZ,
			IZ_DWORD nClearStencil);

		// 描画終了
		void EndRender();

		// クリア
		void Clear(
			IZ_DWORD nClearFlags,
			IZ_COLOR nClearColor,
			IZ_FLOAT fClearZ,
			IZ_DWORD nClearStencil);

		// シーン描画開始
		IZ_BOOL BeginScene(
			IZ_DWORD nClearFlags,
			IZ_COLOR nClearColor = 0,
			IZ_FLOAT fClearZ = 1.0f,
			IZ_DWORD nClearStencil = 0);
		IZ_BOOL BeginScene(
			CSurface** pRT,
			IZ_UINT nCount,
			IZ_DWORD nClearFlags,
			IZ_COLOR nClearColor = 0,
			IZ_FLOAT fClearZ = 1.0f,
			IZ_DWORD nClearStencil = 0);
		IZ_BOOL BeginScene(
			CSurface** pRT,
			IZ_UINT nCount,
			CSurface* pDepth,
			IZ_DWORD nClearFlags,
			IZ_COLOR nClearColor = 0,
			IZ_FLOAT fClearZ = 1.0f,
			IZ_DWORD nClearStencil = 0);

		// シーン描画終了
		void EndScene(IZ_UINT flag = 0xffffffff);

		// 同期
		IZ_BOOL Present();

		// 2D描画開始
		IZ_BOOL Begin2D();

		// 2D描画終了
		IZ_BOOL End2D();

		// 2D描画バッファフラッシュ
		IZ_BOOL Flush2D();

		// 2Dスプライト描画
		IZ_BOOL Draw2DSprite(
			const CFloatRect& rcSrc,
			const CIntRect& rcDst,
			const IZ_COLOR color = IZ_COLOR_RGBA(255, 255, 255, 255));
		IZ_BOOL Draw2DSpriteEx(
			const CIntRect& rcSrc,
			const CIntRect& rcDst,
			const IZ_COLOR color = IZ_COLOR_RGBA(255, 255, 255, 255));

		// 2D矩形描画
		IZ_BOOL Draw2DRect(
			const CIntRect& rcDst,
			const IZ_COLOR color);

		// 2Dライン描画
		IZ_BOOL Draw2DLine(
			const CIntPoint& ptStart,
			const CIntPoint& ptGoal,
			const IZ_COLOR color);

		// 2D描画モード
		void Set2DRenderOp(E_GRAPH_2D_RENDER_OP nOp);
		E_GRAPH_2D_RENDER_OP Get2DRenderOp() const;

	private:
		void ClearRenderState();

	public:
		// テクスチャセット
		IZ_BOOL SetTexture(IZ_UINT nStage, CBaseTexture* pTex);

		// テクスチャ取得
		CBaseTexture* GetTexture(IZ_UINT nStage);

	private:
		// レンダーターゲットセット
		IZ_BOOL PushRenderTarget(CSurface** pSurface, IZ_UINT num);
		void SetRenderTarget(CSurface** pSurface, IZ_UINT num);

		// 深度・ステンシルセット
		IZ_BOOL PushDepthStencil(CSurface* pSurface);
		void SetDepthStencil(CSurface* pSurface);

	public:
		// 頂点バッファセット
		IZ_BOOL SetVertexBuffer(
			IZ_UINT nStreamIdx,
			IZ_UINT nOffsetByte,
			IZ_UINT nStride,
			CVertexBuffer* pVB);

		// FVFセット
		IZ_BOOL SetFVF(IZ_DWORD dwFVF);

		// インデックスバッファセット
		IZ_BOOL SetIndexBuffer(CIndexBuffer* pIB);

		// インデックスバッファ描画
		IZ_BOOL DrawIndexedPrimitive(
			E_GRAPH_PRIM_TYPE prim_type,
			IZ_UINT nBaseIdx,
			IZ_UINT nMinIdx,
			IZ_UINT nVtxNum,
			IZ_UINT nStartIdx,
			IZ_UINT nPrimCnt);

		// インデックスバッファなし描画
		IZ_BOOL DrawPrimitive(
			E_GRAPH_PRIM_TYPE prim_type,
			IZ_UINT nStartIdx,
			IZ_UINT nPrimCnt);

	public:
		// 頂点シェーダセット
		IZ_BOOL SetVertexShader(CVertexShader* pVS);

		// ピクセルシェーダセット
		IZ_BOOL SetPixelShader(CPixelShader* pPS);

		// 頂点宣言セット
		IZ_BOOL SetVertexDeclaration(CVertexDeclaration* pVD);

	public:
		// ビューポートセット
		IZ_BOOL SetViewport(const SViewport& vp);

		// デフォルトのレンダーステートを設定
		void SetDefaultRenderState();

		// レンダーステート保存
		IZ_BOOL SaveRenderState();

		// レンダーステート復帰
		IZ_BOOL LoadRenderState();
		void SetRenderState(const S_RENDER_STATE& sRS);

	public:
		// ディスプレイモード取得
		inline const D3DDISPLAYMODE& GetDisplayMode() const;

		// プレゼントパラメータ取得
		inline const D3DPRESENT_PARAMETERS& GetPresentParam() const;

		// リフレッシュレート取得
		inline IZ_UINT GetRefreshRate() const;

		// サンプラステート取得
		inline const S_SAMPLER_STATE& GetSamplerState(IZ_UINT nStage) const;

		// レンダーステート取得
		inline const S_RENDER_STATE& GetRenderState() const;

		// ビューポート取得
		inline const SViewport& GetViewport() const;

		// シザー矩形
		inline void SetScissorTestRect(const SIntRect& rc);

		// レンダーステート設定
		inline void SetRenderState(E_GRAPH_RENDER_STATE nState, IZ_DWORD val);

	private:
		// テクスチャステージステート設定
		template <typename _TS, typename _T>
		inline void SetTextureStageState(
			IZ_DWORD stage,
			_TS nTSType,
			_T& old_val, _T new_val);

		// サンプラステート設定
		template <typename _SS, typename _T>
		inline void SetSamplerStateAddr(
			IZ_DWORD stage,
			_SS nSSType,
			_T& old_val, _T new_val);
		template <typename _SS, typename _T>
		inline void SetSamplerStateFilter(
			IZ_DWORD stage,
			_SS nSSType,
			_T& old_val, _T new_val);

	public:
		// レンダーターゲット取得
		inline CSurface* GetRenderTarget(IZ_UINT idx);
		inline CSurface* GetDepthSrencil();

		// リセット用コールバックセット
		inline void SetResetCallBack(GraphicsDeviceResetCallBack pCallBack);

		// デバイスロスト用コールバックセット
		inline void SetLostDeviceCallBack(GraphicsDeviceLostDeviceCallBack pCallBack);

	public:
		D3D_DEVICE* GetRawInterface() { return m_pDevice; }
		IMemoryAllocator* GetDeviceAllocator() { return m_pAllocator; }

	private:
		enum {
			// レンダーターゲットのキュー数
			RT_QUEUE_MAX = 4,
		};

	private:
		IMemoryAllocator* m_pAllocator;

		D3D_INST* m_pD3D;

		// 本体
		D3D_DEVICE* m_pDevice;

		D3DDISPLAYMODE m_sDisplayMode;
		D3DPRESENT_PARAMETERS m_sPresentParameters;
		HWND m_hFocusWindow;

		// テクスチャ
		CBaseTexture* m_pTexture[TEX_STAGE_NUM];
		S_SAMPLER_STATE m_sSamplerState[TEX_STAGE_NUM];

		// レンダーステート
		CRenderState m_RenderState;

		// フレームバッファ
		CSurface* m_pRT;
		CSurface* m_pDepth;

		// レンダーターゲット管理
		CRenderTargetManager<RT_QUEUE_MAX> m_cRTMgr[MAX_MRT_NUM];
		CRenderTargetManager<RT_QUEUE_MAX> m_cDepthMgr;

		// 2D描画用
		C2DRenderer* m_p2DRenderer;

		// リセット対策用
		CTexture* m_pResetTexture;
		CVertexBuffer* m_pResetVB;
		CIndexBuffer* m_pResetIB;

		// フラグ
		struct {
			IZ_UINT is_call_begin		: 1;	// BeginSceneを呼んだかどうか
			IZ_UINT is_render_2d		: 1;	// 2D描画中か
			IZ_UINT is_force_set_state	: 1;	// 強制的にステートを設定するかどうか
			IZ_UINT is_lost_device		: 1;	// デバイスロストしているかどうか
			IZ_UINT reserved			: 28;
		} m_Flags;

		// リセット用コールバック
		GraphicsDeviceResetCallBack m_pResetCallBack;

		// デバイスロスト用コールバック
		GraphicsDeviceLostDeviceCallBack m_pLostDeviceCallBack;
	};

	// inline ***************************

	/**
	* グラフィックスデバイス用メモリアロケータダンプ
	*/
	void CGraphicsDevice::Dump()
	{
		s_cDeviceAllocator.Dump();
	}

	/**
	* ディスプレイモード取得
	*/
	const D3DDISPLAYMODE& CGraphicsDevice::GetDisplayMode() const
	{
		return m_sDisplayMode;
	}

	/**
	* プレゼントパラメータ取得
	*/
	const D3DPRESENT_PARAMETERS& CGraphicsDevice::GetPresentParam() const
	{
		return m_sPresentParameters;
	}

	/**
	* リフレッシュレート取得
	*/
	IZ_UINT CGraphicsDevice::GetRefreshRate() const
	{
		return m_sDisplayMode.RefreshRate;
	}

	/**
	* サンプラステート取得
	*/
	const S_SAMPLER_STATE& CGraphicsDevice::GetSamplerState(IZ_UINT nStage) const
	{
		IZ_ASSERT(nStage < TEX_STAGE_NUM);
		return m_sSamplerState[nStage];
	}

	/**
	* レンダーステート取得
	* 非推奨
	*/
	const S_RENDER_STATE& CGraphicsDevice::GetRenderState() const
	{
		return m_RenderState;
	}

	/**
	* ビューポート取得
	*/
	const SViewport& CGraphicsDevice::GetViewport() const
	{
		return m_RenderState.vp;
	}

	/**
	* シザー矩形
	*/
	void CGraphicsDevice::SetScissorTestRect(const SIntRect& rc)
	{
		// 念のためこれくらいはチェックするか
		IZ_C_ASSERT(sizeof(RECT) == sizeof(CIntRect));

		if (m_RenderState.rcScissor != rc) {
			m_RenderState.rcScissor = rc;
			m_pDevice->SetScissorRect(reinterpret_cast<RECT*>(&m_RenderState.rcScissor));
		}
	}

	// テクスチャステージステート設定
	template <typename _TS, typename _T>
	void CGraphicsDevice::SetTextureStageState(
		IZ_DWORD stage,
		_TS nTSType,
		_T& old_val, _T new_val)
	{
		if (m_Flags.is_force_set_state) {
			m_pDevice->SetTextureStageState(stage, nTSType, new_val);
			old_val = new_val;
		}
		else {
			if (old_val != new_val) {
				m_pDevice->SetTextureStageState(stage, nTSType, new_val);
				old_val = new_val;
			}
		}
	}

	// サンプラステート設定
	template <typename _SS, typename _T>
	void CGraphicsDevice::SetSamplerStateAddr(
		IZ_DWORD stage,
		_SS nSSType,
		_T& old_val, _T new_val)
	{
		if (m_Flags.is_force_set_state) {
			m_pDevice->SetSamplerState(
				stage, 
				IZ_GET_TARGET_SAMPLER_STATE_TYPE(nSSType),
				IZ_GET_TARGET_TEX_ADDR(new_val));

			old_val = new_val;
		}
		else {
			if (old_val != new_val) {
				m_pDevice->SetSamplerState(
					stage, 
					IZ_GET_TARGET_SAMPLER_STATE_TYPE(nSSType), 
					IZ_GET_TARGET_TEX_ADDR(new_val));

				old_val = new_val;
			}
		}
	}

	// サンプラステート設定
	template <typename _SS, typename _T>
	void CGraphicsDevice::SetSamplerStateFilter(
		IZ_DWORD stage,
		_SS nSSType,
		_T& old_val, _T new_val)
	{
		if (m_Flags.is_force_set_state) {
			m_pDevice->SetSamplerState(
				stage, 
				IZ_GET_TARGET_SAMPLER_STATE_TYPE(nSSType),
				IZ_GET_TARGET_TEX_FILTER(new_val));

			old_val = new_val;
		}
		else {
			if (old_val != new_val) {
				m_pDevice->SetSamplerState(
					stage, 
					IZ_GET_TARGET_SAMPLER_STATE_TYPE(nSSType), 
					IZ_GET_TARGET_TEX_FILTER(new_val));

				old_val = new_val;
			}
		}
	}

	/**
	* レンダーステート設定
	*/
	void CGraphicsDevice::SetRenderState(E_GRAPH_RENDER_STATE nState, IZ_DWORD val)
	{
		m_RenderState.SetRenderState(this, nState, val);
	}

	/**
	* レンダーターゲット取得
	*/
	CSurface* CGraphicsDevice::GetRenderTarget(IZ_UINT idx)
	{
		IZ_ASSERT(idx < MAX_MRT_NUM);
		return m_RenderState.curRT[idx];
	}

	/**
	* 深度・ステンシル取得
	*/
	CSurface* CGraphicsDevice::GetDepthSrencil()
	{
		return m_RenderState.curDepth;
	}

	/**
	* リセット用コールバックセット
	*/
	void CGraphicsDevice::SetResetCallBack(GraphicsDeviceResetCallBack pCallBack)
	{
		m_pResetCallBack = pCallBack;
	}

	/**
	* デバイスロスト用コールバックセット
	*/
	void CGraphicsDevice::SetLostDeviceCallBack(GraphicsDeviceLostDeviceCallBack pCallBack)
	{
		m_pLostDeviceCallBack = pCallBack;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_H__)
