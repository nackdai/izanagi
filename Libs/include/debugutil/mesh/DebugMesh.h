#if !defined(__IZANAGI_DEBUG_UTIL_MESH_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_H__

#include "izStd.h"
#include "izGraph.h"
#include "izMath.h"

namespace izanagi {
    /**
     */
    enum E_DEBUG_MESH_VTX_FORM {
        // Do not use below....
        E_DEBUG_MESH_VTX_FORM_POS_SHIFT = 0,
        E_DEBUG_MESH_VTX_FORM_COLOR_SHIFT,
        E_DEBUG_MESH_VTX_FORM_NORMAL_SHIFT,
        E_DEBUG_MESH_VTX_FORM_UV_SHIFT,
        E_DEBUG_MESH_VTX_FORM_TANGENT_SHIFT,

        E_DEBUG_MESH_VTX_FORM_NUM,
        // Do not use above....

        E_DEBUG_MESH_VTX_FORM_POS     = 1 << E_DEBUG_MESH_VTX_FORM_POS_SHIFT,       // 頂点位置
        E_DEBUG_MESH_VTX_FORM_COLOR   = 1 << E_DEBUG_MESH_VTX_FORM_COLOR_SHIFT,     // 頂点カラー
        E_DEBUG_MESH_VTX_FORM_NORMAL  = 1 << E_DEBUG_MESH_VTX_FORM_NORMAL_SHIFT,    // 法線
        E_DEBUG_MESH_VTX_FORM_UV      = 1 << E_DEBUG_MESH_VTX_FORM_UV_SHIFT,        // UV座標
        E_DEBUG_MESH_VTX_FORM_TANGENT = 1 << E_DEBUG_MESH_VTX_FORM_TANGENT_SHIFT,   // 接ベクトル

        // For Axis
        E_DEBUG_MESH_AXIS_X = 1 << 28,
        E_DEBUG_MESH_AXIS_Y = 1 << 29,
        E_DEBUG_MESH_AXIS_Z = 1 << 30,
        E_DEBUG_MESH_AXIS_XYZ = E_DEBUG_MESH_AXIS_X | E_DEBUG_MESH_AXIS_Y | E_DEBUG_MESH_AXIS_Z,
    };

    /**
     */
    struct SMeshFace {
        enum {
            VTX_NUM = 3,
        };

        IZ_UINT32 idx[VTX_NUM];
        CStdList<SMeshFace>::Item list_item;

        CStdList<SMeshFace>::Item* GetListItem()
        {
            list_item.Init(this);
            return &list_item;
        }
    };

    /**
     */
    struct SMeshVtx {
        math::SVector4 pos;
        math::SVector4 nml;
        math::SVector4 tangent;
        math::SVector4 binml;
        IZ_COLOR clr;
        IZ_FLOAT uv[2];

        static const IZ_UINT IS_OVERLAP_IDX_FLAG = 0x80000000;
        static const IZ_UINT OVERLAP_IDX_MASK = 0x7fffffff;

        // インデックスは違うが重なる頂点へのインデックス
        IZ_UINT32 overlap;

        CStdList<SMeshFace> face_list;

        IZ_BOOL AddFace(SMeshFace* p)
        {
            face_list.Init(IZ_FALSE);
            return face_list.AddTail(p->GetListItem());
        }

        void SetOverlapIdx(IZ_UINT idx)
        {
            IZ_ASSERT((idx & IS_OVERLAP_IDX_FLAG) == 0);
            overlap = IS_OVERLAP_IDX_FLAG | idx;
        }
        IZ_UINT GetOverlapIdx() const
        {
            return (overlap & OVERLAP_IDX_MASK);
        }
        IZ_BOOL HasOverlapIdx() const { return ((overlap & IS_OVERLAP_IDX_FLAG) > 0); }
    };

    class CDebugMeshAxis;

    /**
     */
    class CDebugMeshUtil {
    private:
        CDebugMeshUtil();
        ~CDebugMeshUtil();

    public:
        typedef IZ_UINT (*SetElemFunc)(IZ_UINT, graph::SVertexElement*, IZ_UINT, IZ_WORD*);
        static SetElemFunc SetElemFuncTbl[E_DEBUG_MESH_VTX_FORM_NUM];

        typedef IZ_UINT (*GetElemSizeFunc)(IZ_UINT);
        static GetElemSizeFunc GetElemSizeFuncTbl[E_DEBUG_MESH_VTX_FORM_NUM];

        static IZ_UINT SetVtxElementPos(IZ_UINT flag, graph::SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset);
        static IZ_UINT SetVtxElementNormal(IZ_UINT flag, graph::SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset);
        static IZ_UINT SetVtxElementColor(IZ_UINT flag, graph::SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset);
        static IZ_UINT SetVtxElementUV(IZ_UINT flag, graph::SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset);
        static IZ_UINT SetVtxElementTangent(IZ_UINT flag, graph::SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset);

        template <class _VTX>
        static IZ_UINT8* SetVtx(IZ_UINT flag, const _VTX& sVtx, IZ_UINT8* pVtx)
        {
            pVtx = SetVtxPos(flag, sVtx, pVtx);
            pVtx = SetVtxNormal(flag, sVtx, pVtx);
            pVtx = SetVtxColor(flag, sVtx, pVtx);
            pVtx = SetVtxUV(flag, sVtx, pVtx);
            pVtx = SetVtxTangent(flag, sVtx, pVtx);
            return pVtx;
        }

        template <class _VTX>
        static IZ_UINT8* SetVtxPos(IZ_UINT flag, const _VTX& sVtx, IZ_UINT8* pVtx)
        {
            if (IsPos(flag)) {
                IZ_FLOAT* pos = (IZ_FLOAT*)pVtx;
                pos[0] = sVtx.pos.v[0];
                pos[1] = sVtx.pos.v[1];
                pos[2] = sVtx.pos.v[2];
                pos[3] = 1.0f;

                pVtx += GetPosSize(flag);
            }
            return pVtx;
        }

        template <class _VTX>
        static IZ_UINT8* SetVtxNormal(IZ_UINT flag, const _VTX& sVtx, IZ_UINT8* pVtx)
        {
            if (IsNormal(flag)) {
                IZ_FLOAT* nml = (IZ_FLOAT*)pVtx;
                nml[0] = sVtx.nml.v[0];
                nml[1] = sVtx.nml.v[1];
                nml[2] = sVtx.nml.v[2];

                pVtx += GetNormalSize(flag);
            }
            return pVtx;
        }

        template <class _VTX>
        static IZ_UINT8* SetVtxColor(IZ_UINT flag, const _VTX& sVtx, IZ_UINT8* pVtx)
        {
            if (IsColor(flag)) {
                *(IZ_COLOR*)pVtx = sVtx.clr;
                pVtx += GetColorSize(flag);
            }
            return pVtx;
        }

        template <class _VTX>
        static IZ_UINT8* SetVtxUV(IZ_UINT flag, const _VTX& sVtx, IZ_UINT8* pVtx)
        {
            if (IsUV(flag)) {
                IZ_FLOAT* uv = (IZ_FLOAT*)pVtx;
                uv[0] = sVtx.uv[0];
                uv[1] = sVtx.uv[1];

                pVtx += GetUVSize(flag);
            }
            return pVtx;
        }

        template <class _VTX>
        static IZ_UINT8* SetVtxTangent(IZ_UINT flag, const _VTX& sVtx, IZ_UINT8* pVtx)
        {
            if (IsTangent(flag)) {
                IZ_FLOAT* tangent = (IZ_FLOAT*)pVtx;
                tangent[0] = sVtx.tangent.v[0];
                tangent[1] = sVtx.tangent.v[1];
                tangent[2] = sVtx.tangent.v[2];
                tangent[3] = sVtx.tangent.v[3];

                pVtx += GetTangentSize(flag);
            }
            return pVtx;
        }

        static IZ_BOOL IsPos(IZ_UINT flag)     { return (flag & E_DEBUG_MESH_VTX_FORM_POS) > 0; }
        static IZ_BOOL IsColor(IZ_UINT flag)   { return (flag & E_DEBUG_MESH_VTX_FORM_COLOR) > 0; }
        static IZ_BOOL IsNormal(IZ_UINT flag)  { return (flag & E_DEBUG_MESH_VTX_FORM_NORMAL) > 0; }
        static IZ_BOOL IsUV(IZ_UINT flag)      { return (flag & E_DEBUG_MESH_VTX_FORM_UV) > 0; }
        static IZ_BOOL IsTangent(IZ_UINT flag)
        {
            // 法線が有効のときに接ベクトルも有効にする
            IZ_BOOL b0 = ((flag & E_DEBUG_MESH_VTX_FORM_TANGENT) > 0);
            IZ_BOOL b1 = IsNormal(flag);
            return b0 && b1;
        }

        static IZ_BOOL IsAxisX(IZ_UINT flag) { return (flag & E_DEBUG_MESH_AXIS_X) > 0; }
        static IZ_BOOL IsAxisY(IZ_UINT flag) { return (flag & E_DEBUG_MESH_AXIS_Y) > 0; }
        static IZ_BOOL IsAxisZ(IZ_UINT flag) { return (flag & E_DEBUG_MESH_AXIS_Z) > 0; }

        static IZ_UINT GetPosSize(IZ_UINT flag)     { return (IsPos(flag)     ? sizeof(IZ_FLOAT) * 4 : 0); }
        static IZ_UINT GetColorSize(IZ_UINT flag)   { return (IsColor(flag)   ? sizeof(IZ_COLOR)     : 0); }
        static IZ_UINT GetNormalSize(IZ_UINT flag)  { return (IsNormal(flag)  ? sizeof(IZ_FLOAT) * 3 : 0); }
        static IZ_UINT GetUVSize(IZ_UINT flag)      { return (IsUV(flag)      ? sizeof(IZ_FLOAT) * 2 : 0); }
        static IZ_UINT GetTangentSize(IZ_UINT flag) { return (IsTangent(flag) ? sizeof(IZ_FLOAT) * 4 : 0); }

        // For Debug.
        static IZ_UINT GetAxisFlag(IZ_UINT flag)
        {
            IZ_UINT ret = 0;

            if (IsNormal(flag)) {
                ret |= E_DEBUG_MESH_AXIS_Z;
            }
            if (IsTangent(flag)) {
                ret |= E_DEBUG_MESH_AXIS_X | E_DEBUG_MESH_AXIS_Y;
            }

            return ret;
        }
    };

    /** デバッグ用形状
    */
    template<class _VTX>
    class CDebugMeshTmpl : public CObject {
    protected:
        CDebugMeshTmpl()
        {
            m_Allocator = IZ_NULL;
    
            m_pVB = IZ_NULL;
            m_pIB = IZ_NULL;
            m_pVD = IZ_NULL;

            m_PrimType = graph::E_GRAPH_PRIM_TYPE_TRIANGLESTRIP;
            m_nPrimCnt = 0;

            m_pVtx = IZ_NULL;
            m_pFace = IZ_NULL;

            m_nVtxFormFlag = 0;
        }

        virtual ~CDebugMeshTmpl()
        {
            SAFE_RELEASE(m_pVB);
            SAFE_RELEASE(m_pIB);
            SAFE_RELEASE(m_pVD);

            ClearDataBuffer();
        }

        NO_COPIABLE(CDebugMeshTmpl);

    protected:
        // 解放
        void InternalRelease()
        {
            delete this;

            if (m_Allocator != IZ_NULL) {
                m_Allocator->Free(this);
            }
        }

    public:
        // 描画
        virtual IZ_BOOL Draw(
            graph::CGraphicsDevice* device,
            IZ_BOOL bEnableDrawDebugAxis = IZ_FALSE)
        {
            IZ_ASSERT(device != IZ_NULL);
            IZ_ASSERT(m_pVB != IZ_NULL);
            IZ_ASSERT(m_pVD != IZ_NULL);

            // 頂点バッファ
            device->SetVertexBuffer(
                0,
                0,
                m_pVB->GetStride(),
                m_pVB);

            // 頂点宣言
            device->SetVertexDeclaration(m_pVD);

            // インデックスバッファ
            if (m_pIB != IZ_NULL) {
                device->SetIndexBuffer(m_pIB);
            }

            // 描画
            if (m_pIB != IZ_NULL) {
                device->DrawIndexedPrimitive(
                    m_PrimType,
                    0,
                    m_pVB->GetVtxNum(),
                    0,
                    m_nPrimCnt);
            }
            else {
                device->DrawPrimitive(
                    m_PrimType,
                    0,
                    m_nPrimCnt);
            }

            if (bEnableDrawDebugAxis) {
                DrawDebugAxis(device);
            }

            return IZ_TRUE;
        }

        IZ_UINT GetVtxFormFlag() const { return m_nVtxFormFlag; }

        graph::CVertexBuffer* GetVB() { return m_pVB; }
        graph::CVertexDeclaration* GetVD() { return m_pVD; }
        graph::CIndexBuffer* GetIB() { return m_pIB; }

        IZ_UINT GetPrimitiveCount() { return m_nPrimCnt; }
        graph::E_GRAPH_PRIM_TYPE GetPrimitiveType() { return m_PrimType; }

    protected:
        virtual void DrawDebugAxis(graph::CGraphicsDevice* device) {}

        // 頂点バッファ作成
        IZ_BOOL CreateVB(
            graph::CGraphicsDevice* device,
            IZ_UINT flag,
            IZ_UINT nVtxNum)
        {
            IZ_UINT nStride = ComputeVtxStride(flag);

            m_pVB = device->CreateVertexBuffer(
                        nStride,
                        nVtxNum,
                        graph::E_GRAPH_RSC_USAGE_STATIC);

            return (m_pVB != IZ_NULL);
        }

        IZ_UINT ComputeVtxStride(IZ_UINT flag)
        {
            IZ_C_ASSERT(COUNTOF(CDebugMeshUtil::GetElemSizeFuncTbl) == E_DEBUG_MESH_VTX_FORM_NUM);

            IZ_UINT nStride = 0;
            for (IZ_UINT i = 0; i < COUNTOF(CDebugMeshUtil::GetElemSizeFuncTbl); ++i) {
                nStride += (*CDebugMeshUtil::GetElemSizeFuncTbl[i])(flag);
            }

            return nStride;
        }

        // インデックスバッファ作成
        IZ_BOOL CreateIB(
            graph::CGraphicsDevice* device,
            IZ_UINT nIdxNum,
            graph::E_GRAPH_INDEX_BUFFER_FMT fmt)
        {
            m_pIB = device->CreateIndexBuffer(
                        nIdxNum,
                        fmt,
                        graph::E_GRAPH_RSC_USAGE_STATIC);

            return (m_pIB != IZ_NULL);
        }


        // 頂点宣言作成
        IZ_BOOL CreateVD(
            graph::CGraphicsDevice* device,
            IZ_UINT flag)
        {
            // +1はENDの分
            graph::SVertexElement VtxElement[E_DEBUG_MESH_VTX_FORM_NUM + 1];
            FILL_ZERO(VtxElement, sizeof(VtxElement));

            IZ_UINT pos = SetVtxElement(VtxElement, flag);

            m_pVD = device->CreateVertexDeclaration(VtxElement, pos);

            return (m_pVD != IZ_NULL);
        }

        IZ_UINT SetVtxElement(
            graph::SVertexElement* elements,
            IZ_UINT flag)
        {
            IZ_C_ASSERT(COUNTOF(CDebugMeshUtil::SetElemFuncTbl) == E_DEBUG_MESH_VTX_FORM_NUM);

            IZ_WORD nOffset = 0;
            IZ_UINT pos = 0;

            for (IZ_UINT i = 0; i < COUNTOF(CDebugMeshUtil::SetElemFuncTbl); ++i) {
                pos = (*CDebugMeshUtil::SetElemFuncTbl[i])(flag, elements, pos, &nOffset);
            }

            return pos;
        }

        // データバッファ作成
        IZ_BOOL CreateDataBuffer(
            IZ_UINT nVtxNum,
            IZ_UINT nIdxNum)
        {
            IZ_ASSERT(m_nPrimCnt > 0);

            m_pVtx = (_VTX*)ALLOC_ZERO(m_Allocator, sizeof(_VTX) * nVtxNum);
            VRETURN_NULL(m_pVtx);

            m_pFace = (SMeshFace*)ALLOC_ZERO(m_Allocator, sizeof(SMeshFace) * m_nPrimCnt);
            VRETURN_NULL(m_pFace);

            return IZ_TRUE;
        }

        // デバッグ用軸メッシュ作成
        virtual IZ_BOOL CreateDebugAxis(
            graph::CGraphicsDevice* device,
            IZ_UINT nVtxNum,
            IZ_UINT flag)
        {
            return IZ_TRUE;
        }

        // データバッファクリア
        void ClearDataBuffer()
        {
            FREE(m_Allocator, m_pVtx);
            FREE(m_Allocator, m_pFace);
        }

        // データをVB、IBにコピーする
        IZ_BOOL CopyDataToBuffer(graph::CGraphicsDevice* device, IZ_UINT flag)
        {
            IZ_ASSERT(m_pVtx != IZ_NULL);
            IZ_ASSERT(m_pFace != IZ_NULL);

            IZ_BOOL ret = IZ_TRUE;

            IZ_UINT8* pVtxData = IZ_NULL;
            IZ_UINT32* pIdxData = IZ_NULL;

            // インデックス
            VGOTO(ret = LockIB(device, (void**)&pIdxData), __EXIT__);
            {
                for (IZ_UINT i = 0; i < m_nPrimCnt; ++i) {
#ifdef IZ_COORD_LEFT_HAND
                    *pIdxData++ = m_pFace[i].idx[0];
                    *pIdxData++ = m_pFace[i].idx[2];
                    *pIdxData++ = m_pFace[i].idx[1];
#else
                    *pIdxData++ = m_pFace[i].idx[0];
                    *pIdxData++ = m_pFace[i].idx[1];
                    *pIdxData++ = m_pFace[i].idx[2];
#endif

                    // 面ごとの計算
                    ComputeFace(&m_pFace[i], flag);
                }
            }
            VGOTO(ret = UnlockIB(device), __EXIT__);

            // 頂点
            VGOTO(ret = LockVB(device, (void**)&pVtxData), __EXIT__);
            {
                IZ_UINT nVtxNum = GetVtxNum();

                for (IZ_UINT i = 0; i < nVtxNum; ++i) {
                    ApplyOverlap(
                        &m_pVtx[i],
                        flag);

                    pVtxData = SetVtxData(
                                m_pVtx[i],
                                flag,
                                pVtxData);
                }
            }
            VGOTO(ret = UnlockVB(device), __EXIT__);

        __EXIT__:
            // もういらない
            ClearDataBuffer();

            return ret;
        }

        IZ_BOOL LockVB(graph::CGraphicsDevice* device, void** p)
        {
            IZ_ASSERT(p != IZ_NULL);

            VRETURN(m_pVB->Lock(device, 0, 0, p, IZ_FALSE));
            VRETURN(BeginDebugAxisRegister(device));

            return IZ_TRUE;
        }

        IZ_BOOL UnlockVB(graph::CGraphicsDevice* device)
        {
            VRETURN(m_pVB->Unlock(device));
            VRETURN(EndDebugAxisRegister(device));
            
            return IZ_TRUE;
        }

        virtual IZ_BOOL BeginDebugAxisRegister(graph::CGraphicsDevice* device) { return IZ_TRUE; }
        virtual IZ_BOOL EndDebugAxisRegister(graph::CGraphicsDevice* device) { return IZ_TRUE; }

        IZ_BOOL LockIB(graph::CGraphicsDevice* device, void** p)
        {
            IZ_ASSERT(p != IZ_NULL);

            VRETURN(m_pIB->Lock(device, 0, 0, p, IZ_FALSE));
            return IZ_TRUE;
        }

        IZ_BOOL UnlockIB(graph::CGraphicsDevice* device)
        {
            return m_pIB->Unlock(device);
        }

        // 頂点データセット
        IZ_UINT8* SetVtxData(
            const SMeshFace& sFace,
            IZ_UINT flag,
            IZ_UINT8* pData)
        {
            for (IZ_UINT i = 0; i < SMeshFace::VTX_NUM; ++i) {
                _VTX* pVtx = GetVtx(sFace.idx[i]);

                pData = SetVtxData(
                            *pVtx,
                            flag,
                            pData);
            }

            return pData;
        }

        virtual IZ_UINT8* SetExtraVtxData(
            const void* vtx,
            IZ_UINT flag,
            IZ_UINT8* data)
        {
            return data;
        }

        IZ_UINT8* SetVtxData(
            const _VTX& sVtx,
            IZ_UINT flag,
            IZ_UINT8* pData)
        {
            pData = CDebugMeshUtil::SetVtx(flag, sVtx, pData);
            pData = SetExtraVtxData(&sVtx, flag, pData);

            SetDebugAxisVtxData(&sVtx, flag);
    
            return pData;
        }

        virtual void SetDebugAxisVtxData(
            const void* vtx,
            IZ_UINT flag)
        {
        }

        // 頂点と面を関連付ける
        IZ_BOOL BindFaceToVtx(SMeshFace* pFace)
        {
            for (IZ_UINT i = 0; i < SMeshFace::VTX_NUM; ++i) {
                _VTX* pVtx = GetVtx(pFace->idx[i]);
                VRETURN_NULL(pVtx);

                VRETURN(pVtx->AddFace(pFace));
            }

            return IZ_TRUE;
        }


        // 面ごとの計算
        void ComputeFace(
            SMeshFace* pFace,
            IZ_UINT flag)
        {
            if (CDebugMeshUtil::IsTangent(flag)) {
#ifdef IZ_COORD_LEFT_HAND
                _VTX* vtx0 = GetVtx(pFace->idx[0]);
                _VTX* vtx1 = GetVtx(pFace->idx[2]);
                _VTX* vtx2 = GetVtx(pFace->idx[1]);
#else
                _VTX* vtx0 = GetVtx(pFace->idx[0]);
                _VTX* vtx1 = GetVtx(pFace->idx[1]);
                _VTX* vtx2 = GetVtx(pFace->idx[2]);
#endif

                // Tangent
                ComputeTangent(vtx0, *vtx1, *vtx2);
                ComputeTangent(vtx1, *vtx2, *vtx0);
                ComputeTangent(vtx2, *vtx0, *vtx1);
            }
        }


        void ComputeTangent(
            _VTX* vtx0,
            const _VTX& vtx1,
            const _VTX& vtx2)
        {
            // NOTE
            // DirectXのuv座標系で考えると
            //
            // +---> u(x)
            // |
            // v(y)
            //
            // 手前がzとなるので、これは左手座標となる

            math::SVector4 vP;
            math::SVector4::Sub(vP, vtx1.pos, vtx0->pos);

            math::SVector4 vQ;
            math::SVector4::Sub(vQ, vtx2.pos, vtx0->pos);
    
            IZ_FLOAT s1 = vtx1.uv[0] - vtx0->uv[0];
            IZ_FLOAT t1 = vtx1.uv[1] - vtx0->uv[1];

            IZ_FLOAT s2 = vtx2.uv[0] - vtx0->uv[0];
            IZ_FLOAT t2 = vtx2.uv[1] - vtx0->uv[1];

            // 行列式の逆数
            IZ_FLOAT fInvDeterminant = 1.0f / (s1 * t2 - s2 * t1);

            // BiNormal
            math::SVector4 vB;
            {
                vB.x = fInvDeterminant * (-s2 * vP.x + s1 * vQ.x);
                vB.y = fInvDeterminant * (-s2 * vP.y + s1 * vQ.y);
                vB.z = fInvDeterminant * (-s2 * vP.z + s1 * vQ.z);
                math::SVector4::Normalize(vB, vB);
            }

            // Tangent
            math::SVector4 vT;
            {
#if 0
                vT.x = fInvDeterminant * (t2 * vP.x - t1 * vQ.x);
                vT.y = fInvDeterminant * (t2 * vP.y - t1 * vQ.y);
                vT.z = fInvDeterminant * (t2 * vP.z - t1 * vQ.z);
                math::SVector4::Normalize(vT, vT);
#else
    #ifdef IZ_COORD_LEFT_HAND
                // X = Y x Z
                izanagi::math::SVector4::Cross(vT, vB, vtx0->nml);

                // Y = Z x X
                izanagi::math::SVector4::Cross(vB, vtx0->nml, vT);
    #else
                // NOTE
                // ここでは、Tangent空間を左手座標にしたい
                // 左手座標にするために、あえて反対に計算する

                // X = Y x Z
                izanagi::math::SVector4::Cross(vT, vtx0->nml, vB);

                // Y = Z x X
                izanagi::math::SVector4::Cross(vB, vT, vtx0->nml);
    #endif
#endif
            }

            math::SMatrix44 mtx;
            mtx.v[0].Set(vT.x, vB.x, vtx0->nml.x, 0.0f);
            mtx.v[1].Set(vT.y, vB.y, vtx0->nml.y, 0.0f);
            mtx.v[2].Set(vT.z, vB.z, vtx0->nml.z, 0.0f);
            mtx.v[3].Set(0.0f, 0.0f,        0.0f, 1.0f);

            IZ_FLOAT determinant = math::SMatrix44::Determinant(mtx);

            vT.w = determinant;

            //math::SVector4::Scale(vB, vB, determinant);

            math::SVector4::Copy(vtx0->tangent, vT);
            math::SVector4::Copy(vtx0->binml, vB);
        }


        void SetOverlapVtx(
            IZ_UINT idx_0,
            IZ_UINT idx_1)
        {
            _VTX* pVtx_0 = GetVtx(idx_0);
            _VTX* pVtx_1 = GetVtx(idx_1);

            pVtx_0->SetOverlapIdx(idx_1);
            pVtx_1->SetOverlapIdx(idx_0);
        }

        void ApplyOverlap(
            SMeshVtx* pVtx,
            IZ_UINT flag)
        {
#if 0
            if (pVtx->HasOverlapIdx()) {
                IZ_UINT idx = pVtx->GetOverlapIdx();
                _VTX* pOverlap = GetVtx(idx);

                if (IsTangent(flag)) {
                    // Tangent
                    math::SVector4::Add(pVtx->tangent, pVtx->tangent, pOverlap->tangent);
                    math::SVector4::Scale(pVtx->tangent, pVtx->tangent, 0.5f);
                    math::SVector4::Normalize(pVtx->tangent, pVtx->tangent);

                    math::SVector4::Copy(pOverlap->tangent, pVtx->tangent);

                    // BiNormal
                    math::SVector4::Add(pVtx->binml, pVtx->binml, pOverlap->binml);
                    math::SVector4::Scale(pVtx->binml, pVtx->binml, 0.5f);
                    math::SVector4::Normalize(pVtx->binml, pVtx->binml);

                    math::SVector4::Copy(pOverlap->binml, pVtx->binml);
                }
            }
#endif
        }


    protected:
        IZ_UINT GetVtxNum() const { return m_pVB->GetVtxNum(); }
        
        SMeshFace* GetFace(IZ_UINT nIdx)
        {
            IZ_ASSERT(nIdx < m_nPrimCnt);
            IZ_ASSERT(m_pFace != IZ_NULL);
            return &m_pFace[nIdx];
        }

        _VTX* GetVtx(IZ_UINT nIdx)
        {
            IZ_ASSERT(nIdx < GetVtxNum());
            IZ_ASSERT(m_pFace != IZ_NULL);
            return &m_pVtx[nIdx];
        }

        SMeshFace* GetFace() { return m_pFace; }
        _VTX* GetVtx() { return m_pVtx; }        

    protected:
        template <typename _T>
        static _T* CreateMesh(
            void* pBuf,
            IMemoryAllocator* pAllocator,
            graph::CGraphicsDevice* pDevice,
            IZ_UINT nVtxFormFlag)
        {
            IZ_ASSERT(pBuf != IZ_NULL);

            _T* ret = new(pBuf)_T;

            ret->AddRef();
            ret->m_Allocator = pAllocator;

            ret->m_nVtxFormFlag = nVtxFormFlag;

            return ret;
        }

    protected:
        IMemoryAllocator* m_Allocator;

        graph::CVertexBuffer* m_pVB;
        graph::CIndexBuffer* m_pIB;
        graph::CVertexDeclaration* m_pVD;

        graph::E_GRAPH_PRIM_TYPE m_PrimType;
        IZ_UINT m_nPrimCnt;

        _VTX* m_pVtx;
        SMeshFace* m_pFace;

        IZ_UINT m_nVtxFormFlag;
    };

    class CDebugMesh : public CDebugMeshTmpl<SMeshVtx> {
    public:
        CDebugMesh();
        virtual ~CDebugMesh();

    protected:
        // デバッグ用軸メッシュ作成
        virtual IZ_BOOL CreateDebugAxis(
            graph::CGraphicsDevice* device,
            IZ_UINT nVtxNum,
            IZ_UINT flag);

        virtual void DrawDebugAxis(graph::CGraphicsDevice* device);

        virtual IZ_BOOL BeginDebugAxisRegister(graph::CGraphicsDevice* device);
        virtual IZ_BOOL EndDebugAxisRegister(graph::CGraphicsDevice* device);

        virtual void SetDebugAxisVtxData(
            const void* vtx,
            IZ_UINT flag);

    private:
        // For Debug
        CDebugMeshAxis* m_pDebugAxis;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_DEBUG_UTIL_MESH_H__)
