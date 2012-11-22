#if !defined(__IZANAGI_STD_STD_BIT_UTIL_H__)
#define __IZANAGI_STD_STD_BIT_UTIL_H__

#include "izDefs.h"

namespace izanagi {
	/////////////////////////////////////////////////////////
	/**
	*/
	class CStdBitInput {
	public:
		inline CStdBitInput(void* src, size_t size);
		~CStdBitInput() {}

		NO_COPIABLE(CStdBitInput);

	public:
		inline IZ_UINT Input(IZ_UINT nBitCnt);

		IZ_UINT GetInByte() const { return m_nInByte; }
		IZ_BOOL IsValid() const { return (m_nCurPos == 0); }

	private:
		IZ_BYTE* m_pSrc;
		IZ_UINT m_nSize;

		IZ_BYTE m_nTmp;
		IZ_UINT m_nCurPos;

		IZ_UINT m_nInByte;
	};

	// inline ********************************

	CStdBitInput::CStdBitInput(void* src, size_t size)
	{
		m_pSrc = reinterpret_cast<IZ_BYTE*>(src);
		m_nSize = static_cast<IZ_UINT>(size);

		m_nTmp = 0;
		m_nCurPos = 0;

		m_nInByte = 0;
	}

	/**
	*/
	IZ_UINT CStdBitInput::Input(IZ_UINT nBitCnt)
	{
		IZ_ASSERT(m_nInByte < m_nSize);
		IZ_ASSERT(nBitCnt <= 32);

		if (m_nInByte >= m_nSize) {
			// Nothing is done.
			return 0;
		}

		// Max 32 bit (= 4 byte)
		nBitCnt = (nBitCnt > 32 ? 32 : nBitCnt);

		IZ_UINT ret = 0;

		for (IZ_UINT i = 0; i < nBitCnt; ++i) {
			if (m_nCurPos == 0) {
				m_nTmp = *(m_pSrc + m_nInByte);
			}

			ret = (ret << 1) | ((m_nTmp >> m_nCurPos) & 0x01);
			++m_nCurPos;

			if (m_nCurPos == 8) {
				++m_nInByte;
				m_nCurPos = 0;
			}
			if (m_nInByte >= m_nSize) {
				break;
			}
		}

		return ret;
	}

	/////////////////////////////////////////////////////////
	/**
	*/
	class CStdBitOutput {
	public:
		inline CStdBitOutput(void* dst, size_t size);
		~CStdBitOutput() {}

		NO_COPIABLE(CStdBitOutput);

	public:
		inline IZ_BOOL Out(IZ_UINT val, IZ_UINT nBitCnt);
		inline IZ_BOOL Flush();

		void* GetDst() { return m_pDst; }
		IZ_UINT GetOutByte() const { return m_nOutByte; }
		IZ_BOOL IsValid() const { return (m_nCurPos == 0); }

	private:
		IZ_BYTE* m_pDst;
		IZ_UINT m_nSize;

		IZ_BYTE m_nTmp;
		IZ_UINT m_nCurPos;

		IZ_UINT m_nOutByte;
	};

	// inline ********************************

	CStdBitOutput::CStdBitOutput(void* dst, size_t size)
	{
		m_pDst = reinterpret_cast<IZ_BYTE*>(dst);
		m_nSize = static_cast<IZ_UINT>(size);

		m_nTmp = 0;
		m_nCurPos = 0;

		m_nOutByte = 0;
	}

	/**
	*/
	IZ_BOOL CStdBitOutput::Out(IZ_UINT val, IZ_UINT nBitCnt)
	{
		for (IZ_UINT i = 0; i < nBitCnt; ++i) {
			m_nTmp = ((m_nTmp << 1) | ((val >> i) & 0x01));
			++m_nCurPos;

			if (m_nCurPos == 8) {
				VRETURN(Flush());
			}
		}

		return IZ_TRUE;
	}

	/**
	*/
	IZ_BOOL CStdBitOutput::Flush()
	{
		if (m_nOutByte >= m_nSize) {
			return IZ_FALSE;
		}

		if (m_nCurPos > 0) {
			*(m_pDst++) = m_nTmp;

			m_nTmp = 0;
			m_nCurPos = 0;

			++m_nOutByte;
		}

		return IZ_TRUE;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_STD_BIT_UTIL_H__)
