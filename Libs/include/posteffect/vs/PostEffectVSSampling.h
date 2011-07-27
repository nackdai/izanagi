#if !defined(__IZANAGI_POSTEFFECT_VS_SAMPLING_H__)
#define __IZANAGI_POSTEFFECT_VS_SAMPLING_H__

#include "PostEffectVS.h"

namespace izanagi {
	///////////////////////////////////////////////////
	/**
	* 周囲Ｎ点をサンプリングする頂点シェーダ
	*/
	template
	<
		IZ_UINT _NUM,			// サンプリングする点数
		IZ_UINT _VECTOR_NUM	// サンプリング位置（ベクトル）数
	>
	class CPostEffectVSSampling : public CPostEffectVS {
	protected:
		CPostEffectVSSampling()
		{
			m_hOffset = IZ_NULL;
			FILL_ZERO(m_vOffset, sizeof(m_vOffset));
		}

		~CPostEffectVSSampling() {}

	public:
		// パラメータセット
		void SetParameter(
			const SVector* pVector,
			IZ_UINT num);

	protected:
		// 初期化
		IZ_BOOL InternalInit();

		// パラメータセット
		void SetShaderParameter();

	protected:
		SHADER_PARAM_HANDLE m_hOffset;
		SVector m_vOffset[_VECTOR_NUM];
	};

	// パラメータセット
	template
	<
		IZ_UINT _NUM,
		IZ_UINT _VECTOR_NUM
	>
	void CPostEffectVSSampling<_NUM, _VECTOR_NUM>::SetParameter(
		const SVector* pVector,
		IZ_UINT num)
	{
		IZ_ASSERT(num <= _VECTOR_NUM);

		for (IZ_UINT i = 0; i < num; ++i) {
			SVector::Copy(m_vOffset[i], pVector[i]);
		}
	}

	// 初期化
	template
	<
		IZ_UINT _NUM,
		IZ_UINT _VECTOR_NUM
	>
	IZ_BOOL CPostEffectVSSampling<_NUM, _VECTOR_NUM>::InternalInit()
	{
		static IZ_PCSTR name = "g_vUVOffsetSampling";

		IZ_BOOL result = GetHandleByName(
							1,
							&name,
							&m_hOffset);

		return result;
	}

	// パラメータセット
	template
	<
		IZ_UINT _NUM,
		IZ_UINT _VECTOR_NUM
	>
	void CPostEffectVSSampling<_NUM, _VECTOR_NUM>::SetShaderParameter()
	{
		IZ_ASSERT(m_hOffset != IZ_NULL);

		m_pShader->SetVectorArray(
			m_hOffset,
			m_vOffset,
			COUNTOF(m_vOffset));
	}

	////////////////////////////////
	/**
	* １点をサンプリング（デフォルト）
	*/
	class CPostEffectVSSampling_1 : public CPostEffectVS {
		friend class CPostEffectVS;

	protected:
		CPostEffectVSSampling_1()
		{
			m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_1;
		}

		~CPostEffectVSSampling_1() {}
	};

	////////////////////////////////
	/**
	* 周囲４点をサンプリング
	*/
	class CPostEffectVSSampling_4 : public CPostEffectVSSampling<4, 2> {
		friend class CPostEffectVS;

	protected:
		CPostEffectVSSampling_4()
		{
			m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_4;
		}

		~CPostEffectVSSampling_4() {}
	};

	////////////////////////////////
	/**
	* 周囲８点をサンプリング
	*/
	class CPostEffectVSSampling_8 : public CPostEffectVSSampling<8, 4> {
		friend class CPostEffectVS;

	protected:
		CPostEffectVSSampling_8()
		{
			m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_8;
		}

		~CPostEffectVSSampling_8() {}
	};

	////////////////////////////////
	/**
	* 周囲１６点をサンプリング
	*/
	class CPostEffectVSSampling_16 : public CPostEffectVSSampling<16, 8> {
		friend class CPostEffectVS;

	protected:
		CPostEffectVSSampling_16()
		{
			m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_16;
		}

		~CPostEffectVSSampling_16() {}
	};

	////////////////////////////////
	/**
	* 周囲５点をサンプリング
	*/
	class CPostEffectVSSampling_5 : public CPostEffectVSSampling<5, 3> {
		friend class CPostEffectVS;

	protected:
		CPostEffectVSSampling_5()
		{
			m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_5;
		}

		~CPostEffectVSSampling_5() {}
	};

	////////////////////////////////
	/**
	* 周囲７点をサンプリング
	*/
	class CPostEffectVSSampling_7 : public CPostEffectVSSampling<7, 4> {
		friend class CPostEffectVS;

	protected:
		CPostEffectVSSampling_7()
		{
			m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_7;
		}

		~CPostEffectVSSampling_7() {}
	};

	////////////////////////////////
	/**
	* 周囲９点をサンプリング
	*/
	class CPostEffectVSSampling_9 : public CPostEffectVSSampling<9, 5> {
		friend class CPostEffectVS;

	protected:
		CPostEffectVSSampling_9()
		{
			m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_9;
		}

		~CPostEffectVSSampling_9() {}
	};

	////////////////////////////////
	/**
	* 周囲１３点をサンプリング
	*/
	class CPostEffectVSSampling_13 : public CPostEffectVSSampling<13, 7> {
		friend class CPostEffectVS;

	protected:
		CPostEffectVSSampling_13()
		{
			m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_13;
		}

		~CPostEffectVSSampling_13() {}
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_VS_SAMPLING_H__)
