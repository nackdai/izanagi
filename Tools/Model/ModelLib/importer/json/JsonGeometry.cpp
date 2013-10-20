#include "JsonGeometry.h"

CJsonGeometry::CJsonGeometry()
{
    m_CurMeshIdx = 0;
    m_VtxFmt = 0;
}

CJsonGeometry::~CJsonGeometry()
{
}

struct FuncGetFloat {
    IZ_FLOAT operator()(rapidjson::Value& entry)
    {
        IZ_FLOAT ret = (IZ_FLOAT)entry.GetDouble();
        return ret;
    }
};

inline IZ_BOOL _IsBitSet(IZ_INT value, IZ_UINT position)
{
	return ((value & (1 << position)) > 0);
}

IZ_BOOL CJsonGeometry::Read(rapidjson::Document& document)
{
    // Vertex
    VRETURN(ReadVertices(document));
    m_VtxFmt |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_POS;

    // NOTE
    // UV階層は許さない
    // つまり、１階層しか扱わない

    // UV
    int uvLayerNum = document["uvs"].Size();

    // Face
    rapidjson::Value& faces = document["faces"];
    rapidjson::Value& normals = document["normals"];
    rapidjson::Value& uvs = document["uvs"][rapidjson::SizeType(0)];

    IZ_UINT offset = 0;
    IZ_UINT length = faces.Size();

    // １頂点に複数法線の可能性があるので
    std::vector< std::vector<SVector3> > tmpNormals;

    while (offset < length) {
        IZ_INT type = faces[rapidjson::SizeType(offset++)].GetInt();

        IZ_BOOL isQuad              = _IsBitSet(type, 0);
	    IZ_BOOL hasMaterial         = _IsBitSet(type, 1);
	    IZ_BOOL hasFaceVertexUv     = _IsBitSet(type, 3);
	    IZ_BOOL hasFaceNormal       = _IsBitSet(type, 4);
	    IZ_BOOL hasFaceVertexNormal = _IsBitSet(type, 5);
	    IZ_BOOL hasFaceColor	    = _IsBitSet(type, 6);
	    IZ_BOOL hasFaceVertexColor  = _IsBitSet(type, 7);

        if (isQuad) {
            // NOTE
            // izanagiは左手系なので逆回り

            SFace faceA;
            {
                faceA.idx[0] = faces[rapidjson::SizeType(offset)].GetInt();
                faceA.idx[1] = faces[rapidjson::SizeType(offset + 3)].GetInt();
                faceA.idx[2] = faces[rapidjson::SizeType(offset + 1)].GetInt();
            }

            SFace faceB;
            {
                faceB.idx[0] = faces[rapidjson::SizeType(offset + 1)].GetInt();
                faceB.idx[1] = faces[rapidjson::SizeType(offset + 3)].GetInt();
                faceB.idx[2] = faces[rapidjson::SizeType(offset + 2)].GetInt();
            }

            offset += 4;

            if (hasMaterial) {
                int mtrlIdx = faces[rapidjson::SizeType(offset++)].GetInt();
                faceA.mtrl = mtrlIdx;
                faceB.mtrl = mtrlIdx;
            }

            if (hasFaceVertexUv) {
                if (m_FaceUVs.size() == 0) {
                    m_FaceUVs.resize(m_Vertices.size());
                }

                SVector2 uv[4];

                for (IZ_UINT n = 0; n < COUNTOF(uv); n++ ) {
                    int uvIndex = faces[rapidjson::SizeType(offset++)].GetInt();

                    uv[n].u = (IZ_FLOAT)uvs[uvIndex * 2].GetDouble();
                    uv[n].v = (IZ_FLOAT)uvs[uvIndex * 2 + 1].GetDouble();
                }
                    
                size_t faceNum = m_Faces.size();

                // NOTE
                // Faceのインデックスの参照順と同じ

                m_FaceUVs[faceA.idx[0]] = uv[0];
                m_FaceUVs[faceA.idx[1]] = uv[3];
                m_FaceUVs[faceA.idx[2]] = uv[1];
                
                m_FaceUVs[faceB.idx[0]] = uv[1];
                m_FaceUVs[faceB.idx[1]] = uv[3];
                m_FaceUVs[faceB.idx[2]] = uv[2];

                m_VtxFmt |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_UV;
            }

            if (hasFaceNormal) {
                if (tmpNormals.size() == 0) {
                    tmpNormals.resize(m_Vertices.size());
                }

                int normalIndex = faces[rapidjson::SizeType(offset++)].GetInt() * 3;

                SVector3 normal;
                {
                    normal.x = (IZ_FLOAT)normals[normalIndex++].GetDouble();
                    normal.y = (IZ_FLOAT)normals[normalIndex++].GetDouble();
                    normal.z = (IZ_FLOAT)normals[normalIndex++].GetDouble();
                }

                tmpNormals[faceA.idx[0]].push_back(normal);
                tmpNormals[faceA.idx[1]].push_back(normal);
                tmpNormals[faceA.idx[2]].push_back(normal);

                tmpNormals[faceB.idx[0]].push_back(normal);
                tmpNormals[faceB.idx[1]].push_back(normal);
                tmpNormals[faceB.idx[2]].push_back(normal);

                m_VtxFmt |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_NORMAL;
            }

            if (hasFaceVertexNormal) {
                if (tmpNormals.size() == 0) {
                    tmpNormals.resize(m_Vertices.size());
                }

                SVector3 normal[4];

                for (int i = 0; i < COUNTOF(normal); i++) {
                    int normalIndex = faces[rapidjson::SizeType(offset++)].GetInt() * 3;

                    normal[i].x = (IZ_FLOAT)normals[normalIndex++].GetDouble();
                    normal[i].y = (IZ_FLOAT)normals[normalIndex++].GetDouble();
                    normal[i].z = (IZ_FLOAT)normals[normalIndex++].GetDouble();
                }

                // NOTE
                // Faceのインデックスの参照順と同じ

                tmpNormals[faceA.idx[0]].push_back(normal[0]);
                tmpNormals[faceA.idx[1]].push_back(normal[3]);
                tmpNormals[faceA.idx[2]].push_back(normal[1]);

                tmpNormals[faceB.idx[0]].push_back(normal[1]);
                tmpNormals[faceB.idx[1]].push_back(normal[3]);
                tmpNormals[faceB.idx[2]].push_back(normal[2]);

                m_VtxFmt |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_NORMAL;
            }

            if (hasFaceColor) {
                // TODO

                m_VtxFmt |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_COLOR;
            }

            if (hasFaceVertexColor) {
                // TODO

                m_VtxFmt |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_COLOR;
            }

            faceA.id = m_Faces.size();
            faceB.id = m_Faces.size() + 1;

            m_Faces[faceA.mtrl].push_back(faceA);
            m_Faces[faceB.mtrl].push_back(faceB);
        }
        else {
            SFace face;
            {
                face.idx[0] = faces[rapidjson::SizeType(offset)].GetInt();
                face.idx[1] = faces[rapidjson::SizeType(offset + 2)].GetInt();
                face.idx[2] = faces[rapidjson::SizeType(offset + 1)].GetInt();
            }

            offset += 3;

            if (hasMaterial) {
                int mtrlIdx = faces[rapidjson::SizeType(offset++)].GetInt();
                face.mtrl = mtrlIdx;
            }

            if (hasFaceVertexUv) {
                if (m_FaceUVs.size() == 0) {
                    m_FaceUVs.resize(m_Vertices.size());
                }

                SVector2 uv[3];

                for (IZ_UINT n = 0; n < COUNTOF(uv); n++ ) {
                    int uvIndex = faces[rapidjson::SizeType(offset++)].GetInt();

                    uv[n].u = (IZ_FLOAT)uvs[uvIndex * 2].GetDouble();
                    uv[n].v = (IZ_FLOAT)uvs[uvIndex * 2 + 1].GetDouble();
                }

                size_t faceNum = m_Faces.size();

                // NOTE
                // Faceのインデックスの参照順と同じ

                m_FaceUVs[face.idx[0]] = uv[0];
                m_FaceUVs[face.idx[1]] = uv[2];
                m_FaceUVs[face.idx[2]] = uv[1];

                m_VtxFmt |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_UV;
            }

            if (hasFaceNormal) {
                if (tmpNormals.size() == 0) {
                    tmpNormals.resize(m_Vertices.size());
                }

                int normalIndex = faces[rapidjson::SizeType(offset++)].GetInt() * 3;

                SVector3 normal;
                {
                    normal.x = (IZ_FLOAT)normals[normalIndex++].GetDouble();
                    normal.y = (IZ_FLOAT)normals[normalIndex++].GetDouble();
                    normal.z = (IZ_FLOAT)normals[normalIndex++].GetDouble();
                }

                tmpNormals[face.idx[0]].push_back(normal);
                tmpNormals[face.idx[1]].push_back(normal);
                tmpNormals[face.idx[2]].push_back(normal);

                m_VtxFmt |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_NORMAL;
            }

            if (hasFaceVertexNormal) {
                if (tmpNormals.size() == 0) {
                    tmpNormals.resize(m_Vertices.size());
                }

                SVector3 normal[3];

                for (int i = 0; i < COUNTOF(normal); i++) {
                    int normalIndex = faces[rapidjson::SizeType(offset++)].GetInt() * 3;

                    normal[i].x = (IZ_FLOAT)normals[normalIndex++].GetDouble();
                    normal[i].y = (IZ_FLOAT)normals[normalIndex++].GetDouble();
                    normal[i].z = (IZ_FLOAT)normals[normalIndex++].GetDouble();
                }

                // NOTE
                // Faceのインデックスの参照順と同じ

                tmpNormals[face.idx[0]].push_back(normal[0]);
                tmpNormals[face.idx[1]].push_back(normal[2]);
                tmpNormals[face.idx[2]].push_back(normal[1]);

                m_VtxFmt |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_NORMAL;
            }

            if (hasFaceColor) {
                // TODO

                m_VtxFmt |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_COLOR;
            }

            if (hasFaceVertexColor) {
                // TODO

                m_VtxFmt |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_COLOR;
            }

            m_Faces[face.mtrl].push_back(face);
        }
    }

    // 法線を平均化して、正規化する
    for (IZ_UINT i = 0; i < tmpNormals.size(); i++) {
        std::vector<SVector3>& normals = tmpNormals[i];

        SVector3 normal;
        {
            normal.x = 0.0f;
            normal.y = 0.0f;
            normal.z = 0.0f;
        }

        IZ_UINT count = normals.size();

        for (IZ_UINT n = 0; n < count; n++) {
            normal.x += normals[n].x;
            normal.y += normals[n].y;
            normal.z += normals[n].z;
        }

        normal.x /= count;
        normal.y /= count;
        normal.z /= count;

        izanagi::math::CVector result(normal.x, normal.y, normal.z, 0.0f);
        result.Normalize();

        normal.x = result.x;
        normal.y = result.y;
        normal.z = result.z;

        m_Normals.push_back(normal);
    }

    return IZ_TRUE;
}

IZ_BOOL CJsonGeometry::ReadVertices(rapidjson::Document& document)
{
    rapidjson::Value& vertices = document["vertices"];

    if (vertices.IsNull()) {
        return IZ_TRUE;
    }

    int size = vertices.Size();
    VRETURN(size % 3 == 0);

    int num = size / 3;
    m_Vertices.resize(num);

    for (int i = 0; i < num; i++) {
        SVector3& vtx = m_Vertices[i];

        vtx.x = (IZ_FLOAT)vertices[rapidjson::SizeType(i * 3 + 0)].GetDouble();
        vtx.y = (IZ_FLOAT)vertices[rapidjson::SizeType(i * 3 + 1)].GetDouble();
        vtx.z = (IZ_FLOAT)vertices[rapidjson::SizeType(i * 3 + 2)].GetDouble();
    }

    return IZ_TRUE;
}

IZ_UINT CJsonGeometry::GetMeshNum() const
{
    return m_Faces.size();
}

void CJsonGeometry::Begin(IZ_UINT meshIdx)
{
    m_CurMeshIdx = meshIdx;
}

void CJsonGeometry::End()
{
}

IZ_UINT CJsonGeometry::GetTriangle(std::vector<STri>& tvTriList)
{
    IZ_UINT vtxNum = 0;

    std::vector<SFace> faces = m_Faces[m_CurMeshIdx];

    tvTriList.reserve(faces.size());

    for (IZ_UINT i = 0; i < faces.size(); i++) {
        STri tri;
        {
            tri.vtx[0] = faces[i].idx[0];
            tri.vtx[1] = faces[i].idx[1];
            tri.vtx[2] = faces[i].idx[2];
        }

        tvTriList.push_back(tri);

        vtxNum += 3;
    }

    return vtxNum;
}

IZ_UINT CJsonGeometry::GetVtxSize() const
{
    static IZ_UINT tblVtxSize[] = {
        izanagi::E_MSH_VTX_SIZE_POS,
        izanagi::E_MSH_VTX_SIZE_NORMAL,
        izanagi::E_MSH_VTX_SIZE_COLOR,
        izanagi::E_MSH_VTX_SIZE_UV,
        izanagi::E_MSH_VTX_SIZE_TANGENT,
        izanagi::E_MSH_VTX_SIZE_BLENDINDICES,
        izanagi::E_MSH_VTX_SIZE_BLENDWEIGHT,
    };
    IZ_C_ASSERT(COUNTOF(tblVtxSize) == izanagi::E_MSH_VTX_FMT_TYPE_NUM);

    IZ_UINT ret = 0;

    for (IZ_UINT i = 0; i < COUNTOF(tblVtxSize); i++) {
        if ((m_VtxFmt & (1 << i)) > 0) {
            ret += tblVtxSize[i];
        }
    }

    return ret;
}

IZ_UINT CJsonGeometry::GetVtxFmt() const
{
    return m_VtxFmt;
}

IZ_BOOL CJsonGeometry::GetVertex(
    IZ_UINT idx,
    izanagi::math::SVector& vec,
    izanagi::E_MSH_VTX_FMT_TYPE type)
{
    switch (type) {
    case izanagi::E_MSH_VTX_FMT_TYPE_POS:
        {
            const SVector3& vtx = m_Vertices[idx];
            vec.Set(vtx.x, vtx.y, vtx.z);
            return IZ_TRUE;
        }
    case izanagi::E_MSH_VTX_FMT_TYPE_NORMAL:
        {
            if (m_Normals.size() == 0) {
                return IZ_FALSE;
            }

            const SVector3& nml = m_Normals[idx];
            vec.Set(nml.x, nml.y, nml.z, 0.0f);
            return IZ_TRUE;
        }
    case izanagi::E_MSH_VTX_FMT_TYPE_UV:
        {
            if (m_FaceUVs.size() == 0) {
                return IZ_FALSE;
            }

            const SVector2& uv = m_FaceUVs[idx];
            vec.Set(uv.u, uv.v, 0.0f, 0.0f);
            return IZ_TRUE;
        }
    case izanagi::E_MSH_VTX_FMT_TYPE_COLOR:
    case izanagi::E_MSH_VTX_FMT_TYPE_TANGENT:
    case izanagi::E_MSH_VTX_FMT_TYPE_BLENDINDICES:
    case izanagi::E_MSH_VTX_FMT_TYPE_BLENDWEIGHT:
        // TODO
        return IZ_FALSE;
    default:
        IZ_ASSERT(IZ_FALSE);
    }

    return IZ_FALSE;
}
