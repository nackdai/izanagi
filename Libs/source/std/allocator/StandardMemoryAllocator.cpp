#include "std/allocator/StandardMemoryAllocator.h"
#include "std/StdUtil.h"

using namespace uranus;

struct CStandardMemoryAllocator::SHeapHeader : public CDebugMemoryAllocator::SHeadHeaderWithDebugInfo {
	UN_UINT8* buf;
	UN_UINT size;
	SHeapHeader* prev;
	SHeapHeader* next;

	void Init()
	{
		buf = UN_NULL;
		size = 0;
		prev = UN_NULL;
		next = UN_NULL;

		ClearDebugInfo();
	}

	UN_UINT8* GetTopAddr() const
	{
		return (UN_UINT8*)this;
	}

	UN_UINT8* GetTailAddr() const
	{
		return buf + size;
	}

	UN_UINT GetAllSize() const
	{
#if 0
		return size + sizeof(SHeapHeader);
#else
		UN_ASSERT(buf > reinterpret_cast<UN_UINT8*>(const_cast<SHeapHeader*>(this)));
		UN_UINT sub = (UN_UINT)(buf - reinterpret_cast<UN_UINT8*>(const_cast<SHeapHeader*>(this)));
		return size + sub;
#endif
	}
};


CStandardMemoryAllocator::CStandardMemoryAllocator()
{
	m_nBufSize = 0;
	m_pBuf = UN_NULL;
}

CStandardMemoryAllocator::CStandardMemoryAllocator(UN_UINT nBufSize, void* pBuf)
{
	UN_ASSERT(Init(nBufSize, pBuf));
}

CStandardMemoryAllocator::~CStandardMemoryAllocator()
{
}

/**
* Initialize memory allocator.
*/
UN_BOOL CStandardMemoryAllocator::Init(UN_UINT nBufSize, void* pBuf)
{
	UN_ASSERT(!m_AllocList.HasItem());

	m_FreeList.Clear();

	UN_BOOL ret = UN_FALSE;

	m_nBufSize = nBufSize;
	m_pBuf = reinterpret_cast<UN_UINT8*>(pBuf);

	// フリーリスト初期化
	SHeapHeader* p = GetHeapHeader(m_pBuf);

	if (p != UN_NULL) {
		p->buf = m_pBuf + sizeof(SHeapHeader);
		p->size = m_nBufSize - sizeof(SHeapHeader);
		m_FreeList.AddTop(p);

		ret = UN_TRUE;
	}

	return ret;
}

/**
* Allocate memory buffer.
*/
void* CStandardMemoryAllocator::Alloc(size_t size, const UN_CHAR* file, UN_UINT line)
{
	void* ret = AllocInternal(size, file, line);
	return ret;
}

/**
* Allocate memory buffer (with filling zero).
*/
void* CStandardMemoryAllocator::AllocZero(size_t size, const UN_CHAR* file, UN_UINT line)
{
	void* ret = Alloc(size, file, line);
	if (ret != UN_NULL) {
		FILL_ZERO(ret, size);
	}
	return ret;
}

/**
* Allocate aligned memory buffer.
*/
void* CStandardMemoryAllocator::AllocAlign(size_t size, UN_UINT align, const UN_CHAR* file, UN_UINT line)
{
	void* ret = AllocInternal(size, align, file, line);
	return ret;
}

/**
* Allocate aligned memory buffer (with filling zero).
*/
void* CStandardMemoryAllocator::AllocAlignZero(size_t size, UN_UINT align, const UN_CHAR* file, UN_UINT line)
{
	void* ret = AllocAlign(size, align, file, line);
	if (ret != UN_NULL) {
		FILL_ZERO(ret, size);
	}
	return ret;
}

/**
* Re-allocate memory buffer.
*/
void* CStandardMemoryAllocator::Realloc(void* ptr, size_t size, const UN_CHAR* file, UN_UINT line)
{
	UN_ASSERT(ptr != UN_NULL);
	UN_ASSERT(size > 0);

	SHeapHeader* p = FindAllocatedHeapHeader(ptr);
	VRETURN_NULL(p != UN_NULL);

	if (p->size == size) {
		// Nothing is done.
		return ptr;
	}
	else {
		if (p->size < size) {
			UN_UINT nReallocSize = size - p->size;

			// Realloc可能なフリーのメモリ領域を探す
			SHeapHeader* pFree = FindReallocableFreedHeapHeader(ptr, nReallocSize);

			if (pFree != UN_NULL) {
				m_FreeList.Remove(pFree);

				UN_UINT nFreedSize = pFree->GetAllSize();

				// 新たにフリーリストに登録する
				SHeapHeader* pNew = UN_NULL;
				if (nFreedSize > nReallocSize + sizeof(SHeapHeader)) {
					UN_UINT8* pFreeTop = pFree->GetTopAddr();

					pNew = reinterpret_cast<SHeapHeader*>(pFreeTop + nReallocSize);

					pNew->Init();
					pNew->buf = reinterpret_cast<UN_UINT8*>(pNew) + sizeof(SHeapHeader);
					pNew->size = nFreedSize - nReallocSize - sizeof(SHeapHeader);

					InsertFreeList(pNew);
				}

				// サイズ更新
				p->size += (pNew != UN_NULL ? nReallocSize : nFreedSize);
				return p->buf;
			}
			else {
				// 解放する
				m_AllocList.Remove(p);
				InsertFreeList(p);

				// 新たにデータを確保する
				void* ret = AllocZero(size, file, line);
				VRETURN_NULL(ret != UN_NULL);

				// コピーする
				memcpy(ret, ptr, size);

				return ret;
			}
		}
		else {
			// TODO
			// 元のサイズより小さくなることは許さない
			UN_ASSERT(UN_FALSE);
		}
	}

	return UN_NULL;
}

/**
* Free memory buffer.
*/
UN_BOOL CStandardMemoryAllocator::Free(void* data)
{
	if (data == UN_NULL) {
		// 何もすることはない
		return UN_TRUE;
	}

	UN_BOOL ret = UN_FALSE;

	SHeapHeader* p = FindAllocatedHeapHeader(data);
	if (p != UN_NULL) {
		// アロケートリストから外す
		m_AllocList.Remove(p);

		// フリーリストに挿入
		InsertFreeList(p);

		ret = UN_TRUE;
	}

	return ret;
}

/**
*/
UN_UINT CStandardMemoryAllocator::GetSize()
{
	return m_nBufSize;
}

/**
* Return allocated memory size.
*/
UN_UINT CStandardMemoryAllocator::GetAllocatedSize()
{
	UN_UINT ret = 0;

	SHeapHeader* p = m_AllocList.GetTop();
	while (p != UN_NULL) {
		ret += p->size;
		p = p->next;
	}

	return ret;
}

/**
* Return freed memory size.
*/
UN_UINT CStandardMemoryAllocator::GetFreedSize()
{
	UN_UINT nAllocatedSize = GetAllocatedSize();
	UN_ASSERT(nAllocatedSize <= m_nBufSize);

	UN_UINT ret = m_nBufSize - nAllocatedSize;
	return ret;
}

/**
* Dump heap information.
*/
void CStandardMemoryAllocator::Dump()
{
#ifdef __UN_DEBUG__
	SHeapHeader* p = m_AllocList.GetTop();
	while (p != UN_NULL) {
		// 情報表示
		UN_PRINTF(
			"Dump Memory cnt[%d] buf[0x%x] size[%d] file[%s] line[%d]\n",
			p->cnt - 1,
			p->buf,
			p->size,
			p->file,
			p->line);

		p = p->next;
	}

	UN_ASSERT(m_FreeList.HasItem());
	UN_ASSERT(m_FreeList.GetTop()->next == UN_NULL);
	UN_ASSERT(m_FreeList.GetTop()->size + sizeof(SHeapHeader) == m_nBufSize);
#endif	// #ifdef __UN_DEBUG__
}

// メモリ確保
void* CStandardMemoryAllocator::AllocInternal(
	size_t size,
	const UN_CHAR* file,
	UN_UINT line)
{
	if (!m_FreeList.HasItem()) {
		return UN_NULL;
	}

	IncrementAllocCnt();

	void* ret = UN_NULL;

	// ヘッダ分を足す
	size_t nSize = size + sizeof(SHeapHeader);

	SHeapHeader* pFree = m_FreeList.GetTop();

	while (pFree != UN_NULL) {
		if (pFree->size >= nSize) {
			// 指定サイズを確保できる

			// 後ろからとっていく
			SHeapHeader* pNew = GetHeapHeader(pFree->GetTailAddr() - nSize);
			pNew->buf = (UN_BYTE*)pNew + sizeof(SHeapHeader);
			pNew->size = (UN_UINT)size;
			pNew->SetDebugInfo(GetAllocCnt(), line, file);

			ret = pNew->buf;

			// アロケートリストに追加
			m_AllocList.AddTail(pNew);

			pFree->size -= (UN_UINT)nSize;
			if (pFree->size == 0) {
				// フリーリストから削除
				m_FreeList.Remove(pFree);
			}

			break;
		}

		pFree = pFree->next;
	}

	return ret;
}

namespace {
	// アライン可能かどうか
	inline UN_BOOL _EnableAlign(UN_UINT align)
	{
		// NOTE
		// アラインは４の倍数でしかできない

		UN_BOOL ret = UN_FALSE;

		if ((align > 0)
			&& ((align & 0x03) == 0))
		{
			ret = UN_TRUE;
		}

		return ret;
	}

	// アラインサイズ計算
	template <typename _T>
	inline _T _ComputeAlignValue(_T v, UN_UINT align)
	{
		_T ret = (v + (align - 1)) / align * align;
		return ret;
	}
}	// namespace

// メモリ確保(アライン有り)
void* CStandardMemoryAllocator::AllocInternal(
	size_t size,
	UN_UINT align,
	const UN_CHAR* file,
	UN_UINT line)
{
	// フリーバッファはあるか
	if (!m_FreeList.HasItem()) {
		return UN_NULL;
	}

	// アライン可能か
	if ((align == 0) || !_EnableAlign(align)) {
		return UN_NULL;
	}

	IncrementAllocCnt();

	void* ret = UN_NULL;

	// アラインサイズ計算
	size = _ComputeAlignValue(size, align);

	// ヘッダ分を足す
	size_t nSize = size + sizeof(SHeapHeader);

	SHeapHeader* pFree = m_FreeList.GetTop();

	while (pFree != UN_NULL) {
		if (pFree->size >= nSize) {
			// 指定サイズを確保できる

			UN_BYTE* pBuf = pFree->GetTailAddr() - nSize;
			pBuf += sizeof(SHeapHeader);
			UN_BYTE* pAlignBuf = CStdUtil::ConvIntToPtr<UN_BYTE, UN_UINT>(
									CStdUtil::ConvPtrToInt<UN_UINT>(pBuf) / align * align);

			// 差分
			UN_ASSERT(pBuf >= pAlignBuf);
			UN_UINT nSub = (UN_UINT)(pBuf - pAlignBuf);

			size_t nAlignSize = nSize + nSub;

			if (pFree->size >= nAlignSize) {
				// 後ろからとっていく
				SHeapHeader* pNew = GetHeapHeader(pFree->GetTailAddr() - nAlignSize);
				pNew->buf = (UN_BYTE*)pNew + sizeof(SHeapHeader) + nSub;
				pNew->size = (UN_UINT)(nAlignSize - sizeof(SHeapHeader));
				pNew->SetDebugInfo(GetAllocCnt(), line, file);

				ret = pNew->buf;
				UN_ASSERT((CStdUtil::ConvPtrToInt<UN_UINT>(ret) & (align - 1)) == 0);

				// アロケートリストに追加
				m_AllocList.AddTail(pNew);

				pFree->size -= (UN_UINT)nAlignSize;
				if (pFree->size == 0) {
					// フリーリストから削除
					m_FreeList.Remove(pFree);
				}

				break;
			}
		}

		pFree = pFree->next;
	}

	return ret;
}

// フリーリストに挿入
void CStandardMemoryAllocator::InsertFreeList(SHeapHeader* pFree)
{
	pFree->prev = UN_NULL;
	pFree->next = UN_NULL;
	pFree->ClearDebugInfo();

	if (m_FreeList.HasItem()) {
		SHeapHeader* p = m_FreeList.GetTop();
		UN_BOOL bIsInserted = UN_FALSE;

		// まずはアドレス位置の順にソートされた状態でリストに挿入する
		while (p != UN_NULL) {
			if (pFree->buf <= p->buf) {
				m_FreeList.JoinBefore(p, pFree);
				bIsInserted = UN_TRUE;
				break;
			}

			p = p->next;
		}

		if (!bIsInserted) {
			m_FreeList.AddTail(pFree);
		}

		p = m_FreeList.GetTop();

		// 今度はリストがくっつくか調べる
		UN_UINT8* pFreeTop = pFree->GetTopAddr();
		UN_UINT8* pFreeTail = pFree->GetTailAddr();

		while (p != UN_NULL) {
			UN_UINT8* top = p->GetTopAddr();
			UN_UINT8* tail = p->GetTailAddr();

			if (pFreeTop == tail) {
				// 上とくっつく
				p->size += pFree->GetAllSize();

#if 0
				p->next = pFree->next;

				if (p->next != UN_NULL) {
					p->next->prev = p;
				}
#else
				m_FreeList.Remove(pFree);
#endif
				
				top = p->GetTopAddr();
				pFree = p;
			}

			if (pFreeTail == top) {
				// 下とくっつく
				pFree->size += p->GetAllSize();
#if 0
				pFree->next = p->next;

				if (p->next != UN_NULL) {
					p->next->prev = pFree;
				}
#else
				m_FreeList.Remove(p);
#endif

				tail = p->GetTailAddr();
				p = pFree;
			}

			p = p->next;
		}
	}
	else {
		m_FreeList.AddTop(pFree);
	}
}

// Realloc可能なフリーのメモリ領域を探す
CStandardMemoryAllocator::SHeapHeader* CStandardMemoryAllocator::FindReallocableFreedHeapHeader(void* ptr, size_t size)
{
	UN_UINT8* pTopPtr = reinterpret_cast<UN_UINT8*>(ptr);
	UN_UINT8* pTailPtr = pTopPtr + size;

	SHeapHeader* p = m_FreeList.GetTop();

	while (p != UN_NULL) {
		UN_UINT nFreedSize = p->GetAllSize();

		if (nFreedSize >= size) {
			// サイズに余裕がある
			UN_UINT8* top = p->GetTopAddr();
			UN_UINT8* tail = p->GetTailAddr();

			if (pTailPtr == top) {
				// 連続する
				return p;
			}
		}
	}

	return UN_NULL;
}

// SHeapHeader探索
CStandardMemoryAllocator::SHeapHeader* CStandardMemoryAllocator::FindAllocatedHeapHeader(void* ptr)
{
	UN_UINT8* pFreeAddr = reinterpret_cast<UN_UINT8*>(ptr);

	SHeapHeader* p = m_AllocList.GetTop();

	while (p != UN_NULL) {
		if (p->buf == pFreeAddr) {
			break;
		}

		p = p->next;
	}

	return p;
}

// SHeapHeader確保
CStandardMemoryAllocator::SHeapHeader* CStandardMemoryAllocator::GetHeapHeader(UN_UINT8* buf)
{
	SHeapHeader* ret = UN_NULL;

	if (buf + sizeof(SHeapHeader) < m_pBuf + m_nBufSize) {
		ret = reinterpret_cast<SHeapHeader*>(buf);
		ret->Init();
	}

	return ret;
}
