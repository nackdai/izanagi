#include "PlyReader.h"
#include "izDefs.h"

bool PlyReader::open(const std::string& path)
{
    close();
    m_in.open(path.c_str(), std::ios::in);

    bool result = m_in.good();

    if (result) {
        readHeader();
    }

    return result;
}

void PlyReader::close()
{
    m_in.close();

    m_vtxNum = 0;
    m_vtxCnt = 0;
}

namespace {
    inline bool findFromFirst(std::string& lhs, const char* rhs)
    {
        return (lhs.find(rhs, 0) != std::string::npos);
    }

    inline std::string subStr(std::string& lhs, const char* rhs)
    {
        auto length = lhs.length();
        auto offset = std::string(rhs).length();
        auto substr = lhs.substr(offset, length - offset);
        return std::move(substr);
    }
}

bool PlyReader::readVtx(PlyReader::Vertex& vtx)
{
    if (m_vtxCnt >= m_vtxNum) {
        return false;
    }

    if (m_in.eof()) {
        return false;
    }

    union Value {
        float f;
        int32_t i;
    };

    std::string line;
    getline(m_in, line);

    std::vector<std::string> values;

    std::string tmp(line);
    size_t pos = 0;

    for (int i = 0; i < m_props.size(); i++) {
        auto length = tmp.find(" ", 0);
        auto value = tmp.substr(0, length);

        pos += length + 1;

        values.push_back(value);

        tmp = line.substr(pos, line.length() - pos);
    }

    IZ_ASSERT(values.size() <= m_props.size());

    for (int i = 0; i < m_props.size(); i++) {
        const auto& prop = m_props[i];

        auto& strValue = values[i];

        Value value;

        switch (prop.type) {
        case Type::Float:
            value.f = atof(strValue.c_str());
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }

        switch (prop.value) {
        case ValueType::x:
            vtx.pos[0] = value.f;
            break;
        case ValueType::y:
            vtx.pos[1] = value.f;
            break;
        case ValueType::z:
            vtx.pos[2] = value.f;
            break;
        case ValueType::intensity:
        {
            uint8_t c = (uint8_t)(255.0f * value.f);
            vtx.color[0] = c;
            vtx.color[1] = c;
            vtx.color[2] = c;
            vtx.color[3] = 255;
        }
            break;
        default:
            break;
        }
    }

    vtx.pos[3] = 1.0f;

    m_vtxCnt++;

    return true;
}

void PlyReader::readHeader()
{
    std::string line;
    getline(m_in, line);

    if (line.find("ply", 0) == std::string::npos) {
        return;
    }

    for (;;) {
        getline(m_in, line);

        if (m_in.eof()) {
            break;
        }

        if (findFromFirst(line, "end_header")) {
            break;
        }

        if (findFromFirst(line, "element vertex")) {
            auto substr = subStr(line, "element vertex ");
            m_vtxNum = atoi(substr.c_str());
        }
        else if (findFromFirst(line, "property")) {
            auto prop = subStr(line, "property ");

            Property property;
            std::string value;

            if (findFromFirst(prop, "float32")) {
                property.type = Type::Float;
                value = subStr(prop, "float32 ");
            }
            else if (findFromFirst(prop, "uint8")) {
            }

            if (value == "x") {
                property.value = ValueType::x;
            }
            else if (value == "y") {
                property.value = ValueType::y;
            }
            else if (value == "z") {
                property.value = ValueType::z;
            }
            else if (value == "confidence") {
                property.value = ValueType::confidence;
            }
            else if (value == "intensity") {
                property.value = ValueType::intensity;
            }

            if (property.isValid()) {
                m_props.push_back(property);
            }
        }
    }
}
