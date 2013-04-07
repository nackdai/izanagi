// MojoShaderProxy.h

#pragma once

using namespace System;

namespace izanagi {
namespace tool {

	public ref class MojoShaderProxy
	{
    private:
        MojoShaderProxy();
        ~MojoShaderProxy();
        !MojoShaderProxy();

    public:
        static bool Parse(const char* filename, const char* output);
	};
}   // namespace tool
}   // namespace izanagi
