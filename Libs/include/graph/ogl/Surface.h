#if !defined(__URANUS_GRAPH_SURFACE_H__)
#define __URANUS_GRAPH_SURFACE_H__

#include "unStd.h"
#include "graph/GraphDefs.h"
#include "unD3DDefs.h"

namespace uranus {
	class IMemoryAllocator;
	class CTexture;

	class CSurface : public CObject {
		friend class CTexture;
		friend class CGraphicsDevice;

	private:
		// �T�[�t�F�X�쐬
		static CSurface* CreateSurface(IMemoryAllocator* pAllocator);

	private:
		CSurface();
		~CSurface();

		CSurface(const CSurface& rhs);
		const CSurface& operator=(const CSurface& rhs);

	private:
		// ���
		void InternalRelease();

		// ���Z�b�g
		UN_BOOL Reset(
			CTexture* pTexture,
			UN_UINT nLevel);

		// ���[��E�E�E
		UN_BOOL SetDesc();

	private:
		inline void ReleaseResource();

		UN_UINT GetRawInterface() { return m_nFBO; }
		UN_UINT GetDepthBuffer() { return m_nDepthBuffer; }
		
	public:
		inline UN_UINT GetWidth() const;
		inline UN_UINT GetHeight() const;
		inline E_GRAPH_PIXEL_FMT GetPixelFormat() const;

	private:
		IMemoryAllocator* m_pAllocator;

		// �{��
		UN_UINT m_nFBO;

		UN_UINT m_nDepthBuffer;

		// �L�q
		struct {
			UN_UINT16 Width;
			UN_UINT16 Height;
			E_GRAPH_PIXEL_FMT Format;
		} m_Desc;
	};

	// inline ********************************

	UN_UINT CSurface::GetWidth() const
	{
		return m_Desc.Width;
	}

	UN_UINT CSurface::GetHeight() const
	{
		return m_Desc.Height;
	}

	E_GRAPH_PIXEL_FMT CSurface::GetPixelFormat() const
	{
		return m_Desc.Format;
	}

	void CSurface::ReleaseResource()
	{
		// Nothing is done.
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_SURFACE_H__)
