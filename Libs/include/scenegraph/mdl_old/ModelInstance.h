#if !defined(__IZANAGI_SCENEGRAPH_MODEL_INSTANCE_H__)
#define __IZANAGI_SCENEGRAPH_MODEL_INSTANCE_H__

#include "izStd.h"
#include "izGraph.h"

#include "scenegraph/mdl/MdlSkeleton.h"
#include "scenegraph/mdl/MdlRenderHandler.h"

namespace izanagi {
	class CModel;

	/**
	*/
	class CModelInstance : public CObject {
	public:
		// Create instance.
		static CModelInstance* CreateModelInstance(
			IMemoryAllocator* pAllocator,
			CModel* pMdl,
			IMdlRenderHandler* pRenderHandler);

	protected:
		CModelInstance();
		~CModelInstance();

		NO_COPIABLE(CModelInstance);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		IZ_BOOL Render();

		void Update();

		void ApplyAnimation(
			IZ_FLOAT fTime,
			CAnimation* pAnm);

		void ApplyAnimationByIdx(
			IZ_UINT nJointIdx,
			IZ_FLOAT fTime,
			CAnimation* pAnm);

		void ApplyAnimationByName(
			IZ_PCSTR pszJointName,
			IZ_FLOAT fTime,
			CAnimation* pAnm);

		void ApplyAnimationByKey(
			IZ_UINT nJointKey,
			IZ_FLOAT fTime,
			CAnimation* pAnm);

	public:
		CMdlSkeleton* GetSkeleton() { return m_pSkeleton; }
		void SetRenderHandler(IMdlRenderHandler* pRenderHandler) { SAFE_REPLACE(m_pRenderHandler, pRenderHandler); }

	protected:
		CModel* m_pMaster;

		IMemoryAllocator* m_pAllocator;

		CMdlSkeleton* m_pSkeleton;

		IMdlRenderHandler* m_pRenderHandler;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MODEL_INSTANCE_H__)
