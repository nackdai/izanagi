#if !defined(__URANUS_FONT_FONT_RENDERER_H__)
#define __URANUS_FONT_FONT_RENDERER_H__

#include "unDefs.h"
#include "unStd.h"
#include "FontDefs.h"

namespace uranus {
	class IMemoryAllocator;
	class CGraphicsDevice;
	class IInputStream;

	/**
	* �t�H���g�����_��
	*/
	class CFontRenderer : public CObject {
	public:
		/**
		*/
		typedef UN_UINT (*RenderFontHandler)(CGraphicsDevice* pDevice, UN_UINT code, const SIntRect& rc, UN_UINT x, UN_UINT y, UN_COLOR color);

		/**
		*/
		static CFontRenderer* CreateFontRendererBmp(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT	nMaxRegisterNum,
			IInputStream* in);

		// TODO
		/**
		*/
		static CFontRenderer* CreateFontRendererFT(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT	nMaxRegisterNum);

	protected:
		enum {
			// �ő�\��������
			MAX_FONT_NUM = 32768,

			// �\�[�X�e�N�X�`�������}�[�W��
			CHAR_MARGIN = 1,
		};

	protected:
		CFontRenderer() { m_pRenderFontHadler = UN_NULL; }
		virtual ~CFontRenderer() {}

		NO_COPIABLE(CFontRenderer);

	public:
		// �o�^
		virtual UN_BOOL Register(void* pStr) = 0;

		// �`��
		// �o�^����Ă��Ȃ��ꍇ�́A�`�悵�Ȃ�
		virtual void Render(
			void* pStr,
			UN_INT nX, UN_INT nY,
			UN_COLOR nColor = UN_COLOR_RGBA(255, 255, 255, 255)) = 0;

		// �`��
		// �o�^����Ă��Ȃ��ꍇ�́A�o�^���s��
		virtual UN_BOOL RenderImmediate(
			void* pStr,
			UN_INT nX, UN_INT nY,
			UN_COLOR nColor = UN_COLOR_RGBA(255, 255, 255, 255)) = 0;

		// �s�N�Z�����擾
		virtual UN_UINT GetWidth(void* pStr) = 0;

		// �o�^�ς݂��ǂ���
		virtual UN_BOOL IsRegistered(UN_UINT code) = 0;

		// �s�N�Z�������擾
		virtual UN_UINT GetHeight() const = 0;

		// �����R�[�h�擾
		virtual E_FONT_CHAR_ENCODE GetCharEncode() const = 0;

		// �t�H���g�`��n���h���Z�b�g
		void SetFonrRenderHandler(RenderFontHandler* pHandler) { m_pRenderFontHadler = pHandler; }

	protected:
		RenderFontHandler* m_pRenderFontHadler;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_FONT_FONT_RENDERER_H__)
