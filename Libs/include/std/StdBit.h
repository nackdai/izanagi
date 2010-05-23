#if !defined(__URANUS_STD_STD_BIT_UTIL_H__)
#define __URANUS_STD_STD_BIT_UTIL_H__

#include "unDefs.h"

namespace uranus {
	/////////////////////////////////////////////////////////
	/**
	*/
	class CStdBitInput {
	public:
		inline CStdBitInput(void* src, size_t size);
		~CStdBitInput() {}

		NO_COPIABLE(CStdBitInput);

	public:
		inline UN_UINT Input(UN_UINT nBitCnt);

		UN_UINT GetInByte() const { return m_nInByte; }
		UN_BOOL IsValid() const { return (m_nCurPos == 0); }

	private:
		UN_BYTE* m_pSrc;
		UN_UINT m_nSize;

		UN_BYTE m_nTmp;
		UN_UINT m_nCurPos;

		UN_UINT m_nInByte;
	};

	// inline ********************************

	CStdBitInput::CStdBitInput(void* src, size_t size)
	{
		m_pSrc = reinterpret_cast<UN_BYTE*>(src);
		m_nSize = static_cast<UN_UINT>(size);

		m_nTmp = 0;
		m_nCurPos = 0;

		m_nInByte = 0;
	}

	/**
	*/
	UN_UINT CStdBitInput::Input(UN_UINT nBitCnt)
	{
		UN_ASSERT(m_nInByte < m_nSize);
		UN_ASSERT(nBitCnt <= 32);

		if (m_nInByte >= m_nSize) {
			// Nothing is done.
			return 0;
		}

		// Max 32 bit (= 4 byte)
		nBitCnt = (nBitCnt > 32 ? 32 : nBitCnt);

		UN_UINT ret = 0;

		for (UN_UINT i = 0; i < nBitCnt; i++) {
			if (m_nCurPos == 0) {
				m_nTmp = *(m_pSrc + m_nInByte);
			}

			ret = (ret << 1) | ((m_nTmp >> m_nCurPos) & 0x01);
			m_nCurPos++;

			if (m_nCurPos == 8) {
				m_nInByte++;
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
		inline UN_BOOL Out(UN_UINT val, UN_UINT nBitCnt);
		inline UN_BOOL Flush();

		void* GetDst() { return m_pDst; }
		UN_UINT GetOutByte() const { return m_nOutByte; }
		UN_BOOL IsValid() const { return (m_nCurPos == 0); }

	private:
		UN_BYTE* m_pDst;
		UN_UINT m_nSize;

		UN_BYTE m_nTmp;
		UN_UINT m_nCurPos;

		UN_UINT m_nOutByte;
	};

	// inline ********************************

	CStdBitOutput::CStdBitOutput(void* dst, size_t size)
	{
		m_pDst = reinterpret_cast<UN_BYTE*>(dst);
		m_nSize = static_cast<UN_UINT>(size);

		m_nTmp = 0;
		m_nCurPos = 0;

		m_nOutByte = 0;
	}

	/**
	*/
	UN_BOOL CStdBitOutput::Out(UN_UINT val, UN_UINT nBitCnt)
	{
		for (UN_UINT i = 0; i < nBitCnt; i++) {
			m_nTmp = ((m_nTmp << 1) | ((val >> i) & 0x01));
			m_nCurPos++;

			if (m_nCurPos == 8) {
				VRETURN(Flush());
			}
		}

		return UN_TRUE;
	}

	/**
	*/
	UN_BOOL CStdBitOutput::Flush()
	{
		if (m_nOutByte >= m_nSize) {
			return UN_FALSE;
		}

		if (m_nCurPos > 0) {
			*(m_pDst++) = m_nTmp;

			m_nTmp = 0;
			m_nCurPos = 0;

			m_nOutByte++;
		}

		return UN_TRUE;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STD_BIT_UTIL_H__)
