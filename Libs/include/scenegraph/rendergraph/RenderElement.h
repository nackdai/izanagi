#if !defined(__IZANAGI_SCENEGRAPH_RENDER_ELEMENT_H__)
#define __IZANAGI_SCENEGRAPH_RENDER_ELEMENT_H__

#include "izStd.h"
#include "scenegraph/mtrl/izMaterial.h"

namespace izanagi {
	class CGraphicsDevice;

	/**
	 */
	class IRenderElement : public CObject {
		friend class CRenderGraph;
		friend class CStdSet<IRenderElement>;

	protected:
		IRenderElement() : m_Z(0.0f) { m_Item.Init(this); }
		virtual ~IRenderElement() {}

		NO_COPIABLE(IRenderElement);

	public:
		/** •`‰æ.
		 */
		virtual IZ_BOOL Render(
			CGraphicsDevice* device,
			void* additional) = 0;

		/** ”¼“§–¾‚©‚Ç‚¤‚©‚ğæ“¾.
		 */
		virtual IZ_BOOL IsTranslucent() const = 0;

		/** İ’è‚³‚ê‚Ä‚¢‚éƒ}ƒeƒŠƒAƒ‹‚ğæ“¾.
		 */
		virtual CMaterial* GetMaterial() { return IZ_NULL; }

	public:
		/**
		 */
		void SetZ(IZ_FLOAT z) { m_Z = z; }

		/**
		 */
		IZ_FLOAT GetZ() const { return m_Z; }

	protected:
		CStdSet<IRenderElement>::Item* GetListItem() { return &m_Item; }

		IZ_BOOL operator>(IRenderElement& rhs)
		{
			return m_Z > rhs.m_Z;
		}

	private:
		IZ_FLOAT m_Z;
		CStdSet<IRenderElement>::Item m_Item;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_RENDER_ELEMENT_H__)
