#if !defined(__PHOTO_ITEM_MANAGER_H__)
#define __PHOTO_ITEM_MANAGER_H__

#include "izSampleKit.h"

class PhotoItem;

/**
 */
class PhotoItemList : public izanagi::CStdList<PhotoItem> {
public:
    PhotoItemList();
    virtual ~PhotoItemList();

    NO_COPIABLE(PhotoItemList);

public:
    void RenderFront(izanagi::graph::CGraphicsDevice* device);

    void RenderTopAndSide(izanagi::graph::CGraphicsDevice* device);
};

/**
 */
class PhotoItemManager {
private:
    static PhotoItemManager s_Instance;

public:
    static PhotoItemManager& Instance();

private:
    PhotoItemManager();
    ~PhotoItemManager();

public:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT itemNum);

private:
    PhotoItemList m_PhotoItemList[3];

    izanagi::shader::CShaderBasic* m_Shader;
};

#endif    // #if !defined(__PHOTO_ITEM_MANAGER_H__)