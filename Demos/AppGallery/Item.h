#if !defined(__ITEM_H__)
#define __ITEM_H__

#include "izSampleKit.h"

class ItemBox;
class ItemBoard;

class Item : public izanagi::CObject {
public:
    static Item* Create(
        izanagi::IMemoryAllocator* allocator,
        ItemBox* box,
        ItemBoard* board);

private:
    Item();
    virtual ~Item();

    IZ_DEFINE_INTERNAL_RELEASE();

public:
    void SetPosAndRotate(IZ_FLOAT theta);

    void Render(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::shader::CShaderBasic* shader);

    void RenderBoard(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::shader::CShaderBasic* shader);

    izanagi::CStdList<Item>::Item* GetListItem();

    const izanagi::math::SMatrix44& GetL2W();

    void SetTexture(izanagi::graph::CTexture* texture);

    void SetIsRequestedLoadTexture(IZ_BOOL flag);

    IZ_BOOL IsRequestedLoadTexture() const;

private:
    izanagi::IMemoryAllocator* m_Allocator;

    ItemBox* m_Box;

    izanagi::math::SMatrix44 m_BoxL2W;
    izanagi::math::SMatrix44 m_BoardL2W;

    izanagi::CStdList<Item>::Item m_ListItem;

    ItemBoard* m_Board;
    izanagi::graph::CTexture* m_Tex;

    IZ_BOOL m_IsRequestedLoadTexture;
};

#endif    // #if !defined(__ITEM_H__)
