#if !defined(__IZANAGI_POSTEFFET_PASS_H__)
#define __IZANAGI_POSTEFFET_PASS_H__

#include "izGraph.h"
#include "PESFormat.h"
#include "util/PostEffectShaderUtil.h"
#include "vs/PostEffectVS.h"

namespace izanagi {
	/**
	* シェーダのパス
	*/
	class CPostEffectPass {
		friend class CPostEffect;
		friend class CPostEffectShader;
		friend class CPostEffectPassTable;
		friend class CPostEffectSystem;

		// パラメータホルダ
		struct SParamInfo {
			IZ_UINT idx;						// パラメータインデックス（そのパスの中で何番目のパラメータか）
			SHADER_PARAM_HANDLE handle;			// パラメータハンドル
		};

		// サンプラホルダ
		struct SSamplerInfo {
			IZ_UINT16 idx;				// パラメータインデックス（そのパスの中で何番目のサンプラか）
			IZ_UINT16 resource_id;		// リソースID (0〜7のいずれか)
			SHADER_PARAM_HANDLE handle;	// パラメータハンドル
		};

		template <typename _T>
		struct SParams {
			IZ_UINT num;
			_T* list;

			SParams()
			{
				num = 0;
				list = IZ_NULL;
			}
		};

		template <typename _T>
		IZ_UINT8* SetInfoBuffer(
			SParams<_T>& sParams,
			IZ_UINT num,
			IZ_UINT8* buf)
		{
			if (num > 0) {
				sParams.num = num;
				sParams.list = reinterpret_cast<_T*>(buf);
				return (buf + sizeof(_T) * num);
			}
			return buf;
		}

		template <typename _T>
		IZ_BOOL InitInfo(
			SParams<_T>& sParams,
			IZ_UINT idx,
			IZ_UINT nIdxInPass,
			IZ_PCSTR name)
		{
			IZ_ASSERT(idx < sParams.num);
			sParams.list[idx].idx = nIdxInPass;
			sParams.list[idx].handle = m_pPS->GetHandleByName(name);
			return (sParams.list[idx].handle != IZ_NULL);
		}

		template <typename _T>
		const _T* GetInfo(
			const SParams<_T>& sParams,
			IZ_UINT idx) const
		{
			IZ_ASSERT(idx < sParams.num);
			return &sParams.list[idx];
		}

	private:
		static size_t ComputeBufferSize(
			IZ_UINT nParamNum,
			IZ_UINT nSmplNum)
		{
			size_t ret = sizeof(SParamInfo) * nParamNum;
			ret += sizeof(SSamplerInfo) * nSmplNum;
			return ret;
		}

	private:
		inline CPostEffectPass();
		inline ~CPostEffectPass();

		NO_COPIABLE(CPostEffectPass);

	private:
		// 描画
		inline void Render(const SFloatRect& rcTexCoord);

		// バッファ割り当て
		IZ_UINT8* SetParamBuffer(IZ_UINT num, IZ_UINT8* p)
		{
			return SetInfoBuffer<SParamInfo>(m_Params, num, p);
		}
		IZ_UINT8* SetSamplerBuffer(IZ_UINT num, IZ_UINT8* p)
		{
			return SetInfoBuffer<SSamplerInfo>(m_Samplers, num, p);
		}

		// 初期化
		inline void Init(
			IZ_UINT idx, 
			const S_PES_PASS* pDesc);

		// 初期化
		inline void InitParam(
			IZ_UINT idx,
			IZ_UINT nParamIdx,
			IZ_PCSTR name);

		// 初期化
		inline IZ_BOOL InitSampler(
			IZ_UINT idx,
			IZ_UINT nSmpleIdx,
			IZ_PCSTR name);

		inline void Clear();

		// ピクセルシェーダのセット
		void SetPS(graph::CPixelShader* pPS) { SAFE_REPLACE(m_pPS, pPS); }

		// ピクセルシェーダ取得
		graph::CPixelShader* GetPS() { return m_pPS; }

		// パラメータ数取得
		IZ_UINT GetParamNum() const { return m_Params.num; }

		// サンプラ数取得
		IZ_UINT GetSamplerNum() const { return m_Samplers.num; }

		// ホルダ取得
		const SParamInfo* GetParamInfo(IZ_UINT idx) const { return GetInfo(m_Params, idx); }
		const SSamplerInfo* GetSamplerInfo(IZ_UINT idx) const { return GetInfo(m_Samplers, idx); }

		// 頂点シェーダのセット
		void SetVS(CPostEffectVS* pVS) { SAFE_REPLACE(m_pVS, pVS); }

		// 頂点シェーダの取得
		CPostEffectVS* GetVS() { return m_pVS; }

		IZ_UINT GetIdx() const { return m_nIdx; }

		const S_PES_PASS* GetDesc() const { return m_pDesc; }

	private:
		IZ_UINT m_nIdx;

		const S_PES_PASS* m_pDesc;

		SParams<SParamInfo> m_Params;
		SParams<SSamplerInfo> m_Samplers;

		// ピクセルシェーダ
		graph::CPixelShader* m_pPS;

		// 頂点シェーダ
		CPostEffectVS* m_pVS;
	};

	// inline ***************************************

	// コンストラクタ
	CPostEffectPass::CPostEffectPass()
	{
		m_nIdx = 0;
		m_pDesc = IZ_NULL;
		m_pPS = IZ_NULL;
		m_pVS = IZ_NULL;
	}

	// デストラクタ
	CPostEffectPass::~CPostEffectPass()
	{
		Clear();
	}

	// 描画
	void CPostEffectPass::Render(const SFloatRect& rcTexCoord)
	{
		m_pVS->Render(
			0.0f, 0.0f,
			&rcTexCoord);
	}

	// 初期化
	void CPostEffectPass::Init(
		IZ_UINT idx, 
		const S_PES_PASS* pDesc)
	{
		m_nIdx = idx;
		m_pDesc = pDesc;

		m_pPS = IZ_NULL;
		m_pVS = IZ_NULL;
	}

	// 初期化
	void CPostEffectPass::InitParam(
		IZ_UINT idx,
		IZ_UINT nParamIdx,
		IZ_PCSTR name)
	{
		IZ_BOOL result = InitInfo<SParamInfo>(m_Params, idx, nParamIdx, name);

#if 0
		if (result) {
			m_Params.list[idx].elements = m_pPS->GetArrayLengthOf(m_Params.list[idx].handle);
			m_Params.list[idx].type = type;
		}
#endif
	}

	// 初期化
	IZ_BOOL CPostEffectPass::InitSampler(
		IZ_UINT idx,
		IZ_UINT nSmpleIdx,
		IZ_PCSTR name)
	{
		IZ_BOOL ret = InitInfo<SSamplerInfo>(m_Samplers, idx, nSmpleIdx, name);
		
		if (ret) {
			m_Samplers.list[idx].resource_id = (IZ_UINT16)CPostEffectShaderUtil::GetSamplerResourceIndexByHandle(
															m_pPS,
															m_Samplers.list[idx].handle);

			// NOTE
			// サンプラレジスタ 0 - 7 の８個まで
			ret = (m_Samplers.list[idx].resource_id < 8);
			IZ_ASSERT(ret);
		}

		return ret;
	}

	void CPostEffectPass::Clear()
	{
		SAFE_RELEASE(m_pPS);
		SAFE_RELEASE(m_pVS);
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFET_PASS_H__)
