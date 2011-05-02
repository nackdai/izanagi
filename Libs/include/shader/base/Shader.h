#if !defined(__IZANAGI_SHADER_SHADER_H__)
#define __IZANAGI_SHADER_SHADER_H__

#include "izStd.h"
#include "izGraph.h"

#include "ShaderDefs.h"

namespace izanagi {
	enum {
		SHD_HASH_MAX = 5,
	};

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
	class CShaderAttr {
	public:
		~CShaderAttr() {}

	private:
		CShaderAttr() {}

		NO_COPIABLE(CShaderAttr);

	public:
		CShaderAttr(IZ_PCSTR pszName, IZ_FLOAT fVal) { Set(pszName, fVal); }
		CShaderAttr(IZ_PCSTR pszName, IZ_UINT nVal) { Set(pszName, nVal); }
		CShaderAttr(IZ_PCSTR pszName, IZ_BOOL bVal) { Set(pszName, bVal); }

		void Set(IZ_PCSTR pszName, IZ_FLOAT fVal) { Set(pszName, E_SHADER_PARAMETER_TYPE_FLOAT, fVal); }
		void Set(IZ_PCSTR pszName, IZ_UINT nVal) { Set(pszName, E_SHADER_PARAMETER_TYPE_INT, nVal); }
		void Set(IZ_PCSTR pszName, IZ_BOOL bVal) { Set(pszName, E_SHADER_PARAMETER_TYPE_BOOL, bVal); }

		IZ_PCSTR GetName() const { return m_Name; }
		E_SHADER_PARAMETER_TYPE GetType() const { return m_Type; }

		IZ_UINT GetValueAsUInt() const { return *(IZ_UINT*)(m_Value); }
		IZ_FLOAT GetValueAsFloat() const { return *(IZ_FLOAT*)(m_Value); }
		IZ_BOOL GetValueAsBool() const { return *(IZ_BOOL*)(m_Value); }

	private:
		template <typename _T>
		void Set(
			IZ_PCSTR pszName, 
			E_SHADER_PARAMETER_TYPE nType,
			_T val)
		{
			m_Name = pszName;
			m_Type = nType;
			memcpy(m_Value, &val, sizeof(_T));
		}

	private:
		IZ_PCSTR m_Name;
		E_SHADER_PARAMETER_TYPE m_Type;
		IZ_UINT8 m_Value[4];
	};

	/**
	*/
	class CShader : public CObject {
		friend class CShaderManager;

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

		PURE_VIRTUAL(IZ_BOOL CommitChanges());

		PURE_VIRTUAL(IZ_SHADER_HANDLE GetParameterByName(IZ_PCSTR pszName));
		PURE_VIRTUAL(IZ_SHADER_HANDLE GetParameterBySemantic(IZ_PCSTR pszSemantic));

		PURE_VIRTUAL(
			IZ_BOOL GetParameterDesc(
				IZ_SHADER_HANDLE handle, 
				SShaderParamDesc& sDesc) const);

		PURE_VIRTUAL(
			IZ_BOOL SetParamValue(
				IZ_SHADER_HANDLE hParam,
				const void* pValue,
				IZ_UINT nBytes));

		PURE_VIRTUAL(
			IZ_BOOL SetTexture(
				IZ_SHADER_HANDLE hTex,
				CBaseTexture* pTex));

		PURE_VIRTUAL(
			IZ_BOOL SetTextureToSampler(
				IZ_SHADER_HANDLE hSmpl,
				CBaseTexture* pTex));

		PURE_VIRTUAL(IZ_UINT GetTechNum() const);

		PURE_VIRTUAL(IZ_UINT GetAttrNum() const);
		PURE_VIRTUAL(IZ_PCSTR GetAttrName(IZ_UINT idx) const);
		PURE_VIRTUAL(IZ_BOOL CmpAttr(const CShaderAttr& attr) const);
		PURE_VIRTUAL(IZ_BOOL CmpAttrValue(IZ_PCSTR attrName, IZ_UINT val) const);
		PURE_VIRTUAL(IZ_BOOL CmpAttrValue(IZ_PCSTR attrName, IZ_FLOAT val) const);
		PURE_VIRTUAL(IZ_BOOL CmpAttrValue(IZ_PCSTR attrName, IZ_BOOL val) const);

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

		void RemoveFromShaderManager()
		{
			if (m_HashItem.HasList()) {
				this->Release();
				m_HashItem.Leave();
			}
		}

	protected:
		void InitItem(IZ_UINT key, CShader* pShader)
		{
			m_HashItem.Init(key, pShader);
		}

	private:
		CStdHash<IZ_UINT, CShader, SHD_HASH_MAX>::Item* GetHashItem() { return &m_HashItem; }

	protected:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

	private:
		CStdHash<IZ_UINT, CShader, SHD_HASH_MAX>::Item m_HashItem;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SHADER_SHADER_H__)
