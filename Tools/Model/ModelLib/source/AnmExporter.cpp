#include "AnmExporter.h"
#include "izToolKit.h"

// NOTE
// フォーマット
// +----------------+
// | ファイルヘッダ |
// +----------------+
// | アニメーション |
// |     ノード     |
// +----------------+
// | アニメーション |
// |   チャンネル   |
// +----------------+
// | アニメーション |
// |      キー      |
// +----------------+

CAnmExporter CAnmExporter::s_cInstance;

CAnmExporter::CAnmExporter()
{
}

CAnmExporter::~CAnmExporter()
{
    m_Out.Finalize();
}

IZ_BOOL CAnmExporter::Export(
    IZ_PCSTR lpszOutFile,
    IZ_UINT nSetIdx,
    IImporter* pImporter)
{
    IZ_BOOL ret = IZ_TRUE;

    VRETURN(m_Out.Open(lpszOutFile));

    izanagi::S_ANM_HEADER sHeader;
    {
        FILL_ZERO(&sHeader, sizeof(sHeader));

        sHeader.sizeHeader = sizeof(sHeader);

        // TODO
        sHeader.keyType = izanagi::E_ANM_KEY_TYPE_TIME;
    }

    // Blank for file's header.
    izanagi::tool::CIoStreamSeekHelper cSeekHelper(&m_Out);
    VRETURN(cSeekHelper.Skip(sizeof(sHeader)));

    VRETURN(pImporter->BeginAnm(nSetIdx));

    IZ_UINT nNodeNum = pImporter->GetAnmNodeNum();

    // Export nodes.
    {
        IZ_UINT channelIdx = 0;

        for (IZ_UINT i = 0; i < nNodeNum; i++) {
            izanagi::S_ANM_NODE sNode;
            FILL_ZERO(&sNode, sizeof(sNode));

            VRETURN(pImporter->GetAnmNode(i, sNode));

            sNode.channelIdx = channelIdx;
            channelIdx += sNode.numChannels;

            IZ_OUTPUT_WRITE_VRETURN(&m_Out, &sNode, 0, sizeof(sNode));
        }
    }

    IZ_UINT nChannelNum = 0;
    std::vector<IZ_UINT> tvKeyNum;

    IZ_UINT keyIdx = 0;

    // Export channels,
    for (IZ_UINT nNodeIdx = 0; nNodeIdx < nNodeNum; nNodeIdx++) {
        IZ_UINT nChannelCnt = pImporter->GetAnmChannelNum(nNodeIdx);

        for (IZ_UINT nChannelIdx = 0; nChannelIdx < nChannelCnt; nChannelIdx++) {
            izanagi::S_ANM_CHANNEL sChannel;
            FILL_ZERO(&sChannel, sizeof(sChannel));

            VRETURN(
                pImporter->GetAnmChannel(
                    nNodeIdx,
                    nChannelIdx,
                    sChannel));

            sChannel.keyIdx = keyIdx;
            keyIdx += sChannel.numKeys;

            IZ_OUTPUT_WRITE_VRETURN(&m_Out, &sChannel, 0, sizeof(sChannel));

            tvKeyNum.push_back(sChannel.numKeys);
        }

        nChannelNum += nChannelCnt;
    }

    IZ_UINT nKeyNum = 0;
    IZ_UINT nKeyPos = 0;

    IZ_FLOAT fMaxTime = 0.0f;

    // Export keys.
    for (IZ_UINT nNodeIdx = 0; nNodeIdx < nNodeNum; nNodeIdx++) {
        IZ_UINT nChannelCnt = pImporter->GetAnmChannelNum(nNodeIdx);

        for (IZ_UINT nChannelIdx = 0; nChannelIdx < nChannelCnt; nChannelIdx++) {
            IZ_UINT nKeyCnt = tvKeyNum[nKeyPos++];

            for (IZ_UINT nKeyIdx = 0; nKeyIdx < nKeyCnt; nKeyIdx++) {
                izanagi::S_ANM_KEY sKey;
                FILL_ZERO(&sKey, sizeof(sKey));

                std::vector<IZ_FLOAT> tvValue;

                VRETURN(
                    pImporter->GetAnmKey(
                        nNodeIdx,
                        nChannelIdx,
                        nKeyIdx,
                        sKey,
                        tvValue));

                fMaxTime = (sKey.keyTime > fMaxTime ? sKey.keyTime : fMaxTime);

                sKey.numParams = static_cast<IZ_UINT8>(tvValue.size());
                sKey.value = 0;

                IZ_OUTPUT_WRITE_VRETURN(&m_Out, &sKey, 0, sizeof(sKey));

#if 0
                IZ_INT nOffset = sizeof(IZ_FLOAT);
                VRETURN(m_Out.Seek(-nOffset, izanagi::E_IO_STREAM_SEEK_POS_CUR));
#endif

                IZ_OUTPUT_WRITE_VRETURN(&m_Out, &tvValue[0], 0, sizeof(IZ_FLOAT) * tvValue.size());
            }

            nKeyNum += nKeyCnt;
        }
    }

    VRETURN(pImporter->EndAnm());

    // Export files's header.
    {
        sHeader.numNodes = nNodeNum;
        sHeader.numChannels = nChannelNum;
        sHeader.numKeys = nKeyNum;

        sHeader.sizeFile = m_Out.GetCurPos();

        sHeader.time = fMaxTime;

        VRETURN(cSeekHelper.Return());
        IZ_OUTPUT_WRITE_VRETURN(&m_Out, &sHeader, 0, sizeof(sHeader));
    }

    return IZ_TRUE;
}
