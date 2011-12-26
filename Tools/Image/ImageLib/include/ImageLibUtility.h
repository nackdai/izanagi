#if !defined(__IZANAGI_IMAGE_LIB_IMAGE_LIB_COMMON_H__)
#define __IZANAGI_IMAGE_LIB_IMAGE_LIB_COMMON_H__

#include "izDefs.h"
#include "izStd.h"
#include "izSceneGraph.h"

namespace izanagi {
namespace tool {

	template <typename _T>
	union S_PIXEL_VAL {
		struct {
			_T r;
			_T g;
			_T b;
			_T a;
		};
		_T val[4];
	};

	struct S_A8_VAL {
		IZ_BYTE a;
	};
	IZ_C_ASSERT(sizeof(S_A8_VAL) == sizeof(IZ_BYTE));

	struct S_R32F_VAL {
		IZ_FLOAT r;
	};
	IZ_C_ASSERT(sizeof(S_R32F_VAL) == sizeof(IZ_FLOAT));

	union S_RGBA4_VAL {
		IZ_UINT16 val;
		struct {
			IZ_UINT16 r : 4;
			IZ_UINT16 g : 4;
			IZ_UINT16 b : 4;
			IZ_UINT16 a : 4;
		};
	};
	IZ_C_ASSERT(sizeof(S_RGBA4_VAL) == sizeof(IZ_UINT16));

	union S_RGB10A2_VAL {
		IZ_UINT32 val;
		struct {
			IZ_UINT32 r : 10;
			IZ_UINT32 g : 10;
			IZ_UINT32 b : 10;
			IZ_UINT32 a : 2;
		};
	};

	const IZ_UINT8  RGBA8_MAX           = 0xff;
	const IZ_FLOAT  DIV_RGBA8_MAX       = 1.0f / RGBA8_MAX;
	const IZ_UINT32 RGB10A2_RGB_MAX     = 0x000003ff;
	const IZ_FLOAT  DIV_RGB10A2_RGB_MAX = 1.0f / RGB10A2_RGB_MAX;
	const IZ_UINT32 RGB10A2_A_MAX       = 0x00000003;
	const IZ_FLOAT  DIV_RGB10A2_A_MAX   = 1.0f / 0x00000003;

	/**
	*/
	class CImageUtil {
	private:
		CImageUtil();
		~CImageUtil();
		NO_COPIABLE(CImageUtil);

	public:
		static inline IZ_BOOL IsDXT(E_GRAPH_PIXEL_FMT nFmt)
		{
			return ((nFmt == E_GRAPH_PIXEL_FMT_DXT1)
					|| (nFmt == E_GRAPH_PIXEL_FMT_DXT3)
					|| (nFmt == E_GRAPH_PIXEL_FMT_DXT5));
		}

		static inline IZ_UINT GetBpp(E_GRAPH_PIXEL_FMT nFmt)
		{
			IZ_UINT ret = 0;

			switch (nFmt) {
			case E_GRAPH_PIXEL_FMT_RGBA8:
			case E_GRAPH_PIXEL_FMT_BGRA8:
			case E_GRAPH_PIXEL_FMT_RGB10A2:
			case E_GRAPH_PIXEL_FMT_R32F:
				ret = 4;
				break;
			case E_GRAPH_PIXEL_FMT_RGBA4:
				ret = 2;
				break;
			case E_GRAPH_PIXEL_FMT_A8:
				ret = 1;
				break;
			case E_GRAPH_PIXEL_FMT_RGBA16F:
				ret = 8;
				break;
			case E_GRAPH_PIXEL_FMT_RGBA32F:
				ret = 16;
				break;
			default:
				IZ_ASSERT(IZ_FALSE);
				break;
			}

			return ret;
		}

		static inline void SetImageHeader(
			S_IMG_HEADER* pHeader,
			IZ_UINT nTexNum)
		{
			pHeader->magic = IMG_MAGIC_NUMBER;
			pHeader->version = E_IMG_VERSION_CURRENT;

			pHeader->sizeHeader = sizeof(S_IMG_HEADER);
			pHeader->numTextures = nTexNum;
			pHeader->sizeJumpTbl = sizeof(IZ_UINT) * pHeader->numTextures;

			pHeader->sizeMax = 0;
		}
	};
}	// namespace tool
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IMAGE_LIB_IMAGE_LIB_COMMON_H__)
