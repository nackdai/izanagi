#if !defined(__IZANAGI_TOOL_KIT_IO_IO_UTIL_H__)
#define __IZANAGI_TOOL_KIT_IO_IO_UTIL_H__

#include "io/stream/OutputStream.h"

namespace izanagi {
namespace tool {
	class CIoStreamSeekHelper {
	protected:
		CIoStreamSeekHelper();

	public:
		CIoStreamSeekHelper(IOutputStream* pOut)
		{
			m_pOut = pOut;
			m_nPos = 0;
			m_nAnchorPos = 0;
		}

		~CIoStreamSeekHelper() {}

	public:
		IZ_BOOL Skip(IZ_UINT nSkip)
		{
			m_nPos = m_pOut->GetCurPos();
			VRETURN(m_pOut->Seek(nSkip, E_IO_STREAM_SEEK_POS_CUR));
			return IZ_TRUE;
		}

		void Step(IZ_UINT nStep)
		{
			m_nPos += nStep;
		}

		IZ_BOOL Return()
		{
			IZ_UINT nCurPos = m_pOut->GetCurPos();
			IZ_INT nOffset = m_nPos - nCurPos;
			VRETURN(m_pOut->Seek(nOffset, E_IO_STREAM_SEEK_POS_CUR));
			return IZ_TRUE;
		}

		IZ_BOOL ReturnWithAnchor()
		{
			m_nAnchorPos = m_pOut->GetCurPos();
			IZ_INT nOffset = m_nPos - m_nAnchorPos;
			VRETURN(m_pOut->Seek(nOffset, E_IO_STREAM_SEEK_POS_CUR));
			return IZ_TRUE;
		}

		IZ_BOOL ReturnToAnchor()
		{
			IZ_UINT nCurPos = m_pOut->GetCurPos();
			IZ_INT nOffset = m_nAnchorPos - nCurPos;
			VRETURN(m_pOut->Seek(nOffset, E_IO_STREAM_SEEK_POS_CUR));
			return IZ_TRUE;
		}

		IOutputStream* GetOutputStream() { return m_pOut; }

	protected:
		IOutputStream* m_pOut;
		IZ_UINT m_nPos;
		IZ_UINT m_nAnchorPos;
	};
}	// namespace tool
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_TOOL_KIT_IO_IO_UTIL_H__)
