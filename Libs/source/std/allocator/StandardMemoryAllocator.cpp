#include "std/allocator/StandardMemoryAllocator.h"
#include "std/StdUtil.h"

using namespace izanagi;

struct CStandardMemoryAllocator::SHeapHeader
	: public IZ_DEBUG_HEAP_HEADER_BASE
{
	IZ_UINT8* buf;
	IZ_UINT size;
	SHeapHeader* prev;
	SHeapHeader* next;

	void Init()
	{
		buf = IZ_NULL;
		size = 0;
		prev = IZ_NULL;
		next = IZ_NULL;

		ClearDebugInfo();
	}

	IZ_UINT8* GetTopAddr() const
	{
		return (IZ_UINT8*)this;
	}

	IZ_UINT8* GetTailAddr() const
	{
		return buf + size;
	}

	IZ_UINT GetAllSize() const
	{
		IZ_ASSERT(buf > reinterpret_cast<IZ_UINT8*>(const_cast<SHeapHeader*>(this)));
		IZ_UINT sub = (IZ_UINT)(buf - reinterpret_cast<IZ_UINT8*>(const_cast<SHeapHeader*>(this)));
		return size + sub;
	}
};


CStandardMemoryAllocator::CStandardMemoryAllocator()
{
	m_nBufSize = 0;
	m_pBuf = IZ_NULL;
}

CStandardMemoryAllocator::CStandardMemoryAllocator(IZ_UINT nBufSize, void* pBuf)
{
	IZ_ASSERT(Init(nBufSize, pBuf));
}

CStandardMemoryAllocator::~CStandardMemoryAllocator()
{
}

/**
* Initialize memory allocator.
*/
IZ_BOOL CStandardMemoryAllocator::Init(IZ_UINT nBufSize, void* pBuf)
{
	IZ_ASSERT(!m_AllocList.HasItem());

	m_FreeList.Clear();

	IZ_BOOL ret = IZ_FALSE;

	m_nBufSize = nBufSize;
	m_pBuf = reinterpret_cast<IZ_UINT8*>(pBuf);

	// フリーリスト初期化
	SHeapHeader* p = GetHeapHeader(m_pBuf);

	if (p != IZ_NULL) {
		p->buf = m_pBuf + sizeof(SHeapHeader);
		p->size = m_nBufSize - sizeof(SHeapHeader);
		m_FreeList.AddTop(p);

		ret = IZ_TRUE;
	}

	return ret;
}

/**
* Allocate memory buffer.
*/
void* CStandardMemoryAllocator::Alloc(size_t size, const IZ_CHAR* file, IZ_UINT line)
{
	void* ret = AllocInternal(size, file, line);
	return ret;
}

/**
* Allocate memory buffer (with filling zero).
*/
void* CStandardMemoryAllocator::AllocZero(size_t size, const IZ_CHAR* file, IZ_UINT line)
{
	void* ret = Alloc(size, file, line);
	if (ret != IZ_NULL) {
		FILL_ZERO(ret, size);
	}
	return ret;
}

/**
* Allocate aligned memory buffer.
*/
void* CStandardMemoryAllocator::AllocAlign(size_t size, IZ_UINT align, const IZ_CHAR* file, IZ_UINT line)
{
	void* ret = AllocInternal(size, align, file, line);
	return ret;
}

/**
* Allocate aligned memory buffer (with filling zero).
*/
void* CStandardMemoryAllocator::AllocAlignZero(size_t size, IZ_UINT align, const IZ_CHAR* file, IZ_UINT line)
{
	void* ret = AllocAlign(size, align, file, line);
	if (ret != IZ_NULL) {
		FILL_ZERO(ret, size);
	}
	return ret;
}

/**
* Re-allocate memory buffer.
*/
void* CStandardMemoryAllocator::Realloc(void* ptr, size_t size, const IZ_CHAR* file, IZ_UINT line)
{
	IZ_ASSERT(ptr != IZ_NULL);
	IZ_ASSERT(size > 0);

	SHeapHeader* p = FindAllocatedHeapHeader(ptr);
	VRETURN_NULL(p != IZ_NULL);

	if (p->size == size) {
		// Nothing is done.
		return ptr;
	}
	else {
		if (p->size < size) {
			IZ_UINT nReallocSize = size - p->size;

			// Realloc可能なフリーのメモリ領域を探す
			SHeapHeader* pFree = FindReallocableFreedHeapHeader(ptr, nReallocSize);

			if (pFree != IZ_NULL) {
				m_FreeList.Remove(pFree);

				IZ_UINT nFreedSize = pFree->GetAllSize();

				// 新たにフリーリストに登録する
				SHeapHeader* pNew = IZ_NULL;
				if (nFreedSize > nReallocSize + sizeof(SHeapHeader)) {
					IZ_UINT8* pFreeTop = pFree->GetTopAddr();

					pNew = reinterpret_cast<SHeapHeader*>(pFreeTop + nReallocSize);

					pNew->Init();
					pNew->buf = reinterpret_cast<IZ_UINT8*>(pNew) + sizeof(SHeapHeader);
					pNew->size = nFreedSize - nReallocSize - sizeof(SHeapHeader);

					InsertFreeList(pNew);
				}

				// サイズ更新
				p->size += (pNew != IZ_NULL ? nReallocSize : nFreedSize);
				return p->buf;
			}
			else {
				// 解放する
				m_AllocList.Remove(p);
				InsertFreeList(p);

				// 新たにデータを確保する
				void* ret = AllocZero(size, file, line);
				VRETURN_NULL(ret != IZ_NULL);

				// コピーする
				memcpy(ret, ptr, size);

				return ret;
			}
		}
		else {
			// TODO
			// 元のサイズより小さくなることは許さない
			IZ_ASSERT(IZ_FALSE);
		}
	}

	return IZ_NULL;
}

/**
* Free memory buffer.
*/
IZ_BOOL CStandardMemoryAllocator::Free(void* data)
{
	if (data == IZ_NULL) {
		// 何もすることはない
		return IZ_TRUE;
	}

	IZ_BOOL ret = IZ_FALSE;

	SHeapHeader* p = FindAllocatedHeapHeader(data);
	if (p != IZ_NULL) {
		// アロケートリストから外す
		m_AllocList.Remove(p);

		// フリーリストに挿入
		InsertFreeList(p);

		ret = IZ_TRUE;
	}

	return ret;
}

/**
*/
IZ_UINT CStandardMemoryAllocator::GetSize()
{
	return m_nBufSize;
}

/**
* Return allocated memory size.
*/
IZ_UINT CStandardMemoryAllocator::GetAllocatedSize()
{
	IZ_UINT ret = 0;

	SHeapHeader* p = m_AllocList.GetTop();
	while (p != IZ_NULL) {
		ret += p->size;
		p = p->next;
	}

	return ret;
}

/**
* Return freed memory size.
*/
IZ_UINT CStandardMemoryAllocator::GetFreedSize()
{
	IZ_UINT nAllocatedSize = GetAllocatedSize();
	IZ_ASSERT(nAllocatedSize <= m_nBufSize);

	IZ_UINT ret = m_nBufSize - nAllocatedSize;
	return ret;
}

/**
* Dump heap information.
*/
IZ_BOOL CStandardMemoryAllocator::Dump()
{
	IZ_PRINTF("Dump Memory---\n");

	SHeapHeader* p = m_AllocList.GetTop();
	while (p != IZ_NULL) {
		// 情報表示
		p->Dump();

		IZ_PRINTF(
			" buf[0x%x] size[%d]\n",
			p->buf,
			p->size);

		p = p->next;
	}

	IZ_PRINTF("--------------\n");

	IZ_ASSERT(m_FreeList.HasItem());
	IZ_ASSERT(m_FreeList.GetTop()->next == IZ_NULL);
	IZ_ASSERT(m_FreeList.GetTop()->size + sizeof(SHeapHeader) == m_nBufSize);

	IZ_BOOL ret = m_FreeList.HasItem();
	return ret;
}

// メモリ確保
void* CStandardMemoryAllocator::AllocInternal(
	size_t size,
	const IZ_CHAR* file,
	IZ_UINT line)
{
	if (!m_FreeList.HasItem()) {
		return IZ_NULL;
	}

	IncrementAllocCnt();

	void* ret = IZ_NULL;

	// ヘッダ分を足す
	size_t nSize = size + sizeof(SHeapHeader);

	SHeapHeader* pFree = m_FreeList.GetTop();

	while (pFree != IZ_NULL) {
		if (pFree->size >= nSize) {
			// 指定サイズを確保できる

			// 後ろからとっていく
			SHeapHeader* pNew = GetHeapHeader(pFree->GetTailAddr() - nSize);
			pNew->buf = (IZ_BYTE*)pNew + sizeof(SHeapHeader);
			pNew->size = (IZ_UINT)size;
			pNew->SetDebugInfo(GetAllocCnt() - 1, line, file);

			ret = pNew->buf;

			// アロケートリストに追加
			m_AllocList.AddTail(pNew);

			pFree->size -= (IZ_UINT)nSize;
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
	inline IZ_BOOL _EnableAlign(IZ_UINT align)
	{
		// NOTE
		// アラインは４の倍数でしかできない

		IZ_BOOL ret = IZ_FALSE;

		if ((align > 0)
			&& ((align & 0x03) == 0))
		{
			ret = IZ_TRUE;
		}

		return ret;
	}

	// アラインサイズ計算
	template <typename _T>
	inline _T _ComputeAlignValue(_T v, IZ_UINT align)
	{
		_T ret = (v + (align - 1)) / align * align;
		return ret;
	}
}	// namespace

// メモリ確保(アライン有り)
void* CStandardMemoryAllocator::AllocInternal(
	size_t size,
	IZ_UINT align,
	const IZ_CHAR* file,
	IZ_UINT line)
{
	// フリーバッファはあるか
	if (!m_FreeList.HasItem()) {
		return IZ_NULL;
	}

	// アライン可能か
	if ((align == 0) || !_EnableAlign(align)) {
		return IZ_NULL;
	}

	IncrementAllocCnt();

	void* ret = IZ_NULL;

	// アラインサイズ計算
	size = _ComputeAlignValue(size, align);

	// ヘッダ分を足す
	size_t nSize = size + sizeof(SHeapHeader);

	SHeapHeader* pFree = m_FreeList.GetTop();

	while (pFree != IZ_NULL) {
		if (pFree->size >= nSize) {
			// 指定サイズを確保できる

			IZ_BYTE* pBuf = pFree->GetTailAddr() - nSize;
			pBuf += sizeof(SHeapHeader);
			IZ_BYTE* pAlignBuf = CStdUtil::ConvIntToPtr<IZ_BYTE, IZ_UINT>(
									CStdUtil::ConvPtrToInt<IZ_UINT>(pBuf) / align * align);

			// 差分
			IZ_ASSERT(pBuf >= pAlignBuf);
			IZ_UINT nSub = (IZ_UINT)(pBuf - pAlignBuf);

			size_t nAlignSize = nSize + nSub;

			if (pFree->size >= nAlignSize) {
				// 後ろからとっていく
				SHeapHeader* pNew = GetHeapHeader(pFree->GetTailAddr() - nAlignSize);
				pNew->buf = (IZ_BYTE*)pNew + sizeof(SHeapHeader) + nSub;
				pNew->size = (IZ_UINT)(nAlignSize - sizeof(SHeapHeader));
				pNew->SetDebugInfo(GetAllocCnt() - 1, line, file);

				ret = pNew->buf;
				IZ_ASSERT((CStdUtil::ConvPtrToInt<IZ_UINT>(ret) & (align - 1)) == 0);

				// アロケートリストに追加
				m_AllocList.AddTail(pNew);

				pFree->size -= (IZ_UINT)nAlignSize;
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
	pFree->prev = IZ_NULL;
	pFree->next = IZ_NULL;
	pFree->ClearDebugInfo();

	if (m_FreeList.HasItem()) {
		SHeapHeader* p = m_FreeList.GetTop();
		IZ_BOOL bIsInserted = IZ_FALSE;

		// まずはアドレス位置の順にソートされた状態でリストに挿入する
		while (p != IZ_NULL) {
			if (pFree->buf <= p->buf) {
				m_FreeList.JoinBefore(p, pFree);
				bIsInserted = IZ_TRUE;
				break;
			}

			p = p->next;
		}

		if (!bIsInserted) {
			m_FreeList.AddTail(pFree);
		}

		p = m_FreeList.GetTop();

		// 今度はリストがくっつくか調べる
		IZ_UINT8* pFreeTop = pFree->GetTopAddr();
		IZ_UINT8* pFreeTail = pFree->GetTailAddr();

		while (p != IZ_NULL) {
			IZ_UINT8* top = p->GetTopAddr();
			IZ_UINT8* tail = p->GetTailAddr();

			if (pFreeTop == tail) {
				// 上とくっつく
				p->size += pFree->GetAllSize();

#if 0
				p->next = pFree->next;

				if (p->next != IZ_NULL) {
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

				if (p->next != IZ_NULL) {
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
	IZ_UINT8* pTopPtr = reinterpret_cast<IZ_UINT8*>(ptr);
	IZ_UINT8* pTailPtr = pTopPtr + size;

	SHeapHeader* p = m_FreeList.GetTop();

	while (p != IZ_NULL) {
		IZ_UINT nFreedSize = p->GetAllSize();

		if (nFreedSize >= size) {
			// サイズに余裕がある
			IZ_UINT8* top = p->GetTopAddr();
			IZ_UINT8* tail = p->GetTailAddr();

			if (pTailPtr == top) {
				// 連続する
				return p;
			}
		}
	}

	return IZ_NULL;
}

// SHeapHeader探索
CStandardMemoryAllocator::SHeapHeader* CStandardMemoryAllocator::FindAllocatedHeapHeader(void* ptr)
{
	IZ_UINT8* pFreeAddr = reinterpret_cast<IZ_UINT8*>(ptr);

	SHeapHeader* p = m_AllocList.GetTop();

	while (p != IZ_NULL) {
		if (p->buf == pFreeAddr) {
			break;
		}

		p = p->next;
	}

	return p;
}

// SHeapHeader確保
CStandardMemoryAllocator::SHeapHeader* CStandardMemoryAllocator::GetHeapHeader(IZ_UINT8* buf)
{
	SHeapHeader* ret = IZ_NULL;

	if (buf + sizeof(SHeapHeader) < m_pBuf + m_nBufSize) {
		ret = reinterpret_cast<SHeapHeader*>(buf);
		ret->Init();
	}

	return ret;
}
