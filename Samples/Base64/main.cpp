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

void Sample3()
{
    std::vector<IZ_CHAR> dst(256);

    IZ_UINT length = izanagi::CBase64::Encode(
        (IZ_BYTE*)string,
        strlen(string),
        &dst[0]);

    std::vector<IZ_BYTE> dst2(256);

    izanagi::CBase64::Decode(
        &dst[0],
        length,
        &dst2[0]);

    dst.push_back(0);
    IZ_CHAR* tmp = &dst[0];
    IZ_PRINTF("%s\n", tmp);

    if (memcmp(&dst2[0], string, strlen(string)) == 0) {
        IZ_PRINTF("Succeeded Sample3.\n");
    }
    else {
        IZ_PRINTF("Failed Sample3.\n");
    }
}

void Sample4()
{
    std::vector<IZ_CHAR> dst(256);

    IZ_UINT length = izanagi::CBase64::Encode(
        (IZ_BYTE*)numbers,
        sizeof(numbers),
        &dst[0]);

    std::vector<IZ_BYTE> dst2(256);

    izanagi::CBase64::Decode(
        &dst[0],
        length,
        &dst2[0]);

    dst.push_back(0);
    IZ_CHAR* tmp = &dst[0];
    IZ_PRINTF("%s\n", tmp);

    if (memcmp(&dst2[0], numbers, sizeof(numbers)) == 0) {
        IZ_PRINTF("Succeeded Sample4.\n");
    }
    else {
        IZ_PRINTF("Failed Sample4.\n");
    }
}

IzMain(0, 0)
{
    Sample1();
    Sample2();

    Sample3();
    Sample4();

    return 0;
}
