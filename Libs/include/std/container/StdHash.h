#if !defined(__URANUS_STD_HASH_H__)
#define __URANUS_STD_HASH_H__

#include "unDefs.h"
#include "StdList.h"
#include "std/allocator/MemoryAllocator.h"

namespace uranus {
	//=================================================
	/**
	*/
	template <typename _KEY, typename _T, UN_UINT _NUM>
	class CStdHash {
	public:
		class Item {
			friend class CStdHash<_KEY, _T, _NUM>;

		public:
			Item() : m_Key((UN_UINT)0)
			{
				//m_HashItem.Init(this);
				//m_OrderItem.Init(this);
			}

			~Item() {}

			// ������
			void Init(const _KEY& tKey, _T* pSelf)
			{
				m_HashItem.Init(this);
				m_OrderItem.Init(this);

				UN_ASSERT(pSelf != NULL);
				m_pSelf = pSelf;

				m_Key = tKey;
			}

			// �f�[�^���擾
			_T* GetData()
			{
				return m_pSelf;
			}

			// �L�[���擾
			const _KEY& GetKey() const
			{
				return m_Key;
			}

			void Leave()
			{
				m_HashItem.Leave();
				m_OrderItem.Leave();
			}

		protected:
			typename CStdList<Item>::Item* GetHashItem() { return &m_HashItem; }
			typename CStdList<Item>::Item* GetOrderItem() { return &m_OrderItem; }

		protected:
			_KEY m_Key;
			_T* m_pSelf;
			typename CStdList<Item>::Item m_HashItem;
			typename CStdList<Item>::Item m_OrderItem;
		};

	public:
		// �R���X�g���N�^
		CStdHash(){}

		// �f�X�g���N�^
		~CStdHash()
		{
			Clear();
		}

		// �n�b�V���T�C�Y���擾
		UN_UINT GetHashSize() const
		{
			return _NUM;
		}

		// ���f�[�^���擾
		UN_UINT GetDataNum() const
		{
			UN_UINT ret = m_OrderList.GetItemNum();
			return ret;
		}

		// �o�^
		UN_BOOL Add(Item* pItem)
		{
			UN_ASSERT(pItem != UN_NULL);

			UN_UINT nHashPos = pItem->m_Key % _NUM;
			
			// �n�b�V�����X�g�ɓo�^
			UN_BOOL ret = m_HashList[nHashPos].AddTail(pItem->GetHashItem());
			if (ret) {
				// �I�[�_�[���X�g�ɓo�^
				ret = m_OrderList.AddTail(pItem->GetOrderItem());
			}

			if (!ret) {
				// �o�^�Ɏ��s�����̂ŁA���X�g����O��
				pItem->GetHashItem()->Leave();
				pItem->GetOrderItem()->Leave();
			}

			return ret;
		}

		Item* Find(const _KEY& tKey)
		{
			UN_UINT nHashPos = tKey % _NUM;

			// �Y������n�b�V�����X�g
			CStdList<Item>& cList = m_HashList[nHashPos];
			
			// �T��
			CStdList<Item>::Item* pListItem = cList.GetTop();
			while (pListItem != UN_NULL) {
				Item* pHashItem = pListItem->GetData();

				if (pHashItem->m_Key == tKey) {
					// ��������
					return pHashItem;
				}

				pListItem = pListItem->GetNext();
			}

			return UN_NULL;
		}

		// �L�[�ɊY������f�[�^��T��
		_T* FindData(const _KEY& tKey)
		{
			_T* ret = UN_NULL;

			UN_UINT nHashPos = tKey % _NUM;

			// �Y������n�b�V�����X�g
			CStdList<Item>& cList = m_HashList[nHashPos];
			
			// �T��
			CStdList<Item>::Item* pListItem = cList.GetTop();
			while (pListItem != UN_NULL) {
				Item* pHashItem = pListItem->GetData();

				if (pHashItem->m_Key == tKey) {
					// ��������
					ret = pHashItem->GetData();
					break;
				}

				pListItem = pListItem->GetNext();
			}

			return ret;
		}

		// �N���A
		void Clear()
		{
			// �n�b�V�����X�g�N���A
			for (UN_UINT i = 0; i < _NUM; i++) {
				m_HashList[i].Clear();
			}

			m_OrderList.Clear();
		}

		Item* GetOrderTop()
		{
			CStdList<Item>::Item* pOrderListItem = m_OrderList.GetTop();
			return (pOrderListItem != UN_NULL ? pOrderListItem->GetData() : UN_NULL);
		}
		Item* GetOrderTail()
		{
			CStdList<Item>::Item* pOrderListItem = m_OrderList.GetTail();
			return (pOrderListItem != UN_NULL ? pOrderListItem->GetData() : UN_NULL);
		}
		Item* GetOrderAt(UN_UINT idx)
		{
			CStdList<Item>::Item* pOrderListItem = m_OrderList.GetAt(idx);
			return (pOrderListItem != UN_NULL ? pOrderListItem->GetData() : UN_NULL);
		}

	private:
		// �n�b�V�����X�g
		CStdList<Item> m_HashList[_NUM];

		// �o�^���ꂽ���ɕ��ԃ��X�g
		CStdList<Item> m_OrderList;

		UN_C_ASSERT(_NUM > 0);
	};

	//=================================================
	/**
	*/
	template <typename _KEY, typename _T>
	class CStdHashEx {
	public:
		class Item {
			friend class CStdHashEx<_KEY, _T>;

		public:
			Item() : m_Key(0)
			{
				m_HashItem.Init(this);
				m_OrderItem.Init(this);
			}

			~Item() {}

			// ������
			void Init(const _KEY& tKey, _T* pSelf)
			{
				UN_ASSERT(pSelf != NULL);
				m_pSelf = pSelf;

				m_Key = tKey;
			}

			// �f�[�^���擾
			_T* GetData()
			{
				return m_pSelf;
			}

			// �L�[���擾
			const _KEY& GetKey() const
			{
				return m_Key;
			}

			void Leave()
			{
				m_HashItem.Leave();
				m_OrderItem.Leave();
			}

		protected:
			typename CStdList<Item>::Item* GetHashItem() { return &m_HashItem; }
			typename CStdList<Item>::Item* GetOrderItem() { return &m_OrderItem; }

		protected:
			_KEY m_Key;
			_T* m_pSelf;
			typename CStdList<Item>::Item m_HashItem;
			typename CStdList<Item>::Item m_OrderItem;
		};

	public:
		// �R���X�g���N�^
		CStdHashEx()
		{
			m_pAllocator != NULL;

			m_nSize = 0;
			m_pHashList = NULL;
		}

		// �f�X�g���N�^
		~CStdHashEx()
		{
			Clear();
		}

		// ������
		UN_BOOL Init(IMemoryAllocator* pAllocator, UN_UINT nSize)
		{
			UN_ASSERT(pAllocator != UN_NULL);
			UN_ASSERT(nSize > 0);

			if (m_pAllocator != NULL) {
				Clear();
			}

			// �������m��
			UN_UINT8* pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CStdList<Item>) * nSize);
			VRETURN(pBuf != NULL);

			UN_BOOL ret = InitEx(pBuf, nSize);

			return ret;
		}

		// ������
		UN_BOOL InitEx(void* pBuf, UN_UINT nSize)
		{
			UN_ASSERT(m_pHashList == UN_NULL);
			UN_ASSERT(pBuf != NULL);
			UN_ASSERT(nSize > 0);

			UN_UINT8* p = reinterpret_cast<UN_UINT8*>(pBuf);
			
			CStdList<Item>* pList = m_pHashList;

			// ���X�g������
			for (UN_UINT i = 0; i < nSize; i++) {
				pList = new(p) CStdList<Item>;
				pList++;

				p += sizeof(CStdList<Item>);
			}

			m_nSize = nSize;

			return UN_TRUE;
		}

		// �n�b�V���T�C�Y���擾
		UN_UINT GetHashSize() const
		{
			return m_nSize;
		}

		// ���f�[�^���擾
		UN_UINT GetDataNum() const
		{
			UN_UINT ret = m_OrderList.GetItemNum;
			return ret;
		}

		// �o�^
		UN_BOOL Add(Item* pItem)
		{
			UN_ASSERT(pItem != UN_NULL);
			UN_ASSERT(m_pHashList != NULL);
			UN_ASSERT(m_nSize > 0);

			UN_UINT nHashPos = pItem->m_Key % m_nSize;
			
			// �n�b�V�����X�g�ɓo�^
			UN_BOOL ret = m_pHashList[nHashPos].AddTail(pItem->GetHashItem());
			if (ret) {
				// �I�[�_�[���X�g�ɓo�^
				ret = m_OrderList.AddTail(pItem->GetOrderItem());
			}

			if (!ret) {
				// �o�^�Ɏ��s�����̂ŁA���X�g����O��
				pItem->GetHashItem()->Leave();
				pItem->GetOrderItem()->Leave();
			}

			return ret;
		}

		// �L�[�ɊY������f�[�^��T��
		_T* FindData(const _KEY& tKey)
		{
			_T* ret = UN_NULL;

			UN_UINT nHashPos = tKey % m_nSize;

			// �Y������n�b�V�����X�g
			CStdList<Item>& cList = &m_pHashList[nHashPos];
			
			// �T��
			CStdList<Item>::Item* pListItem = cList.GetTop();
			while (pListItem != UN_NULL) {
				Item* pHashItem = pListItem->GetData();

				if (pHashItem->m_Key == tKey) {
					// ��������
					ret = pHashItem->GetData();
					break;
				}

				pListItem = pListItem->GetNext();
			}

			return ret;
		}

		// �N���A
		void Clear()
		{
			UN_ASSERT(m_pAllocator != NULL);

			// �n�b�V�����X�g�N���A
			{
				for (UN_UINT i = 0; i < m_nSize; i++) {
					m_pHashList[i].Clear();
				}

				FREE(m_pAllocator, m_pHashList);
				m_pHashList = NULL;

				m_nSize = 0;
			}

			m_OrderList.Clear();
		}

		Item* GetOrderTop()
		{
			CStdList<Item>::Item* cOrderListItem = m_OrderList.GetTop();
			return (cOrderListItem != UN_NULL ? cOrderListItem->GetData() : UN_NULL);
		}
		Item* GetOrderTail()
		{
			CStdList<Item>::Item* cOrderListItem = m_OrderList.GetTail();
			return (cOrderListItem != UN_NULL ? cOrderListItem->GetData() : UN_NULL);
		}

	private:
		IMemoryAllocator* m_pAllocator;

		// �n�b�V�����X�g
		UN_UINT m_nSize;
		CStdList<Item>* m_pHashList;

		// �o�^���ꂽ���ɕ��ԃ��X�g
		CStdList<Item> m_OrderList;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_HASH_H__)
