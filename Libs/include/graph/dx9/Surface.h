#if !defined(__URANUS_GRAPH_SURFACE_H__)
#define __URANUS_GRAPH_SURFACE_H__

#include "unStd.h"
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

	private:
		inline void ReleaseResource();

	public:
		inline UN_UINT GetWidth() const;
		inline UN_UINT GetHeight() const;
		inline D3DFORMAT GetPixelFormat() const;

	public:
		D3D_SURFACE* GetRawInterface() { return m_pSurface; }

		// ���[��E�E�E
		inline UN_BOOL SetDesc();

	private:
		IMemoryAllocator* m_pAllocator;

		// �{��
		D3D_SURFACE* m_pSurface;

		// �L�q
		D3DSURFACE_DESC m_Desc;
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

	D3DFORMAT CSurface::GetPixelFormat() const
	{
		return m_Desc.Format;
	}

	void CSurface::ReleaseResource()
	{
		SAFE_RELEASE(m_pSurface);
	}

	// ���[��E�E�E
	UN_BOOL CSurface::SetDesc()
	{
		UN_BOOL ret = UN_FALSE;
		if (m_pSurface != UN_NULL) {
			HRESULT hr = m_pSurface->GetDesc(&m_Desc);
			ret = SUCCEEDED(hr);
		}
		return ret;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_SURFACE_H__)
