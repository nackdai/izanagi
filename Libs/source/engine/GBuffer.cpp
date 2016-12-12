#include "engine/GBuffer.h"

namespace izanagi {
namespace engine {
    GBuffer* GBuffer::crreate(IMemoryAllocator* allocator)
    {
        IZ_ASSERT(allocator);

        void* buf = ALLOC(allocator, sizeof(GBuffer));
        VRETURN_NULL(buf);

        GBuffer* instance = new(buf)GBuffer;
        {
            instance->m_Allocator = allocator;

            instance->AddRef();
        }

        return instance;
    }

    GBuffer::Result GBuffer::addBuffer(
        graph::CGraphicsDevice* device,
        IZ_UINT width, IZ_UINT height,
        graph::E_GRAPH_PIXEL_FMT format)
    {
        IZ_ASSERT(device);

        Result res(0, nullptr);

        void* buf = ALLOC(m_Allocator, sizeof(Buffer));
        if (!buf) {
            return res;
        }

        IZ_UINT idx = m_buffers.GetItemNum();

        Buffer* buffer = new(buf)Buffer;
        
        buffer->rt = device->CreateRenderTarget(width, height, format);
        if (!buffer->rt) {
            deleteBuffer(buffer);
            return res;
        }

        m_buffers.AddTail(&buffer->item);

        res = Result(idx, buffer->rt);

        return res;
    }

    void GBuffer::deleteBuffer(GBuffer::Buffer* buffer)
    {
        if (buffer) {
            SAFE_RELEASE(buffer->rt);

            delete buffer;
            FREE(m_Allocator, buffer);
        }
    }

    IZ_BOOL GBuffer::removeBuffer(IZ_UINT idx)
    {
        auto item = m_buffers.GetAt(idx);

        if (!item) {
            return IZ_FALSE;
        }

        auto buffer = item->GetData();

        deleteBuffer(buffer);

        return IZ_TRUE;
    }

    void GBuffer::removeAll()
    {
        auto item = m_buffers.GetTop();

        while (item) {
            auto next = item->GetNext();

            auto buffer = item->GetData();
            deleteBuffer(buffer);

            item = next;
        }
    }

    graph::CRenderTarget* GBuffer::getBuffer(IZ_UINT idx)
    {
        auto item = m_buffers.GetAt(idx);

        if (!item) {
            return nullptr;
        }

        auto buffer = item->GetData();
        auto rt = buffer->rt;

        return rt;
    }

    IZ_UINT GBuffer::getBufferNum() const
    {
        auto ret = m_buffers.GetItemNum();
        return ret;
    }

    IZ_UINT GBuffer::getBuffers(
        graph::CRenderTarget* rts[],
        IZ_UINT* targets, IZ_UINT targetNum)
    {
        IZ_ASSERT(targets);
        IZ_ASSERT(targetNum <= getBufferNum());

        IZ_UINT rtNum = 0;

        for (IZ_UINT i = 0; i < targetNum; i++) {
            const auto target = targets[i];

            auto item = m_buffers.GetAt(target);
            IZ_ASSERT(item);

            if (item) {
                auto buffer = item->GetData();
                auto rt = buffer->rt;
                IZ_ASSERT(rt);

                rts[rtNum++] = rt;
            }
        }

        return rtNum;
    }

    IZ_BOOL GBuffer::begin(
        graph::CGraphicsDevice* device,
        IZ_UINT* targets, IZ_UINT targetNum,
        graph::E_GRAPH_CLEAR_FLAG clearFlag/*= graph::E_GRAPH_CLEAR_FLAG_ALL*/,
        IZ_COLOR clearColor/*= 0*/,
        IZ_FLOAT clearZ/*= 1.0f*/,
        IZ_DWORD clearStencil/*= 0*/)
    {
        IZ_ASSERT(device);
        
        IZ_BOOL result = IZ_FALSE;

        graph::CRenderTarget* rts[MaxRT] = { nullptr };

        IZ_ASSERT(targetNum < MaxRT);

        auto rtNum = getBuffers(rts, targets, targetNum);

        if (rtNum > 0) {
            result = device->BeginScene(
                rts, rtNum,
                clearFlag,
                clearColor,
                clearZ,
                clearStencil);
        }

        IZ_ASSERT(result);
        return result;
    }

    IZ_BOOL GBuffer::end(graph::CGraphicsDevice* device)
    {
        device->EndScene();
        return IZ_TRUE;
    }

    IZ_BOOL GBuffer::bind(
        graph::CGraphicsDevice* device,
        BindOp* targets, IZ_UINT targetNum)
    {
        IZ_ASSERT(targetNum <= getBufferNum());

        for (IZ_UINT i = 0; i < targetNum; i++) {
            auto op = targets[i];

            auto item = m_buffers.GetAt(op.idx);
            IZ_ASSERT(item);

            if (item) {
                auto buffer = item->GetData();
                auto rt = buffer->rt;
                IZ_ASSERT(rt);

                auto result = device->SetTexture(op.stage, rt);
                VRETURN(result);
            }
        }

        return IZ_TRUE;
    }

    void GBuffer::dumpBuffers(
        graph::CGraphicsDevice* device,
        IZ_UINT* targets, IZ_UINT targetNum,
        IZ_INT basex, IZ_INT basey,
        IZ_INT w, IZ_INT h)
    {
        auto width = device->GetBackBufferWidth();
        auto height = device->GetBackBufferHeight();

        IZ_INT x = basex;
        IZ_INT y = basey;

        IZ_ASSERT(device);

        graph::CRenderTarget* rts[MaxRT] = { nullptr };

        IZ_ASSERT(targetNum < MaxRT);

        auto rtNum = getBuffers(rts, targets, targetNum);

        if (rtNum > 0) {
            if (device->Begin2D()) {
                for (IZ_UINT i = 0; i < rtNum; i++) {
                    device->SetTexture(0, rts[i]);

                    if (y + h > height) {
                        y = basey;
                        x += w;
                    }

                    device->Draw2DSprite(
                        izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
                        izanagi::CIntRect(x, y, w, h));

                    y += h;
                }

                device->End2D();
            }
        }
    }
}   // namespace engine
}   // namespace izanagi

