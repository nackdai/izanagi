#if !defined(__IZANAGI_IMGUI_PROC_H__)
#define __IZANAGI_IMGUI_PROC_H__

#include <imgui.h>

#include "izDefs.h"
#include "izStd.h"
#include "izSystem.h"
#include "izGraph.h"

namespace izanagi
{
namespace debugutil
{
    class ImGuiProc: public sys::CMessageHandler, CObject {
    public:
        static ImGuiProc* init(
            IMemoryAllocator* allocator,
            graph::CGraphicsDevice* device);

        static ImGuiProc* get();

    private:
        static ImGuiProc* s_instance;

    private:
        ImGuiProc() {}
        virtual ~ImGuiProc();

        NO_COPIABLE(ImGuiProc);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        void beginFrame();

    private:
        virtual void OnKeyDown(sys::E_KEYBOARD_BUTTON key) override;

        virtual void OnKeyUp(sys::E_KEYBOARD_BUTTON key) override;

        virtual void OnMouseLBtnDown(const CIntPoint& point) override;

        virtual void OnMouseLBtnUp(const CIntPoint& point) override;

        virtual void OnMouseRBtnDown(const CIntPoint& point) override;

        virtual void OnMouseRBtnUp(const CIntPoint& point) override;

        virtual void OnMouseMove(const CIntPoint& point) override;

        virtual void OnMouseWheel(IZ_INT delta) override;

        virtual void OnInit(const sys::WindowHandle& handle) override;

        virtual void OnTerminate() override;
        virtual void OnDestroy() override;

    private:
        static void renderDrawLists(ImDrawData* draw_data);

        IZ_BOOL createFontTexture();
        IZ_BOOL createGraphObjects(IZ_UINT vtxCount, IZ_UINT idxCount);
        IZ_BOOL createShader();

        struct Vertex {
            IZ_FLOAT pos[4];
            IZ_COLOR color;
            IZ_FLOAT uv[2];
        };

    private:
        IMemoryAllocator* m_Allocator{ nullptr };

        graph::CGraphicsDevice* m_device{ nullptr };
        graph::CTexture* m_font{ nullptr };
        graph::CTexture* m_texture{ nullptr };
        graph::CShaderProgram* m_shader{ nullptr };
        graph::CVertexBuffer* m_vb{ nullptr };
        graph::CIndexBuffer* m_ib{ nullptr };
        graph::CVertexDeclaration* m_vd{ nullptr };

        sys::CTimer m_timer;
    };
}   // namespace debugutil
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_IMGUI_PROC_H__)
