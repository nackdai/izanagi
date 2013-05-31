#include "shader/base/ShaderBasic.h"
#include "izIo.h"

using namespace izanagi;

IZ_BOOL CShaderBasic::Init(
    graph::CGraphicsDevice* pDevice,
    IInputStream* pIn)
{
    IZ_ASSERT(m_Allocator != IZ_NULL);

    // Read file's header.
    IZ_INPUT_READ_VRETURN(pIn, (IZ_UINT8*)&m_Header, 0, sizeof(m_Header));

    // TODO
    // Check magic number...

    // Compute reading size.
    size_t nReadSize = m_Header.posProgram - m_Header.sizeHeader;

    // Compute allocating size.
    size_t nBufSize = nReadSize;
    nBufSize += sizeof(CShaderPass) * m_Header.numPass;
    nBufSize += CShaderPass::ComputeBufferSize(
                    m_Header.numParam,
                    m_Header.numSmpl);

    // Allocate buffer.
    IZ_UINT8* pBuffer = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(m_Allocator, nBufSize));
    VRETURN(pBuffer != IZ_NULL);

    // Keep top of allocated buffer.
    m_pBuffer = pBuffer;

    // Read data from input stream.
    IZ_INPUT_READ_ASSERT(pIn, pBuffer, 0, nReadSize);

    // Read chunks.
    for (;;) {
        S_SHD_CHUNK_HEADER sChunkHeader;
        memcpy(&sChunkHeader, pBuffer, sizeof(sChunkHeader));
        pBuffer += sizeof(sChunkHeader);

        if (sChunkHeader.magicChunk == SHD_PROGRAM_CHUNK_TERMINATER) {
            // End of data.
            break;
        }
        else if (sChunkHeader.magicChunk ==  SHD_CHUNK_MAGIC_NUMBER_STRING) {
            // String
            pBuffer = m_StringBuffer.Init(m_Header.sizeStringBuffer, pBuffer);
        }
        else if (sChunkHeader.magicChunk ==  SHD_CHUNK_MAGIC_NUMBER_PARAM) {
            // Parameters.
            pBuffer = m_ParamTbl.Init(pBuffer);
        }
        else if (sChunkHeader.magicChunk ==  SHD_CHUNK_MAGIC_NUMBER_TEX) {
            // Textures.
            pBuffer = m_TexTbl.Init(pBuffer);
        }
        else if (sChunkHeader.magicChunk ==  SHD_CHUNK_MAGIC_NUMBER_SMPL) {
            // Samplers
            pBuffer = m_SmplTbl.Init(pBuffer);
        }
        else if (sChunkHeader.magicChunk ==  SHD_CHUNK_MAGIC_NUMBER_PASS) {
            // Passes
            pBuffer = m_PassTbl.Init(pBuffer);
        }
        else if (sChunkHeader.magicChunk ==  SHD_CHUNK_MAGIC_NUMBER_TECH) {
            // Techniques.
            pBuffer = m_TechTbl.Init(pBuffer);
        }
        else if (sChunkHeader.magicChunk ==  SHD_CHUNK_MAGIC_NUMBER_PROG) {
            // Allocate buffer for CShaderPass.
            m_pPass = reinterpret_cast<CShaderPass*>(pBuffer);
            pBuffer += sizeof(CShaderPass) * m_Header.numPass;

            pBuffer = CreatePass(pDevice, pIn, pBuffer);

            // This is terminator, so end of data.
            break;
        }
        else if (sChunkHeader.magicChunk == SHD_CHUNK_MAGIC_NUMBER_ATTR) {
            // Attributes.
            pBuffer = m_AttrTbl.Init(pBuffer);
        }
    }

    VRETURN(CStdUtil::GetPtrDistance(m_pBuffer, pBuffer) == nBufSize);

    // Initialize item of hash and list for CShaderManager,
    InitItem(m_Header.nameKey, this);

    return IZ_TRUE;
}

CShaderBasic::CShaderBasic()
{
    m_Allocator = IZ_NULL;
    m_pDevice = IZ_NULL;

    m_pBuffer = IZ_NULL;

    FILL_ZERO(&m_Header, sizeof(m_Header));

    m_pPass = IZ_NULL;

    m_bIsSavedRS = IZ_FALSE;

    m_nCurTech = -1;
    m_nCurPass = -1;
}

CShaderBasic::~CShaderBasic()
{
    for (IZ_UINT i = 0; i < m_Header.numPass; i++) {
        m_pPass[i].Clear();
    }

    SAFE_RELEASE(m_pDevice);
    FREE(m_Allocator, m_pBuffer);
}

IZ_UINT8* CShaderBasic::CreatePass(
    graph::CGraphicsDevice* pDevice,
    IInputStream* pIn,
    IZ_UINT8* pBuffer)
{
    IZ_ASSERT(m_Allocator != IZ_NULL);
    IZ_ASSERT(m_pDevice != IZ_NULL);
    IZ_ASSERT(m_pPass != IZ_NULL);

    // プログラム読み込み用バッファ
    void* pProgramBuf = ALLOC(m_Allocator, m_Header.maxProgamSize);
    VRETURN_NULL(pProgramBuf != IZ_NULL);

    for (IZ_UINT i = 0; i < m_Header.numPass; ++i) {
        CShaderPass& cPass = m_pPass[i];
        const S_SHD_PASS* pDesc = m_PassTbl.GetDesc(i);

        cPass.SetDesc(pDesc);

        // Vertex Program
        VGOTO(
            IZ_INPUT_READ(pIn, pProgramBuf, 0, pDesc->sizeVS),
            __EXIT__);
            
        graph::CVertexShader* pVS = pDevice->CreateVertexShader(pProgramBuf);
        VGOTO(pVS != IZ_NULL, __EXIT__);

        // Pixel Program
        VGOTO(
            IZ_INPUT_READ(pIn, pProgramBuf, 0, pDesc->sizePS),
            __EXIT__);

        graph::CPixelShader* pPS = pDevice->CreatePixelShader(pProgramBuf);
        VGOTO(pPS != IZ_NULL, __EXIT__);

        // Shader Program
        graph::CShaderProgram* program = pDevice->CreateShaderProgram();
        VGOTO(program != IZ_NULL, __EXIT__);

        program->AttachVertexShader(pVS);
        program->AttachPixelShader(pPS);

        // ShaderProgramにオーナーを委譲する
        SAFE_RELEASE(pVS);
        SAFE_RELEASE(pPS);

        cPass.SetShaderProgram(program);

        // Passにオーナーを委譲する
        SAFE_RELEASE(program);

        pBuffer = cPass.SetParamBuffer(pDesc->numConst, pBuffer);
        pBuffer = cPass.SetSamplerBuffer(pDesc->numSampler, pBuffer);

        // For parameters.
        for (IZ_UINT n = 0; n < pDesc->numConst; n++) {
            IZ_UINT nParamIdx = pDesc->GetConstIdx(n);

            const S_SHD_PARAMETER* pParamDesc = m_ParamTbl.GetDesc(nParamIdx);
            IZ_ASSERT(pParamDesc != IZ_NULL);

            IZ_PCSTR pszName = m_StringBuffer.GetString(pParamDesc->posName);
            IZ_ASSERT(pszName != IZ_NULL);

            cPass.InitParam(n, nParamIdx, pszName);
        }

        // For samplers.
        for (IZ_UINT n = 0; n < pDesc->numSampler; n++) {
            IZ_UINT nSmplIdx = pDesc->GetSmplIdx(n);

            const S_SHD_SAMPLER* pSmplDesc = m_SmplTbl.GetDesc(nSmplIdx);
            IZ_ASSERT(pSmplDesc != IZ_NULL);

            IZ_PCSTR pszName = m_StringBuffer.GetString(pSmplDesc->posName);
            IZ_ASSERT(pszName != IZ_NULL);

            VGOTO(
                cPass.InitSampler(
                    m_SmplTbl,
                    pDevice, n, nSmplIdx, pszName),
                __EXIT__);
        }
    }

__EXIT__:
    FREE(m_Allocator, pProgramBuf);
    return pBuffer;
}

// シェーダの名前を設定
IZ_BOOL CShaderBasic::SetName(IZ_PCSTR name)
{
    VRETURN(strlen(name) < SHD_NAME_LEN_MAX);

    m_Header.name.SetString(name);
    m_Header.nameKey = m_Header.name.GetKeyValue();

    return IZ_TRUE;
}

IZ_UINT CShaderBasic::Begin(
    IZ_UINT nTechIdx,
    IZ_BOOL bIsSaveState)
{
    IZ_ASSERT(m_pDevice != IZ_NULL);
    IZ_ASSERT(nTechIdx < m_TechTbl.GetTechNum());

    if (bIsSaveState) {
        VRETURN_VAL(m_pDevice->SaveRenderState(), 0);
        m_bIsSavedRS = bIsSaveState;
    }

    m_nCurTech = nTechIdx;

    const S_SHD_TECHNIQUE* pTechDesc = m_TechTbl.GetDesc(nTechIdx);
    VRETURN_VAL(pTechDesc != IZ_NULL, 0);

    IZ_UINT ret = pTechDesc->numPass;
    return ret;
}

IZ_BOOL CShaderBasic::End()
{
    IZ_ASSERT(m_pDevice != IZ_NULL);

    if (m_bIsSavedRS) {
        VRETURN(m_pDevice->LoadRenderState());
        m_bIsSavedRS = IZ_FALSE;
    }

    m_nCurTech = -1;

    return IZ_TRUE;
}

// **************************************************
namespace {
    enum _E_SHADER_PARAM_TYPE {
        _E_SHADER_PARAM_TYPE_PARAM = 1 << 0,
        _E_SHADER_PARAM_TYPE_TEX   = 1 << 1,
        _E_SHADER_PARAM_TYPE_SMPL  = 1 << 2,
    };
}   // namespace

#define _IZ_SHADER_HANDLE_IDX_MASK      0x0000ffff
#define _IZ_SHADER_HANDLE_TYPE_MASK     0xffff0000
#define _IZ_SHADER_HANDLE_TYPE_SHIFT    16

#define _IZ_SHADER_HANDLE(n, f)         ((((f) << _IZ_SHADER_HANDLE_TYPE_SHIFT) & _IZ_SHADER_HANDLE_TYPE_MASK) | ((n) & _IZ_SHADER_HANDLE_IDX_MASK))
#define _IZ_SHADER_HANDLE_GET_IDX(h)    ((h) & _IZ_SHADER_HANDLE_IDX_MASK)
#define _IZ_SHADER_HANDLE_GET_TYPE(h)   (_E_SHADER_PARAM_TYPE)(((h) & _IZ_SHADER_HANDLE_TYPE_MASK) >> _IZ_SHADER_HANDLE_TYPE_SHIFT)

#define _IZ_SHADER_HANDLE_PARAM(n)  _IZ_SHADER_HANDLE(n, _E_SHADER_PARAM_TYPE_PARAM)
#define _IZ_SHADER_HANDLE_TEX(n)    _IZ_SHADER_HANDLE(n, _E_SHADER_PARAM_TYPE_TEX)
#define _IZ_SHADER_HANDLE_SMPL(n)   _IZ_SHADER_HANDLE(n, _E_SHADER_PARAM_TYPE_SMPL)

#define _IZ_IS_SHADER_HANDLE_PARAM(h)   (_IZ_SHADER_HANDLE_GET_TYPE(h) == _E_SHADER_PARAM_TYPE_PARAM)
#define _IZ_IS_SHADER_HANDLE_TEX(h)     (_IZ_SHADER_HANDLE_GET_TYPE(h) == _E_SHADER_PARAM_TYPE_TEX)
#define _IZ_IS_SHADER_HANDLE_SMPL(h)    (_IZ_SHADER_HANDLE_GET_TYPE(h) == _E_SHADER_PARAM_TYPE_SMPL)
// **************************************************

IZ_BOOL CShaderBasic::BeginPass(IZ_UINT nPassIdx)
{
    IZ_ASSERT(m_nCurTech >= 0);

    const S_SHD_TECHNIQUE* pTechDesc = m_TechTbl.GetDesc(m_nCurTech);
    VRETURN(pTechDesc != IZ_NULL);

    VRETURN(nPassIdx < pTechDesc->numPass);

    m_nCurPass = nPassIdx;

    return IZ_TRUE;
}

IZ_BOOL CShaderBasic::EndPass()
{
    m_nCurPass = -1;
    return IZ_TRUE;
}

IZ_BOOL CShaderBasic::CommitChanges()
{
    IZ_ASSERT(m_nCurPass >= 0);

    // テクニックとパスの指定からパス全体のインデックスを計算する
    IZ_UINT passIdx = 0;

    for (IZ_INT16 i = 0; i < m_nCurTech; i++) {
        const S_SHD_TECHNIQUE* techDesc = m_TechTbl.GetDesc(i);
        IZ_ASSERT(techDesc != IZ_NULL);

        passIdx += techDesc->numPass;
    }

    IZ_ASSERT(passIdx + m_nCurPass < m_PassTbl.GetPassNum());

    CShaderPass& cPass = m_pPass[passIdx + m_nCurPass];
    const S_SHD_PASS* pDesc = cPass.GetDesc();

    // Set RenderStates.
    {
        m_pDevice->SetRenderState(graph::E_GRAPH_RS_ALPHABLENDENABLE, pDesc->state.AlphaBlendEnable);
        m_pDevice->SetRenderState(graph::E_GRAPH_RS_BLENDMETHOD,      pDesc->state.AlphaBlendMethod);
        m_pDevice->SetRenderState(graph::E_GRAPH_RS_ALPHATESTENABLE,  pDesc->state.AlphaTestEnable);
        m_pDevice->SetRenderState(graph::E_GRAPH_RS_ALPHAREF,         pDesc->state.AlphaTestRef);
        m_pDevice->SetRenderState(graph::E_GRAPH_RS_ALPHAFUNC,        pDesc->state.AlphaTestFunc);

        m_pDevice->SetRenderState(graph::E_GRAPH_RS_ZENABLE,          pDesc->state.ZEnable);
        m_pDevice->SetRenderState(graph::E_GRAPH_RS_ZWRITEENABLE,     pDesc->state.ZWriteEnable);
        m_pDevice->SetRenderState(graph::E_GRAPH_RS_ZFUNC,            pDesc->state.ZFunc);
    }

    // Set Shader.
    graph::CShaderProgram* shader = cPass.GetShaderProgram();
    m_pDevice->SetShaderProgram(shader);

    IZ_UINT nBytes = 0;

    // Set parameters.
    for (IZ_UINT i = 0; i < pDesc->numConst; i++) {
        VRETURN(SetParamValue(i, cPass, shader));
    }

    return IZ_TRUE;
}

/**
* Return number of techniques.
*/
IZ_UINT CShaderBasic::GetTechNum() const
{
    return m_TechTbl.GetTechNum();
}

/**
* Return number of attributes.
*/
IZ_UINT CShaderBasic::GetAttrNum() const
{
    return m_Header.numAttr;
}

/**
* Return specified attribute's name.
*/
IZ_PCSTR CShaderBasic::GetAttrName(IZ_UINT idx) const
{
    IZ_ASSERT(idx < m_Header.numAttr);

    const S_SHD_ATTRIBUTE* pAttr = m_AttrTbl.GetAttrByIdx(idx);

    IZ_PCSTR pszName = m_StringBuffer.GetString(pAttr->posName);
    return pszName;
}

/**
* Compare specified attribute's value.
*/
IZ_BOOL CShaderBasic::CmpAttr(const CShaderAttr& attr) const
{
    E_SHADER_PARAMETER_TYPE type = attr.GetType();
    IZ_PCSTR name = attr.GetName();

    switch (type) {
    case E_SHADER_PARAMETER_TYPE_INT:
        return CmpAttrValue(name, attr.GetValueAsUInt());
    case E_SHADER_PARAMETER_TYPE_FLOAT:
        return CmpAttrValue(name, attr.GetValueAsFloat());
    case E_SHADER_PARAMETER_TYPE_BOOL:
        return CmpAttrValue(name, attr.GetValueAsBool());
    }

    IZ_ASSERT(IZ_FALSE);
    return IZ_FALSE;
}

/**
* Compare specified attribute's value.
*/
IZ_BOOL CShaderBasic::CmpAttrValue(IZ_PCSTR attrName, IZ_UINT val) const
{
    IZ_BOOL ret = CmpAttrValue(
                    attrName, 
                    E_SHADER_PARAMETER_TYPE_INT,
                    val);
    return ret;
}

/**
* Compare specified attribute's value.
*/
IZ_BOOL CShaderBasic::CmpAttrValue(IZ_PCSTR attrName, IZ_FLOAT val) const
{
    IZ_BOOL ret = CmpAttrValue(
                    attrName, 
                    E_SHADER_PARAMETER_TYPE_FLOAT,
                    val);
    return ret;
}

/**
* Compare specified attribute's value.
*/
IZ_BOOL CShaderBasic::CmpAttrValue(IZ_PCSTR attrName, IZ_BOOL val) const
{
    IZ_BOOL ret = CmpAttrValue(
                    attrName, 
                    E_SHADER_PARAMETER_TYPE_BOOL,
                    val);
    return ret;
}

template <typename _T>
IZ_BOOL CShaderBasic::CmpAttrValue(
    IZ_PCSTR attrName, 
    E_SHADER_PARAMETER_TYPE type,
    _T val) const
{
    IZ_UINT nKey = CKey::GenerateValue(attrName);
    IZ_ASSERT(nKey > 0);

    // Ummm...
    const S_SHD_ATTRIBUTE* pAttr = const_cast<CShaderAttrTable&>(m_AttrTbl).GetAttrByKey(nKey);

    if (pAttr == IZ_NULL) {
        return IZ_FALSE;
    }

    VRETURN(pAttr->type == type);

    _T nAttrVal = *(_T*)(pAttr->param);
    return (nAttrVal == val);
}

IZ_BOOL CShaderBasic::SetParamValue(
    IZ_UINT idx,
    CShaderPass& cPass,
    graph::CShaderProgram* pShd)
{
    const CShaderPass::SParamInfo* pParamInfo = cPass.GetParamInfo(idx);

    // TODO
    S_SHD_PARAMETER* pParamDesc = const_cast<S_SHD_PARAMETER*>(m_ParamTbl.GetDesc(idx));

    // For Debug.
#if 0
    IZ_PCSTR name = m_StringBuffer.GetString(pParamDesc->posName);

    SHADER_PARAM_HANDLE handle = pShd->GetHandleByName(name);
    IZ_ASSERT(handle == (bIsVS ? pParamInfo->handleVS : pParamInfo->handlePS));
#endif

    // パラメータに変化があったときだけ
    IZ_BOOL bIsDirty = pParamDesc->isDirty;
    
    if (bIsDirty) {
        const SHADER_PARAM_HANDLE& handle = pParamInfo->handle;

        if (handle != 0) {
            // 有効なハンドル
            IZ_UINT nBytes = 0;

            const void* pParam = m_ParamTbl.GetParam(pParamInfo->idx, &nBytes);
            IZ_ASSERT(pParam != IZ_NULL);

            VRETURN(
                pShd->SetValue(
                    m_pDevice,
                    handle,
                    pParam,
                    nBytes));

             pParamDesc->isDirty = IZ_FALSE;
        }
    }

    return IZ_TRUE;
}

IZ_SHADER_HANDLE CShaderBasic::GetParameterByName(IZ_PCSTR pszName)
{
    IZ_SHADER_HANDLE ret = 0;

    CKey cKey(pszName);

    // Parameter
    IZ_INT idx = m_ParamTbl.GetIdxByNameKey(cKey);

    if (idx >= 0) {
        ret = _IZ_SHADER_HANDLE_PARAM(idx);
    }
    else {
        // Sampler
        idx = m_SmplTbl.GetIdxByNameKey(cKey);
        if (idx >= 0) {
            ret = _IZ_SHADER_HANDLE_SMPL(idx);
        }
        else {
            // Texture
            idx = m_TexTbl.GetIdxByNameKey(cKey);
            if (idx >= 0) {
                ret = _IZ_SHADER_HANDLE_TEX(idx);
            }
            else {
                return 0;
            }
        }
    }

    return ret;
}

IZ_SHADER_HANDLE CShaderBasic::GetParameterBySemantic(IZ_PCSTR pszSemantic)
{
    // Only parameters have semantic...

    IZ_INT idx = m_ParamTbl.GetIdxBySemantic(pszSemantic);
    VRETURN_VAL(idx >= 0, 0);

    IZ_SHADER_HANDLE ret = _IZ_SHADER_HANDLE_PARAM(idx);
    return ret;
}

graph::CGraphicsDevice* CShaderBasic::GetDevice()
{
    return m_pDevice;
}

IZ_BOOL CShaderBasic::GetParameterDesc(
    IZ_SHADER_HANDLE handle, 
    SShaderParamDesc& sDesc) const
{
    IZ_ASSERT(handle > 0);

    IZ_UINT nIdx = _IZ_SHADER_HANDLE_GET_IDX(handle);
    _E_SHADER_PARAM_TYPE type = _IZ_SHADER_HANDLE_GET_TYPE(handle);

    switch (type) {
    case _E_SHADER_PARAM_TYPE_PARAM:
        {
            const S_SHD_PARAMETER* pParamDesc = m_ParamTbl.GetDesc(nIdx);
            sDesc.columns = pParamDesc->Columns;
            sDesc.rows = pParamDesc->Rows;
            sDesc.elements = pParamDesc->Elements;
            sDesc.bytes = pParamDesc->Bytes;
            sDesc.clazz = pParamDesc->Class;
            sDesc.type = pParamDesc->Type;
        }
        break;
    case _E_SHADER_PARAM_TYPE_TEX:
        sDesc.clazz = E_SHADER_PARAMETER_CLASS_OBJECT;
        sDesc.type = E_SHADER_PARAMETER_TYPE_TEXTURE;
        break;
    case _E_SHADER_PARAM_TYPE_SMPL:
        {
            const S_SHD_SAMPLER* pSmplDesc = m_SmplTbl.GetDesc(nIdx);
            sDesc.clazz = E_SHADER_PARAMETER_CLASS_OBJECT;
            sDesc.type = pSmplDesc->Type;
        }
        break;
    default:
        VRETURN(IZ_FALSE);
        break;
    }

    return IZ_TRUE;
}

IZ_BOOL CShaderBasic::SetParamValue(
    IZ_SHADER_HANDLE hParam,
    const void* pValue,
    IZ_UINT nBytes)
{
    VRETURN(_IZ_IS_SHADER_HANDLE_PARAM(hParam));

    IZ_UINT nIdx = _IZ_SHADER_HANDLE_GET_IDX(hParam);
    //VRETURN(nIdx < m_Header.numParam);

    VRETURN(m_ParamTbl.SetParam(nIdx, pValue, nBytes));

    return IZ_TRUE;
}

IZ_BOOL CShaderBasic::SetParamValue(
    IZ_PCSTR name,
    const void* value,
    IZ_UINT bytes)
{
    IZ_BOOL ret = IZ_FALSE;
    IZ_SHADER_HANDLE handle = GetParameterByName(name);
    
    if (handle > 0)
    {
        ret = SetParamValue(handle, value, bytes);
    }

    return ret;
}

// シェーダで利用するテクスチャを設定.
IZ_BOOL CShaderBasic::SetTexture(
    IZ_SHADER_HANDLE hTex,
    graph::CBaseTexture* pTex)
{
    VRETURN(_IZ_IS_SHADER_HANDLE_TEX(hTex));

    IZ_UINT nIdx = _IZ_SHADER_HANDLE_GET_IDX(hTex);
    VRETURN(nIdx < m_TexTbl.GetTexNum());

    VRETURN(m_TexTbl.SetTexture(nIdx, pTex));

    return IZ_TRUE;
}

// シェーダで利用するテクスチャを設定.
IZ_BOOL CShaderBasic::SetTexture(
    IZ_PCSTR name,
    graph::CBaseTexture* tex)
{
    IZ_BOOL ret = IZ_FALSE;
    IZ_SHADER_HANDLE handle = GetParameterByName(name);

    if (handle > 0)
    {
        ret = SetTexture(handle, tex);
    }

    return ret;
}

// シェーダで利用するテクスチャをサンプラに設定.
IZ_BOOL CShaderBasic::SetTextureToSampler(
    IZ_SHADER_HANDLE hSmpl,
    graph::CBaseTexture* pTex)
{
    VRETURN(_IZ_IS_SHADER_HANDLE_SMPL(hSmpl));

    IZ_UINT nIdx = _IZ_SHADER_HANDLE_GET_IDX(hSmpl);
    VRETURN(nIdx < m_Header.numSmpl);

    const S_SHD_SAMPLER* pSmplDesc = m_SmplTbl.GetDesc(nIdx);
    IZ_ASSERT(pSmplDesc != IZ_NULL);
    
    VRETURN(pSmplDesc->BindTexIdx >= 0);
    VRETURN((IZ_UINT)pSmplDesc->BindTexIdx < m_TexTbl.GetTexNum());

    VRETURN(
        m_TexTbl.SetTexture(
            pSmplDesc->BindTexIdx,
            pTex));

    return IZ_TRUE;
}

// シェーダで利用するテクスチャをサンプラに設定.
IZ_BOOL CShaderBasic::SetTextureToSampler(
    IZ_PCSTR name,
    graph::CBaseTexture* tex)
{
    IZ_BOOL ret = IZ_FALSE;
    IZ_SHADER_HANDLE handle = GetParameterByName(name);

    if (handle > 0)
    {
        ret = SetTextureToSampler(handle, tex);
    }

    return ret;
}
