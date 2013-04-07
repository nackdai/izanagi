// これは メイン DLL ファイルです。

#include "stdafx.h"

#include "MojoShaderWrapper.h"
#include "MojoShaderProxy.h"

namespace izanagi {
namespace tool {
    MojoShaderProxy::MojoShaderProxy()
    {
    }

    MojoShaderProxy::~MojoShaderProxy()
    {
    }

    MojoShaderProxy::!MojoShaderProxy()
    {
    }

    bool MojoShaderProxy::Parse(const char* filename, const char* output)
    {
        return ParseToGLSL(filename, output);
    }
}   // namespace tool
}   // namespace izanagi
