#if !defined(__MODEL_LIB_COLLADA_GEOMETRY_H__)
#define __MODEL_LIB_COLLADA_GEOMETRY_H__

#include <dom/domCOLLADA.h>
#include <dae.h>

#include "GeometryCommon.h"
#include "izSceneGraph.h"

#include <vector>

class CColladaGeometry {
    static CColladaGeometry* s_pInstance;

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

#if 0
        inline bool operator==(izanagi::E_MSH_VTX_FMT_TYPE _type);
#endif
    };

public:
    static CColladaGeometry& GetInstance();
    static void DeleteInstance();

private:
    CColladaGeometry() {}
    ~CColladaGeometry() {}  

public:
    void Clear();

    void Begin(
        DAE* pDAE,
        domMesh* pMesh,
        IZ_UINT nMeshIdx);

    void End();

    // Return mesh count.
    IZ_UINT GetMeshNum(DAE* pDAE);

    // Get informaitons of skin.
    void GetSkinList(
        DAE* pDAE,
        std::vector<SSkin>& tvSkinList);

    // Returns count of triangles in specified mesh.
    IZ_UINT GetTriangles(
        domMesh* pMesh,
        std::vector<STri>& tvTriList);

    // Returns index of skin that influeces specified vertex.
    IZ_UINT GetSkinIdx(
        domMesh* pMesh,
        IZ_UINT nVtxIdx);

    // Returns size per vertex.
    IZ_UINT GetVtxSize();

    // Returns vertex format.
    IZ_UINT GetVtxFmt();

    // Get vertex paremter by specifed type.
    IZ_BOOL GetVertex(
        domMesh* pMesh,
        IZ_UINT nIdx,
        izanagi::math::SVector& vec,
        izanagi::E_MSH_VTX_FMT_TYPE type);

    // メッシュにバインドされるマテリアル情報を取得
    IZ_BOOL GetMaterialForMesh(
        IZ_UINT nIdx,
        izanagi::S_MSH_MTRL& sMtrl);

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

    // Return index of skin that influeces specified vertex in specified primitive.
    template <typename _T>
    IZ_UINT GetSkinIdx(
        _T pPrim,
        IZ_UINT nVtxIdx);

    // メッシュのプリミティブタイプを取得
    E_PRIM_TYPE GetPrimType(
        domMesh* pMesh,
        IZ_UINT nIdx,
        IZ_UINT* pMeshSubsetPos);

    // メッシュにバインドされるマテリアルを読み込み
    template <typename _T>
    void ReadMeshMtrl(
        DAE* pDAE,
        IZ_UINT nIdx,
        _T pPrim);

private:
    E_PRIM_TYPE m_PrimType;
    IZ_UINT m_nMeshSubsetPos;

    std::vector<STri>* m_TriList;

    std::vector<SVtx> m_VtxList;
    std::set<SVtxFmt> m_VtxFmtList;

    std::map<IZ_UINT, std::string> m_Mtrl;
};

#endif  // #if !defined(__MODEL_LIB_COLLADA_GEOMETRY_H__)
