#include "gtest/gtest.h"
#include "izDefs.h"
#include "izStd.h"

static IZ_UINT value_1 = 0xfe136ad5;
static IZ_UINT value_2 = 0xabcdef12;

TEST(BitStreamInput, Read)
{
    izanagi::CStdBitInput bitIn(&value_1, sizeof(value_1));

    ASSERT_TRUE(bitIn.IsValid());

    ASSERT_EQ(bitIn.Read(4), 0x05);
    ASSERT_EQ(bitIn.Read(8), 0xad);
}

TEST(BitStreamInput, InvalidArgument_Over32)
{
    izanagi::CStdBitInput bitIn(&value_1, sizeof(value_1));

    ASSERT_TRUE(bitIn.IsValid());

    ASSERT_EQ(bitIn.Read(128), value_1);
}

TEST(BitStreamInput, InvalidArgument_Minus)
{
    izanagi::CStdBitInput bitIn(&value_1, sizeof(value_1));

    ASSERT_TRUE(bitIn.IsValid());

    ASSERT_EQ(bitIn.Read(-1), value_1);
}

TEST(BitStreamInput, Reset_1)
{
    izanagi::CStdBitInput bitIn(&value_1, sizeof(value_1));

    ASSERT_TRUE(bitIn.IsValid());

    ASSERT_EQ(bitIn.Read(4), 0x05);

    bitIn.Reset(&value_2, sizeof(value_2));
    ASSERT_EQ(bitIn.Read(8), 0x12);
}

TEST(BitStreamInput, Reset_2)
{
    izanagi::CStdBitInput bitIn(&value_1, sizeof(value_1));

    ASSERT_TRUE(bitIn.IsValid());

    ASSERT_EQ(bitIn.Read(4), 0x05);

    bitIn.Reset();
    ASSERT_EQ(bitIn.Read(8), 0xd5);
}

TEST(BitStreamInput, GetReadBytes)
{
    izanagi::CStdBitInput bitIn(&value_1, sizeof(value_1));

    ASSERT_TRUE(bitIn.IsValid());

    ASSERT_EQ(bitIn.Read(16), 0x6ad5);

    ASSERT_EQ(bitIn.GetReadBytes(), 2);
}

TEST(BitStreamInput, NoArgumentConstructor)
{
    izanagi::CStdBitInput bitIn;

    ASSERT_FALSE(bitIn.IsValid());
    ASSERT_EQ(bitIn.Read(10), 0);
}

TEST(BitStreamInput, AllRead)
{
    izanagi::CStdBitInput bitIn(&value_1, sizeof(value_1));

    ASSERT_TRUE(bitIn.IsValid());

    ASSERT_EQ(bitIn.Read(32), value_1);
    ASSERT_EQ(bitIn.Read(4), 0);
}