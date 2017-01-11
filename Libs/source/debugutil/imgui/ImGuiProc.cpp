#include <imgui.h>
#include "debugutil/imgui/ImGuiProc.h"
#include "izMath.h"

namespace izanagi
{
namespace debugutil
{
    class ImGuiRenderer {
    public:
        static void renderDrawLists(ImDrawData* draw_data);
    };

    void ImGuiRenderer::renderDrawLists(ImDrawData* draw_data)
    {
        ImGuiIO& io = ImGui::GetIO();

        auto fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
        auto fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);

        if (fb_width == 0 || fb_height == 0) {
            return;
        }

        draw_data->ScaleClipRects(io.DisplayFramebufferScale);

        auto proc = ImGuiProc::get();
        IZ_ASSERT(proc);

        proc->createGraphObjects(
            draw_data->TotalVtxCount,
            draw_data->TotalIdxCount);

        proc->createShader();

        auto device = proc->m_device;
        auto vb = proc->m_vb;
        auto ib = proc->m_ib;
        auto vd = proc->m_vd;
        auto shader = proc->m_shader;
        auto tex = proc->m_texture;

        device->SaveRenderState();

        device->SetRenderState(graph::E_GRAPH_RS_CULLMODE, graph::E_GRAPH_CULL_NONE);
        device->SetRenderState(graph::E_GRAPH_RS_ZENABLE, IZ_FALSE);
        device->SetRenderState(graph::E_GRAPH_RS_ZWRITEENABLE, IZ_FALSE);
        device->SetRenderState(graph::E_GRAPH_RS_ALPHABLENDENABLE, IZ_TRUE);
        device->SetRenderState(graph::E_GRAPH_RS_ALPHATESTENABLE, IZ_FALSE);
        device->SetRenderState(graph::E_GRAPH_RS_BLENDMETHOD, graph::E_GRAPH_ALPHA_BLEND_NORMAL);
        device->SetRenderState(graph::E_GRAPH_RS_SCISSORTESTENABLE, IZ_TRUE);

        device->SetViewport(
            graph::SViewport(
            0, 0,
            (IZ_UINT)io.DisplaySize.x,
            (IZ_UINT)io.DisplaySize.y));

        // Write to vertex buffer.
        {
            ImGuiProc::Vertex* dataVB = nullptr;
            vb->Lock(device, 0, draw_data->TotalVtxCount * sizeof(ImGuiProc::Vertex), (void**)&dataVB, IZ_FALSE, IZ_TRUE);

            for (IZ_INT n = 0; n < draw_data->CmdListsCount; n++)
            {
                const ImDrawList* cmd_list = draw_data->CmdLists[n];
                const ImDrawVert* vtx_src = cmd_list->VtxBuffer.Data;

                for (int i = 0; i < cmd_list->VtxBuffer.Size; i++)
                {
                    auto a = ((vtx_src->col & 0xFF000000) >> 24);
                    auto b = ((vtx_src->col & 0x00FF0000) >> 16);
                    auto g = ((vtx_src->col & 0x0000FF00) >> 8);
                    auto r = (vtx_src->col & 0x000000FF);

                    dataVB->pos[0] = vtx_src->pos.x;
                    dataVB->pos[1] = vtx_src->pos.y;
                    dataVB->pos[2] = 0.0f;
                    dataVB->pos[3] = 1.0f;
                    dataVB->color = IZ_COLOR_RGBA(r, g, b, a);
                    dataVB->uv[0] = vtx_src->uv.x;
                    dataVB->uv[1] = vtx_src->uv.y;

                    dataVB++;
                    vtx_src++;
                }
            }

            vb->Unlock(device);
        }

        // Write to index buffer.
        {
            ImDrawIdx* dataIB = nullptr;
            ib->Lock(device, 0, draw_data->TotalIdxCount * sizeof(ImDrawIdx), (void**)&dataIB, IZ_FALSE, IZ_TRUE);

            for (IZ_INT n = 0; n < draw_data->CmdListsCount; n++)
            {
                const ImDrawList* cmd_list = draw_data->CmdLists[n];
                memcpy(dataIB, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
                dataIB += cmd_list->IdxBuffer.Size;
            }

            ib->Unlock(device);
        }

        device->SetVertexBuffer(0, 0, sizeof(ImGuiProc::Vertex), vb);
        device->SetIndexBuffer(ib);
        device->SetVertexDeclaration(vd);

        device->SetShaderProgram(shader);

#if 1
        const float L = 0.0f;
        const float R = ImGui::GetIO().DisplaySize.x;
        const float B = ImGui::GetIO().DisplaySize.y;
        const float T = 0.0f;

        math::CMatrix44 mtxProj(
#if 1
            2.0f / (R - L), 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f / (T - B), 0.0f, 0.0f,
            0.0f, 0.0f, 0.5f, 0.0f,
            (R + L) / (L - R), (T + B) / (B - T), 0.5f, 1.0f);
#else
            2.0f / (R - L), 0.0f, 0.0f, (R + L) / (L - R),
            0.0f, 2.0f / (T - B), 0.0f, (T + B) / (B - T),
            0.0f, 0.0f, 0.5f, 0.5f,
            0.0f, 0.0f, 0.0f, 1.0f);
#endif
#else
        math::CMatrix44 mtxProj(
            2.0f / io.DisplaySize.x, 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f / -io.DisplaySize.y, 0.0f, 0.0f,
            0.0f, 0.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 1.0f);
#endif

        auto hProjMtx = shader->GetHandleByName("ProjMtx");
        shader->SetMatrix(device, hProjMtx, mtxProj);

        int vtx_offset = 0;
        int idx_offset = 0;

        // Draw.
        for (int n = 0; n < draw_data->CmdListsCount; n++)
        {
            const ImDrawList* cmd_list = draw_data->CmdLists[n];

            for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
            {
                const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
                if (pcmd->UserCallback)
                {
                    pcmd->UserCallback(cmd_list, pcmd);
                }
                else
                {
                    tex->overrideNativeResource((void*)pcmd->TextureId, graph::E_GRAPH_OVERRIDE_NATIVE_RSC_BEHAVIOUR::Assign);
                    tex->SetFilter(graph::E_GRAPH_TEX_FILTER_LINEAR, graph::E_GRAPH_TEX_FILTER_LINEAR, graph::E_GRAPH_TEX_FILTER_LINEAR);

                    device->SetTexture(0, tex);

                    CIntRect rc(
                        (IZ_INT)(pcmd->ClipRect.x),
                        (IZ_INT)(fb_height - pcmd->ClipRect.w),
                        (IZ_INT)(pcmd->ClipRect.z - pcmd->ClipRect.x),
                        (IZ_INT)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                    device->SetScissorTestRect(rc);

                    device->DrawIndexedPrimitive(
                        graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST,
                        vtx_offset,
                        cmd_list->VtxBuffer.Size,
                        idx_offset,
                        pcmd->ElemCount / 3);
                }

                idx_offset += pcmd->ElemCount;
            }

            vtx_offset += cmd_list->VtxBuffer.Size;
        }

        device->LoadRenderState();
    }

    ImGuiProc* ImGuiProc::s_instance = nullptr;

    ImGuiProc* ImGuiProc::init(
        IMemoryAllocator* allocator,
        graph::CGraphicsDevice* device)
    {
        if (s_instance) {
            return s_instance;
        }

        IZ_BOOL result = IZ_FALSE;

        void* buf = ALLOC_ZERO(allocator, sizeof(ImGuiProc));
        VRETURN_NULL(buf);

        ImGuiProc* instance = new(buf)ImGuiProc;
        {
            instance->m_Allocator = allocator;
            SAFE_REPLACE(instance->m_device, device);
            instance->AddRef();

            result = instance->init();
        }

        if (result) {
            s_instance = instance;
        }

        return instance;
    }

    ImGuiProc* ImGuiProc::get()
    {
        IZ_ASSERT(s_instance);

        return s_instance;
    }

    ImGuiProc::~ImGuiProc()
    {
        SAFE_RELEASE(m_device);
        SAFE_RELEASE(m_font);
        SAFE_RELEASE(m_shader);
        SAFE_RELEASE(m_vb);
        SAFE_RELEASE(m_ib);
        SAFE_RELEASE(m_vd);

        m_texture->overrideNativeResource(nullptr, graph::E_GRAPH_OVERRIDE_NATIVE_RSC_BEHAVIOUR::Assign);
        SAFE_RELEASE(m_texture);

        s_instance = nullptr;
    }

    void ImGuiProc::beginFrame()
    {
        createFontTexture();

        ImGuiIO& io = ImGui::GetIO();

        auto size = sys::CSysWindow::getDisplaySize();
        io.DisplaySize.x = (float)size.x;
        io.DisplaySize.y = (float)size.y;

        auto w = m_device->GetBackBufferWidth();
        auto h = m_device->GetBackBufferHeight();
        io.DisplayFramebufferScale.x = w / (IZ_FLOAT)size.x;
        io.DisplayFramebufferScale.y = h / (IZ_FLOAT)size.y;

        // TODO.
        // Fix time?
        auto time = m_timer.End();
        io.DeltaTime = time;
        m_timer.Begin();

        if (io.MouseDrawCursor) {
            sys::CSysWindow::HideCursor();
        }

        // Start the frame.
        ImGui::NewFrame();
    }

    void ImGuiProc::OnKeyDown(sys::E_KEYBOARD_BUTTON key)
    {
        ImGuiIO& io = ImGui::GetIO();

        auto org = sys::CSysWindow::GetOriginalKey(key);

        if (org >= 0) {
            if (CStdUtf::IsAscii(org)) {
                io.AddInputCharacter(org);
            }
            else if (org < 256) {
                io.KeysDown[org] = true;
            }
        }
    }

    void ImGuiProc::OnKeyUp(sys::E_KEYBOARD_BUTTON key)
    {
        ImGuiIO& io = ImGui::GetIO();

        auto org = sys::CSysWindow::GetOriginalKey(key);

        if (org >= 0) {
            if (org < 256) {
                io.KeysDown[org] = false;
            }
        }
    }

    void ImGuiProc::OnMouseLBtnDown(const CIntPoint& point)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[0] = true;
    }

    void ImGuiProc::OnMouseLBtnUp(const CIntPoint& point)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[0] = false;
    }

    void ImGuiProc::OnMouseRBtnDown(const CIntPoint& point)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[1] = true;
    }

    void ImGuiProc::OnMouseRBtnUp(const CIntPoint& point)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[1] = false;
    }

    void ImGuiProc::OnMouseMove(const CIntPoint& point)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos.x = (IZ_FLOAT)point.x;
        io.MousePos.y = (IZ_FLOAT)point.y;
    }

    void ImGuiProc::OnMouseWheel(IZ_INT delta)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += delta > 0 ? 1.0f : -1.0f;
    }

    void ImGuiProc::OnInit(const sys::WindowHandle& handle)
    {
    }

    void ImGuiProc::OnTerminate()
    {
        ImGui::Shutdown();
    }

    void ImGuiProc::OnDestroy()
    {
    }

    IZ_BOOL ImGuiProc::init()
    {
        ImGuiIO& io = ImGui::GetIO();

        io.KeyMap[ImGuiKey_Tab] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_TAB);
        io.KeyMap[ImGuiKey_LeftArrow] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_LEFT);
        io.KeyMap[ImGuiKey_RightArrow] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_RIGHT);
        io.KeyMap[ImGuiKey_UpArrow] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_UP);
        io.KeyMap[ImGuiKey_DownArrow] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_DOWN);
        io.KeyMap[ImGuiKey_PageUp] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_PRIOR);
        io.KeyMap[ImGuiKey_PageDown] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_NEXT);
        io.KeyMap[ImGuiKey_Home] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_HOME);
        io.KeyMap[ImGuiKey_End] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_END);
        io.KeyMap[ImGuiKey_Delete] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_DELETE);
        io.KeyMap[ImGuiKey_Backspace] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_BACK);
        io.KeyMap[ImGuiKey_Enter] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_RETURN);
        io.KeyMap[ImGuiKey_Escape] = sys::CSysWindow::GetOriginalKey(sys::E_KEYBOARD_BUTTON_ESCAPE);
        io.KeyMap[ImGuiKey_A] = 'A';
        io.KeyMap[ImGuiKey_C] = 'C';
        io.KeyMap[ImGuiKey_V] = 'V';
        io.KeyMap[ImGuiKey_X] = 'X';
        io.KeyMap[ImGuiKey_Y] = 'Y';
        io.KeyMap[ImGuiKey_Z] = 'Z';

        // Load Fonts
        // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
        io.Fonts->AddFontDefault();

        io.RenderDrawListsFn = &ImGuiRenderer::renderDrawLists;

        return IZ_TRUE;
    }

    IZ_BOOL ImGuiProc::createFontTexture()
    {
        if (!m_font) {
            IZ_ASSERT(m_device);

            ImGuiIO& io = ImGui::GetIO();
            unsigned char* pixels;
            int width, height, bytes_per_pixel;
            io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);

            m_font = m_device->CreateTexture(
                width, height,
                graph::E_GRAPH_PIXEL_FMT_RGBA8,
                pixels);
            VRETURN_NULL(m_font);

            auto nativeTex = m_font->GetTexHandle();

            io.Fonts->TexID = (void*)nativeTex;
        }

        return IZ_TRUE;
    }

    IZ_BOOL ImGuiProc::createGraphObjects(IZ_UINT vtxCount, IZ_UINT idxCount)
    {
        if (m_vb) {
            auto num = m_vb->GetVtxNum();
            if (vtxCount > num) {
                SAFE_RELEASE(m_vb);
            }
        }

        if (m_ib) {
            auto num = m_ib->GetIdxNum();
            if (idxCount > num) {
                SAFE_RELEASE(m_ib);
            }
        }

        if (!m_vb) {
            m_vb = m_device->CreateVertexBuffer(
                sizeof(Vertex),
                vtxCount,
                graph::E_GRAPH_RSC_USAGE_DYNAMIC);
            VRETURN(m_vb);
        }

        if (!m_ib) {
            m_ib = m_device->CreateIndexBuffer(
                idxCount,
                sizeof(ImDrawIdx) == 2 ? graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX16 : graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX32,
                graph::E_GRAPH_RSC_USAGE_DYNAMIC);
            VRETURN(m_ib);
        }

        if (!m_vd) {
            static const graph::SVertexElement elems[] = {
                { 0, 0, graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4, graph::E_GRAPH_VTX_DECL_USAGE_POSITION, 0},
                { 0, 16, graph::E_GRAPH_VTX_DECL_TYPE_COLOR, graph::E_GRAPH_VTX_DECL_USAGE_COLOR, 0},
                { 0, 20, graph::E_GRAPH_VTX_DECL_TYPE_FLOAT2, graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD, 0},
            };

            m_vd = m_device->CreateVertexDeclaration(elems, COUNTOF(elems));
            VRETURN(m_vd);
        }

        if (!m_texture) {
            m_texture = m_device->CreateTexture(
                1, 1,
                graph::E_GRAPH_PIXEL_FMT_RGBA8,
                nullptr);
            VRETURN(m_texture);

            m_texture->Disable();
        }

        return IZ_TRUE;
    }

#ifdef __IZ_DX9__
    #include "debugutil/imgui/mainVS.h"
    #include "debugutil/imgui/mainPS.h"

    static const IZ_BYTE* VS_Program = g_vs20_mainVS;
    static const IZ_BYTE* PS_Program = g_ps20_mainPS;
#elif  __IZ_OGL__
    #define GLSL_EXTERNAL_INCLUDE(...) #__VA_ARGS__

    #if defined(__IZ_GLUT__) || defined(__IZ_GLFW__)
        static const IZ_CHAR VS_Program[] = {
            #include "debugutil/imgui/imgui.vs"
        };
        static const IZ_CHAR PS_Program[] = {
            #include "debugutil/imgui/imgui.fs"
        };
    #elif __IZ_GLES2__
        IZ_C_ASSERT(IZ_FALSE);
    #endif
#else
    IZ_C_ASSERT(IZ_FALSE);
#endif

    IZ_BOOL ImGuiProc::createShader()
    {
        if (m_shader) {
            return IZ_TRUE;
        }

        IZ_ASSERT(m_device);

        auto vs = m_device->CreateVertexShader(VS_Program);
        auto ps = m_device->CreatePixelShader(PS_Program);

        IZ_BOOL result = vs && ps;
        VGOTO(result, __EXIT__);

        m_shader = m_device->CreateShaderProgram();

        result = (m_shader != nullptr);
        VGOTO(result, __EXIT__);

        m_shader->AttachVertexShader(vs);
        m_shader->AttachPixelShader(ps);
        
    __EXIT__:
        SAFE_RELEASE(vs);
        SAFE_RELEASE(ps);

        return result;
    }
}   // namespace debugutil
}   // namespace izanagi
