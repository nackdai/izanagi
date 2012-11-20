#include "StateBase.h"
#include "izSampleKit.h"

CStateBase::CStateBase(
	izanagi::sample::CSampleApp* app,
	izanagi::SCameraParam& camera)
: m_App(app), 
  m_Camera(camera)
{
}

void CStateBase::RenderName(
	izanagi::graph::CGraphicsDevice* device,
	const char* name)
{
	izanagi::CDebugFont* debugFont = m_App->GetDebugFont();

	if (device->Begin2D()) {
		debugFont->Begin(0, izanagi::CDebugFont::FONT_SIZE * 2);

		debugFont->DBPrint(
			"%s\n",
			name);

		debugFont->End();

		device->End2D();
	}
}
