#if !defined(__MODEL_LIB_IMPORTER_H__)
#define __MODEL_LIB_IMPORTER_H__

#include "GeometryCommon.h"
#include "izToolKit.h"
#include "izSceneGraph.h"

#include <vector>

// ���f���^�C�v
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

    /** �W�I���g���`�����N�̃G�N�X�|�[�g�������������Ƃ�ʒm.
     */
    PURE_VIRTUAL(void ExportGeometryCompleted());
    
    PURE_VIRTUAL(IZ_UINT GetMeshNum());

    /** ���b�V���Ɋւ��鏈�����J�n.
     */
    PURE_VIRTUAL(void BeginMesh(IZ_UINT nIdx));

    /** ���b�V���Ɋւ��鏈�����I��.
     */
    PURE_VIRTUAL(void EndMesh());

    /** BeginMesh�Ŏw�肳�ꂽ���b�V���Ɋ܂܂�X�L�j���O�����擾.
     */
    PURE_VIRTUAL(void GetSkinList(std::vector<SSkin>& tvSkinList));

    /** BeginMesh�Ŏw�肳�ꂽ���b�V���Ɋ܂܂��O�p�`���擾.
     */
    PURE_VIRTUAL(IZ_UINT GetTriangles(std::vector<STri>& tvTriList));

    /** �w�肳�ꂽ���_�ɉe����^����X�L�j���O���ւ̃C���f�b�N�X���擾.
     */
    PURE_VIRTUAL(IZ_UINT GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx));

    /** �P���_������̃T�C�Y���擾.
     * �������A�X�L�j���O�Ɋւ���T�C�Y�͊܂܂Ȃ�
     */
    PURE_VIRTUAL(IZ_UINT GetVtxSize());

    /** ���_�t�H�[�}�b�g���擾.
     * �������A�X�L�j���O�Ɋւ���t�H�[�}�b�g�͊܂܂Ȃ�
     */
    PURE_VIRTUAL(IZ_UINT GetVtxFmt());

    /** �w�肳�ꂽ���_�ɂ�����w��t�H�[�}�b�g�̃f�[�^���擾.
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

    /** �֐߃f�[�^�̏o�͊�����ʒm.
     */
    PURE_VIRTUAL(void ExportJointCompleted());

    /** �֐߂Ɋւ��鏈�����J�n.
     */
    PURE_VIRTUAL(IZ_BOOL BeginJoint());

    /** �֐߂Ɋւ��鏈�����I��.
     */
    PURE_VIRTUAL(void EndJoint());

    /** �֐ߐ����擾.
     */
    PURE_VIRTUAL(IZ_UINT GetJointNum());

    /** �w�肳�ꂽ�֐߂̖��O���擾.
     */
    PURE_VIRTUAL(IZ_PCSTR GetJointName(IZ_UINT nIdx));

    /** �e�֐߂ւ̃C���f�b�N�X���擾.
     */
    PURE_VIRTUAL(
        IZ_INT GetJointParent(
            IZ_UINT nIdx,
            const std::vector<izanagi::S_SKL_JOINT>& tvJoint));

    /** �w�肳�ꂽ�֐߂̋t�}�g���N�X���擾.
     */
    PURE_VIRTUAL(
        void GetJointInvMtx(
            IZ_UINT nIdx,
            izanagi::math::SMatrix44& mtx));
        
    /** �֐߂̎p�����擾.
     */
    PURE_VIRTUAL(
        void GetJointTransform(
            IZ_UINT nIdx,
            const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
            std::vector<SJointTransform>& tvTransform));

    //////////////////////////////////
    // For animation.

    /** ���[�V�����̑ΏۂƂȂ郂�f���f�[�^���w��.
     */
    PURE_VIRTUAL(IZ_BOOL ReadBaseModel(IZ_PCSTR pszName));

    /** �t�@�C���Ɋ܂܂�郂�[�V�����̐����擾.
     */
    PURE_VIRTUAL(IZ_UINT GetAnmSetNum());

    /** ���[�V�����Ɋւ��鏈�����J�n.
     */
    PURE_VIRTUAL(IZ_BOOL BeginAnm(IZ_UINT nSetIdx));

    /** ���[�V�����Ɋւ��鏈�����I��.
     */
    PURE_VIRTUAL(IZ_BOOL EndAnm());

    /** ���[�V�����m�[�h�i�K�p�W���C���g�j�̐����擾.
     */
    PURE_VIRTUAL(IZ_UINT GetAnmNodeNum());

    /** �A�j���[�V�����`�����l���̐����擾.
     *
     * �A�j���[�V�����`�����l���Ƃ�
     * �W���C���g�̃p�����[�^�iex. �ʒu�A��]�Ȃǁj���Ƃ̃A�j���[�V�������̂���
     */
    PURE_VIRTUAL(
        IZ_UINT GetAnmChannelNum(IZ_UINT nNodeIdx));

    /** ���[�V�����m�[�h�i�K�p�W���C���g�j�̏����擾.
     */
    PURE_VIRTUAL(
        IZ_BOOL GetAnmNode(
            IZ_UINT nNodeIdx,
            izanagi::S_ANM_NODE& sNode));

    /** �A�j���[�V�����`�����l���̏����擾.
     *
     * �A�j���[�V�����`�����l���Ƃ�
     * �W���C���g�̃p�����[�^�iex. �ʒu�A��]�Ȃǁj���Ƃ̃A�j���[�V�������̂���
     */
    PURE_VIRTUAL(
        IZ_BOOL GetAnmChannel(
            IZ_UINT nNodeIdx,
            IZ_UINT nChannelIdx,
            izanagi::S_ANM_CHANNEL& sChannel));
    
    /** �L�[�t���[�������擾.
     *
     * �L�[�t���[��������̃W���C���g�̃p�����[�^�ɓK�p����p�����[�^���擾.
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
