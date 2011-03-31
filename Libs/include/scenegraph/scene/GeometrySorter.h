#if !defined(__IZANAGI_SCENEGRAPH_GEOMETRY_SORTER_H__)
#define __IZANAGI_SCENEGRAPH_GEOMETRY_SORTER_H__

#include "izStd.h"
#include "izGraph.h"
#include "scenegraph/SceneDefs.h"
#include "scenegraph/msh/MshRenderHandler.h"
#include "scenegraph/scene/Scene.h"

namespace izanagi {
	/**
	*/
	template <IZ_UINT _ZLEVEL, typename _T_MDL, typename _T_MESH>
	class CGeometrySorter : public CObject {
	protected:
		// List ordered by Z.
		class CZList {
		public:
			CZList() {}
			~CZList() {}

		public:
			// Add mesh to list.
			IZ_BOOL Add(
				IZ_FLOAT fAddZ,
				_T_MESH* pAddMesh)
			{
				IZ_BOOL ret = IZ_FALSE;

				CStdList<_T_MESH>::Item* p = m_List.GetTop();
				if (p == IZ_NULL) {
					// List is empty.
					ret = m_List.AddTop(pAddMesh->GetListItem());
					return ret;
				}

				// Sort by Z value.
				while (p != IZ_NULL) {
					const _T_MESH* pMesh = p->GetData();

					IZ_FLOAT fZ = pMesh->GetZ();

					if (fAddZ >= fZ) {
						ret = pAddMesh->GetListItem()->JoinAfter(p);
						break;
					}

					p = p->GetNext();
				}

				if (!ret) {
					// Z value is larger than all items.
					ret = m_List.AddTail(pAddMesh->GetListItem());
				}

				IZ_ASSERT(ret);
				return ret;
			}

			void Clear() { m_List.Clear(); }

			CStdList<_T_MESH>& GetList() { return m_List; }

		private:
			CStdList<_T_MESH> m_List;
		};

		enum ERenderOrder {
			ERenderOrder_Forward = 0,
			ERenderOrder_Reverse,
			ERenderOrder_Num,
			ERenderOrder_FORCE_INT32 = 0x7fffffff,
		};

	public:
		/**
		*/
		static CGeometrySorter* CreateGeometrySorter(IMemoryAllocator* pAllocator)
		{
			IZ_ASSERT(pAllocator != IZ_NULL);
			void* buf = ALLOC_ZERO(pAllocator, sizeof(CGeometrySorter));
			VRETURN_NULL(buf != IZ_NULL);
			CGeometrySorter* pInstance = new(buf) CGeometrySorter;
			pInstance->AddRef();
			pInstance->m_pAllocator = pAllocator;
			return pInstance;
		}

	protected:
		CGeometrySorter()
		{
			m_pAllocator = IZ_NULL;
		}

		~CGeometrySorter()
		{
			Clear();
		}

		NO_COPIABLE(CGeometrySorter);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		/**
		*/
		void BeginRegister()
		{
			Clear();
		}

		/**
		*/
		void EndRegister()
		{
			// Nothing is done...
		}

		/**
		*/
		IZ_BOOL Register(
			IZ_FLOAT z,
			_T_MDL* pMdl,
			E_SCENE_REGISTER_TYPE type)
		{
			IZ_UINT level = static_cast<IZ_UINT>(z * _ZLEVEL);
			return RegisterInternal(z, level, pMdl, type);
		}

		/**
		*/
		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			CScene* pScene,
			IMshRenderHandler* pRenderHandler)
		{
			// Opaque
			for (IZ_UINT i = 0; i < _ZLEVEL; i++) {
				VRETURN(Render(pDevice, pScene, pRenderHandler, m_Opaque[i], ERenderOrder_Forward));
			}

			// Tranlucent
			for (IZ_UINT i = 0; i < _ZLEVEL; i++) {
				VRETURN(Render(pDevice, pScene, pRenderHandler, m_Translucent[i], ERenderOrder_Reverse));
			}

			return IZ_TRUE;
		}

		/**
		*/
		CStdList<_T_MESH>& GetOpaqueList() { return m_Opaque; }

		/**
		*/
		CStdList<_T_MESH>& GetTranslucentList() { return m_Translucent; }

	protected:
		IZ_BOOL RegisterInternal(
			IZ_FLOAT z,
			IZ_UINT level,
			_T_MDL* pMdl,
			E_SCENE_REGISTER_TYPE type)
		{
			IZ_ASSERT(level < _ZLEVEL);

			IZ_BOOL ret = IZ_TRUE;

			IZ_UINT nMshSetNum = pMdl->GetMeshSetNum();

			for (IZ_UINT i = 0; i < nMshSetNum; i++) {
				_T_MESH* pMesh = pMdl->GetMeshSet(i);

				// TODO
				pMesh->SetZ(z);

				if (type == E_SCENE_REGISTER_TYPE_NORMAL) {
					type = (pMesh->IsTranslucent()
							? E_SCENE_REGISTER_TYPE_TRANSLUCENT
							: E_SCENE_REGISTER_TYPE_OPACITY);
				}

				switch (type) {
				case E_SCENE_REGISTER_TYPE_OPACITY:
					ret = m_Opaque[level].Add(z, pMesh);
					break;
				case E_SCENE_REGISTER_TYPE_TRANSLUCENT:
					ret = m_Translucent[level].Add(z, pMesh);
					break;
				}

				VRETURN(ret);
			}

			return ret;
		}

		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			CScene* pScene,
			IMshRenderHandler* pRenderHandler,
			CZList& cList,
			ERenderOrder nOrder)
		{
			// Get top or tail of ZList.
			CStdList<_T_MESH>::Item* pItem = (nOrder == ERenderOrder_Forward
												? cList.GetList().GetTop()
												: cList.GetList().GetTail());

			for (; pItem != IZ_NULL;) {
				_T_MESH* pMesh = pItem->GetData();
				IZ_ASSERT(pMesh != IZ_NULL);

				IZ_UINT nPassNum = pScene->Begin(pMesh);

				// Keep loop top.
				CStdList<_T_MESH>::Item* pItemBegin = pItem;

				for (IZ_UINT nPass = 0; nPass < nPassNum; nPass++) {
					while (pItem != IZ_NULL) {
						pMesh = pItem->GetData();

						if (pScene->Iter(nPass, pMesh)) {
							pItem = (nOrder == ERenderOrder_Forward
										? pItem->GetNext()
										: pItem->GetPrev());
						}
						else {
							if (!result && (nPass == nPassNum - 1)) {
								// Change shader.
								VRETURN(pScene->End());
								break;
							}

							// Return to loop top.
							pItem = pItemBegin;
						}
					}
				}
			}

			return IZ_TRUE;
		}

		void Clear()
		{
			for (IZ_UINT i = 0; i < _ZLEVEL; i++) {
				m_Opaque[i].Clear();
				m_Translucent[i].Clear();
			}
		}

	protected:
		IMemoryAllocator* m_pAllocator;

		CZList m_Opaque[_ZLEVEL];
		CZList m_Translucent[_ZLEVEL];
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_GEOMETRY_SORTER_H__)
