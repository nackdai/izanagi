#if !defined(__STATE_C_H__)
#define __STATE_C_H__

#include "StateBase.h"

class CStateC : public CStateBase {
public:
    CStateC() {}
    virtual ~CStateC() {}

public:
    // 更新.
    virtual IZ_BOOL Update();

    virtual IZ_UINT GetIndex();

    // For Windows

    // キー押下
    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);
};

#endif    // #if !defined(__STATE_C_H__)
