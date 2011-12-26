#if !defined(__PARALLAX_SHADER_H__)
#define __PARALLAX_SHADER_H__

#include "izSceneGraph.h"
#include "PhongShader.h"

// 視差シェーダ
class CParallaxShader : public CPhongShader {
	friend class CShaderBase;

protected:
	CParallaxShader() {}
	~CParallaxShader() {}

	CParallaxShader(const CParallaxShader& rhs);
	const CParallaxShader& operator=(const CParallaxShader& rhs);
};

#endif	// #if !defined(__PARALLAX_SHADER_H__)
