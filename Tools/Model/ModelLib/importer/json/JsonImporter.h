#if !defined(__MODEL_LIB_JSON_IMPORTER_H__)
#define __MODEL_LIB_JSON_IMPORTER_H__

#include "Importer.h"
#include <rapidjson/document.h>
#include <rapidjson/filestream.h>
#include "JsonGeometry.h"

class CJsonImporter : public IImporter {
    friend class IImporter;

protected:
    CJsonImporter();
    ~CJsonImporter() { Close(); }

    NO_COPIABLE(CJsonImporter);

public:
    virtual IZ_BOOL Open(IZ_PCSTR pszName);
    virtual IZ_BOOL Close();

    //////////////////////////////////
    // For geometry chunk.

    virtual void ExportGeometryCompleted();

    virtual IZ_UINT GetMeshNum();

    // メッシュに関する処理を開始.
    virtual void BeginMesh(IZ_UINT nIdx);

    // メッシュに関する処理を終了.
    virtual void EndMesh();

    // BeginMeshで指定されたメッシュに含まれスキニング情報を取得.
    virtual void GetSkinList(std::vector<SSkin>& tvSkinList);

    // BeginMeshで指定されたメッシュに含まれる三角形を取得.
    virtual IZ_UINT GetTriangles(std::vector<STri>& tvTriList);

    // 指定された頂点に影響を与えるスキニング情報へのインデックスを取得.
    virtual IZ_UINT GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx);

    // １頂点あたりのサイズを取得.
    // ただし、スキニングに関するサイズは含まない
    virtual IZ_UINT GetVtxSize();

    // 頂点フォーマットを取得.
    // ただし、スキニングに関するフォーマットは含まない
    virtual IZ_UINT GetVtxFmt();

    // 指定された頂点における指定フォーマットのデータを取得.
    virtual IZ_BOOL GetVertex(
        IZ_UINT nIdx,
        izanagi::math::SVector4& vec,
        izanagi::E_MSH_VTX_FMT_TYPE type);

    virtual void GetMaterialForMesh(
        IZ_UINT nIdx,
        izanagi::S_MSH_MTRL& sMtrl);

    //////////////////////////////////
    // For joint chunk.

    // 関節データの出力完了を通知.
    virtual void ExportJointCompleted();

    // 関節に関する処理を開始.
    virtual IZ_BOOL BeginJoint();

    // 関節に関する処理を終了.
    virtual void EndJoint();

    // 関節数を取得.
    virtual IZ_UINT GetJointNum();

    // 指定された関節の名前を取得.
    virtual IZ_PCSTR GetJointName(IZ_UINT nIdx);

    // 親関節へのインデックスを取得.    
    virtual IZ_INT GetJointParent(
        IZ_UINT nIdx,
        const std::vector<izanagi::S_SKL_JOINT>& tvJoint);

    // 指定された関節の逆マトリクスを取得.  
    virtual void GetJointInvMtx(
        IZ_UINT nIdx,
        izanagi::math::SMatrix44& mtx);
        
    // 関節の姿勢を取得.
    virtual void GetJointTransform(
        IZ_UINT nIdx,
        const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
        std::vector<SJointTransform>& tvTransform);

    //////////////////////////////////
    // For animation.

    // モーションの対象となるモデルデータを指定.
    virtual IZ_BOOL ReadBaseModel(IZ_PCSTR pszName);

    // ファイルに含まれるモーションの数を取得.
    virtual IZ_UINT GetAnmSetNum();

    // モーションに関する処理を開始.
    virtual IZ_BOOL BeginAnm(IZ_UINT nSetIdx);

    // モーションに関する処理を終了.
    virtual IZ_BOOL EndAnm();

    // モーションノード（適用ジョイント）の数を取得.
    virtual IZ_UINT GetAnmNodeNum();

    // アニメーションチャンネルの数を取得.
    // アニメーションチャンネルとは
    // ジョイントのパラメータ（ex. 位置、回転など）ごとのアニメーション情報のこと
    virtual IZ_UINT GetAnmChannelNum(IZ_UINT nNodeIdx);

    // モーションノード（適用ジョイント）の情報を取得.
    virtual IZ_BOOL GetAnmNode(
        IZ_UINT nNodeIdx,
        izanagi::S_ANM_NODE& sNode);

    // アニメーションチャンネルの情報を取得.
    // アニメーションチャンネルとは
    // ジョイントのパラメータ（ex. 位置、回転など）ごとのアニメーション情報のこと
    virtual IZ_BOOL GetAnmChannel(
        IZ_UINT nNodeIdx,
        IZ_UINT nChannelIdx,
        izanagi::S_ANM_CHANNEL& sChannel);

    // キーフレーム情報を取得.
    // キーフレームあたりのジョイントのパラメータに適用するパラメータを取得.
    virtual IZ_BOOL GetAnmKey(
        IZ_UINT nNodeIdx,
        IZ_UINT nChannelIdx,
        IZ_UINT nKeyIdx,
        izanagi::S_ANM_KEY& sKey,
        std::vector<IZ_FLOAT>& tvValue);

    //////////////////////////////////
    // For material.

    virtual IZ_BOOL BeginMaterial();

    virtual IZ_BOOL EndMaterial();

    virtual IZ_UINT GetMaterialNum();

    virtual IZ_BOOL GetMaterial(
        IZ_UINT nMtrlIdx,
        izanagi::S_MTRL_MATERIAL& sMtrl);

    virtual void GetMaterialTexture(
        IZ_UINT nMtrlIdx,
        IZ_UINT nTexIdx,
        izanagi::S_MTRL_TEXTURE& sTex);

    virtual void GetMaterialShader(
        IZ_UINT nMtrlIdx,
        IZ_UINT nShaderIdx,
        izanagi::S_MTRL_SHADER& sShader);

    virtual void GetMaterialParam(
        IZ_UINT nMtrlIdx,
        IZ_UINT nParamIdx,
        izanagi::S_MTRL_PARAM& sParam);

    virtual void GetMaterialParamValue(
        IZ_UINT nMtrlIdx,
        IZ_UINT nParamIdx,
        std::vector<IZ_FLOAT>& tvValue);

protected:
    rapidjson::Document m_Document;

    FILE* m_FP;
    rapidjson::FileStream* m_FileStream;

    CJsonGeometry m_Geometry;
};

#endif  // #if !defined(__MODEL_LIB_JSON_IMPORTER_H__)
