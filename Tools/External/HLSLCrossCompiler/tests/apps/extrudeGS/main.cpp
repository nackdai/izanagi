
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <Shader.h>
#include <vectormath_aos.h>
#include "common/debug.h"
#include <model.h>
#include <itransform.h>

struct Vertex
{
    float Pos[3];
    float Normal[3];
	float TexCoord[3];
};

struct cbConstant
{
	float vLightDir[3];
	float padding;
};

struct cbChangesEveryFrame
{
	float World[16];
	float View[16];
	float Projection[16];
	float Time;
	float padding[3];
};

struct cbUserChanges
{
    float Explode;
	float padding[3];
};

using namespace Vectormath::Aos;

int WindowWidth = 640;
int WindowHeight = 480;

class Demo : public ITransform
{
public:
	void Init();
	void Display(float t);
	void ResizeDisplay(int w, int h) {
		glViewport (0, 0, (GLsizei) w, (GLsizei) h);
        //D3D clip space (z = 0, +1) to GL (z = -1 to +1)
        gProjection = Matrix4::scale(Vector3(1, -1, 2));
        gProjection *= Matrix4::translation(Vector3(0, 0, -0.5));
		gProjection *= Matrix4::perspective(3.14159f * 0.25f, w / ( float )h, 0.1f, 5000.0f);

	}
	const Matrix4& GetWorldMatrix() const {
		return gWorld;
	}
	void SetWorldMatrix(Matrix4& newWorld) {
		gWorld = newWorld;
		UpdateShaderConstants();
	}

	void UpdateShaderConstants() {

		SetFloatArray(gWorld, gChangesEveryFrame.World);
		SetFloatArray(gView, gChangesEveryFrame.View);
		SetFloatArray(gProjection, gChangesEveryFrame.Projection);

		gChangesEveryFrame.Time = gTime;
		mExtrudeEffect.SetMat4(std::string("cbChangesEveryFrameVS.World"), 1, gChangesEveryFrame.World);
		mExtrudeEffect.SetMat4(std::string("cbChangesEveryFrameVS.View"), 1, gChangesEveryFrame.View);
		mExtrudeEffect.SetMat4(std::string("cbChangesEveryFrameVS.Projection"), 1, gChangesEveryFrame.Projection);
		mExtrudeEffect.SetVec4(std::string("cbChangesEveryFrameVS.Time"), 1, &gChangesEveryFrame.Time);

		mExtrudeEffect.SetMat4(std::string("cbChangesEveryFrameGS.World"), 1, gChangesEveryFrame.World);
		mExtrudeEffect.SetMat4(std::string("cbChangesEveryFrameGS.View"), 1, gChangesEveryFrame.View);
		mExtrudeEffect.SetMat4(std::string("cbChangesEveryFrameGS.Projection"), 1, gChangesEveryFrame.Projection);
		mExtrudeEffect.SetFloat(std::string("cbChangesEveryFrameGS.Time"), 1, &gChangesEveryFrame.Time);

		SetFloatArray(vLightDirs, &gConstant.vLightDir[0]);
		mExtrudeEffect.SetVec3(std::string("cbConstantPS.vLightDir"), 1, &gConstant.vLightDir[0]);

		gUserChanges.Explode = gExplode;
		mExtrudeEffect.SetFloat(std::string("cbUserChangesGS.Explode"), 1, (float*)&gUserChanges.Explode);

        mExtrudeEffect.SetTexture(std::string("g_txDiffuse"), 0);
	}
private:
	ShaderEffect mExtrudeEffect;
	Matrix4 gWorld;
	Matrix4 gView;
	Matrix4 gProjection;
	GLuint gIndexBuffer;
	GLuint gVertexBuffer;

	cbConstant gConstant;
	cbChangesEveryFrame gChangesEveryFrame;
	cbUserChanges gUserChanges;

	Model gModel;

	Vector4 vLightDirs;
	float gExplode;
	float gTime;

	GLuint mColTex;
	GLuint mDepthTex;
	GLuint mFramebuffer;
};

Demo gDemo;

void Demo::Display(float t) {

	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);

    gWorld = Matrix4::rotationY(t * 60 * 3.14159f / 180.f);
    
    gWorld *= Matrix4::rotationX(-90.0f * 3.14159f / 180.f);

	gTime = t;
	gExplode = 0.5 * t;

    vLightDirs = Vector4(-0.577f, 0.577f, -0.577f, 1.0f);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

    mExtrudeEffect.Enable();

	gModel.Draw(*this);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, mFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, WindowWidth, WindowHeight,
						 0, 0, WindowWidth, WindowHeight,
						 GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void display(void)
{
    // Update our time
    static float t = 0.0f;

    static uint32_t dwTimeStart = 0;
    uint32_t dwTimeCur = GetTickCount();
    if( dwTimeStart == 0 )
        dwTimeStart = dwTimeCur;
    t = ( dwTimeCur - dwTimeStart ) / 1000.0f;

	gDemo.Display(t);

    glutSwapBuffers();
}

void reshape (int w, int h)
{
	gDemo.ResizeDisplay(w, h);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

void Demo::Init() {

	glGenTextures(1, &mColTex);
	glBindTexture(GL_TEXTURE_2D, mColTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WindowWidth, WindowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glGenTextures(1, &mDepthTex);
	glBindTexture(GL_TEXTURE_2D, mDepthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, WindowWidth, WindowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);

	glGenFramebuffers(1, &mFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTex, 0);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glClearColor(0.0f, 0.125f, 0.3f, 1.0f);

    mExtrudeEffect.Create();
	mExtrudeEffect.AddCompileFlags(HLSLCC_FLAG_GS_ENABLED);
    mExtrudeEffect.FromByteFile(std::string("shaders/ExtrudePS.o"));
    mExtrudeEffect.FromByteFile(std::string("shaders/ExtrudeVS.o"));
	mExtrudeEffect.FromByteFile(std::string("shaders/ExtrudeGS.o"));
    mExtrudeEffect.Link();

    gWorld = Matrix4::identity();

	Point3 Eye( 0.0f, 0.0f, -800.0f );

	Point3 At( 0.0f, 0.0f, 0.0f );
    Vector3 Up( 0.0f, 1.0f, 0.0f );

    gView = Matrix4::lookAt(Eye, At, Up);

    ResizeDisplay(WindowWidth, WindowHeight);

	gModel.Import3DFromFile("models/Tiny.x");
}

void Init(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glewInit();

    glutInitContextVersion (3, 2);
#ifdef _DEBUG
    glutInitContextFlags (GLUT_DEBUG);
#endif
    glutInitWindowSize (WindowWidth, WindowHeight); 
    glutInitWindowPosition(50, 50);
    glutCreateWindow (argv[0]);
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc (keyboard);
    glutIdleFunc(display);

    glewInit();
#ifdef _DEBUG
    SetupOpenGLDebugCallback();
#endif

	gDemo.Init();
}

int main(int argc, char** argv)
{
    Init(argc, argv);
    glutMainLoop();

   return 0;
}
