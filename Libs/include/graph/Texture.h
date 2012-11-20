#if !defined(__IZANAGI_GRAPH_TEXTURE_H__)
#define __IZANAGI_GRAPH_TEXTURE_H__

#include "izDefs.h"
#include "izStd.h"
#include "graph/BaseTexture.h"
#include "graph/Surface.h"

namespace izanagi
{
namespace graph
{
    /**
     */
	class CTexture : public CBaseTexture
    {
	protected:
        CTexture() {}
        virtual ~CTexture() {}

		NO_COPIABLE(CTexture);

	public:
		// ロック
        PURE_VIRTUAL(
		    IZ_UINT Lock(
			    UINT level,
			    void** data,
			    IZ_BOOL isReadOnly,
			    IZ_BOOL isDiscard = IZ_FALSE));

		// アンロック
		PURE_VIRTUAL(IZ_BOOL Unlock(IZ_UINT level));

		// サーフェス取得
		PURE_VIRTUAL(CSurface* GetSurface(IZ_UINT idx));
	};
}   // namespace graph
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_TEXTURE_H__)
