#include "shader/base/table/ShaderPassTable.h"

namespace izanagi
{
namespace shader
{
    size_t CShaderPassTable::ComputeBufferSize(IZ_UINT16 numPass)
    {
        size_t ret = sizeof(SPassParameterHolder) * numPass;
        return ret;
    }

    // �R���X�g���N�^
    CShaderPassTable::CShaderPassTable()
    {
        FILL_ZERO(&m_Header, sizeof(m_Header));
        m_pDesc = IZ_NULL;
        m_Holder = IZ_NULL;

        m_UsedParamIdxList = IZ_NULL;
    }

    // ������
    IZ_UINT8* CShaderPassTable::Init(IZ_UINT8* p)
    {
        // NOTE
        // +----------------+
        // |  �p�X�e�[�u��  |
        // +----------------+
        // |�p�X�ŗ��p�����|
        // |  �p�����[�^��  |
        // |   �T���v����   |
        // |  �C���f�b�N�X  |
        // +----------------+

        memcpy(&m_Header, p, sizeof(m_Header));
        p += sizeof(m_Header);

        // �p�X�L�q
        if (m_Header.numPass > 0) {
            m_pDesc = (S_SHD_PASS*)p;
            p += sizeof(S_SHD_PASS) * m_Header.numPass;
        }

        // �Q�Ƃ���p�����[�^ or �T���v���C���f�b�N�X
        if (m_Header.numPass > 0) {
            // TODO
            // ���̏����ł����̂��H
            m_UsedParamIdxList = reinterpret_cast<S_SHD_PARAM_IDX*>(p);

            // �|�C���^��i�߂�E�E�E
            for (IZ_UINT i = 0; i < m_Header.numPass; ++i) {
                S_SHD_PASS& sDesc = m_pDesc[i];

                if (sDesc.numConst > 0) {
                    p += sizeof(S_SHD_PARAM_IDX) * sDesc.numConst;
                }
                if (sDesc.numSampler > 0) {
                    p += sizeof(S_SHD_PARAM_IDX) * sDesc.numSampler;
                }
            }
        }
        
        return p;
    }

    IZ_UINT8* CShaderPassTable::SetInternalBuffer(IZ_UINT8* p)
    {
        if (m_Header.numPass > 0) {
            m_Holder = (SPassParameterHolder*)p;
            p += sizeof(SPassParameterHolder) * m_Header.numPass;

            IZ_UINT nPos = 0;
            
            for (IZ_UINT i = 0; i < m_Header.numPass; ++i) {
                S_SHD_PASS& sDesc = m_pDesc[i];

                if (sDesc.numConst > 0) {
                    m_Holder[i].ptrConst = m_UsedParamIdxList + nPos;
                    nPos += sDesc.numConst;
                }

                if (sDesc.numSampler > 0) {
                    m_Holder[i].ptrSampler = m_UsedParamIdxList + nPos;
                    nPos += sDesc.numSampler;
                }
            }
        }
        
        return p;
    }

    // �p�X���擾
    IZ_UINT CShaderPassTable::GetPassNum() const
    {
        return m_Header.numPass;
    }

    // �p�X�L�q�擾
    const S_SHD_PASS* CShaderPassTable::GetDesc(IZ_UINT idx) const
    {
        IZ_ASSERT(idx < m_Header.numPass);
        IZ_ASSERT(m_pDesc != IZ_NULL);

        S_SHD_PASS* ret = &m_pDesc[idx];
        return ret;
    }

    IZ_UINT CShaderPassTable::GetConstIdx(IZ_UINT passIdx, IZ_UINT constPos) const
    {
        IZ_ASSERT(passIdx < m_Header.numPass);
        IZ_ASSERT(m_pDesc != IZ_NULL);
        IZ_ASSERT(m_Holder != IZ_NULL);

        IZ_ASSERT(constPos < m_pDesc[passIdx].numConst);

        return m_Holder[passIdx].ptrConst[constPos].idx;
    }

    IZ_UINT CShaderPassTable::GetSmplIdx(IZ_UINT passIdx, IZ_UINT smplPos) const
    {
        IZ_ASSERT(passIdx < m_Header.numPass);
        IZ_ASSERT(m_pDesc != IZ_NULL);
        IZ_ASSERT(m_Holder != IZ_NULL);

        IZ_ASSERT(smplPos < m_pDesc[passIdx].numSampler);

        return m_Holder[passIdx].ptrSampler[smplPos].idx;
    }
}   // namespace shader
}   // namespace izanagi
