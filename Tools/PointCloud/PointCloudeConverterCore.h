#include <string>
#include "proxy.h"

#include "TMPFormat.h"

struct Point {
    float pos[3];
    uint8_t rgba[4];
};

int main(int argc, char* argv[])
{
    if (argc < 3) {
        return 1;
    }

    std::string inFile(argv[1]);
    std::string outFile(argv[2]);

    auto reader = (Potree::LASPointReader*)Proxy::createPointReader(inFile);

    if (!reader) {
        // TODO
        return 1;
    }

    auto aabb = reader->getAABB();

    aabb.size = aabb.max - aabb.min;

    aabb.makeCubic();

    uint64_t pointNum = 0;
    uint64_t flushNum = 0;

    FILE* fp = nullptr;
    fopen_s(&fp, outFile.c_str(), "wb");

    fseek(fp, sizeof(TMPHeader), SEEK_SET);

    std::vector<Point> points;
    points.reserve(reader->numPoints());

    static const uint32_t FLUSH_NUM = 10000;

    while (reader->readNextPoint()) {
        auto point = reader->getPoint();

        pointNum++;
        flushNum++;

        Point pt;

        pt.pos[0] = point.position.x;
        pt.pos[1] = point.position.y;
        pt.pos[2] = point.position.z;

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
        TMPHeader header;
        header.fileSize = ftell(fp);
        header.vtxFormat = VtxFormat::Position | VtxFormat::Color;
        header.vtxNum = pointNum;
        
        auto& aabbMin = aabb.min;
        auto& aabbMax = aabb.max;

        header.aabbMin[0] = aabbMin.x;
        header.aabbMin[1] = aabbMin.y;
        header.aabbMin[2] = aabbMin.z;

        header.aabbMax[0] = aabbMax.x;
        header.aabbMax[1] = aabbMax.y;
        header.aabbMax[2] = aabbMax.z;

        fseek(fp, 0, SEEK_SET);
        fwrite(&header, sizeof(header), 1, fp);
    }

    fclose(fp);

	return 0;
}

