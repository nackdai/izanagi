#if !defined(__STATE_POST_EFFCT_H__)
#define __STATE_POST_EFFCT_H__

#include "izSceneGraph.h"
#include "StateBase.h"

class CStatePostEffect : public CStateBase {
public:
    CStatePostEffect(
        izanagi::sample::CSampleApp* app,
        izanagi::SCameraParam& camera);
    virtual ~CStatePostEffect();

public:
    // 初期化.
    virtual IZ_BOOL Init();

    // 更新.
    virtual IZ_BOOL Update();

    // 描画.
    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

    // 終了.
    virtual IZ_BOOL Destroy();

    // ステートから抜ける（終了）.
    virtual IZ_BOOL Leave();

    // キー押下
    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);
};

#endif    // #if !defined(__STATE_POST_EFFCT_H__)
