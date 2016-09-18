#if !defined(__SAMPLE_SHADER_H__)
#define __SAMPLE_SHADER_H__

#include "izGraph.h"
#include "izSystem.h"

namespace izanagi {
namespace sample {
    class Shader {
    public:
        Shader() {}
        ~Shader()
        {
            release();
        }

        IZ_BOOL init(
            izanagi::graph::CGraphicsDevice* device,
            const char* vsPath,
            const char* psPath)
        {
            IZ_BOOL result = IZ_TRUE;

            izanagi::graph::CVertexShader* vs = nullptr;
            izanagi::graph::CPixelShader* ps = nullptr;

            {
                izanagi::CFileInputStream in;
                in.Open(vsPath);

                auto size = in.GetSize();

                std::vector<char> program(size + 1);
                in.Read(&program[0], 0, size);
                program[size] = 0;

                vs = device->CreateVertexShader(&program[0]);
                VGOTO(result = (vs != nullptr), __EXIT__);
            }

            {
                izanagi::CFileInputStream in;
                in.Open(psPath);

                auto size = in.GetSize();

                std::vector<char> program(size + 1);
                in.Read(&program[0], 0, size);
                program[size] = 0;

                ps = device->CreatePixelShader(&program[0]);
                VGOTO(result = (ps != nullptr), __EXIT__);
            }

            m_program = device->CreateShaderProgram();
            VGOTO(result = (m_program != nullptr), __EXIT__);

            result = m_program->AttachVertexShader(vs);
            if (result) {
                result = m_program->AttachPixelShader(ps);
            }

        __EXIT__:
            if (!result) {
                release();
            }

            SAFE_RELEASE(vs);
            SAFE_RELEASE(ps);

            return result;
        }

        void release()
        {
            SAFE_RELEASE(m_program);
        }

    public:
        izanagi::graph::CShaderProgram* m_program{ nullptr };
    };
}
}

#endif    // #if !defined(__SAMPLE_OBJ_MODEL_H__)