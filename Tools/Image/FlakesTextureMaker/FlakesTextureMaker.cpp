#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmdline.h>

#include "gldefs.h"
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static const char* TITLE = "FlakesTextureMaker";

Shader g_shd;

struct Options {
    std::string output;
    int width{ 1280 };
    int height{ 720 };

    float flake_scale{ 50.0f };               // Smaller values zoom into the flake map, larger values zoom out.
    float flake_size{ 0.5f };                 // Relative size of the flakes
    float flake_size_variance{ 0.7f };        // 0.0 makes all flakes the same size, 1.0 assigns random size between 0 and the given flake size
    float flake_normal_orientation{ 0.5f };   // Blend between the flake normals (0.0) and the surface normal (1.0)
};

Options g_opt;

static inline GLint GetHandle(GLuint program, const char* name)
{
    CALL_GL_API(auto handle = ::glGetUniformLocation(program, name));
    return handle;
}

float time = 0.0f;

static void Display()
{
    CALL_GL_API(::glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    CALL_GL_API(::glClearDepthf(1.0f));
    CALL_GL_API(::glClearStencil(0));
    CALL_GL_API(::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

    // Disable culling for rendering full screen quad without vertex buffer.
    CALL_GL_API(::glDisable(GL_CULL_FACE));

    CALL_GL_API(::glUseProgram(g_shd.m_program));

    auto hResolution = GetHandle(g_shd.m_program, "u_resolution");
    CALL_GL_API(glUniform4f(hResolution, g_opt.width, g_opt.height, 0.0f, 0.0f));

    auto hTime = GetHandle(g_shd.m_program, "u_time");
    CALL_GL_API(glUniform1f(hTime, 1));

    auto hSize = GetHandle(g_shd.m_program, "flake_size");
    CALL_GL_API(glUniform1f(hSize, g_opt.flake_size));

    auto hScale = GetHandle(g_shd.m_program, "flake_scale");
    CALL_GL_API(glUniform1f(hScale, g_opt.flake_scale));

    auto hVar = GetHandle(g_shd.m_program, "flake_size_variance");
    CALL_GL_API(glUniform1f(hVar, g_opt.flake_size_variance));

    auto hOrientation = GetHandle(g_shd.m_program, "flake_normal_orientation");
    CALL_GL_API(glUniform1f(hOrientation, g_opt.flake_normal_orientation));

    time += 1.0f / 60.0f;

    CALL_GL_API(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
}

static void Init()
{
    GLenum result = glewInit();
    assert(result == GLEW_OK);

    auto version = ::glGetString(GL_VERSION);
    IZ_PRINTF("GL Version(%s)\n", version);

    CALL_GL_API(::glClipControl(
        GL_LOWER_LEFT,
        GL_ZERO_TO_ONE));

    CALL_GL_API(::glFrontFace(GL_CCW));

    CALL_GL_API(::glViewport(0, 0, g_opt.width, g_opt.height));
    CALL_GL_API(::glDepthRangef(0.0f, 1.0f));

    g_shd.Init("./FlakesTextureMaker_vs.glsl", "./FlakesTextureMaker_fs.glsl");
}

static void closeCallback(GLFWwindow* window)
{
    ::glfwSetWindowShouldClose(window, GL_TRUE);
}

static void errorCallback(int error, const char* description)
{
    IZ_PRINTF("Error: %s\n", description);
}

inline float clamp(float x, float a, float b)
{
    x = (x < a ? a : x);
    x = (x > b ? b : x);
    return x;
}

bool parseOption(
    int argc, char* argv[],
    cmdline::parser& cmd,
    Options& opt)
{
    {
        cmd.add<std::string>("output", 'o', "output filename base", false, "result");
        cmd.add<int>("width", 'w', "output map width", false, opt.width);
        cmd.add<int>("height", 'h', "output map height", false, opt.height);

        cmd.add<float>("flake_size", 's', "Smaller values zoom into the flake map, larger values zoom out", false, opt.flake_size);
        cmd.add<float>("flake_scale", 'c', "Relative size of the flakes", false, opt.flake_scale);
        cmd.add<float>("flake_size_variance", 'v', "0.0 makes all flakes the same size, 1.0 assigns random size between 0 and the given flake size", false, opt.flake_size_variance);
        cmd.add<float>("flake_normal_orientation", 'n', "Blend between the flake normals (0.0) and the surface normal (1.0)", false, opt.flake_normal_orientation);

        cmd.add<std::string>("help", '?', "print usage", false);
    }

    bool isCmdOk = cmd.parse(argc, argv);

    if (argc == 1 || cmd.exist("help")){
        std::cerr << cmd.usage();
        return false;
    }

    if (!isCmdOk){
        std::cerr << cmd.error() << std::endl << cmd.usage();
        return false;
    }

    if (cmd.exist("output")) {
        auto output = cmd.get<std::string>("output");

        // TODO
        // 拡張子...

        opt.output = output + ".png";
    }
    else {
        opt.output = "result.png";
    }

    if (cmd.exist("width")) {
        opt.width = cmd.get<int>("width");
    }
    if (cmd.exist("height")) {
        opt.height = cmd.get<int>("height");
    }

    if (cmd.exist("flake_size")) {
        opt.flake_size = cmd.get<float>("flake_size");
    }
    if (cmd.exist("flake_scale")) {
        opt.flake_scale = cmd.get<float>("flake_scale");
    }
    if (cmd.exist("flake_size_variance")) {
        opt.flake_normal_orientation = cmd.get<float>("flake_size_variance");
    }
    if (cmd.exist("flake_normal_orientation")) {
        opt.flake_normal_orientation = cmd.get<float>("flake_orientation");
    }

    opt.flake_size = max(opt.flake_size, 0.0001f);
    opt.flake_scale = max(opt.flake_scale, 0.0001f);
    opt.flake_size_variance = clamp(opt.flake_size_variance, 0.0f, 1.0f);
    opt.flake_normal_orientation = clamp(opt.flake_normal_orientation, 0.0f, 1.0f);

    return true;
}

int main(int argc, char* argv[])
{
    cmdline::parser cmd;

    if (!parseOption(argc, argv, cmd, g_opt)) {
        return 1;
    }

    ::glfwSetErrorCallback(errorCallback);

    auto result = ::glfwInit();
    if (!result) {
        assert(false);
        return 1;
    }

    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    auto window = ::glfwCreateWindow(
        g_opt.width, g_opt.height,
        TITLE,
        NULL, NULL);

    if (!window) {
        ::glfwTerminate();
        assert(false);
        return 1;
    }

    ::glfwSetWindowCloseCallback(
        window,
        closeCallback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    Init();

    std::vector<uint8_t> pixels(g_opt.width * g_opt.height * 4);

    while (!glfwWindowShouldClose(window)) {
        Display();

        ::glfwSwapBuffers(window);
        ::glfwPollEvents();

        {
            CALL_GL_API(::glFlush());
            CALL_GL_API(::glFinish());

            CALL_GL_API(::glReadBuffer(GL_FRONT));

            CALL_GL_API(::glReadPixels(0, 0, g_opt.width, g_opt.height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));
        }

        break;
    }

    ::glfwDestroyWindow(window);
    ::glfwTerminate();

    {
        static const int bpp = sizeof(uint8_t) * 4;
        const int pitch = g_opt.width * bpp;

#if 0
        std::vector<uint8_t> dst(g_opt.width * g_opt.height * 3);

#pragma omp parallel for
        for (int y = 0; y < g_opt.height; y++) {
            int yy = g_opt.height - 1 - y;

            memcpy(
                &dst[yy * pitch],
                &pixels[y * pitch],
                pitch);
        }

        auto ret = ::stbi_write_png(g_opt.output.c_str(), g_opt.width, g_opt.height, bpp, &dst[0], pitch);
#else
        // OpenGLなので上下反転しているが、flakesテクスチャの場合は上下変転していてもほとんど影響がないのでそのまま出力.
        auto ret = ::stbi_write_png(g_opt.output.c_str(), g_opt.width, g_opt.height, bpp, &pixels[0], pitch);
#endif
        IZ_ASSERT(ret > 0);
    }

    return 1;
}