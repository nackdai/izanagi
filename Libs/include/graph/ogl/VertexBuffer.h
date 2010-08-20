#if !defined(__URANUS_GRAPH_VERTEX_BUFFER_H__)
#define __URANUS_GRAPH_VERTEX_BUFFER_H__

#include "graph/GraphDefs.h"
#include "GraphicsDevice.h"
#include "std/StdObject.h"

namespace uranus {
	class IMemoryAllocator;

	// ���_�o�b�t�@
	class CVertexBuffer : public CObject {
		friend class CGraphicsDevice;
		friend class C2DRenderer;

	private:
		// �C���X�^���X�쐬
		static CVertexBuffer* CreateVertexBuffer(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			UN_UINT nStride,
			UN_UINT nVtxNum,
			E_GRAPH_RSC_CREATE_TYPE nCreateType);

	private:
		CVertexBuffer();
		~CVertexBuffer();

		CVertexBuffer(const CVertexBuffer& rhs);
		const CVertexBuffer& operator=(const CVertexBuffer& rhs);

	private:
		// ���
		inline void InternalRelease();

		inline void ReleaseResource();

		// �{�̍쐬
		UN_BOOL CreateBody(
			UN_UINT nStride,
			UN_UINT nVtxNum,
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
		// ���_�T�C�Y�擾
		inline UN_UINT GetStride() const;

		// ���_���擾
		inline UN_UINT GetVtxNum() const;

		// �T�C�Y�擾
		inline UN_UINT GetSize() const;

	private:
		// ���I���\�[�X���ǂ���
		UN_BOOL IsDynamic() const { return (m_nCreateType == E_GRAPH_RSC_CREATE_TYPE_DYNAMIC); }

	public:
		UN_UINT GetRawInterface() { return m_nVBO; }

	private:
		CGraphicsDevice* m_pDevice;
		IMemoryAllocator* m_pAllocator;

		// �{��
		UN_UINT m_nVBO;

		UN_UINT m_nStride;	// ���_�T�C�Y
		UN_UINT m_nVtxNum;	// ���_��

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
	void CVertexBuffer::InternalRelease()
	{
		delete this;

		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	void CVertexBuffer::ReleaseResource()
	{
		// Nothing is done.
		UN_ASSERT(UN_FALSE);
	}

	// ���_�T�C�Y�擾
	UN_UINT CVertexBuffer::GetStride() const
	{
		return m_nStride;
	}

	// ���_���擾
	UN_UINT CVertexBuffer::GetVtxNum() const
	{
		return m_nVtxNum;
	}

	// �T�C�Y�擾
	UN_UINT CVertexBuffer::GetSize() const
	{
		return m_nVtxNum * m_nStride;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_VERTEX_BUFFER_H__)
