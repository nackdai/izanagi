#include <string>
#include <vector>
#include <stdint.h>

#include "PCDFormat.h"
#include "LASPointReader.h"

bool convert(
    std::string& inFile,
    std::string& outFile)
{
    auto reader = new Potree::LASPointReader(inFile);

    if (reader->numPoints() == 0) {
        // TODO
        return false;
    }

    auto aabb = reader->getAABB();

    aabb.size = aabb.max - aabb.min;

    aabb.makeCubic();

    uint64_t pointNum = 0;
    uint64_t flushNum = 0;

    FILE* fp = nullptr;
    fopen_s(&fp, outFile.c_str(), "wb");

    fseek(fp, sizeof(PCDHeader), SEEK_SET);

    std::vector<Point> points;
    points.reserve(reader->numPoints());

    static const uint32_t FLUSH_NUM = 10000;

    while (reader->readNextPoint()) {
        auto point = reader->getPoint();

        pointNum++;
        flushNum++;

        Point pt;

        pt.pos[0] = (float)point.position.x;
        pt.pos[1] = (float)point.position.y;
        pt.pos[2] = (float)point.position.z;

        pt.rgba[0] = point.color.x;
        pt.rgba[1] = point.color.y;
        pt.rgba[2] = point.color.z;
        pt.rgba[3] = 0xff;

        points.push_back(pt);

        if (flushNum == FLUSH_NUM) {
            printf("%d\n", pointNum);
            fwrite(&points[0], sizeof(Point), flushNum, fp);
            points.clear();
            flushNum = 0;
        }
    }

    if (flushNum > 0) {
        printf("%d\n", pointNum);
        fwrite(&points[0], sizeof(Point), flushNum, fp);
        points.clear();
        flushNum = 0;
    }

    // Write header.
    {
        PCDHeader header;
        header.fileSize = ftell(fp);
        header.vtxFormat = VtxFormat::Position | VtxFormat::Color;
        header.vtxNum = pointNum;
        
        auto& aabbMin = aabb.min;
        auto& aabbMax = aabb.max;

        header.aabbMin[0] = (float)aabbMin.x;
        header.aabbMin[1] = (float)aabbMin.y;
        header.aabbMin[2] = (float)aabbMin.z;

        header.aabbMax[0] = (float)aabbMax.x;
        header.aabbMax[1] = (float)aabbMax.y;
        header.aabbMax[2] = (float)aabbMax.z;

        fseek(fp, 0, SEEK_SET);
        fwrite(&header, sizeof(header), 1, fp);
    }

    fclose(fp);

	return true;
}

