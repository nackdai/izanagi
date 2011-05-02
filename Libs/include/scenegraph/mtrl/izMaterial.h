#if	!defined(__IZANAGI_SCENEGRAPH_IZ_MATERIAL_H__)
#define __IZANAGI_SCENEGRAPH_IZ_MATERIAL_H__

#include "izStd.h"
#include "MTRLFormat.h"

namespace izanagi {
	class IInputStream;
	class CGraphicsDevice;
	class CShader;

	/**
	*/
	class CMaterial : public CObject {
	public:
		static IZ_BOOL CreateMaterial(
			IMemoryAllocator* pAllocator,
			IInputStream* pIn);

		static CMaterial* CreateMaterial(
			IMemoryAllocator* pAllocator,
			IZ_PCSTR pszName,
			IZ_UINT nTexNum,
			IZ_UINT nParamNum,
			IZ_UINT nParamBytes);

	private:
		static CMaterial* CreateMaterial(
			IZ_UINT nTexNum,
			IZ_UINT nParamNum,
			IZ_UINT nParamBytes,
			IMemoryAllocator* pAllocator);

	private:
		CMaterial();
		~CMaterial();

		NO_COPIABLE(CMaterial);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		IZ_BOOL Prepare(CGraphicsDevice* pDevice);

		IZ_BOOL AddTexture(
			IZ_PCSTR pszName,
			const S_MTRL_TEXTURE_TYPE& type,
			CBaseTexture* pTex);

		IZ_BOOL SetTexture(
			IZ_PCSTR pszName, 
			CBaseTexture* pTex);
		IZ_BOOL SetTexture(
			IZ_UINT nKey,
			CBaseTexture* pTex);

		IZ_BOOL AddShader(CShader* pShader);
		IZ_BOOL SetShader(CShader* pShader);

		IZ_BOOL AddParameter(
			IZ_PCSTR pszName,
			E_MTRL_PARAM_TYPE type,
			IZ_UINT nNum,
			const void* pValue);

		IZ_BOOL SetParameter(
			IZ_PCSTR pszName,
			IZ_UINT nBytes,
			const void* pValue);
		IZ_BOOL SetParameter(
			IZ_UINT nKey,
			IZ_UINT nBytes,
			const void* pValue);

		IZ_BOOL IsValid() const;

	public:
		IZ_UINT GetTexNum() const { return m_Header.numTex; }
#if 0
		IZ_UINT GetShaderNum() const { return m_Header.numShader; }
#endif
		IZ_UINT GetParamNum() const { return m_Header.numParam; }

		const S_MTRL_TEXTURE* GetTexInfoByIdx(IZ_UINT idx) const;
		const S_MTRL_TEXTURE* GetTexInfoByName(IZ_PCSTR pszName) const;
		const S_MTRL_TEXTURE* GetTexInfoByKey(const CKey& key) const;

#if 0
		const S_MTRL_SHADER* GetShaderInfoByIdx(IZ_UINT idx) const;
		const S_MTRL_SHADER* GetShaderInfoByName(IZ_PCSTR pszName) const;
		const S_MTRL_SHADER* GetShaderInfoByKey(const CKey& key) const;
#else
		const S_MTRL_SHADER* GetShaderInfo() const { return m_pShaderInfo; }
#endif

		const S_MTRL_PARAM* GetParamInfoByIdx(IZ_UINT idx) const;
		const S_MTRL_PARAM* GetParamInfoByName(IZ_PCSTR pszName) const;
		const S_MTRL_PARAM* GetParamInfoByKey(const CKey& key) const;

		CBaseTexture* GetTextureByIdx(IZ_UINT idx);
		CBaseTexture* GetTextureByName(IZ_PCSTR pszName);
		CBaseTexture* GetTextureByKey(const CKey& key);

#if 0
		CShader* GetShaderByIdx(IZ_UINT idx);
		CShader* GetShaderByName(IZ_PCSTR pszName);
		CShader* GetShaderByKey(const CKey& key);
#else
		CShader* GetShader()
		{
			return (m_pShaderInfo != IZ_NULL ? m_pShaderInfo->shader : IZ_NULL);
		}
#endif

		IZ_BOOL SetShaderTechnique(IZ_UINT idx);
		IZ_INT GetShaderTechnique() const;

		const void* GetParamByIdx(IZ_UINT idx) const;
		const void* GetParamByName(IZ_PCSTR pszName) const;
		const void* GetParamByKey(const CKey& key) const;

		CStdList<CMaterial>::Item* GetListItem() { return &m_ListItem; }

	private:
		IZ_BOOL Read(IInputStream* pIn);

		void AttachParamBuf();

	private:
		IMemoryAllocator* m_pAllocator;

		IZ_BOOL m_IsFromMtrlFile;
		IZ_UINT m_nAttachBufPos;

		S_MTRL_MATERIAL m_Header;

		S_MTRL_TEXTURE* m_pTexInfo;

		S_MTRL_SHADER* m_pShaderInfo;

		S_MTRL_PARAM* m_pParamInfo;
		IZ_UINT8* m_pParamBuf;

		CStdList<CMaterial>::Item m_ListItem;
	};
}	// namespace izanagi

#endif	// #ifndef(__IZANAGI_SCENEGRAPH_IZ_MATERIAL_H__)
