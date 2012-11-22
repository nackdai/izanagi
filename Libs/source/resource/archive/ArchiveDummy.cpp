#include "resource/archive/ArchiveDummy.h"

using namespace izanagi;

// Get InputStream.
IInputStream* CArchiveDummy::GetInputStream()
{
	// TOOD
	return &m_Input;
}

// Seek file.
IZ_BOOL CArchiveDummy::Seek(IZ_PCSTR path)
{
	m_Input.Finalize();
	IZ_BOOL ret = m_Input.Open(path);
	IZ_ASSERT(ret);
	return ret;
}
