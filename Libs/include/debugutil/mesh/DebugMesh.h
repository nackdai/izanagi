#if !defined(__IZANAGI_DEBUG_UTIL_MESH_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_H__

#include "izStd.h"
#include "izGraph.h"
#include "izMath.h"

namespace izanagi {
	enum E_DEBUG_MESH_VTX_FORM {
		// Do not use below....
		E_DEBUG_MESH_VTX_FORM_POS_SHIFT = 0,
		E_DEBUG_MESH_VTX_FORM_COLOR_SHIFT,
		E_DEBUG_MESH_VTX_FORM_NORMAL_SHIFT,
		E_DEBUG_MESH_VTX_FORM_UV_SHIFT,
		E_DEBUG_MESH_VTX_FORM_TANGENT_SHIFT,

		E_DEBUG_MESH_VTX_FORM_NUM,
		// Do not use above....

		E_DEBUG_MESH_VTX_FORM_POS     = 1 << E_DEBUG_MESH_VTX_FORM_POS_SHIFT,		// 頂点位置
		E_DEBUG_MESH_VTX_FORM_COLOR   = 1 << E_DEBUG_MESH_VTX_FORM_COLOR_SHIFT,		// 頂点カラー
		E_DEBUG_MESH_VTX_FORM_NORMAL  = 1 << E_DEBUG_MESH_VTX_FORM_NORMAL_SHIFT,	// 法線
		E_DEBUG_MESH_VTX_FORM_UV      = 1 << E_DEBUG_MESH_VTX_FORM_UV_SHIFT,		// UV座標
		E_DEBUG_MESH_VTX_FORM_TANGENT = 1 << E_DEBUG_MESH_VTX_FORM_TANGENT_SHIFT,	// 接ベクトル

		// For Axis
		E_DEBUG_MESH_AXIS_X = 1 << 28,
		E_DEBUG_MESH_AXIS_Y = 1 << 29,
		E_DEBUG_MESH_AXIS_Z = 1 << 30,
		E_DEBUG_MESH_AXIS_XYZ = E_DEBUG_MESH_AXIS_X | E_DEBUG_MESH_AXIS_Y | E_DEBUG_MESH_AXIS_Z,
	};

	class CDebugMeshAxis;

	/**
	* デバッグ用形状
	*/
	class CDebugMesh : public CObject {
	protected:
		CDebugMesh();
		virtual ~CDebugMesh();

		NO_COPIABLE(CDebugMesh);

	protected:
		// 解放
		inline void InternalRelease();

	public:
		// 描画
		virtual IZ_BOOL Draw(IZ_BOOL bEnableDrawDebugAxis = IZ_FALSE);

		IZ_UINT GetVtxFormFlag() const { return m_nVtxFormFlag; }

	protected:
		struct SMeshFace;

		struct SMeshVtx {
			SVector pos;
			SVector nml;
			SVector tangent;
			SVector binml;
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

	protected:
		// 頂点バッファ作成
		IZ_BOOL CreateVB(
			IZ_UINT flag,
			IZ_UINT nVtxNum);

		// インデックスバッファ作成
		IZ_BOOL CreateIB(
			IZ_UINT nIdxNum,
			E_GRAPH_INDEX_BUFFER_FMT fmt);

		// 頂点宣言作成
		IZ_BOOL CreateVD(IZ_UINT flag);

		// データバッファ作成
		IZ_BOOL CreateDataBuffer(
			IZ_UINT nVtxNum,
			IZ_UINT nIdxNum);

		// デバッグ用軸メッシュ作成
		IZ_BOOL CreateDebugAxis(
			IZ_UINT nVtxNum,
			IZ_UINT flag);

		// データバッファクリア
		void ClearDataBuffer();

		// データをVB、IBにコピーする
		IZ_BOOL CopyDataToBuffer(IZ_UINT flag);

		IZ_BOOL LockVB(void** p);
		IZ_BOOL UnlockVB();

		IZ_BOOL LockIB(void** p);
		IZ_BOOL UnlockIB();

		// 頂点データセット
		IZ_UINT8* SetVtxData(
			const SMeshFace& sFace,
			IZ_UINT flag,
			IZ_UINT8* pData);
		IZ_UINT8* SetVtxData(
			const SMeshVtx& sVtx,
			IZ_UINT flag,
			IZ_UINT8* pData);

		// 頂点と面を関連付ける
		IZ_BOOL BindFaceToVtx(SMeshFace* pFace);

		// 面ごとの計算
		void ComputeFace(
			SMeshFace* pFace,
			IZ_UINT flag);

		void ComputeTangent(
			SMeshVtx* vtx0,
			const SMeshVtx& vtx1,
			const SMeshVtx& vtx2);

		void SetOverlapVtx(
			IZ_UINT idx_0,
			IZ_UINT idx_1);

		void ApplyOverlap(
			SMeshVtx* pVtx,
			IZ_UINT flag);

	protected:
		inline IZ_UINT GetVtxNum() const { return m_pVB->GetVtxNum(); }
		
		inline SMeshFace* GetFace(IZ_UINT nIdx);
		inline SMeshVtx* GetVtx(IZ_UINT nIdx);

		SMeshFace* GetFace() { return m_pFace; }
		SMeshVtx* GetVtx() { return m_pVtx; }

	private:
		typedef IZ_UINT (*SetElemFunc)(IZ_UINT, SVertexElement*, IZ_UINT, IZ_WORD*);
		static SetElemFunc SetElemFuncTbl[];

		typedef IZ_UINT8* (*SetVtxFunc)(IZ_UINT, const SMeshVtx&, IZ_UINT8*);
		static SetVtxFunc SetVtxFuncTbl[];

		typedef IZ_UINT (*GetElemSizeFunc)(IZ_UINT);
		static GetElemSizeFunc GetElemSizeFuncTbl[];

		static IZ_UINT SetVtxElementPos(IZ_UINT flag, SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset);
		static IZ_UINT SetVtxElementNormal(IZ_UINT flag, SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset);
		static IZ_UINT SetVtxElementColor(IZ_UINT flag, SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset);
		static IZ_UINT SetVtxElementUV(IZ_UINT flag, SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset);
		static IZ_UINT SetVtxElementTangent(IZ_UINT flag, SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset);

		static IZ_UINT8* SetVtxPos(IZ_UINT flag, const SMeshVtx& sVtx, IZ_UINT8* pVtx);
		static IZ_UINT8* SetVtxNormal(IZ_UINT flag, const SMeshVtx& sVtx, IZ_UINT8* pVtx);
		static IZ_UINT8* SetVtxColor(IZ_UINT flag, const SMeshVtx& sVtx, IZ_UINT8* pVtx);
		static IZ_UINT8* SetVtxUV(IZ_UINT flag, const SMeshVtx& sVtx, IZ_UINT8* pVtx);
		static IZ_UINT8* SetVtxTangent(IZ_UINT flag, const SMeshVtx& sVtx, IZ_UINT8* pVtx);

	protected:
		template <typename _T>
		static _T* CreateMesh(
			void* pBuf,
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IZ_UINT nVtxFormFlag)
		{
			IZ_ASSERT(pBuf != IZ_NULL);

			_T* ret = new(pBuf)_T;

			ret->AddRef();
			ret->m_pAllocator = pAllocator;
			SAFE_REPLACE(ret->m_pDevice, pDevice);

			ret->m_nVtxFormFlag = nVtxFormFlag;

			return ret;
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

	protected:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		CVertexBuffer* m_pVB;
		CIndexBuffer* m_pIB;
		CVertexDeclaration* m_pVD;

		E_GRAPH_PRIM_TYPE m_PrimType;
		IZ_UINT m_nPrimCnt;

		SMeshVtx* m_pVtx;
		SMeshFace* m_pFace;

		IZ_UINT m_nVtxFormFlag;

		// For Debug
		CDebugMeshAxis* m_pDebugAxis;
	};

	// inline *****************************************

	// 解放
	void CDebugMesh::InternalRelease()
	{
		delete this;

		if (m_pAllocator != IZ_NULL) {
			m_pAllocator->Free(this);
		}
	}

	CDebugMesh::SMeshFace* CDebugMesh::GetFace(IZ_UINT nIdx)
	{
		IZ_ASSERT(nIdx < m_nPrimCnt);
		IZ_ASSERT(m_pFace != IZ_NULL);
		return &m_pFace[nIdx];
	}

	CDebugMesh::SMeshVtx* CDebugMesh::GetVtx(IZ_UINT nIdx)
	{
		IZ_ASSERT(nIdx < GetVtxNum());
		IZ_ASSERT(m_pFace != IZ_NULL);
		return &m_pVtx[nIdx];
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUG_UTIL_MESH_H__)
