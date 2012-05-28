#include "PMDImporter.h"
#include "izToolKit.h"

CPmdImporter::CPmdImporter()
{
	m_CurMeshIdx = -1;
}

IZ_BOOL CPmdImporter::Open(IZ_PCSTR pszName)
{
	IZ_BOOL ret = m_Loader.Load(pszName);
	return ret;
}

IZ_BOOL CPmdImporter::Close()
{
	m_Loader.Clear();
	return IZ_TRUE;
}

//////////////////////////////////
// For geometry chunk.

void CPmdImporter::ExportGeometryCompleted()
{
	m_Loader.ClearGeometryData();
}

void CPmdImporter::BeginMesh(IZ_UINT nIdx)
{
	IZ_ASSERT(!m_FaceIdxInMtrlList.empty());
	IZ_ASSERT(nIdx < m_FaceIdxInMtrlList.size());

	m_CurMeshIdx = nIdx;
}

void CPmdImporter::EndMesh()
{
	m_CurMeshIdx = -1;
}

IZ_UINT CPmdImporter::GetMeshNum()
{
	if (m_FaceIdxInMtrlList.empty())
	{
		IZ_UINT vtxPos = 0;

		for (IZ_UINT i = 0; i < m_Loader.GetMtrlNum(); i++)
		{
			const SPmdMaterial& mtrl = m_Loader.GetMtrl(i);

			// NOTE
			// １面あたり３頂点なので
			// 頂点位置を３で割れば、それは面の位置になる
			m_FaceIdxInMtrlList.push_back(vtxPos / 3);

			vtxPos += mtrl.faceVtxNum;
		}
	}

	// メッシュとマテリアルが１対１に結びつくので
	// メッシュ数 = マテリアル数 となる
	IZ_UINT ret = m_Loader.GetMtrlNum();
	IZ_ASSERT(ret == m_FaceIdxInMtrlList.size());

	return ret;
}

void CPmdImporter::GetSkinList(std::vector<SSkin>& tvSkinList)
{
	IZ_UINT vtxNum = m_Loader.GetVtxNum();

	for (IZ_UINT i = 0; i < vtxNum; i++)
	{
		const SPmdVertex& vtx = m_Loader.GetVertex(i);

		SSkin skin;
		// TODO

		tvSkinList.push_back(skin);
	}
}

IZ_UINT CPmdImporter::GetTriangles(std::vector<STri>& tvTriList)
{
	IZ_ASSERT(m_CurMeshIdx >= 0);

	// 指定されたメッシュセットの面の位置を取得
	const IZ_UINT facePos = m_FaceIdxInMtrlList[m_CurMeshIdx];

	// NOTE
	// １面あたり３頂点なので
	// 頂点位置を３で割れば、それは面の数になる
	const SPmdMaterial& mtrl = m_Loader.GetMtrl(m_CurMeshIdx);
	const IZ_UINT faceNum = mtrl.faceVtxNum / 3;

	for (IZ_UINT i = 0; i < faceNum; i++)
	{
		IZ_UINT idx = facePos + i;
		const SPmdFace& face = m_Loader.GetFace(idx);

		STri tri;
		{
			tri.vtx[0] = face.vtxIdx[0];
			tri.vtx[1] = face.vtxIdx[1];
			tri.vtx[2] = face.vtxIdx[2];
		}

		tvTriList.push_back(tri);
	}

	return mtrl.faceVtxNum;
}

IZ_UINT CPmdImporter::GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx)
{
	IZ_ASSERT(nVtxIdx < m_Loader.GetVtxNum());
	return nVtxIdx;
}

IZ_UINT CPmdImporter::GetVtxSize()
{
	IZ_UINT ret = izanagi::E_MSH_VTX_SIZE_POS;
	ret += izanagi::E_MSH_VTX_SIZE_NORMAL;
	ret += izanagi::E_MSH_VTX_SIZE_UV;
	return ret;
}

IZ_UINT CPmdImporter::GetVtxFmt()
{
	IZ_UINT ret = 1 << izanagi::E_MSH_VTX_FMT_TYPE_POS;
	ret = 1 << izanagi::E_MSH_VTX_FMT_TYPE_NORMAL;
	ret = 1 << izanagi::E_MSH_VTX_FMT_TYPE_UV;
	return ret;
}

IZ_BOOL CPmdImporter::GetVertex(
	IZ_UINT nIdx,
	izanagi::SVector& vec,
	izanagi::E_MSH_VTX_FMT_TYPE type)
{
	// NOTE
	// スキンニングについては別であつかうので
	// ここでは考慮しない

	const SPmdVertex& vtx = m_Loader.GetVertex(nIdx);

	IZ_BOOL ret = IZ_TRUE;

	switch (type)
	{
	case izanagi::E_MSH_VTX_FMT_TYPE_POS:
		vec.Set(vtx.pos[0], vtx.pos[1], vtx.pos[2], 1.0f);
		break;
	case izanagi::E_MSH_VTX_FMT_TYPE_NORMAL:
		vec.Set(vtx.normal[0], vtx.normal[1], vtx.normal[2], 0.0f);
		break;
	case izanagi::E_MSH_VTX_FMT_TYPE_UV:
		vec.Set(vtx.uv[0], vtx.uv[1], 0.0f, 0.0f);
		break;
#if 0
	case izanagi::E_MSH_VTX_FMT_TYPE_BLENDINDICES:
		vec.Set(vtx.boneIdx[0], vtx.boneIdx[1], 0.0f, 0.0f);
		break;
	case izanagi::E_MSH_VTX_FMT_TYPE_BLENDWEIGHT:
		{
			IZ_FLOAT weight = IZ_MIN(vtx.boneWeight * 0.1f, 1.0f);
			vec.Set(weight, 1.0f - weight, 0.0f, 0.0f);
		}
		break;
#endif
	default:
		ret = IZ_FALSE;
		break;
	}

	return ret;
}

void CPmdImporter::GetMaterialForMesh(
	IZ_UINT nIdx,
	izanagi::S_MSH_MTRL& sMtrl)
{
	// NOTE
	// PMDのマテリアルは名前を持たないので
	// 適当に名前を付ける

	izanagi::tool::CString str;
	str.format("materal_%d\0", nIdx);

	sMtrl.name.SetString(str.c_str());
	sMtrl.nameKey = sMtrl.name.GetKeyValue();
}

//////////////////////////////////
// For joint chunk.

void CPmdImporter::ExportJointCompleted()
{
}

IZ_BOOL CPmdImporter::BeginJoint()
{
	// 関節の姿勢を計算
	m_Loader.ComputeJointPose();

	return IZ_TRUE;
}

void CPmdImporter::EndJoint()
{
	// Nothing is done...
}

IZ_UINT CPmdImporter::GetJointNum()
{
	IZ_UINT ret = m_Loader.GetBoneNum();
	return ret;
}

IZ_PCSTR CPmdImporter::GetJointName(IZ_UINT nIdx)
{
	// TODO
	izanagi::tool::CString str;
	str.format("Joint_%d", nIdx);
	return str.c_str();
}

IZ_INT CPmdImporter::GetJointParent(
	IZ_UINT nIdx,
	const std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
	const SPmdBone& bone = m_Loader.GetBone(nIdx);
	IZ_INT ret = (bone.parentIdx == 0xFFFF ? -1 : bone.parentIdx);
	return ret;
}

void CPmdImporter::GetJointInvMtx(
	IZ_UINT nIdx,
	izanagi::SMatrix& mtx)
{
	const izanagi::SMatrix& mtxJoint = m_Loader.GetMatrix(nIdx);
	izanagi::SMatrix::Inverse(mtx, mtxJoint);
}

void CPmdImporter::GetJointTransform(
	IZ_UINT nIdx,
	const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
	std::vector<SJointTransform>& tvTransform)
{
	const izanagi::S_SKL_JOINT_POSE& pose = m_Loader.GetPose(nIdx);

	// NOTE
	// translateのみを考えればいい

	SJointTransform transform;
	transform.type = E_MDL_JOINT_TRANSFORM_TRANSLATE;
	transform.param.push_back(pose.trans[0]);
	transform.param.push_back(pose.trans[1]);
	transform.param.push_back(pose.trans[2]);
}

//////////////////////////////////
// For animation.

IZ_BOOL CPmdImporter::ReadBaseModel(IZ_PCSTR pszName)
{
	return IZ_TRUE;
}

IZ_UINT CPmdImporter::GetAnmSetNum()
{
	return 0;
}

IZ_BOOL CPmdImporter::BeginAnm(IZ_UINT nSetIdx)
{
	return IZ_TRUE;
}

IZ_BOOL CPmdImporter::EndAnm()
{
	return IZ_TRUE;
}

IZ_UINT CPmdImporter::GetAnmNodeNum()
{
	return 0;
}

IZ_UINT CPmdImporter::GetAnmChannelNum(IZ_UINT nNodeIdx)
{
	return 0;
}

IZ_BOOL CPmdImporter::GetAnmNode(
	IZ_UINT nNodeIdx,
	izanagi::S_ANM_NODE& sNode)
{
	return IZ_TRUE;
}

IZ_BOOL CPmdImporter::GetAnmChannel(
	IZ_UINT nNodeIdx,
	IZ_UINT nChannelIdx,
	izanagi::S_ANM_CHANNEL& sChannel)
{
	return IZ_TRUE;
}

IZ_BOOL CPmdImporter::GetAnmKey(
	IZ_UINT nNodeIdx,
	IZ_UINT nChannelIdx,
	IZ_UINT nKeyIdx,
	izanagi::S_ANM_KEY& sKey,
	std::vector<IZ_FLOAT>& tvValue)
{
	return IZ_TRUE;
}

//////////////////////////////////
// For material.

IZ_BOOL CPmdImporter::BeginMaterial()
{
	return IZ_TRUE;
}

IZ_BOOL CPmdImporter::EndMaterial()
{
	return IZ_TRUE;
}

IZ_UINT CPmdImporter::GetMaterialNum()
{
	return 0;
}

IZ_BOOL CPmdImporter::GetMaterial(
	IZ_UINT nMtrlIdx,
	izanagi::S_MTRL_MATERIAL& sMtrl)
{
	return IZ_TRUE;
}

void CPmdImporter::GetMaterialTexture(
	IZ_UINT nMtrlIdx,
	IZ_UINT nTexIdx,
	izanagi::S_MTRL_TEXTURE& sTex)
{
}

void CPmdImporter::GetMaterialShader(
	IZ_UINT nMtrlIdx,
	IZ_UINT nShaderIdx,
	izanagi::S_MTRL_SHADER& sShader)
{
}

void CPmdImporter::GetMaterialParam(
	IZ_UINT nMtrlIdx,
	IZ_UINT nParamIdx,
	izanagi::S_MTRL_PARAM& sParam)
{
}

void CPmdImporter::GetMaterialParamValue(
	IZ_UINT nMtrlIdx,
	IZ_UINT nParamIdx,
	std::vector<IZ_FLOAT>& tvValue)
{
}
