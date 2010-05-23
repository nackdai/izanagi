#if !defined(__URANUS_DEBUG_UTIL_FONT_H__)
#define __URANUS_DEBUG_UTIL_FONT_H__

#include "unDefs.h"
#include "unStd.h"

namespace uranus {
	class IMemoryAllocator;
	class CGraphicsDevice;
	class CTexture;

	class CDebugFont : public CObject {
	public:
		// �C���X�^���X�쐬
		static CDebugFont* CreateDebugFont(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice);
			
		enum {
			FONT_SIZE = 16,			// �t�H���g�T�C�Y
			IMAGE_WIDTH = 256,		// �t�H���g�C���[�W��
			IMAGE_HEIGHT = 128,		// �t�H���g�C���[�W����

			// �C���[�W�ɂ����ĂP���C��������̃t�H���g��
			FONT_NUM_PER_LINE_IN_IMAGE = IMAGE_WIDTH / FONT_SIZE,
		};

	protected:
		CDebugFont();
		~CDebugFont();

		CDebugFont(const CDebugFont& rhs);
		const CDebugFont& operator =(const CDebugFont& rhs);

	protected:
		// ���
		void InternalRelease();

	public:
		void Begin();
		void End();

		void DBPrint(
			const UN_CHAR* str, ...);
		void DBPrint(
			UN_DWORD color,
			const UN_CHAR* str, ...);
		void DBPrint(
			UN_INT left, UN_INT top,
			const UN_CHAR* str, ...);
		void DBPrint(
			UN_INT left, UN_INT top,
			UN_DWORD color,
			const UN_CHAR* str, ...);

	protected:
		// �e�N�X�`���쐬
		UN_BOOL CreateTexture();

		// �����`��
		void DrawFont(const UN_CHAR* str);

	protected:
		// �؂�o����`���Z�b�g����
		inline void SetTexRect(
			UN_CHAR ch,
			CIntRect& rc);

	public:
		// �`��p�����[�^�Z�b�g
		inline void SetRenderParam(
			UN_INT left, UN_INT top,
			UN_DWORD color);

		void SetFontPos(UN_INT left, UN_INT top)
		{
			m_nLeft = left; m_nTop = top;
			m_nLeftBase = left;
		}

		void SetFontColor(UN_DWORD color) { m_nColor = color; }

	public:
		UN_UINT GetFontSize() { return FONT_SIZE; }
		UN_INT GetFontPosLeft() { return m_nLeft; }
		UN_INT GetFontPosTop() { return m_nTop; }
		UN_DWORD GetFontColor() { return m_nColor; }

	protected:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		CTexture* m_pFontTex;

		// �`��p�����[�^
		UN_INT m_nLeft;			// �����ʒu�i���j
		UN_INT m_nTop;			// �����ʒu�i��j
		UN_DWORD m_nColor;		// �����F

		UN_INT m_nLeftBase;

		UN_BOOL m_bIsBegin;
	};

	// inline ***********************************

	// �؂�o����`���Z�b�g����
	void CDebugFont::SetTexRect(
		UN_CHAR ch,
		CIntRect& rc)
	{
		UN_INT nPosY = ch / FONT_NUM_PER_LINE_IN_IMAGE;
		UN_INT nPosX = ch % FONT_NUM_PER_LINE_IN_IMAGE;

		rc.left = FONT_SIZE * nPosX;
		rc.top = FONT_SIZE * nPosY;
		rc.right = rc.left + FONT_SIZE;
		rc.bottom = rc.top + FONT_SIZE;
	}

	// �`��p�����[�^�Z�b�g
	void CDebugFont::SetRenderParam(
		UN_INT left, UN_INT top,
		UN_DWORD color)
	{
		SetFontPos(left, top);
		SetFontColor(color);
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_FONT_H__)
