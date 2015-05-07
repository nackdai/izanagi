#include "std/StdBitStream.h"
#include "std/allocator/MemoryAllocator.h"

namespace izanagi {
    CStdBitInput::CStdBitInput(void* src, size_t size)
    {
        m_Src = reinterpret_cast<IZ_BYTE*>(src);
        m_Size = static_cast<IZ_UINT>(size);

        m_CurData = 0;
        m_CurBitPos = 0;

        m_ReadBytes = 0;
    }

    /**
    */
    IZ_UINT CStdBitInput::Read(IZ_UINT bitCnt)
    {
        IZ_ASSERT(m_ReadBytes < m_Size);
        IZ_ASSERT(bitCnt <= 32);

        if (m_ReadBytes >= m_Size) {
            // Nothing is done.
            return 0;
        }

        // Max 32 bit (= 4 byte)
        bitCnt = (bitCnt > 32 ? 32 : bitCnt);

        IZ_UINT ret = 0;

        for (IZ_UINT i = 0; i < bitCnt; ++i) {
            if (m_CurBitPos == 0) {
                // Get current 1byte(= 8bit) data.
                m_CurData = *(m_Src + m_ReadBytes);
            }

            // Read 1bit.
            ret = (ret << 1) | ((m_CurData >> m_CurBitPos) & 0x01);
            ++m_CurBitPos;

            if (m_CurBitPos == 8) {
                // Read 8bit, and count up read bytes.
                ++m_ReadBytes;

                // Reset bit position.
                m_CurBitPos = 0;
            }

            // If read bytes over source data size, it will be finished.
            if (m_ReadBytes >= m_Size) {
                break;
            }
        }

        return ret;
    }

    IZ_UINT CStdBitInput::GetReadBytes() const
    {
        return m_ReadBytes;
    }
    
    IZ_BOOL CStdBitInput::IsValid() const
    {
        return (m_Src != IZ_NULL);
    }

    /////////////////////////////////////////

    CStdBitOutput::CStdBitOutput(void* dst, size_t size)
    {
        m_Dst = reinterpret_cast<IZ_BYTE*>(dst);
        m_Size = static_cast<IZ_UINT>(size);

        m_IsAllocated = IZ_FALSE;

        m_CurData = 0;
        m_CurBitPos = 0;

        m_WriteBytes = 0;
    }

    CStdBitOutput::CStdBitOutput(size_t size)
    {
        m_Dst = reinterpret_cast<IZ_BYTE*>(CMemoryAllocator::Alloc(size));
        m_Size = static_cast<IZ_UINT>(size);

        m_IsAllocated = IZ_TRUE;

        m_CurData = 0;
        m_CurBitPos = 0;

        m_WriteBytes = 0;
    }

    CStdBitOutput::~CStdBitOutput()
    {
        if (m_IsAllocated) {
            CMemoryAllocator::Free(m_Dst);
        }
    }

    /**
    */
    IZ_BOOL CStdBitOutput::Write(IZ_UINT val, IZ_UINT bitCnt)
    {
        // Max 32 bit (= 4 byte)
        bitCnt = (bitCnt > 32 ? 32 : bitCnt);

        for (IZ_UINT i = 0; i < bitCnt; ++i) {
            // Write 1bit.
            m_CurData = ((m_CurData << 1) | ((val >> i) & 0x01));
            ++m_CurBitPos;

            // If write bit size is 8bit(=1byte), flush data.
            if (m_CurBitPos == 8) {
                VRETURN(Flush());
            }
        }

        return IZ_TRUE;
    }

    /**
    */
    IZ_BOOL CStdBitOutput::Flush()
    {
        if (m_WriteBytes >= m_Size) {
            return IZ_FALSE;
        }

        if (m_CurBitPos > 0) {
            // Write current 1byte data to destination buffer.
            *(m_Dst++) = m_CurData;

            // Reset data, bit position.
            m_CurData = 0;
            m_CurBitPos = 0;

            // Count up wrote bytes.
            ++m_WriteBytes;
        }

        return IZ_TRUE;
    }

    const void* CStdBitOutput::GetDst()
    {
        return m_Dst;
    }

    IZ_UINT CStdBitOutput::GetWriteBytes() const
    {
        return m_WriteBytes;
    }

    IZ_BOOL CStdBitOutput::IsValid() const
    {
        return (m_Dst != IZ_NULL);
    }
}   // namespace izanagi
