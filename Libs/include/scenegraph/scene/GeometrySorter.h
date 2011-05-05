﻿#if !defined(__IZANAGI_SCENEGRAPH_GEOMETRY_SORTER_H__)
#define __IZANAGI_SCENEGRAPH_GEOMETRY_SORTER_H__

#include "izStd.h"
#include "izGraph.h"
#include "scenegraph/SceneDefs.h"
#include "scenegraph/msh/MshRenderHandler.h"
#include "scenegraph/scene/Scene.h"

namespace izanagi {
	class CMeshSetInstance;
	class CModel;

	/**
	 * ジオメトリソートを行うクラス
	 */
	class CGeometrySorter : public CObject {
	protected:
		// List ordered by Z.
		class CZList : public CPlacementNew {
		public:
			CZList() {}
			~CZList() {}

		public:
			// Add mesh to list.
			IZ_BOOL Add(
				IZ_FLOAT fAddZ,
				CMeshSetInstance* pAddMesh);

			void Clear() { m_List.Clear(); }

			CStdList<CMeshSetInstance>& GetList() { return m_List; }

		private:
			CStdList<CMeshSetInstance> m_List;
		};

		enum ERenderOrder {
			ERenderOrder_Forward = 0,
			ERenderOrder_Reverse,
			ERenderOrder_Num,
			ERenderOrder_FORCE_INT32 = 0x7fffffff,
		};

	public:
		/**
		 * インスタンス作成
		 */
		static CGeometrySorter* CreateGeometrySorter(
			IMemoryAllocator* pAllocator,
			IZ_UINT nZLevel);

	protected:
		CGeometrySorter();
		~CGeometrySorter();

		NO_COPIABLE(CGeometrySorter);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		/**
		 * モデル登録開始
		 */
		void BeginRegister()
		{
			Clear();
		}

		/**
		 * モデル登録終了
		 */
		void EndRegister()
		{
			// Nothing is done...
		}

		/**
		 * モデル登録
		 */
		IZ_BOOL Register(
			IZ_FLOAT z,
			CModel* pMdl,
			E_SCENE_REGISTER_TYPE type);

		/**
		 * 描画
		 */
		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			CScene* pScene,
			IMshRenderHandler* pRenderHandler);
		
	protected:
		// 登録内部処理
		IZ_BOOL RegisterInternal(
			IZ_FLOAT z,
			IZ_UINT level,
			CModel* pMdl,
			E_SCENE_REGISTER_TYPE type);

		// 描画内部処理
		IZ_BOOL RenderInternal(
			CGraphicsDevice* pDevice,
			CScene* pScene,
			IMshRenderHandler* pRenderHandler,
			CZList& cList,
			ERenderOrder nOrder);
		
		void Clear()
		{
			for (IZ_UINT i = 0; i < m_nZLevvel; i++) {
				m_Opaque[i]->Clear();
				m_Translucent[i]->Clear();
			}
		}

	protected:
		IMemoryAllocator* m_pAllocator;

		IZ_UINT m_nZLevvel;

		CZList** m_Opaque;
		CZList** m_Translucent;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_GEOMETRY_SORTER_H__)
