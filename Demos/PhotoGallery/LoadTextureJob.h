#if !defined(__LOAD_TEXTURE_JOB_H__)
#define __LOAD_TEXTURE_JOB_H__

#include "izSampleKit.h"
#include "izThreadModel.h"

class PhotoItem;

class TextureLoader {
    friend class LoadTextureJob;

private:
    static TextureLoader s_Instance;

public:
    static TextureLoader& Instance();

private:
    TextureLoader();
    ~TextureLoader();

public:
    void Init(izanagi::IMemoryAllocator* allocator);

    void Terminate();

    izanagi::threadmodel::CJob* AddTarget(
        izanagi::graph::CGraphicsDevice* device,
        const char* path,
        PhotoItem* target);

private:
    izanagi::IMemoryAllocator* m_Allocator;
    izanagi::threadmodel::CJobQueue m_JobQueue;

    izanagi::threadmodel::CTheadSafeAllocator<izanagi::CStandardMemoryAllocator> m_JobSafeAllocator;
};

/**
 */
class LoadTextureJob : public izanagi::threadmodel::CJob {
    friend class TextureLoader;

public:
    LoadTextureJob();

    virtual ~LoadTextureJob();

private:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        const char* path,
        PhotoItem* target);

    virtual IZ_BOOL OnRun();

    virtual void OnFinish(IZ_BOOL runResult);

    virtual void OnCancel();

private:
    izanagi::IMemoryAllocator* m_InternalAllocator;
    izanagi::graph::CGraphicsDevice* m_Device;

    IZ_UINT8* m_PixelData;
    IZ_UINT m_TexWidth;
    IZ_UINT m_TexHeight;

    const char* m_Path;
    PhotoItem* m_TargetItem;
};

#endif    // #if !defined(__LOAD_TEXTURE_JOB_H__)