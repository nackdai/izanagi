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
	* �������p�����[�^
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
	* �O���t�B�b�N�X�f�o�C�X
	*/
	class CGraphicsDevice : public CObject {
		friend class CTexture;
		friend class CVertexBuffer;
		friend class CIndexBuffer;

	private:
		static CMemoryAllocator s_cDeviceAllocator;
		static CGraphicsDevice* s_pInstance;

	public:
		// �C���X�^���X�쐬
		static CGraphicsDevice* CreateGrapicsDevice(
			UN_UINT nBufSize,
			void* pDeviceBuffer);

		// �O���t�B�b�N�X�f�o�C�X�p�������A���P�[�^�_���v
		static inline void Dump();

	private:
		CGraphicsDevice();
		~CGraphicsDevice();

		NO_COPIABLE(CGraphicsDevice);

	private:
		// ���
		void InternalRelease();

	public:
		// �t�@�C������e�N�X�`���쐬
		CTexture* CreateTextureFromFile(
			UN_PCSTR lpszPathName,
			E_GRAPH_PIXEL_FMT fmt = E_GRAPH_PIXEL_FMT_NUM);

		// ����������e�N�X�`���쐬
		CTexture* CreateTextureFromMemory(
			void* pData,
			UN_UINT nDataSize,
			E_GRAPH_PIXEL_FMT fmt);

		// �e�N�X�`���쐬
		CTexture* CreateTexture(
			UN_UINT nWidth,
			UN_UINT nHeight,
			UN_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			E_GRAPH_RSC_CREATE_TYPE nCreateType);

		// �t�@�C������L���[�u�e�N�X�`���쐬
		CCubeTexture* CreateCubeTextureFromFile(
			UN_PCSTR lpszPathName,
			E_GRAPH_PIXEL_FMT fmt = E_GRAPH_PIXEL_FMT_NUM);

		// �L���[�u�e�N�X�`���쐬
		CCubeTexture* CreateCubeTexture(
			UN_UINT nWidth,
			UN_UINT nHeight,
			UN_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			UN_BOOL bIsDynamic);

		// �����_�[�^�[�Q�b�g�쐬
		CTexture* CreateRenderTarget(
			UN_UINT nWidth, UN_UINT nHeight,
			E_GRAPH_PIXEL_FMT fmt);
		
		// �V�X�e����������Ƀe�N�X�`���쐬
		CTexture* CreateTextureOnSysMem(
			UN_UINT nWidth, UN_UINT nHeight,
			E_GRAPH_PIXEL_FMT fmt,
			UN_UINT nMipLevel);

		// ���_�o�b�t�@�쐬
		CVertexBuffer* CreateVertexBuffer(
			UN_UINT nStride,
			UN_UINT nVtxNum,
			E_GRAPH_RSC_CREATE_TYPE nCreateType);

		// �C���f�b�N�X�o�b�t�@�쐬
		CIndexBuffer* CreateIndexBuffer(
			UN_UINT nIdxNum,
			E_GRAPH_INDEX_BUFFER_FMT fmt,
			E_GRAPH_RSC_CREATE_TYPE nCreateType);

		// ���_�V�F�[�_�쐬
		CVertexShader* CreateVertexShader(const void* pProgram);

		// �s�N�Z���V�F�[�_�쐬
		CPixelShader* CreatePixelShader(const void* pProgram);

		// ���_�錾�쐬
		CVertexDeclaration* CreateVertexDeclaration(const D3D_VTX_ELEMENT* pElem);
		CVertexDeclaration* CreateVertexDeclaration(const SVertexElement* pElem, UN_UINT nNum);

	private:
		// ���\�[�X�}��
		template <class _T>
		_T* InsertResource(_T* p, _T* pListTop);

		// ���\�[�X�폜
		template <class _T>
		_T* RemoveResource(_T* p, _T* pListTop);

		void RemoveTexture(CTexture* p);
		void RemoveVertexBuffer(CVertexBuffer* p);
		void RemoveIndexBuffer(CIndexBuffer* p);

	public:
		// ���Z�b�g
		UN_BOOL Reset(const SGraphicsDeviceInitParams& sParams);

	private:
		// �{�̍쐬
		UN_BOOL CreateBody(const SGraphicsDeviceInitParams& sParams);

		// ���Z�b�g
		UN_BOOL ResetInternal(const SGraphicsDeviceInitParams& sParams);

	private:
		template <class _T>
		inline void ReleaseResource(_T* pList);

		template <class _T>
		inline void ResetResource(_T* pList);

	public:
		// �`��J�n
		UN_BOOL BeginRender(
			UN_DWORD nClearFlags,
			UN_COLOR nClearColor,
			UN_FLOAT fClearZ,
			UN_DWORD nClearStencil);

		// �`��I��
		void EndRender();

		// �N���A
		void Clear(
			UN_DWORD nClearFlags,
			UN_COLOR nClearColor,
			UN_FLOAT fClearZ,
			UN_DWORD nClearStencil);

		// �V�[���`��J�n
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

		// �V�[���`��I��
		void EndScene(UN_UINT flag = 0xffffffff);

		// ����
		UN_BOOL Present();

		// 2D�`��J�n
		UN_BOOL Begin2D();

		// 2D�`��I��
		UN_BOOL End2D();

		// 2D�`��o�b�t�@�t���b�V��
		UN_BOOL Flush2D();

		// 2D�X�v���C�g�`��
		UN_BOOL Draw2DSprite(
			const CFloatRect& rcSrc,
			const CIntRect& rcDst,
			const UN_COLOR color = UN_COLOR_RGBA(255, 255, 255, 255));
		UN_BOOL Draw2DSpriteEx(
			const CIntRect& rcSrc,
			const CIntRect& rcDst,
			const UN_COLOR color = UN_COLOR_RGBA(255, 255, 255, 255));

		// 2D��`�`��
		UN_BOOL Draw2DRect(
			const CIntRect& rcDst,
			const UN_COLOR color);

		// 2D���C���`��
		UN_BOOL Draw2DLine(
			const CIntPoint& ptStart,
			const CIntPoint& ptGoal,
			const UN_COLOR color);

		// 2D�`�惂�[�h
		void Set2DRenderOp(E_GRAPH_2D_RENDER_OP nOp);
		E_GRAPH_2D_RENDER_OP Get2DRenderOp() const;

	private:
		void ClearRenderState();

	public:
		// �e�N�X�`���Z�b�g
		UN_BOOL SetTexture(UN_UINT nStage, CBaseTexture* pTex);

		// �e�N�X�`���擾
		CBaseTexture* GetTexture(UN_UINT nStage);

	private:
		// �����_�[�^�[�Q�b�g�Z�b�g
		UN_BOOL PushRenderTarget(CSurface** pSurface, UN_UINT num);
		void SetRenderTarget(CSurface** pSurface, UN_UINT num);

		// �[�x�E�X�e���V���Z�b�g
		UN_BOOL PushDepthStencil(CSurface* pSurface);
		void SetDepthStencil(CSurface* pSurface);

	public:
		// ���_�o�b�t�@�Z�b�g
		UN_BOOL SetVertexBuffer(
			UN_UINT nStreamIdx,
			UN_UINT nOffsetByte,
			UN_UINT nStride,
			CVertexBuffer* pVB);

		// FVF�Z�b�g
		UN_BOOL SetFVF(UN_DWORD dwFVF);

		// �C���f�b�N�X�o�b�t�@�Z�b�g
		UN_BOOL SetIndexBuffer(CIndexBuffer* pIB);

		// �C���f�b�N�X�o�b�t�@�`��
		UN_BOOL DrawIndexedPrimitive(
			E_GRAPH_PRIM_TYPE prim_type,
			UN_UINT nBaseIdx,
			UN_UINT nMinIdx,
			UN_UINT nVtxNum,
			UN_UINT nStartIdx,
			UN_UINT nPrimCnt);

		// �C���f�b�N�X�o�b�t�@�Ȃ��`��
		UN_BOOL DrawPrimitive(
			E_GRAPH_PRIM_TYPE prim_type,
			UN_UINT nStartIdx,
			UN_UINT nPrimCnt);

	public:
		// ���_�V�F�[�_�Z�b�g
		UN_BOOL SetVertexShader(CVertexShader* pVS);

		// �s�N�Z���V�F�[�_�Z�b�g
		UN_BOOL SetPixelShader(CPixelShader* pPS);

		// ���_�錾�Z�b�g
		UN_BOOL SetVertexDeclaration(CVertexDeclaration* pVD);

	public:
		// �r���[�|�[�g�Z�b�g
		UN_BOOL SetViewport(const SViewport& vp);

		// �f�t�H���g�̃����_�[�X�e�[�g��ݒ�
		void SetDefaultRenderState();

		// �����_�[�X�e�[�g�ۑ�
		UN_BOOL SaveRenderState();

		// �����_�[�X�e�[�g���A
		UN_BOOL LoadRenderState();
		void SetRenderState(const S_RENDER_STATE& sRS);

	public:
		// �f�B�X�v���C���[�h�擾
		inline const D3DDISPLAYMODE& GetDisplayMode() const;

		// �v���[���g�p�����[�^�擾
		inline const D3DPRESENT_PARAMETERS& GetPresentParam() const;

		// ���t���b�V�����[�g�擾
		inline UN_UINT GetRefreshRate() const;

		// �T���v���X�e�[�g�擾
		inline const S_SAMPLER_STATE& GetSamplerState(UN_UINT nStage) const;

		// �����_�[�X�e�[�g�擾
		inline const S_RENDER_STATE& GetRenderState() const;

		// �r���[�|�[�g�擾
		inline const SViewport& GetViewport() const;

		// �V�U�[��`
		inline void SetScissorTestRect(const SIntRect& rc);

		// �����_�[�X�e�[�g�ݒ�
		inline void SetRenderState(E_GRAPH_RENDER_STATE nState, UN_DWORD val);

	private:
		// �e�N�X�`���X�e�[�W�X�e�[�g�ݒ�
		template <typename _TS, typename _T>
		inline void SetTextureStageState(
			UN_DWORD stage,
			_TS nTSType,
			_T& old_val, _T new_val);

		// �T���v���X�e�[�g�ݒ�
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
		// �����_�[�^�[�Q�b�g�擾
		inline CSurface* GetRenderTarget(UN_UINT idx);
		inline CSurface* GetDepthSrencil();

		// ���Z�b�g�p�R�[���o�b�N�Z�b�g
		inline void SetResetCallBack(GraphicsDeviceResetCallBack pCallBack);

		// �f�o�C�X���X�g�p�R�[���o�b�N�Z�b�g
		inline void SetLostDeviceCallBack(GraphicsDeviceLostDeviceCallBack pCallBack);

	public:
		D3D_DEVICE* GetRawInterface() { return m_pDevice; }

	private:
		enum {
			// �����_�[�^�[�Q�b�g�̃L���[��
			RT_QUEUE_MAX = 4,
		};

	private:
		IMemoryAllocator* m_pAllocator;

		D3D_INST* m_pD3D;

		// �{��
		D3D_DEVICE* m_pDevice;

		D3DDISPLAYMODE m_sDisplayMode;
		D3DPRESENT_PARAMETERS m_sPresentParameters;
		HWND m_hFocusWindow;

		// �e�N�X�`��
		CBaseTexture* m_pTexture[TEX_STAGE_NUM];
		S_SAMPLER_STATE m_sSamplerState[TEX_STAGE_NUM];

		// �����_�[�X�e�[�g
		CRenderState m_RenderState;

		// �t���[���o�b�t�@
		CSurface* m_pRT;
		CSurface* m_pDepth;

		// �����_�[�^�[�Q�b�g�Ǘ�
		CRenderTargetManager<RT_QUEUE_MAX> m_cRTMgr[MAX_MRT_NUM];
		CRenderTargetManager<RT_QUEUE_MAX> m_cDepthMgr;

		// 2D�`��p
		C2DRenderer* m_p2DRenderer;

		// ���Z�b�g�΍��p
		CTexture* m_pResetTexture;
		CVertexBuffer* m_pResetVB;
		CIndexBuffer* m_pResetIB;

		// �t���O
		struct {
			UN_UINT is_call_begin		: 1;	// BeginScene���Ă񂾂��ǂ���
			UN_UINT is_render_2d		: 1;	// 2D�`�撆��
			UN_UINT is_force_set_state	: 1;	// �����I�ɃX�e�[�g��ݒ肷�邩�ǂ���
			UN_UINT is_lost_device		: 1;	// �f�o�C�X���X�g���Ă��邩�ǂ���
			UN_UINT reserved			: 28;
		} m_Flags;

		// ���Z�b�g�p�R�[���o�b�N
		GraphicsDeviceResetCallBack m_pResetCallBack;

		// �f�o�C�X���X�g�p�R�[���o�b�N
		GraphicsDeviceLostDeviceCallBack m_pLostDeviceCallBack;
	};

	// inline ***************************

	/**
	* �O���t�B�b�N�X�f�o�C�X�p�������A���P�[�^�_���v
	*/
	void CGraphicsDevice::Dump()
	{
		s_cDeviceAllocator.Dump();
	}

	/**
	* �f�B�X�v���C���[�h�擾
	*/
	const D3DDISPLAYMODE& CGraphicsDevice::GetDisplayMode() const
	{
		return m_sDisplayMode;
	}

	/**
	* �v���[���g�p�����[�^�擾
	*/
	const D3DPRESENT_PARAMETERS& CGraphicsDevice::GetPresentParam() const
	{
		return m_sPresentParameters;
	}

	/**
	* ���t���b�V�����[�g�擾
	*/
	UN_UINT CGraphicsDevice::GetRefreshRate() const
	{
		return m_sDisplayMode.RefreshRate;
	}

	/**
	* �T���v���X�e�[�g�擾
	*/
	const S_SAMPLER_STATE& CGraphicsDevice::GetSamplerState(UN_UINT nStage) const
	{
		UN_ASSERT(nStage < TEX_STAGE_NUM);
		return m_sSamplerState[nStage];
	}

	/**
	* �����_�[�X�e�[�g�擾
	* �񐄏�
	*/
	const S_RENDER_STATE& CGraphicsDevice::GetRenderState() const
	{
		return m_RenderState;
	}

	/**
	* �r���[�|�[�g�擾
	*/
	const SViewport& CGraphicsDevice::GetViewport() const
	{
		return m_RenderState.vp;
	}

	/**
	* �V�U�[��`
	*/
	void CGraphicsDevice::SetScissorTestRect(const SIntRect& rc)
	{
		// �O�̂��߂��ꂭ�炢�̓`�F�b�N���邩
		UN_C_ASSERT(sizeof(RECT) == sizeof(CIntRect));

		if (m_RenderState.rcScissor != rc) {
			m_RenderState.rcScissor = rc;
			m_pDevice->SetScissorRect(reinterpret_cast<RECT*>(&m_RenderState.rcScissor));
		}
	}

	// �e�N�X�`���X�e�[�W�X�e�[�g�ݒ�
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

	// �T���v���X�e�[�g�ݒ�
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

	// �T���v���X�e�[�g�ݒ�
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
	* �����_�[�X�e�[�g�ݒ�
	*/
	void CGraphicsDevice::SetRenderState(E_GRAPH_RENDER_STATE nState, UN_DWORD val)
	{
		m_RenderState.SetRenderState(this, nState, val);
	}

	/**
	* �����_�[�^�[�Q�b�g�擾
	*/
	CSurface* CGraphicsDevice::GetRenderTarget(UN_UINT idx)
	{
		UN_ASSERT(idx < MAX_MRT_NUM);
		return m_RenderState.curRT[idx];
	}

	/**
	* �[�x�E�X�e���V���擾
	*/
	CSurface* CGraphicsDevice::GetDepthSrencil()
	{
		return m_RenderState.curDepth;
	}

	/**
	* ���Z�b�g�p�R�[���o�b�N�Z�b�g
	*/
	void CGraphicsDevice::SetResetCallBack(GraphicsDeviceResetCallBack pCallBack)
	{
		m_pResetCallBack = pCallBack;
	}

	/**
	* �f�o�C�X���X�g�p�R�[���o�b�N�Z�b�g
	*/
	void CGraphicsDevice::SetLostDeviceCallBack(GraphicsDeviceLostDeviceCallBack pCallBack)
	{
		m_pLostDeviceCallBack = pCallBack;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRPAH_GRAPHICS_DEVICE_H__)
