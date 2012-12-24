#if !defined(__STATE_A_H__)
#define __STATE_A_H__

#include "StateBase.h"

class CStateA : public CStateBase {
public:
    CStateA() {}
    virtual ~CStateA() {}

public:
    // 更新.
    virtual IZ_BOOL Update();

    virtual IZ_UINT GetIndex();

    // For Windows

    // キー押下
    virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);
};

#endif    // #if !defined(__STATE_A_H__)
