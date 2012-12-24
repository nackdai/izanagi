#if !defined(__MODEL_LIB_COLLADA_ANIMATION_H__)
#define __MODEL_LIB_COLLADA_ANIMATION_H__

#include <vector>
#include <string>
#include <set>
#include <dae.h>
#include <dom/domCOLLADA.h>

#include "izStd.h"
#include "izSceneGraph.h"
#include "izToolKit.h"

class CColladaAnimation {
    static CColladaAnimation* s_pInstance;

private:
    enum E_INPUT_SEMANTIC {
        E_INPUT_SEMANTIC_INPUT,
        E_INPUT_SEMANTIC_OUTPUT,
        E_INPUT_SEMANTIC_IN_TANGENT,
        E_INPUT_SEMANTIC_OUT_TANGENT,
        E_INPUT_SEMANTIC_INTERPOLATION,

        E_INPUT_SEMANTIC_NUM,
    };

    enum E_TRANSFROM_TYPE {
        E_TRANSFROM_TYPE_TRANSLATE,
        E_TRANSFROM_TYPE_QUARTANION,
        E_TRANSFROM_TYPE_SCALE,

        E_TRANSFROM_TYPE_NUM,
    };

    struct SJointTransform {
        std::string name;
        E_TRANSFROM_TYPE type;
        std::vector<IZ_FLOAT> param;

        bool operator==(const std::string& rhs) const
        {
            return (rhs == name);
        }
    };

    struct SJoint {
        std::string name;
        std::vector<SJointTransform> transforms;

        bool operator==(const std::string& rhs) const
        {
            return (name == rhs);
        }
    };

    struct SAnmInput {
        E_INPUT_SEMANTIC semantic;
        IZ_UINT stride;
        std::vector<IZ_FLOAT> params;

        bool operator<(const SAnmInput& rhs) const
        {
            return semantic < rhs.semantic;
        }

        bool operator==(E_INPUT_SEMANTIC rhs) const
        {
            return semantic == rhs;
        }
    };

    struct SAnmChannel {
        std::string joint;
        std::string transform;
        IZ_UINT type;
        izanagi::E_ANM_INTERP_TYPE interp;

        std::vector<SAnmInput> inputs;

        SAnmInput* FindInput(E_INPUT_SEMANTIC semantic)
        {
            std::vector<SAnmInput>::iterator it = inputs.end();
            it = std::find(inputs.begin(), inputs.end(), semantic);
            VRETURN_NULL(it != inputs.end());
            if (it != inputs.end()) {
                return &(*it);
            }
            else {
                return IZ_NULL;
            }
        }

        const SAnmInput* FindInput(E_INPUT_SEMANTIC semantic) const
        {
            std::vector<SAnmInput>::const_iterator it = inputs.end();
            it = std::find(inputs.begin(), inputs.end(), semantic);
            if (it != inputs.end()) {
                return &(*it);
            }
            else {
                return IZ_NULL;
            }
        }
    };

    struct SAnmNode {
        IZ_PCSTR nameJoint;
        IZ_UINT idxJoint;
        std::vector<SAnmChannel> channels;

        bool operator==(const std::string& rhs) const
        {
            return (izanagi::tool::CString::CmpStr(nameJoint, rhs.c_str()) == IZ_TRUE);
        }
    };

public:
    static CColladaAnimation& GetInstance();
    static void DeleteInstance();

private:
    CColladaAnimation();
    ~CColladaAnimation();

public:
    IZ_UINT GetAnmLibNum(domCOLLADA* pRoot);

    IZ_BOOL Begin(
        domCOLLADA* pRoot,
        domVisual_scene* pScene,
        IZ_UINT nAnmLibIdx);

    IZ_BOOL End();

    IZ_UINT GetAnmNodeNum();

    IZ_UINT GetAnmChannelNum(IZ_UINT nNodeIdx);

    IZ_BOOL GetAnmNode(
        IZ_UINT nNodeIdx,
        izanagi::S_ANM_NODE& sNode);

    IZ_BOOL GetAnmChannel(
        IZ_UINT nNodeIdx,
        IZ_UINT nChannelIdx,
        izanagi::S_ANM_CHANNEL& sChannel);

    IZ_BOOL GetAnmKey(
        IZ_UINT nNodeIdx,
        IZ_UINT nChannelIdx,
        IZ_UINT nKeyIdx,
        izanagi::S_ANM_KEY& sKey,
        std::vector<IZ_FLOAT>& tvValue);


private:
    IZ_BOOL ReadJoint(domVisual_scene* pScene);

    IZ_BOOL ReadJoint(domNodeRef pNode);

    E_TRANSFROM_TYPE GetTransformType(IZ_PCSTR pszType);

    template <typename _T, IZ_UINT _NUM>
    void GetTransform(
        domElement* pContent,
        SJointTransform& sTransform);

private:
    IZ_BOOL GetAnmTarget(
        domChannelRef pChannel,
        SAnmChannel& sAnmChannel);

    E_INPUT_SEMANTIC ConvSemanticFromString(daeString str);

    IZ_BOOL GetAnmInputParams(
        domInputLocalRef pInput,
        const SAnmChannel& sAnmChannel,
        SAnmInput& sAnmInput);

    IZ_BOOL GetInterpType(
        domInputLocalRef pInput,
        SAnmChannel& sAnmChannel);

    IZ_BOOL CreateSlerp(IZ_UINT nNodeIdx);

private:
    std::vector<SJoint> m_Joints;
    std::vector<SAnmNode> m_AnmNodes;
};

#endif  // #if !defined(__MODEL_LIB_COLLADA_ANIMATION_H__)
