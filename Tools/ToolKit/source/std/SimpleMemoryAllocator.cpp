#include "std/SimpleMemoryAllocator.h"

namespace izanagi {
namespace tool {

void* CSimpleMemoryAllocator::Alloc(
	size_t size, 
	const IZ_CHAR* file/*= IZ_NULL*/, 
	IZ_UINT line/*= IZ_NULL*/)
{
	// 確保サイズ
	size_t nSize = size + sizeof(SHeapHeader);

	// メモリ確保
	IZ_BYTE* buf = reinterpret_cast<IZ_BYTE*>(malloc(nSize));
	VRETURN_NULL(buf != IZ_NULL);

	SHeapHeader* pHeader = AddHeader(
							buf,
							size,
							file,
							line);

	IZ_BYTE* ret = pHeader->buf;

	return ret;
}

void* CSimpleMemoryAllocator::AllocZero(
	size_t size, 
	const IZ_CHAR* file/*= IZ_NULL*/, 
	IZ_UINT line/*= IZ_NULL*/)
{
	void* ret = Alloc(size, file, line);
	memset(ret, 0, size);

	return ret;
}

void* CSimpleMemoryAllocator::Realloc(
	void* ptr, 
	size_t size, 
	const IZ_CHAR* file/*= IZ_NULL*/, 
	IZ_UINT line/*= IZ_NULL*/)
{
	IZ_ASSERT(ptr != IZ_NULL);

	// ヘッダ探す
	SHeapHeader* pHeader = FindHeader(ptr);
	VRETURN_NULL(pHeader != IZ_NULL);

	if (pHeader->size > size) {
		// Nothing is done.
		return ptr;
	}

	// 一度リストからはずす
	pHeader->item.Leave();

	IZ_BYTE* buf = reinterpret_cast<IZ_BYTE*>(realloc(pHeader, size + sizeof(SHeapHeader)));

	void* ret = IZ_NULL;

	if (buf != IZ_NULL) {
		pHeader = AddHeader(
					buf,
					size,
					file,
					line);

		ret = pHeader->buf;
	}
	else {
		IZ_ASSERT(IZ_FALSE);
		
		// アロケートリストに登録
		m_AllocList.AddTail(&pHeader->item);

		ret = ptr;
	}

	return ret;
}

IZ_BOOL CSimpleMemoryAllocator::Free(void* data)
{
	if (data == IZ_NULL) {
		return IZ_TRUE;
	}

	// ヘッダ探す
	SHeapHeader* pHeader = FindHeader(data);
	VRETURN(pHeader != IZ_NULL);

	// アロケートリストから外す
	pHeader->item.Leave();

	// 解放
	delete pHeader;
	free(pHeader);

	return IZ_TRUE;
}

/**
*/
IZ_UINT CSimpleMemoryAllocator::GetSize()
{
	return 0;
}

/**
* Return allocated memory size.
*/
IZ_UINT CSimpleMemoryAllocator::GetAllocatedSize()
{
	IZ_UINT ret = 0;

	CStdList<SHeapHeader>::Item* pItem = m_AllocList.GetTop();
	while (pItem != IZ_NULL) {
		SHeapHeader* p = pItem->GetData();

		ret += static_cast<IZ_UINT>(p->size);

		pItem = pItem->GetNext();
	}

	return ret;
}

/**
* Return freed memory size.
*/
IZ_UINT CSimpleMemoryAllocator::GetFreedSize()
{
	// This can't have freed memory size.
	return 0;
}

IZ_BOOL CSimpleMemoryAllocator::Dump()
{
	CStdList<SHeapHeader>::Item* pItem = m_AllocList.GetTop();
	while (pItem != IZ_NULL) {
		SHeapHeader* p = pItem->GetData();

		// 情報表示
		IZ_PRINTF(
			"Dump Memory buf[0x%x] size[%d] file[%s] line[%d]\n",
			p->buf,
			p->size,
			p->file,
			p->line);

		pItem = pItem->GetNext();
	}

	IZ_BOOL hasItem = m_AllocList.HasItem();
	return !hasItem;
}

CSimpleMemoryAllocator::SHeapHeader* CSimpleMemoryAllocator::AddHeader(
	IZ_BYTE* ptr, 
	size_t size, 
	const IZ_CHAR* file, 
	IZ_UINT line)
{
	SHeapHeader* pHeader = new(ptr) SHeapHeader;
	{
		ptr += sizeof(SHeapHeader);

		pHeader->Init();

		pHeader->buf = ptr;
		pHeader->size = size;

		pHeader->SetDebugInfo(line, file);
	}

	// アロケートリストに登録
	m_AllocList.AddTail(&pHeader->item);

	return pHeader;
}

CSimpleMemoryAllocator::SHeapHeader* CSimpleMemoryAllocator::FindHeader(void* ptr)
{
	IZ_ASSERT(ptr != IZ_NULL);

	IZ_BYTE* p = reinterpret_cast<IZ_BYTE*>(ptr);

	SHeapHeader* pHeader = reinterpret_cast<SHeapHeader*>(p - sizeof(SHeapHeader));

	// 整合性チェック
	IZ_BOOL result = (pHeader->buf == ptr);
	VRETURN_NULL(result);

	return pHeader;
}

}   // namespace tool
}   // namespace izanagi