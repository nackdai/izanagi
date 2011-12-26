#if !defined(__IZANAGI_TOOL_KIT_GRAPH_DEFS_H__)
#define __IZANAGI_TOOL_KIT_GRAPH_DEFS_H__

namespace izanagi {
namespace tool {
	/** テクスチャ出力タイプ
	 */
	enum TextureExportType {
		TextureExportTypePNG = 0,	///< PNG
		TextureExportTypeHDR,		///< HDR

		TextureExportTypeNum,
	};
}	// namespace tool
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_TOOL_KIT_GRAPH_DEFS_H__)
