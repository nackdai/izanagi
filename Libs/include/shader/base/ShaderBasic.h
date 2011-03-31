#if !defined(__IZANAGI_SHADER_SHADER_BASIC_H__)
#define __IZANAGI_SHADER_SHADER_BASIC_H__

#include "izStd.h"
#include "izGraph.h"

#include "Shader.h"
#include "table/ShaderParameterTable.h"
#include "table/ShaderSamplerTable.h"
#include "table/ShaderStringBuffer.h"
#include "table/ShaderPassTable.h"
#include "table/ShaderTechTable.h"
#include "table/ShaderTextureTable.h"
#include "ShaderPass.h"
#include "ShaderDefs.h"

namespace izanagi {
	class IInputStream;

	/**
	*/
	class CShaderBasic : public CShader {
	public:
		/**
		* Creates an instance of CShaderBasic.
		*/
		template <typename _SHADER>
		static _SHADER* CreateShader(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IInputStream* pIn)
		{
			void* pBuf = ALLOC_ZERO(pAllocator, sizeof(_SHADER));
			VRETURN_NULL(pBuf != IZ_NULL);

			_SHADER* pInstance = new(pBuf) _SHADER;
			{
				pInstance->AddRef();
				pInstance->m_pAllocator = pAllocator;
				SAFE_REPLACE(pInstance->m_pDevice, pDevice);

				if (!pInstance->Init(pDevice, pIn)) {
					SAFE_RELEASE(pInstance);
					IZ_ASSERT(IZ_FALSE);
				}
			}

			return pInstance;
		}

	protected:
		CShaderBasic();
		virtual ~CShaderBasic();

		NO_COPIABLE(CShaderBasic);

	protected:
		IZ_BOOL Init(
			CGraphicsDevice* pDevice,
			IInputStream* pIn);

		IZ_UINT8* CreatePass(
			CGraphicsDevice* pDevice,
			IInputStream* pIn,
			IZ_UINT8* pBuffer);

	public:
		IZ_PCSTR GetName() const { return m_Header.name.GetString(); }
		IZ_UINT GetKey() const { return m_Header.nameKey; }

		virtual IZ_UINT Begin(
			IZ_UINT nTechIdx,
			IZ_BOOL bIsSaveState);
		virtual IZ_BOOL End();

		virtual IZ_BOOL BeginPass(IZ_UINT nPassIdx);
		virtual IZ_BOOL EndPass();

		virtual IZ_BOOL CommitChages();

		virtual IZ_UINT GetTechNum() const;

	public:
		IZ_SHADER_HANDLE GetParameterByName(IZ_PCSTR pszName);
		IZ_SHADER_HANDLE GetParameterBySemantic(IZ_PCSTR pszSemantic);

		IZ_BOOL GetParameterDesc(
			IZ_SHADER_HANDLE handle, 
			SShaderParamDesc& sDesc) const;

		IZ_BOOL SetParamValue(
			IZ_SHADER_HANDLE hParam,
			const void* pValue,
			IZ_UINT nBytes);

		IZ_BOOL SetTexture(
			IZ_SHADER_HANDLE hTex,
			CBaseTexture* pTex);

		IZ_BOOL SetTextureToSampler(
			IZ_SHADER_HANDLE hSmpl,
			CBaseTexture* pTex);

		CGraphicsDevice* GetDevice();

	private:
		template <typename _SHD>
		IZ_BOOL SetParamValue(
			IZ_UINT idx,
			CShaderPass& cPass,
			_SHD* pShd);

	protected:
		IZ_UINT8* m_pBuffer;

		S_SHD_HEADER m_Header;

		CShaderStringBuffer   m_StringBuffer;	// strings of shader.
		CShaderParameterTable m_ParamTbl;		// paramters of shader.
		CShaderTextureTable   m_TexTbl;			// textures of shader.
		CShaderSamplerTable   m_SmplTbl;		// samplers of shader.
		CShaderPassTable      m_PassTbl;		// passed of shader.
		CShaderTechTable      m_TechTbl;		// techniques of shader.

		CShaderPass* m_pPass;

		IZ_BOOL m_bIsSavedRS;
		IZ_INT16 m_nCurTech;
		IZ_INT16 m_nCurPass;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SHADER_SHADER_BASIC_H__)
