#include "MdlExporter.h"
#include "GeometryChunk.h"
#include "JointChunk.h"
#include "izToolKit.h"

// フォーマット
// +--------------------+
// |   ファイルヘッダ   |
// +--------------------+
// | ジオメトリチャンク |
// +--------------------+
// | ジョイントチャンク |
// +--------------------+
// | マテリアルチャンク |
// +--------------------+

CMdlExporter CMdlExporter::s_cInstance;

CMdlExporter::CMdlExporter()
{
}

CMdlExporter::~CMdlExporter()
{
	m_Out.Finalize();
}

IZ_BOOL CMdlExporter::Export(
	IZ_UINT maxJointMtxNum,
	IZ_PCSTR lpszOutFile,
	IImporter* pImporter)
{
	IZ_BOOL ret = IZ_TRUE;

	VRETURN(m_Out.Open(lpszOutFile));

	izanagi::S_MDL_HEADER sHeader;
	{
		FILL_ZERO(&sHeader, sizeof(sHeader));

		sHeader.sizeHeader = sizeof(sHeader);
	}

	// Blank for file's header.
	izanagi::izanagi_tk::CIoStreamSeekHelper cSeekHelper(&m_Out);
	VGOTO(ret = cSeekHelper.Skip(sizeof(sHeader)), __EXIT__);

#if 0
#if 1
	// geometry chunk
	ret = CGeometryChunk::GetInstance().Export(
			&m_Out,
			pImporter);
	CGeometryChunk::GetInstance().Clear();
	VGOTO(ret, __EXIT__);
#endif

#if 1
	// joint chunk
	ret = CJointChunk::Export(
			&m_Out,
			pImporter);
	VGOTO(ret, __EXIT__);
#endif

	// Export terminater.
	{
		izanagi::S_MDL_CHUNK_HEADER sChunkHeader;
		sChunkHeader.magicChunk = izanagi::MDL_CHUNK_MAGIC_TERMINATE;
		IZ_OUTPUT_WRITE_VRETURN(&m_Out, &sChunkHeader, 0, sizeof(sChunkHeader));
	}

	// Export files's header.
	{
		sHeader.sizeFile = m_Out.GetCurPos();

		const izanagi::SVector& vMin = CGeometryChunk::GetInstance().GetMin();
		const izanagi::SVector& vMax = CGeometryChunk::GetInstance().GetMax();

		sHeader.minVtx[0] = vMin.x;
		sHeader.minVtx[1] = vMin.y;
		sHeader.minVtx[2] = vMin.z;

		sHeader.maxVtx[0] = vMax.x;
		sHeader.maxVtx[1] = vMax.y;
		sHeader.maxVtx[2] = vMax.z;

		VRETURN(cSeekHelper.Return());
		IZ_OUTPUT_WRITE_VRETURN(&m_Out, &sHeader, 0, sizeof(sHeader));
	}
#else
	// Mesh chunk.
	{
		izanagi::S_MDL_CHUNK_HEADER sChunkHeader;
		sChunkHeader.magicChunk = izanagi::MDL_CHUNK_MAGIC_MESH;
		ret = IZ_OUTPUT_WRITE(&m_Out, &sChunkHeader, 0, sizeof(sChunkHeader));
		VGOTO(ret, __EXIT__);

		ret = CGeometryChunk::GetInstance().Export(
				maxJointMtxNum,
				&m_Out,
				pImporter);
	}

	// Skeleton chunk.
	{
		izanagi::S_MDL_CHUNK_HEADER sChunkHeader;
		sChunkHeader.magicChunk = izanagi::MDL_CHUNK_MAGIC_SKELETON;
		ret = IZ_OUTPUT_WRITE(&m_Out, &sChunkHeader, 0, sizeof(sChunkHeader));
		VGOTO(ret, __EXIT__);

		ret = CJointChunk::Export(
				&m_Out,
				pImporter);
	}

	// Export terminater.
	{
		izanagi::S_MDL_CHUNK_HEADER sChunkHeader;
		sChunkHeader.magicChunk = izanagi::MDL_CHUNK_MAGIC_TERMINATE;
		ret = IZ_OUTPUT_WRITE(&m_Out, &sChunkHeader, 0, sizeof(sChunkHeader));
		VGOTO(ret, __EXIT__);
	}

	// Export files's header.
	{
		sHeader.sizeFile = m_Out.GetCurPos();

		VGOTO(ret = cSeekHelper.Return(), __EXIT__);
		ret = IZ_OUTPUT_WRITE(&m_Out, &sHeader, 0, sizeof(sHeader));
		VGOTO(ret, __EXIT__);
	}
#endif

__EXIT__:
	m_Out.Finalize();

	return ret;
}
