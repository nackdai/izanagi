#include <string.h>
#include <algorithm>
#include <dom/domConstants.h>
#include "ColladaAnimation.h"

CColladaAnimation* CColladaAnimation::s_pInstance = IZ_NULL;

CColladaAnimation& CColladaAnimation::GetInstance()
{
    if (s_pInstance == IZ_NULL) {
        s_pInstance = new CColladaAnimation();
    }
    return *s_pInstance;
}

void CColladaAnimation::DeleteInstance()
{
    SAFE_DELETE(s_pInstance);
}

CColladaAnimation::CColladaAnimation()
{
}

CColladaAnimation::~CColladaAnimation()
{
}

IZ_UINT CColladaAnimation::GetAnmLibNum(domCOLLADA* pRoot)
{
    size_t nAnmNum = pRoot->getLibrary_animations_array().getCount();
    return static_cast<IZ_UINT>(nAnmNum);
}

IZ_BOOL CColladaAnimation::Begin(
    domCOLLADA* pRoot,
    domVisual_scene* pScene,
    IZ_UINT nAnmLibIdx)
{
    VRETURN(ReadJoint(pScene));

    m_AnmNodes.resize(m_Joints.size());
    for (size_t i = 0; i < m_AnmNodes.size(); i++) {
        m_AnmNodes[i].nameJoint = m_Joints[i].name.c_str();
        m_AnmNodes[i].idxJoint = static_cast<IZ_UINT>(i);
    }

    domLibrary_animationsRef pAnmLib = pRoot->getLibrary_animations_array().get(nAnmLibIdx);

    size_t nAnmNum = pAnmLib->getAnimation_array().getCount();

    for (size_t nAnmCnt = 0; nAnmCnt < nAnmNum; nAnmCnt++) {
        domAnimationRef pAnm = pAnmLib->getAnimation_array().get(nAnmCnt);

        SAnmChannel sAnmChannel;
        {
            // Set initial value...
            sAnmChannel.interp = izanagi::E_ANM_INTERP_TYPE_LINEAR;
        }

        IZ_ASSERT(pAnm->getChannel_array().getCount() == 1);
        domChannelRef pChannel = pAnm->getChannel_array().get(0);

        VRETURN(GetAnmTarget(pChannel, sAnmChannel));

        // Find target joint.
        std::vector<SAnmNode>::iterator it = std::find(
                                                m_AnmNodes.begin(),
                                                m_AnmNodes.end(),
                                                sAnmChannel.joint);
        if (it == m_AnmNodes.end()) {
            // Not find target joint...
            continue;
        }

        IZ_ASSERT(pAnm->getSampler_array().getCount() == 1);
        domSamplerRef pSmpl = pAnm->getSampler_array().get(0);

        size_t nInputNum = pSmpl->getInput_array().getCount();

        sAnmChannel.inputs.resize(nInputNum);

        for (size_t nInputCnt = 0; nInputCnt < nInputNum; nInputCnt++) {
            SAnmInput& sAnmInput = sAnmChannel.inputs[nInputCnt];

            domInputLocalRef pInput = pSmpl->getInput_array().get(nInputCnt);

            daeString strSemantic = pInput->getSemantic();
            sAnmInput.semantic = ConvSemanticFromString(strSemantic);
            VRETURN(sAnmInput.semantic < E_INPUT_SEMANTIC_NUM);

            if (sAnmInput.semantic == E_INPUT_SEMANTIC_INTERPOLATION) {
                // Get interpolation type.
                VRETURN(GetInterpType(pInput, sAnmChannel));
            }
            else {
                VRETURN(
                    GetAnmInputParams(
                        pInput,
                        sAnmChannel,
                        sAnmInput));
            }
        }

        size_t nNodeIdx = std::distance(m_AnmNodes.begin(), it);
        m_AnmNodes[nNodeIdx].channels.push_back(sAnmChannel);
    }

    // Remove node which has no animation channel.
    std::vector<SAnmNode>::iterator it = m_AnmNodes.begin();
    for (; it != m_AnmNodes.end();) {
        const SAnmNode& sNode = *it;
        if (sNode.channels.empty()) {
            it = m_AnmNodes.erase(it);
        }
        else {
            it++;
        }
    }

    for (IZ_UINT i = 0; i < m_AnmNodes.size(); i++) {
        VRETURN(CreateSlerp(i));
    }

    return IZ_TRUE;
}

IZ_BOOL CColladaAnimation::End()
{
    m_Joints.clear();
    m_AnmNodes.clear();
    return IZ_TRUE;
}

IZ_UINT CColladaAnimation::GetAnmNodeNum()
{
    return static_cast<IZ_UINT>(m_AnmNodes.size());
}

IZ_UINT CColladaAnimation::GetAnmChannelNum(IZ_UINT nNodeIdx)
{
    IZ_ASSERT(nNodeIdx < m_AnmNodes.size());
    return static_cast<IZ_UINT>(m_AnmNodes[nNodeIdx].channels.size());
}

IZ_BOOL CColladaAnimation::GetAnmNode(
    IZ_UINT nNodeIdx,
    izanagi::S_ANM_NODE& sNode)
{
    IZ_ASSERT(nNodeIdx < m_AnmNodes.size());

    const SAnmNode& sAnmNode = m_AnmNodes[nNodeIdx];

    sNode.target.SetString(sAnmNode.nameJoint);
    sNode.targetKey = sNode.target.GetKeyValue();

    sNode.targetIdx = sAnmNode.idxJoint;

    sNode.numChannels = GetAnmChannelNum(nNodeIdx);

    return IZ_TRUE;
}

IZ_BOOL CColladaAnimation::GetAnmChannel(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    izanagi::S_ANM_CHANNEL& sChannel)
{
    IZ_ASSERT(nNodeIdx < m_AnmNodes.size());
    const SAnmNode& sAnmNode = m_AnmNodes[nNodeIdx];

    IZ_ASSERT(nChannelIdx < sAnmNode.channels.size());
    const SAnmChannel& sAnmChannel = sAnmNode.channels[nChannelIdx];

    const SAnmInput* pKeyTime = sAnmChannel.FindInput(E_INPUT_SEMANTIC_INPUT);
    VRETURN(pKeyTime != IZ_NULL);

    const SAnmInput* pValue = sAnmChannel.FindInput(E_INPUT_SEMANTIC_OUTPUT);
    VRETURN(pValue != IZ_NULL);

    sChannel.interp = sAnmChannel.interp;
    sChannel.type = sAnmChannel.type;
    sChannel.numKeys = static_cast<IZ_UINT>(pKeyTime->params.size());
    sChannel.stride = pValue->stride;

    return IZ_TRUE;
}

IZ_BOOL CColladaAnimation::GetAnmKey(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    IZ_UINT nKeyIdx,
    izanagi::S_ANM_KEY& sKey,
    std::vector<IZ_FLOAT>& tvValue)
{
    IZ_ASSERT(nNodeIdx < m_AnmNodes.size());
    const SAnmNode& sAnmNode = m_AnmNodes[nNodeIdx];

    IZ_ASSERT(nChannelIdx < sAnmNode.channels.size());
    const SAnmChannel& sAnmChannel = sAnmNode.channels[nChannelIdx];

    const SAnmInput* pKeyTime = sAnmChannel.FindInput(E_INPUT_SEMANTIC_INPUT);
    VRETURN(pKeyTime != IZ_NULL);
    VRETURN(nKeyIdx < pKeyTime->params.size());

    const SAnmInput* pValue = sAnmChannel.FindInput(E_INPUT_SEMANTIC_OUTPUT);
    VRETURN(pValue != IZ_NULL);

    sKey.keyTime = pKeyTime->params[nKeyIdx];
    sKey.stride = pValue->stride;

    const SAnmInput* pInTanget = sAnmChannel.FindInput(E_INPUT_SEMANTIC_IN_TANGENT);
    const SAnmInput* pOutTanget = sAnmChannel.FindInput(E_INPUT_SEMANTIC_OUT_TANGENT);
    
    if ((pInTanget != IZ_NULL) && (pOutTanget != IZ_NULL)) {
        VRETURN((sKey.stride == pInTanget->stride)
                && (sKey.stride == pOutTanget->stride));

        IZ_UINT pos = sKey.stride * nKeyIdx;

        for (IZ_UINT i = 0; i < sKey.stride; i++) {
            tvValue.push_back(pValue->params[pos + i]);
            tvValue.push_back(pInTanget->params[pos + i]);
            tvValue.push_back(pOutTanget->params[pos + i]);
        }
    }
    else {
        VRETURN((pInTanget == IZ_NULL)
                && (pOutTanget == IZ_NULL));

        tvValue.resize(sKey.stride);
        IZ_UINT pos = sKey.stride * nKeyIdx;

        for (IZ_UINT i = 0; i < sKey.stride; i++) {
            tvValue[i] = pValue->params[pos + i];
        }
    }

    sKey.numParams = static_cast<IZ_UINT>(tvValue.size());

    return IZ_TRUE;
}

IZ_BOOL CColladaAnimation::ReadJoint(domVisual_scene* pScene)
{
    size_t nNodeCnt = pScene->getNode_array().getCount();

    for (size_t i = 0; i < nNodeCnt; i++) {
        domNodeRef pNode = pScene->getNode_array().get(i);
        VRETURN(ReadJoint(pNode));
    }

    return IZ_TRUE;
}

IZ_BOOL CColladaAnimation::ReadJoint(domNodeRef pNode)
{
    domNodeType type = pNode->getType();

    if (type == NODETYPE_JOINT) {
        SJoint sJoint;

        sJoint.name = pNode->getName();

        for (size_t n = 0; n < pNode->getContents().getCount(); n++) {
            domElement* pContent = pNode->getContents().get(n);

            daeString strType = pContent->getTypeName();
            if (izanagi::tool::CString::CmpStr(strType, "node")) {
                // terminate...
                continue;
            }

            SJointTransform sTransform;
            sTransform.type = GetTransformType(strType);

            switch (sTransform.type) {
            case E_TRANSFROM_TYPE_TRANSLATE:
                GetTransform<domTranslate, 3>(pContent, sTransform);
                break;
            case E_TRANSFROM_TYPE_QUARTANION:
                GetTransform<domRotate, 4>(pContent, sTransform);
                break;
            case E_TRANSFROM_TYPE_SCALE:
                GetTransform<domScale, 3>(pContent, sTransform);
                break;
            default:
                VRETURN(IZ_FALSE);
                break;
            }

            sJoint.transforms.push_back(sTransform);
        }

        if (!sJoint.transforms.empty()) {
            m_Joints.push_back(sJoint);
        }
    }

    size_t nChildNodeCnt = pNode->getNode_array().getCount();

    for (size_t n = 0; n < nChildNodeCnt; n++) {
        domNodeRef node = pNode->getNode_array().get(n);
        VRETURN(ReadJoint(node));
    }

    return IZ_TRUE;
}

CColladaAnimation::E_TRANSFROM_TYPE CColladaAnimation::GetTransformType(IZ_PCSTR pszType)
{
    IZ_PCSTR tblType[] = {
        "translate",    // E_MDL_JOINT_TRANSFORM_TRANSLATE
        "rotate",       // E_MDL_JOINT_TRANSFORM_QUATERNION
        "scale",        // E_MDL_JOINT_TRANSFORM_SCALE
    };
    IZ_C_ASSERT(COUNTOF(tblType) == E_TRANSFROM_TYPE_NUM);

    for (IZ_UINT i = 0; i < E_TRANSFROM_TYPE_NUM; i++) {
        if (izanagi::tool::CString::CmpStr(tblType[i], pszType)) {
            return (E_TRANSFROM_TYPE)i;
        }
    }

    IZ_ASSERT(IZ_FALSE);
    return E_TRANSFROM_TYPE_NUM;
}

template <typename _T, IZ_UINT _NUM>
void CColladaAnimation::GetTransform(
    domElement* pContent,
    SJointTransform& sTransform)
{
    IZ_C_ASSERT((0 < _NUM) && (_NUM <= 4));

    _T* pTransform = reinterpret_cast<_T*>(pContent);
    IZ_ASSERT(pTransform->getValue().getCount() == _NUM);

    sTransform.name = pTransform->getSid();

    for (IZ_UINT i = 0; i < _NUM; i++) {
        sTransform.param.push_back((IZ_FLOAT)pTransform->getValue().get(i));
    }
}

namespace {
    inline IZ_PCSTR _FindStr(IZ_PCSTR str, IZ_PCSTR c )
    {
        return strstr(str, c);
    }
}   // namespace

IZ_BOOL CColladaAnimation::GetAnmTarget(
    domChannelRef pChannel,
    SAnmChannel& sAnmChannel)
{
    static IZ_CHAR tmp[128];

    daeString target = pChannel->getTarget();

    // Get target joint's name.
    IZ_PCSTR pszId = _FindStr(target, "/");
    {
        if (pszId == IZ_NULL) {
            pszId = _FindStr(target, "(");
        }
        if (pszId == IZ_NULL) {
            pszId = _FindStr(target, ".");
        }

        VRETURN(pszId != IZ_NULL);

        size_t nSize = static_cast<size_t>(izanagi::CStdUtil::GetPtrDistance(target, pszId));
        VRETURN((1 < nSize) && (nSize < COUNTOF(tmp)));

        FILL_ZERO(tmp, sizeof(tmp));
        memcpy(tmp, target, nSize);

        sAnmChannel.joint = tmp;
    }

    static IZ_CHAR strMember[izanagi::ANM_NAME_LEN + 1];

    // Get target transform's sid.
    IZ_PCSTR pszSId = _FindStr(target, ".");
    {
        if (pszSId == IZ_NULL) {
            pszSId = _FindStr(target, "(");
        }
        if (pszSId == IZ_NULL) {
            pszSId = target + strlen(target);
        }

        VRETURN(pszSId != IZ_NULL);
        VRETURN(pszSId > pszId);

        size_t nSize = static_cast<size_t>(izanagi::CStdUtil::GetPtrDistance(pszSId, pszId));
        VRETURN((1 < nSize) && (nSize < COUNTOF(tmp)));

        FILL_ZERO(tmp, sizeof(tmp));
        memcpy(tmp, pszId + 1, nSize - 1);

        sAnmChannel.transform = tmp;

        FILL_ZERO(strMember, sizeof(strMember));
        if ((pszSId != '\0') && (strlen(pszSId) > 1)) {
            memcpy(strMember, pszSId + 1, strlen(pszSId) - 1);
        }
    }

    domElement* pRootNode = pChannel->getDocument()->getDomRoot();

    // Get transform element of target node.
    daeSIDResolver sidResolver(pRootNode, target);
    domElement* pElement = sidResolver.getElement();
    VRETURN(pElement != IZ_NULL);

#if 0
    COLLADA_TYPE::TypeEnum type = pElement->getElementType();
    switch (type) {
    case COLLADA_TYPE::TRANSLATE:
        sAnmChannel.type = izanagi::E_ANM_TRANSFORM_TYPE_TRANSLATE;
        break;
    case COLLADA_TYPE::ROTATE:
        sAnmChannel.type = izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION;
        break;
    case COLLADA_TYPE::SCALE:
        sAnmChannel.type = izanagi::E_ANM_TRANSFORM_TYPE_SCALE;
        break;
    default:
        VRETURN(IZ_FALSE);
        break;
    }
#else
    izanagi::tool::CString type(pElement->getElementName());
    if (type == ColladaDOM141::COLLADA_TYPE_TRANSLATE)
    {
        sAnmChannel.type = izanagi::E_ANM_TRANSFORM_TYPE_TRANSLATE;
    }
    else if (type == ColladaDOM141::COLLADA_TYPE_ROTATE)
    {
        sAnmChannel.type = izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION;
    }
    else if (type == ColladaDOM141::COLLADA_TYPE_SCALE)
    {
        sAnmChannel.type = izanagi::E_ANM_TRANSFORM_TYPE_SCALE;
    }
    else
    {
        VRETURN(IZ_FALSE);
    }
#endif

    if (strlen(strMember) > 0) {
        if (sAnmChannel.type & izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION) {
            if (izanagi::tool::CString::CmpStr(strMember, "AXIS")) {
                sAnmChannel.type = izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION_XYZ;
            }
            else if (izanagi::tool::CString::CmpStr(strMember, "ANGLE")) {
                sAnmChannel.type = izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION_W;
            }
        }
        else if (izanagi::tool::CString::CmpStr(strMember, "X")) {
            sAnmChannel.type |= izanagi::E_ANM_TRANSFORM_TYPE_X;
        }
        else if (izanagi::tool::CString::CmpStr(strMember, "Y")) {
            sAnmChannel.type |= izanagi::E_ANM_TRANSFORM_TYPE_Y;
        }
        else if (izanagi::tool::CString::CmpStr(strMember, "Z")) {
            sAnmChannel.type |= izanagi::E_ANM_TRANSFORM_TYPE_Z;
        }
    }
    else {
        if (sAnmChannel.type & izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION) {
            sAnmChannel.type |= izanagi::E_ANM_TRANSFORM_TYPE_XYZW;
        }
        else {
            sAnmChannel.type |= izanagi::E_ANM_TRANSFORM_TYPE_XYZ;
        }
    }

    return IZ_TRUE;
}

CColladaAnimation::E_INPUT_SEMANTIC CColladaAnimation::ConvSemanticFromString(daeString str)
{
    daeString tblSemantic[] = {
        "INPUT",
        "OUTPUT",
        "IN_TANGENT",
        "OUT_TANGENT",
        "INTERPOLATION",
    };
    IZ_C_ASSERT(COUNTOF(tblSemantic) == E_INPUT_SEMANTIC_NUM);

    for (IZ_UINT i = 0; i < E_INPUT_SEMANTIC_NUM; i++) {
        if (izanagi::tool::CString::CmpStr(tblSemantic[i], str)) {
            return static_cast<E_INPUT_SEMANTIC>(i);
        }
    }

    IZ_ASSERT(IZ_FALSE);
    return E_INPUT_SEMANTIC_NUM;
}

namespace {
    inline IZ_UINT _GetStride(IZ_UINT nType)
    {
        izanagi::E_ANM_TRANSFORM_TYPE nTargetType = static_cast<izanagi::E_ANM_TRANSFORM_TYPE>(nType);

        IZ_UINT ret = 0;

        switch (nTargetType) {
        case izanagi::E_ANM_TRANSFORM_TYPE_TRANSLATE_X:
        case izanagi::E_ANM_TRANSFORM_TYPE_ROTATE_X:
        case izanagi::E_ANM_TRANSFORM_TYPE_SCALE_X:
        case izanagi::E_ANM_TRANSFORM_TYPE_TRANSLATE_Y:
        case izanagi::E_ANM_TRANSFORM_TYPE_ROTATE_Y:
        case izanagi::E_ANM_TRANSFORM_TYPE_SCALE_Y:
        case izanagi::E_ANM_TRANSFORM_TYPE_TRANSLATE_Z:
        case izanagi::E_ANM_TRANSFORM_TYPE_ROTATE_Z:
        case izanagi::E_ANM_TRANSFORM_TYPE_SCALE_Z:
        case izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION_W:
            ret = 1;
            break;
        case izanagi::E_ANM_TRANSFORM_TYPE_TRANSLATE_XYZ:
        case izanagi::E_ANM_TRANSFORM_TYPE_ROTATE_XYZ:
        case izanagi::E_ANM_TRANSFORM_TYPE_SCALE_XYZ:
        case izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION_XYZ:
            ret = 3;
            break;
        case izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION_XYZW:
            ret = 4;
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }

        return ret;
    }
}   // namespace

IZ_BOOL CColladaAnimation::GetAnmInputParams(
    domInputLocalRef pInput,
    const SAnmChannel& sAnmChannel,
    SAnmInput& sAnmInput)
{
    domSource* pSrc = (domSource*)((domElement*)pInput->getSource().getElement());
    VRETURN(pSrc != IZ_NULL);

    if (sAnmInput.semantic == E_INPUT_SEMANTIC_INPUT) {
        // Stride must be 1.
        sAnmInput.stride = (IZ_UINT)pSrc->getTechnique_common()->getAccessor()->getStride();
        VRETURN(sAnmInput.stride == 1);

        // Input parmeter means "TIME".
        domUint nParamCnt = pSrc->getTechnique_common()->getAccessor()->getParam_array().getCount();
        VRETURN(nParamCnt == 1);

        domParamRef pParam = pSrc->getTechnique_common()->getAccessor()->getParam_array().get(0);
        daeString name = pParam->getName();
        VRETURN(izanagi::tool::CString::CmpStr(name, "TIME"));

        domUint num = pSrc->getFloat_array()->getCount();

        for (size_t i = 0; i < num; i++) {
            IZ_FLOAT time = (IZ_FLOAT)pSrc->getFloat_array()->getValue().get(i);
            sAnmInput.params.push_back(time);
        }
    }
    else if ((sAnmInput.semantic == E_INPUT_SEMANTIC_OUTPUT)
            || (sAnmInput.semantic == E_INPUT_SEMANTIC_IN_TANGENT)
            || (sAnmInput.semantic == E_INPUT_SEMANTIC_OUT_TANGENT))
    {
        domUint nStride = pSrc->getTechnique_common()->getAccessor()->getStride();

        // TODO
        // Check paramater stride size is valid.
        // ex) translate -> 3 / rotate -> 1 / scale -> 3 etc...
        IZ_ASSERT(_GetStride(sAnmChannel.type) == nStride);

        IZ_UINT num = static_cast<IZ_UINT>(pSrc->getFloat_array()->getCount());

        for (IZ_UINT i = 0; i < num; i++) {
            IZ_FLOAT val = (IZ_FLOAT)pSrc->getFloat_array()->getValue().get(i);
            sAnmInput.params.push_back(val);
        }

        sAnmInput.stride = (IZ_UINT)nStride;
    }

    return IZ_TRUE;
}

IZ_BOOL CColladaAnimation::GetInterpType(
    domInputLocalRef pInput,
    SAnmChannel& sAnmChannel)
{
    domSource* pSrc = (domSource*)((domElement*)pInput->getSource().getElement());
    VRETURN(pSrc != IZ_NULL);

    domUint num = pSrc->getName_array()->getCount();

    izanagi::E_ANM_INTERP_TYPE nInterp = izanagi::E_ANM_INTERP_TYPE_NUM;

    for (domUint i = 0; i < num; i++) {
        daeString name = pSrc->getName_array()->getValue().get((size_t)i);

        if (izanagi::tool::CString::CmpStr(name, "BEZIER")) {
            // NOTE
            // I think collada bezier parameters are for hermite not bezier...
            nInterp = izanagi::E_ANM_INTERP_TYPE_HERMITE;
        }
        else {
            // TODO
            VRETURN(IZ_FALSE);
        }

        if (i > 0) {
            // Interpolation must be same.
            VRETURN(sAnmChannel.interp == nInterp);
        }

        sAnmChannel.interp = nInterp;
    }

    return IZ_TRUE;
}

namespace {
    struct SQuatParam {
        IZ_UINT idx;
        std::string nameTransform;
        IZ_FLOAT time;
        IZ_FLOAT params[4];

        SQuatParam(const std::string& str, IZ_FLOAT t)
        {
            nameTransform = str;
            time = t;
        }

        bool operator==(const SQuatParam& rhs) const
        {
            bool b0 = (nameTransform == rhs.nameTransform);
            bool b1 = (time == rhs.time);
            return (b0 && b1);
        }
    };

    struct SSortQuatParam {
        bool operator()(const SQuatParam& lhs, const SQuatParam& rhs) const
        {
            if (lhs.time == rhs.time) {
                return (lhs.idx < rhs.idx);
            }
            
            return (lhs.time < rhs.time);
        }
    };

    inline void _RotateAxis(
        izanagi::math::SMatrix44& dst,
        const izanagi::math::SMatrix44& src,
        IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z, IZ_FLOAT angle)
    {
        izanagi::math::SQuat quat;
        izanagi::math::SQuat::SetQuatFromRadAxis(quat, angle, x, y, z);

        izanagi::math::SMatrix44 mtx;
        izanagi::math::SQuat::MatrixFromQuat(mtx, quat);

        izanagi::math::SMatrix44::Mul(dst, src, mtx);
    }
}   // namespace

IZ_BOOL CColladaAnimation::CreateSlerp(IZ_UINT nNodeIdx)
{
    IZ_ASSERT(nNodeIdx < m_AnmNodes.size());
    SAnmNode& sAnmNode = m_AnmNodes[nNodeIdx];

    std::vector<SQuatParam> tvQuatList;

    std::vector<SAnmChannel>::iterator itAnmChannel = sAnmNode.channels.begin();

    for (; itAnmChannel != sAnmNode.channels.end(); ) {
        const SAnmChannel& sAnmChannel = *itAnmChannel;

        if (!(sAnmChannel.type & izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION)) {
            itAnmChannel++;
            continue;
        }

        // TODO
        IZ_ASSERT((sAnmChannel.type & izanagi::E_ANM_TRANSFORM_TYPE_W) > 0);

        const SAnmInput* pKeyTime = sAnmChannel.FindInput(E_INPUT_SEMANTIC_INPUT);
        VRETURN(pKeyTime != IZ_NULL);

        const SAnmInput* pValue = sAnmChannel.FindInput(E_INPUT_SEMANTIC_OUTPUT);
        VRETURN(pValue != IZ_NULL);
        VRETURN((pValue->params.size() / pValue->stride) == pKeyTime->params.size());

        // TODO
        IZ_ASSERT(pValue->stride == 1);

        const SJoint& sJoint = m_Joints[sAnmNode.idxJoint];

        for (size_t nKey = 0; nKey < pKeyTime->params.size(); nKey++) {
            IZ_FLOAT fKeyTime = pKeyTime->params[nKey];

            // Find target transform's param.
            std::vector<SQuatParam>::iterator itQuatParam;
            itQuatParam = std::find(
                            tvQuatList.begin(),
                            tvQuatList.end(),
                            SQuatParam(sAnmChannel.transform.c_str(), fKeyTime));

            if (itQuatParam != tvQuatList.end()) {
                // Replace parameter.
                SQuatParam& sQuatParams = *itQuatParam;
                sQuatParams.params[3] = IZ_DEG2RAD(pValue->params[nKey]);
            }
            else {
                // Create new transform's param.
                std::vector<SJointTransform>::const_reverse_iterator rit = sJoint.transforms.rbegin();

                for (; rit != sJoint.transforms.rend(); rit++) {
                    const SJointTransform& sJointTransform = *rit;

                    SQuatParam sQuatParams(sJointTransform.name, fKeyTime);

                    if (sJointTransform.type == E_TRANSFROM_TYPE_QUARTANION) {
                        VRETURN(sJointTransform.param.size() == 4);

                        sQuatParams.params[0] = sJointTransform.param[0];
                        sQuatParams.params[1] = sJointTransform.param[1];
                        sQuatParams.params[2] = sJointTransform.param[2];

                        if (sJointTransform.name == sAnmChannel.transform) {
                            sQuatParams.params[3] = IZ_DEG2RAD(pValue->params[nKey]);
                        }
                        else {
                            sQuatParams.params[3] = IZ_DEG2RAD(sJointTransform.param[3]);
                        }

                        sQuatParams.idx = static_cast<IZ_UINT>(tvQuatList.size());
                        tvQuatList.push_back(sQuatParams);
                    }
                }
            }
        }

        // Remove sAnmChannel.
        itAnmChannel = sAnmNode.channels.erase(itAnmChannel);
    }

    if (!tvQuatList.empty()) {
        // Sort by time.
        std::sort(
            tvQuatList.begin(),
            tvQuatList.end(),
            SSortQuatParam());

        SAnmChannel sAnmChannel;
        {
            sAnmChannel.type = izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION_XYZW;
            sAnmChannel.interp = izanagi::E_ANM_INTERP_TYPE_SLERP;

            // Inputs are times and quartanions.
            sAnmChannel.inputs.resize(2);

            // Get times.
            std::set<IZ_FLOAT> times;
            for (size_t i = 0; i < tvQuatList.size(); i++) {
                const SQuatParam& sQuatParam = tvQuatList[i];
                times.insert(sQuatParam.time);
            }

            SAnmInput& sAnmInput = sAnmChannel.inputs[0];
            {
                sAnmInput.semantic = E_INPUT_SEMANTIC_INPUT;
                sAnmInput.stride = 1;
            }

            for (std::set<IZ_FLOAT>::const_iterator it = times.begin(); it != times.end(); it++) {
                sAnmInput.params.push_back(*it);
            }
        }

        // For quartanion.
        SAnmInput& sAnmInput = sAnmChannel.inputs[1];
        {
            sAnmInput.semantic = E_INPUT_SEMANTIC_OUTPUT;
            sAnmInput.stride = 4;

            sAnmInput.params.reserve(4 * sAnmChannel.inputs[0].params.size());
        }

        // Compute new quartanion.
        IZ_FLOAT fPrevTime = IZ_FLOAT_MAX;

        izanagi::math::SMatrix44 mtx;

        for (size_t i = 0; i < tvQuatList.size(); i++) {
            const SQuatParam& sQuatParam = tvQuatList[i];

            IZ_FLOAT x = sQuatParam.params[0];
            IZ_FLOAT y = sQuatParam.params[1];
            IZ_FLOAT z = sQuatParam.params[2];
            IZ_FLOAT angle = sQuatParam.params[3];

            if (fPrevTime == sQuatParam.time) {
                _RotateAxis(
                    mtx, mtx,
                    x, y, z, angle);
            }
            else {
                if (i > 0) {
                    izanagi::math::SQuat quat;
                    izanagi::math::SQuat::QuatFromMatrix(quat, mtx);
                    izanagi::math::SQuat::Normalize(quat, quat);
                    sAnmInput.params.push_back(quat.x);
                    sAnmInput.params.push_back(quat.y);
                    sAnmInput.params.push_back(quat.z);
                    sAnmInput.params.push_back(quat.w);
                }

                izanagi::math::SMatrix44::SetUnit(mtx);
                _RotateAxis(
                    mtx, mtx,
                    x, y, z, angle);

                fPrevTime = sQuatParam.time;
            }
        }

        {
            izanagi::math::SQuat quat;
            izanagi::math::SQuat::QuatFromMatrix(quat, mtx);
            izanagi::math::SQuat::Normalize(quat, quat);
            sAnmInput.params.push_back(quat.x);
            sAnmInput.params.push_back(quat.y);
            sAnmInput.params.push_back(quat.z);
            sAnmInput.params.push_back(quat.w);
        }

        // Add new channel.
        sAnmNode.channels.push_back(sAnmChannel);
    }

    return IZ_TRUE;
}
