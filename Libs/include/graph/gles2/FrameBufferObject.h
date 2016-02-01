#if !defined(__IZANAGI_FRAME_BUFFER_OBJECT_H__)
#define __IZANAGI_FRAME_BUFFER_OBJECT_H__

#include "izStd.h"
#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;
    class CRenderTarget;

    // FrameBufferObject
    class CFrameBufferObject : public CObject
    {
        friend class CGraphicsDeviceGLES2;

    private:
        // インスタンス作成
        static CFrameBufferObject* CreateFBO(
            CGraphicsDeviceGLES2* device,
            IMemoryAllocator* allocator);

    protected:
        inline CFrameBufferObject();
        virtual ~CFrameBufferObject();

        NO_COPIABLE(CFrameBufferObject);

        IZ_DEFINE_INTERNAL_RELEASE();

    private:
        IZ_BOOL CreateInternal();

        IZ_BOOL SetRenderTarget(
            IZ_UINT idx,
            CRenderTarget* rt,
            IZ_BOOL isDepth);

        void ClearForcibly();

        IZ_BOOL StartOffScreen();
        IZ_BOOL EndOffScreen();

    public:
        GLuint GetRawInterface() { return m_FBO; }

    private:
        IMemoryAllocator* m_Allocator;

        GLuint m_FBO;

        CRenderTarget* m_Color[MAX_MRT_NUM];
        CRenderTarget* m_Depth;

        IZ_BOOL m_IsOnOffScreen;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_FRAME_BUFFER_OBJECT_H__)
