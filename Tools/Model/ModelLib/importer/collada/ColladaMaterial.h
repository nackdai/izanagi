#if !defined(__MODEL_LIB_COLLADA_MATERIAL_H__)
#define __MODEL_LIB_COLLADA_MATERIAL_H__

#include <dom/domCOLLADA.h>
#include <dom/domProfile_COMMON.h>
#include <dae.h>
#include <map>
#include <string>

#include "izStd.h"
#include "izSceneGraph.h"

class CColladaMaterial {
    static CColladaMaterial* s_pInstance;

    struct SMaterial {
        std::string effectName;
    };

    struct SEffectParam {
        std::string name;
        std::vector<IZ_FLOAT> params;

        SEffectParam() {}
        SEffectParam(const char* _name, IZ_FLOAT* _params, IZ_UINT num)
        {
            name = _name;
            for (IZ_UINT i = 0; i < num; i++) {
                params.push_back(_params[i]);
            }
        }
    };

    struct SEffect {
        std::string name;
        std::vector<SEffectParam> params;
        std::vector<std::string> texName;
    };

public:
    static CColladaMaterial& GetInstance();
    static void DeleteInstance();

private:
    CColladaMaterial() {}
    ~CColladaMaterial() {}

public:
    void ReadImages(domCOLLADA* pRoot);
    IZ_BOOL ReadMaterials(domCOLLADA* pRoot);

    IZ_UINT GetMaterialNum() { return (IZ_UINT)m_Mtrls.size(); }

    IZ_BOOL GetMaterial(
        IZ_UINT nMtrlIdx,
        izanagi::S_MTRL_MATERIAL& sMtrl);

    void GetMaterialTexture(
        IZ_UINT nMtrlIdx,
        IZ_UINT nTexIdx,
        izanagi::S_MTRL_TEXTURE& sTex);

    void GetMaterialShader(
        IZ_UINT nMtrlIdx,
        IZ_UINT nShaderIdx,
        izanagi::S_MTRL_SHADER& sShader);

    void GetMaterialParam(
        IZ_UINT nMtrlIdx,
        IZ_UINT nParamIdx,
        izanagi::S_MTRL_PARAM& sParam);

    void GetMaterialParamValue(
        IZ_UINT nMtrlIdx,
        IZ_UINT nParamIdx,
        std::vector<IZ_FLOAT>& tvValue);

private:
    IZ_BOOL ReadEffect(SMaterial& sMtrl, domEffect* effect);

    template <typename _T>
    void ReadEffectParams(SEffect& sEffect, domProfile_COMMON* prof, _T* param);

    template <>
    void ReadEffectParams(SEffect& sEffect, domProfile_COMMON* prof, domProfile_COMMON::domTechnique::domConstant* param);

    template <>
    void ReadEffectParams(SEffect& sEffect, domProfile_COMMON* prof, domProfile_COMMON::domTechnique::domLambert* param);

    IZ_BOOL GetMaterial(
        IZ_UINT nMtrlIdx,
        SMaterial& retMtrl,
        std::string& retMtrlName);

private:
    std::map<std::string, std::string> m_Images;
    std::map<std::string, SMaterial> m_Mtrls;

    std::map<std::string, SEffect> m_Effects;
};

#endif  // #if !defined(__MODEL_LIB_COLLADA_MATERIAL_H__)
