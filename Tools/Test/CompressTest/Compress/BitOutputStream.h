#if !defined(__BIT_OUTPUT_STREAM_H__)
#define __BIT_OUTPUT_STREAM_H__

#include "ToolkitIoDefs.h"

namespace izanagi {
	/**
	*/
	class CBitOutputStream {
	public:
		CBitOutputStream()
		{
			m_pOutput = IZ_NULL;

			FILL_ZERO(m_nBuffer, sizeof(m_nBuffer));
			m_nPos = 0;
		}

		~CBitOutputStream() {}

		NO_COPIABLE(CBitOutputStream);

	public:
		IZ_BOOL PutByte(IZ_BYTE n);
		IZ_BOOL PutBit(IZ_BYTE n);
		IZ_BOOL Flush();

	public:
		inline IZ_BOOL PutBit1();
		inline IZ_BOOL PutBit0();

	public:
		void SetOutputStream(izanagi_tk::IOutputStream* pOutput) { m_pOutput = pOutput; }
		izanagi_tk::IOutputStream* GetOutputStream() { return m_pOutput; }

	private:
		enum {
			BUFFER_SIZE = 32,
			BUFFER_BIT = BUFFER_SIZE << 3,
		};

	private:
		izanagi_tk::IOutputStream* m_pOutput;

		IZ_UINT m_nBuffer[BUFFER_SIZE];
		IZ_UINT m_nPos;
	};

	// inline ************************************

	/**
	* １ビット（１）を出力
	*/
	IZ_BOOL CBitOutputStream::PutBit1()
	{
		return PutBit(1);
	}

	/**
	* １ビット（０）を出力
	*/
	IZ_BOOL CBitOutputStream::PutBit0()
	{
		return PutBit(0);
	}
}	// namespace izanagi

#endif	// #if !defined(__BIT_OUTPUT_STREAM_H__)
