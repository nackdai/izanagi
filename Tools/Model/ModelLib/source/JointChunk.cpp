#include "JointChunk.h"
#include "izToolKit.h"

// ジョイントチャンク
// +----------------------+
// |    チャンクヘッダ    |
// +----------------------+
// |  ジョイントチャンク  |
// |       ヘッダ         |
// +----------------------+
// |  ジョイントテーブル  |
// | +------------------+ |
// | |     ジョイント   | |
// | |+----------------+| |
// | ||     ヘッダ     || |
// | |+----------------+| |
// | |+----------------+| |
// | ||    変形データ  || |
// | |+----------------+| |
// | |       ・・・     | |
// | +------------------+ |
// |        ・・・        |
// +----------------------+

IZ_BOOL CJointChunk::Export(
	izanagi::IOutputStream* pOut,
	IImporter* pImporter)
{
	VRETURN(pImporter->BeginJoint());

	IZ_UINT nJointNum = pImporter->GetJointNum();
	if (nJointNum == 0) {
		// There is no joints, so this function fihish.
		return IZ_TRUE;
	}

	izanagi::S_SKL_HEADER sHeader;
	{
		FILL_ZERO(&sHeader, sizeof(sHeader));

		// TODO
		// version, magic number

		sHeader.numJoint = nJointNum;
		sHeader.sizeHeader = sizeof(sHeader);
	}

	// Blank for S_SKL_HEADER.
	izanagi::tool::CIoStreamSeekHelper cSeekHelper(pOut);
	VRETURN(cSeekHelper.Skip(sizeof(sHeader)));

	std::vector<izanagi::S_SKL_JOINT> tvJoint;

	tvJoint.resize(pImporter->GetJointNum());

	sHeader.numJoint = (IZ_UINT)tvJoint.size();

	// Get joint's info.
	GetJointInfo(
		pOut,
		pImporter,
		tvJoint);

	// Export.
	VRETURN(
		ExportJoint(
			pOut,
			pImporter,
			tvJoint));

	pImporter->EndJoint();

	// Export S_SKL_HEADER.
	{
		sHeader.sizeFile = pOut->GetCurPos();

		// Rmenber end of geometry chunk.
		VRETURN(cSeekHelper.ReturnWithAnchor());

		IZ_OUTPUT_WRITE_VRETURN(pOut, &sHeader, 0, sizeof(sHeader));
		cSeekHelper.Step(sizeof(sHeader));

		// Return end of geometry chunk.
		VRETURN(cSeekHelper.ReturnToAnchor());
	}

	pImporter->ExportJointCompleted();

	return IZ_TRUE;
}

void CJointChunk::GetJointInfo(
	izanagi::IOutputStream* pOut,
	IImporter* pImporter,
	std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
	for (IZ_UINT i = 0; i < (IZ_UINT)tvJoint.size(); i++) {
		izanagi::S_SKL_JOINT& sJoint = tvJoint[i];

		sJoint.name.SetString(
			pImporter->GetJointName(i));
		sJoint.nameKey = sJoint.name.GetKeyValue();

		sJoint.idx = i;

		pImporter->GetJointInvMtx(
			i,
			sJoint.mtxInvBind);
	}

	for (size_t i = 0; i < tvJoint.size(); i++) {
		izanagi::S_SKL_JOINT& sJoint = tvJoint[i];

		sJoint.parent = pImporter->GetJointParent((IZ_UINT)i, tvJoint);
	}
}

IZ_BOOL CJointChunk::ExportJoint(
	izanagi::IOutputStream* pOut,
	IImporter* pImporter,
	std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
	for (size_t i = 0; i < tvJoint.size(); i++) {
		izanagi::S_SKL_JOINT& sJoint = tvJoint[i];

		std::vector<SJointTransform> tvTransform;

		pImporter->GetJointTransform(
			(IZ_UINT)i,
			tvJoint,
			tvTransform);

		izanagi::math::SMatrix mtxRot;
		izanagi::math::SMatrix::SetUnit(mtxRot);

		sJoint.validParam = 0;
		sJoint.validAnmParam = 0;

		sJoint.pose.trans[0] = 0.0f;
		sJoint.pose.trans[1] = 0.0f;
		sJoint.pose.trans[2] = 0.0f;

		sJoint.pose.scale[0] = 1.0f;
		sJoint.pose.scale[1] = 1.0f;
		sJoint.pose.scale[2] = 1.0f;

		sJoint.pose.quat.Set(0.0f, 0.0f, 0.0f, 1.0f);

		IZ_BOOL bHasQuatFromAxisRot = IZ_FALSE;

		for (size_t n = 0; n < tvTransform.size(); n++) {
			SJointTransform& sTransform = tvTransform[n];

			IZ_ASSERT(sTransform.param.size() > 0);

			if (sTransform.type == E_MDL_JOINT_TRANSFORM_TRANSLATE) {
				IZ_ASSERT(sTransform.param.size() >= 3);
				sJoint.pose.trans[0] = sTransform.param[0];
				sJoint.pose.trans[1] = sTransform.param[1];
				sJoint.pose.trans[2] = sTransform.param[2];

				sJoint.validParam |= izanagi::E_SKL_JOINT_PARAM_TRANSLATE;
			}
			else if (sTransform.type == E_MDL_JOINT_TRANSFORM_SCALE) {
				IZ_ASSERT(sTransform.param.size() >= 3);
				sJoint.pose.scale[0] = sTransform.param[0];
				sJoint.pose.scale[1] = sTransform.param[1];
				sJoint.pose.scale[2] = sTransform.param[2];

				sJoint.validParam |= izanagi::E_SKL_JOINT_PARAM_SCALE;
			}
			else if (sTransform.type == E_MDL_JOINT_TRANSFORM_AXIS_ROT) {
				IZ_FLOAT x = sTransform.param[0];
				IZ_FLOAT y = sTransform.param[1];
				IZ_FLOAT z = sTransform.param[2];
				IZ_FLOAT angle = sTransform.param[3];

				izanagi::math::SQuat quat;
				izanagi::math::SQuat::SetQuatFromRadAxis(quat, angle, x, y, z);

				izanagi::math::SMatrix mtx;
				izanagi::math::SQuat::MatrixFromQuat(mtx, quat);

				izanagi::math::SMatrix::Mul(mtxRot, mtxRot, mtx);

				bHasQuatFromAxisRot = IZ_TRUE;
			}
			else if (sTransform.type == E_MDL_JOINT_TRANSFORM_QUATERNION) {
				IZ_ASSERT(sTransform.param.size() == 4);
				sJoint.pose.quat.x = sTransform.param[0];
				sJoint.pose.quat.y = sTransform.param[1];
				sJoint.pose.quat.z = sTransform.param[2];
				sJoint.pose.quat.w = sTransform.param[3];

				sJoint.validParam |= izanagi::E_SKL_JOINT_PARAM_QUARTANION;
			}
			else {
				VRETURN(IZ_FALSE);
			}
		}

		if (bHasQuatFromAxisRot) {
			izanagi::math::SQuat quat;
			izanagi::math::SQuat::QuatFromMatrix(quat, mtxRot);

			izanagi::math::SQuat::Copy(sJoint.pose.quat, quat);

			sJoint.validParam |= izanagi::E_SKL_JOINT_PARAM_QUARTANION;
		}

		IZ_OUTPUT_WRITE_VRETURN(pOut, &sJoint, 0, sizeof(sJoint));
	}

	return IZ_TRUE;
}
