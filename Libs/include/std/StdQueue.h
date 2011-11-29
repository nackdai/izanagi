#if !defined(__IZANAGI_STD_QUEUE_H__)
#define __IZANAGI_STD_QUEUE_H__

#include "izDefs.h"
#include "std/container/StdList.h"

namespace izanagi {
	/** 上限があるキュー.
	 */
	template<class _T, IZ_UINT _NUM>
	class CQueue {
	public:
		CQueue()
		{
			m_Param.idxFront = 0;
			m_Param.idxRear = 0;
			m_Param.num = 0;
		}
		~CQueue() {}
		
		NO_COPIABLE(CQueue);
		
	public:
		/** キューが一杯か.
		 */
		IZ_BOOL IsFull() const
		{
			return (m_Param.num == _NUM);
		}

		/** キューが空か.
		 */
		IZ_BOOL IsEmpty() const
		{
			return (m_Param.num == 0);
		}
		
		/** アイテムをキューから取得.
		 */
		_T& GetRear()
		{
			IZ_ASSERT(m_Param.idxRear < _NUM);
			return m_Items[m_Param.idxRear];
		}
		_T& GetFront()
		{
			IZ_ASSERT(m_Param.idxFront < _NUM);
			return m_Items[m_Param.idxFront];
		}
		
		/** キューをプッシュする.
		 */
		IZ_BOOL Push()
		{
			if (!IsFull()) {
				m_Param.idxRear = GetNextIndex(m_Param.idxRear);
				++(m_Param.num);
				return IZ_TRUE;
			}
			
			return IZ_FALSE;
		}
		
		/** キューをポップする.
		 */
		IZ_BOOL Pop()
		{
			if (!IsEmpty()) {
				m_Param.idxFront = GetNextIndex(m_Param.idxFront);
				m_Param.num--;
				return IZ_TRUE;
			}

			return IZ_FALSE;
		}

		/** 最大登録可能アイテム数取得.
		 */
		IZ_UINT GetMaxNum() const
		{
			return _NUM;
		}

		/** 登録アイテム数取得.
		 */
		IZ_UINT GetNum() const
		{
			return m_Param.num;
		}

	private:
		// 次のインデックスを取得
		IZ_UINT16 GetNextIndex(IZ_UINT16 idx) const
		{
			IZ_C_ASSERT(_NUM > 0);
			return (UINT16)((idx + 1) % _NUM);
		}
					
	private:
		_T m_Items[_NUM];

		struct {
			IZ_INT idxFront;
			IZ_INT idxRear;
			IZ_INT num;
		} m_Param;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_QUEUE_H__)
