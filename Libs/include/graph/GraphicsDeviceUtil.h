#if !defined(__URANUS_GRAPH_GRAPHICS_DEVICE_UTIL_H__)
#define __URANUS_GRAPH_GRAPHICS_DEVICE_UTIL_H__

#include "GraphicsDeviceProxy.h"

namespace uranus {
	/**
	*/
	class CGraphicsDeviceUtil {
	private:
		CGraphicsDeviceUtil();
		~CGraphicsDeviceUtil();

		CGraphicsDeviceUtil(const CGraphicsDeviceUtil& rhs);
		const CGraphicsDeviceUtil& operator=(const CGraphicsDeviceUtil& rhs);

	public:
		// �����_�[�X�e�[�g

		// �[�x
		static inline void EnableZWrite(CGraphicsDevice* pDevice, UN_DWORD flag);			// �[�x�l�`�����ݗL���E����
		static inline void EnableZTest(CGraphicsDevice* pDevice, UN_DWORD flag);			// �[�x�e�X�g�L���E����
		static inline void SetZTestFunc(CGraphicsDevice* pDevice, UN_DWORD func);			// �[�x�e�X�g������@

		// �A���t�@
		static inline void EnableAlphaTest(CGraphicsDevice* pDevice, UN_DWORD flag);			// �A���t�@�e�X�g�L���E����
		static inline void SetAlphaTestRef(CGraphicsDevice* pDevice, UN_DWORD ref);			// �A���t�@�e�X�g��l
		static inline void SetAlphaTestFunc(CGraphicsDevice* pDevice, UN_DWORD func);			// �A���t�@�e�X�g������@
		static inline void EnableAlphaBlend(CGraphicsDevice* pDevice, UN_DWORD flag);			// �A���t�@�u�����h�L���E����
		static inline void SetAlphaBlendMethod(CGraphicsDevice* pDevice, UN_DWORD method);	// �A���t�@�u�����h���@

		// �`�惂�[�h
		static inline void SetFillMode(CGraphicsDevice* pDevice, UN_DWORD fill);			// �t�B�����[�h
		static inline void SetCullMode(CGraphicsDevice* pDevice, UN_DWORD cull);			// �J�����O���[�h

		// �����_�[�^�[�Q�b�g�̃J���[�o�b�t�@�̕`�����ݐݒ�
		static inline void EnableRenderColorRGB(CGraphicsDevice* pDevice, UN_DWORD enableRGB);
		static inline void EnableRenderColorA(CGraphicsDevice* pDevice, UN_DWORD enableA);

		// �V�U�[�e�X�g
		static inline void EnableScissorTest(CGraphicsDevice* pDevice, UN_DWORD flag);
	};

	/**
	* �[�x�l�`�����ݗL���E����
	*/
	void CGraphicsDeviceUtil::EnableZWrite(CGraphicsDevice* pDevice, UN_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ZWRITEENABLE, flag);
	}

	/**
	* �[�x�e�X�g�L���E����
	*/
	void CGraphicsDeviceUtil::EnableZTest(CGraphicsDevice* pDevice, UN_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ZENABLE, flag);
	}

	/**
	* �[�x�e�X�g������@
	*/
	void CGraphicsDeviceUtil::SetZTestFunc(CGraphicsDevice* pDevice, UN_DWORD func)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ZFUNC, func);
	}
	
	/**
	* �A���t�@�e�X�g�L���E����
	*/
	void CGraphicsDeviceUtil::EnableAlphaTest(CGraphicsDevice* pDevice, UN_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ALPHATESTENABLE, flag);
	}

	/**
	* �A���t�@�e�X�g��l
	*/
	void CGraphicsDeviceUtil::SetAlphaTestRef(CGraphicsDevice* pDevice, UN_DWORD ref)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ALPHAREF, ref);
	}

	/**
	* �A���t�@�e�X�g������@
	*/
	void CGraphicsDeviceUtil::SetAlphaTestFunc(CGraphicsDevice* pDevice, UN_DWORD func)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ALPHAFUNC, func);
	}

	/**
	* �A���t�@�u�����h�L���E����
	*/
	void CGraphicsDeviceUtil::EnableAlphaBlend(CGraphicsDevice* pDevice, UN_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ALPHABLENDENABLE, flag);
	}

	/**
	* �A���t�@�u�����h���@
	*/
	void CGraphicsDeviceUtil::SetAlphaBlendMethod(CGraphicsDevice* pDevice, UN_DWORD method)
	{
		pDevice->SetRenderState(E_GRAPH_RS_BLENDMETHOD, method);
	}

	/**
	* �t�B�����[�h
	*/
	void CGraphicsDeviceUtil::SetFillMode(CGraphicsDevice* pDevice, UN_DWORD fill)
	{
		pDevice->SetRenderState(E_GRAPH_RS_FILLMODE, fill);
	}

	/**
	* �J�����O���[�h
	*/
	void CGraphicsDeviceUtil::SetCullMode(CGraphicsDevice* pDevice, UN_DWORD cull)
	{
		pDevice->SetRenderState(E_GRAPH_RS_CULLMODE, cull);
	}

	/**
	* �����_�[�^�[�Q�b�g�̃J���[�o�b�t�@�̕`�����ݐݒ�
	*/
	void CGraphicsDeviceUtil::EnableRenderColorRGB(CGraphicsDevice* pDevice, UN_DWORD enableRGB)
	{
		pDevice->SetRenderState(E_GRAPH_RS_COLORWRITEENABLE_RGB, enableRGB);
	}

	/**
	* �����_�[�^�[�Q�b�g�̃J���[�o�b�t�@�̕`�����ݐݒ�
	*/
	void CGraphicsDeviceUtil::EnableRenderColorA(CGraphicsDevice* pDevice, UN_DWORD enableA)
	{
		pDevice->SetRenderState(E_GRAPH_RS_COLORWRITEENABLE_A, enableA);
	}

	/**
	* �V�U�[�e�X�g
	*/
	void CGraphicsDeviceUtil::EnableScissorTest(CGraphicsDevice* pDevice, UN_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_SCISSORTESTENABLE, flag);
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_GRAPHICS_DEVICE_UTIL_H__)
