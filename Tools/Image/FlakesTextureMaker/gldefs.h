#pragma once

#include "izDefs.h"

#define CALL_GL_API(func)\
    func; \
        {\
        GLenum __gl_err__ = ::glGetError();\
        if (__gl_err__ != GL_NO_ERROR) { IZ_PRINTF("GL Error[%x]\n", __gl_err__); IZ_ASSERT(false); }\
        }
