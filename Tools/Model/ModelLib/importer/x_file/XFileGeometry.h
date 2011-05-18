#if !defined(__MODEL_LIB_XFILE_GEOMETRY_H__)
#define __MODEL_LIB_XFILE_GEOMETRY_H__

#include <vector>
#include "XFileHelper.h"
#include "GeometryCommon.h"

class CXFileParser;

class CXFileGeometry {
	static CXFileGeometry* s_pInstance;

private:
	struct SFace {
		IZ_UINT idxMesh;
		IZ_UINT idxMtrl;
		IZ_UINT idxFace;
	};

	struct SVtx {
		std::vector<IZ_UINT> input;

		// For std::find
		inline bool operator==(const SVtx& rhs);
	};

	struct SVtxFmt {
		std::vector<izanagi::E_MSH_VTX_FMT_TYPE> type;
	};

public:
	static CXFileGeometry& GetInstance();
	static void DeleteInstance();

private:
	CXFileGeometry() {}
	~CXFileGeometry() {}

public:
	void Clear();

	void Init(CXFileParser* pParser);

	IZ_UINT GetMeshNum() { return static_cast<IZ_UINT>(m_FaceList.size()); }

	IZ_UINT GetTriangles(
		CXFileParser* pParser,
		IZ_UINT nFaceIdx,
		std::vector<STri>& tvTriList);

	IZ_UINT GetVtxSize(IZ_UINT nMeshIdx);
	IZ_UINT GetVtxFmt(IZ_UINT nMeshIdx);

	IZ_BOOL GetVertex(
		IZ_UINT nFaceIdx,
		IZ_UINT nVtxIdx,
		izanagi::SVector& vec,
		izanagi::E_MSH_VTX_FMT_TYPE type);

	void GetSkinList(
		CXFileParser* pParser,
		IZ_UINT nFaceIdx,
		std::vector<SSkin>& tvSkinList);

	IZ_UINT GetSkinIdx(
		CXFileParser* pParser,
		IZ_UINT nFaceIdx,
		IZ_UINT nVtxIdx);

	void GetMaterial(
		IZ_UINT nIdx,
		izanagi::S_MSH_MTRL& sMtrl);

private:
	const SXFileMesh* GetMesh(IZ_UINT nFaceIdx);

private:
	std::vector<SXFileMesh*> m_MeshList;
	std::vector<SXFileMaterial*> m_MtrlList;
	std::vector< std::vector<SFace> > m_FaceList;

	std::vector<SVtx> m_VtxList;
	std::vector<SVtxFmt> m_VtxFmtList;
};

#endif	// #if !defined(__MODEL_LIB_XFILE_GEOMETRY_H__)
