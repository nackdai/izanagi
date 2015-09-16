#if !defined(__MODEL_LIB_VMD_LOADER_H__)
#define __MODEL_LIB_VMD_LOADER_H__

#include "izDefs.h"
#include "izToolKit.h"
#include "VMDFormat.h"
#include "izSceneGraph.h"
#include <vector>
#include <set>

class CPmdLoader;

struct SFrameInfo
{
    const SVmdFrame& frame;

    SFrameInfo(const SVmdFrame& f) : frame(f)
    {
    }

    bool operator<(const SFrameInfo& rhs) const
    {
        return frame.frameNo < rhs.frame.frameNo;
    }
};

class CVmdLoader {
public:
    CVmdLoader() {}
    ~CVmdLoader() {}

    NO_COPIABLE(CVmdLoader);

public:
    // �t�@�C�����[�h
    IZ_BOOL Load(IZ_PCSTR path);

    // �N���A
    void Clear();

    // �t���[���f�[�^�擾
    const SVmdFrame& GetFrame(IZ_UINT idx);

    // �ړ��A�j���[�V�������L�����ǂ���
    IZ_BOOL IsValidTrans(IZ_UINT idx);

    // ��]�A�j���[�V�������L�����ǂ���
    IZ_BOOL IsValidRotate(IZ_UINT idx);

    // �`�����l�������擾.
    IZ_UINT GetChannelNum(IZ_UINT idx);

    // �t���[���̐�����s��
    IZ_BOOL MakeFrameOrder(const CPmdLoader& pmd);

    // �{�[�����Ƃɂ܂Ƃ߂�ꂽ�t���[�������擾
    const std::set<SFrameInfo>& GetFrameInfo(IZ_UINT idx);

    // �t���[�������擾.
    const SVmdFrame* GetFrameInfo(
        IZ_UINT nodeIdx,
        IZ_UINT frameIdx);

private:
    IZ_BOOL LoadFrameChunk(izanagi::IInputStream* stream);

private:
    SVmdHeader m_Header;
    std::vector<SVmdFrame> m_FrameList;

    std::vector< std::set<SFrameInfo> > m_FrameOrder;
};

#endif  // #if !defined(__MODEL_LIB_VMD_LOADER_H__)
