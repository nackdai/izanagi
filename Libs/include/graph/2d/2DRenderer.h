#if !defined(__URANUS_GRAPH_INTERNAL_2D_RENDERER_H__)
#define __URANUS_GRAPH_INTERNAL_2D_RENDERER_H__

#include "unD3DDefs.h"
#include "std/StdObject.h"
#include "std/2DRect.h"
#include "graph/GraphDefs.h"
#include "2DShader.h"

namespace uranus {
	class IMemoryAllocator;
	class CGraphicsDevice;
	class CVertexBuffer;
	class CIndexBuffer;
	class CVertexDeclaration;

	/**
	*/
	class C2DRenderer : public CObject {
		friend class CGraphicsDevice;

	protected:
		// �C���X�^���X�쐬
		static C2DRenderer* Create2DRenderer(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator);

	protected:
		C2DRenderer();
		~C2DRenderer();

		C2DRenderer(const C2DRenderer& rhs);
		const C2DRenderer& operator=(const C2DRenderer& rhs);

	protected:
		struct CUSTOM_VERTEX {
			float x, y, z, w;
			UN_COLOR color;
			float u, v;
		};

		//static const DWORD FVF_CUSTOMVERTEX = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

		enum PRIM_TYPE {
			PRIM_TYPE_SPRITE = 0,
			PRIM_TYPE_RECT,
			PRIM_TYPE_LINE,

			PRIM_TYPE_NUM,
			PRIM_TYPE_FORCE_INT32 = 0x7fffffff,
		};

	protected:
		// ���
		void InternalRelease();

		// ������
		UN_BOOL Init(CGraphicsDevice* pDevice);

	protected:
		// �t���[���J�n���ɌĂԂ���
		void BeginFrame();

		// �`��J�n
		UN_BOOL BeginDraw();

		// �`��I��
		UN_BOOL EndDraw(CGraphicsDevice* pDevice);

		// �`��R�}���h���t���b�V��
		UN_BOOL Flush(CGraphicsDevice* pDevice);

		// �X�v���C�g�`��
		UN_BOOL DrawSprite(
			CGraphicsDevice* pDevice,
			const CFloatRect& rcSrc,
			const CIntRect& rcDst,
			const UN_COLOR color = UN_COLOR_RGBA(255, 255, 255, 255));
		UN_BOOL DrawSpriteEx(
			CGraphicsDevice* pDevice,
			const CIntRect& rcSrc,
			const CIntRect& rcDst,
			const UN_COLOR color = UN_COLOR_RGBA(255, 255, 255, 255));

		// ��`�`��
		UN_BOOL DrawRect(
			CGraphicsDevice* pDevice,
			const CIntRect& rcDst,
			const UN_COLOR color);

		// ���`��
		UN_BOOL DrawLine(
			CGraphicsDevice* pDevice,
			const CIntPoint& ptStart,
			const CIntPoint& ptGoal,
			const UN_COLOR color);

		// �`��ݒ�
		UN_BOOL SetRenderOp(
			CGraphicsDevice* pDevice,
			E_GRAPH_2D_RENDER_OP nOp);

	protected:
		// �`�揀��
		UN_BOOL PrepareDraw(
			CGraphicsDevice* pDevice,
			PRIM_TYPE nPrimType);

		// ���b�N
		UN_BOOL Lock();

		// �A�����b�N
		void Unlock();

		// ���_�f�[�^�Z�b�g
		void SetVtx(
			const CFloatRect& rcSrc,
			const CIntRect& rcDst,
			const UN_COLOR color);

		// �C���f�b�N�X�f�[�^�Z�b�g
		void SetIdx();

		// ���\�[�X���Z�b�g
		void ResetResource();

		// ���Z�b�g
		void Reset();

	protected:
		// ���b�N�t���O����
		UN_BOOL IsLock() const { return m_bIsLock; }
		void ToggleIsLock() { m_bIsLock = !m_bIsLock; }

		// �`��ݒ�擾
		inline E_GRAPH_2D_RENDER_OP GetRenderOp() const;

	protected:
		enum {
			MAX_RECT_NUM = 1000,

			// NOTE
			// x4 �͂P��` = �S���_ �Ȃ̂�
			MAX_VERTEX_NUM = MAX_RECT_NUM * 4,

			// NOTE
			// �P��` = �S���_
			// ��`�Ƌ�`�̊Ԃ��Ȃ������Ȃ��O�p�`�p�̃C���f�b�N�X�����Q
			// ex) �Q��` -> 0123 34 4567 -> 4 * 2 + 2
			//               012-123-[233-334-344-445]-456-567
			MAX_INDEX_NUM = MAX_RECT_NUM * 4 + (MAX_RECT_NUM - 1) * 2,

			// �P���_�̃T�C�Y
			VTX_STRIDE = sizeof(CUSTOM_VERTEX),

			// FVF
			//VTX_FVF = FVF_CUSTOMVERTEX,
		};

	protected:
		// �o�b�t�@���
		struct SBufferInfo {
			UN_UINT offset;	// �I�t�Z�b�g
			UN_UINT num;	// �f�[�^��
			void* buf_ptr;	// ���b�N�����Ƃ��̃|�C���^

			// ���̃��b�N��DISCARD���ǂ���
			UN_BOOL next_lock_discard;

			SBufferInfo()
			{
				Clear();
				next_lock_discard = UN_FALSE;
			}

			void Clear()
			{
				offset = 0;
				num = 0;
				buf_ptr = UN_NULL;
			}
		};

	protected:
		IMemoryAllocator* m_pAllocator;

		CVertexBuffer* m_pVB;		// ���_�o�b�t�@
		CIndexBuffer* m_pIB;		// �C���f�b�N�X�o�b�t�@
		CVertexDeclaration* m_pVD;	// ���_�錾

		// �o�b�t�@��ԊǗ�
		SBufferInfo m_sVBInfo;	// ���_�o�b�t�@�p
		SBufferInfo m_sIBInfo;	// �C���f�b�N�X�o�b�t�@�p

		// �V�F�[�_
		C2DShader* m_pShader;

		// �`��v���~�e�B�u�^�C�v
		PRIM_TYPE m_nPrimType;

		// ���ݐݒ肳��Ă���v���~�e�B�u��
		UN_UINT m_nCurPrimNum;

		// ���݃Z�b�g����Ă���C���f�b�N�X
		UN_UINT16 m_nCurIdx;

		// ���b�N�t���O
		UN_BOOL m_bIsLock;
	};

	// inline ********************************

	// �`��ݒ�擾
	E_GRAPH_2D_RENDER_OP C2DRenderer::GetRenderOp() const
	{
		UN_ASSERT(m_pShader != UN_NULL);
		return m_pShader->GetRenderOp();
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_INTERNAL_2D_RENDERER_H__)
