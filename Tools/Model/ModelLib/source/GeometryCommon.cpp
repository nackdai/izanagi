#include "GeometryCommon.h"
#include "izStd.h"

///////////////////////////////////////

IZ_UINT STri::ComputeKey() const
{
	static IZ_UINT nJointTbl[4];

	IZ_UINT ret = 0;

	if (!joint.empty()) {
		size_t num = joint.size();

		std::set<IZ_UINT>::const_iterator it = joint.begin();
		for (size_t n = 0; n < num; n++, it++) {
			nJointTbl[n] = *it;
		}

		// Generate key value by joint indices.
		ret = izanagi::CKey::GenerateValue(
				nJointTbl,
				(IZ_UINT)num);
	}

	return ret;
}

IZ_UINT STri::GetJointNum() const
{
	size_t ret = 0;

	if (!joint.empty()) {
		ret = joint.size();
	}

	return (IZ_UINT)ret;
}

void STri::EraseJoint(IZ_UINT idx)
{
	std::set<IZ_UINT>::iterator it = joint.begin();

	for (; it != joint.end(); it++) {
		if (*it == idx) {
			joint.erase(it);
			break;
		}
	}
}

///////////////////////////////////////

namespace {
	template <typename _T>
	void _EraseItem(
		std::vector<_T>& tvList,
		size_t pos)
	{
		std::vector<_T>::iterator it = tvList.begin();
		std::advance(it, pos);
		tvList.erase(it);
	}
}	// namespace

void SSkin::Add(IZ_UINT nJointIdx, IZ_FLOAT fWeight)
{
	if (weight.size() < 4) {
		joint.push_back(nJointIdx);
		weight.push_back(fWeight);
	}
	else {
		// If num of skin is over 4, num of skin is limited to 4 by weight.
		size_t nMinIdx = 0;
		IZ_FLOAT fMinWeight = weight[0];

		for (size_t i = 1; i < weight.size(); i++) {
			IZ_FLOAT f = weight[i];
			if (fMinWeight > f) {
				fMinWeight = f;
				nMinIdx = i;
			}
		}

		if (fWeight > fMinWeight) {
			_EraseItem(joint, nMinIdx);
			_EraseItem(weight, nMinIdx);

			joint.push_back(nJointIdx);
			weight.push_back(fWeight);
		}
	}
}

void SSkin::Normalize()
{
	IZ_FLOAT fWeightSum = 0.0f;
	for (size_t i = 0; i < weight.size(); i++) {
		fWeightSum += weight[i];
	}

	for (size_t i = 0; i < weight.size(); i++) {
		weight[i] /= fWeightSum;
	}
}

IZ_BOOL SSkin::EraseJoint(IZ_UINT idx)
{
	for (size_t i = 0; i < joint.size(); i++) {
		if (joint[i] == idx) {
			std::vector<IZ_UINT>::iterator itJoint = joint.begin();
			std::advance(itJoint, i);
			joint.erase(itJoint);

			std::vector<IZ_FLOAT>::iterator itWeight = weight.begin();
			std::advance(itWeight, i);
			weight.erase(itWeight);

			return IZ_TRUE;
		}
	}

	return IZ_FALSE;
}

////////////////////////////////////////////

std::vector<STri>* SPrimSet::ptrTriList = IZ_NULL;

#if 0
bool SPrimSet::operator==(const SPrimSet& rhs)
{
	std::vector<STri>& tvTriList = *GetTriList();

	const STri& sTri = tvTriList[tri[0]];
	const STri& sTri_Rhs = tvTriList[rhs.tri[0]];

	bool ret = (sTri.GetJointNum() > sTri_Rhs.GetJointNum());

	if (ret) {
		size_t nMatchCnt = 0;

		std::set<IZ_UINT>::const_iterator itRhs = sTri_Rhs.joint.begin();
		for (; itRhs != sTri_Rhs.joint.end(); itRhs++) {
			IZ_UINT nRhsJoint = *itRhs;

			std::set<IZ_UINT>::const_iterator it = sTri.joint.begin();
			for (; it != sTri.joint.end(); it++) {
				IZ_UINT nJoint = *it;

				if (nJoint == nRhsJoint) {
					nMatchCnt++;
					break;
				}
			}
		}

		ret = (nMatchCnt == sTri_Rhs.GetJointNum());
	}

	return ret;
}
#else
bool SPrimSet::operator==(const SPrimSet& rhs)
{
	return (this == &rhs);
}
#endif
