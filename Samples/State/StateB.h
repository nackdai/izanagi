#if !defined(__STATE_B_H__)
#define __STATE_B_H__

#include "StateBase.h"

class CStateB : public CStateBase {
public:
    CStateB() {}
    virtual ~CStateB() {}

public:
    // �X�V.
    virtual IZ_BOOL Update(
        IZ_FLOAT time,
        izanagi::graph::CGraphicsDevice* device);

    virtual IZ_UINT GetIndex();

    // For Windows

    // �L�[����
    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);
};

#endif    // #if !defined(__STATE_B_H__)
