#include "gtest/gtest.h"
#include "izDefs.h"
#include "izStd.h"

static const IZ_CHAR* string = "This is izanagi BASE64 sample.";

static const IZ_UINT numbers[] = {
    24546545,
    541097,
    47,
    2454,
    44431845,
};

TEST(Base64, EncDecStringFromSTL)
{
    std::vector<IZ_BYTE> src(strlen(string));
    memcpy(&src[0], string, src.size());

    std::vector<IZ_CHAR> dst;

    ASSERT_TRUE(izanagi::CBase64::Encode(src, dst));

    src.clear();

    ASSERT_TRUE(izanagi::CBase64::Decode(dst, src));

    ASSERT_TRUE(memcmp(&src[0], string, strlen(string)) == 0);
}

TEST(Base64, EncDecNumberFromSTL)
{
    std::vector<IZ_BYTE> src(sizeof(numbers));
    memcpy(&src[0], numbers, src.size());

    std::vector<IZ_CHAR> dst;

    ASSERT_TRUE(izanagi::CBase64::Encode(src, dst));

    src.clear();

    ASSERT_TRUE(izanagi::CBase64::Decode(dst, src));

    ASSERT_TRUE(memcmp(&src[0], numbers, src.size()) == 0);
}

TEST(Base64, EncDecStringFromMemory)
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

    ASSERT_TRUE(memcmp(&dst2[0], string, strlen(string)) == 0);
}

TEST(Base64, EncDecNumberFromMemory)
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

    ASSERT_TRUE(memcmp(&dst2[0], numbers, sizeof(numbers)) == 0);
}

TEST(Base64, FailDecode)
{
    std::vector<IZ_CHAR> src(1);
    std::vector<IZ_BYTE> dst;

    ASSERT_FALSE(izanagi::CBase64::Decode(src, dst));
}
