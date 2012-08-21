#if !defined(__IZANAGI_SCENEGRAPH_LIGHT_PARAM_H__)
#define __IZANAGI_SCENEGRAPH_LIGHT_PARAM_H__

#include "izStd.h"
#include "../math/MathVector.h"

namespace izanagi {
	/**
	* 平行光源
	*/
	struct SParallelLightParam {
		SVector vDir;
		SVector color;
	};

	/**
	* 環境光
	*/
	struct SAmbientLightParam {
		SVector color;
	};

	/**
	* 点光源
	*/
	struct SPointLightParam {
		SVector vPos;
		SVector color;
		SVector attn;	// 0 - 2 : attn
	};

	/**
	* スポットライト
	*/
	struct SSpotLightParam {
		SVector vPos;
		SVector vDir;
		SVector color;
		SVector param;	// 0 - 2 : attn / 3 : power
	};

	/**
	* 半球ライト
	*/
	struct SHemisphereLightParam {
		SVector vAxis;
		SVector upColor;
		SVector downColor;
	};

	/**
	* マテリアル
	*/
	struct SMaterialParam {
		// Diffuse
		SVector vDiffuse;

		// Ambient
		SVector vAmbient;

		// Specular
		// NOTE
		// w = Specular Light Power
		// Specular Light Power が大きいほどスペキュラが強くでる
		SVector vSpecular;

		// Emissive
		SVector vEmissive;

		// Rim
		// NOTE
		// w = Rim Light Power
		// Rim Light Power は０に近いほどリムライトが広がる
		SVector vRim;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_LIGHT_PARAM_H__)
