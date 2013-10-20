#if !defined(__MODEL_LIB_JSON_GEOMETRY_H__)
#define __MODEL_LIB_JSON_GEOMETRY_H__

#include <vector>
#include <set>
#include <map>
#include "izToolKit.h"
#include "GeometryCommon.h"
#include <rapidjson/document.h>

class CJsonGeometry
{
    struct SVector2 {
        IZ_FLOAT u, v;
    };

    struct SVector3 {
        IZ_FLOAT x, y, z;
    };

    struct SFace {
        IZ_UINT id;

        IZ_UINT idx[3];
        IZ_UINT mtrl;
    };

public:
    CJsonGeometry();
    ~CJsonGeometry();

    NO_COPIABLE(CJsonGeometry);

public:
    IZ_BOOL Read(rapidjson::Document& document);

    IZ_UINT GetMeshNum() const;

    void Begin(IZ_UINT meshIdx);
    void End();

    IZ_UINT GetTriangle(std::vector<STri>& tvTriList);

    IZ_UINT GetVtxSize() const;
    IZ_UINT GetVtxFmt() const;

    IZ_BOOL GetVertex(
        IZ_UINT idx,
        izanagi::math::SVector& vec,
        izanagi::E_MSH_VTX_FMT_TYPE type);

private:
    IZ_BOOL ReadVertices(rapidjson::Document& document);

private:
    std::vector<SVector3> m_Vertices;
    std::vector<SVector3> m_Normals;

    std::vector<SVector2> m_FaceUVs;

    // マテリアルをキーとしてfaceをまとめる
    std::map<IZ_UINT, std::vector<SFace> > m_Faces;

    IZ_UINT m_CurMeshIdx;
    IZ_UINT m_VtxFmt;
};

#endif  // #if !defined(__MODEL_LIB_JSON_GEOMETRY_H__)
