#pragma once

#include <assert.h>
#include <GL/glew.h>

class Shader {
public:
    Shader() {}
    ~Shader() {}

    bool Init(
        const char* pathVS,
        const char* pathFS)
    {
        m_vs = CreateShader(pathVS, GL_VERTEX_SHADER);
        assert(m_vs > 0);

        m_fs = CreateShader(pathFS, GL_FRAGMENT_SHADER);
        assert(m_vs > 0);

        if (m_vs > 0 && m_fs > 0) {
            m_program = CreateShaderProgram(m_vs, m_fs);
            assert(m_program > 0);
        }

        return (m_program > 0);
    }

    GLuint CreateShader(const char* shaderPath, GLenum type);

    GLuint CreateShaderProgram(GLuint vs, GLuint fs);

public:
    GLuint m_vs{ 0 };
    GLuint m_fs{ 0 };
    GLuint m_program{ 0 };
};

