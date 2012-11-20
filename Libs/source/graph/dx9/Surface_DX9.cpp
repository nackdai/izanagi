#include "graph/dx9/Surface_DX9.h"
#include "graph/dx9/Texture_DX9.h"
#include "graph/internal/ParamValueConverter.h"
#include "graph/dx9/GraphicsDevice_DX9.h"

namespace izanagi
{
namespace graph
{
    // サーフェース作成.
    CSurface* CSurfaceDX9::CreateDepthStencilSurface(
	    IMemoryAllocator* allocator,
	    CGraphicsDeviceDX9* device,
	    IZ_UINT width, 
	    IZ_UINT height,
	    E_GRAPH_PIXEL_FMT fmt)
    {
	    VRETURN_NULL(fmt == E_GRAPH_PIXEL_FMT_D24S8);

	    IZ_BOOL result = IZ_FALSE;

	    void* buf = ALLOC_ZERO(allocator, sizeof(CSurfaceDX9));
	    VRETURN_NULL(buf != IZ_NULL);

	    CSurfaceDX9* instance = new(buf) CSurfaceDX9();
	    {
		    instance->m_Allocator = allocator;
		    instance->AddRef();

		    D3D_DEVICE* d3dDevice = device->GetRawInterface();

		    HRESULT hr = d3dDevice->CreateDepthStencilSurface(
						    width, height,
						    CD3D9ParamValueConverter::ConvAbstractToTarget_PixelFormat(fmt),
						    D3DMULTISAMPLE_NONE, 0,
						    TRUE,
						    &instance->m_Surface,
						    NULL);
		    result = SUCCEEDED(hr);
		    if (result) {
			    instance->SetDesc();
		    }
	    }

	    if (!result) {
		    SAFE_RELEASE(instance);
	    }
	    return instance;
    }

    CSurfaceDX9* CSurfaceDX9::CreateSurface(IMemoryAllocator* allocator)
    {
	    IZ_BOOL result = IZ_TRUE;
	    IZ_UINT8* buf = IZ_NULL;
	    CSurfaceDX9* instance = IZ_NULL;

	    // メモリ確保
	    buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CSurfaceDX9));
	    if (!(result = (buf != IZ_NULL))) {
		    IZ_ASSERT(IZ_FALSE);
		    goto __EXIT__;
	    }

	    // インスタンス作成
	    instance = new (buf)CSurfaceDX9;
	    {
		    instance->m_Allocator = allocator;
		    instance->AddRef();
	    }

    __EXIT__:
	    if (!result) {
		    if (instance != IZ_NULL) {
			    SAFE_RELEASE(instance);
		    }
		    else if (buf != IZ_NULL) {
			    allocator->Free(buf);
		    }
	    }

	    return instance;
    }

    // コンストラクタ
    CSurfaceDX9::CSurfaceDX9()
    {
	    m_Surface = IZ_NULL;
	    FILL_ZERO(&m_Desc, sizeof(m_Desc));
    }

    // デストラクタ
    CSurfaceDX9::~CSurfaceDX9()
    {
	    SAFE_RELEASE(m_Surface);
    }

    // リセット
    IZ_BOOL CSurfaceDX9::Reset(
	    CTexture* texture,
	    IZ_UINT level)
    {
	    IZ_BOOL ret = IZ_TRUE;

	    SAFE_RELEASE(m_Surface);

	    if (texture != IZ_NULL) {
		    IZ_ASSERT(level < texture->GetMipMapNum());

            CTextureDX9* dx9Tex = reinterpret_cast<CTextureDX9*>(texture);
		    D3D_TEXTURE* pD3DTex = dx9Tex->GetRawInterface();

		    // TODO
		    // 最上位レベルのみ
		    HRESULT hr = pD3DTex->GetSurfaceLevel(level, &m_Surface);

		    IZ_BOOL ret = SUCCEEDED(hr);
		    IZ_ASSERT(ret);
	    }

#if 0
	    if (ret && (m_Surface != IZ_NULL)) {
		    // 記述取得
		    HRESULT hr = m_Surface->GetDesc(&m_Desc);
		    ret = SUCCEEDED(hr);
		    IZ_ASSERT(ret);
	    }
#else
	    if (ret && (m_Surface != IZ_NULL)) {
		    ret = SetDesc();
		    IZ_ASSERT(ret);
	    }
#endif

	    return ret;
    }

    // うーん・・・
    IZ_BOOL CSurfaceDX9::SetDesc()
    {
	    IZ_BOOL ret = IZ_FALSE;
	    if (m_Surface != IZ_NULL) {
		    HRESULT hr = m_Surface->GetDesc(&m_Desc);
		    ret = SUCCEEDED(hr);

		    if (ret) {
			    m_Fmt = IZ_GET_ABST_PIXEL_FMT(m_Desc.Format);
		    }
	    }

	    return ret;
    }

    void CSurfaceDX9::ReleaseResource()
	{
		SAFE_RELEASE(m_Surface);
	}
}   // namespace graph
}   // namespace izanagi