#if !defined(__IZANAGI_SCENEGRAPH_VECTOR_CAMERA_H__)
#define __IZANAGI_SCENEGRAPH_VECTOR_CAMERA_H__

#include "izDefs.h"
#include "izMath.h"
#include "Camera.h"

namespace izanagi {
	/**
	 */
	class CVectorCamera : public CCamera {
	public:
		CVectorCamera();
		virtual ~CVectorCamera();

		NO_COPIABLE(CVectorCamera);

	public:
		/** 初期化.
		 */
		void Init(
			const SVector& pos,
			const SVector& at,
			IZ_FLOAT fNear, IZ_FLOAT fFar,
			IZ_FLOAT fov,
			IZ_FLOAT aspect);

		/** カメラ更新.
		 */
		virtual void Update();

		/** 移動.
		 */
		void Translate(const SVector& trans);

		/** X軸回転.
		 */
		void RotateByX(IZ_FLOAT rad);

		/** Y軸回転.
		 */
		void RotateByY(IZ_FLOAT rad);

		/** Z軸回転.
		 */
		void RotateByZ(IZ_FLOAT rad);

		/** 任意軸回転.
		 */
		void Rotate(
			const SVector& axis,
			IZ_FLOAT rad);

		/** 位置指定.
		 */
		void SetPos(const SVector& pos);

	public:
		/** カメラのX軸取得
		 */
		const SVector& GetX() const { return m_X; }

		/** カメラのY軸取得
		 */
		const SVector& GetY() const { return m_Y; }

		/** カメラのZ軸取得
		 */
		const SVector& GetZ() const { return m_Z; }

		/** カメラの位置取得
		 */
		const SVector& GetPos() const { return m_Pos; }

		/**
		 */
		const SMatrix& GetTransform() const { return m_Transform; }

	private:
		// 使わせない

		void Init(
			const SVector& vecPos,
			const SVector& vecRef,
			const SVector& vecUp,
			IZ_FLOAT fNear, IZ_FLOAT fFar,
			IZ_FLOAT fFov,
			IZ_FLOAT fAspect)
		{
		}

		void Init(const SCameraParam& sParam) {}

		void SetAt(const SVector& vecAt) {}

		void SetUp(const SVector& vecUp) {}

	private:
		SVector m_X;
		
		// NOTE
		// m_Param.up の参照とすることで m_Param.up と同期するようにする
		SVector& m_Y;

		SVector m_Z;

		// NOTE
		// m_Param.pos の参照とすることで m_Param.pos と同期するようにする
		SVector& m_Pos;

		// カメラの変換マトリクス
		SMatrix m_Transform;
	};

}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_VECTOR_CAMERA_H__)
