#if !defined(__IZANAGI_POSTEFFECT_SHADER_H__)
#define __IZANAGI_POSTEFFECT_SHADER_H__

#include "izStd.h"
#include "izGraph.h"
#include "izIo.h"
#include "table/PostEffectShaderStringTable.h"
#include "table/PostEffectShaderConstTable.h"
#include "table/PostEffectShaderTextureTable.h"
#include "table/PostEffectShaderSamplerTable.h"
#include "table/PostEffectShaderPassTable.h"
#include "table/PostEffectShaderTechTable.h"
#include "util/PostEffectUtilityParam.h"

namespace izanagi {
	class CPostEffectFunctor;
	class CPostEffectTextureList;
	class CPostEffectVSManager;

	/**
	* ポストエフェクトシェーダ
	*/
	class CPostEffectShader : public CObject {
		friend class CPostEffect;

		static CPostEffectShader* CreatePostEffectShader(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			CPostEffectVSManager* pVSMgr,
			IInputStream* in);

	private:
		CPostEffectShader();
		~CPostEffectShader();

		NO_COPIABLE(CPostEffectShader);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		// 描画
		IZ_BOOL Render(
			const SIntRect* pSrcRect,
			const SIntRect* pDstRect);
		IZ_BOOL Render(const SFloatRect& rcTexCoord);

		// 描画開始
		IZ_BOOL BeginRender(
			CPostEffectFunctor* pFunctor,
			IZ_UINT nTechIdx,
			IZ_UINT nPassIdx,
			CTexture* pTex,
			IZ_BOOL bIsClear);
		IZ_BOOL BeginRender(
			CPostEffectFunctor* pFunctor,
			IZ_UINT nTechIdx,
			IZ_UINT nPassIdx);

		// 描画終了
		IZ_BOOL EndRender();

		IZ_POSTEFFECT_HANDLE GetParamHandleByName(IZ_PCSTR pszName) const;
		IZ_POSTEFFECT_HANDLE GetParamHandleBySemantic(IZ_PCSTR pszSemantic) const;

		IZ_POSTEFFECT_HANDLE GetParamHandleByIdx(IZ_UINT nIdx);
		IZ_POSTEFFECT_HANDLE GetTexHandleByIdx(IZ_UINT nIdx);

		// パラメータをセットする
		IZ_BOOL SetParameter(
			IZ_POSTEFFECT_HANDLE nHandle,
			const void* pValue,
			IZ_UINT nBytes);

		// テクスチャをセットする
		IZ_BOOL SetTexture(
			IZ_POSTEFFECT_HANDLE nHandle,
			CTexture* pTex);

		// 直接頂点シェーダにテクスチャオフセットパラメータをセットする
		IZ_BOOL SetTextureOffsetParameter(
			const SVector* pVector,
			IZ_UINT num);

		// テクスチャオフセットパラメータをセットする
		IZ_BOOL SetTextureOffsetParameter(const CTexture* pTex);

		// コミットチェンジ
		IZ_BOOL CommitChanges();		

	private:
		// 初期化
		IZ_BOOL Init(
			size_t nBufSize,
			IZ_UINT8* pBuffer,
			const S_PES_HEADER* pHeader,
			CPostEffectVSManager* pVSMgr,
			IInputStream* in);

		// パスデータ作成
		IZ_UINT8* CreatePass(
			IZ_UINT8* pBuffer,
			IInputStream* in);

	private:
		template <typename _T, typename _U>
		const _T* GetDesc(_U& cDescTbl, IZ_PCSTR name) const;

		// パラメータ記述を取得
		const S_PES_PARAMETER* GetParamDesc(IZ_UINT idx) const { return m_cConstTable.GetDesc(idx); }
		const S_PES_PARAMETER* GetParamDescByName(IZ_PCSTR pszName) const { return GetDesc<S_PES_PARAMETER>(m_cConstTable, pszName); }

		// パラメータアノテーションを取得
		const S_PES_PARAM_ANN* GetParamAnn(IZ_UINT idx) const { return m_cConstTable.GetAnn(idx); }
		const S_PES_PARAM_ANN* GetParamAnnByName(IZ_PCSTR pszName) const;

		// パラメータ取得
		const void* GetParameter(IZ_UINT idx, IZ_UINT* pBytes) const;
		const void* GetParameterByName(IZ_PCSTR pszName, IZ_UINT* pBytes) const;

		// サンプラ記述取得
		const S_PES_SAMPLER* GetSamplerDesc(IZ_UINT idx) const { return m_cSmplTable.GetDesc(idx); }
		const S_PES_SAMPLER* GetSamplerDescByName(IZ_PCSTR pszName) const { return GetDesc<S_PES_SAMPLER>(m_cSmplTable, pszName); }

		// パス記述取得
		const S_PES_PASS* GetPassDesc(IZ_UINT idx) const { return m_cPassTable.GetDesc(idx); }
		const S_PES_PASS* GetPassDescByName(IZ_PCSTR pszName) const { return GetDesc<S_PES_PASS>(m_cPassTable, pszName); }

		// テクニック記述取得
		const S_PES_TECHNIQUE* GetTechniqueDesc(IZ_UINT idx) const { return m_cTechTable.GetDesc(idx); }
		const S_PES_TECHNIQUE* GetTechniqueDescByName(IZ_PCSTR pszName) const { return GetDesc<S_PES_TECHNIQUE>(m_cTechTable, pszName); }

		// テクスチャ記述を取得
		const S_PES_TEXTURE* GetTexDesc(IZ_UINT idx) const { return m_cTexTable.GetDesc(idx); }
		const S_PES_TEXTURE* GetTexDescByName(IZ_PCSTR pszName) const { return GetDesc<S_PES_TEXTURE>(m_cTexTable, pszName); }

	private:
		// ファイルヘッダ取得
		const S_PES_HEADER& GetHeader() const { return m_sHeader; }

		// テクニックのパス数取得
		inline IZ_UINT GetPassNumInTechnique(IZ_UINT idx) const;

		// パスインデックス変換
		inline IZ_UINT ConvertPassIdx(IZ_UINT nTechIdx, IZ_UINT nPassIdx);

		// ファンクタタイプ取得
		inline IZ_PCSTR GetFunctorName(IZ_UINT nTechIdx, IZ_UINT nPassIdx);

		// パス名取得
		inline IZ_PCSTR GetPassName(IZ_UINT nTechIdx, IZ_UINT nPassIdx);

		// ファンクタ引数取得
		inline const IZ_FLOAT* GetFunctorArgs(IZ_UINT nTechIdx, IZ_UINT nPassIdx);

		// パス取得
		inline CPostEffectPass* GetPass(IZ_UINT idx);
		inline CPostEffectPass* GetPass(IZ_UINT nTechIdx, IZ_UINT nPassIdx);

		// パラメータテーブル取得
		CPostEffectConstTable& GetConstTable() { return m_cConstTable; }

		// パステーブル取得
		CPostEffectPassTable& GetPassTable() { return m_cPassTable; }

		// テクスチャテーブル取得
		CPostEffectTextureTable& GetTextureTable() { return m_cTexTable; }

		// サンプラテーブル取得
		CPostEffectSamplerTable& GetSamplerTable() { return m_cSmplTable; }

		// テクニックテーブル取得
		CPostEffectTechTable& GetTechniqueTable() { return m_cTechTable; }

		// 文字列取得
		IZ_PCSTR GetString(IZ_UINT pos) const { return m_cStrTable.GetString(pos); }

		// うーん・・・
		inline const S_PES_PASS_ANN* GetPassAnn(IZ_UINT nTechIdx, IZ_UINT nPassIdx);

	public:
		// アルファブレンドステートをセット
		inline void SetAlphaBlendState(
			IZ_BOOL bEnable,
			IZ_UINT nBlendMethod);

		// アルファテストステートをセット
		inline void SetAlphaTestState(
			IZ_BOOL bEnable,
			E_GRAPH_CMP_FUNC nFunc,
			IZ_UINT8 nRef);

		// テクスチャインデックス取得
		inline IZ_INT GetSrcTexIdx(IZ_UINT nSamplerIdx, IZ_UINT nTechIdx, IZ_UINT nPassIdx);
		inline IZ_INT GetDstTexIdx(IZ_UINT nTechIdx, IZ_UINT nPassIdx);

		// テクスチャ取得
#if 0
		CTexture* GetTexture(IZ_UINT idx) { return m_cTexTable.GetTexture(idx); }
#else
		CTexture* GetTexture(IZ_UINT idx)
		{
			CTexture* ret = m_cTexTableTmp.GetTextureIfDirty(idx);
			if (ret == IZ_NULL) {
				ret = m_cTexTable.GetTexture(idx);
			}
			return ret;
		}
#endif

	private:
		struct SStateAlphaBlend {
			struct {
				IZ_UINT isDirty	: 1;
				IZ_UINT enable	: 1;
			};

			IZ_UINT method;	// アルファブレンド式

			// コンストラクタ
			SStateAlphaBlend()
			{
				isDirty = IZ_FALSE;
				enable = IZ_FALSE;
				method = E_GRAPH_ALPHA_BLEND_NORMAL;
			}

			// 有効かどうか
			IZ_BOOL IsDirty() const { return isDirty; }

			// 有効フラグおろす
			void SetDisable() { isDirty = IZ_FALSE; }
		};

		struct SStateAlphaTest {
			struct {
				IZ_UINT isDirty	: 1;
				IZ_UINT enable	: 1;	// アルファテストの可否
				IZ_UINT ref		: 8;	// アルファテスト閾値
			};

			E_GRAPH_CMP_FUNC func;	// アルファテスト式

			// コンストラクタ
			SStateAlphaTest()
			{
				FILL_ZERO(this, sizeof(SStateAlphaTest));
			}

			// 有効かどうか
			IZ_BOOL IsDirty() const { return isDirty; }

			// 有効フラグおろす
			void SetDisable() { isDirty = IZ_FALSE; }
		};

		struct SRenderParam {
			IZ_INT tech_idx;
			IZ_INT pass_idx;
			CPostEffectFunctor* functor;
			CTexture* render_target;

			SRenderParam();
			void Clear();
			IZ_BOOL IsValid();
		};

	private:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		// ファイルヘッダ
		S_PES_HEADER m_sHeader;

		// 各種テーブル
		CPostEffectStringTable m_cStrTable;		// 文字列テーブル
		CPostEffectConstTable m_cConstTable;	// シェーダ定数テーブル
		CPostEffectTextureTable m_cTexTable;	// テクスチャテーブル
		CPostEffectSamplerTable m_cSmplTable;	// サンプラテーブル
		CPostEffectPassTable m_cPassTable;		// パステーブル
		CPostEffectTechTable m_cTechTable;		// テクニックテーブル

		CPostEffectPass* m_pPass;

		// テクスチャ一時保管先
		CPostEffectTextureTable m_cTexTableTmp;

		IZ_BOOL m_bIsBeginScene;

		SRenderParam m_RenderParam;

		// レンダーステート(アルファブレンド)
		SStateAlphaBlend m_StateAlphaBlend;

		// レンダーステート(アルファテスト)
		SStateAlphaTest m_StateAlphaTest;

		typedef CPostEffectPass::SSamplerInfo	SSmplInfo;
	};

	// inline ********************************

	// テクニックのパス数取得
	IZ_UINT CPostEffectShader::GetPassNumInTechnique(IZ_UINT idx) const
	{
		IZ_ASSERT(idx < m_sHeader.numTech);
		IZ_UINT ret = m_cTechTable.GetDesc(idx)->numPass;
		return ret;
	}

	// パスインデックス変換
	IZ_UINT CPostEffectShader::ConvertPassIdx(IZ_UINT nTechIdx, IZ_UINT nPassIdx)
	{
		IZ_ASSERT(nTechIdx < m_sHeader.numTech);

		const S_PES_TECHNIQUE* pTech = m_cTechTable.GetDesc(nTechIdx);
		IZ_ASSERT(nPassIdx < pTech->numPass);

		IZ_UINT pos = pTech->posPass + nPassIdx;

		return pos;
	}

	// ファンクタ名取得
	IZ_PCSTR CPostEffectShader::GetFunctorName(IZ_UINT nTechIdx, IZ_UINT nPassIdx)
	{
		// 全てのテクニック・パスを通じたパスの位置を取得
		IZ_UINT pos = ConvertPassIdx(nTechIdx, nPassIdx);

		const S_PES_PASS* pDesc = m_cPassTable.GetDesc(pos);
		VRETURN_NULL(pDesc != IZ_NULL);

		IZ_PCSTR ret = m_cStrTable.GetString(pDesc->ann.posNameFunctor);
		
		return ret;
	}

	// パス名取得
	IZ_PCSTR CPostEffectShader::GetPassName(IZ_UINT nTechIdx, IZ_UINT nPassIdx)
	{
		// 全てのテクニック・パスを通じたパスの位置を取得
		IZ_UINT pos = ConvertPassIdx(nTechIdx, nPassIdx);

		// パス記述取得
		const S_PES_PASS* pDesc = m_cPassTable.GetDesc(pos);
		IZ_ASSERT(pDesc != IZ_NULL);

		IZ_PCSTR ret = m_cStrTable.GetString(pDesc->posName);

		return ret;
	}

	// ファンクタ引数取得
	const IZ_FLOAT* CPostEffectShader::GetFunctorArgs(IZ_UINT nTechIdx, IZ_UINT nPassIdx)
	{
		IZ_UINT pos = ConvertPassIdx(nTechIdx, nPassIdx);

		const S_PES_PASS* pDesc = m_cPassTable.GetDesc(pos);
		IZ_ASSERT(pDesc != IZ_NULL);

		return pDesc->ann.FunctorArgs;
	}

	// パス取得
	CPostEffectPass* CPostEffectShader::GetPass(IZ_UINT idx)
	{
		IZ_ASSERT(m_pPass != IZ_NULL);
		IZ_ASSERT(idx < m_cPassTable.GetPassNum());
		CPostEffectPass* ret = &m_pPass[idx];
		return ret;
	}

	// パス取得
	CPostEffectPass* CPostEffectShader::GetPass(IZ_UINT nTechIdx, IZ_UINT nPassIdx)
	{
		IZ_UINT pos = ConvertPassIdx(nTechIdx, nPassIdx);
		CPostEffectPass* ret = GetPass(pos);
		return ret;
	}

	// うーん・・・
	const S_PES_PASS_ANN* CPostEffectShader::GetPassAnn(IZ_UINT nTechIdx, IZ_UINT nPassIdx)
	{
		// 全体を通じたパス位置を取得
		IZ_UINT pos = ConvertPassIdx(nTechIdx, nPassIdx);

		const S_PES_PASS* pDesc = m_cPassTable.GetDesc(pos);
		IZ_ASSERT(pDesc != IZ_NULL);

		return &pDesc->ann;
	}

	/**
	* アルファブレンドステートをセット
	*/
	void CPostEffectShader::SetAlphaBlendState(
		IZ_BOOL bEnable,
		IZ_UINT nBlendMethod)
	{
		m_StateAlphaBlend.isDirty = IZ_TRUE;
		m_StateAlphaBlend.enable = bEnable;
		m_StateAlphaBlend.method = nBlendMethod;
	}

	/**
	* アルファテストステートをセット
	*/
	void CPostEffectShader::SetAlphaTestState(
		IZ_BOOL bEnable,
		E_GRAPH_CMP_FUNC nFunc,
		IZ_UINT8 nRef)
	{
		m_StateAlphaTest.isDirty = IZ_TRUE;
		m_StateAlphaTest.enable = bEnable;
		m_StateAlphaTest.func = nFunc;
		m_StateAlphaTest.ref = nRef;
	}

	/**
	* テクスチャインデックス取得
	*/
	IZ_INT CPostEffectShader::GetSrcTexIdx(IZ_UINT nSamplerIdx, IZ_UINT nTechIdx, IZ_UINT nPassIdx)
	{
		IZ_INT ret = -1;

		CPostEffectPass* pPass = GetPass(nTechIdx, nPassIdx);

		if ((pPass->GetSamplerNum() > 0)
			&& (nSamplerIdx < pPass->GetSamplerNum()))
		{
			IZ_UINT nDescIdx = pPass->GetSamplerInfo(nSamplerIdx)->idx;
			const S_PES_SAMPLER* pDesc = GetSamplerTable().GetDesc(nDescIdx);
			IZ_ASSERT(pDesc != IZ_NULL);

			ret = pDesc->state.BindTexIdx;
		}

		return ret;
	}

	/**
	* テクスチャインデックス取得
	*/
	IZ_INT CPostEffectShader::GetDstTexIdx(IZ_UINT nTechIdx, IZ_UINT nPassIdx)
	{
		IZ_INT ret = -1;

		IZ_UINT pos = ConvertPassIdx(nTechIdx, nPassIdx);

		const S_PES_PASS* pDesc = GetPassTable().GetDesc(pos);
		IZ_ASSERT(pDesc != IZ_NULL);

		if (pDesc != IZ_NULL) {
			ret = pDesc->ann.RenderTargetIdx;
		}

		return ret;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SHADER_H__)
