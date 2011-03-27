#if !defined(__IZANAGI_POSTEFFECT_VS_H__)
#define __IZANAGI_POSTEFFECT_VS_H__

#include "izStd.h"
#include "izGraph.h"
#include "../PostEffectDefs.h"

namespace izanagi {
	///////////////////////////////////////////////////
	// 頂点シェーダ基本
	class CPostEffectVS : public CObject {
		friend class CPostEffectVSManager;

	public:
		// インスタンス作成
		template <class _T>
		static _T* Create(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IZ_UINT8* pProgram);

	protected:
		CPostEffectVS();
		virtual ~CPostEffectVS();

		CPostEffectVS(const CPostEffectVS& rhs);
		const CPostEffectVS& operator=(const CPostEffectVS& rhs);

	protected:
		// 解放
		void InternalRelease();

		// シェーダ作成
		IZ_BOOL CreateShader(
			CGraphicsDevice* pDevice,
			IZ_UINT8* pProgram);

		// シェーダ定数ハンドル取得
		IZ_BOOL GetHandleByName(
			IZ_UINT num,
			IZ_PCSTR pNameList[],
			SHADER_PARAM_HANDLE* pHandleList);

	public:
		// 描画
		virtual void Render(
			IZ_FLOAT fPosOffsetX = 0.0f,
			IZ_FLOAT fPosOffsetY = 0.0f,
			const SFloatRect* pTexCoord = IZ_NULL);

		// パラメータセット
		virtual void SetParameter(
			const SVector* pVector,
			IZ_UINT num);

	protected:
		// 共通パラメータセット
		void SetCommonShaderParameter(
			IZ_FLOAT fPosOffsetX,
			IZ_FLOAT fPosOffsetY,
			const SFloatRect* pTexCoord);

		// パラメータセット
		virtual void SetShaderParameter();

		// 初期化
		virtual IZ_BOOL InternalInit();

	protected:
		// 頂点バッファセット
		inline void SetVertexBuffer(CVertexBuffer* pVB);

		// 頂点宣言セット
		inline void SetVertexDeclaration(CVertexDeclaration* pVtxDecl);

		// 頂点シェーダタイプ取得
		inline E_POSTEFFECT_VTX_SHADER GetVSType() const;

	protected:
		enum {
			COMMON_HANDLE_PosOffset = 0,
			COMMON_HANDLE_TexParam,

			COMMON_HANDLE_NUM,
		};

	protected:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		E_POSTEFFECT_VTX_SHADER m_Type;

		CVertexShader* m_pShader;
		CVertexBuffer* m_pVB;
		CVertexDeclaration* m_pVtxDecl;

		// 共通シェーダ定数ハンドル
		SHADER_PARAM_HANDLE m_hCommonHandle[COMMON_HANDLE_NUM];

		SVector m_vecPosOffset;
		SVector m_vecTexParam;
	};

	// inline ****************************

	// インスタンス作成
	template <class _T>
	_T* CPostEffectVS::Create(
		IMemoryAllocator* pAllocator,
		CGraphicsDevice* pDevice,
		IZ_UINT8* pProgram)
	{
		_T* pInstance = IZ_NULL;

		// メモリ確保
		IZ_UINT8* pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(_T));
		IZ_BOOL result = (pBuf != IZ_NULL);
		VGOTO(result, __EXIT__);

		// インスタンス作成
		pInstance = new(pBuf) _T;
		{
			pInstance->AddRef();
			pInstance->m_pAllocator = pAllocator;

			// シェーダ作成
			result = pInstance->CreateShader(pDevice, pProgram);
			VGOTO(result, __EXIT__);
		}

	__EXIT__:
		if (!result) {
			if (pInstance != IZ_NULL) {
				SAFE_RELEASE(pInstance);
			}
			else if (pBuf != NULL) {
				pAllocator->Free(pBuf);
			}
		}
		return pInstance;
	}

	// 頂点バッファセット
	void CPostEffectVS::SetVertexBuffer(CVertexBuffer* pVB)
	{
		SAFE_REPLACE(m_pVB, pVB);
	}

	// 頂点宣言セット
	void CPostEffectVS::SetVertexDeclaration(CVertexDeclaration* pVtxDecl)
	{
		SAFE_REPLACE(m_pVtxDecl, pVtxDecl);
	}

	// 頂点シェーダタイプ取得
	E_POSTEFFECT_VTX_SHADER CPostEffectVS::GetVSType() const
	{
		return m_Type;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_VS_H__)
