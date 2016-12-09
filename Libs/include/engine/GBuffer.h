#if !defined(__IZANAGI_ENGINE_GBUFFER_H__)
#define __IZANAGI_ENGINE_GBUFFER_H__

#include "izStd.h"
#include "izGraph.h"

namespace izanagi {
namespace engine {
    class GBuffer : public CObject {
    public:
        static const IZ_UINT BufferNameLength = 16;

    public:
        static GBuffer* crreate(IMemoryAllocator* allocator);

    private:
        GBuffer() {}
        ~GBuffer()
        {
            removeAll();
        }

        NO_COPIABLE(GBuffer);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        using Result = std::tuple < IZ_UINT, graph::CRenderTarget* > ;

        Result addBuffer(
            graph::CGraphicsDevice* device,
            graph::E_GRAPH_PIXEL_FMT format,
            IZ_UINT width, IZ_UINT height);

        IZ_BOOL removeBuffer(IZ_UINT idx);

        void removeAll();

        graph::CRenderTarget* getBuffer(IZ_UINT idx);

        IZ_UINT getBufferNum() const;

        IZ_BOOL begin(
            graph::CGraphicsDevice* device,
            IZ_UINT* targets, IZ_UINT targetNum,
            graph::E_GRAPH_CLEAR_FLAG clearFlag,
            IZ_COLOR clearColor = 0,
            IZ_FLOAT clearZ = 1.0f,
            IZ_DWORD clearStencil = 0);

        IZ_BOOL end(graph::CGraphicsDevice* device);

        void dumpBuffers(
            graph::CGraphicsDevice* device,
            IZ_UINT* targets, IZ_UINT targetNum,
            IZ_INT basex, IZ_INT basey,
            IZ_INT w, IZ_INT h);

    private:
        IZ_UINT getBuffers(
            graph::CRenderTarget* rts[],
            IZ_UINT* targets, IZ_UINT targetNum);

    private:
        IMemoryAllocator* m_Allocator{ nullptr };

        class Buffer;
        //using Buffers = CStdHash < CKey, Buffer, 4 > ;
        using Buffers = CStdList < Buffer >;

        Buffers m_buffers;

        using BufferName = CStdString < IZ_CHAR, BufferNameLength + 1 > ;

        class Buffer : public CPlacementNew {
        public:
            Buffer()
            {
                //item.Init(key, this);
                item.Init(this);
            }
            ~Buffer() {}

            graph::CRenderTarget* rt{ nullptr };
            Buffers::Item item;
        };

        inline void deleteBuffer(Buffer* buffer);
    };
}   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_GBUFFER_H__)

