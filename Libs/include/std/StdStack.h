#if !defined(__IZANAGI_STD_STACK_H__)
#define __IZANAGI_STD_STACK_H__

#include "izDefs.h"

namespace {
	/**
	* スタック（LIFO）
	*/
	template<
		class _T,
		IZ_UINT _NUM
	>
	class CStack {
	public:
		CStack()
		{
			m_nPos = 0;
			m_nNum = 0;
		}

		~CStack() {}
		
	private:
		CStack(const CStack& rhs);
		const CStack& operator =(const CStack& rhs);
		
	public:
		// スタックが一杯か
		IZ_BOOL IsFull() const
		{
			return (m_nNum == _NUM);
		}

		// スタックが空か
		IZ_BOOL IsEmpty() const
		{
			return (m_nNum == 0);
		}
		
		// アイテムをスタックから取得
		_T& Get()
		{
			IZ_ASSERT(!IsFull());
			return m_Items[m_nPos];
		}
		
		// スタックをプッシュする
		IZ_BOOL Push()
		{
			if (!IsFull()) {
				++m_nPos;
				++m_nNum;
				return IZ_TRUE;
			}
			
			return IZ_FALSE;
		}
		
		// スタックをポップする
		IZ_BOOL Pop()
		{
			if (!IsEmpty()) {
				m_nPos--;
				m_nNum--;
				return IZ_TRUE;
			}

			return IZ_FALSE;
		}

		// 最大登録可能アイテム数取得
		IZ_UINT GetMaxNum() const
		{
			return _NUM;
		}

		// 登録アイテム数取得
		IZ_UINT GetNum() const
		{
			return m_nNum;
		}
					
	private:
		_T m_Items[_NUM];

		IZ_INT m_nPos;
		IZ_INT m_nNum;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_STACK_H__)
