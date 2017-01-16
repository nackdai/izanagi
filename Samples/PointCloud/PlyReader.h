#if !defined(__PLY_READER_H__)
#define __PLY_READER_H__

#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class PlyReader {
public:
    PlyReader() {}
    ~PlyReader()
    {
        close();
    }

public:
    bool open(const std::string& path);
    void close();

    struct Vertex {
        float pos[4];
        uint8_t color[4];
    };

    bool readVtx(PlyReader::Vertex& vtx);

    uint32_t getVtxNum() const
    {
        return m_vtxNum;
    }

    uint32_t getVtxCnt() const
    {
        return m_vtxCnt;
    }

private:
    void readHeader();

private:
    uint32_t m_vtxNum{ 0 };

    std::ifstream m_in;

    uint32_t m_vtxCnt{ 0 };

    enum Type {
        TypeInvalid,
        Int32,
        Float,
    };
    enum ValueType {
        ValueInvalid,
        x,
        y,
        z,
        confidence,
        intensity,
    };
    struct Property {
        Type type{ Type::TypeInvalid };
        ValueType value{ ValueType::ValueInvalid };
        bool isValid()
        {
            return (type != Type::TypeInvalid && value != ValueType::ValueInvalid);
        }
    };
    std::vector<Property> m_props;
};

#endif    // #if !defined(__PLY_READER_H__)