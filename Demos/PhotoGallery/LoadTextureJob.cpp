#include "LoadTextureJob.h"
#include "PhotoItem.h"

#include "jinclude.h"
#include "jpeglib.h"
#include "jerror.h"

// TODO
static const IZ_UINT BUF_SIZE = 16 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

////////////////////////////////////////

TextureLoader TextureLoader::s_Instance;

TextureLoader& TextureLoader::Instance()
{
    return s_Instance;
}

TextureLoader::TextureLoader()
{
    m_Allocator = IZ_NULL;

    m_JobSafeAllocator.Init(BUF, BUF_SIZE);
}

TextureLoader::~TextureLoader()
{
}

void TextureLoader::Init(izanagi::IMemoryAllocator* allocator)
{
    if (m_Allocator == IZ_NULL) {
        m_Allocator = allocator;

        m_JobQueue.Init(m_Allocator, 1);
        m_JobQueue.Start();

        m_Mutex.Open();
    }
}

void TextureLoader::Terminate()
{
    m_JobQueue.Terminate();
    m_Mutex.Close();
}

izanagi::threadmodel::CJob* TextureLoader::EnqueueLoadingRequest(
    izanagi::graph::CGraphicsDevice* device,
    const char* path,
    PhotoItem* target)
{
    IZ_ASSERT(m_Allocator != IZ_NULL);

    // Create loading texture job.
    LoadTextureJob* job = (LoadTextureJob*)izanagi::threadmodel::CJob::CreateJob<LoadTextureJob>(m_Allocator);
    VRETURN_NULL(job != IZ_NULL);

    job->Init(
        &m_JobSafeAllocator,
        device,
        path,
        target);

    m_JobQueue.Enqueue(job, IZ_TRUE);

    return job;
}

izanagi::sys::CMutex& TextureLoader::GetMutex()
{
    return m_Mutex;
}

////////////////////////////////////////

LoadTextureJob::LoadTextureJob()
{
    m_InternalAllocator = IZ_NULL;
    m_Device = IZ_NULL;

    m_PixelData = IZ_NULL;

    m_Path = IZ_NULL;
    m_TargetItem = IZ_NULL;
}

LoadTextureJob::~LoadTextureJob()
{
    SAFE_RELEASE(m_Device);

    FREE(m_InternalAllocator, m_PixelData);
}

IZ_BOOL LoadTextureJob::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    const char* path,
    PhotoItem* target)
{
    m_InternalAllocator = allocator;

    SAFE_REPLACE(m_Device, device);

    m_Path = path;
    m_TargetItem = target;

    return IZ_TRUE;
}

IZ_BOOL LoadTextureJob::OnRun()
{
    // Open file.
    izanagi::CFileInputStream input;
    VRETURN(input.Open(m_Path));

    // Read to memory.
    IZ_UINT8* buf = (IZ_UINT8*)ALLOC(m_InternalAllocator, input.GetSize());
    size_t size = input.GetSize();
    VRETURN(buf != IZ_NULL);
    input.Read(buf, 0, size);
    input.Close();

    // Read jpeg.
    jpeg_decompress_struct cinfo;
    jpeg_error_mgr jerror;
    
    cinfo.err = jpeg_std_error(&jerror);
    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo, buf, size);

    jpeg_read_header(&cinfo, IZ_TRUE);

    jpeg_start_decompress(&cinfo);

    // Alloc buffer to store line data.
    IZ_UINT pitch = cinfo.output_width * cinfo.output_components;
    IZ_UINT8* lineData = (IZ_UINT8*)ALLOC(m_InternalAllocator, pitch + 10);
    VGOTO(lineData != IZ_NULL, __EXIT__);

    // Convert to RGBA8 format.
    const IZ_UINT bpp = izanagi::graph::CGraphUtil::GetBPP(izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
    const IZ_UINT stride = bpp * cinfo.output_width;

    // Alloc buffer to store rgba data.
    IZ_UINT bytes = cinfo.output_width * cinfo.output_height * bpp;
    m_PixelData = (IZ_UINT8*)ALLOC(m_InternalAllocator, bytes);
    VGOTO(m_PixelData != IZ_NULL, __EXIT__);

    IZ_BOOL ret = IZ_TRUE;
    IZ_UINT8* dst = m_PixelData;

    while(cinfo.output_scanline < cinfo.output_height) {
        IZ_UINT8* buffer[1];
        buffer[0] = (IZ_UINT8*)lineData;

        jpeg_read_scanlines(&cinfo, buffer, 1);

        // If this job is canceld...
        if (IsCanceled()) {
            FREE(m_InternalAllocator, m_PixelData);
            ret = IZ_FALSE;
            break;
        }

        // Convert to rgba.
        for (IZ_UINT i = 0; i < cinfo.output_width; i++) {
            dst[i * 4 + 0] = lineData[i * 3 + 2];
            dst[i * 4 + 1] = lineData[i * 3 + 1];
            dst[i * 4 + 2] = lineData[i * 3 + 0];
            dst[i * 4 + 3] = 0xff;
        }

        dst += stride;
    }

    // Keep image size.
    m_TexWidth = cinfo.output_width;
    m_TexHeight = cinfo.output_height;

#if 0
    TextureLoader::Instance().GetMutex().Lock();
    {
        // Create texture from pixel data.
        izanagi::graph::CTexture* texture = m_Device->CreateTexture(
            m_TexWidth, m_TexHeight,
            izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8,
            m_PixelData);

        // Set texture to target.
        IZ_ASSERT(m_TargetItem != IZ_NULL);
        m_TargetItem->SetTexture(texture);
        SAFE_RELEASE(texture);

        // Free memory.
        
    }
    TextureLoader::Instance().GetMutex().Unlock();
#endif

__EXIT__:
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    FREE(m_InternalAllocator, buf);
    FREE(m_InternalAllocator, lineData);

#if 0
    FREE(m_InternalAllocator, m_PixelData);
#endif

    return ret;
}

void LoadTextureJob::OnFinish(IZ_BOOL runResult)
{
#if 1
    if (IsCanceled() || !runResult) {
        // Free memory.
        FREE(m_InternalAllocator, m_PixelData);
        return;
    }
    
    // Create texture from pixel data.
    izanagi::graph::CTexture* texture = m_Device->CreateTexture(
        m_TexWidth, m_TexHeight,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8,
        m_PixelData);

    // Set texture to target.
    IZ_ASSERT(m_TargetItem != IZ_NULL);
    m_TargetItem->SetTexture(texture);
    SAFE_RELEASE(texture);

    // Free memory.
    FREE(m_InternalAllocator, m_PixelData);
#endif
}

void LoadTextureJob::OnCancel()
{
}
