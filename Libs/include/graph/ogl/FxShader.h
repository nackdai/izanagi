#if !defined(__URANUS_GRAPH_FX_SHADER_H__)
#define __URANUS_GRAPH_FX_SHADER_H__

#include "unDefs.h"
#include "unD3DDefs.h"
#include "unStd.h"

namespace uranus {
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
			UN_UINT nProgramLen);

		// Creates instance of CFxShader from effect file.
		static CFxShader* CreateFxShaderFromFile(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_PCSTR pszName);

		static CFxShader* CreateFxShader(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice);

	private:
		template <typename _T, typename _Func>
		static CFxShader* CreateFxShader(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			_T tInput,
			UN_UINT nInputLen,
			_Func& func);

	public:
		/**
		* Description of paramter.
		*/
		struct SParamDesc {
			UN_PCSTR Name;
			UN_PCSTR Semantic;

			UN_UINT Rows;
			UN_UINT Columns;
			UN_UINT Elements;
			UN_UINT Bytes;
		};

	protected:
		CFxShader();
		~CFxShader();

		NO_COPIABLE(CFxShader);

	protected:
		void InternalRelease();

	public:
		UN_BOOL SetProgram(
			const void* pProgram,
			UN_UINT nProgramLen);

		// Retruns num of techniques.
		UN_UINT GetTechniqueNum();

		UN_BOOL SetTechnique(UN_UINT nTechIdx);

		UN_BOOL Begin(UN_UINT* pPassNum);
		UN_BOOL End();

		UN_BOOL BeingPass(UN_UINT nPassIdx);
		UN_BOOL EndPass();

		// Returns handle of parameter by name.
		SHADER_PARAM_HANDLE GetParameterByName(UN_PCSTR pszName);

		// Returns handle of parameter by semantic.
		SHADER_PARAM_HANDLE GetParameterBySemantic(UN_PCSTR pszSemantic);

		// Gets description of parameter.
		UN_BOOL GetParameterDesc(
			SHADER_PARAM_HANDLE hParam,
			SParamDesc* pDesc);

		// Sets value of parameter.
		UN_BOOL SetValue(
			SHADER_PARAM_HANDLE hParam,
			void* pValue,
			UN_UINT nSize);

	public:
		D3D_EFFECT* GetRawInterface() { return m_pBody; }

	protected:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		D3D_EFFECT* m_pBody;
		D3DXEFFECT_DESC m_sDesc;

		UN_UINT m_nCurTechPassNum;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_FX_SHADER_H__)
