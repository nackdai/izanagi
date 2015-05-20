#include "gtest/gtest.h"
#include "izDefs.h"
#include "izStd.h"

static IZ_UINT value_1 = 0xfe136ad5;
static IZ_UINT value_2 = 0xabcdef12;

TEST(BitStreamOutput, Write)
{
    IZ_UINT out = 0;
    izanagi::CStdBitOutput bitOut(&out, sizeof(out));

    ASSERT_TRUE(bitOut.IsValid());

    ASSERT_TRUE(bitOut.Write(value_1, 4));
    ASSERT_TRUE(bitOut.Flush());

    const void* dst = bitOut.GetDst();
    ASSERT_NE(dst, (const void*)IZ_NULL);

    IZ_UINT tmp = *(const IZ_UINT*)dst;
    ASSERT_EQ(tmp, 0x05);
}

TEST(BitStreamOutput, WriteToAllocatedMemory)
{
    izanagi::CStdBitOutput bitOut(4);

    ASSERT_TRUE(bitOut.IsValid());

    ASSERT_TRUE(bitOut.Write(value_1, 4));
    ASSERT_TRUE(bitOut.Flush());

    const void* dst = bitOut.GetDst();
    ASSERT_NE(dst, (const void*)IZ_NULL);

    IZ_UINT tmp = *(const IZ_UINT*)dst;
    ASSERT_EQ(tmp, 0x05);
}

TEST(BitStreamOutput, InvalidArgument_Over32)
{
    IZ_UINT out = 0;
    izanagi::CStdBitOutput bitOut(&out, sizeof(out));

    ASSERT_TRUE(bitOut.IsValid());

    ASSERT_TRUE(bitOut.Write(value_1, 128));
    ASSERT_TRUE(bitOut.Flush());

    const void* dst = bitOut.GetDst();
    ASSERT_NE(dst, (const void*)IZ_NULL);

    IZ_UINT tmp = *(const IZ_UINT*)dst;
    ASSERT_EQ(tmp, value_1);
}

TEST(BitStreamOutput, InvalidArgument_Minus)
{
    IZ_UINT out = 0;
    izanagi::CStdBitOutput bitOut(&out, sizeof(out));

    ASSERT_TRUE(bitOut.IsValid());

    ASSERT_TRUE(bitOut.Write(value_1, -1));
    ASSERT_TRUE(bitOut.Flush());

    const void* dst = bitOut.GetDst();
    ASSERT_NE(dst, (const void*)IZ_NULL);

    IZ_UINT tmp = *(const IZ_UINT*)dst;
    ASSERT_EQ(tmp, value_1);
}

TEST(BitStreamOutput, Reset_1)
{
    IZ_UINT out = 0;
    izanagi::CStdBitOutput bitOut(&out, sizeof(out));

    IZ_UINT out2 = 0;
    ASSERT_TRUE(bitOut.Reset(&out2, sizeof(out2)));

    const void* dst = bitOut.GetDst();
    ASSERT_EQ(dst, &out2);
}

TEST(BitStreamOutput, Reset_2)
{
    izanagi::CStdBitOutput bitOut(4);

    ASSERT_TRUE(bitOut.Reset(4));
}

TEST(BitStreamOutput, GetFlushedBytes)
{
    IZ_UINT out = 0;
    izanagi::CStdBitOutput bitOut(&out, sizeof(out));

    ASSERT_TRUE(bitOut.IsValid());

    ASSERT_TRUE(bitOut.Write(value_1, 4));
    ASSERT_TRUE(bitOut.Flush());

    ASSERT_EQ(bitOut.GetWriteBytes(), 1);

    ASSERT_TRUE(bitOut.Write(value_1, 12));
    ASSERT_TRUE(bitOut.Flush());

    ASSERT_EQ(bitOut.GetWriteBytes(), 3);
}

TEST(BitStreamOutput, GetWriteBytes)
{
    IZ_UINT out = 0;
    izanagi::CStdBitOutput bitOut(&out, sizeof(out));

    ASSERT_TRUE(bitOut.IsValid());

    ASSERT_TRUE(bitOut.Write(value_1, 4));

    ASSERT_EQ(bitOut.GetWriteBytes(), 0);

    ASSERT_TRUE(bitOut.Write(value_1, 12));
    ASSERT_TRUE(bitOut.Flush());

    ASSERT_EQ(bitOut.GetWriteBytes(), 2);
}
