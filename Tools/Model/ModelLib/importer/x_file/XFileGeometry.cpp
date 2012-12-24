#include "XFileGeometry.h"
#include "XFileParser.h"

#include <algorithm>

//////////////////////////////////////////////////

bool CXFileGeometry::SVtx::operator ==(const CXFileGeometry::SVtx &rhs)
{
    bool ret = true;
    for (size_t i = 0; i < input.size(); i++) {
        ret = (input[i] == rhs.input[i]);
        if (!ret) {
            break;
        }
    }
    return ret;
}

////////////////////////////////////////////

CXFileGeometry* CXFileGeometry::s_pInstance = IZ_NULL;

CXFileGeometry& CXFileGeometry::GetInstance()
{
    if (s_pInstance == IZ_NULL) {
        s_pInstance = new CXFileGeometry();
    }
    return *s_pInstance;
}

void CXFileGeometry::DeleteInstance()
{
    SAFE_DELETE(s_pInstance);
}

void CXFileGeometry::Clear()
{
    m_MeshList.clear();
    m_MtrlList.clear();
    m_FaceList.clear();

    m_VtxList.clear();
    m_VtxFmtList.clear();
}

namespace {
    inline void _GetMeshes(
        SXFileNode* pNode,
        std::vector<SXFileMesh*>& tvMeshList)
    {   
        for (size_t i = 0; i < pNode->children.size(); i++) {
            SXFileNode* node = pNode->children[i];
            for (size_t n = 0; n < node->meshes.size(); n++) {
                tvMeshList.push_back(node->meshes[n]);
            }

            _GetMeshes(node, tvMeshList);
        }
    }
}   // namespace

void CXFileGeometry::Init(CXFileParser* pParser)
{
    // Classify faces by belonged material.

    SXFileNode* pRootNode = pParser->GetRootNode();

    _GetMeshes(pRootNode, m_MeshList);

    std::vector<size_t> tvMtrlIdxList;

    for (size_t i = 0; i < m_MeshList.size(); i++) {
        SXFileMesh* pMesh = m_MeshList[i];

        size_t nBaseMtrlIdx = m_MtrlList.size();
        tvMtrlIdxList.push_back(nBaseMtrlIdx);

        // Create material list.
        for (size_t n = 0; n < pMesh->mtrls.size(); n++) {
            m_MtrlList.push_back(&pMesh->mtrls[n]);
        }
    }

    m_FaceList.resize(m_MtrlList.size());

    for (size_t i = 0; i < m_MeshList.size(); i++) {
        SXFileMesh* pMesh = m_MeshList[i];
        size_t nBaseMtrlIdx = tvMtrlIdxList[i];

        for (size_t n = 0; n < pMesh->mtrl_idx.size(); n++) {
            size_t nMtrlIdx = nBaseMtrlIdx + pMesh->mtrl_idx[n];

            std::vector<SFace>& tvFaceList = m_FaceList[nMtrlIdx];

            SFace sFace;
            {
                sFace.idxMesh = static_cast<IZ_UINT>(i);
                sFace.idxMtrl = static_cast<IZ_UINT>(nMtrlIdx);
                sFace.idxFace = static_cast<IZ_UINT>(n);
            }

            tvFaceList.push_back(sFace);
        }
    }

    // Get vertex format per mesh.
    IZ_UINT nMeshNum = GetMeshNum();
    for (IZ_UINT i = 0; i < nMeshNum; i++) {
        const std::vector<SFace>& tvFaceList = m_FaceList[i];

        const SFace& sFace = tvFaceList[0];
        const SXFileMesh* pMesh = m_MeshList[sFace.idxMesh];

        IZ_ASSERT(!pMesh->pos_faces.empty());

        // Set vertex format.
        SVtxFmt sVtxFmt;
        
        sVtxFmt.type.push_back(izanagi::E_MSH_VTX_FMT_TYPE_POS);

        if (!pMesh->nml_faces.empty()) {
            sVtxFmt.type.push_back(izanagi::E_MSH_VTX_FMT_TYPE_NORMAL);
        }
        if (!pMesh->texcoords.empty()) {
            sVtxFmt.type.push_back(izanagi::E_MSH_VTX_FMT_TYPE_UV);
        }
        if (!pMesh->colors.empty()) {
            sVtxFmt.type.push_back(izanagi::E_MSH_VTX_FMT_TYPE_COLOR);
        }

        m_VtxFmtList.push_back(sVtxFmt);
    }
}

// Get triangles in specified face.
IZ_UINT CXFileGeometry::GetTriangles(
    CXFileParser* pParser,
    IZ_UINT nFaceIdx,
    std::vector<STri>& tvTriList)
{
    IZ_ASSERT(nFaceIdx < m_FaceList.size());

    // Get face list.
    const std::vector<SFace>& tvFaceList = m_FaceList[nFaceIdx];
    std::vector<SFace>::const_iterator it = tvFaceList.begin();

    for (; it != tvFaceList.end(); it++) {
        const SFace& sFace = *it;
        const SXFileMesh* pMesh = m_MeshList[sFace.idxMesh];

        VRETURN_VAL(!pMesh->pos_faces.empty(), 0);
        
        const SXFileFace& sXFileFace = pMesh->pos_faces[sFace.idxFace];
        const SXFileFace* pXFileNmlFace = (pMesh->nml_faces.empty()
                                            ? IZ_NULL
                                            : &pMesh->nml_faces[sFace.idxFace]);

        // Get count of faces.
        size_t nFaceIdxNum = sXFileFace.indices.size();
        VRETURN_VAL(nFaceIdxNum % 3 == 0, 0);

        size_t nTriNum = nFaceIdxNum / 3;

        for (IZ_UINT nTriCnt = 0; nTriCnt < nTriNum; nTriCnt += 3) {
            STri sTri;

            IZ_UINT nOffset = 0;

            for (IZ_UINT n = 0; n <3; n++) {
                // Set triangle indices.
                sTri.vtx[n] = static_cast<IZ_UINT>(m_VtxList.size());

                // Set vertex format index of vertex.
                m_VtxList.push_back(SVtx());
                SVtx& sVtx = m_VtxList.back();

                // position, uv, color
                IZ_UINT idx = sXFileFace.indices[nOffset + n];
                sVtx.input.push_back(idx);

                // normal
                if (pXFileNmlFace != IZ_NULL) {
                    idx = pXFileNmlFace->indices[nOffset + n];
                    sVtx.input.push_back(idx);
                }
            }

            nOffset += 3;

            tvTriList.push_back(sTri);
        }
    }

    IZ_UINT ret = static_cast<IZ_UINT>(m_VtxList.size());
    return ret;
}

// Returns vertex size.
IZ_UINT CXFileGeometry::GetVtxSize(IZ_UINT nMeshIdx)
{
    IZ_ASSERT(nMeshIdx < m_VtxFmtList.size());

    static izanagi::E_MSH_VTX_SIZE tblVtxSize[] = {
        izanagi::E_MSH_VTX_SIZE_POS,
        izanagi::E_MSH_VTX_SIZE_NORMAL,
        izanagi::E_MSH_VTX_SIZE_COLOR,
        izanagi::E_MSH_VTX_SIZE_UV,
        izanagi::E_MSH_VTX_SIZE_TANGENT,
        izanagi::E_MSH_VTX_SIZE_BLENDINDICES,
        izanagi::E_MSH_VTX_SIZE_BLENDWEIGHT,
    };
    IZ_C_ASSERT(COUNTOF(tblVtxSize) == izanagi::E_MSH_VTX_FMT_TYPE_NUM);

    const SVtxFmt& sVtxFmt = m_VtxFmtList[nMeshIdx];

    IZ_UINT ret = 0;

    for (size_t i = 0; i < sVtxFmt.type.size(); i++) {
        ret += tblVtxSize[sVtxFmt.type[i]];
    }

    return ret;
}

// Get vertex format.
IZ_UINT CXFileGeometry::GetVtxFmt(IZ_UINT nMeshIdx)
{
    IZ_ASSERT(nMeshIdx < m_VtxFmtList.size());

    const SVtxFmt& sVtxFmt = m_VtxFmtList[nMeshIdx];

    IZ_UINT ret = 0;

    for (size_t i = 0; i < sVtxFmt.type.size(); i++) {
        ret |= (1 << sVtxFmt.type[i]);
    }

    return ret;
}

// Get vertex paremter by specifed type.
//  ex) If specfied type is E_MSH_VTX_FMT_TYPE_NORMAL, so get vertex normal.
IZ_BOOL CXFileGeometry::GetVertex(
    IZ_UINT nFaceIdx,
    IZ_UINT nVtxIdx,
    izanagi::math::SVector& vec,
    izanagi::E_MSH_VTX_FMT_TYPE type)
{
    IZ_ASSERT(nFaceIdx < m_FaceList.size());
    IZ_ASSERT(nVtxIdx < m_VtxList.size());

    const SVtxFmt& sVtxFmt = m_VtxFmtList[nFaceIdx];

    std::vector<izanagi::E_MSH_VTX_FMT_TYPE>::const_iterator itVtxFmt = std::find(
                                                                            sVtxFmt.type.begin(),
                                                                            sVtxFmt.type.end(),
                                                                            type);

    if (itVtxFmt == sVtxFmt.type.end()) {
        return IZ_FALSE;
    }

    const SXFileMesh* pMesh = GetMesh(nFaceIdx);

    const SVtx& sVtx = m_VtxList[nVtxIdx];

    if (type == izanagi::E_MSH_VTX_FMT_TYPE_POS) {
        IZ_UINT idx = sVtx.input[0];
        izanagi::math::SVector::Copy(vec, pMesh->positions[idx]);
    }
    else if (type == izanagi::E_MSH_VTX_FMT_TYPE_NORMAL) {
        IZ_UINT idx = sVtx.input[1];
        izanagi::math::SVector::Copy(vec, pMesh->normals[idx]);
    }
    else if (type == izanagi::E_MSH_VTX_FMT_TYPE_COLOR) {
        IZ_UINT idx = sVtx.input[0];
        IZ_UINT nColor = pMesh->colors[idx];

        izanagi::CColor cColor(nColor);
        IZ_UINT8 r = cColor.r;
        IZ_UINT8 g = cColor.g;
        IZ_UINT8 b = cColor.b;
        IZ_UINT8 a = cColor.a;

        vec.x = r;
        vec.y = g;
        vec.z = b;
        vec.w = a;
    }
    else if (type == izanagi::E_MSH_VTX_FMT_TYPE_UV) {
        IZ_UINT idx = sVtx.input[0];
        const SXFileUV& texcoord = pMesh->texcoords[idx];
        vec.x = texcoord.u;
        vec.y = texcoord.v;
    }
    else {
        // TODO
        VRETURN(IZ_FALSE);
    }

    return IZ_TRUE;
}

namespace {
    inline IZ_INT _GetNodeIdxByName(
        const std::vector<SXFileNode*>& tvNodeList,
        const std::string& name)
    {
        for (size_t i = 0; i < tvNodeList.size(); i++) {
            const SXFileNode* pNode = tvNodeList[i];
            if (pNode->name.compare(name) == 0) {
                return static_cast<IZ_UINT>(i);
            }
        }
        return -1;
    }
}   // namespace

void CXFileGeometry::GetSkinList(
    CXFileParser* pParser,
    IZ_UINT nFaceIdx,
    std::vector<SSkin>& tvSkinList)
{
    const std::vector<SXFileNode*>& tvNodeList = pParser->GetNodeList();

    if (pParser->HasSkin()) {
        const SXFileMesh* pMesh = GetMesh(nFaceIdx);

        if (pMesh->skins.empty()) {
            return;
        }

        tvSkinList.resize(pMesh->positions.size());

        for (size_t b = 0; b < pMesh->skins.size(); b++) {
            const SXFileSkin& sXFileSkin = pMesh->skins[b];

            for (size_t i = 0; i < sXFileSkin.vertex.size(); i++) {
                const IZ_UINT nVtxIdx = sXFileSkin.vertex[i];
                IZ_ASSERT(nVtxIdx < tvSkinList.size());

                SSkin& sSkin = tvSkinList[nVtxIdx];
                {
                    IZ_INT nJointIdx = _GetNodeIdxByName(tvNodeList, sXFileSkin.bone.name);
                    if (nJointIdx >= 0) {
                        sSkin.joint.push_back(nJointIdx);
                        sSkin.weight.push_back(sXFileSkin.weight[i]);
                    }
                }
            }
        }
    }
    else {
        // There are joints, but not skins...
        tvSkinList.resize(tvNodeList.size());

        for (size_t i = 0; i < tvSkinList.size(); i++) {
            SSkin& sSkin = tvSkinList[i];
            sSkin.Add((IZ_UINT)i, 1.0f);
        }
    }
}

IZ_UINT CXFileGeometry::GetSkinIdx(
    CXFileParser* pParser,
    IZ_UINT nFaceIdx,
    IZ_UINT nVtxIdx)
{
    // NOTE
    // Get skin information index from vertex index.

    IZ_INT ret = -1;

    const std::vector<SXFileNode*>& tvNodeList = pParser->GetNodeList();

    const SXFileMesh* pMesh = GetMesh(nFaceIdx);

    if (pParser->HasSkin()) {
#if 0
        for (size_t i = 0; i < pMesh->skins.size(); i++) {
            const SXFileSkin& sBone = pMesh->skins[i];

            std::vector<IZ_UINT>::const_iterator it = std::find(
                                                        sBone.vertex.begin(),
                                                        sBone.vertex.end(),
                                                        nVtxIdx);
            if (it != sBone.vertex.end()) {
                ret = sBone.idx;
                break;
            }
        }
#else
        const SVtx& sVtx = m_VtxList[nVtxIdx];
        IZ_UINT nRealVtxIdx = sVtx.input[0];

        return nRealVtxIdx;
#endif
    }
    else {
        // TODO
        // Node index = Skin index...

        std::vector<SXFileNode*>::const_iterator it = std::find(
                                                        tvNodeList.begin(),
                                                        tvNodeList.end(),
                                                        pMesh->node);
        IZ_ASSERT(it != tvNodeList.end());

        ret = (IZ_UINT)std::distance(tvNodeList.begin(), it);
    }

    IZ_ASSERT(ret >= 0);
    
    return ret;
}

void CXFileGeometry::GetMaterial(
    IZ_UINT nIdx,
    izanagi::S_MSH_MTRL& sMtrl)
{
    IZ_ASSERT(nIdx < (IZ_UINT)m_MtrlList.size());

    const SXFileMaterial* pXFileMtrl = m_MtrlList[nIdx];

    sMtrl.name.SetString(pXFileMtrl->name.c_str());
    sMtrl.nameKey = sMtrl.name.GetKeyValue();
}

const SXFileMesh* CXFileGeometry::GetMesh(IZ_UINT nFaceIdx)
{
    std::vector<SFace>& tvFaceList = m_FaceList[nFaceIdx];

    const SFace& sFace = tvFaceList[0];

    // NOTE
    // All faces has same mesh index.

    // Get mesh.
    const SXFileMesh* pMesh = m_MeshList[sFace.idxMesh];

    return pMesh;
}
