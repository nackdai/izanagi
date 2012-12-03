#if !defined(__IZANAGI_SCENEGRAPH_LIGHT_PARAM_H__)
#define __IZANAGI_SCENEGRAPH_LIGHT_PARAM_H__

#include "izStd.h"
#include "../math/MathVector.h"

namespace izanagi {
	/**
	* 平行光源
	*/
	struct SParallelLightParam {
		math::SVector vDir;
		math::SVector color;
	};

	/**
	* 環境光
	*/
	struct SAmbientLightParam {
		math::SVector color;
	};

	/**
	* 点光源
	*/
	struct SPointLightParam {
		math::SVector vPos;
		math::SVector color;
		math::SVector attn;	// 0 - 2 : attn
	};

	/**
	* スポットライト
	*/
	struct SSpotLightParam {
		math::SVector vPos;
		math::SVector vDir;
		math::SVector color;
		math::SVector param;	// 0 - 2 : attn / 3 : power
	};

	/**
	* 半球ライト
	*/
	struct SHemisphereLightParam {
		math::SVector vAxis;
		math::SVector upColor;
		math::SVector downColor;
	};

	/**
	* マテリアル
	*/
	struct SMaterialParam {
		// Diffuse
		math::SVector vDiffuse;

		// Ambient
		math::SVector vAmbient;

		// Specular
		// NOTE
		// w = Specular Light Power
		// Specular Light Power が大きいほどスペキュラが強くでる
		math::SVector vSpecular;

		// Emissive
		math::SVector vEmissive;

		// Rim
		// NOTE
		// w = Rim Light Power
		// Rim Light Power は０に近いほどリムライトが広がる
		math::SVector vRim;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_LIGHT_PARAM_H__)
