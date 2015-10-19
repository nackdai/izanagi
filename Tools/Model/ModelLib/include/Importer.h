#if !defined(__MODEL_LIB_IMPORTER_H__)
#define __MODEL_LIB_IMPORTER_H__

#include "GeometryCommon.h"
#include "izToolKit.h"
#include "izSceneGraph.h"

#include <vector>

// モデルタイプ
enum ModelType {
    ModelTypeCollada = 0,
    ModelTypeXFile,
    ModelTypeFBX,
    ModelTypePMD,
    ModelTypeJSON,

    ModelTypeUnknown,

    ModelTypeNum,
};

class IImporter {
public:
    static IImporter* CreateImporter(ModelType type);

protected:
    IImporter() {}
    virtual ~IImporter() {}

public:
    PURE_VIRTUAL(IZ_BOOL Open(IZ_PCSTR pszName));
    PURE_VIRTUAL(IZ_BOOL Close());

    //////////////////////////////////
    // For geometry chunk.

    /** ジオメトリチャンクのエクスポートが完了したことを通知.
     */
    PURE_VIRTUAL(void ExportGeometryCompleted());
    
    PURE_VIRTUAL(IZ_UINT GetMeshNum());

    /** メッシュに関する処理を開始.
     */
    PURE_VIRTUAL(void BeginMesh(IZ_UINT nIdx));

    /** メッシュに関する処理を終了.
     */
    PURE_VIRTUAL(void EndMesh());

    /** BeginMeshで指定されたメッシュに含まれスキニング情報を取得.
     */
    PURE_VIRTUAL(void GetSkinList(std::vector<SSkin>& tvSkinList));

    /** BeginMeshで指定されたメッシュに含まれる三角形を取得.
     */
    PURE_VIRTUAL(IZ_UINT GetTriangles(std::vector<STri>& tvTriList));

    /** 指定された頂点に影響を与えるスキニング情報へのインデックスを取得.
     */
    PURE_VIRTUAL(IZ_UINT GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx));

    /** １頂点あたりのサイズを取得.
     * ただし、スキニングに関するサイズは含まない
     */
    PURE_VIRTUAL(IZ_UINT GetVtxSize());

    /** 頂点フォーマットを取得.
     * ただし、スキニングに関するフォーマットは含まない
     */
    PURE_VIRTUAL(IZ_UINT GetVtxFmt());

    /** 指定された頂点における指定フォーマットのデータを取得.
     */
    PURE_VIRTUAL(
        IZ_BOOL GetVertex(
            IZ_UINT nIdx,
            izanagi::math::SVector4& vec,
            izanagi::E_MSH_VTX_FMT_TYPE type));

    PURE_VIRTUAL(
        void GetMaterialForMesh(
            IZ_UINT nIdx,
            izanagi::S_MSH_MTRL& sMtrl));

    //////////////////////////////////
    // For joint chunk.

    /** 関節データの出力完了を通知.
     */
    PURE_VIRTUAL(void ExportJointCompleted());

    /** 関節に関する処理を開始.
     */
    PURE_VIRTUAL(IZ_BOOL BeginJoint());

    /** 関節に関する処理を終了.
     */
    PURE_VIRTUAL(void EndJoint());

    /** 関節数を取得.
     */
    PURE_VIRTUAL(IZ_UINT GetJointNum());

    /** 指定された関節の名前を取得.
     */
    PURE_VIRTUAL(IZ_PCSTR GetJointName(IZ_UINT nIdx));

    /** 親関節へのインデックスを取得.
     */
    PURE_VIRTUAL(
        IZ_INT GetJointParent(
            IZ_UINT nIdx,
            const std::vector<izanagi::S_SKL_JOINT>& tvJoint));

    /** 指定された関節の逆マトリクスを取得.
     */
    PURE_VIRTUAL(
        void GetJointInvMtx(
            IZ_UINT nIdx,
            izanagi::math::SMatrix44& mtx));
        
    /** 関節の姿勢を取得.
     */
    PURE_VIRTUAL(
        void GetJointTransform(
            IZ_UINT nIdx,
            const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
            std::vector<SJointTransform>& tvTransform));

    //////////////////////////////////
    // For animation.

    /** モーションの対象となるモデルデータを指定.
     */
    PURE_VIRTUAL(IZ_BOOL ReadBaseModel(IZ_PCSTR pszName));

    /** ファイルに含まれるモーションの数を取得.
     */
    PURE_VIRTUAL(IZ_UINT GetAnmSetNum());

    /** モーションに関する処理を開始.
     */
    PURE_VIRTUAL(IZ_BOOL BeginAnm(IZ_UINT nSetIdx));

    /** モーションに関する処理を終了.
     */
    PURE_VIRTUAL(IZ_BOOL EndAnm());

    /** モーションノード（適用ジョイント）の数を取得.
     */
    PURE_VIRTUAL(IZ_UINT GetAnmNodeNum());

    /** アニメーションチャンネルの数を取得.
     *
     * アニメーションチャンネルとは
     * ジョイントのパラメータ（ex. 位置、回転など）ごとのアニメーション情報のこと
     */
    PURE_VIRTUAL(
        IZ_UINT GetAnmChannelNum(IZ_UINT nNodeIdx));

    /** モーションノード（適用ジョイント）の情報を取得.
     */
    PURE_VIRTUAL(
        IZ_BOOL GetAnmNode(
            IZ_UINT nNodeIdx,
            izanagi::S_ANM_NODE& sNode));

    /** アニメーションチャンネルの情報を取得.
     *
     * アニメーションチャンネルとは
     * ジョイントのパラメータ（ex. 位置、回転など）ごとのアニメーション情報のこと
     */
    PURE_VIRTUAL(
        IZ_BOOL GetAnmChannel(
            IZ_UINT nNodeIdx,
            IZ_UINT nChannelIdx,
            izanagi::S_ANM_CHANNEL& sChannel));
    
    /** キーフレーム情報を取得.
     *
     * キーフレームあたりのジョイントのパラメータに適用するパラメータを取得.
     */
    PURE_VIRTUAL(
        IZ_BOOL GetAnmKey(
            IZ_UINT nNodeIdx,
            IZ_UINT nChannelIdx,
            IZ_UINT nKeyIdx,
            izanagi::S_ANM_KEY& sKey,
            std::vector<IZ_FLOAT>& tvValue));

    //////////////////////////////////
    // For material.

    PURE_VIRTUAL(IZ_BOOL BeginMaterial());

    PURE_VIRTUAL(IZ_BOOL EndMaterial());

    PURE_VIRTUAL(IZ_UINT GetMaterialNum());

    PURE_VIRTUAL(
        IZ_BOOL GetMaterial(
            IZ_UINT nMtrlIdx,
            izanagi::S_MTRL_MATERIAL& sMtrl));

    PURE_VIRTUAL(
        void GetMaterialTexture(
            IZ_UINT nMtrlIdx,
            IZ_UINT nTexIdx,
            izanagi::S_MTRL_TEXTURE& sTex));

    PURE_VIRTUAL(
        void GetMaterialShader(
            IZ_UINT nMtrlIdx,
            IZ_UINT nShaderIdx,
            izanagi::S_MTRL_SHADER& sShader));

    PURE_VIRTUAL(
        void GetMaterialParam(
            IZ_UINT nMtrlIdx,
            IZ_UINT nParamIdx,
            izanagi::S_MTRL_PARAM& sParam));

    PURE_VIRTUAL(
        void GetMaterialParamValue(
            IZ_UINT nMtrlIdx,
            IZ_UINT nParamIdx,
            std::vector<IZ_FLOAT>& tvValue));
};

#endif  // #if !defined(__MODEL_LIB_IMPORTER_H__)
