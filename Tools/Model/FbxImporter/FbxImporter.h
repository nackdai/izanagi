#if !defined(__MODEL_LIB_FBX_IMPORTER_H__)
#define __MODEL_LIB_FBX_IMPORTER_H__

#include <map>
#include "Importer.h"

class FbxDataManager;

class CFbxImporter : public IImporter {
    friend class IImporter;

public:
    CFbxImporter();
    ~CFbxImporter() { Close(); }

    NO_COPIABLE(CFbxImporter);

public:
    virtual IZ_BOOL Open(IZ_PCSTR pszName, IZ_BOOL isOpenForAnm = IZ_FALSE) override;
    virtual IZ_BOOL Close() override;

    //////////////////////////////////
    // For geometry chunk.

    virtual void ExportGeometryCompleted() override;

    virtual IZ_UINT GetMeshNum() override;

    // メッシュに関する処理を開始.
    virtual void BeginMesh(IZ_UINT nIdx) override;

    // メッシュに関する処理を終了.
    virtual void EndMesh() override;

    // BeginMeshで指定されたメッシュに含まれスキニング情報を取得.
    virtual void GetSkinList(std::vector<SSkin>& tvSkinList) override;

    // BeginMeshで指定されたメッシュに含まれる三角形を取得.
    virtual IZ_UINT GetTriangles(std::vector<STri>& tvTriList) override;

    // 指定された頂点に影響を与えるスキニング情報へのインデックスを取得.
    virtual IZ_UINT GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx) override;

    // １頂点あたりのサイズを取得.
    // ただし、スキニングに関するサイズは含まない
    virtual IZ_UINT GetVtxSize() override;

    // 頂点フォーマットを取得.
    // ただし、スキニングに関するフォーマットは含まない
    virtual IZ_UINT GetVtxFmt() override;

    // 指定された頂点における指定フォーマットのデータを取得.
    virtual IZ_BOOL GetVertex(
        IZ_UINT nIdx,
        izanagi::math::SVector4& vec,
        izanagi::E_MSH_VTX_FMT_TYPE type) override;

    virtual void GetMaterialForMesh(
        IZ_UINT nMeshIdx,
        izanagi::S_MSH_MTRL& sMtrl) override;

    //////////////////////////////////
    // For joint chunk.

    // 関節データの出力完了を通知.
    virtual void ExportJointCompleted() override;

    // 関節に関する処理を開始.
    virtual IZ_BOOL BeginJoint() override;

    // 関節に関する処理を終了.
    virtual void EndJoint() override;

    // 関節数を取得.
    virtual IZ_UINT GetJointNum() override;

    // 指定された関節の名前を取得.
    virtual IZ_PCSTR GetJointName(IZ_UINT nIdx) override;

    // 親関節へのインデックスを取得.    
    virtual IZ_INT GetJointParent(
        IZ_UINT nIdx,
        const std::vector<izanagi::S_SKL_JOINT>& tvJoint) override;

    // 指定された関節の逆マトリクスを取得.  
    virtual void GetJointInvMtx(
        IZ_UINT nIdx,
        izanagi::math::SMatrix44& mtx) override;
        
    // 関節の姿勢を取得.
    virtual void GetJointTransform(
        IZ_UINT nIdx,
        const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
        std::vector<SJointTransform>& tvTransform) override;

    //////////////////////////////////
    // For animation.

    // モーションの対象となるモデルデータを指定.
    virtual IZ_BOOL ReadBaseModel(IZ_PCSTR pszName) override;

    // ファイルに含まれるモーションの数を取得.
    virtual IZ_UINT GetAnmSetNum() override;

    // モーションに関する処理を開始.
    virtual IZ_BOOL BeginAnm(IZ_UINT nSetIdx) override;

    // モーションに関する処理を終了.
    virtual IZ_BOOL EndAnm() override;

    // モーションノード（適用ジョイント）の数を取得.
    virtual IZ_UINT GetAnmNodeNum() override;

    // アニメーションチャンネルの数を取得.
    // アニメーションチャンネルとは
    // ジョイントのパラメータ（ex. 位置、回転など）ごとのアニメーション情報のこと
    virtual IZ_UINT GetAnmChannelNum(IZ_UINT nNodeIdx) override;

    // モーションノード（適用ジョイント）の情報を取得.
    virtual IZ_BOOL GetAnmNode(
        IZ_UINT nNodeIdx,
        izanagi::S_ANM_NODE& sNode) override;

    // アニメーションチャンネルの情報を取得.
    // アニメーションチャンネルとは
    // ジョイントのパラメータ（ex. 位置、回転など）ごとのアニメーション情報のこと
    virtual IZ_BOOL GetAnmChannel(
        IZ_UINT nNodeIdx,
        IZ_UINT nChannelIdx,
        izanagi::S_ANM_CHANNEL& sChannel) override;

    // キーフレーム情報を取得.
    // キーフレームあたりのジョイントのパラメータに適用するパラメータを取得.
    virtual IZ_BOOL GetAnmKey(
        IZ_UINT nNodeIdx,
        IZ_UINT nChannelIdx,
        IZ_UINT nKeyIdx,
        izanagi::S_ANM_KEY& sKey,
        std::vector<IZ_FLOAT>& tvValue) override;

    //////////////////////////////////
    // For material.

    virtual IZ_BOOL BeginMaterial() override;

    virtual IZ_BOOL EndMaterial() override;

    virtual IZ_UINT GetMaterialNum() override;

    virtual IZ_BOOL GetMaterial(
        IZ_UINT nMtrlIdx,
        izanagi::S_MTRL_MATERIAL& sMtrl) override;

    virtual void GetMaterialTexture(
        IZ_UINT nMtrlIdx,
        IZ_UINT nTexIdx,
        izanagi::S_MTRL_TEXTURE& sTex) override;

    virtual void GetMaterialShader(
        IZ_UINT nMtrlIdx,
        IZ_UINT nShaderIdx,
        izanagi::S_MTRL_SHADER& sShader) override;

    virtual void GetMaterialParam(
        IZ_UINT nMtrlIdx,
        IZ_UINT nParamIdx,
        izanagi::S_MTRL_PARAM& sParam) override;

    virtual void GetMaterialParamValue(
        IZ_UINT nMtrlIdx,
        IZ_UINT nParamIdx,
        std::vector<IZ_FLOAT>& tvValue) override;

private:
    FbxDataManager* m_dataMgr{ nullptr };
    FbxDataManager* m_dataMgrBase{ nullptr };

    IZ_UINT m_curMeshIdx{ 0 };
    IZ_UINT m_posVtx{ 0 };

    IZ_UINT m_curAnmIdx{ 0 };

    struct MaterialTex {
        void* fbxMtrl{ nullptr };
        std::string paramName;
        void* texture{ nullptr };
        izanagi::S_MTRL_TEXTURE_TYPE type;
    };
    std::map<IZ_UINT, std::vector<MaterialTex>> m_mtrlTex;

    struct MaterialShading {
        void* fbxMtrl{ nullptr };
        std::string name;
    };
    std::map<IZ_UINT, std::vector<MaterialShading>> m_mtrlShd;

    struct MaterialParam {
        void* fbxMtrl{ nullptr };
        std::string name;
        std::vector<IZ_FLOAT> values;
    };
    std::map<IZ_UINT, std::vector<MaterialParam>> m_mtrlParam;

    void getLambertParams(void* mtrl, std::vector<MaterialParam>& list);
    void getPhongParams(void* mtrl, std::vector<MaterialParam>& list);

    enum ParamType {
        Tranlate,
        Scale,
        Rotate,

        Num,
    };

    struct AnmKey {
        IZ_UINT key;
        IZ_FLOAT value[4];

        AnmKey() {}
    };

    struct AnmChannel {
        IZ_UINT nodeIdx;
        ParamType type[ParamType::Num];

        std::vector<AnmKey> keys[ParamType::Num];

        AnmChannel()
        {
            for (IZ_UINT i = 0; i < ParamType::Num; i++) {
                type[i] = ParamType::Num;
            }
        }
    };
    std::vector<AnmChannel> m_channels;
};

#endif  // #if !defined(__MODEL_LIB_JSON_IMPORTER_H__)
