#include "graph/dx9/ShaderConstTable.h"

using namespace uranus;

// �f�X�g���N�^
CShaderConstTable::~CShaderConstTable()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pConstTable);
}
