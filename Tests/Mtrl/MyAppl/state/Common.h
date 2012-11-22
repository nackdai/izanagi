#if !defined(__COMMON_H__)
#define __COMMON_H__

#include "izDefs.h"
#include "izShader.h"

//////////////////////////////////////////////////////

class CShaderTest : public izanagi::CShaderBasic {
	friend class izanagi::CShaderBasic;

protected:
	CShaderTest() {}
	~CShaderTest() {}

public:
	inline izanagi::IZ_SHADER_HANDLE GetParameterByName(IZ_PCSTR pszName);
	inline izanagi::IZ_SHADER_HANDLE GetParameterBySemantic(IZ_PCSTR pszSemantic);
};

#define STR_CMP(str0, str1)	(memcmp(str0, str1, strlen(str1)) == 0)

izanagi::IZ_SHADER_HANDLE CShaderTest::GetParameterByName(IZ_PCSTR pszName)
{
	if (STR_CMP(pszName, "diffuse")) {
		return CShaderBasic::GetParameterByName("g_vMtrlDiffuse");
	}
	else if (STR_CMP(pszName, "specular")) {
		return CShaderBasic::GetParameterByName("g_vMtrlSpecular");
	}
	else if (STR_CMP(pszName, "emission")) {
		return CShaderBasic::GetParameterByName("g_vMtrlAmbient");
	}

	return CShaderBasic::GetParameterByName(pszName);
}

izanagi::IZ_SHADER_HANDLE CShaderTest::GetParameterBySemantic(IZ_PCSTR pszSemantic)
{
	// Nothing is done.
	return 0;
}

///////////////////////////////////////////////

class CTestMdlRenderHandler : public izanagi::IMshRenderHandler {
	friend class izanagi::IMshRenderHandler;

private:
	CTestMdlRenderHandler()
	{
		m_pShader = IZ_NULL;
	}

	~CTestMdlRenderHandler()
	{
		SAFE_RELEASE(m_pShader);
	}

public:
	inline void BeginRenderMesh();
	inline void EndRenderMesh();

	inline void SetJointMatrix(
		IZ_UINT nIdx,
		const izanagi::SMatrix& mtx);

	inline void CommitChanges();

public:
	void SetShader(izanagi::CShaderBasic* pShader)
	{
		SAFE_REPLACE(m_pShader, pShader);
	}

private:
	izanagi::CShaderBasic* m_pShader;

	IZ_UINT m_nCnt;
	izanagi::SMatrix m_Mtx[48];

	izanagi::IZ_SHADER_HANDLE m_Handle;
};

void CTestMdlRenderHandler::BeginRenderMesh()
{
	m_nCnt = 0;

	izanagi::SMatrix::SetUnit(m_Mtx[0]);
	izanagi::SMatrix::SetUnit(m_Mtx[1]);
	izanagi::SMatrix::SetUnit(m_Mtx[2]);
	izanagi::SMatrix::SetUnit(m_Mtx[3]);

	m_Handle = 0;
}

void CTestMdlRenderHandler::EndRenderMesh()
{
}

void CTestMdlRenderHandler::SetJointMatrix(
	IZ_UINT nIdx,
	const izanagi::SMatrix& mtx)
{
	izanagi::SMatrix::Copy(m_Mtx[m_nCnt], mtx);
	m_nCnt++;
}

void CTestMdlRenderHandler::CommitChanges()
{
	if (m_Handle == 0) {
		m_Handle = m_pShader->GetParameterByName("vJointMatrix");
		IZ_ASSERT(m_Handle > 0);
	}

	m_pShader->SetParamValue(
		m_Handle,
		m_Mtx,
		sizeof(izanagi::SMatrix) * m_nCnt);

	m_pShader->CommitChanges();
}

#endif	// #if !defined(__COMMON_H__)
