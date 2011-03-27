#if !defined(__BIT_IMPUT_STREAM_H__)
#define __BIT_IMPUT_STREAM_H__

#include "ToolkitIoDefs.h"

namespace izanagi {
	/**
	*/
	class CBitInputStream {
	public:
		CBitInputStream()
		{
			m_pInput = IZ_NULL;
			m_nPos = 0;
		}

		~CBitInputStream() {}

		NO_COPIABLE(CBitInputStream);

	public:
		IZ_BYTE GetBit();

	public:
		// 初期化
		inline void Init(IInputStream* pInput);

		inline IZ_BYTE GetByte();
		inline IZ_UINT GetNBits(IZ_UINT n);

		inline IZ_BOOL IsEnableInput() const;

		// IInputStream 取得
		IInputStream* GetInputStream() { return m_pInput; }

	private:
		enum {
			BUFFER_SIZE = 32,
			BUFFER_BIT = BUFFER_SIZE << 3,
		};

	private:
		IInputStream* m_pInput;

		IZ_UINT m_nBuffer[BUFFER_SIZE];
		IZ_UINT m_nPos;
	};

	// inline ******************************************

	// 初期化
	void CBitInputStream::Init(IInputStream* pInput)
	{
		m_pInput = pInput;
		m_nPos = 0;
	}

	/**
	* １バイト取得
	*/
	IZ_BYTE CBitInputStream::GetByte()
	{
		IZ_BYTE ret = static_cast<IZ_BYTE>(GetNBits(8));
		return ret;
	}

	/**
	* Ｎビット取得
	*/
	IZ_UINT CBitInputStream::GetNBits(IZ_UINT n)
	{
		n = (n > 32 ? 32 : n);

		IZ_UINT ret = 0;
		for (IZ_UINT i = 0; i < n; i++) {
			ret = (ret << 1) | GetBit();
		}

		return ret;
	}

	/**
	* 入力可能か
	*/
	IZ_BOOL CBitInputStream::IsEnableInput() const
	{
		IZ_ASSERT(m_pInput != IZ_NULL);
		return (m_pInput->GetCurPos() < m_pInput->GetSize());
	}
}	// namespace

#endif	// #if !defined(__BIT_IMPUT_STREAM_H__)
