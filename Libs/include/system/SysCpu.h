#if !defined(__URANUS_SYSTEM_SYS_CPU_H__)
#define __URANUS_SYSTEM_SYS_CPU_H__

#include "unDefs.h"

namespace uranus {
	/**
	*/
	class CSysCpu {
	private:
		CSysCpu();
		~CSysCpu();

		NO_COPIABLE(CSysCpu);

	public:
		// Return CPU num.
		static UN_UINT GetCpuNum();
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_SYSTEM_SYS_CPU_H__)
