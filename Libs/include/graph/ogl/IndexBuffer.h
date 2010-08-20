#if !defined(__URANUS_GRAPH_INDEX_BUFFER_H__)
#define __URANUS_GRAPH_INDEX_BUFFER_H__

#include "graph/GraphDefs.h"
#include "GraphicsDevice.h"
#include "std/StdObject.h"

namespace uranus {
	class IMemoryAllocator;

	// �C���f�b�N�X�o�b�t�@
	class CIndexBuffer : public CObject {
		friend class CGraphicsDevice;
		friend class C2DRenderer;

	private:
		// �C���X�^���X�쐬
		static CIndexBuffer* CreateIndexBuffer(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			UN_UINT nIdxNum,
			E_GRAPH_INDEX_BUFFER_FMT fmt,
			E_GRAPH_RSC_CREATE_TYPE nCreateType);

	private:
		inline CIndexBuffer();
		inline ~CIndexBuffer();

		CIndexBuffer(const CIndexBuffer& rhs);
		const CIndexBuffer& operator=(const CIndexBuffer& rhs);

	private:
		// ���
		inline void InternalRelease();

		inline void ReleaseResource();

		// �{�̍쐬
		UN_BOOL CreateBody(
			UN_UINT nIdxNum,
			E_GRAPH_INDEX_BUFFER_FMT nFmt,
			E_GRAPH_RSC_CREATE_TYPE nCreateType);

	public:
		// ���b�N
		UN_BOOL Lock(
			UN_UINT nOffset,
			UN_UINT nSize,
			void** data,
			UN_BOOL bIsReadOnly,
			UN_BOOL bIsDiscard = UN_FALSE);

		// �A�����b�N
		UN_BOOL Unlock();

	private:
		// ���Z�b�g
		UN_BOOL Reset();

	public:
		// �C���f�b�N�X�T�C�Y�擾
		inline UN_UINT GetStride() const;

		// �C���f�b�N�X���擾
		inline UN_UINT GetIdxNum() const;

		// �o�b�t�@�T�C�Y�擾
		inline UN_UINT GetSize() const;

		// �t�H�[�}�b�g�擾
		inline E_GRAPH_INDEX_BUFFER_FMT GetFormat() const;

	private:
		// ���I���\�[�X���ǂ���
		UN_BOOL IsDynamic() const { return (m_nCreateType == E_GRAPH_RSC_CREATE_TYPE_DYNAMIC); }

	public:
		UN_UINT GetRawInterface() { return m_nIBO; }

	private:
		CGraphicsDevice* m_pDevice;
		IMemoryAllocator* m_pAllocator;

		// �{��
		UN_UINT m_nIBO;

		UN_UINT m_nIdxNum;				// �C���f�b�N�X��
		E_GRAPH_INDEX_BUFFER_FMT m_Fmt;	// �t�H�[�}�b�g

		// ���b�N���
		struct {
			void* data;
			UN_UINT offset;
			UN_UINT size;

			void Clear()
			{
				data = UN_NULL;
				offset = 0;
				size = 0;
			}

			UN_BOOL IsLock() const { return (data != UN_NULL); }
		} m_LockInfo;

		E_GRAPH_RSC_CREATE_TYPE m_nCreateType;
	};

	// inline *************************************

	// ���
	void CIndexBuffer::InternalRelease()
	{
		delete this;

		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	void CIndexBuffer::ReleaseResource()
	{
		// Nothing is done.
		UN_ASSERT(UN_FALSE);
	}

	// �C���f�b�N�X�T�C�Y�擾
	UN_UINT CIndexBuffer::GetStride() const
	{
		UN_UINT ret = (m_Fmt == E_GRAPH_INDEX_BUFFER_FMT_INDEX16
						? sizeof(UN_UINT16) 
						: sizeof(UN_UINT32));
		return ret;
	}

	// �C���f�b�N�X���擾
	UN_UINT CIndexBuffer::GetIdxNum() const
	{
		return m_nIdxNum;
	}

	// �o�b�t�@�T�C�Y�擾
	UN_UINT CIndexBuffer::GetSize() const
	{
		return m_nIdxNum * GetStride();
	}

	// �t�H�[�}�b�g�擾
	E_GRAPH_INDEX_BUFFER_FMT CIndexBuffer::GetFormat() const
	{
		return m_Fmt;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_INDEX_BUFFER_H__)
