#if !defined(__STATE_A_H__)
#define __STATE_A_H__

#include "StateBase.h"

class CStateA : public CStateBase {
public:
    CStateA() {}
    virtual ~CStateA() {}

public:
    // �X�V.
    virtual IZ_BOOL Update();

    virtual IZ_UINT GetIndex();

    // For Windows

    // �L�[����
    virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);
};

#endif    // #if !defined(__STATE_A_H__)
