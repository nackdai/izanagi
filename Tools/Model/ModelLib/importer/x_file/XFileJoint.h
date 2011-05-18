#if !defined(__MODEL_LIB_XFILE_JOINT_H__)
#define __MODEL_LIB_XFILE_JOINT_H__

#include <vector>
#include "izSceneGraph.h"
#include "XFileParser.h"

struct SJointTransform;

class CXFileJoint {
	static CXFileJoint* s_pInstance;

private:
	struct SJoint {
		std::string name;		
		izanagi::S_SKL_JOINT_POSE pose;

		IZ_BOOL isMtxInvFromSkin;
		izanagi::SMatrix mtxInv;

		IZ_INT parent;

		SJoint()
		{
			isMtxInvFromSkin = IZ_FALSE;
			parent = -1;
		}

		bool operator==(const std::string& strName)
		{
			return (name.compare(strName) == 0);
		}
	};

public:
	static CXFileJoint& GetInstance();
	static void DeleteInstance();

private:
	CXFileJoint();
	~CXFileJoint();

public:
	void Clear();

	IZ_BOOL Init(CXFileParser* pParser);

	IZ_UINT GetJointNum();

	IZ_PCSTR GetJointName(IZ_UINT nIdx);

	IZ_INT GetJointParent(IZ_UINT nIdx);

	void GetJointInvMtx(
		IZ_UINT nIdx,
		izanagi::SMatrix& mtx);

	void GetJointTransform(
		IZ_UINT nIdx,
		std::vector<SJointTransform>& tvTransform);

private:
	std::vector<SJoint> m_JointList;
};

#endif	// #if !defined(__MODEL_LIB_XFILE_JOINT_H__)
