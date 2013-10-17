#include "JsonGeometry.h"

CJsonGeometry::CJsonGeometry()
{
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
    VRETURN(
        ReadArray(
            document["vertices"],
            m_Vertices,
            FuncGetFloat()));

    // Normal
    VRETURN(
        ReadArray(
            document["normals"],
            m_Vertices,
            FuncGetFloat()));

    // Face
    rapidjson::Value& faces = document["faces"];

    IZ_UINT offset = 0;

    IZ_INT type = faces[rapidjson::SizeType(offset++)].GetInt();

    IZ_BOOL isQuad              = _IsBitSet(type, 0);
	IZ_BOOL hasMaterial         = _IsBitSet(type, 1);
	IZ_BOOL hasFaceVertexUv     = _IsBitSet(type, 3);
	IZ_BOOL hasFaceNormal       = _IsBitSet(type, 4);
	IZ_BOOL hasFaceVertexNormal = _IsBitSet(type, 5);
	IZ_BOOL hasFaceColor	    = _IsBitSet(type, 6);
	IZ_BOOL hasFaceVertexColor  = _IsBitSet(type, 7);

    if (isQuad) {
    }
    else {
    }

    return IZ_TRUE;
}

template <typename _T, typename Func>
IZ_BOOL CJsonGeometry::ReadArray(
    rapidjson::Value& entry,
    std::vector<_T>& list,
    Func& func)
{
    if (entry.IsNull()) {
        return IZ_TRUE;
    }

    IZ_INT count = MEMBER_COUNTOF(_T, v);

    int size = entry.Size();
    VRETURN(size % count == 0);

    int num = size / count;
    m_Vertices.resize(num);

    for (int i = 0; i < num; i++) {
        _T& value = list[i];

        for (int n = 0; n < count; n++) {
            value.v[n] = func(entry[rapidjson::SizeType(i * count + n)]);
        }
    }

    return IZ_TRUE;
}
