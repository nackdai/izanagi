#if !defined(__IZANAGI_SCENEGRAPH_SKELETON_INSTANCE_H__)
#define __IZANAGI_SCENEGRAPH_SKELETON_INSTANCE_H__

#include "SKLFormat.h"
#include "izMath.h"

namespace izanagi {
	class IInputStream;
	class CSkeleton;
	class IAnimation;
	struct S_ANM_NODE;

	class CSkeletonInstance : public CObject {
		friend class CModel;

	public:
		static CSkeletonInstance* CreateSkeletonInstance(
			IMemoryAllocator* pAllocator,
			CSkeleton* pSkl);

	private:
		static IZ_UINT ComputeBufferSize(const CSkeleton* pSkl);

	private:
		CSkeletonInstance();
		~CSkeletonInstance();

		NO_COPIABLE(CSkeletonInstance);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		void BuildMatrix(const SMatrix* mtxL2W);

		const SMatrix* GetJointMtx(IZ_INT idx) const;

		IZ_UINT GetJointNum() { return m_nJointNum; }

		void ApplyAnimation(
			IZ_FLOAT fTime,
			IAnimation* pAnm);

		void ApplyAnimationByIdx(
			IZ_UINT nJointIdx,
			IZ_FLOAT fTime,
			IAnimation* pAnm);

		// 姿勢情報更新開始
		IZ_BOOL BeginUpdatePose(IZ_UINT idx);

		// 姿勢情報更新終了
		void EndUpdatePose(
			IZ_UINT idx,
			IZ_UINT8 updateFlag);

		// 姿勢情報更新
		void UpdatePose(
			IZ_UINT idx,
			IZ_UINT transformType,
			IZ_UINT paramType,
			const SVector& param);

		// ジョイントキーからジョイントインデックスを逆引きする
		IZ_INT GetJointIdxByKey(IZ_UINT key);

	private:
		IZ_UINT8* SetJointData(IZ_UINT8* pBuf);

		void BuildLocalMatrix(IZ_UINT nIdx);

		inline void ApplyParentMatrix(
			IZ_UINT nIdx,
			IZ_UINT nParentIdx);

		inline void ApplyInvBindMatrix(IZ_UINT nIdx);

	private:
		IMemoryAllocator* m_Allocator;

		// 骨情報本体
		CSkeleton* m_pBody;

		// 関節数
		IZ_UINT m_nJointNum;

		// 姿勢情報
		S_SKL_JOINT_POSE* m_pJointPose;

		// 最終的な行列
		SMatrix* m_pGlobalPose;

		// 姿勢情報を更新中かどうか
		IZ_BOOL m_IsUpdatingPose;
	};
}	// namespace iznagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_SKELETON_INSTANCE_H__)
