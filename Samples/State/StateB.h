#if !defined(__STATE_B_H__)
#define __STATE_B_H__

#include "StateBase.h"

class CStateB : public CStateBase {
public:
    CStateB() {}
    virtual ~CStateB() {}

public:
    // 更新.
    virtual IZ_BOOL Update();

    virtual IZ_UINT GetIndex();

    // For Windows

    // キー押下
    virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);
};

#endif    // #if !defined(__STATE_B_H__)
