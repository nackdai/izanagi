#include "StateTest1.h"
#include "MySystem.h"
#include "izResource.h"
#include "izSceneGraph.h"
#include "izFont.h"

#include "../../data/test_1.h"

CStateTest1::CStateTest1()
{
}

CStateTest1::~CStateTest1()
{
}

IZ_BOOL CStateTest1::Create()
{
	return IZ_TRUE;
}

IZ_BOOL CStateTest1::Render()
{
	if (CMySystem::GetInstance().GetGraphicsDevice()->Begin2D()) {
		// Image
		izanagi::CImage* pImage = m_pArchProxy->Find<izanagi::CImage>(test_1::a::Image_img);
		CMySystem::GetInstance().GetGraphicsDevice()->SetTexture(0, pImage->GetTexture(0));
		CMySystem::GetInstance().GetGraphicsDevice()->Set2DRenderOp(izanagi::E_GRAPH_2D_RENDER_OP_MODULATE);
		CMySystem::GetInstance().GetGraphicsDevice()->Draw2DSprite(
			izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
			izanagi::CIntRect(300, 100, 556, 228));

		// Font.
		izanagi::CFontRenderer* pFntRenderer = m_pArchProxy->Find<izanagi::CFontRenderer>(test_1::b::c::test_fnt);
		pFntRenderer->Register("あいうえおかきくけこabcdefghijklmnopqrstuvwxyz");
		pFntRenderer->Render(
			"あいうえおかきくけこabcdefghijklmnopqrstuvwxyz",
			0, 200,
			izanagi::CColor::GREEN);

		CMySystem::GetInstance().GetGraphicsDevice()->End2D();
	}

	return IZ_TRUE;
}

IZ_BOOL CStateTest1::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CStateTest1::Destroy()
{
	return IZ_TRUE;
}

namespace {
	class CImgCreateListener : public izanagi::CCreateRscListener {
	public:
		izanagi::CObject* CreateRsc(izanagi::IMemoryAllocator* pAllocator, izanagi::CGraphicsDevice* pDevice, izanagi::IInputStream* pIn)
		{
			return izanagi::CImage::CreateImage(pAllocator, pDevice, pIn);
		}
	};

	class CFntCreateListener : public izanagi::CCreateRscListener {
	public:
		izanagi::CObject* CreateRsc(izanagi::IMemoryAllocator* pAllocator, izanagi::CGraphicsDevice* pDevice, izanagi::IInputStream* pIn)
		{
			return izanagi::CFontRenderer::CreateFontRendererBmp(pAllocator, pDevice, 128, pIn);
		}
	};
}	// namespace

IZ_BOOL CStateTest1::Enter()
{
	VRETURN(m_Input.Open("data/test_1.arc"));

	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();

	izanagi::CArchive* pArchive = izanagi::CArchiveImpl::CreateArchive(
									pAllocator,
									&m_Input);
	IZ_ASSERT(pArchive != IZ_NULL);

	m_pArchProxy = izanagi::CArchiveProxy::CreateArchiveProxy(pAllocator, pArchive);
	IZ_ASSERT(m_pArchProxy != IZ_NULL);

	CImgCreateListener cImgCreateListener;
	CFntCreateListener cFntCreateListener;

	m_pArchProxy->AddCreateRscListener(
		izanagi::CRscType("img"),
		&cImgCreateListener);
	m_pArchProxy->AddCreateRscListener(
		izanagi::CRscType("fnt"),
		&cFntCreateListener);

	m_pArchProxy->CreateResource<izanagi::CImage>(
		test_1::a::Image_img,
		pAllocator,
		pDevice);

	m_pArchProxy->CreateResource<izanagi::CFontRenderer>(
		test_1::b::c::test_fnt,
		pAllocator,
		pDevice);

	SAFE_RELEASE(pArchive);

	return IZ_TRUE;
}

IZ_BOOL CStateTest1::Leave()
{
	SAFE_RELEASE(m_pArchProxy);
	return IZ_TRUE;
}
