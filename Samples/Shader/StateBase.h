#if !defined(__STATE_BASE_H__)
#define __STATE_BASE_H__

#include "izSceneGraph.h"
#include "izDebugUtil.h"

namespace izanagi {
	namespace sample {
		class CSampleApp;
	}
}

class CStateBase : public izanagi::CSceneStateBase {
public:
	CStateBase(
		izanagi::sample::CSampleApp* app,
		izanagi::SCameraParam& camera);
	virtual ~CStateBase() {}

public:
    // ������.
	virtual IZ_BOOL Init();

	// �X�V.
	virtual IZ_BOOL Update();

    // �I��.
	virtual IZ_BOOL Destroy();

    // �L�[����
    virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);

protected:
	void RenderName(
		izanagi::CGraphicsDevice* device,
		const char* name);

protected:
	izanagi::sample::CSampleApp* m_App;
	izanagi::SCameraParam& m_Camera;
};

#endif	// #if !defined(__STATE_BASE_H__)
