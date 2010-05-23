#if !defined(__URANUS_DEBUG_UTIL_MESH_H__)
#define __URANUS_DEBUG_UTIL_MESH_H__

#include "unStd.h"
#include "unGraph.h"
#include "unMath.h"

namespace uranus {
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
		virtual UN_BOOL Draw(UN_BOOL bEnableDrawDebugAxis = UN_FALSE);

		UN_UINT GetVtxFormFlag() const { return m_nVtxFormFlag; }

	protected:
		struct SMeshFace;

		struct SMeshVtx {
			SVector pos;
			SVector nml;
			SVector tangent;
			SVector binml;
			UN_COLOR clr;
			UN_FLOAT uv[2];

			static const UN_UINT IS_OVERLAP_IDX_FLAG = 0x80000000;
			static const UN_UINT OVERLAP_IDX_MASK = 0x7fffffff;

			// インデックスは違うが重なる頂点へのインデックス
			UN_UINT32 overlap;

			CStdList<SMeshFace> face_list;

			UN_BOOL AddFace(SMeshFace* p)
			{
				face_list.Init(UN_FALSE);
				return face_list.AddTail(p->GetListItem());
			}

			void SetOverlapIdx(UN_UINT idx)
			{
				UN_ASSERT((idx & IS_OVERLAP_IDX_FLAG) == 0);
				overlap = IS_OVERLAP_IDX_FLAG | idx;
			}
			UN_UINT GetOverlapIdx() const
			{
				return (overlap & OVERLAP_IDX_MASK);
			}
			UN_BOOL HasOverlapIdx() const { return ((overlap & IS_OVERLAP_IDX_FLAG) > 0); }
		};

		struct SMeshFace {
			enum {
				VTX_NUM = 3,
			};

			UN_UINT32 idx[VTX_NUM];
			CStdList<SMeshFace>::Item list_item;

			CStdList<SMeshFace>::Item* GetListItem()
			{
				list_item.Init(this);
				return &list_item;
			}
		};

	protected:
		// 頂点バッファ作成
		UN_BOOL CreateVB(
			UN_UINT flag,
			UN_UINT nVtxNum);

		// インデックスバッファ作成
		UN_BOOL CreateIB(
			UN_UINT nIdxNum,
			E_GRAPH_INDEX_BUFFER_FMT fmt);

		// 頂点宣言作成
		UN_BOOL CreateVD(UN_UINT flag);

		// データバッファ作成
		UN_BOOL CreateDataBuffer(
			UN_UINT nVtxNum,
			UN_UINT nIdxNum);

		// デバッグ用軸メッシュ作成
		UN_BOOL CreateDebugAxis(
			UN_UINT nVtxNum,
			UN_UINT flag);

		// データバッファクリア
		void ClearDataBuffer();

		// データをVB、IBにコピーする
		UN_BOOL CopyDataToBuffer(UN_UINT flag);

		UN_BOOL LockVB(void** p);
		UN_BOOL UnlockVB();

		UN_BOOL LockIB(void** p);
		UN_BOOL UnlockIB();

		// 頂点データセット
		UN_UINT8* SetVtxData(
			const SMeshFace& sFace,
			UN_UINT flag,
			UN_UINT8* pData);
		UN_UINT8* SetVtxData(
			const SMeshVtx& sVtx,
			UN_UINT flag,
			UN_UINT8* pData);

		// 頂点と面を関連付ける
		UN_BOOL BindFaceToVtx(SMeshFace* pFace);

		// 面ごとの計算
		void ComputeFace(
			SMeshFace* pFace,
			UN_UINT flag);

		void ComputeTangent(
			SMeshVtx* vtx0,
			const SMeshVtx& vtx1,
			const SMeshVtx& vtx2);

		void SetOverlapVtx(
			UN_UINT idx_0,
			UN_UINT idx_1);

		void ApplyOverlap(
			SMeshVtx* pVtx,
			UN_UINT flag);

	protected:
		inline UN_UINT GetVtxNum() const { return m_pVB->GetVtxNum(); }
		
		inline SMeshFace* GetFace(UN_UINT nIdx);
		inline SMeshVtx* GetVtx(UN_UINT nIdx);

		SMeshFace* GetFace() { return m_pFace; }
		SMeshVtx* GetVtx() { return m_pVtx; }

	private:
		typedef UN_UINT (*SetElemFunc)(UN_UINT, SVertexElement*, UN_UINT, UN_WORD*);
		static SetElemFunc SetElemFuncTbl[];

		typedef UN_UINT8* (*SetVtxFunc)(UN_UINT, const SMeshVtx&, UN_UINT8*);
		static SetVtxFunc SetVtxFuncTbl[];

		typedef UN_UINT (*GetElemSizeFunc)(UN_UINT);
		static GetElemSizeFunc GetElemSizeFuncTbl[];

		static UN_UINT SetVtxElementPos(UN_UINT flag, SVertexElement* pElem, UN_UINT nPos, UN_WORD* pOffset);
		static UN_UINT SetVtxElementNormal(UN_UINT flag, SVertexElement* pElem, UN_UINT nPos, UN_WORD* pOffset);
		static UN_UINT SetVtxElementColor(UN_UINT flag, SVertexElement* pElem, UN_UINT nPos, UN_WORD* pOffset);
		static UN_UINT SetVtxElementUV(UN_UINT flag, SVertexElement* pElem, UN_UINT nPos, UN_WORD* pOffset);
		static UN_UINT SetVtxElementTangent(UN_UINT flag, SVertexElement* pElem, UN_UINT nPos, UN_WORD* pOffset);

		static UN_UINT8* SetVtxPos(UN_UINT flag, const SMeshVtx& sVtx, UN_UINT8* pVtx);
		static UN_UINT8* SetVtxNormal(UN_UINT flag, const SMeshVtx& sVtx, UN_UINT8* pVtx);
		static UN_UINT8* SetVtxColor(UN_UINT flag, const SMeshVtx& sVtx, UN_UINT8* pVtx);
		static UN_UINT8* SetVtxUV(UN_UINT flag, const SMeshVtx& sVtx, UN_UINT8* pVtx);
		static UN_UINT8* SetVtxTangent(UN_UINT flag, const SMeshVtx& sVtx, UN_UINT8* pVtx);

	protected:
		template <typename _T>
		static _T* CreateMesh(
			void* pBuf,
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT nVtxFormFlag)
		{
			UN_ASSERT(pBuf != UN_NULL);

			_T* ret = new(pBuf)_T;

			ret->AddRef();
			ret->m_pAllocator = pAllocator;
			SAFE_REPLACE(ret->m_pDevice, pDevice);

			ret->m_nVtxFormFlag = nVtxFormFlag;

			return ret;
		}

		static UN_BOOL IsPos(UN_UINT flag)     { return (flag & E_DEBUG_MESH_VTX_FORM_POS) > 0; }
		static UN_BOOL IsColor(UN_UINT flag)   { return (flag & E_DEBUG_MESH_VTX_FORM_COLOR) > 0; }
		static UN_BOOL IsNormal(UN_UINT flag)  { return (flag & E_DEBUG_MESH_VTX_FORM_NORMAL) > 0; }
		static UN_BOOL IsUV(UN_UINT flag)      { return (flag & E_DEBUG_MESH_VTX_FORM_UV) > 0; }
		static UN_BOOL IsTangent(UN_UINT flag)
		{
			// 法線が有効のときに接ベクトルも有効にする
			UN_BOOL b0 = ((flag & E_DEBUG_MESH_VTX_FORM_TANGENT) > 0);
			UN_BOOL b1 = IsNormal(flag);
			return b0 && b1;
		}

		static UN_BOOL IsAxisX(UN_UINT flag) { return (flag & E_DEBUG_MESH_AXIS_X) > 0; }
		static UN_BOOL IsAxisY(UN_UINT flag) { return (flag & E_DEBUG_MESH_AXIS_Y) > 0; }
		static UN_BOOL IsAxisZ(UN_UINT flag) { return (flag & E_DEBUG_MESH_AXIS_Z) > 0; }

		static UN_UINT GetPosSize(UN_UINT flag)     { return (IsPos(flag)     ? sizeof(UN_FLOAT) * 4 : 0); }
		static UN_UINT GetColorSize(UN_UINT flag)   { return (IsColor(flag)   ? sizeof(UN_COLOR)     : 0); }
		static UN_UINT GetNormalSize(UN_UINT flag)  { return (IsNormal(flag)  ? sizeof(UN_FLOAT) * 3 : 0); }
		static UN_UINT GetUVSize(UN_UINT flag)      { return (IsUV(flag)      ? sizeof(UN_FLOAT) * 2 : 0); }
		static UN_UINT GetTangentSize(UN_UINT flag) { return (IsTangent(flag) ? sizeof(UN_FLOAT) * 3 : 0); }

		// For Debug.
		static UN_UINT GetAxisFlag(UN_UINT flag)
		{
			UN_UINT ret = 0;

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
		UN_UINT m_nPrimCnt;

		SMeshVtx* m_pVtx;
		SMeshFace* m_pFace;

		UN_UINT m_nVtxFormFlag;

		// For Debug
		CDebugMeshAxis* m_pDebugAxis;
	};

	// inline *****************************************

	// 解放
	void CDebugMesh::InternalRelease()
	{
		delete this;

		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	CDebugMesh::SMeshFace* CDebugMesh::GetFace(UN_UINT nIdx)
	{
		UN_ASSERT(nIdx < m_nPrimCnt);
		UN_ASSERT(m_pFace != UN_NULL);
		return &m_pFace[nIdx];
	}

	CDebugMesh::SMeshVtx* CDebugMesh::GetVtx(UN_UINT nIdx)
	{
		UN_ASSERT(nIdx < GetVtxNum());
		UN_ASSERT(m_pFace != UN_NULL);
		return &m_pVtx[nIdx];
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_H__)
