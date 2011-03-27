#if !defined(__MODEL_LIB_COLLADA_GEOMETRY_H__)
#define __MODEL_LIB_COLLADA_GEOMETRY_H__

#include <dom/domCOLLADA.h>
#include <dae.h>

#include "GeometryCommon.h"
#include "izSceneGraph.h"

#include <vector>

class CColladaGeometry {
	static CColladaGeometry s_cIntance;

private:
	enum E_PRIM_TYPE {
		E_PRIM_TYPE_TRIANGLES = 0,
		E_PRIM_TYPE_TRISTRIP,
		E_PRIM_TYPE_TRIFAN,
		E_PRIM_TYPE_POLYLIST,
		E_PRIM_TYPE_POLYGONS,

		E_PRIM_TYPE_NUM,
		E_PRIM_TYPE_FORCE_INT32 = 0x7fffffff,
	};

	struct SVtx {
		std::vector<IZ_UINT> input;

		// For std::find
		inline bool operator==(const SVtx& rhs);
	};

	struct SVtxFmt {
		daeString src_id;
		daeString semantic;
		size_t offset;
		izanagi::E_MSH_VTX_FMT_TYPE type;

		inline bool operator==(IZ_PCSTR pszSemantic);
		inline bool operator<(const SVtxFmt& rhs) const;

		inline bool operator==(izanagi::E_MSH_VTX_FMT_TYPE _type);
	};

public:
	static CColladaGeometry& GetInstance() { return s_cIntance; }

private:
	CColladaGeometry() {}
	~CColladaGeometry() {}	

public:
	void Clear();

	void Begin(
		domMesh* pMesh,
		IZ_UINT nMeshIdx);

	void End();

	IZ_UINT GetMeshNum(DAE* pDAE);

	void GetSkinList(
		DAE* pDAE,
		std::vector<SSkin>& tvSkinList);

	IZ_UINT GetTriangles(
		domMesh* pMesh,
		std::vector<STri>& tvTriList);

	IZ_UINT GetSkinIdx(
		domMesh* pMesh,
		IZ_UINT nVtxIdx);

	IZ_UINT GetVtxSize();

	IZ_UINT GetVtxFmt();

	IZ_BOOL GetVertex(
		domMesh* pMesh,
		IZ_UINT nIdx,
		izanagi::SVector& vec,
		izanagi::E_MSH_VTX_FMT_TYPE type);

private:
	template <typename _T>
	void ReadVtx(_T pPrim);

	template <>
	void ReadVtx(domTrianglesRef pTriangles);

	template <typename _T>
	void ReadVtx(
		size_t nInputNum,
		IZ_UINT nTriNum,
		size_t nOffset,
		_T pPrim);

	void AddVtx(
		STri& sTri,
		IZ_UINT nPos,
		SVtx& sVtx);

	template <typename _T>
	void ReadVtxFmt(
		domMesh* pMesh,
		_T pPrim);

	template <typename _T>
	IZ_UINT GetSkinIdx(
		_T pPrim,
		IZ_UINT nVtxIdx);

	E_PRIM_TYPE GetPrimType(
		domMesh* pMesh,
		IZ_UINT nIdx,
		IZ_UINT* pMeshSubsetPos);

private:
	E_PRIM_TYPE m_PrimType;
	IZ_UINT m_nMeshSubsetPos;

	std::vector<STri>* m_TriList;

	std::vector<SVtx> m_VtxList;
	std::set<SVtxFmt> m_VtxFmtList;
};

#endif	// #if !defined(__MODEL_LIB_COLLADA_GEOMETRY_H__)
