#if !defined(__IZANAGI_SCENEGRAPH_LIGHT_PARAM_H__)
#define __IZANAGI_SCENEGRAPH_LIGHT_PARAM_H__

#include "izStd.h"
#include "../math/MathVector4.h"

namespace izanagi {
    /**
    * 平行光源
    */
    struct SParallelLightParam {
        math::SVector4 vDir;
        math::SVector4 color;
    };

    /**
    * 環境光
    */
    struct SAmbientLightParam {
        math::SVector4 color;
    };

    /**
    * 点光源
    */
    struct SPointLightParam {
        math::SVector4 vPos;
        math::SVector4 color;
        math::SVector4 attn; // 0 - 2 : attn
    };

    /**
    * スポットライト
    */
    struct SSpotLightParam {
        math::SVector4 vPos;
        math::SVector4 vDir;
        math::SVector4 color;
        math::SVector4 param;    // 0 - 2 : attn / 3 : power
    };

    /**
    * 半球ライト
    */
    struct SHemisphereLightParam {
        math::SVector4 vAxis;
        math::SVector4 upColor;
        math::SVector4 downColor;
    };

    /**
    * マテリアル
    */
    struct SMaterialParam {
        // Diffuse
        math::SVector4 vDiffuse;

        // Ambient
        math::SVector4 vAmbient;

        // Specular
        // NOTE
        // w = Specular Light Power
        // Specular Light Power が大きいほどスペキュラが強くでる
        math::SVector4 vSpecular;

        // Emissive
        math::SVector4 vEmissive;

        // Rim
        // NOTE
        // w = Rim Light Power
        // Rim Light Power は０に近いほどリムライトが広がる
        math::SVector4 vRim;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_LIGHT_PARAM_H__)
