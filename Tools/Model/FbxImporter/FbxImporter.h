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

    // ���b�V���Ɋւ��鏈�����J�n.
    virtual void BeginMesh(IZ_UINT nIdx) override;

    // ���b�V���Ɋւ��鏈�����I��.
    virtual void EndMesh() override;

    // BeginMesh�Ŏw�肳�ꂽ���b�V���Ɋ܂܂�X�L�j���O�����擾.
    virtual void GetSkinList(std::vector<SSkin>& tvSkinList) override;

    // BeginMesh�Ŏw�肳�ꂽ���b�V���Ɋ܂܂��O�p�`���擾.
    virtual IZ_UINT GetTriangles(std::vector<STri>& tvTriList) override;

    // �w�肳�ꂽ���_�ɉe����^����X�L�j���O���ւ̃C���f�b�N�X���擾.
    virtual IZ_UINT GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx) override;

    // �P���_������̃T�C�Y���擾.
    // �������A�X�L�j���O�Ɋւ���T�C�Y�͊܂܂Ȃ�
    virtual IZ_UINT GetVtxSize() override;

    // ���_�t�H�[�}�b�g���擾.
    // �������A�X�L�j���O�Ɋւ���t�H�[�}�b�g�͊܂܂Ȃ�
    virtual IZ_UINT GetVtxFmt() override;

    // �w�肳�ꂽ���_�ɂ�����w��t�H�[�}�b�g�̃f�[�^���擾.
    virtual IZ_BOOL GetVertex(
        IZ_UINT nIdx,
        izanagi::math::SVector4& vec,
        izanagi::E_MSH_VTX_FMT_TYPE type) override;

    virtual void GetMaterialForMesh(
        IZ_UINT nMeshIdx,
        izanagi::S_MSH_MTRL& sMtrl) override;

    //////////////////////////////////
    // For joint chunk.

    // �֐߃f�[�^�̏o�͊�����ʒm.
    virtual void ExportJointCompleted() override;

    // �֐߂Ɋւ��鏈�����J�n.
    virtual IZ_BOOL BeginJoint() override;

    // �֐߂Ɋւ��鏈�����I��.
    virtual void EndJoint() override;

    // �֐ߐ����擾.
    virtual IZ_UINT GetJointNum() override;

    // �w�肳�ꂽ�֐߂̖��O���擾.
    virtual IZ_PCSTR GetJointName(IZ_UINT nIdx) override;

    // �e�֐߂ւ̃C���f�b�N�X���擾.    
    virtual IZ_INT GetJointParent(
        IZ_UINT nIdx,
        const std::vector<izanagi::S_SKL_JOINT>& tvJoint) override;

    // �w�肳�ꂽ�֐߂̋t�}�g���N�X���擾.  
    virtual void GetJointInvMtx(
        IZ_UINT nIdx,
        izanagi::math::SMatrix44& mtx) override;
        
    // �֐߂̎p�����擾.
    virtual void GetJointTransform(
        IZ_UINT nIdx,
        const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
        std::vector<SJointTransform>& tvTransform) override;

    //////////////////////////////////
    // For animation.

    // ���[�V�����̑ΏۂƂȂ郂�f���f�[�^���w��.
    virtual IZ_BOOL ReadBaseModel(IZ_PCSTR pszName) override;

    // �t�@�C���Ɋ܂܂�郂�[�V�����̐����擾.
    virtual IZ_UINT GetAnmSetNum() override;

    // ���[�V�����Ɋւ��鏈�����J�n.
    virtual IZ_BOOL BeginAnm(IZ_UINT nSetIdx) override;

    // ���[�V�����Ɋւ��鏈�����I��.
    virtual IZ_BOOL EndAnm() override;

    // ���[�V�����m�[�h�i�K�p�W���C���g�j�̐����擾.
    virtual IZ_UINT GetAnmNodeNum() override;

    // �A�j���[�V�����`�����l���̐����擾.
    // �A�j���[�V�����`�����l���Ƃ�
    // �W���C���g�̃p�����[�^�iex. �ʒu�A��]�Ȃǁj���Ƃ̃A�j���[�V�������̂���
    virtual IZ_UINT GetAnmChannelNum(IZ_UINT nNodeIdx) override;

    // ���[�V�����m�[�h�i�K�p�W���C���g�j�̏����擾.
    virtual IZ_BOOL GetAnmNode(
        IZ_UINT nNodeIdx,
        izanagi::S_ANM_NODE& sNode) override;

    // �A�j���[�V�����`�����l���̏����擾.
    // �A�j���[�V�����`�����l���Ƃ�
    // �W���C���g�̃p�����[�^�iex. �ʒu�A��]�Ȃǁj���Ƃ̃A�j���[�V�������̂���
    virtual IZ_BOOL GetAnmChannel(
        IZ_UINT nNodeIdx,
        IZ_UINT nChannelIdx,
        izanagi::S_ANM_CHANNEL& sChannel) override;

    // �L�[�t���[�������擾.
    // �L�[�t���[��������̃W���C���g�̃p�����[�^�ɓK�p����p�����[�^���擾.
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
