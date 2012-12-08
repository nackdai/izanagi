#if !defined(__IZANAGI_SYSTEM_MOUSE_H__)
#define __IZANAGI_SYSTEM_MOUSE_H__

#include "izStd.h"
#include "InputDeviceDefs.h"

namespace izanagi
{
namespace sys
{
	/** マウス
	 */
	class CMouse : public CObject {
	public:
		// インスタンス作成
		static CMouse* CreateMouse(
            IMemoryAllocator* allocator,
            SInputDeviceInitParam* initParam);

	protected:
		CMouse();
		virtual ~CMouse();

		NO_COPIABLE(CMouse);

        IZ_DEFINE_INTERNAL_RELEASE();

	public:
		// 更新
		PURE_VIRTUAL(IZ_BOOL Update());

		PURE_VIRTUAL(IZ_BOOL IsClick(IZ_UINT idx) const);

		PURE_VIRTUAL(IZ_BOOL IsClickL() const);	// 右
		PURE_VIRTUAL(IZ_BOOL IsClickR() const);	// 左
		PURE_VIRTUAL(IZ_BOOL IsClickM() const);	// 中
        
		PURE_VIRTUAL(IZ_DWORD GetX() const);
		PURE_VIRTUAL(IZ_DWORD GetY() const);
		PURE_VIRTUAL(IZ_DWORD GetZ() const);

        PURE_VIRTUAL(const void* GetRawState() const);

	protected:
		IMemoryAllocator* m_Allocator;

		// 更新に成功したかどうか
		IZ_BOOL m_bSucceedUpdate;
	};
}   // namespace sys
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_MOUSE_H__)
