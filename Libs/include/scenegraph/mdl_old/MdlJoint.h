#if !defined(__IZANAGI_SCENEGRAPH_MDL_JOINT_H__)
#define __IZANAGI_SCENEGRAPH_MDL_JOINT_H__

#include "MDLFormat.h"
#include "izMath.h"

namespace izanagi {
	class IInputStream;

	class CMdlJoint : public CObject {
		friend class CModel;
		friend class CMdlSkeleton;

	private:
		static CMdlJoint* CreateMdlJoint(
			IMemoryAllocator* pAllocator,
			IInputStream* pIn);

		static IZ_UINT ComputeBufferSize(
			const S_MDL_JOINT_CHUNK_HEADER& sHeader);

	private:
		CMdlJoint();
		~CMdlJoint();

		NO_COPIABLE(CMdlJoint);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		IZ_UINT GetJointNum() const { return m_Header.numJoint; }

	private:
		IZ_UINT8* ReadJoint(
			IZ_UINT8* pBuf,
			IInputStream* pIn);

		const S_MDL_JOINT* GetJoint(IZ_INT idx) const;

		const S_MDL_JOINT_CHUNK_HEADER& GetHeader() const { return m_Header; }

	private:
		IMemoryAllocator* m_pAllocator;

		S_MDL_JOINT_CHUNK_HEADER m_Header;

		S_MDL_JOINT* m_pJoint;
	};
}	// namespace iznagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MDL_JOINT_H__)
