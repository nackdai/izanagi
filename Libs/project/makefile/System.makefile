# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLES2_Include_Path=-I"include" -I"../../../External/pthreads/pthreads.2" -I"../../include" 
Release_GLES2_Include_Path=-I"include" -I"../../../External/pthreads/pthreads.2" -I"../../include" 

# Library paths...
Debug_GLES2_Library_Path=
Release_GLES2_Library_Path=

# Additional libraries...
Debug_GLES2_Libraries=
Release_GLES2_Libraries=

# Preprocessor definitions...
Debug_GLES2_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _LIB -D __IZ_DEBUG__ -D __IZ_GLES2__ 
Release_GLES2_Preprocessor_Definitions=-D GCC_BUILD -D NDEBUG -D _LIB -D __IZ_GLES2__ 

# Implictly linked object files...
Debug_GLES2_Implicitly_Linked_Objects=
Release_GLES2_Implicitly_Linked_Objects=

# Compiler flags...
Debug_GLES2_Compiler_Flags=-O0 
Release_GLES2_Compiler_Flags=-O2 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug_GLES2 Release_GLES2 

# Builds the Debug_GLES2 configuration...
.PHONY: Debug_GLES2
Debug_GLES2: create_folders gccDebug_GLES2/../../source/system/Pad.o gccDebug_GLES2/../../source/system/win32/SysEnvironment_win32 .o gccDebug_GLES2/../../source/system/win32/SysTimer_win32.o gccDebug_GLES2/../../source/system/win32/SysUtil_win32.o gccDebug_GLES2/../../source/system/win32/SysWindow_win32.o gccDebug_GLES2/../../source/system/win32/thread/SysEvent_win32.o gccDebug_GLES2/../../source/system/win32/thread/SysMutex_win32.o gccDebug_GLES2/../../source/system/win32/thread/SysSemaphore_win32.o gccDebug_GLES2/../../source/system/win32/thread/SysThread_win32.o gccDebug_GLES2/../../source/system/win32/device/DeviceUtil.o gccDebug_GLES2/../../source/system/win32/device/Pad_DInput.o gccDebug_GLES2/../../source/system/win32/device/Pad_XInput.o gccDebug_GLES2/../../source/system/linux/thread/SysEvent_pthread.o gccDebug_GLES2/../../source/system/linux/thread/SysMutex_pthread.o gccDebug_GLES2/../../source/system/linux/thread/SysSemaphore_pthread.o gccDebug_GLES2/../../source/system/linux/thread/SysThread_pthread.o 
	ar rcs ../lib/gccDebug_GLES2/libSystem.a gccDebug_GLES2/../../source/system/Pad.o gccDebug_GLES2/../../source/system/win32/SysEnvironment_win32 .o gccDebug_GLES2/../../source/system/win32/SysTimer_win32.o gccDebug_GLES2/../../source/system/win32/SysUtil_win32.o gccDebug_GLES2/../../source/system/win32/SysWindow_win32.o gccDebug_GLES2/../../source/system/win32/thread/SysEvent_win32.o gccDebug_GLES2/../../source/system/win32/thread/SysMutex_win32.o gccDebug_GLES2/../../source/system/win32/thread/SysSemaphore_win32.o gccDebug_GLES2/../../source/system/win32/thread/SysThread_win32.o gccDebug_GLES2/../../source/system/win32/device/DeviceUtil.o gccDebug_GLES2/../../source/system/win32/device/Pad_DInput.o gccDebug_GLES2/../../source/system/win32/device/Pad_XInput.o gccDebug_GLES2/../../source/system/linux/thread/SysEvent_pthread.o gccDebug_GLES2/../../source/system/linux/thread/SysMutex_pthread.o gccDebug_GLES2/../../source/system/linux/thread/SysSemaphore_pthread.o gccDebug_GLES2/../../source/system/linux/thread/SysThread_pthread.o  $(Debug_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/system/Pad.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/Pad.d
gccDebug_GLES2/../../source/system/Pad.o: ../../source/system/Pad.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/Pad.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/Pad.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/Pad.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/Pad.d

# Compiles file ../../source/system/win32/SysEnvironment_win32 .cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/win32/SysEnvironment_win32 .d
gccDebug_GLES2/../../source/system/win32/SysEnvironment_win32 .o: ../../source/system/win32/SysEnvironment_win32 .cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/win32/SysEnvironment_win32 .cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/win32/SysEnvironment_win32 .o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/win32/SysEnvironment_win32 .cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/win32/SysEnvironment_win32 .d

# Compiles file ../../source/system/win32/SysTimer_win32.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/win32/SysTimer_win32.d
gccDebug_GLES2/../../source/system/win32/SysTimer_win32.o: ../../source/system/win32/SysTimer_win32.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/win32/SysTimer_win32.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/win32/SysTimer_win32.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/win32/SysTimer_win32.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/win32/SysTimer_win32.d

# Compiles file ../../source/system/win32/SysUtil_win32.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/win32/SysUtil_win32.d
gccDebug_GLES2/../../source/system/win32/SysUtil_win32.o: ../../source/system/win32/SysUtil_win32.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/win32/SysUtil_win32.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/win32/SysUtil_win32.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/win32/SysUtil_win32.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/win32/SysUtil_win32.d

# Compiles file ../../source/system/win32/SysWindow_win32.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/win32/SysWindow_win32.d
gccDebug_GLES2/../../source/system/win32/SysWindow_win32.o: ../../source/system/win32/SysWindow_win32.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/win32/SysWindow_win32.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/win32/SysWindow_win32.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/win32/SysWindow_win32.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/win32/SysWindow_win32.d

# Compiles file ../../source/system/win32/thread/SysEvent_win32.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/win32/thread/SysEvent_win32.d
gccDebug_GLES2/../../source/system/win32/thread/SysEvent_win32.o: ../../source/system/win32/thread/SysEvent_win32.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/win32/thread/SysEvent_win32.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/win32/thread/SysEvent_win32.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/win32/thread/SysEvent_win32.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/win32/thread/SysEvent_win32.d

# Compiles file ../../source/system/win32/thread/SysMutex_win32.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/win32/thread/SysMutex_win32.d
gccDebug_GLES2/../../source/system/win32/thread/SysMutex_win32.o: ../../source/system/win32/thread/SysMutex_win32.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/win32/thread/SysMutex_win32.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/win32/thread/SysMutex_win32.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/win32/thread/SysMutex_win32.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/win32/thread/SysMutex_win32.d

# Compiles file ../../source/system/win32/thread/SysSemaphore_win32.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/win32/thread/SysSemaphore_win32.d
gccDebug_GLES2/../../source/system/win32/thread/SysSemaphore_win32.o: ../../source/system/win32/thread/SysSemaphore_win32.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/win32/thread/SysSemaphore_win32.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/win32/thread/SysSemaphore_win32.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/win32/thread/SysSemaphore_win32.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/win32/thread/SysSemaphore_win32.d

# Compiles file ../../source/system/win32/thread/SysThread_win32.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/win32/thread/SysThread_win32.d
gccDebug_GLES2/../../source/system/win32/thread/SysThread_win32.o: ../../source/system/win32/thread/SysThread_win32.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/win32/thread/SysThread_win32.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/win32/thread/SysThread_win32.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/win32/thread/SysThread_win32.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/win32/thread/SysThread_win32.d

# Compiles file ../../source/system/win32/device/DeviceUtil.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/win32/device/DeviceUtil.d
gccDebug_GLES2/../../source/system/win32/device/DeviceUtil.o: ../../source/system/win32/device/DeviceUtil.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/win32/device/DeviceUtil.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/win32/device/DeviceUtil.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/win32/device/DeviceUtil.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/win32/device/DeviceUtil.d

# Compiles file ../../source/system/win32/device/Pad_DInput.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/win32/device/Pad_DInput.d
gccDebug_GLES2/../../source/system/win32/device/Pad_DInput.o: ../../source/system/win32/device/Pad_DInput.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/win32/device/Pad_DInput.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/win32/device/Pad_DInput.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/win32/device/Pad_DInput.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/win32/device/Pad_DInput.d

# Compiles file ../../source/system/win32/device/Pad_XInput.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/win32/device/Pad_XInput.d
gccDebug_GLES2/../../source/system/win32/device/Pad_XInput.o: ../../source/system/win32/device/Pad_XInput.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/win32/device/Pad_XInput.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/win32/device/Pad_XInput.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/win32/device/Pad_XInput.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/win32/device/Pad_XInput.d

# Compiles file ../../source/system/linux/thread/SysEvent_pthread.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/linux/thread/SysEvent_pthread.d
gccDebug_GLES2/../../source/system/linux/thread/SysEvent_pthread.o: ../../source/system/linux/thread/SysEvent_pthread.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysEvent_pthread.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/linux/thread/SysEvent_pthread.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysEvent_pthread.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/linux/thread/SysEvent_pthread.d

# Compiles file ../../source/system/linux/thread/SysMutex_pthread.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/linux/thread/SysMutex_pthread.d
gccDebug_GLES2/../../source/system/linux/thread/SysMutex_pthread.o: ../../source/system/linux/thread/SysMutex_pthread.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysMutex_pthread.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/linux/thread/SysMutex_pthread.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysMutex_pthread.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/linux/thread/SysMutex_pthread.d

# Compiles file ../../source/system/linux/thread/SysSemaphore_pthread.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/linux/thread/SysSemaphore_pthread.d
gccDebug_GLES2/../../source/system/linux/thread/SysSemaphore_pthread.o: ../../source/system/linux/thread/SysSemaphore_pthread.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysSemaphore_pthread.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/linux/thread/SysSemaphore_pthread.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysSemaphore_pthread.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/linux/thread/SysSemaphore_pthread.d

# Compiles file ../../source/system/linux/thread/SysThread_pthread.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/system/linux/thread/SysThread_pthread.d
gccDebug_GLES2/../../source/system/linux/thread/SysThread_pthread.o: ../../source/system/linux/thread/SysThread_pthread.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysThread_pthread.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/system/linux/thread/SysThread_pthread.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysThread_pthread.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/system/linux/thread/SysThread_pthread.d

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders gccRelease_GLES2/../../source/system/Pad.o gccRelease_GLES2/../../source/system/win32/SysEnvironment_win32 .o gccRelease_GLES2/../../source/system/win32/SysTimer_win32.o gccRelease_GLES2/../../source/system/win32/SysUtil_win32.o gccRelease_GLES2/../../source/system/win32/SysWindow_win32.o gccRelease_GLES2/../../source/system/win32/thread/SysEvent_win32.o gccRelease_GLES2/../../source/system/win32/thread/SysMutex_win32.o gccRelease_GLES2/../../source/system/win32/thread/SysSemaphore_win32.o gccRelease_GLES2/../../source/system/win32/thread/SysThread_win32.o gccRelease_GLES2/../../source/system/win32/device/DeviceUtil.o gccRelease_GLES2/../../source/system/win32/device/Pad_DInput.o gccRelease_GLES2/../../source/system/win32/device/Pad_XInput.o gccRelease_GLES2/../../source/system/linux/thread/SysEvent_pthread.o gccRelease_GLES2/../../source/system/linux/thread/SysMutex_pthread.o gccRelease_GLES2/../../source/system/linux/thread/SysSemaphore_pthread.o gccRelease_GLES2/../../source/system/linux/thread/SysThread_pthread.o 
	ar rcs ../lib/gccRelease_GLES2/libSystem.a gccRelease_GLES2/../../source/system/Pad.o gccRelease_GLES2/../../source/system/win32/SysEnvironment_win32 .o gccRelease_GLES2/../../source/system/win32/SysTimer_win32.o gccRelease_GLES2/../../source/system/win32/SysUtil_win32.o gccRelease_GLES2/../../source/system/win32/SysWindow_win32.o gccRelease_GLES2/../../source/system/win32/thread/SysEvent_win32.o gccRelease_GLES2/../../source/system/win32/thread/SysMutex_win32.o gccRelease_GLES2/../../source/system/win32/thread/SysSemaphore_win32.o gccRelease_GLES2/../../source/system/win32/thread/SysThread_win32.o gccRelease_GLES2/../../source/system/win32/device/DeviceUtil.o gccRelease_GLES2/../../source/system/win32/device/Pad_DInput.o gccRelease_GLES2/../../source/system/win32/device/Pad_XInput.o gccRelease_GLES2/../../source/system/linux/thread/SysEvent_pthread.o gccRelease_GLES2/../../source/system/linux/thread/SysMutex_pthread.o gccRelease_GLES2/../../source/system/linux/thread/SysSemaphore_pthread.o gccRelease_GLES2/../../source/system/linux/thread/SysThread_pthread.o  $(Release_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/system/Pad.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/Pad.d
gccRelease_GLES2/../../source/system/Pad.o: ../../source/system/Pad.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/Pad.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/Pad.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/Pad.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/Pad.d

# Compiles file ../../source/system/win32/SysEnvironment_win32 .cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/win32/SysEnvironment_win32 .d
gccRelease_GLES2/../../source/system/win32/SysEnvironment_win32 .o: ../../source/system/win32/SysEnvironment_win32 .cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/win32/SysEnvironment_win32 .cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/win32/SysEnvironment_win32 .o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/win32/SysEnvironment_win32 .cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/win32/SysEnvironment_win32 .d

# Compiles file ../../source/system/win32/SysTimer_win32.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/win32/SysTimer_win32.d
gccRelease_GLES2/../../source/system/win32/SysTimer_win32.o: ../../source/system/win32/SysTimer_win32.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/win32/SysTimer_win32.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/win32/SysTimer_win32.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/win32/SysTimer_win32.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/win32/SysTimer_win32.d

# Compiles file ../../source/system/win32/SysUtil_win32.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/win32/SysUtil_win32.d
gccRelease_GLES2/../../source/system/win32/SysUtil_win32.o: ../../source/system/win32/SysUtil_win32.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/win32/SysUtil_win32.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/win32/SysUtil_win32.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/win32/SysUtil_win32.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/win32/SysUtil_win32.d

# Compiles file ../../source/system/win32/SysWindow_win32.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/win32/SysWindow_win32.d
gccRelease_GLES2/../../source/system/win32/SysWindow_win32.o: ../../source/system/win32/SysWindow_win32.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/win32/SysWindow_win32.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/win32/SysWindow_win32.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/win32/SysWindow_win32.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/win32/SysWindow_win32.d

# Compiles file ../../source/system/win32/thread/SysEvent_win32.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/win32/thread/SysEvent_win32.d
gccRelease_GLES2/../../source/system/win32/thread/SysEvent_win32.o: ../../source/system/win32/thread/SysEvent_win32.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/win32/thread/SysEvent_win32.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/win32/thread/SysEvent_win32.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/win32/thread/SysEvent_win32.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/win32/thread/SysEvent_win32.d

# Compiles file ../../source/system/win32/thread/SysMutex_win32.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/win32/thread/SysMutex_win32.d
gccRelease_GLES2/../../source/system/win32/thread/SysMutex_win32.o: ../../source/system/win32/thread/SysMutex_win32.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/win32/thread/SysMutex_win32.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/win32/thread/SysMutex_win32.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/win32/thread/SysMutex_win32.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/win32/thread/SysMutex_win32.d

# Compiles file ../../source/system/win32/thread/SysSemaphore_win32.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/win32/thread/SysSemaphore_win32.d
gccRelease_GLES2/../../source/system/win32/thread/SysSemaphore_win32.o: ../../source/system/win32/thread/SysSemaphore_win32.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/win32/thread/SysSemaphore_win32.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/win32/thread/SysSemaphore_win32.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/win32/thread/SysSemaphore_win32.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/win32/thread/SysSemaphore_win32.d

# Compiles file ../../source/system/win32/thread/SysThread_win32.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/win32/thread/SysThread_win32.d
gccRelease_GLES2/../../source/system/win32/thread/SysThread_win32.o: ../../source/system/win32/thread/SysThread_win32.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/win32/thread/SysThread_win32.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/win32/thread/SysThread_win32.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/win32/thread/SysThread_win32.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/win32/thread/SysThread_win32.d

# Compiles file ../../source/system/win32/device/DeviceUtil.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/win32/device/DeviceUtil.d
gccRelease_GLES2/../../source/system/win32/device/DeviceUtil.o: ../../source/system/win32/device/DeviceUtil.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/win32/device/DeviceUtil.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/win32/device/DeviceUtil.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/win32/device/DeviceUtil.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/win32/device/DeviceUtil.d

# Compiles file ../../source/system/win32/device/Pad_DInput.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/win32/device/Pad_DInput.d
gccRelease_GLES2/../../source/system/win32/device/Pad_DInput.o: ../../source/system/win32/device/Pad_DInput.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/win32/device/Pad_DInput.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/win32/device/Pad_DInput.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/win32/device/Pad_DInput.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/win32/device/Pad_DInput.d

# Compiles file ../../source/system/win32/device/Pad_XInput.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/win32/device/Pad_XInput.d
gccRelease_GLES2/../../source/system/win32/device/Pad_XInput.o: ../../source/system/win32/device/Pad_XInput.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/win32/device/Pad_XInput.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/win32/device/Pad_XInput.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/win32/device/Pad_XInput.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/win32/device/Pad_XInput.d

# Compiles file ../../source/system/linux/thread/SysEvent_pthread.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/linux/thread/SysEvent_pthread.d
gccRelease_GLES2/../../source/system/linux/thread/SysEvent_pthread.o: ../../source/system/linux/thread/SysEvent_pthread.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysEvent_pthread.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/linux/thread/SysEvent_pthread.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysEvent_pthread.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/linux/thread/SysEvent_pthread.d

# Compiles file ../../source/system/linux/thread/SysMutex_pthread.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/linux/thread/SysMutex_pthread.d
gccRelease_GLES2/../../source/system/linux/thread/SysMutex_pthread.o: ../../source/system/linux/thread/SysMutex_pthread.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysMutex_pthread.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/linux/thread/SysMutex_pthread.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysMutex_pthread.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/linux/thread/SysMutex_pthread.d

# Compiles file ../../source/system/linux/thread/SysSemaphore_pthread.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/linux/thread/SysSemaphore_pthread.d
gccRelease_GLES2/../../source/system/linux/thread/SysSemaphore_pthread.o: ../../source/system/linux/thread/SysSemaphore_pthread.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysSemaphore_pthread.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/linux/thread/SysSemaphore_pthread.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysSemaphore_pthread.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/linux/thread/SysSemaphore_pthread.d

# Compiles file ../../source/system/linux/thread/SysThread_pthread.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/system/linux/thread/SysThread_pthread.d
gccRelease_GLES2/../../source/system/linux/thread/SysThread_pthread.o: ../../source/system/linux/thread/SysThread_pthread.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysThread_pthread.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/system/linux/thread/SysThread_pthread.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysThread_pthread.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/system/linux/thread/SysThread_pthread.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLES2/source
	mkdir -p ../lib/gccDebug_GLES2
	mkdir -p gccRelease_GLES2/source
	mkdir -p ../lib/gccRelease_GLES2

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLES2/*.o
	rm -f gccDebug_GLES2/*.d
	rm -f ../lib/gccDebug_GLES2/*.a
	rm -f ../lib/gccDebug_GLES2/*.so
	rm -f ../lib/gccDebug_GLES2/*.dll
	rm -f ../lib/gccDebug_GLES2/*.exe
	rm -f gccRelease_GLES2/*.o
	rm -f gccRelease_GLES2/*.d
	rm -f ../lib/gccRelease_GLES2/*.a
	rm -f ../lib/gccRelease_GLES2/*.so
	rm -f ../lib/gccRelease_GLES2/*.dll
	rm -f ../lib/gccRelease_GLES2/*.exe

