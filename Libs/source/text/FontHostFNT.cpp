#include "text/FontHostFNT.h"
#include "izIo.h"
#include "izGraph.h"
#include "izStd.h"

namespace izanagi
{
namespace text
{
    // �C���X�^���X�쐬
    CFontHostFNT* CFontHostFNT::CreateFontHostFNT(
        IMemoryAllocator* allocator,
        IInputStream* in)
    {
        IZ_ASSERT(allocator != IZ_NULL);
        IZ_ASSERT(in != IZ_NULL);

        IZ_UINT8* buf = IZ_NULL;
        CFontHostFNT* instance = IZ_NULL;

        // �w�b�_�ǂݍ���
        S_FNT_HEADER header;
        IZ_BOOL result = IZ_INPUT_READ(in, &header, 0, sizeof(header));
        VGOTO(result, __EXIT__);

        // �}�W�b�N�i���o�[�`�F�b�N
        result = IS_FMT_ENCODE(header.magic);
        VGOTO(result, __EXIT__);

        // �m�ۃT�C�Y�v�Z
        size_t size = sizeof(CFontHostFNT);
        size += header.sizeFile - header.sizeHeader;

        buf = (IZ_UINT8*)ALLOC(allocator, size);

        instance = new(buf) CFontHostFNT;
        {
            buf += sizeof(CFontHostFNT);

            instance->AddRef();

            instance->m_Allocator = allocator;

            memcpy(&instance->m_Header, &header, sizeof(header));

            // ���������蓖��
            {
                // �t�H���g�}�b�v���
                instance->m_MapList = reinterpret_cast<S_FNT_MAP*>(buf);
                buf += sizeof(S_FNT_MAP) * header.numFont;

                // �t�H���g�C���[�W���
                instance->m_ImageList = reinterpret_cast<S_FNT_IMAGE*>(buf);
                buf += sizeof(S_FNT_IMAGE) * header.numTex;

                // NOTE
                // �t�H���g�C���[�W�f�[�^�́A��񕔂�ǂݍ��܂Ȃ��ƃT�C�Y���m�肵�Ȃ��̂ŁA
                // ��Ŋ��蓖�Ă�
            }

            result = instance->Read(in, buf);
        }

__EXIT__:
        if (!result)
        {
            if (instance != IZ_NULL)
            {
                SAFE_RELEASE(instance);
            }
            else if (buf != IZ_NULL)
            {
                FREE(allocator, buf);
            }
        }

        return instance;
    }

    CFontHostFNT::CFontHostFNT()
    {
        m_Allocator = IZ_NULL;

        // �w�b�_
        CStdUtil::FillZero(&m_Header, 1);

        // �t�H���g�}�b�v���
        m_MapList = IZ_NULL;

        // �t�H���g�C���[�W���
        m_ImageList = IZ_NULL;
    }

    CFontHostFNT::~CFontHostFNT()
    {
    }

    IZ_BOOL CFontHostFNT::Read(
        IInputStream* in,
        IZ_UINT8* buf)
    {
        // �t�H���g�}�b�v���ǂݍ���
        IZ_BOOL ret = IZ_INPUT_READ(
            in, 
            m_MapList,
            0,
            sizeof(S_FNT_MAP) * m_Header.numFont);
        VRETURN(ret);

        // �t�H���g�C���[�W���ǂݍ���
        for (IZ_UINT i = 0; i < m_Header.numTex; ++i) {
            // ��񕔓ǂݍ���
            ret = IZ_INPUT_READ(
                in,
                &m_ImageList[i],
                0,
                sizeof(S_FNT_IMAGE));
            VRETURN(ret);

            m_ImageList[i].images = reinterpret_cast<const IZ_UINT8*>(buf);
            buf += m_ImageList[i].sizeData;

            IZ_UINT8* readDst = const_cast<IZ_UINT8*>(m_ImageList[i].images);

            // �f�[�^���ǂݍ���
            ret = IZ_INPUT_READ(
                in,
                reinterpret_cast<void*>(readDst),
                0,
                m_ImageList[i].sizeData);
            VRETURN(ret);
        }

        return IZ_TRUE;
    }

    // �}�b�v���擾
    const S_FNT_MAP* CFontHostFNT::GetFontMap(IZ_UINT code)
    {
        S_FNT_MAP* ret = IZ_NULL;

        // �񕪒T������
        IZ_INT idxTop = m_MapList[0].idx;
        IZ_INT idxTail = m_MapList[m_Header.numFont - 1].idx;

        while (idxTop <= idxTail) {
            IZ_UINT idxMid = ((idxTop + idxTail) >> 1);
            S_FNT_MAP* pMid = &m_MapList[idxMid];

            if (pMid->code < code) {
                idxTop = idxMid + 1;
                VRETURN_NULL(idxTop < m_Header.numFont);
            }
            else if (pMid->code > code) {
                idxTail = idxMid - 1;
                VRETURN_NULL(idxTail >= 0);
            }
            else {
                // ������
                ret = pMid;
                break;
            }
        }

        if (ret == IZ_NULL) {
            // ���`�T�����Ă݂�
            for (IZ_UINT i = 0; i < m_Header.numFont; i++) {
                if (m_MapList[i].code == code) {
                    ret = &m_MapList[i];
                    break;
                }
            }
            IZ_ASSERT(ret != IZ_NULL);
        }

        return ret;
    }

    const S_FNT_MAP* CFontHostFNT::GetFontMapByID(IZ_UINT id)
    {
        // NOTE
        // 0�͖���`�̈Ӗ��Ȃ̂� -1 ����K�v������
        IZ_ASSERT(id - 1 < m_Header.numFont);

        S_FNT_MAP& map = m_MapList[id];

        return &map;
    }

    IZ_UINT CFontHostFNT::GetGlyphID(IZ_UINT code)
    {
        const S_FNT_MAP* map = GetFontMap(code);

        if (map != IZ_NULL)
        {
            return map->idx;
        }

        // NOTE
        // 0�͖���`�̈Ӗ�
        return 0;
    }

    IZ_BOOL CFontHostFNT::GetGlyphMetricsByID(IZ_UINT id, SGlyphMetrics& metrics)
    {
        const S_FNT_MAP* map = GetFontMapByID(id);

        metrics.advance = map->metrics.advance;
        metrics.bearingX = map->metrics.bearingX;
        metrics.bearingY = map->metrics.bearingY;
        metrics.width = map->metrics.width;
        metrics.height = map->metrics.height;

        return IZ_TRUE;
    }

    IZ_BOOL CFontHostFNT::GetGlyphMetricsByCode(IZ_UINT code, SGlyphMetrics& metrics)
    {
        IZ_UINT id = GetGlyphID(code);
        return GetGlyphMetricsByID(id, metrics);
    }

    IZ_UINT CFontHostFNT::GetPixelSize()
    {
        return m_Header.pixelSize;
    }

    IZ_BOOL CFontHostFNT::GetImage(
        IZ_UINT code,
        SGlyphImage& image,
        SGlyphMetrics& metrics)
    {
        IZ_UINT id = GetGlyphID(code);
        return GetImageByID(id, image, metrics);
    }

    IZ_BOOL CFontHostFNT::GetImageByID(
        IZ_UINT id,
        SGlyphImage& image,
        SGlyphMetrics& metrics)
    {
        const S_FNT_MAP* map = GetFontMapByID(id);
        S_FNT_IMAGE& fntImage = m_ImageList[map->texID];

        {
            image.leftOffset = map->leftOffset;
            image.topOffset = map->topOffset;
            image.rowBytes = map->metrics.width;
            image.rows = map->metrics.height;
            image.pitch = fntImage.width;

            image.bmp = fntImage.images + map->srcX + map->srcX * image.pitch;
        }

        {
            metrics.advance = map->metrics.advance;
            metrics.bearingX = map->metrics.bearingX;
            metrics.bearingY = map->metrics.bearingY;
            metrics.width = map->metrics.width;
            metrics.height = map->metrics.height;
        }

        return IZ_TRUE;
    }

    IZ_UINT CFontHostFNT::GetAscender()
    {
        return m_Header.ascender;
    }

    IZ_UINT CFontHostFNT::GetDescender()
    {
        return m_Header.descender;
    }

    IZ_UINT CFontHostFNT::GetUnitsPerEM()
    {
        // TODO
        IZ_ASSERT(IZ_FALSE);
        return 0;
    }
}    // namespace text
}    // namespace izanagi
