#if !defined(__STATE_XFILE_H__)
#define __STATE_XFILE_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "StateBase.h"

class CStateXFile : public CStateBase {
public:
    CStateXFile(izanagi::sample::CSampleApp* app);
    virtual ~CStateXFile();

public:
    // 開始
    virtual IZ_BOOL Enter(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::CValue& arg);

    // ステートから抜ける（終了）.
    virtual IZ_BOOL Leave();

    virtual IZ_UINT GetTexIdx() const { return 1; }
    virtual IZ_UINT GetShaderPassIdx() const { return 1; }

    virtual IZ_FLOAT GetElapesedTime();
};

#endif    // #if !defined(__STATE_XFILE_H__)
