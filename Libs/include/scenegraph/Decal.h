#if !defined(__IZ_SCENEGRAPH_DECAL_H__)
#define __IZ_SCENEGRAPH_DECAL_H__

#include "izGraph.h"
#include "izMath.h"

#include <vector>

#define ENABLE_STL

namespace izanagi {
namespace scenegraph {
    /** �f�J�[��
     */
    class CDecal : public izanagi::CObject
    {
    protected:
        /** �C���X�^���X�쐬
         *
         * @param[in] allocator
         * @param[in] point �f�J�[�����S
         * @param[in] normal �f�J�[���@��
         * @param[in] rectangleLengthX �f�J�[����`��X����������
         * @param[in] rectangleLengthZ �f�J�[����`��Z����������
         */
        template <typename _DECAL>
        static CDecal* Create(
            izanagi::IMemoryAllocator* allocator,
            const izanagi::math::SVector4& point,
            const izanagi::math::SVector4& normal,
            IZ_FLOAT rectangleLengthX,
            IZ_FLOAT rectangleLengthZ)
        {
            izanagi::STLMemoryAllocatorProxy::SetAllocator(allocator);

            void* buf = ALLOC_ZERO(allocator, sizeof(CDecal));

            CDecal* instance = new(buf) _DECAL;
            {
                instance->m_Allocator = allocator;
                instance->AddRef();

                instance->SetRectangle(
                    point, 
                    normal,
                    rectangleLengthX,
                    rectangleLengthZ);
            }

            return instance;
        }

    protected:
        CDecal();
        virtual ~CDecal();

        NO_COPIABLE(CDecal);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        /** �f�J�[����`�����Z�b�g
         *
         * @param[in] point �f�J�[�����S
         * @param[in] normal �f�J�[���@��
         * @param[in] rectangleLengthX �f�J�[����`��X����������
         * @param[in] rectangleLengthZ �f�J�[����`��Z����������
         */
        void SetRectangle(
            const izanagi::math::SVector4& point,
            const izanagi::math::SVector4& normal,
            IZ_FLOAT rectangleLengthX,
            IZ_FLOAT rectangleLengthZ);

        /** �w�肳�ꂽ�O�p�`�����ƂɃf�J�[�����b�V�����v�Z
         *
         * @param[in] �V�U�����O���s���O�p�`�̔z��
         * @param[in] �O�p�`�̐�
         */
        virtual void ComputeDecalMesh(
            const izanagi::math::CTriangle tri[],
            IZ_UINT triNum) = 0;

        /** �f�J�[���`��p�̃O���t�B�b�N�X�I�u�W�F�N�g���쐬
         *
         * @param[in] device �O���t�B�b�N�X�f�o�C�X
         */
        void CreateGraphicsObject(izanagi::graph::CGraphicsDevice* device);

        /** �f�J�[���`��
         *
         * @param[in] device �O���t�B�b�N�X�f�o�C�X
         */
        void Draw(izanagi::graph::CGraphicsDevice* device);

        /** �f�J�[�����S���擾
         *
         * @return �f�J�[�����S
         */
        const izanagi::math::SVector4& GetCenter() const;

    protected:
        izanagi::IMemoryAllocator* m_Allocator;

        izanagi::math::CRectangle m_Rectangle;

        enum {
            PLANE_LEFT,
            PLANE_RIGHT,
            PLANE_BOTTOM,
            PLANE_TOP,
            PLANE_FRONT,
            PLANE_BACK,

            PLANE_NUM,
        };

        // �V�U�����O�p�̖�
        izanagi::math::CPlane m_Planes[PLANE_NUM];

        // �f�J�[�����\������O�p�`��
        IZ_UINT m_TriNum;

        // �f�J�[�����\������O�p�`
        izanagi::math::CTriangle* m_Triangles;

        struct SVtx {
            izanagi::math::SVector4 point;
            IZ_COLOR color;
            IZ_FLOAT uv[2];
        };

        // �O���t�B�b�N�X�I�u�W�F�N�g���쐬����K�v�����邩�ǂ���
        IZ_BOOL m_NeedCreateGraphicsObject;

        izanagi::graph::CVertexBuffer* m_VB;
        izanagi::graph::CVertexDeclaration* m_VD;
    };

    /**
     */
    class CDecalAlloc : public CDecal {
        friend class CDecal;

    public:
        static CDecal* Create(
            izanagi::IMemoryAllocator* allocator,
            const izanagi::math::SVector4& point,
            const izanagi::math::SVector4& normal,
            IZ_FLOAT rectangleLengthX,
            IZ_FLOAT rectangleLengthZ)
        {
            CDecal* ret = CDecal::Create<CDecalAlloc>(
                allocator,
                point, normal,
                rectangleLengthX, rectangleLengthZ);
            return ret;
        }

    protected:
        CDecalAlloc() {}
        virtual ~CDecalAlloc() {}

    public:
        /** �w�肳�ꂽ�O�p�`�����ƂɃf�J�[�����b�V�����v�Z
         *
         * @param[in] �V�U�����O���s���O�p�`�̔z��
         * @param[in] �O�p�`�̐�
         */
        virtual void ComputeDecalMesh(
            const izanagi::math::CTriangle tri[],
            IZ_UINT triNum);

    private:
        // �V�U�����O�ɂ���ĐV�����쐬�����O�p�`�̐����v�Z����
        void ComputeNewTriNumByScissoring(
            const izanagi::math::CTriangle tri[],
            IZ_UINT triNum);
    };

    /**
     */
    class CDecalSTL : public CDecal {
     friend class CDecal;

    public:
        static CDecal* Create(
            izanagi::IMemoryAllocator* allocator,
            const izanagi::math::SVector4& point,
            const izanagi::math::SVector4& normal,
            IZ_FLOAT rectangleLengthX,
            IZ_FLOAT rectangleLengthZ)
        {
            CDecal* ret = CDecal::Create<CDecalSTL>(
                allocator,
                point, normal,
                rectangleLengthX, rectangleLengthZ);
            return ret;
        }

    protected:
        CDecalSTL() {}
        virtual ~CDecalSTL() {}

    public:
        /** �w�肳�ꂽ�O�p�`�����ƂɃf�J�[�����b�V�����v�Z
         *
         * @param[in] �V�U�����O���s���O�p�`�̔z��
         * @param[in] �O�p�`�̐�
         */
        virtual void ComputeDecalMesh(
            const izanagi::math::CTriangle tri[],
            IZ_UINT triNum);

    private:
        typedef std::vector<izanagi::math::CTriangle, izanagi::STLMemoryAllocator<izanagi::math::CTriangle> > TriangleVector;

        struct STriListProxy
        {
            TriangleVector triList_0;
            TriangleVector triList_1;

            TriangleVector& Get(IZ_UINT idx)
            {
                if (idx == 0)
                {
                    return triList_0;
                }
                else
                {
                    return triList_1;
                }
            }

            void Clear()
            {
                triList_0.clear();
                triList_1.clear();
            }
        };
    };
}   // namespace scenegraph
}   // namespace izanagi

#endif    // #if !defined(__IZ_SCENEGRAPH_DECAL_H__)
