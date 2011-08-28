#if !defined(__IZANAGI_SCENEGRAPH_IZ_SKELETON_H__)
#define __IZANAGI_SCENEGRAPH_IZ_SKELETON_H__

#include "SKLFormat.h"
#include "izMath.h"

namespace izanagi {
	class IInputStream;

	class CSkeleton : public CObject {
		friend class CSkeletonInstance;

	public:
		static CSkeleton* CreateSkeleton(
			IMemoryAllocator* pAllocator,
			IInputStream* pIn);

	private:
		static IZ_UINT ComputeBufferSize(
			const S_SKL_HEADER& sHeader);

	private:
		CSkeleton();
		~CSkeleton();

		NO_COPIABLE(CSkeleton);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		IZ_UINT GetJointNum() const { return m_nJointNum; }

		const S_SKL_JOINT* GetJoint(IZ_INT idx) const;

	private:
		IZ_UINT8* ReadJoint(
			IZ_UINT8* pBuf,
			IInputStream* pIn);

		S_SKL_JOINT* GetJoint(IZ_INT idx);

	private:
		IMemoryAllocator* m_pAllocator;

		IZ_UINT m_nJointNum;
		S_SKL_JOINT* m_pJoint;
	};
}	// namespace iznagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_IZ_SKELETON_H__)
