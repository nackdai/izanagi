#include "GeometryChunk.h"

#include "NvTriStrip.h"
#include "izToolKit.h"

#include <algorithm>

CGeometryChunk CGeometryChunk::s_cInstance;

// ジオメトリチャンク
// +------------------------+
// |     チャンクヘッダ     |
// +------------------------+
// |   ジオメトリチャンク   |
// |         ヘッダ         |
// +------------------------+
// |   頂点データテーブル   |
// | +--------------------+ |
// | |      ヘッダ        | |
// | +--------------------+ |
// | |     頂点データ     | |
// | +--------------------+ |
// |         ・・・         |
// |                        |
// |    メッシュテーブル    |
// | +--------------------+ |
// | |      メッシュ      | |
// | |+------------------+| |
// | ||     ヘッダ       || |
// | |+------------------+| |
// | |                    | |
// | |     サブセット     | |
// | |+------------------+| |
// | ||     ヘッダ       || |
// | |+------------------+| |
// | ||インデックスデータ|| |
// | |+------------------+| |
// | |      ・・・        | |
// | +--------------------+ |
// |        ・・・          |
// +------------------------+

IZ_BOOL CGeometryChunk::Export(
    IZ_UINT maxJointMtxNum,
    izanagi::IOutputStream* pOut,
    IImporter* pImporter)
{
    // メッシュが影響を受けるマトリクスの最大数
    m_MaxJointMtxNum = max(
                        maxJointMtxNum,
                        izanagi::MSH_BELONGED_JOINT_MIN);
    m_MaxJointMtxNum = ((m_MaxJointMtxNum & 0x01) == 1
                        ? m_MaxJointMtxNum + 1
                        : m_MaxJointMtxNum);

    
    {
        FILL_ZERO(&m_Header, sizeof(m_Header));

        // TODO
        // version, magic number...

        m_Header.sizeHeader = sizeof(m_Header);
    }

    // Blank for S_MSH_HEADER.
    izanagi::tool::CIoStreamSeekHelper cSeekHelper(pOut);
    VRETURN(cSeekHelper.Skip(sizeof(m_Header)));

    // TODO
    // Export mesh groups.
    VRETURN(ExportGroup(pOut, pImporter));

    m_Header.sizeFile = pOut->GetCurPos();

    m_Header.minVtx[0] = m_vMin.x;
    m_Header.minVtx[1] = m_vMin.y;
    m_Header.minVtx[2] = m_vMin.z;

    m_Header.maxVtx[0] = m_vMin.x;
    m_Header.maxVtx[1] = m_vMin.y;
    m_Header.maxVtx[2] = m_vMin.z;

    // TODO
    m_Header.numMeshGroup = 1;

    // Export S_MSH_HEADER.
    {
        // Rmenber end of geometry chunk.
        VRETURN(cSeekHelper.ReturnWithAnchor());

        IZ_OUTPUT_WRITE_VRETURN(pOut, &m_Header, 0, sizeof(m_Header));
        cSeekHelper.Step(sizeof(m_Header));

        // Return end of geometry chunk.
        VRETURN(cSeekHelper.ReturnToAnchor());
    }

    pImporter->ExportGeometryCompleted();

    return IZ_TRUE;
}

void CGeometryChunk::Clear()
{
    m_MeshList.clear();
    m_TriList.clear();
    m_SkinList.clear();
    m_VtxList.clear();
}

// メッシュグループ出力
IZ_BOOL CGeometryChunk::ExportGroup(
    izanagi::IOutputStream* pOut,
    IImporter* pImporter)
{
    izanagi::S_MSH_MESH_GROUP sGroupInfo;
    {
        FILL_ZERO(&sGroupInfo, sizeof(sGroupInfo));

        // メッシュグループに所属するメッシュセット数
        sGroupInfo.numMeshSet = pImporter->GetMeshNum();
    }

    IZ_UINT nChunkStartPos = pOut->GetCurPos();

    // Blank for S_MSH_MESH_GROUP.
    izanagi::tool::CIoStreamSeekHelper cSeekHelper(pOut);
    VRETURN(cSeekHelper.Skip(sizeof(sGroupInfo)));

    // メッシュセットリスト
    m_MeshList.resize(sGroupInfo.numMeshSet);

    IZ_UINT nVtxNum = 0;

#if 0
    // 三角形リストの保持先を覚えさせておく
    SPrimSet::SetTriList(&m_TriList);
#endif

    for (IZ_UINT nMeshIdx = 0; nMeshIdx < sGroupInfo.numMeshSet; nMeshIdx++) {
        SMesh& sMesh = m_MeshList[nMeshIdx];

        pImporter->BeginMesh(nMeshIdx);

        // メッシュセットに含まれる三角形の開始位置
        sMesh.startTri = static_cast<IZ_UINT>(m_TriList.size());

        // Get triangels.
        nVtxNum += pImporter->GetTriangles(m_TriList);

        // メッシュセットに含まれる三角形の終了位置
        sMesh.endTri = static_cast<IZ_UINT>(m_TriList.size());

        // Get skin data.
        pImporter->GetSkinList(m_SkinList);

        // Bind joint to triangle.
        BindJointToTri(pImporter, sMesh);

        // Classify triangles by joint
        ClassifyTriByJoint(sMesh);

        GetMeshInfo(pImporter, sMesh);

        pImporter->EndMesh();
    }

    m_VtxList.resize(nVtxNum);

    // Compute Normal, Tangent, etc...
    ComputeVtxParemters(pImporter);

    // Export vertices.
    sGroupInfo.numVB = ExportVertices(
                        pOut,
                        pImporter);
    VRETURN(sGroupInfo.numVB > 0);

    // Export meshes.
    VRETURN(
        ExportMesh(
            pOut,
            pImporter));

    m_Header.numVB += sGroupInfo.numVB;
    m_Header.numMeshSet += sGroupInfo.numMeshSet;

    // Export S_MSH_MESH_GROUP.
    {
        // Rmenber end of geometry chunk.
        VRETURN(cSeekHelper.ReturnWithAnchor());

        IZ_OUTPUT_WRITE_VRETURN(pOut, &sGroupInfo, 0, sizeof(sGroupInfo));
        cSeekHelper.Step(sizeof(sGroupInfo));

        // Return end of geometry chunk.
        VRETURN(cSeekHelper.ReturnToAnchor());
    }

    pImporter->ExportGeometryCompleted();

    return IZ_TRUE;
}

namespace {
    // 関節情報
    struct SJointInfo {
        // 関節インデックス
        IZ_UINT idx;

        // 指定された関節におけるスキニングウエイト最大値
        IZ_FLOAT maxWeight;

        SJointInfo()
        {
            maxWeight = IZ_FLOAT_MIN;
        }

        void SetWeight(IZ_FLOAT weight)
        {
            maxWeight = (maxWeight < weight ? weight : maxWeight);
        }

        bool operator==(IZ_UINT i)
        {
            return (idx == i);
        }

        // ソート用
        bool operator<(const SJointInfo& rhs) const
        {
            return (maxWeight < rhs.maxWeight);
        }
    };
}   // namespace

// 指定されたメッシュについて三角形と関節を関連付ける
void CGeometryChunk::BindJointToTri(
    IImporter* pImporter,
    SMesh& sMesh)
{
    if (m_SkinList.empty()) {
        // There is no joints, so nothing is done.
        return;
    }

    // 関節情報の一時保存用のリスト
    std::vector<SJointInfo> tvJointInfo;

    for (IZ_UINT nTriPos = sMesh.startTri; nTriPos < sMesh.endTri; nTriPos++) {
        // Get triangle belonged to specified mesh.
        STri& sTri = m_TriList[nTriPos];

        for (IZ_UINT i = 0; i < 3; i++) {
            // 三角形が持つ頂点に関連したスキニング情報へのインデックスを取得
            IZ_UINT nSkinIdx = pImporter->GetSkinIdxAffectToVtx(sTri.vtx[i]);

            // Get skin information.
            const SSkin& sSkin = m_SkinList[nSkinIdx];

            for (size_t n = 0; n < sSkin.joint.size(); n++) {
                // 関節インデックス取得
                IZ_UINT idx = sSkin.joint[n];

                // // 三角形に影響を与える関節インデックスが設定されているか調べる

                if (std::find(sTri.joint.begin(), sTri.joint.end(), idx) == sTri.joint.end()) {
                    // 未設定

                    // 三角形に影響を与える関節インデックスを登録
                    sTri.joint.insert(idx);

                    // 関節情報を登録
                    tvJointInfo.push_back(SJointInfo());
                    SJointInfo& sJointInfo = tvJointInfo.back();
                    {
                        sJointInfo.idx = idx;
                        sJointInfo.SetWeight(sSkin.weight[n]);
                    }
                }
                else {
                    // 設定済み

                    // 三角形に影響を与える関節情報を取得
                    // triangle belongs to joint.
                    std::vector<SJointInfo>::iterator itJointInfo = std::find(
                                                                        tvJointInfo.begin(),
                                                                        tvJointInfo.end(),
                                                                        idx);
                    itJointInfo->SetWeight(sSkin.weight[n]);
                }
            }
        }

        // Sort in order in which weight value is small.
        std::sort(tvJointInfo.begin(), tvJointInfo.end());

        IZ_ASSERT(sTri.joint.size() > 0);
        
        // 三角形に影響を与える関節数は最大４まで
        // それを超えた場合は一番影響が小さいもの（ウエイトが小さいもの）を削除する
        // If num of skin is over 4, num of skin is limited to 4 by weight.
        if (sTri.joint.size() > 4) {
            // 先頭の関節情報を取得
            // ここでは既にウエイトが小さいものの順にソートされているので
            // 一番影響が小さい関節の情報を取得
            std::vector<SJointInfo>::iterator itJointInfo = tvJointInfo.begin();

            // 三角形に影響を与える関節数が４以下になるまで削除を繰り返す
            for (size_t n = sTri.joint.size(); n > 4; itJointInfo++, n--) {
                const SJointInfo& sJointInfo = *itJointInfo;

                // Delete joint which has smallest weight value.
                sTri.EraseJoint(sJointInfo.idx);

                for (IZ_UINT i = 0; i < 3; i++) {
                    // 指定された頂点に影響を与えるスキニング情報を取得.
                    IZ_UINT nSkinIdx = pImporter->GetSkinIdxAffectToVtx(sTri.vtx[i]);

                    // スキニング情報からも関節を削除
                    SSkin& sSkin = m_SkinList[nSkinIdx];
                    if (sSkin.EraseJoint(sJointInfo.idx)) {
                        // ウエイト値の合計が１になるように正規化する
                        sSkin.Normalize();
                    }
                }
            }
        }

        IZ_ASSERT(sTri.joint.size() <= 4);

        tvJointInfo.clear();
    }
}

namespace {
    // 基準となるプリミティブセットとのマージ候補を探すための関数オブジェクト
    struct FuncFindIncludedJointIdx {
        // マージ候補のプリミティブセット
        // 基準となっているプリミティブセットと一致する関節数ごとに候補を登録する
        static std::vector< std::vector<const SPrimSet*> > candidateList;

        // 基準となっているプリミティブセット
        const SPrimSet& master;

        // 最大関節数
        const IZ_UINT maxJointMtxNum;

        FuncFindIncludedJointIdx(const SPrimSet& prim, IZ_UINT num)
            : master(prim),
              maxJointMtxNum(num)
        {}

        ~FuncFindIncludedJointIdx() {}

        void operator()(const SPrimSet& rhs)
        {
            if (&master == &rhs) {
                // 同じものは無視する
                return;
            }

            // 所属関節群がどれだけ一致するか調べる

            IZ_UINT nMatchCnt = 0;

            std::set<IZ_UINT>::const_iterator it = master.joint.begin();
            for (; it != master.joint.end(); it++) {
                IZ_UINT jointIdx = *it; 

                std::set<IZ_UINT>::const_iterator itRhs = rhs.joint.begin();
                for (; itRhs != rhs.joint.end(); itRhs++) {
                    IZ_UINT rhsJointIdx = *itRhs;

                    if (jointIdx == rhsJointIdx) {
                        nMatchCnt++;
                    }
                }
            }

            // 一致する関節があった
            if (nMatchCnt > 0) {
                // 一致する関節を持つプリミティブセットを登録する
                // しかし、そこで関節の最大数を超えてはまずいのでチェックする

                // 一致しない関節の数が新たに増える関節数になるので
                // 一致しない関節数を計算する
                IZ_UINT added = (IZ_UINT)rhs.joint.size() - nMatchCnt;

                if (added + master.joint.size() <= maxJointMtxNum) {
                    // 新たに登録しても最大数は超えないので登録する
                    candidateList[nMatchCnt - 1].push_back(&rhs);
                }
            }
        }
    };

    std::vector< std::vector<const SPrimSet*> > FuncFindIncludedJointIdx::candidateList;
}   // namespace

// 三角形に影響を与える関節に応じて三角形を分類する
void CGeometryChunk::ClassifyTriByJoint(SMesh& sMesh)
{
    for (IZ_UINT nTriPos = sMesh.startTri; nTriPos < sMesh.endTri; nTriPos++) {
        STri& sTri = m_TriList[nTriPos];

        // 所属する関節群からキー値を計算する
        IZ_UINT nKey = sTri.ComputeKey();

        // 自分の所属する可能性のあるプリミティブセットを探す
        std::vector<SPrimSet>::iterator itSubset = std::find(
                                                        sMesh.subset.begin(),
                                                        sMesh.subset.end(),
                                                        nKey);
        if (itSubset == sMesh.subset.end()) {
            // 見つからなかった
            // 見つからなかったということは初出の関節群なので新規にプリミティブセットを作る
            sMesh.subset.push_back(SPrimSet());
            SPrimSet& sPrimSet = sMesh.subset.back();
            {
                // 関節群から計算されたキー値
                sPrimSet.key = nKey;

                // 三角形を登録
                sPrimSet.tri.push_back(nTriPos);

                // 所属関節インデックスを登録
                std::set<IZ_UINT>::const_iterator it = sTri.joint.begin();
                for (; it != sTri.joint.end(); it++) {
                    sPrimSet.joint.insert(*it);
                }
            }
        }
        else {
            // 見つかったので三角形を登録
            SPrimSet& sPrimSet = *itSubset;
            sPrimSet.tri.push_back(nTriPos);
        }
    }

#if 0
    // Merge triangles by joint idx.
    if (sMesh.subset.size() > 1) {
        std::vector<SPrimSet>::iterator it = sMesh.subset.begin();

        for (; it != sMesh.subset.end(); ) {
            SPrimSet& sPrimSet = *it;

            IZ_BOOL bIsAdvanceIter = IZ_TRUE;

            // NOTE
            // SPrimSet::tri : SPrimSetを構成する三角形情報へのインデックス
            // SPrimSetを構成する三角形 -> 同じ関節群（スキン計算を行うための関節）に所属する三角形
            // そのため、関節情報についてはどれでも共通なので、sPrimSet.tri[0] を持ってくればいい

            IZ_UINT nTriIdx = sPrimSet.tri[0];
            const STri& sTri = m_TriList[nTriIdx];

            if (sTri.GetJointNum() < 4) {
                // sMeshGroup の持つJointIdxを全て内包するMeshGroupを探す
                std::vector<SPrimSet>::iterator itFind = std::find(
                                                                sMesh.subset.begin(),
                                                                sMesh.subset.end(),
                                                                sPrimSet);
                if (itFind != sMesh.subset.end()) {
                    SPrimSet& sSubsetMatch = *itFind;

                    // Merge to found MeshGroup.
                    sSubsetMatch.tri.insert(
                        sSubsetMatch.tri.end(),
                        sPrimSet.tri.begin(),
                        sPrimSet.tri.end());

                    // Register joint indices.
                    std::set<IZ_UINT>::const_iterator itJoint = sTri.joint.begin();
                    for (; itJoint != sTri.joint.end(); itJoint++) {
                        sSubsetMatch.joint.insert(*itJoint);
                    }

                    // Delete the MeshGroup because of merging to another MeshGroup.
                    it = sMesh.subset.erase(it);
                    bIsAdvanceIter = IZ_FALSE;
                }
            }

            if (bIsAdvanceIter) {
                it++;
            }
        }
    }
#else
    // 影響を受ける関節が同じ三角形をまとめる
    // Merge triangles by joint idx.
    if (sMesh.subset.size() > 1) {
        // 候補リストを最大関節数だけ確保
        FuncFindIncludedJointIdx::candidateList.resize(m_MaxJointMtxNum);

        std::vector<SPrimSet>::iterator it = sMesh.subset.begin();

        for (; it != sMesh.subset.end(); ) {
            SPrimSet& sPrimSet = *it;
            IZ_UINT masterKey = sPrimSet.key;

            IZ_BOOL erased = IZ_FALSE;

            if (sPrimSet.joint.size() < m_MaxJointMtxNum) {
                // マージ候補を探す
                std::for_each(
                    sMesh.subset.begin(),
                    sMesh.subset.end(),
                    FuncFindIncludedJointIdx(sPrimSet, m_MaxJointMtxNum));

                std::vector<IZ_UINT> releaseList;

                // 一致した関節数ごとに処理を行う

                for (IZ_INT i = m_MaxJointMtxNum - 1; i >= 0; i--) {
                    std::vector<const SPrimSet*>::iterator candidate = FuncFindIncludedJointIdx::candidateList[i].begin();

                    for (; candidate != FuncFindIncludedJointIdx::candidateList[i].end(); candidate++) {
                        const SPrimSet& prim = *(*candidate);

                        // マージする
                        SPrimSet& sSubsetMatch = const_cast<SPrimSet&>(prim);

                        if (sPrimSet.joint.size() + sSubsetMatch.joint.size() >= m_MaxJointMtxNum) {
                            // もう入らない可能性が高いので終了
                            break;
                        }

                        // ベースとなるプリミティブセットにマージ
                        // プリミティブセットを構成する三角形をベースとなるプリミティブセットに登録
                        sPrimSet.tri.insert(
                            sPrimSet.tri.end(),
                            sSubsetMatch.tri.begin(),
                            sSubsetMatch.tri.end());

                        // Register joint indices.
                        std::set<IZ_UINT>::const_iterator itJoint = sSubsetMatch.joint.begin();
                        for (; itJoint != sSubsetMatch.joint.end(); itJoint++) {
                            sPrimSet.joint.insert(*itJoint);
                        }

                        // 削除候補を登録
                        releaseList.push_back(prim.key);
                    }

                    FuncFindIncludedJointIdx::candidateList[i].clear();
                }

                // マージしたので消す
                for (size_t n = 0; n < releaseList.size(); n++) {
                    IZ_UINT key = releaseList[n];

                    std::vector<SPrimSet>::iterator itFind = std::find(
                                                                sMesh.subset.begin(),
                                                                sMesh.subset.end(),
                                                                key);
                    sMesh.subset.erase(itFind);
                    erased = IZ_TRUE;
                }
            }

            if (erased) {
                it = std::find(
                        sMesh.subset.begin(),
                        sMesh.subset.end(),
                        masterKey);
                IZ_ASSERT(it != sMesh.subset.end());
                it++;
            }
            else {
                it++;
            }
        }
    }
#endif
}

// メッシュ情報を取得
void CGeometryChunk::GetMeshInfo(
    IImporter* pImporter,
    SMesh& sMesh)
{
    sMesh.fmt = pImporter->GetVtxFmt();
    sMesh.sizeVtx = pImporter->GetVtxSize();

    // For skin.
    if (!m_SkinList.empty()) {
        sMesh.fmt |= (1 << izanagi::E_MSH_VTX_FMT_TYPE_BLENDWEIGHT);
        sMesh.fmt |= (1 << izanagi::E_MSH_VTX_FMT_TYPE_BLENDINDICES);

        sMesh.sizeVtx += izanagi::E_MSH_VTX_SIZE_BLENDINDICES;
        sMesh.sizeVtx += izanagi::E_MSH_VTX_SIZE_BLENDWEIGHT;
    }
}

IZ_BOOL CGeometryChunk::ComputeVtxNormal(
    IImporter* pImporter,
    const STri& sTri)
{
    izanagi::math::SVector4 vecPos[3];

    for (IZ_UINT i = 0; i < 3; i++) {
        IZ_UINT nVtxIdx = sTri.vtx[i];

        IZ_BOOL result = pImporter->GetVertex(
                            nVtxIdx,
                            vecPos[i],
                            izanagi::E_MSH_VTX_FMT_TYPE_POS);
        VRETURN(result);
    }

    // NOTE
    // Counter Clock Wise
    //     1
    //   /   \
    //  /     \
    // 0 ----- 2

    for (IZ_UINT i = 0; i < 3; i++) {
        IZ_UINT nVtxIdx = sTri.vtx[i];

        IZ_UINT nIdx_0 = i;
        IZ_UINT nIdx_1 = (i + 1) % 3;
        IZ_UINT nIdx_2 = (i + 2) % 3;

        izanagi::math::SVector4 vP;
        izanagi::math::SVector4::SubXYZ(vP, vecPos[nIdx_1], vecPos[nIdx_0]);

        izanagi::math::SVector4 vQ;
        izanagi::math::SVector4::SubXYZ(vQ, vecPos[nIdx_2], vecPos[nIdx_0]);

        izanagi::math::SVector4 nml;
        izanagi::math::SVector4::Cross(nml, vP, vQ);

        SVtxAdditional& sVtx = m_VtxList[nVtxIdx];
        sVtx.nml.push_back(nml);
    }

    return IZ_TRUE;
}

#ifndef RETURN
    #define RETURN(b)   if (!(b)) { return IZ_FALSE; }
#endif  // #ifndef RETURN

IZ_BOOL CGeometryChunk::ComputeVtxTangent(
    IImporter* pImporter,
    const STri& sTri)
{
    izanagi::math::SVector4 vecPos[3];
    izanagi::math::SVector4 vecUV[3];
    izanagi::math::SVector4 vecNml[3];

    for (IZ_UINT i = 0; i < 3; i++) {
        IZ_UINT nVtxIdx = sTri.vtx[i];

        // Get position.
        IZ_BOOL result = pImporter->GetVertex(
                            nVtxIdx,
                            vecPos[i],
                            izanagi::E_MSH_VTX_FMT_TYPE_POS);
        VRETURN(result);

        // Get texture coordinate.
        result = pImporter->GetVertex(
                    nVtxIdx,
                    vecUV[i],
                    izanagi::E_MSH_VTX_FMT_TYPE_UV);
        RETURN(result);

        // Get normal.
        result = pImporter->GetVertex(
                    nVtxIdx,
                    vecNml[i],
                    izanagi::E_MSH_VTX_FMT_TYPE_NORMAL);
        if (!result) {
            // If mesh don't have normal, get normal from computed normal.
            RETURN(m_VtxList.size() > nVtxIdx);

            const SVtxAdditional& sVtx = m_VtxList[nVtxIdx];
            sVtx.GetNormal(vecNml[i]);
        }
    }

    // NOTE
    // Counter Clock Wise
    //     1
    //   /   \
    //  /     \
    // 0 ----- 2

    IZ_FLOAT fCoeff[4];

    for (IZ_UINT i = 0; i < 3; i++) {
        IZ_UINT nVtxIdx = sTri.vtx[i];

        IZ_UINT nIdx_0 = i;
        IZ_UINT nIdx_1 = (i + 1) % 3;
        IZ_UINT nIdx_2 = (i + 2) % 3;

        izanagi::math::SVector4 vP;
        izanagi::math::SVector4::SubXYZ(vP, vecPos[nIdx_1], vecPos[nIdx_0]);

        izanagi::math::SVector4 vQ;
        izanagi::math::SVector4::SubXYZ(vQ, vecPos[nIdx_2], vecPos[nIdx_0]);

        fCoeff[0] = vecUV[nIdx_2].v[1] - vecUV[nIdx_0].v[1];
        fCoeff[1] = -(vecUV[nIdx_1].v[1] - vecUV[nIdx_0].v[1]);
        fCoeff[2] = -(vecUV[nIdx_2].v[0] - vecUV[nIdx_0].v[0]);
        fCoeff[3] = vecUV[nIdx_1].v[0] - vecUV[nIdx_0].v[0];

        IZ_FLOAT fInvDeterminant = 1.0f / (fCoeff[3] * fCoeff[0] - fCoeff[2] * fCoeff[1]);

        // BiNormal
        izanagi::math::SVector4 vB;
        {
            izanagi::math::SVector4::Scale(vP, vP, fInvDeterminant * fCoeff[2]);
            izanagi::math::SVector4::Scale(vQ, vQ, fInvDeterminant * fCoeff[3]);
            izanagi::math::SVector4::Add(vB, vP, vQ);
            izanagi::math::SVector4::Normalize(vB, vB);
        }

        // Tangent
        izanagi::math::SVector4 vT;
        {
            // X(T) = Y(B) x Z(N)
            izanagi::math::SVector4::Cross(vT, vB, vecNml[nIdx_0]);
        }

        SVtxAdditional& sVtx = m_VtxList[nVtxIdx];
        sVtx.tangent.push_back(vT);
    }

    return IZ_TRUE;
}

void CGeometryChunk::ComputeVtxParemters(IImporter* pImporter)
{
    // TODO
}

// 頂点データを出力
IZ_UINT CGeometryChunk::ExportVertices(
    izanagi::IOutputStream* pOut,
    IImporter* pImporter)
{
    izanagi::tool::CIoStreamSeekHelper cSeekHelper(pOut);

    IZ_UINT nVBCnt = 0;
    IZ_UINT nPrevFmt = 0;

#if 0
    izanagi::S_MSH_VERTICES sVtxInfo;
    FILL_ZERO(&sVtxInfo, sizeof(sVtxInfo));
#endif

    for (size_t i = 0; i < m_MeshList.size(); i++) {
        SMesh& sMesh = m_MeshList[i];

        pImporter->BeginMesh((IZ_UINT)i);

#if 0
        // 頂点数予測
        IZ_UINT nTriNum = sMesh.endTri - sMesh.startTri;
        IZ_UINT nVtxNum = nTriNum * 3;

        IZ_UINT nCurVtxNum = (IZ_UINT)m_ExportedVtx.size();

        IZ_BOOL bIsNewVB = ((nPrevFmt != sMesh.fmt)
                            || (nCurVtxNum + nVtxNum > IZ_UINT16_MAX));

        if (bIsNewVB) {
            if (m_ExportedVtx.size() > 0) {
                // Return to position of expoting S_MSH_VERTICES.
                VRETURN(cSeekHelper.ReturnWithAnchor());

                sVtxInfo.numVtx = (IZ_UINT16)m_ExportedVtx.size();

                // Export S_MSH_VERTICES.
                IZ_OUTPUT_WRITE_VRETURN(pOut, &sVtxInfo, 0, sizeof(sVtxInfo));

                VRETURN(cSeekHelper.ReturnToAnchor());

                nVBCnt++;
            }

            // Blank S_MSH_VERTICES. 
            VRETURN_VAL(cSeekHelper.Skip(sizeof(sVtxInfo)), 0);

            FILL_ZERO(&sVtxInfo, sizeof(sVtxInfo));
            sVtxInfo.sizeVtx = sMesh.sizeVtx;

            m_ExportedVtx.clear();
            m_ExportedVtx.reserve(nVtxNum);

            nPrevFmt = sMesh.fmt;
        }

        for (size_t n = 0; n < sMesh.subset.size(); n++) {
            SPrimSet& sPrimSet = sMesh.subset[n];

            VRETURN_VAL(
                ExportVertices(
                    pOut,
                    pImporter,
                    sMesh,
                    sPrimSet), 0);

            sPrimSet.idxVB = nVBCnt;
        }
#else
        for (size_t n = 0; n < sMesh.subset.size(); n++) {
            SPrimSet& sPrimSet = sMesh.subset[n];

            izanagi::S_MSH_VERTICES sVtxInfo;
            sVtxInfo.sizeVtx = sMesh.sizeVtx;

            // Blank S_MSH_VERTICES. 
            VRETURN_VAL(cSeekHelper.Skip(sizeof(sVtxInfo)), 0);

            VRETURN_VAL(
                ExportVertices(
                    pOut,
                    pImporter,
                    sMesh,
                    sPrimSet), 0);

            // Return to position of expoting S_MSH_VERTICES.
            VRETURN(cSeekHelper.ReturnWithAnchor());

            sVtxInfo.numVtx = (IZ_UINT16)m_ExportedVtx.size();

            // Export S_MSH_VERTICES.
            IZ_OUTPUT_WRITE_VRETURN(pOut, &sVtxInfo, 0, sizeof(sVtxInfo));

            VRETURN(cSeekHelper.ReturnToAnchor());

            sPrimSet.idxVB = nVBCnt;

            nVBCnt++;
            m_ExportedVtx.clear();
        }
#endif

        pImporter->EndMesh();
    }

#if 0
    if (m_ExportedVtx.size() > 0) {
        // Return to position of expoting S_MSH_VERTICES.
        VRETURN(cSeekHelper.ReturnWithAnchor());

        sVtxInfo.numVtx = (IZ_UINT16)m_ExportedVtx.size();

        // Export S_MSH_VERTICES.
        IZ_OUTPUT_WRITE_VRETURN(pOut, &sVtxInfo, 0, sizeof(sVtxInfo));

        VRETURN(cSeekHelper.ReturnToAnchor());

        nVBCnt++;
    }
#endif

    m_ExportedVtx.clear();

    return nVBCnt;
}

namespace {
    // 指定された関節リストにおける関節インデックスの格納位置を探す
    inline IZ_INT _FindJointIdx(
        const std::set<IZ_UINT>& tsJoint,
        IZ_UINT nJointIdx)
    {
        std::set<IZ_UINT>::const_iterator it = tsJoint.begin();

        for (IZ_UINT pos = 0; it != tsJoint.end(); it++, pos++) {
            IZ_UINT idx = *it;

            if (idx == nJointIdx) {
                return pos;
            }
        }

        IZ_ASSERT(IZ_FALSE);
        return -1;
    }
}   // namespace

// 頂点データを出力.
IZ_BOOL CGeometryChunk::ExportVertices(
    izanagi::IOutputStream* pOut,
    IImporter* pImporter,
    const SMesh& sMesh,
    SPrimSet& sPrimSet)
{
    // 頂点データサイズのテーブル
    static IZ_UINT tblVtxSize[] = {
        izanagi::E_MSH_VTX_SIZE_POS,
        izanagi::E_MSH_VTX_SIZE_NORMAL, 
        izanagi::E_MSH_VTX_SIZE_COLOR,
        izanagi::E_MSH_VTX_SIZE_UV,
        izanagi::E_MSH_VTX_SIZE_TANGENT,
    };

    m_vMin.Set(IZ_FLOAT_MAX, IZ_FLOAT_MAX, IZ_FLOAT_MAX);
    m_vMax.Set(IZ_FLOAT_MIN, IZ_FLOAT_MIN, IZ_FLOAT_MIN);

    IZ_BOOL bEnableSkin = (m_SkinList.size() > 0);

    IZ_UINT16 nMinIdx = IZ_UINT16_MAX;
    IZ_UINT16 nMaxIdx = 0;

    for (size_t i = 0; i < sPrimSet.tri.size(); i++) {
        // 三角形を取得
        IZ_UINT nTriIdx = sPrimSet.tri[i];
        STri& sTri = m_TriList[nTriIdx];

        for (size_t nVtxPos = 0; nVtxPos < 3; nVtxPos++) {
            // 頂点インデックスを取得
            IZ_UINT nVtxIdx = sTri.vtx[nVtxPos];

            // 出力済み頂点かどうか
            std::vector<IZ_UINT>::iterator itFind = std::find(
                                                        m_ExportedVtx.begin(),
                                                        m_ExportedVtx.end(),
                                                        nVtxIdx);

            // 頂点データ出力に応じたインデックスに変換
            if (itFind != m_ExportedVtx.end()) {
                // Exported...
                sTri.vtx[nVtxPos] = (IZ_UINT)std::distance(
                                                m_ExportedVtx.begin(),
                                                itFind);
            }
            else {
                sTri.vtx[nVtxPos] = (IZ_UINT)m_ExportedVtx.size();
            }

            IZ_UINT nIdx = sTri.vtx[nVtxPos];
            IZ_ASSERT(nIdx <= IZ_UINT16_MAX);

            nMinIdx = (nIdx < nMinIdx ? nIdx : nMinIdx);
            nMaxIdx = (nIdx > nMaxIdx ? nIdx : nMaxIdx);

            if (itFind != m_ExportedVtx.end()) {
                // 出力済み頂点なのでこれ以上は何もしない
                continue;
            }

            // 出力済み頂点リストに登録
            m_ExportedVtx.push_back(nVtxIdx);

            for (IZ_UINT nVtxFmt = 0; nVtxFmt < izanagi::E_MSH_VTX_FMT_TYPE_NUM; nVtxFmt++) {
                izanagi::math::SVector4 vec;

                // 指定された頂点における指定フォーマットのデータを取得.
                IZ_BOOL bIsExist = pImporter->GetVertex(
                                    nVtxIdx,
                                    vec, 
                                    (izanagi::E_MSH_VTX_FMT_TYPE)nVtxFmt);

                // NOTE
                // bIsExist means whether specified format is exist.

                if (bIsExist) {
                    // 指定された頂点に指定フォーマットのデータは存在する
                    IZ_ASSERT(nVtxFmt < COUNTOF(tblVtxSize));
                    IZ_ASSERT(sizeof(vec) >= tblVtxSize[nVtxFmt]);

                    if (nVtxFmt == izanagi::E_MSH_VTX_FMT_TYPE_COLOR) {
                        // カラーの場合は変換してから出力
                        IZ_UINT8 r = (IZ_UINT8)vec.x;
                        IZ_UINT8 g = (IZ_UINT8)vec.y;
                        IZ_UINT8 b = (IZ_UINT8)vec.z;
                        IZ_UINT8 a = (IZ_UINT8)vec.w;
                        IZ_UINT color = IZ_COLOR_RGBA(r, g, b, a);
                        VRETURN(pOut->Write(&color, 0, tblVtxSize[nVtxFmt]));
                    }
                    else {
                        VRETURN(pOut->Write(&vec, 0, tblVtxSize[nVtxFmt]));
                    }

                    if (nVtxFmt == izanagi::E_MSH_VTX_FMT_TYPE_POS) {
                        m_vMin.x = IZ_MIN(m_vMin.x, vec.x);
                        m_vMin.y = IZ_MIN(m_vMin.y, vec.y);
                        m_vMin.z = IZ_MIN(m_vMin.z, vec.z);

                        m_vMax.x = IZ_MAX(m_vMax.x, vec.x);
                        m_vMax.y = IZ_MAX(m_vMax.y, vec.y);
                        m_vMax.z = IZ_MAX(m_vMax.z, vec.z);
                    }
                }
            }

            // For skin.
            if (bEnableSkin) {
                // 指定頂点におけるスキニング情報を取得
                IZ_UINT nSkinIdx = pImporter->GetSkinIdxAffectToVtx(nVtxIdx);
                const SSkin& sSkin = m_SkinList[nSkinIdx];

                izanagi::math::SVector4 vecJoint;
                vecJoint.Set(0.0f, 0.0f, 0.0f, 0.0f);

                izanagi::math::SVector4 vecWeight;
                vecWeight.Set(0.0f, 0.0f, 0.0f, 0.0f);
                
                for (size_t n = 0; n < sSkin.joint.size(); n++) {
#if 1
                    // プリミティブセット内での関節位置を探す
                    // これが描画時における関節インデックスとなる
                    vecJoint.v[n] = (IZ_FLOAT)_FindJointIdx(sPrimSet.joint, sSkin.joint[n]);
#else
                    vecJoint.v[n] = sSkin.joint[n];
#endif
                    vecWeight.v[n] = sSkin.weight[n];
                }

                VRETURN(pOut->Write(&vecJoint, 0, izanagi::E_MSH_VTX_SIZE_BLENDINDICES));
                VRETURN(pOut->Write(&vecWeight, 0, izanagi::E_MSH_VTX_SIZE_BLENDWEIGHT));
            }
        }
    }

    sPrimSet.minIdx = nMinIdx;
    sPrimSet.maxIdx = nMaxIdx;

    return IZ_TRUE;
}

IZ_BOOL CGeometryChunk::ExportMesh(
    izanagi::IOutputStream* pOut,
    IImporter* pImporter)
{
    for (size_t i = 0; i < m_MeshList.size(); i++) {
        const SMesh& sMesh = m_MeshList[i];

        izanagi::S_MSH_MESH_SET sMeshInfo;
        {
            FILL_ZERO(&sMeshInfo, sizeof(sMeshInfo));

            sMeshInfo.numSubset = (IZ_UINT)sMesh.subset.size();
            sMeshInfo.fmt = sMesh.fmt;
        }

        // Blank S_MSH_MESH_SET. 
        izanagi::tool::CIoStreamSeekHelper cSeekHelper(pOut);
        VRETURN(cSeekHelper.Skip(sizeof(sMeshInfo)));

        m_Header.numMeshSubset += sMeshInfo.numSubset;

        pImporter->BeginMesh((IZ_UINT)i);

        izanagi::math::SVector4 vMin;
        izanagi::math::SVector4 vMax;

        vMin.Set(IZ_FLOAT_MAX, IZ_FLOAT_MAX, IZ_FLOAT_MAX);
        vMax.Set(IZ_FLOAT_MIN, IZ_FLOAT_MIN, IZ_FLOAT_MIN);

        for (size_t n = 0; n < sMesh.subset.size(); n++) {
            const SPrimSet& sPrimSet = sMesh.subset[n];

            VRETURN(
                ExportPrimSet(
                    pOut,
                    pImporter,
                    sPrimSet));

            // Get min and max position in primitives.
            GetMinMaxPos(
                pImporter,
                vMin, vMax,
                sPrimSet);
        }

        pImporter->EndMesh();

        // Compute center position in meshset.
        sMeshInfo.center[0] = (vMin.x + vMax.x) * 0.5f;
        sMeshInfo.center[1] = (vMin.y + vMax.y) * 0.5f;
        sMeshInfo.center[2] = (vMin.z + vMax.z) * 0.5f;

        // Get material information.
        pImporter->GetMaterialForMesh(
            static_cast<IZ_UINT>(i),
            sMeshInfo.mtrl);

        // Return to position of expoting S_MSH_MESH_SET.
        VRETURN(cSeekHelper.ReturnWithAnchor());

        // Export S_MSH_PRIM_SET.
        IZ_OUTPUT_WRITE_VRETURN(pOut, &sMeshInfo, 0, sizeof(sMeshInfo));

        VRETURN(cSeekHelper.ReturnToAnchor());
    }

    return IZ_TRUE;
}

void CGeometryChunk::GetMinMaxPos(
    IImporter* pImporter,
    izanagi::math::SVector4& vMin,
    izanagi::math::SVector4& vMax,
    const SPrimSet& sPrimSet)
{
    for (size_t i = 0; i < sPrimSet.tri.size(); i++) {
        IZ_UINT nTriIdx = sPrimSet.tri[i];
        const STri& sTri = m_TriList[nTriIdx];

        for (IZ_UINT n = 0; n < 3; ++n) {
            izanagi::math::SVector4 vec;

            // Get vertex's position.
            IZ_BOOL bIsExist = pImporter->GetVertex(
                                sTri.vtx[n],
                                vec, 
                                izanagi::E_MSH_VTX_FMT_TYPE_POS);
            IZ_ASSERT(bIsExist);

            vMin.x = IZ_MIN(vMin.x, vec.x);
            vMin.y = IZ_MIN(vMin.y, vec.y);
            vMin.z = IZ_MIN(vMin.z, vec.z);

            vMax.x = IZ_MAX(vMax.x, vec.x);
            vMax.y = IZ_MAX(vMax.y, vec.y);
            vMax.z = IZ_MAX(vMax.z, vec.z);
        }
    }
}

IZ_BOOL CGeometryChunk::ExportPrimSet(
    izanagi::IOutputStream* pOut,
    IImporter* pImporter,
    const SPrimSet& sPrimSet)
{
    izanagi::S_MSH_PRIM_SET sSubsetInfo;
    {
        FILL_ZERO(&sSubsetInfo, sizeof(sSubsetInfo));

        sSubsetInfo.idxVB = sPrimSet.idxVB;
        sSubsetInfo.minIdx = sPrimSet.minIdx;
        sSubsetInfo.maxIdx = sPrimSet.maxIdx;

        sSubsetInfo.typePrim = (m_ExportTriList
            ? izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST
            : izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLESTRIP);

        // TODO
        sSubsetInfo.fmtIdx = izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX32;

        sSubsetInfo.numJoints = (IZ_UINT16)sPrimSet.joint.size();
    }

    // のべ所属関節インデックス数
    m_Header.numAllJointIndices += sSubsetInfo.numJoints;

    // Blank S_MSH_PRIM_SET. 
    izanagi::tool::CIoStreamSeekHelper cSeekHelper(pOut);
    VRETURN(cSeekHelper.Skip(sizeof(sSubsetInfo)));

    // 所属関節へのインデックス
    {
        std::set<IZ_UINT>::const_iterator it = sPrimSet.joint.begin();
        for (; it != sPrimSet.joint.end(); it++) {
            IZ_UINT16 idx = *it;
            IZ_OUTPUT_WRITE_VRETURN(pOut, &idx, 0, sizeof(idx));
        }
    }

    // Export indices.
    sSubsetInfo.numIdx = ExportIndices(
                            pOut,
                            pImporter,
                            sPrimSet);
    VRETURN(sSubsetInfo.numIdx > 0);

    // Return to position of expoting S_MSH_PRIM_SET.
    VRETURN(cSeekHelper.ReturnWithAnchor());

    // Export S_MSH_PRIM_SET.
    IZ_OUTPUT_WRITE_VRETURN(pOut, &sSubsetInfo, 0, sizeof(sSubsetInfo));

    VRETURN(cSeekHelper.ReturnToAnchor());

    return IZ_TRUE;
}

IZ_UINT CGeometryChunk::ExportIndices(
    izanagi::IOutputStream* pOut,
    IImporter* pImporter,
    const SPrimSet& sPrimSet)
{
    IZ_BOOL result = IZ_FALSE;
    IZ_UINT nIdxNum = 0;

    std::vector<IZ_UINT> tvIndices;
    tvIndices.reserve(sPrimSet.tri.size() * 3);

    // Gather indices.
    for (size_t i = 0; i < sPrimSet.tri.size(); i++) {
        IZ_UINT nTriIdx = sPrimSet.tri[i];
        const STri& sTri = m_TriList[nTriIdx];

        tvIndices.push_back(sTri.vtx[0]);
        tvIndices.push_back(sTri.vtx[1]);
        tvIndices.push_back(sTri.vtx[2]);
    }

    if (m_ExportTriList)
    {
        // TriangleList

        nIdxNum = static_cast<IZ_UINT>(tvIndices.size());

        result = pOut->Write(&tvIndices[0], 0, sizeof(IZ_UINT) * tvIndices.size());
        IZ_ASSERT(result);
    }
    else
    {
        // TriangleStrip

        PrimitiveGroup* pPrimGroup = IZ_NULL;
        IZ_USHORT nPrimGroupNum = 0;

        // Crteate triangle strip.
        VRETURN_VAL(
            GenerateStrips(
                &tvIndices[0],
                (IZ_UINT)tvIndices.size(),
                &pPrimGroup,
                &nPrimGroupNum), 0);

        VRETURN_VAL(nPrimGroupNum == 1, 0);
        VRETURN_VAL(pPrimGroup != IZ_NULL, 0);

        nIdxNum = pPrimGroup->numIndices;

        // Export indices.
        result = pOut->Write(pPrimGroup->indices, 0, sizeof(UINT32) * pPrimGroup->numIndices);
        IZ_ASSERT(result);

        SAFE_DELETE_ARRAY(pPrimGroup);
    }

    return (result ? nIdxNum : 0);
}
