#include "izSampleKit.h"

static const IZ_CHAR* string = "This is izanagi BASE64 sample.";

static const IZ_UINT numbers[] = {
    24546545,
    541097,
    47,
    2454,
    44431845,
};

void Sample1()
{
    std::vector<IZ_BYTE> src(strlen(string));
    memcpy(&src[0], string, src.size());

    std::vector<IZ_CHAR> dst;

    izanagi::CBase64::Encode(src, dst);

    src.clear();

    izanagi::CBase64::Decode(dst, src);

    dst.push_back(0);
    IZ_CHAR* tmp = &dst[0];
    IZ_PRINTF("%s\n", tmp);

    if (memcmp(&src[0], string, strlen(string)) == 0) {
        IZ_PRINTF("Succeeded Sample1.\n");
    }
    else {
        IZ_PRINTF("Failed Sample1.\n");
    }
}

void Sample2()
{
    std::vector<IZ_BYTE> src(sizeof(numbers));
    memcpy(&src[0], numbers, src.size());

    std::vector<IZ_CHAR> dst;

    izanagi::CBase64::Encode(src, dst);

    src.clear();

    izanagi::CBase64::Decode(dst, src);

    dst.push_back(0);
    IZ_CHAR* tmp = &dst[0];
    IZ_PRINTF("%s\n", tmp);

    if (memcmp(&src[0], numbers, src.size()) == 0) {
        IZ_PRINTF("Succeeded Sample2.\n");
    }
    else {
        IZ_PRINTF("Failed Sample2.\n");
    }
}


IzMain(0, 0)
{
    Sample1();
    Sample2();

    return 0;
}
