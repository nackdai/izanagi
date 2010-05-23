#include "graph/dx9/RenderState.h"
#include "graph/dx9/GraphicsDevice.h"

using namespace uranus;

namespace {
	// �����_�[�X�e�[�g�ݒ�
	template <typename _RS, typename _T>
	void _SetRenderState(
		CGraphicsDevice* pDevice,
		_RS nRSType,
		_T& old_val, _T new_val)
	{
		UN_ASSERT(pDevice != UN_NULL);

		D3D_DEVICE* pD3DDev = pDevice->GetRawInterface();

		if (old_val != new_val) {
			pD3DDev->SetRenderState(nRSType, new_val);
			old_val = new_val;
		}
	}
}	// namespace

// �R���X�g���N�^
CRenderState::CRenderState()
{
	m_bIsSavedRS = UN_FALSE;
}

// �f�X�g���N�^
CRenderState::~CRenderState()
{
}

// �����_�[�X�e�[�g���Z�b�g
void CRenderState::SetRenderState(
	CGraphicsDevice* pDevice,
	E_GRAPH_RENDER_STATE nState,
	UN_DWORD val)
{
	UN_ASSERT(nState < E_GRAPH_RS_NUM);

	typedef void (CRenderState::*FUNC)(CGraphicsDevice*, UN_DWORD);
	static FUNC func[] = {
		&CRenderState::EnableZWrite,			// E_GRAPH_RS_ZWRITEENABLE
		&CRenderState::EnableZTest,				// E_GRAPH_RS_ZENABLE
		&CRenderState::SetZTestFunc,			// E_GRAPH_RS_ZFUNC

		&CRenderState::EnableAlphaTest,			// E_GRAPH_RS_ALPHATESTENABLE
		&CRenderState::SetAlphaTestRef,			// E_GRAPH_RS_ALPHAREF
		&CRenderState::SetAlphaTestFunc,		// E_GRAPH_RS_ALPHAFUNC
		&CRenderState::EnableAlphaBlend,		// E_GRAPH_RS_ALPHABLENDENABLE
		&CRenderState::SetAlphaBlendMethod,		// E_GRAPH_RS_BLENDMETHOD

		&CRenderState::SetFillMode,				// E_GRAPH_RS_FILLMODE
		&CRenderState::SetCullMode,				// E_GRAPH_RS_CULLMODE

		&CRenderState::EnableRenderColorRGB,	// E_GRAPH_RS_COLORWRITEENABLE_RGB
		&CRenderState::EnableRenderColorA,		// E_GRAPH_RS_COLORWRITEENABLE_A
		
		&CRenderState::EnableScissorTest,		// E_GRAPH_RS_SCISSORTESTENABLE
	};
	UN_C_ASSERT(COUNTOF(func) == E_GRAPH_RS_NUM);

	(this->*func[nState])(pDevice, val);
}

// ���݂̃����_�[�X�e�[�g��ۑ�
UN_BOOL CRenderState::Save()
{
	// �ێ����łȂ�����
	UN_ASSERT(!m_bIsSavedRS);

	UN_BOOL ret = UN_FALSE;

	if (!m_bIsSavedRS) {
		memcpy(&m_SaveRS.dwRS, this->dwRS, sizeof(m_SaveRS.dwRS));

		m_bIsSavedRS = UN_TRUE;
		ret = UN_TRUE;
	}

	return ret;
}

// �ۑ����������_�[�X�e�[�g�����ɖ߂�
UN_BOOL CRenderState::Load(CGraphicsDevice* pDevice)
{
	// �ێ����ł��邱��
	UN_ASSERT(m_bIsSavedRS);

	UN_BOOL ret = UN_FALSE;

	if (m_bIsSavedRS) {
		for (UN_UINT i = 0; i < E_GRAPH_RS_NUM; i++) {
			SetRenderState(
				pDevice,
				static_cast<E_GRAPH_RENDER_STATE>(i),
				m_SaveRS.dwRS[i]);
		}

		m_bIsSavedRS = UN_FALSE;
		ret = UN_TRUE;
	}

	return ret;
}

#define _CHECK(f)	{ HRESULT hr = f; UN_ASSERT(SUCCEEDED(hr)); }

// �O���t�B�b�N�X�f�o�C�X���猻�ݐݒ肳��Ă���l���擾����
void CRenderState::GetParamsFromGraphicsDevice(CGraphicsDevice* pDevice)
{
	D3D_DEVICE* pD3DDev = pDevice->GetRawInterface();

	// �[�x
	_CHECK(pD3DDev->GetRenderState(D3DRS_ZWRITEENABLE, (DWORD*)&isZWriteEnable));
	_CHECK(pD3DDev->GetRenderState(D3DRS_ZENABLE,      (DWORD*)&isZTestEnable));
	_CHECK(pD3DDev->GetRenderState(D3DRS_ZFUNC,        (DWORD*)&cmpZFunc));
	
	// �A���t�@�e�X�g
	_CHECK(pD3DDev->GetRenderState(D3DRS_ALPHATESTENABLE,  (DWORD*)&isAlphaTestEnable));
	_CHECK(pD3DDev->GetRenderState(D3DRS_ALPHAREF,         (DWORD*)&alphaRef));
	_CHECK(pD3DDev->GetRenderState(D3DRS_ALPHAFUNC,        (DWORD*)&cmpAlphaFunc));

	// �A���t�@�u�����h
	_CHECK(pD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, (DWORD*)&isAlphaBlendEnable));

	// �u�����h���@
	UN_DWORD nOp, nSrc, nDst;
	_CHECK(pD3DDev->GetRenderState(D3DRS_BLENDOP,   (DWORD*)&nOp));
	_CHECK(pD3DDev->GetRenderState(D3DRS_SRCBLEND,  (DWORD*)&nSrc));
	_CHECK(pD3DDev->GetRenderState(D3DRS_DESTBLEND, (DWORD*)&nDst));
	methodAlphaBlend = UN_GRAPH_ALPHA_BLEND_VAL(nOp, nSrc, nDst);

	// �`�惂�[�h
	_CHECK(pD3DDev->GetRenderState(D3DRS_FILLMODE, (DWORD*)&fillMode));
	_CHECK(pD3DDev->GetRenderState(D3DRS_CULLMODE, (DWORD*)&cullMode));

	// �V�U�[��`
	_CHECK(pD3DDev->GetRenderState(D3DRS_SCISSORTESTENABLE, (DWORD*)&isScissorEnable));
	_CHECK(pD3DDev->GetScissorRect((RECT*)&rcScissor));

	// �J���[�`��L���E����
	UN_DWORD nColorWriteFlag;
	_CHECK(pD3DDev->GetRenderState(D3DRS_COLORWRITEENABLE, (DWORD*)&nColorWriteFlag));
	isEnableRenderRGB = (nColorWriteFlag & (D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED));
	isEnableRenderA = (nColorWriteFlag & D3DCOLORWRITEENABLE_ALPHA);
}

// �[�x�l�`�����ݗL���E����
void CRenderState::EnableZWrite(CGraphicsDevice* pDevice, UN_DWORD flag)
{
	_SetRenderState(
		pDevice,
		D3DRS_ZWRITEENABLE,
		isZWriteEnable, flag);
}

// �[�x�e�X�g�L���E����
void CRenderState::EnableZTest(CGraphicsDevice* pDevice, UN_DWORD flag)
{
	_SetRenderState(
		pDevice,
		D3DRS_ZENABLE,
		isZTestEnable, flag);
}

// �[�x�e�X�g������@
void CRenderState::SetZTestFunc(CGraphicsDevice* pDevice, UN_DWORD func)
{
	_SetRenderState(
		pDevice,
		D3DRS_ZFUNC,
		cmpZFunc, func);
}

// �A���t�@�e�X�g�L���E����
void CRenderState::EnableAlphaTest(CGraphicsDevice* pDevice, UN_DWORD flag)
{
	_SetRenderState(
		pDevice,
		D3DRS_ALPHATESTENABLE,
		isAlphaTestEnable, flag);
}

// �A���t�@�e�X�g��l
void CRenderState::SetAlphaTestRef(CGraphicsDevice* pDevice, UN_DWORD ref)
{
	_SetRenderState(
		pDevice,
		D3DRS_ALPHAREF,
		alphaRef, ref);
}

// �A���t�@�e�X�g������@
void CRenderState::SetAlphaTestFunc(CGraphicsDevice* pDevice, UN_DWORD func)
{
	_SetRenderState(
		pDevice,
		D3DRS_ALPHAFUNC,
		cmpAlphaFunc, func);
}

// �A���t�@�u�����h�L���E����
void CRenderState::EnableAlphaBlend(CGraphicsDevice* pDevice, UN_DWORD flag)
{
	_SetRenderState(
		pDevice,
		D3DRS_ALPHABLENDENABLE ,
		isAlphaBlendEnable, flag);
}

// �A���t�@�u�����h���@
void CRenderState::SetAlphaBlendMethod(CGraphicsDevice* pDevice, UN_DWORD method)
{
	// �V�����ݒ�l
	UN_DWORD nNewOp = UN_GRAPH_GET_ALPHA_BLEND_OP(method);
	UN_DWORD nNewSrc = UN_GRAPH_GET_ALPHA_BLEND_SRC(method);
	UN_DWORD nNewDst = UN_GRAPH_GET_ALPHA_BLEND_DST(method);

	// ���݂̐ݒ�l
	UN_DWORD nCurOp = UN_GRAPH_GET_ALPHA_BLEND_OP(methodAlphaBlend);
	UN_DWORD nCurSrc = UN_GRAPH_GET_ALPHA_BLEND_SRC(methodAlphaBlend);
	UN_DWORD nCurDst = UN_GRAPH_GET_ALPHA_BLEND_DST(methodAlphaBlend);

	_SetRenderState(
		pDevice,
		D3DRS_BLENDOP,
		nCurOp, nNewOp);
	_SetRenderState(
		pDevice,
		D3DRS_SRCBLEND,
		nCurSrc, nNewSrc);
	_SetRenderState(
		pDevice,
		D3DRS_DESTBLEND,
		nCurDst, nNewDst);

	// �X�V
	methodAlphaBlend = UN_GRAPH_ALPHA_BLEND_VAL(nNewOp, nNewSrc, nNewDst);
}

// �t�B�����[�h
void CRenderState::SetFillMode(CGraphicsDevice* pDevice, UN_DWORD fill)
{
	_SetRenderState(
		pDevice,
		D3DRS_FILLMODE,
		fillMode, fill);
}

// �J�����O���[�h
void CRenderState::SetCullMode(CGraphicsDevice* pDevice, UN_DWORD cull)
{
	_SetRenderState(
		pDevice,
		D3DRS_CULLMODE,
		cullMode, cull);
}

#define GET_RGB_FLAG(b)	(b ? (D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED) : 0)
#define GET_A_FLAG(b)	(b ? D3DCOLORWRITEENABLE_ALPHA : 0)

// �����_�[�^�[�Q�b�g�̃J���[�o�b�t�@�̕`�����ݐݒ�
void CRenderState::EnableRenderColorRGB(CGraphicsDevice* pDevice, UN_DWORD enableRGB)
{
	// �V�����ݒ�l
	UN_DWORD nNewFlag = GET_RGB_FLAG(enableRGB);
	nNewFlag |= GET_A_FLAG(isEnableRenderA);

	// ���݂̐ݒ�l
	UN_DWORD nCurFlag = GET_RGB_FLAG(isEnableRenderRGB);
	nCurFlag |= GET_A_FLAG(isEnableRenderA);

	_SetRenderState(
		pDevice,
		D3DRS_COLORWRITEENABLE,
		nCurFlag, nNewFlag);

	// �X�V
	isEnableRenderRGB = enableRGB;
}

void CRenderState::EnableRenderColorA(CGraphicsDevice* pDevice, UN_DWORD enableA)
{
	// �V�����ݒ�l
	UN_DWORD nNewFlag = GET_RGB_FLAG(isEnableRenderRGB);
	nNewFlag |= GET_A_FLAG(enableA);

	// ���݂̐ݒ�l
	UN_DWORD nCurFlag = GET_RGB_FLAG(isEnableRenderRGB);
	nCurFlag |= GET_A_FLAG(enableA);

	_SetRenderState(
		pDevice,
		D3DRS_COLORWRITEENABLE,
		nCurFlag, nNewFlag);

	// �X�V
	isEnableRenderA = enableA;
}

// �V�U�[�e�X�g
void CRenderState::EnableScissorTest(CGraphicsDevice* pDevice, UN_DWORD flag)
{
	_SetRenderState(
		pDevice,
		D3DRS_SCISSORTESTENABLE,
		isScissorEnable, flag);
}
