#if !defined(__MODEL_LIB_GEOMETRY_CHUNK_H__)
#define __MODEL_LIB_GEOMETRY_CHUNK_H__

#include "izIo.h"
#include "izSceneGraph.h"
#include "Importer.h"
#include "GeometryCommon.h"

#include <vector>

class CGeometryChunk {
	static CGeometryChunk s_cInstance;

public:
	static CGeometryChunk& GetInstance() { return s_cInstance; }

protected:
	CGeometryChunk() {}
	~CGeometryChunk() {}

public:
	IZ_BOOL Export(
		izanagi::IOutputStream* pOut,
		IImporter* pImporter);

	void Clear();

	const izanagi::SVector& GetMin() { return m_vMin; }
	const izanagi::SVector& GetMax() { return m_vMax; }

protected:
	IZ_BOOL ExportGroup(
		izanagi::IOutputStream* pOut,
		IImporter* pImporter);

	void BindJointToTri(
		IImporter* pImporter,
		SMesh& sMesh);

	void ClassifyTriByJoint(SMesh& sMesh);

	void GetMeshInfo(
		IImporter* pImporter,
		SMesh& sMesh);

	IZ_BOOL ComputeVtxNormal(
		IImporter* pImporter,
		const STri& sTri);

	IZ_BOOL ComputeVtxTangent(
		IImporter* pImporter,
		const STri& sTri);

	void ComputeVtxParemters(IImporter* pImporter);

	IZ_UINT ExportVertices(
		izanagi::IOutputStream* pOut,
		IImporter* pImporter);

	IZ_BOOL ExportVertices(
		izanagi::IOutputStream* pOut,
		IImporter* pImporter,
		const SMesh& sMesh,
		SPrimSet& sPrimSet);

	IZ_BOOL ExportMesh(
		izanagi::IOutputStream* pOut,
		IImporter* pImporter);

	void GetMinMaxPos(
		IImporter* pImporter,
		izanagi::SVector& vMin,
		izanagi::SVector& vMax,
		const SPrimSet& sPrimSet);

	IZ_BOOL ExportPrimSet(
		izanagi::IOutputStream* pOut,
		IImporter* pImporter,
		const SPrimSet& sPrimSet);

	IZ_UINT ExportIndices(
		izanagi::IOutputStream* pOut,
		IImporter* pImporter,
		const SPrimSet& sPrimSet);

protected:
	std::vector<SMesh> m_MeshList;
	std::vector<STri> m_TriList;
	std::vector<SSkin> m_SkinList;
	std::vector<SVtxAdditional> m_VtxList;

	std::vector<IZ_UINT> m_ExportedVtx;

	izanagi::SVector m_vMin;
	izanagi::SVector m_vMax;

	IZ_UINT m_nExportedVBNum;
	IZ_UINT m_nExportedSetNum;
	IZ_UINT m_nExportedSubsetNum;
};

#endif	// #if !defined(__MODEL_LIB_GEOMETRY_CHUNK_H__)
