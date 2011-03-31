#if !defined(__IZANAGI_SCENEGRAPH_SCENE_PARAM_H__)
#define __IZANAGI_SCENEGRAPH_SCENE_PARAM_H__

#include "izStd.h"
#include "Camera.h"
#include "LightParam.h"

namespace izanagi {
	/**
	* シーンパラメータ
	*/
	class CSceneParam : public CObject {
	public:
		// インスタンス作成
		static CSceneParam* CreateSceneParam(
			IMemoryAllocator* pAllocator,
			IZ_UINT nParallelLightNum,
			IZ_UINT nPointLightNum,
			IZ_UINT nSpotLightNum);

	protected:
		CSceneParam();
		~CSceneParam() {}

		NO_COPIABLE(CSceneParam);

	protected:
		// 解放
		inline void InternalRelease();

	public:
		inline SParallelLightParam* GetParallelLightParam(IZ_UINT idx);
		inline SPointLightParam* GetPointLightParam(IZ_UINT idx);
		inline SSpotLightParam* GetSpotLightParam(IZ_UINT idx);

		inline SAmbientLightParam* GetAmbientLightParam();
		inline SHemisphereLightParam* GetHemisphereLightParam();

		inline IZ_UINT GetParallelLightNum() const;
		inline IZ_UINT GetPointLightNum() const;
		inline IZ_UINT GetSpotLightNum() const;

	protected:
		IMemoryAllocator* m_pAllocator;

		SParallelLightParam* m_pParallel;	// 平行光源
		SPointLightParam* m_pPoint;			// 点光源
		SSpotLightParam* m_pSpot;			// スポットライト

		SAmbientLightParam m_sAmbient;			// 環境光
		SHemisphereLightParam m_sHemisphere;	// 半球ライト

		// ライト数
		IZ_UINT m_nParallelLightNum;		// 平行光源
		IZ_UINT m_nPointLightNum;			// 点光源
		IZ_UINT m_nSpotLightNum;			// スポットライト
	};

	// inline ***********************************

	// 解放
	void CSceneParam::InternalRelease()
	{
		delete this;
		FREE(m_pAllocator, this);
	}

	/**
	* 平行光源パラメータ取得
	*/
	SParallelLightParam* CSceneParam::GetParallelLightParam(IZ_UINT idx)
	{
		IZ_ASSERT(idx < m_nParallelLightNum);
		IZ_ASSERT(m_pParallel != IZ_NULL);
		return &m_pParallel[idx];
	}

	/**
	* 点光源パラメータ取得
	*/
	SPointLightParam* CSceneParam::GetPointLightParam(IZ_UINT idx)
	{
		IZ_ASSERT(idx < m_nPointLightNum);
		IZ_ASSERT(m_pPoint != IZ_NULL);
		return &m_pPoint[idx];
	}

	/**
	* スポットライトパラメータ取得
	*/
	SSpotLightParam* CSceneParam::GetSpotLightParam(IZ_UINT idx)
	{
		IZ_ASSERT(idx < m_nSpotLightNum);
		IZ_ASSERT(m_pSpot != IZ_NULL);
		return &m_pSpot[idx];
	}

	/**
	* 環境光パラメータ取得
	*/
	SAmbientLightParam* CSceneParam::GetAmbientLightParam()
	{
		return &m_sAmbient;
	}

	/**
	* 半球ライトパラメータ取得
	*/
	SHemisphereLightParam* CSceneParam::GetHemisphereLightParam()
	{
		return &m_sHemisphere;
	}

	/**
	* 平行光源数取得
	*/
	IZ_UINT CSceneParam::GetParallelLightNum() const
	{
	}

	/**
	* 点光源数取得
	*/
	IZ_UINT CSceneParam::GetPointLightNum() const
	{
		return m_nPointLightNum;
	}

	/**
	* スポットライト数取得
	*/
	IZ_UINT CSceneParam::GetSpotLightNum() const
	{
		return m_nSpotLightNum;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_SCENE_PARAM_H__)
