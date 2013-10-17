#if !defined(__MODEL_LIB_JSON_GEOMETRY_H__)
#define __MODEL_LIB_JSON_GEOMETRY_H__

#include <vector>
#include "izToolKit.h"
#include <rapidjson/document.h>

class CJsonGeometry
{
    struct SVector3 {
        union {
            IZ_FLOAT v[3];
            struct {
                IZ_FLOAT x, y, z;
            };
        };
    };

public:
    CJsonGeometry();
    ~CJsonGeometry();

    NO_COPIABLE(CJsonGeometry);

public:
    IZ_BOOL Read(rapidjson::Document& document);

private:
    template <typename _T, typename Func>
    IZ_BOOL ReadArray(
        rapidjson::Value& entry,
        std::vector<_T>& list,
        Func& func);

private:
    std::vector<SVector3> m_Vertices;
    std::vector<SVector3> m_Normals;
};

#endif  // #if !defined(__MODEL_LIB_JSON_GEOMETRY_H__)
