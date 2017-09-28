#include "gldefs.h"
#include "shader.h"
#include <vector>

GLuint Shader::CreateShader(const char* shaderPath, GLenum type)
{
    FILE* fp = nullptr;
    fopen_s(&fp, shaderPath, "rt");
    IZ_ASSERT(fp != nullptr);

    fseek(fp, 0, SEEK_END);
    auto size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    std::vector<char> program(size);
    fread(&program[0], 1, size, fp);

    CALL_GL_API(auto shader = ::glCreateShader(type));
    IZ_ASSERT(shader != 0);

    const char* p = &program[0];
    const char** pp = &p;

    CALL_GL_API(::glShaderSource(
        shader,
        1,
        pp,
        nullptr));

    CALL_GL_API(::glCompileShader(shader));

    return shader;
}

GLuint Shader::CreateShaderProgram(GLuint vs, GLuint fs)
{
    auto program = ::glCreateProgram();
    assert(program != 0);

    CALL_GL_API(::glAttachShader(program, vs));
    CALL_GL_API(::glAttachShader(program, fs));

    CALL_GL_API(::glLinkProgram(program));

    GLint isLinked = 0;
    CALL_GL_API(::glGetProgramiv(program, GL_LINK_STATUS, &isLinked));

    if (isLinked == 0) {
        GLint infoLen = 0;

        CALL_GL_API(::glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen));

        if (infoLen > 1) {
            char* log = (char*)malloc(infoLen);
            memset(log, 0, infoLen);

            CALL_GL_API(::glGetProgramInfoLog(program, infoLen, NULL, log));
            IZ_ASSERT(false);

            free(log);
        }

        //assert(false);
    }

    return program;
}
