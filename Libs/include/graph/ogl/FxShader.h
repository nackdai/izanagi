#if !defined(__IZANAGI_GRAPH_FX_SHADER_H__)
#define __IZANAGI_GRAPH_FX_SHADER_H__

#include "izDefs.h"
#include "izD3DDefs.h"
#include "izStd.h"

namespace izanagi {
	class IMemoryAllocator;
	class CGraphicsDevice;

	/**
	* Shader for effect.
	*/
	class CFxShader : public CObject {
	public:
		// Creates instance of CFxShader from binary data.
		static CFxShader* CreateFxShader(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			const void* pProgram,
			IZ_UINT nProgramLen);

		// Creates instance of CFxShader from effect file.
		static CFxShader* CreateFxShaderFromFile(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IZ_PCSTR pszName);

		static CFxShader* CreateFxShader(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice);

	private:
		template <typename _T, typename _Func>
		static CFxShader* CreateFxShader(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			_T tInput,
			IZ_UINT nInputLen,
			_Func& func);

	public:
		/**
		* Description of paramter.
		*/
		struct SParamDesc {
			IZ_PCSTR Name;
			IZ_PCSTR Semantic;

			IZ_UINT Rows;
			IZ_UINT Columns;
			IZ_UINT Elements;
			IZ_UINT Bytes;
		};

	protected:
		CFxShader();
		~CFxShader();

		NO_COPIABLE(CFxShader);

	protected:
		void InternalRelease();

	public:
		IZ_BOOL SetProgram(
			const void* pProgram,
			IZ_UINT nProgramLen);

		// Retruns num of techniques.
		IZ_UINT GetTechniqueNum();

		IZ_BOOL SetTechnique(IZ_UINT nTechIdx);

		IZ_BOOL Begin(IZ_UINT* pPassNum);
		IZ_BOOL End();

		IZ_BOOL BeingPass(IZ_UINT nPassIdx);
		IZ_BOOL EndPass();

		// Returns handle of parameter by name.
		SHADER_PARAM_HANDLE GetParameterByName(IZ_PCSTR pszName);

		// Returns handle of parameter by semantic.
		SHADER_PARAM_HANDLE GetParameterBySemantic(IZ_PCSTR pszSemantic);

		// Gets description of parameter.
		IZ_BOOL GetParameterDesc(
			SHADER_PARAM_HANDLE hParam,
			SParamDesc* pDesc);

		// Sets value of parameter.
		IZ_BOOL SetValue(
			SHADER_PARAM_HANDLE hParam,
			void* pValue,
			IZ_UINT nSize);

	public:
		D3D_EFFECT* GetRawInterface() { return m_pBody; }

	protected:
		IMemoryAllocator* m_Allocator;
		CGraphicsDevice* m_pDevice;

		D3D_EFFECT* m_pBody;
		D3DXEFFECT_DESC m_sDesc;

		IZ_UINT m_nCurTechPassNum;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_FX_SHADER_H__)
