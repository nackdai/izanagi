#if !defined(__MODEL_LIB_COLLADA_IMPORTER_H__)
#define __MODEL_LIB_COLLADA_IMPORTER_H__

#include "Importer.h"
#include <dae.h>
#include <dom/domCOLLADA.h>
#include <vector>

class CColladaImporter : public IImporter {
	friend class IImporter;

protected:
	CColladaImporter();
	~CColladaImporter() { Close(); }

	NO_COPIABLE(CColladaImporter);

public:
	IZ_BOOL Open(IZ_PCSTR pszName);
	IZ_BOOL Close();

	//////////////////////////////////
	// For geometry chunk.

	void ExportGeometryCompleted();

	IZ_UINT GetMeshNum();

	// ���b�V���Ɋւ��鏈�����J�n.
	void BeginMesh(IZ_UINT nIdx);

	// ���b�V���Ɋւ��鏈�����I��.
	void EndMesh();

	// BeginMesh�Ŏw�肳�ꂽ���b�V���Ɋ܂܂�X�L�j���O�����擾.
	void GetSkinList(std::vector<SSkin>& tvSkinList);

	// BeginMesh�Ŏw�肳�ꂽ���b�V���Ɋ܂܂��O�p�`���擾.
	IZ_UINT GetTriangles(std::vector<STri>& tvTriList);

	// �w�肳�ꂽ���_�ɉe����^����X�L�j���O���ւ̃C���f�b�N�X���擾.
	IZ_UINT GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx);

	// �P���_������̃T�C�Y���擾.
	// �������A�X�L�j���O�Ɋւ���T�C�Y�͊܂܂Ȃ�
	IZ_UINT GetVtxSize();

	// ���_�t�H�[�}�b�g���擾.
	// �������A�X�L�j���O�Ɋւ���t�H�[�}�b�g�͊܂܂Ȃ�
	IZ_UINT GetVtxFmt();

	// �w�肳�ꂽ���_�ɂ�����w��t�H�[�}�b�g�̃f�[�^���擾.
	IZ_BOOL GetVertex(
		IZ_UINT nIdx,
		izanagi::math::SVector& vec,
		izanagi::E_MSH_VTX_FMT_TYPE type);

	void GetMaterialForMesh(
		IZ_UINT nIdx,
		izanagi::S_MSH_MTRL& sMtrl);

	//////////////////////////////////
	// For joint chunk.

	// �֐߃f�[�^�̏o�͊�����ʒm.
	void ExportJointCompleted();

	// �֐߂Ɋւ��鏈�����J�n.
	IZ_BOOL BeginJoint();

	// �֐߂Ɋւ��鏈�����I��.
	void EndJoint();

	// �֐ߐ����擾.
	IZ_UINT GetJointNum();

	// �w�肳�ꂽ�֐߂̖��O���擾.
	IZ_PCSTR GetJointName(IZ_UINT nIdx);

	// �e�֐߂ւ̃C���f�b�N�X���擾.	
	IZ_INT GetJointParent(
		IZ_UINT nIdx,
		const std::vector<izanagi::S_SKL_JOINT>& tvJoint);

	// �w�肳�ꂽ�֐߂̋t�}�g���N�X���擾.	
	void GetJointInvMtx(
		IZ_UINT nIdx,
		izanagi::math::SMatrix& mtx);
		
	// �֐߂̎p�����擾.
	void GetJointTransform(
		IZ_UINT nIdx,
		const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
		std::vector<SJointTransform>& tvTransform);

	//////////////////////////////////
	// For animation.

	// ���[�V�����̑ΏۂƂȂ郂�f���f�[�^���w��.
	IZ_BOOL ReadBaseModel(IZ_PCSTR pszName);

	// �t�@�C���Ɋ܂܂�郂�[�V�����̐����擾.
	IZ_UINT GetAnmSetNum();

	// ���[�V�����Ɋւ��鏈�����J�n.
	IZ_BOOL BeginAnm(IZ_UINT nSetIdx);

	// ���[�V�����Ɋւ��鏈�����I��.
	IZ_BOOL EndAnm();

	// ���[�V�����m�[�h�i�K�p�W���C���g�j�̐����擾.
	IZ_UINT GetAnmNodeNum();

	// �A�j���[�V�����`�����l���̐����擾.
	// �A�j���[�V�����`�����l���Ƃ�
	// �W���C���g�̃p�����[�^�iex. �ʒu�A��]�Ȃǁj���Ƃ̃A�j���[�V�������̂���
	IZ_UINT GetAnmChannelNum(IZ_UINT nNodeIdx);

	// ���[�V�����m�[�h�i�K�p�W���C���g�j�̏����擾.
	IZ_BOOL GetAnmNode(
		IZ_UINT nNodeIdx,
		izanagi::S_ANM_NODE& sNode);

	// �A�j���[�V�����`�����l���̏����擾.
	// �A�j���[�V�����`�����l���Ƃ�
	// �W���C���g�̃p�����[�^�iex. �ʒu�A��]�Ȃǁj���Ƃ̃A�j���[�V�������̂���
	IZ_BOOL GetAnmChannel(
		IZ_UINT nNodeIdx,
		IZ_UINT nChannelIdx,
		izanagi::S_ANM_CHANNEL& sChannel);

	// �L�[�t���[�������擾.
	// �L�[�t���[��������̃W���C���g�̃p�����[�^�ɓK�p����p�����[�^���擾.
	IZ_BOOL GetAnmKey(
		IZ_UINT nNodeIdx,
		IZ_UINT nChannelIdx,
		IZ_UINT nKeyIdx,
		izanagi::S_ANM_KEY& sKey,
		std::vector<IZ_FLOAT>& tvValue);

	//////////////////////////////////
	// For material.

	IZ_BOOL BeginMaterial();

	IZ_BOOL EndMaterial();

	IZ_UINT GetMaterialNum();

	IZ_BOOL GetMaterial(
		IZ_UINT nMtrlIdx,
		izanagi::S_MTRL_MATERIAL& sMtrl);

	void GetMaterialTexture(
		IZ_UINT nMtrlIdx,
		IZ_UINT nTexIdx,
		izanagi::S_MTRL_TEXTURE& sTex);

	void GetMaterialShader(
		IZ_UINT nMtrlIdx,
		IZ_UINT nShaderIdx,
		izanagi::S_MTRL_SHADER& sShader);

	void GetMaterialParam(
		IZ_UINT nMtrlIdx,
		IZ_UINT nParamIdx,
		izanagi::S_MTRL_PARAM& sParam);

	void GetMaterialParamValue(
		IZ_UINT nMtrlIdx,
		IZ_UINT nParamIdx,
		std::vector<IZ_FLOAT>& tvValue);

protected:
	domGeometry* GetGeometry();
	inline domMesh* GetMesh();

protected:
	DAE* m_pDAE;
	domCOLLADA* m_pRoot;

	domGeometry* m_pGeometry;

	DAE* m_pBaseDAEForANm;
	domCOLLADA* m_pBaseRootForAnm;
};

#endif	// #if !defined(__MODEL_LIB_COLLADA_IMPORTER_H__)
