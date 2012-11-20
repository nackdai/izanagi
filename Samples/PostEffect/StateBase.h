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

protected:
	void RenderName(
		izanagi::graph::CGraphicsDevice* device,
		const char* name);

protected:
	izanagi::sample::CSampleApp* m_App;
	izanagi::SCameraParam& m_Camera;
};

#endif	// #if !defined(__STATE_BASE_H__)
