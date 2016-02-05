#if !defined(__DXT_ENCODER_H__)
#define __DXT_ENCODER_H__

#include "izSampleKit.h"

class DxtEncoder
{
public:
    DxtEncoder() {}
    ~DxtEncoder();

public:
    IZ_BOOL init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT width, IZ_UINT height,
        const char* vtxShader,
        const char* dxtShader,
        const char* pixelShader);

    void encode(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::graph::CTexture* texture);

    void terminate();

private:
    izanagi::graph::CVertexShader* m_vs{ nullptr };
    izanagi::graph::CPixelShader* m_dxt{ nullptr };

    izanagi::graph::CShaderProgram* m_shd{ nullptr };

    IZ_UINT m_width{ 0 };
    IZ_UINT m_height{ 0 };

    izanagi::SHADER_PARAM_HANDLE m_hImage{ 0 };
    izanagi::SHADER_PARAM_HANDLE m_hMode{ 0 };

    GLuint m_fbo;
    izanagi::graph::CTexture* m_tex{ nullptr };

    GLuint m_pbo;
};

#endif    // #if !defined(__DXT_ENCODER_H__)