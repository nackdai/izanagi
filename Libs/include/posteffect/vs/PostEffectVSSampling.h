#if !defined(__IZANAGI_POSTEFFECT_VS_SAMPLING_H__)
#define __IZANAGI_POSTEFFECT_VS_SAMPLING_H__

#include "PostEffectVS.h"

namespace izanagi {
    ///////////////////////////////////////////////////
    /**
    * 周囲Ｎ点をサンプリングする頂点シェーダ
    */
    template
    <
        IZ_UINT _NUM,           // サンプリングする点数
        IZ_UINT _VECTOR_NUM // サンプリング位置（ベクトル）数
    >
    class CPostEffectVSSampling : public CPostEffectVS {
    protected:
        CPostEffectVSSampling()
        {
            m_hOffset = 0;
            FILL_ZERO(m_vOffset, sizeof(m_vOffset));
        }

        ~CPostEffectVSSampling() {}

    public:
        // パラメータセット
        virtual void RegisterParameter(
            const math::SVector4* pVector,
            IZ_UINT num)
        {
            IZ_ASSERT(num <= _VECTOR_NUM);

            for (IZ_UINT i = 0; i < num; ++i) {
                math::SVector4::Copy(m_vOffset[i], pVector[i]);
            }
        }

        // シェーダパラメータ初期化済みかどうか
        virtual IZ_BOOL IsInitilizedShaderParameter()
        {
            if (CPostEffectVS::IsInitilizedShaderParameter())
            {
                if (graph::CShaderProgram::IsValidHandle(m_hOffset))
                {
                    return IZ_TRUE;
                }
            }
            return IZ_FALSE;
        }

        // シェーダパラメータ初期化
        virtual void InitShaderParameter(
            graph::CGraphicsDevice* device,
            graph::CShaderProgram* program)
        {
            CPostEffectVS::InitShaderParameter(device, program);

            static IZ_PCSTR name = "g_vUVOffsetSampling";
            m_hOffset = program->GetHandleByName(name);
            IZ_ASSERT(graph::CShaderProgram::IsValidHandle(m_hOffset));
        }

        // パラメータセット
        virtual void ApplyShaderParameter(
            graph::CGraphicsDevice* device,
            graph::CShaderProgram* program)
        {
            IZ_ASSERT(graph::CShaderProgram::IsValidHandle(m_hOffset));

            program->SetVectorArray(
                device,
                m_hOffset,
                m_vOffset,
                COUNTOF(m_vOffset));
        }

    protected:
        SHADER_PARAM_HANDLE m_hOffset;
        math::SVector4 m_vOffset[_VECTOR_NUM];
    };

    ////////////////////////////////
    /**
    * １点をサンプリング（デフォルト）
    */
    class CPostEffectVSSampling_1 : public CPostEffectVS {
        friend class CPostEffectVS;

    protected:
        CPostEffectVSSampling_1()
        {
            m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_1;
        }

        ~CPostEffectVSSampling_1() {}
    };

    ////////////////////////////////
    /**
    * 周囲４点をサンプリング
    */
    class CPostEffectVSSampling_4 : public CPostEffectVSSampling<4, 2> {
        friend class CPostEffectVS;

    protected:
        CPostEffectVSSampling_4()
        {
            m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_4;
        }

        ~CPostEffectVSSampling_4() {}
    };

    ////////////////////////////////
    /**
    * 周囲８点をサンプリング
    */
    class CPostEffectVSSampling_8 : public CPostEffectVSSampling<8, 4> {
        friend class CPostEffectVS;

    protected:
        CPostEffectVSSampling_8()
        {
            m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_8;
        }

        ~CPostEffectVSSampling_8() {}
    };

    ////////////////////////////////
    /**
    * 周囲１６点をサンプリング
    */
    class CPostEffectVSSampling_16 : public CPostEffectVSSampling<16, 8> {
        friend class CPostEffectVS;

    protected:
        CPostEffectVSSampling_16()
        {
            m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_16;
        }

        ~CPostEffectVSSampling_16() {}
    };

    ////////////////////////////////
    /**
    * 周囲５点をサンプリング
    */
    class CPostEffectVSSampling_5 : public CPostEffectVSSampling<5, 3> {
        friend class CPostEffectVS;

    protected:
        CPostEffectVSSampling_5()
        {
            m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_5;
        }

        ~CPostEffectVSSampling_5() {}
    };

    ////////////////////////////////
    /**
    * 周囲７点をサンプリング
    */
    class CPostEffectVSSampling_7 : public CPostEffectVSSampling<7, 4> {
        friend class CPostEffectVS;

    protected:
        CPostEffectVSSampling_7()
        {
            m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_7;
        }

        ~CPostEffectVSSampling_7() {}
    };

    ////////////////////////////////
    /**
    * 周囲９点をサンプリング
    */
    class CPostEffectVSSampling_9 : public CPostEffectVSSampling<9, 5> {
        friend class CPostEffectVS;

    protected:
        CPostEffectVSSampling_9()
        {
            m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_9;
        }

        ~CPostEffectVSSampling_9() {}
    };

    ////////////////////////////////
    /**
    * 周囲１３点をサンプリング
    */
    class CPostEffectVSSampling_13 : public CPostEffectVSSampling<13, 7> {
        friend class CPostEffectVS;

    protected:
        CPostEffectVSSampling_13()
        {
            m_Type = E_POSTEFFECT_VTX_SHADER_SAMPLING_13;
        }

        ~CPostEffectVSSampling_13() {}
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_POSTEFFECT_VS_SAMPLING_H__)
