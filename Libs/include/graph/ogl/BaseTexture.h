#if !defined(__URANUS_GRAPH_BASE_TEXTURE_H__)
#define __URANUS_GRAPH_BASE_TEXTURE_H__

#include "unStd.h"
#include "unD3DDefs.h"
#include "graph/GraphDefs.h"

namespace uranus {
	class CGraphicsDevice;

	/**
	* �x�[�X�e�N�X�`��
	*/
	class CBaseTexture : public CObject {
		friend class CGraphicsDevice;

	protected:
		CBaseTexture();
		virtual ~CBaseTexture();

		CBaseTexture(const CBaseTexture& rhs);
		const CBaseTexture& operator=(const CBaseTexture& rhs);

	public:
		// ���擾
		inline UN_UINT GetWidth(UN_UINT level = 0) const;

		// �����擾
		inline UN_UINT GetHeight(UN_UINT level = 0) const;

		// �t�H�[�}�b�g�擾
		inline E_GRAPH_PIXEL_FMT GetPixelFormat() const;

		// MIPMAP���擾
		inline UN_UINT GetMipMapNum() const;

		// �t�B���^�Z�b�g
		inline void SetFilter(
			E_GRAPH_TEX_FILTER minFilter,
			E_GRAPH_TEX_FILTER magFilter,
			E_GRAPH_TEX_FILTER mipFilter);

		// �A�h���b�V���O�Z�b�g
		inline void SetAddress(
			E_GRAPH_TEX_ADDRESS addressU,
			E_GRAPH_TEX_ADDRESS addressV);

		// �T���v���X�e�[�g�擾
		inline const S_SAMPLER_STATE& GetState() const;

		inline UN_BOOL IsRenderTarget() const;
		inline UN_BOOL IsDynamic() const;

	protected:
		inline UN_BOOL IsOnSysMem() const;
		inline UN_BOOL IsOnVram() const;

	public:
		virtual TEX_HANDLE GetTexHandle() = 0;

	protected:
		CGraphicsDevice* m_pDevice;
		IMemoryAllocator* m_pAllocator;

		// �e�N�X�`�����
		struct {
			UN_UINT16 width;
			UN_UINT16 height;
			
			UN_UINT8  level;
			UN_UINT8  depth;

			struct {
				UN_UINT16 is_dynamic		: 1;	// ���I�e�N�X�`�����ǂ���
				UN_UINT16 is_on_sysmem		: 1;	// �V�X�e����������݂̂ɂ��邩�ǂ���
				UN_UINT16 is_on_vram		: 1;	// VRAM��݂̂ɂ��邩�ǂ���
				UN_UINT16 is_rendertarget	: 1;	// �����_�[�^�[�Q�b�g���ǂ���
			};

			E_GRAPH_PIXEL_FMT fmt;

			GLenum glFmt;
			GLenum glType;
		} m_TexInfo;		

		// �X�e�[�g
		S_SAMPLER_STATE m_sState;
	};

	// inline ***********************************************

	// ���擾
	UN_UINT CBaseTexture::GetWidth(UN_UINT level/*= 0*/) const
	{
		UN_ASSERT(level < m_TexInfo.level);
		UN_UINT ret = (m_TexInfo.width >> level);
		return ret;
	}

	// �����擾
	UN_UINT CBaseTexture::GetHeight(UN_UINT level/*= 0*/) const
	{
		UN_ASSERT(level < m_TexInfo.level);
		UN_UINT ret = (m_TexInfo.height >> level);
		return ret;
	}

	// �t�H�[�}�b�g�擾
	E_GRAPH_PIXEL_FMT CBaseTexture::GetPixelFormat() const
	{
		return m_TexInfo.fmt;
	}

	// MIPMAP���擾
	UN_UINT CBaseTexture::GetMipMapNum() const
	{
		return m_TexInfo.level;
	}

	// �t�B���^�Z�b�g
	void CBaseTexture::SetFilter(
		E_GRAPH_TEX_FILTER minFilter,
		E_GRAPH_TEX_FILTER magFilter,
		E_GRAPH_TEX_FILTER mipFilter)
	{
		m_sState.minFilter = minFilter;
		m_sState.magFilter = magFilter;
		m_sState.mipFilter = mipFilter;
	}

	// �A�h���b�V���O�Z�b�g
	void CBaseTexture::SetAddress(
		E_GRAPH_TEX_ADDRESS addressU,
		E_GRAPH_TEX_ADDRESS addressV)
	{
		m_sState.addressU = addressU;
		m_sState.addressV = addressV;
	}

	// �e�N�X�`���X�e�[�g�擾
	const S_SAMPLER_STATE& CBaseTexture::GetState() const
	{
		return m_sState;
	}

	UN_BOOL CBaseTexture::IsRenderTarget() const
	{
		return m_TexInfo.is_rendertarget;
	}

	UN_BOOL CBaseTexture::IsDynamic() const
	{
		return m_TexInfo.is_dynamic;
	}

	UN_BOOL CBaseTexture::IsOnSysMem() const
	{
		return m_TexInfo.is_on_sysmem;
	}

	UN_BOOL CBaseTexture::IsOnVram() const
	{
		return m_TexInfo.is_on_vram;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_BASE_TEXTURE_H__)
