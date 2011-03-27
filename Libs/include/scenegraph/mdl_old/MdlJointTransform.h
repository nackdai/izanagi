#if !defined(__IZANAGI_SCENEGRAPH_MDL_JOINT_TRANSFORM_H__)
#define __IZANAGI_SCENEGRAPH_MDL_JOINT_TRANSFORM_H__

#include "MDLFormat.h"
#include "../anm/ANMFormat.h"

#if 0
namespace izanagi {
	class IInputStream;

	class CMdlJointTransform {
		friend class CMdlSkeleton;

	private:
		CMdlJointTransform() {}
		~CMdlJointTransform() {}

	private:
		inline void Init(S_MDL_JOINT_TRANSFORM* pTransform);

		inline void Reset();

		const S_MDL_JOINT_TRANSFORM& GetTransformInfo() const { return *m_pTransform; }
		const SVector& GetParam() const { return m_Params; }

		SVector& GetParam() { return m_Params; }

	private:
		S_MDL_JOINT_TRANSFORM* m_pTransform;
		SVector m_Params;
	};

	// inline **********************************

	void CMdlJointTransform::Init(S_MDL_JOINT_TRANSFORM* pTransform)
	{
		IZ_ASSERT(pTransform != IZ_NULL);
		m_pTransform = pTransform;
		CopyVector(m_Params, m_pTransform->param);
	}

	void CMdlJointTransform::Reset()
	{
		IZ_ASSERT(m_pTransform != IZ_NULL);
		CopyVector(m_Params, m_pTransform->param);
	}
}	// namespace iznagi
#endif

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MDL_JOINT_TRANSFORM_H__)
