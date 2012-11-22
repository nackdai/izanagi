#if !defined(__NORMAL_MAP_SHADER_H__)
#define __NORMAL_MAP_SHADER_H__

#include "izSceneGraph.h"
#include "PhongShader.h"

// 法線マップシェーダ
class CNormalMapShader : public CPhongShader {
	friend class CShaderBase;

protected:
	CNormalMapShader() {}
	~CNormalMapShader() {}

	CNormalMapShader(const CNormalMapShader& rhs);
	const CNormalMapShader& operator=(const CNormalMapShader& rhs);
};

#endif	// #if !defined(__NORMAL_MAP_SHADER_H__)
