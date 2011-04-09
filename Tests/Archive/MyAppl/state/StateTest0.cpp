#include <stdafx.h>
#include "StateTest0.h"
#include "MySystem.h"
#include "izResource.h"

#include "../../data/test_0.h"

CStateTest0::CStateTest0()
{
}

CStateTest0::~CStateTest0()
{
}

IZ_BOOL CStateTest0::Create()
{
	return IZ_TRUE;
}

IZ_BOOL CStateTest0::Render()
{
	if (CMySystem::GetInstance().GetGraphicsDevice()->Begin2D()) {
		CMySystem::GetInstance().GetDebugFont()->Begin();

		CMySystem::GetInstance().GetDebugFont()->DBPrint(
			0xffffffff,
			"Decoding Archive Done\n");

		CMySystem::GetInstance().GetDebugFont()->End();

		CMySystem::GetInstance().GetGraphicsDevice()->End2D();
	}

	return IZ_TRUE;
}

IZ_BOOL CStateTest0::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CStateTest0::Destroy()
{
	return IZ_TRUE;
}

namespace {
	inline IZ_PCSTR _GetFileNameFromPath(IZ_PCSTR pszPath)
	{
		size_t len = strlen(pszPath);

		IZ_PCSTR ret = pszPath;

		for (IZ_INT i = len - 1; i >= 0; i--) {
			IZ_CHAR ch = pszPath[i];

			if (ch == '/') {
				ret = &pszPath[i + 1];
				break;
			}
		}

		return ret;
	}

	IZ_BOOL _ExportDataFromArchive(
		izanagi::CArchive* pArchive,
		IZ_UINT nKey)
	{
		static IZ_BYTE BUF[2048];

		izanagi::SArchiveFileDesc sDesc;

		VRETURN(pArchive->Seek(nKey));
		VRETURN(pArchive->GetFileDesc(&sDesc));

		IZ_PCSTR name = _GetFileNameFromPath(sDesc.name);

		izanagi::IInputStream* pArcStream = pArchive->GetInputStream();

		izanagi::CFileOutputStream out;
		VRETURN(out.Open(name));

		IZ_UINT nReadSize = sDesc.size;

		for (;;) {
			size_t nSize = (sizeof(BUF) < nReadSize ? sizeof(BUF) : nReadSize);
			nSize = pArcStream->Read(BUF, 0, nSize);
			if (nSize == 0) {
				break;
			}

			out.Write(BUF, 0, nSize);

			if (nReadSize == nSize) {
				break;
			}
			else {
				nReadSize -= nSize;
			}
		}

		return IZ_TRUE;
	}
}	// namespace

IZ_BOOL CStateTest0::Enter()
{
	
	izanagi::CFileInputStream input;
	VRETURN(input.Open("data/test_0.arc"));

	izanagi::CArchive* pArchive = izanagi::CArchiveImpl::CreateArchive(
									CMySystem::GetInstance().GetMemoryAllocator(),
									&input);
	IZ_ASSERT(pArchive != IZ_NULL);

	VGOTO(_ExportDataFromArchive(pArchive, test::a::ArchiveBuilderImpl_cpp), __EXIT__);
	VGOTO(_ExportDataFromArchive(pArchive, test::b::c::Tweener_cpp), __EXIT__);

__EXIT__:
	SAFE_RELEASE(pArchive);
	input.Close();

	return IZ_TRUE;
}

IZ_BOOL CStateTest0::Leave()
{
	return IZ_TRUE;
}
