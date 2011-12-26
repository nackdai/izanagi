#if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_UTIL_H__)
#define __IZANAGI_SCENEGRAPH_ANIMATION_UTIL_H__

#include "ANMFormat.h"

namespace izanagi {
	class CSkeletonInstance;
	struct S_SKL_JOINT_POSE;

	class CAnimationUtil {
	private:
		CAnimationUtil();
		~CAnimationUtil();

	public:
		static IZ_BOOL IsScalarInterp(IZ_UINT type);

		static IZ_FLOAT ComputeInterp(
			E_ANM_INTERP_TYPE nInterp,
			IZ_FLOAT fTime,
			IZ_UINT nKeyNum,
			IZ_UINT nPos,
			S_ANM_KEY** const pKeys);

		static void ComputeInterp(
			SVector& vRef,
			E_ANM_INTERP_TYPE nInterp,
			IZ_FLOAT fTime,
			IZ_UINT nKeyNum,
			IZ_UINT nPos,
			S_ANM_KEY** const pKeys);

	private:
		static IZ_FLOAT ComputeLinear(
			IZ_FLOAT fTime,
			IZ_UINT nKeyNum,
			IZ_UINT nPos,
			S_ANM_KEY** const pKeys);

		static IZ_FLOAT ComputeBezier(
			IZ_FLOAT fTime,
			IZ_UINT nKeyNum,
			IZ_UINT nPos,
			S_ANM_KEY** const pKeys);

		static IZ_FLOAT ComputeHermite(
			IZ_FLOAT fTime,
			IZ_UINT nKeyNum,
			IZ_UINT nPos,
			S_ANM_KEY** const pKeys);

		static void ComputeSlerp(
			SVector& vRef,
			IZ_FLOAT fTime,
			IZ_UINT nKeyNum,
			IZ_UINT nPos,
			S_ANM_KEY** const pKeys);

	private:
		static IZ_FLOAT ComputeNomralizedTime(
			IZ_FLOAT fTime,
			IZ_INT& nPrev,
			IZ_INT& nNext,
			IZ_UINT nKeyNum,
			S_ANM_KEY** const pKeys);
	};

	class CPoseUpdater {
	public:
		CPoseUpdater(S_SKL_JOINT_POSE* pose);
		virtual ~CPoseUpdater() {}

	protected:
		CPoseUpdater() {}

	public:
		virtual IZ_BOOL BeginUpdate(IZ_UINT idx);
		virtual void EndUpdate(IZ_UINT idx, IZ_UINT updateFlag);
		virtual void Update(
			IZ_UINT idx,
			IZ_UINT transformType,
			IZ_UINT paramType,
			const SVector& param);

	private:
		S_SKL_JOINT_POSE* m_Pose;
	};

	class CSklPoseUpdater : public CPoseUpdater {
	public:
		CSklPoseUpdater(CSkeletonInstance* skl);
		virtual ~CSklPoseUpdater();

	public:
		IZ_BOOL BeginUpdate(IZ_UINT idx);
		void EndUpdate(IZ_UINT idx, IZ_UINT updateFlag);
		void Update(
			IZ_UINT idx,
			IZ_UINT transformType,
			IZ_UINT paramType,
			const SVector& param);

	private:
		CSkeletonInstance* m_Skl;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_UTIL_H__)
