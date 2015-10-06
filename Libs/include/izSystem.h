#if !defined(__IZANAGI_SYSTEM_H__)
#define __IZANAGI_SYSTEM_H__

#include "system/SysEnvironment.h"
#include "system/SysTimer.h"
#include "system/SysUtil.h"

// Thread
#include "system/SysSemaphore.h"
#include "system/SysEvent.h"
#include "system/SysSpinLock.h"
#include "system/SysThread.h"
#include "system/SysMutex.h"

// Device
#include "system/Pad.h"

#include "system/SysKeyDefs.h"
#include "system/SysWindow.h"

// IO
#include "system/io/IODefs.h"

#include "system/io/InputStream.h"
#include "system/io/FileInputStream.h"
#include "system/io/MemoryInputStream.h"
#include "system/io/HeapInputStream.h"

#include "system/io/OutputStream.h"
#include "system/io/FileOutputStream.h"
#include "system/io/MemoryOutputStream.h"

#endif  // #if !defined(__IZANAGI_SYSTEM_H__)
