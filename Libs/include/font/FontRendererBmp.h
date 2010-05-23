#if !defined(__URANUS_FONT_FONT_RENDERER_BMP_H__)
#define __URANUS_FONT_FONT_RENDERER_BMP_H__

#include "unDefs.h"
#include "FNTFormat.h"
#include "FontRenderer.h"

namespace uranus {
	class IInputStream;
	class IMemoryAllocator;
	class CGraphicsDevice;
	class CTexture;

	/**
	* �t�H���g�����_��
	*/
	class CFontRendererBmp : public CFontRenderer {
		friend class CFontRenderer;

	private:
#if 0
		static const E_GRAPH_PIXEL_FMT TEX_FORMAT = E_GRAPH_PIXEL_FMT_RGBA8;
		static const UN_UINT TEX_BPP = 4;
		static const E_GRAPH_2D_RENDER_OP RENDER_2D_OP = E_GRAPH_2D_RENDER_OP_MODULATE;
#else
		static const E_GRAPH_PIXEL_FMT TEX_FORMAT = E_GRAPH_PIXEL_FMT_A8;
		static const UN_UINT TEX_BPP = 1;
		static const E_GRAPH_2D_RENDER_OP RENDER_2D_OP = E_GRAPH_2D_RENDER_OP_MODULATE_ALPHA;
#endif

		// �o�^���
		struct SRegInfo;

	private:
		// �C���X�^���X�쐬
		static CFontRenderer* CreateFontRendererBmp(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT	nMaxRegisterNum,
			IInputStream* in);

	private:
		CFontRendererBmp();
		~CFontRendererBmp();

		NO_COPIABLE(CFontRendererBmp);

	private:
		// ���
		void InternalRelease();

		// �e�N�X�`���쐬
		UN_BOOL CreateTexture(IInputStream* in);

	public:
		// �o�^
		UN_BOOL Register(void* pStr);

		// �`��
		// �o�^����Ă��Ȃ��ꍇ�́A�`�悵�Ȃ�
		void Render(
			void* pStr,
			UN_INT nX, UN_INT nY,
			UN_COLOR nColor = UN_COLOR_RGBA(255, 255, 255, 255));

		// �`��
		// �o�^����Ă��Ȃ��ꍇ�́A�o�^���s��
		UN_BOOL RenderImmediate(
			void* pStr,
			UN_INT nX, UN_INT nY,
			UN_COLOR nColor = UN_COLOR_RGBA(255, 255, 255, 255));

		// �s�N�Z�����擾
		UN_UINT GetWidth(void* pStr);

		// �o�^�ς݂��ǂ���
		UN_BOOL IsRegistered(UN_UINT code);

		// �s�N�Z�������擾
		UN_UINT GetHeight() const { return m_sHeader.fontHeight; }

		// �����R�[�h�擾
		E_FONT_CHAR_ENCODE GetCharEncode() const { return m_sHeader.charEncode; }

	private:
		// �t�H���g�o�^
		SRegInfo* RegisterInternal(UN_UINT code);

		// �t�H���g�o�^�J�n
		UN_BOOL BeginRegister();

		// �t�H���g�o�^�I��
		void EndRegister();

		// �o�^���擾
		SRegInfo* GetRegInfo(UN_UINT code);

		// �}�b�v���擾
		const S_FNT_MAP* GetFontMap(UN_UINT code);

		// �]�����e�N�X�`���Ƀt�H���g�`��
		void RenderToSrcTex(
			SRegInfo* pRegInfo,
			UN_UINT code,
			UN_BOOL bRegister);

		// �]��
		UN_BOOL UpdateSurface();

		// �t�H���g�`��
		void RenderInternal(
			UN_UINT code,
			CIntPoint* pDstPoint,
			UN_COLOR nColor);

	private:
		enum {
			HASH_SIZE = 5,
		};

		typedef CStdHash<UN_UINT, SRegInfo, HASH_SIZE>	CFntHash;
		typedef CFntHash::Item			CFntHashItem;
		typedef CStdList<CFntHashItem>	CFntOrderList;

		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		// �w�b�_
		S_FNT_HEADER m_sHeader;

		// �t�H���g�}�b�v���
		S_FNT_MAP* m_pMapList;

		// �t�H���g�C���[�W���
		S_FNT_IMAGE* m_pImageList;

		// �t�H���g�e�N�X�`��
		CTexture* m_pSrcTex;
		CTexture* m_pDstTex;

		// �C���[�W���
		struct {
			UN_UINT16 x;
			UN_UINT16 y;

			UN_UINT8* data;
			UN_UINT32 pitch;
		} m_sImageInfo;

		// �ő�o�^�\������
		UN_UINT m_nMaxRegisterNum;

		// �o�^��
		UN_UINT m_nRegNum;

		// �o�^���
		SRegInfo* m_pRegList;

		CFntHash m_FontHash;

		struct {
			UN_UINT isNeedUpdateSurface	: 1;	// UpdateSurface���K�v���ǂ���
		} m_Flags;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_FONT_FONT_RENDERER_BMP_H__)
