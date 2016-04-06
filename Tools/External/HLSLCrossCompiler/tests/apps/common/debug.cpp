#if defined(_DEBUG)
#include "common/debug.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

static void APIENTRY OpenGLDebugCallback (GLenum source,
                                        GLenum type,
                                        GLuint id,
                                        GLenum severity,
                                        GLsizei length,
                                        const GLchar* message,
                                        GLvoid* userParam)
{
    //String mappings
    const char * pszSource = NULL;
    const char * pszType = NULL;
    const char * pszSeverity = NULL;

    switch(source)
    {
        case GL_DEBUG_SOURCE_API_ARB:
        {
            pszSource = "OpenGL API";
            break;
        }
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
        {
            pszSource = "Window system";
            break;
        }
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
        {
            pszSource = "OpenGL shader compiler";
            break;
        }
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
        {
            pszSource = "3rd party";
            break;
        }
        case GL_DEBUG_SOURCE_APPLICATION_ARB:
        {
            pszSource = "Self";
            break;
        }
        case GL_DEBUG_SOURCE_OTHER_ARB:
        {
            pszSource = "Other";
            break;
        }
        default:
        {
            pszSource = "Unknown";
            break;
        }
    }

    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH_ARB:
        {
            pszSeverity = "High";
            break;
        }
        case GL_DEBUG_SEVERITY_MEDIUM_ARB:
        {
            pszSeverity = "Medium";
            break;
        }
        case GL_DEBUG_SEVERITY_LOW_ARB:
        {
            pszSeverity = "Low";
            break;
        }
        default:
        {
            pszSeverity = "Unknown";
            break;
        }
    }

    switch(type)
    {
        case GL_DEBUG_TYPE_ERROR_ARB:
        {
            pszType = "Error";
            break;
        }
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
        {
            pszType = "Deprecated";
            break;
        }
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
        {
            pszType = "Undefined";
            break;
        }
        case GL_DEBUG_TYPE_PORTABILITY_ARB:
        {
            pszType = "Portability";
            break;
        }
        case GL_DEBUG_TYPE_PERFORMANCE_ARB:
        {
            pszType = "Performance";
            break;
        }
        case GL_DEBUG_TYPE_OTHER_ARB:
        {
            pszType = "Other";
            break;
        }
        default:
        {
            pszType = "Unknown";
            break;
        }
    }

    std::string fullMessage = std::string(message) + std::string("\n");
    DebugString(fullMessage.c_str());

    /* Make sure important messages are noticed. */
    //ASSERT(severity == GL_DEBUG_SEVERITY_LOW_ARB);
}

void SetupOpenGLDebugCallback()
{
    if(GLEW_ARB_debug_output)
    {
        glDebugMessageCallbackARB(OpenGLDebugCallback, NULL);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

        DebugString("ARB_debug_output is available. Enabling debug callback\n");

        /* All messages are initially enabled unless their assigned severity is DEBUG_SEVERITY_LOW_ARB.*/

        /* Enable low severity */
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW_ARB, 0, NULL, GL_TRUE);

        /* Enable all */
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    }
}
#endif
