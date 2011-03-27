#include "graph/dx9/ShaderConstTable.h"

using namespace izanagi;

// デストラクタ
CShaderConstTable::~CShaderConstTable()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pConstTable);
}
