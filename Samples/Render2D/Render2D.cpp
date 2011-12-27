#include "izSampleKit.h"

class CRender2DApp : public izanagi::sample::CSampleApp {
public:
	CRender2DApp();
	virtual ~CRender2DApp();

protected:
	// ������.
	virtual IZ_BOOL InitInternal(
		izanagi::IMemoryAllocator* allocator,
		izanagi::CGraphicsDevice* device,
		izanagi::sample::CSampleCamera& camera);

	// ���.
	virtual void ReleaseInternal();

	// �X�V.
	virtual void UpdateInternal(izanagi::CCamera& camera);

	// �`��.
	virtual void RenderInternal(izanagi::CGraphicsDevice* device);

private:
	izanagi::CImage* m_Img;
};

CRender2DApp::CRender2DApp()
{
	m_Img = IZ_NULL;
}

CRender2DApp::~CRender2DApp()
{
}

// ������.
IZ_BOOL CRender2DApp::InitInternal(
	izanagi::IMemoryAllocator* allocator,
	izanagi::CGraphicsDevice* device,
	izanagi::sample::CSampleCamera& camera)
{
	izanagi::CFileInputStream in;
	VRETURN(in.Open("data/Image.img"));

	m_Img = izanagi::CImage::CreateImage(
				allocator,
				device,
				&in);

	return (m_Img != IZ_NULL);
}

// ���.
void CRender2DApp::ReleaseInternal()
{
	SAFE_RELEASE(m_Img);
}

// �X�V.
void CRender2DApp::UpdateInternal(izanagi::CCamera& camera)
{
	// Nothing is done...
}

// �`��.
void CRender2DApp::RenderInternal(izanagi::CGraphicsDevice* device)
{
	static const IZ_COLOR bgColor = IZ_COLOR_RGBA(0, 128, 255, 255);

	device->BeginRender(
		izanagi::E_GRAPH_CLEAR_FLAG_ALL,
		bgColor, 1.0f, 0);
	{
		if (device->Begin2D()) {
			// �X�v���C�g
			device->SetTexture(0, m_Img->GetTexture(0));
			device->Set2DRenderOp(izanagi::E_GRAPH_2D_RENDER_OP_MODULATE);
			device->Draw2DSprite(
				izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
				izanagi::CIntRect(300, 100, 556, 228));

			// �h��Ԃ���`
			device->Draw2DRect(
				izanagi::CIntRect(100, 100, 200, 200),
				IZ_COLOR_RGBA(0, 0xff, 0, 0xff));

			// ���C��
			device->Draw2DLine(
				izanagi::CIntPoint(100, 100),	// �n�_
				izanagi::CIntPoint(200, 200),	// �I�_
				IZ_COLOR_RGBA(0xff, 0, 0, 0xff));

			device->End2D();
		}
	}
	device->EndRender();
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1024;
static const IZ_UINT SCREEN_HEIGHT = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CRender2DApp app;
	izanagi::CStandardMemoryAllocator allocator(BUF_SIZE, BUF);

	izanagi::sample::SSampleParam sampleParam = {
		&app,
		&allocator,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		IZ_TRUE,
		"Render2D",
		hInstance,
		GFX_BUF_SIZE,
		GFX_BUF
	};

	IZ_BOOL result = SampleMainLoop(sampleParam);

	int ret = (result ? 0 : 1);

	return ret;
}
