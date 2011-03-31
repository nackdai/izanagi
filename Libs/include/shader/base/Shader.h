#if !defined(__IZANAGI_SHADER_SHADER_H__)
#define __IZANAGI_SHADER_SHADER_H__

#include "izStd.h"
#include "izGraph.h"

#include "ShaderDefs.h"

namespace izanagi {
	/**
	*/
	struct SShaderParamDesc {
		IZ_UINT8 columns;
		IZ_UINT8 rows;
		IZ_UINT8 elements;
		IZ_UINT8 padding_1;

		IZ_UINT16 bytes;
		IZ_UINT16 padding_2;

		E_SHADER_PARAMETER_CLASS clazz;
		E_SHADER_PARAMETER_TYPE type;

		SShaderParamDesc()
		{
			columns = 0;
			rows = 0;
			elements = 0;
			padding_1 = 0;

			bytes = 0;
			padding_2 = 0;

			clazz = E_SHADER_PARAMETER_CLASS_UNKNOWN;
			type = E_SHADER_PARAMETER_TYPE_NUM;
		}
	};

	/**
	*/
	class CShader : public CObject {
	protected:
		CShader()
		{
			m_pAllocator = IZ_NULL;
			m_pDevice = IZ_NULL;
		}

		virtual ~CShader() { SAFE_RELEASE(m_pDevice); }

		NO_COPIABLE(CShader);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		PURE_VIRTUAL(IZ_PCSTR GetName() const);
		PURE_VIRTUAL(IZ_UINT GetKey() const);

		PURE_VIRTUAL(
			IZ_UINT Begin(
				IZ_UINT nTechIdx,
				IZ_BOOL bIsSaveState));
		PURE_VIRTUAL(IZ_BOOL End());

		PURE_VIRTUAL(IZ_BOOL BeginPass(IZ_UINT nPassIdx));
		PURE_VIRTUAL(IZ_BOOL EndPass());

		PURE_VIRTUAL(IZ_BOOL CommitChages());

		PURE_VIRTUAL(IZ_SHADER_HANDLE GetParameterByName(IZ_PCSTR pszName));
		PURE_VIRTUAL(IZ_SHADER_HANDLE GetParameterBySemantic(IZ_PCSTR pszSemantic));

		PURE_VIRTUAL(
			IZ_BOOL GetParameterDesc(
				IZ_SHADER_HANDLE handle, 
				SShaderParamDesc& sDesc) const);

		PURE_VIRTUAL(IZ_UINT GetTechNum() const);

	public:
		IZ_BOOL HasParameterByName(IZ_PCSTR pszName)
		{
			IZ_SHADER_HANDLE handle = GetParameterByName(pszName);
			return (handle > 0);
		}

		IZ_BOOL HasParameterBySemantic(IZ_PCSTR pszSemantic)
		{
			IZ_SHADER_HANDLE handle = GetParameterBySemantic(pszSemantic);
			return (handle > 0);
		}

	protected:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SHADER_SHADER_H__)
