#include "ColladaMaterial.h"

#include "izToolKit.h"

CColladaMaterial* CColladaMaterial::s_pInstance = IZ_NULL;

CColladaMaterial& CColladaMaterial::GetInstance()
{
    if (s_pInstance == IZ_NULL) {
        s_pInstance = new CColladaMaterial();
    }
    return *s_pInstance;
}

void CColladaMaterial::DeleteInstance()
{
    SAFE_DELETE(s_pInstance);
}

void CColladaMaterial::ReadImages(domCOLLADA* pRoot)
{
    IZ_ASSERT(pRoot != IZ_NULL);

    for (size_t i = 0; i < pRoot->getLibrary_images_array().getCount(); i++) {
        domLibrary_imagesRef imgRef =  pRoot->getLibrary_images_array()[i];

        for (size_t n = 0; n < imgRef->getImage_array().getCount(); n++) {
            domImageRef img = imgRef->getImage_array()[n];

            std::string tmp(img->getInit_from()->getValue().str());

            IZ_PCSTR name = izanagi::tool::CFileUtility::GetFileNameFromPath(tmp.c_str());

            m_Images[img->getId()] = name;
        }
    }
}

IZ_BOOL CColladaMaterial::ReadMaterials(domCOLLADA* pRoot)
{
    for (size_t i = 0; i < pRoot->getLibrary_materials_array().getCount(); i++) {
        domLibrary_materialsRef mtrlRef = pRoot->getLibrary_materials_array()[i];

        for (size_t n = 0; n < mtrlRef->getMaterial_array().getCount(); n++) {
            domMaterialRef mtrl = mtrlRef->getMaterial_array()[n];

            if (mtrl->getInstance_effect() == NULL) {
                continue;
            }

            domElement* element = mtrl->getInstance_effect()->getUrl().getElement(); 
            if (element == NULL) {
                continue;
            }

            domEffect* effect = (domEffect*)element;

            m_Mtrls[mtrl->getId()] = SMaterial();

            SMaterial& sMtrl = m_Mtrls.find(mtrl->getId())->second;
            {
                sMtrl.effectName = effect->getId();
            }

            VRETURN(ReadEffect(sMtrl, effect));
        }
    }

    return IZ_TRUE;
}

namespace {
    // 文字列を分解し、float値として格納する
    void _GetValue(
        daeElement* element,
        std::vector<IZ_FLOAT>& values)
    {
        static char tmp[128];

        std::string data = element->getCharData();

        const char* str = data.c_str();

        IZ_UINT pos = 0;
        for (size_t i = 0; i < strlen(str); i++) {
            char ch = str[i];
            if (ch == ' ') {
                tmp[pos] = 0;
                pos = 0;

                IZ_FLOAT f = (IZ_FLOAT)::atof(tmp);
                values.push_back(f);
            }
            else {
                tmp[pos++] = ch;
            }
        }

        if (pos > 0) {
            tmp[pos] = 0;
            IZ_FLOAT f = (IZ_FLOAT)::atof(tmp);
            values.push_back(f);
        }
    }

    IZ_BOOL _GetTextureName(
        std::string& retTexName,
        daeElement* prof,
        daeElement* texture)
    {
        std::string smplName = texture->getAttribute("texture");
        VRETURN(!smplName.empty());

        std::map<std::string, daeElement*> samplers;
        std::map<std::string, daeElement*> surfaces;

        for (size_t i = 0; i < prof->getChildren().getCount(); i++) {
            daeElement* element = prof->getChildren()[i];

            if (strcmp(element->getElementName(), "newparam") == 0) {
                std::string sid = element->getAttribute("sid");

                if (!sid.empty()) {
                    if (element->getChildren().getCount() == 1) {
                        daeElement* child = element->getChildren()[0];

                        if ((strcmp(child->getElementName(), "sampler2D") == 0)
                            || (strcmp(child->getElementName(), "sampler3D") == 0))
                        {
                            if (strcmp(sid.c_str(), smplName.c_str()) == 0) {
                                samplers[sid.c_str()] = child;
                            }
                        }
                        else if (strcmp(child->getElementName(), "surface") == 0) {
                            surfaces[sid.c_str()] = child;
                        }
                    }
                }
            }
        }

        // samplerから関係するsurfaceを探し、texture名を引いてくる

        std::map<std::string, daeElement*>::iterator itSampler = samplers.begin();
        for (; itSampler != samplers.end(); itSampler++) {
            daeElement* sampler = itSampler->second;

            for (size_t i = 0; i < sampler->getChildren().getCount(); i++) {
                daeElement* smplParam = sampler->getChildren()[i];
                if (strcmp(smplParam->getElementName(), "source") == 0) {
                    std::string surfaceName = smplParam->getCharData();

                    // samplerに設定されているsurfaceを探す
                    daeElement* surface = surfaces[surfaceName.c_str()];
                    VRETURN(surface != IZ_NULL);

                    // surfaceブロック内からテクスチャ名を探す
                    for (size_t n = 0; n < surface->getChildren().getCount(); n++) {
                        daeElement* surfParam = surface->getChildren()[0];
                        
                        if (strcmp(surfParam->getElementName(), "init_from") == 0) {
                            retTexName = surfParam->getCharData();
                            return IZ_TRUE;
                        }
                    }
                }
            }
        }

        return IZ_FALSE;
    }
}   // namespace

IZ_BOOL CColladaMaterial::ReadEffect(SMaterial& sMtrl, domEffect* effect)
{
    // 登録済みか調べる
    std::map<std::string, SEffect>::iterator it = m_Effects.find(effect->getId());

    if (it != m_Effects.end()) {
        // 登録済み

        // TODO

        return IZ_TRUE;
    }

    m_Effects[effect->getId()] = SEffect();

    SEffect& sEffect = m_Effects.find(effect->getId())->second;
    {
        sEffect.name = effect->getId();
    }

    // １つしかゆるさない
    size_t numProfiles = effect->getChildren().getCount();
    VRETURN(numProfiles == 1);

    const char* typeName = effect->getChildren()[0]->getElementName();

#if 0
    if (strcmp("profile_COMMON", typeName) == 0) {
        domProfile_COMMON* profCommon = (domProfile_COMMON *)(domFx_profile_abstract*)effect->getFx_profile_abstract_array()[0]; 

        domProfile_COMMON::domTechnique* technique = profCommon->getTechnique();
        VRETURN(technique != IZ_NULL);

        // NOTE
        // 上から コンスタントカラー、ランバート、フォン、ブリンシェーダのパラメータを調べていく
        // どれかに当たるはず？

        domProfile_COMMON::domTechnique::domConstant* constant = technique->getConstant();
        ReadEffectParams(sEffect, profCommon, constant);

        domProfile_COMMON::domTechnique::domLambert* lambert = technique->getLambert();
        ReadEffectParams(sEffect, profCommon, lambert);

        domProfile_COMMON::domTechnique::domPhong* phong = technique->getPhong();
        ReadEffectParams(sEffect, profCommon, phong);

        domProfile_COMMON::domTechnique::domBlinn* blinn = technique->getBlinn();
        ReadEffectParams(sEffect, profCommon, blinn);
    }
    else
#endif
    {
        // ユーザー任意の設定の場合

        daeElement* prof = effect->getChildren()[0];

        for (size_t i = 0; i < prof->getChildren().getCount(); i++) {
            daeElement* tech = prof->getChildren()[i];
            if (strcmp(tech->getElementName(), "technique") == 0) {
                daeElement* effect = tech->getChildren()[0];
                IZ_ASSERT(effect != IZ_NULL);

                for (size_t n = 0; n < effect->getChildren().getCount(); n++) {
                    daeElement* param = effect->getChildren()[n];

                    VRETURN(param->getChildren().getCount() == 1);
                    daeElement* value = param->getChildren()[0];

                    std::vector<IZ_FLOAT> values;

                    if (strcmp(value->getElementName(), "texture") == 0) {
                        // 参照名を取得
                        std::string texName;
                        VRETURN(_GetTextureName(texName, prof, value));

                        if (!texName.empty()) {
                            // ファイル名を取得
                            std::string& imgName = m_Images[texName.c_str()];
                            sEffect.texName.push_back(imgName);
                        }
                    }
                    else {
                        if (strcmp(value->getElementName(), "color") == 0) {
                            // float4
                            _GetValue(value, values);
                        }
                        else if (strcmp(value->getElementName(), "float") == 0) {
                            _GetValue(value, values);
                        }
                        else {
                            // 何かある？
                            VRETURN(IZ_FALSE);
                        }

                        const char* name = param->getElementName();

                        sEffect.params.push_back(SEffectParam(name, &values[0], (IZ_UINT)values.size()));
                    }
                }

                break;
            }
        }
    }

    return IZ_TRUE;
}

namespace {
    izanagi::math::SVector _ReadColorOrTextureType(domCommon_color_or_texture_type_complexType* color_or_texture)
    {
        izanagi::math::SVector ret;

        if (color_or_texture->getColor()) {
            const domFx_color_common& color = color_or_texture->getColor()->getValue();

            ret.Set(
                (float)color[0],
                (float)color[1],
                (float)color[2],
                (float)color[3]);
        }
        else {
            ret.Set(1.0f, 1.0f, 1.0f, 1.0f);
        }

        return ret;
    }

    IZ_FLOAT _ReadFloatOrParamType(domCommon_float_or_param_typeRef float_or_param)
    {
        if (float_or_param != IZ_NULL) {
            IZ_FLOAT ret = static_cast<IZ_FLOAT>(float_or_param->getFloat()->getValue());
            return ret;
        }

        return 0.0f;
    }

    IZ_PCSTR _ReadTexture(domProfile_COMMON* prof, domCommon_color_or_texture_type* color_or_texture)
    {
        domCommon_color_or_texture_type::domTexture* texture = color_or_texture->getTexture();
        if (texture == IZ_NULL) {
            return IZ_NULL;
        }

        std::map<std::string, domCommon_newparam_type*> newParams;

        for (size_t i = 0; i < prof->getNewparam_array().getCount(); i++) {
            domCommon_newparam_type* param = prof->getNewparam_array()[i];

            newParams[param->getSid()] = param;
        }

        std::string sidSampler2D = texture->getTexture();
        std::string sidSurface = newParams[sidSampler2D]->getSampler2D()->getSource()->getValue();

        VRETURN_VAL(
            newParams[sidSurface]->getSurface()->getFx_surface_init_common() != IZ_NULL,
            IZ_NULL);

        xsIDREF& idRef = newParams[sidSurface]->getSurface()->getFx_surface_init_common()->getInit_from_array()[0]->getValue();
        idRef.resolveElement();
        domImage* imageElement = (domImage*)(domElement*) idRef.getElement();
        return imageElement->getInit_from()->getValue().str().c_str();
    }
}   // namespace

#define _READ_COLOR_OR_TEXTURE(p, func, name)\
    do {\
        domCommon_color_or_texture_type_complexType* type = p->func();\
        if (type != IZ_NULL) {\
            izanagi::math::SVector color = _ReadColorOrTextureType(type);\
            sEffect.params.push_back(SEffectParam(#name, color.v, COUNTOF(color.v)));\
        }\
    } while(0)

#define _READ_FLOAT_OR_PARAM(p, func, name)\
    do {\
        domCommon_float_or_param_typeRef type = p->func();\
        if (type != IZ_NULL) {\
            IZ_FLOAT f = _ReadFloatOrParamType(type);\
            sEffect.params.push_back(SEffectParam(#name, &f, 1));\
        }\
    } while(0)


template <typename _T>
void CColladaMaterial::ReadEffectParams(SEffect& sEffect, domProfile_COMMON* prof, _T* param)
{
    if (param == IZ_NULL) {
        return;
    }

    _READ_COLOR_OR_TEXTURE(param, getDiffuse, "diffuse");
    _READ_COLOR_OR_TEXTURE(param, getAmbient, "ambient");
    _READ_COLOR_OR_TEXTURE(param, getSpecular, "specular");
    _READ_FLOAT_OR_PARAM(param, getShininess, "shininess");

    IZ_PCSTR texName = _ReadTexture(prof, param->getDiffuse());
    if (texName != IZ_NULL) {
        std::string name = izanagi::tool::CFileUtility::GetFileNameFromPath(texName);
        sEffect.texName.push_back(name);
    }

    _READ_COLOR_OR_TEXTURE(param, getEmission, "emission");
    _READ_COLOR_OR_TEXTURE(param, getReflective, "reflective");
    _READ_FLOAT_OR_PARAM(param, getReflectivity, "reflectivity");
    _READ_COLOR_OR_TEXTURE(param, getTransparent, "transparent");
    _READ_FLOAT_OR_PARAM(param, getTransparency, "transparency");
    _READ_FLOAT_OR_PARAM(param, getIndex_of_refraction, "refraction");
}

template <>
void CColladaMaterial::ReadEffectParams(SEffect& sEffect, domProfile_COMMON* prof, domProfile_COMMON::domTechnique::domConstant* param)
{
    if (param == IZ_NULL) {
        return;
    }

    _READ_COLOR_OR_TEXTURE(param, getEmission, "emission");
    _READ_COLOR_OR_TEXTURE(param, getReflective, "reflective");
    _READ_FLOAT_OR_PARAM(param, getReflectivity, "reflectivity");
    _READ_COLOR_OR_TEXTURE(param, getTransparent, "transparent");
    _READ_FLOAT_OR_PARAM(param, getTransparency, "transparency");
    _READ_FLOAT_OR_PARAM(param, getIndex_of_refraction, "refraction");
}

template <>
void CColladaMaterial::ReadEffectParams(SEffect& sEffect, domProfile_COMMON* prof, domProfile_COMMON::domTechnique::domLambert* param)
{
    if (param == IZ_NULL) {
        return;
    }

    _READ_COLOR_OR_TEXTURE(param, getDiffuse, "diffuse");
    _READ_COLOR_OR_TEXTURE(param, getAmbient, "ambient");

    IZ_PCSTR texName = _ReadTexture(prof, param->getDiffuse());
    if (texName != IZ_NULL) {
        std::string name = izanagi::tool::CFileUtility::GetFileNameFromPath(texName);
        sEffect.texName.push_back(name);
    }

    _READ_COLOR_OR_TEXTURE(param, getEmission, "emission");
    _READ_COLOR_OR_TEXTURE(param, getReflective, "reflective");
    _READ_FLOAT_OR_PARAM(param, getReflectivity, "reflectivity");
    _READ_COLOR_OR_TEXTURE(param, getTransparent, "transparent");
    _READ_FLOAT_OR_PARAM(param, getTransparency, "transparency");
    _READ_FLOAT_OR_PARAM(param, getIndex_of_refraction, "refraction");
}

IZ_BOOL CColladaMaterial::GetMaterial(
    IZ_UINT nMtrlIdx,
    SMaterial& retMtrl,
    std::string& retMtrlName)
{
    std::map<std::string, SMaterial>::iterator it = m_Mtrls.begin();
    for (IZ_UINT cnt = 0; it != m_Mtrls.end(); it++, cnt++) {
        if (nMtrlIdx == cnt) {
            retMtrlName = it->first;
            retMtrl = it->second;
            break;
        }
    }

    return (!retMtrlName.empty());
}

IZ_BOOL CColladaMaterial::GetMaterial(
    IZ_UINT nMtrlIdx,
    izanagi::S_MTRL_MATERIAL& sMtrl)
{
    IZ_ASSERT(nMtrlIdx < GetMaterialNum());

    SMaterial mtrl;
    std::string mtrlName;

    VRETURN(
        GetMaterial(nMtrlIdx, mtrl, mtrlName));

    const SEffect& effect = m_Effects[mtrl.effectName.c_str()];

    sMtrl.name.SetString(mtrlName.c_str());
    sMtrl.keyMaterial = sMtrl.name.GetKeyValue();

    sMtrl.numTex = (IZ_UINT16)effect.texName.size();

    // NOTE
    // １つしかない（はず）
    sMtrl.numShader = 1;

    sMtrl.numParam = (IZ_UINT16)effect.params.size();

    sMtrl.paramBytes = 0;

    for (size_t i = 0; i < effect.params.size(); i++) {
        const SEffectParam& param = effect.params[i];

        sMtrl.paramBytes += (IZ_UINT16)(sizeof(IZ_FLOAT) * param.params.size());
    }

    return IZ_TRUE;
}

void CColladaMaterial::GetMaterialTexture(
    IZ_UINT nMtrlIdx,
    IZ_UINT nTexIdx,
    izanagi::S_MTRL_TEXTURE& sTex)
{
    IZ_ASSERT(nMtrlIdx < GetMaterialNum());

    SMaterial mtrl;
    std::string mtrlName;

    IZ_BOOL result = GetMaterial(nMtrlIdx, mtrl, mtrlName);
    IZ_ASSERT(result);

    const SEffect& effect = m_Effects[mtrl.effectName.c_str()];

    IZ_ASSERT(nTexIdx < (IZ_UINT)effect.texName.size());
    const std::string& texName = effect.texName[nTexIdx];

    sTex.name.SetString(texName.c_str());
    sTex.key = sTex.name.GetKeyValue();

    // TODO
    sTex.type.flags = 0;
}

void CColladaMaterial::GetMaterialShader(
    IZ_UINT nMtrlIdx,
    IZ_UINT nShaderIdx,
    izanagi::S_MTRL_SHADER& sShader)
{
    IZ_ASSERT(nMtrlIdx < GetMaterialNum());

    SMaterial mtrl;
    std::string mtrlName;

    IZ_BOOL result = GetMaterial(nMtrlIdx, mtrl, mtrlName);
    IZ_ASSERT(result);

    const SEffect& effect = m_Effects[mtrl.effectName.c_str()];

    // TODO
    // シェーダ名はどうやって持ってくる？
    // というよりもどこにある？

    sShader.name.SetString("DefaultShader");
    sShader.key = sShader.name.GetKeyValue();
}

void CColladaMaterial::GetMaterialParam(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    izanagi::S_MTRL_PARAM& sParam)
{
    IZ_ASSERT(nMtrlIdx < GetMaterialNum());

    SMaterial mtrl;
    std::string mtrlName;

    IZ_BOOL result = GetMaterial(nMtrlIdx, mtrl, mtrlName);
    IZ_ASSERT(result);

    const SEffect& effect = m_Effects[mtrl.effectName.c_str()];

    IZ_ASSERT(nParamIdx < (IZ_UINT)effect.params.size());
    const SEffectParam& param = effect.params[nParamIdx];

    sParam.name.SetString(param.name.c_str());
    sParam.key = sParam.name.GetKeyValue();
    sParam.elements = (IZ_UINT)param.params.size();
    sParam.bytes = sizeof(IZ_FLOAT) * sParam.elements;

    switch (sParam.elements) {
    case 1:
        sParam.type = izanagi::E_MTRL_PARAM_TYPE_FLOAT;
        break;
    case 2:
    case 3:
    case 4:
        sParam.type = izanagi::E_MTRL_PARAM_TYPE_VECTOR;
        break;
    case 16:
        sParam.type = izanagi::E_MTRL_PARAM_TYPE_MATRIX;
        break;
    default:
        IZ_ASSERT(IZ_FALSE);
    }
}

void CColladaMaterial::GetMaterialParamValue(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    std::vector<IZ_FLOAT>& tvValue)
{
    IZ_ASSERT(nMtrlIdx < GetMaterialNum());

    SMaterial mtrl;
    std::string mtrlName;

    IZ_BOOL result = GetMaterial(nMtrlIdx, mtrl, mtrlName);
    IZ_ASSERT(result);

    const SEffect& effect = m_Effects[mtrl.effectName.c_str()];

    IZ_ASSERT(nParamIdx < (IZ_UINT)effect.params.size());
    const SEffectParam& param = effect.params[nParamIdx];

    tvValue.insert(tvValue.begin(), param.params.begin(), param.params.end());
}
