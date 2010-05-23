#if !defined(__URANUS_GRPAH_GRAPHICS_DEVICE_H__)
#define __URANUS_GRPAH_GRAPHICS_DEVICE_H__

//////////////////////////////////////////
// For OpenGL
//////////////////////////////////////////

#include "unDefs.h"
#include "unD3DDefs.h"
#include "unStd.h"
#include "RenderState.h"
#include "../internal/RenderTargetManager.h"
#include "../internal/ParamValueConverter.h"
#include "graph/GraphDefs.h"

namespace uranus {
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

		UN_UINT BackBufferWidth;
		UN_UINT BackBufferHeight;
		UN_BOOL Windowed;
		D3DMULTISAMPLE_TYPE MultiSampleType;

		UN_UINT Adapter;
		D3DDEVTYPE DeviceType;
		UN_DWORD BehaviorFlags;

		D3DFORMAT DepthStencilFormat;

		UN_UINT PresentationInterval;
	};

	/**
	* グラフィックスデバイス
	*/
	class CGraphicsDevice : public CObject {
		friend class CTexture;
		friend class CVertexBuffer;
		friend class CIndexBuffer;

	private:
		static CMemoryAllocator s_cDeviceAllocator;
		static CGraphicsDevice* s_pInstance;

	public:
		// インスタンス作成
		static CGraphicsDevice* CreateGrapicsDevice(
			UN_UINT nBufSize,
			void* pDeviceBuffer);

		// グラフィックスデバイス用メモリアロケータダンプ
		static inline void Dump();

	private:
		CGraphicsDevice();
		~CGraphicsDevice();

		NO_COPIABLE(CGraphicsDevice);

	private:
		// 解放
		void InternalRelease();

	public:
		// ファイルからテクスチャ作成
		CTexture* CreateTextureFromFile(
			UN_PCSTR lpszPathName,
			E_GRAPH_PIXEL_FMT fmt = E_GRAPH_PIXEL_FMT_NUM);

		// メモリからテクスチャ作成
		CTexture* CreateTextureFromMemory(
			void* pData,
			UN_UINT nDataSize,
			E_GRAPH_PIXEL_FMT fmt);

		// テクスチャ作成
		CTexture* CreateTexture(
			UN_UINT nWidth,
			UN_UINT nHeight,
			UN_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			E_GRAPH_RSC_CREATE_TYPE nCreateType);

		// ファイルからキューブテクスチャ作成
		CCubeTexture* CreateCubeTextureFromFile(
			UN_PCSTR lpszPathName,
			E_GRAPH_PIXEL_FMT fmt = E_GRAPH_PIXEL_FMT_NUM);

		// キューブテクスチャ作成
		CCubeTexture* CreateCubeTexture(
			UN_UINT nWidth,
			UN_UINT nHeight,
			UN_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			UN_BOOL bIsDynamic);

		// レンダーターゲット作成
		CTexture* CreateRenderTarget(
			UN_UINT nWidth, UN_UINT nHeight,
			E_GRAPH_PIXEL_FMT fmt);
		
		// システムメモリ上にテクスチャ作成
		CTexture* CreateTextureOnSysMem(
			UN_UINT nWidth, UN_UINT nHeight,
			E_GRAPH_PIXEL_FMT fmt,
			UN_UINT nMipLevel);

		// 頂点バッファ作成
		CVertexBuffer* CreateVertexBuffer(
			UN_UINT nStride,
			UN_UINT nVtxNum,
			E_GRAPH_RSC_CREATE_TYPE nCreateType);

		// インデックスバッファ作成
		CIndexBuffer* CreateIndexBuffer(
			UN_UINT nIdxNum,
			E_GRAPH_INDEX_BUFFER_FMT fmt,
			E_GRAPH_RSC_CREATE_TYPE nCreateType);

		// 頂点シェーダ作成
		CVertexShader* CreateVertexShader(const void* pProgram);

		// ピクセルシェーダ作成
		CPixelShader* CreatePixelShader(const void* pProgram);

		// 頂点宣言作成
		CVertexDeclaration* CreateVertexDeclaration(const D3D_VTX_ELEMENT* pElem);
		CVertexDeclaration* CreateVertexDeclaration(const SVertexElement* pElem, UN_UINT nNum);

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
		UN_BOOL Reset(const SGraphicsDeviceInitParams& sParams);

	private:
		// 本体作成
		UN_BOOL CreateBody(const SGraphicsDeviceInitParams& sParams);

		// リセット
		UN_BOOL ResetInternal(const SGraphicsDeviceInitParams& sParams);

	private:
		template <class _T>
		inline void ReleaseResource(_T* pList);

		template <class _T>
		inline void ResetResource(_T* pList);

	public:
		// 描画開始
		UN_BOOL BeginRender(
			UN_DWORD nClearFlags,
			UN_COLOR nClearColor,
			UN_FLOAT fClearZ,
			UN_DWORD nClearStencil);

		// 描画終了
		void EndRender();

		// クリア
		void Clear(
			UN_DWORD nClearFlags,
			UN_COLOR nClearColor,
			UN_FLOAT fClearZ,
			UN_DWORD nClearStencil);

		// シーン描画開始
		UN_BOOL BeginScene(
			UN_DWORD nClearFlags,
			UN_COLOR nClearColor = 0,
			UN_FLOAT fClearZ = 1.0f,
			UN_DWORD nClearStencil = 0);
		UN_BOOL BeginScene(
			CSurface** pRT,
			UN_UINT nCount,
			UN_DWORD nClearFlags,
			UN_COLOR nClearColor = 0,
			UN_FLOAT fClearZ = 1.0f,
			UN_DWORD nClearStencil = 0);
		UN_BOOL BeginScene(
			CSurface** pRT,
			UN_UINT nCount,
			CSurface* pDepth,
			UN_DWORD nClearFlags,
			UN_COLOR nClearColor = 0,
			UN_FLOAT fClearZ = 1.0f,
			UN_DWORD nClearStencil = 0);

		// シーン描画終了
		void EndScene(UN_UINT flag = 0xffffffff);

		// 同期
		UN_BOOL Present();

		// 2D描画開始
		UN_BOOL Begin2D();

		// 2D描画終了
		UN_BOOL End2D();

		// 2D描画バッファフラッシュ
		UN_BOOL Flush2D();

		// 2Dスプライト描画
		UN_BOOL Draw2DSprite(
			const CFloatRect& rcSrc,
			const CIntRect& rcDst,
			const UN_COLOR color = UN_COLOR_RGBA(255, 255, 255, 255));
		UN_BOOL Draw2DSpriteEx(
			const CIntRect& rcSrc,
			const CIntRect& rcDst,
			const UN_COLOR color = UN_COLOR_RGBA(255, 255, 255, 255));

		// 2D矩形描画
		UN_BOOL Draw2DRect(
			const CIntRect& rcDst,
			const UN_COLOR color);

		// 2Dライン描画
		UN_BOOL Draw2DLine(
			const CIntPoint& ptStart,
			const CIntPoint& ptGoal,
			const UN_COLOR color);

		// 2D描画モード
		void Set2DRenderOp(E_GRAPH_2D_RENDER_OP nOp);
		E_GRAPH_2D_RENDER_OP Get2DRenderOp() const;

	private:
		void ClearRenderState();

	public:
		// テクスチャセット
		UN_BOOL SetTexture(UN_UINT nStage, CBaseTexture* pTex);

		// テクスチャ取得
		CBaseTexture* GetTexture(UN_UINT nStage);

	private:
		// レンダーターゲットセット
		UN_BOOL PushRenderTarget(CSurface** pSurface, UN_UINT num);
		void SetRenderTarget(CSurface** pSurface, UN_UINT num);

		// 深度・ステンシルセット
		UN_BOOL PushDepthStencil(CSurface* pSurface);
		void SetDepthStencil(CSurface* pSurface);

	public:
		// 頂点バッファセット
		UN_BOOL SetVertexBuffer(
			UN_UINT nStreamIdx,
			UN_UINT nOffsetByte,
			UN_UINT nStride,
			CVertexBuffer* pVB);

		// FVFセット
		UN_BOOL SetFVF(UN_DWORD dwFVF);

		// インデックスバッファセット
		UN_BOOL SetIndexBuffer(CIndexBuffer* pIB);

		// インデックスバッファ描画
		UN_BOOL DrawIndexedPrimitive(
			E_GRAPH_PRIM_TYPE prim_type,
			UN_UINT nBaseIdx,
			UN_UINT nMinIdx,
			UN_UINT nVtxNum,
			UN_UINT nStartIdx,
			UN_UINT nPrimCnt);

		// インデックスバッファなし描画
		UN_BOOL DrawPrimitive(
			E_GRAPH_PRIM_TYPE prim_type,
			UN_UINT nStartIdx,
			UN_UINT nPrimCnt);

	public:
		// 頂点シェーダセット
		UN_BOOL SetVertexShader(CVertexShader* pVS);

		// ピクセルシェーダセット
		UN_BOOL SetPixelShader(CPixelShader* pPS);

		// 頂点宣言セット
		UN_BOOL SetVertexDeclaration(CVertexDeclaration* pVD);

	public:
		// ビューポートセット
		UN_BOOL SetViewport(const SViewport& vp);

		// デフォルトのレンダーステートを設定
		void SetDefaultRenderState();

		// レンダーステート保存
		UN_BOOL SaveRenderState();

		// レンダーステート復帰
		UN_BOOL LoadRenderState();
		void SetRenderState(const S_RENDER_STATE& sRS);

	public:
		// ディスプレイモード取得
		inline const D3DDISPLAYMODE& GetDisplayMode() const;

		// プレゼントパラメータ取得
		inline const D3DPRESENT_PARAMETERS& GetPresentParam() const;

		// リフレッシュレート取得
		inline UN_UINT GetRefreshRate() const;

		// サンプラステート取得
		inline const S_SAMPLER_STATE& GetSamplerState(UN_UINT nStage) const;

		// レンダーステート取得
		inline const S_RENDER_STATE& GetRenderState() const;

		// ビューポート取得
		inline const SViewport& GetViewport() const;

		// シザー矩形
		inline void SetScissorTestRect(const SIntRect& rc);

		// レンダーステート設定
		inline void SetRenderState(E_GRAPH_RENDER_STATE nState, UN_DWORD val);

	private:
		// テクスチャステージステート設定
		template <typename _TS, typename _T>
		inline void SetTextureStageState(
			UN_DWORD stage,
			_TS nTSType,
			_T& old_val, _T new_val);

		// サンプラステート設定
		template <typename _SS, typename _T>
		inline void SetSamplerStateAddr(
			UN_DWORD stage,
			_SS nSSType,
			_T& old_val, _T new_val);
		template <typename _SS, typename _T>
		inline void SetSamplerStateFilter(
			UN_DWORD stage,
			_SS nSSType,
			_T& old_val, _T new_val);

	public:
		// レンダーターゲット取得
		inline CSurface* GetRenderTarget(UN_UINT idx);
		inline CSurface* GetDepthSrencil();

		// リセット用コールバックセット
		inline void SetResetCallBack(GraphicsDeviceResetCallBack pCallBack);

		// デバイスロスト用コールバックセット
		inline void SetLostDeviceCallBack(GraphicsDeviceLostDeviceCallBack pCallBack);

	public:
		D3D_DEVICE* GetRawInterface() { return m_pDevice; }

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
			UN_UINT is_call_begin		: 1;	// BeginSceneを呼んだかどうか
			UN_UINT is_render_2d		: 1;	// 2D描画中か
			UN_UINT is_force_set_state	: 1;	// 強制的にステートを設定するかどうか
			UN_UINT is_lost_device		: 1;	// デバイスロストしているかどうか
			UN_UINT reserved			: 28;
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
	UN_UINT CGraphicsDevice::GetRefreshRate() const
	{
		return m_sDisplayMode.RefreshRate;
	}

	/**
	* サンプラステート取得
	*/
	const S_SAMPLER_STATE& CGraphicsDevice::GetSamplerState(UN_UINT nStage) const
	{
		UN_ASSERT(nStage < TEX_STAGE_NUM);
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
		UN_C_ASSERT(sizeof(RECT) == sizeof(CIntRect));

		if (m_RenderState.rcScissor != rc) {
			m_RenderState.rcScissor = rc;
			m_pDevice->SetScissorRect(reinterpret_cast<RECT*>(&m_RenderState.rcScissor));
		}
	}

	// テクスチャステージステート設定
	template <typename _TS, typename _T>
	void CGraphicsDevice::SetTextureStageState(
		UN_DWORD stage,
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
		UN_DWORD stage,
		_SS nSSType,
		_T& old_val, _T new_val)
	{
		if (m_Flags.is_force_set_state) {
			m_pDevice->SetSamplerState(
				stage, 
				UN_GET_TARGET_SAMPLER_STATE_TYPE(nSSType),
				UN_GET_TARGET_TEX_ADDR(new_val));

			old_val = new_val;
		}
		else {
			if (old_val != new_val) {
				m_pDevice->SetSamplerState(
					stage, 
					UN_GET_TARGET_SAMPLER_STATE_TYPE(nSSType), 
					UN_GET_TARGET_TEX_ADDR(new_val));

				old_val = new_val;
			}
		}
	}

	// サンプラステート設定
	template <typename _SS, typename _T>
	void CGraphicsDevice::SetSamplerStateFilter(
		UN_DWORD stage,
		_SS nSSType,
		_T& old_val, _T new_val)
	{
		if (m_Flags.is_force_set_state) {
			m_pDevice->SetSamplerState(
				stage, 
				UN_GET_TARGET_SAMPLER_STATE_TYPE(nSSType),
				UN_GET_TARGET_TEX_FILTER(new_val));

			old_val = new_val;
		}
		else {
			if (old_val != new_val) {
				m_pDevice->SetSamplerState(
					stage, 
					UN_GET_TARGET_SAMPLER_STATE_TYPE(nSSType), 
					UN_GET_TARGET_TEX_FILTER(new_val));

				old_val = new_val;
			}
		}
	}

	/**
	* レンダーステート設定
	*/
	void CGraphicsDevice::SetRenderState(E_GRAPH_RENDER_STATE nState, UN_DWORD val)
	{
		m_RenderState.SetRenderState(this, nState, val);
	}

	/**
	* レンダーターゲット取得
	*/
	CSurface* CGraphicsDevice::GetRenderTarget(UN_UINT idx)
	{
		UN_ASSERT(idx < MAX_MRT_NUM);
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
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRPAH_GRAPHICS_DEVICE_H__)
