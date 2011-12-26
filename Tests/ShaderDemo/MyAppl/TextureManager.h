#if !defined(__TEXTURE_MANAGER_H__)
#define __TEXTURE_MANAGER_H__

#include "izStd.h"
#include "izGraph.h"

enum DEMO_TEX {
	DEMO_TEX_EARTH = 0,
	DEMO_TEX_TILE,
	DEMO_TEX_WALL,
	DEMO_TEX_STONES,
	DEMO_TEX_WOOD,
	DEMO_TEX_ROCKS,

	DEMO_TEX_ALBEDO_MAX,
	DEMO_TEX_ALBEDO_NUM = DEMO_TEX_ALBEDO_MAX,

	DEMO_TEX_NMH_SAINT = DEMO_TEX_ALBEDO_NUM,
	DEMO_TEX_NMH_ROCKS,
	DEMO_TEX_NMH_FOUR,
	DEMO_TEX_NMH_STONES,
	DEMO_TEX_NMH_WALL,

	DEMO_TEX_NMH_MAX,
	DEMO_TEX_NMH_NUM = DEMO_TEX_NMH_MAX - DEMO_TEX_ALBEDO_MAX - 1,

	DEMO_TEX_NUM = DEMO_TEX_NMH_MAX,
};

namespace izanagi {
	class CTextureManager {
	private:
		static CTextureManager s_cInstance;

	public:
		static CTextureManager& GetInstance() { return s_cInstance; }

	private:
		CTextureManager();
		~CTextureManager();

	public:
		IZ_BOOL Init(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice);

		void Clear();

		IZ_INT Read(IZ_PCSTR pszFile);

		CTexture* Get(IZ_UINT nIdx);

	public:
		IZ_BOOL AllRead();

	private:
		class CTexHolder : public izanagi::CPlacementNew {
		public:
			CTexHolder() {
				m_pTex = IZ_NULL;
				m_ListItem.Init(this);
			}
			~CTexHolder() {}

		public:
			void SetTexture(CTexture* p) { m_pTex = p; }

			CTexture* GetTexture() { return m_pTex; }
			CStdList<CTexHolder>::Item* GetListItem() { return &m_ListItem; }

			void Release() { SAFE_RELEASE(m_pTex); }

		private:
			CTexture* m_pTex;
			CStdList<CTexHolder>::Item m_ListItem;
		};

	private:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		CStdList<CTexHolder> m_List;
	};
}	// namespace izanagi

#endif	// #if !defined(__TEXTURE_MANAGER_H__)
