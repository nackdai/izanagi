# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLES2_Include_Path=-I"../../include" 
Release_GLES2_Include_Path=-I"../../include" 

# Library paths...
Debug_GLES2_Library_Path=
Release_GLES2_Library_Path=

# Additional libraries...
Debug_GLES2_Libraries=
Release_GLES2_Libraries=

# Preprocessor definitions...
Debug_GLES2_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _LIB -D __IZ_DEBUG__ -D __IZ_GLES2__ -D __IZANAGI_NO_USE_D3D__ 
Release_GLES2_Preprocessor_Definitions=-D GCC_BUILD -D NDEBUG -D _LIB -D __IZ_GLES2__ -D __IZANAGI_NO_USE_D3D__ 

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
Debug_GLES2: create_folders gccDebug_GLES2/source/system/Pad.o gccDebug_GLES2/source/system/linux/SysEnvironment_linux.o gccDebug_GLES2/source/system/linux/SysTimer_linux.o gccDebug_GLES2/source/system/linux/SysUtil_linux.o gccDebug_GLES2/source/system/linux/device/Pad_linux.o gccDebug_GLES2/source/system/linux/thread/SysEvent_pthread.o gccDebug_GLES2/source/system/linux/thread/SysMutex_pthread.o gccDebug_GLES2/source/system/linux/thread/SysSemaphore_pthread.o gccDebug_GLES2/source/system/linux/thread/SysThread_pthread.o 
	ar rcs ../lib/gccDebug_GLES2/libSystem.a gccDebug_GLES2/source/system/Pad.o gccDebug_GLES2/source/system/linux/SysEnvironment_linux.o gccDebug_GLES2/source/system/linux/SysTimer_linux.o gccDebug_GLES2/source/system/linux/SysUtil_linux.o gccDebug_GLES2/source/system/linux/device/Pad_linux.o gccDebug_GLES2/source/system/linux/thread/SysEvent_pthread.o gccDebug_GLES2/source/system/linux/thread/SysMutex_pthread.o gccDebug_GLES2/source/system/linux/thread/SysSemaphore_pthread.o gccDebug_GLES2/source/system/linux/thread/SysThread_pthread.o  $(Debug_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/system/Pad.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/system/Pad.d
gccDebug_GLES2/source/system/Pad.o: ../../source/system/Pad.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/Pad.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/system/Pad.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/Pad.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/system/Pad.d

# Compiles file ../../source/system/linux/SysEnvironment_linux.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/system/linux/SysEnvironment_linux.d
gccDebug_GLES2/source/system/linux/SysEnvironment_linux.o: ../../source/system/linux/SysEnvironment_linux.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/linux/SysEnvironment_linux.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/system/linux/SysEnvironment_linux.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/linux/SysEnvironment_linux.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/system/linux/SysEnvironment_linux.d

# Compiles file ../../source/system/linux/SysTimer_linux.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/system/linux/SysTimer_linux.d
gccDebug_GLES2/source/system/linux/SysTimer_linux.o: ../../source/system/linux/SysTimer_linux.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/linux/SysTimer_linux.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/system/linux/SysTimer_linux.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/linux/SysTimer_linux.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/system/linux/SysTimer_linux.d

# Compiles file ../../source/system/linux/SysUtil_linux.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/system/linux/SysUtil_linux.d
gccDebug_GLES2/source/system/linux/SysUtil_linux.o: ../../source/system/linux/SysUtil_linux.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/linux/SysUtil_linux.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/system/linux/SysUtil_linux.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/linux/SysUtil_linux.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/system/linux/SysUtil_linux.d

# Compiles file ../../source/system/linux/device/Pad_linux.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/system/linux/device/Pad_linux.d
gccDebug_GLES2/source/system/linux/device/Pad_linux.o: ../../source/system/linux/device/Pad_linux.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/linux/device/Pad_linux.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/system/linux/device/Pad_linux.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/linux/device/Pad_linux.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/system/linux/device/Pad_linux.d

# Compiles file ../../source/system/linux/thread/SysEvent_pthread.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/system/linux/thread/SysEvent_pthread.d
gccDebug_GLES2/source/system/linux/thread/SysEvent_pthread.o: ../../source/system/linux/thread/SysEvent_pthread.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysEvent_pthread.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/system/linux/thread/SysEvent_pthread.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysEvent_pthread.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/system/linux/thread/SysEvent_pthread.d

# Compiles file ../../source/system/linux/thread/SysMutex_pthread.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/system/linux/thread/SysMutex_pthread.d
gccDebug_GLES2/source/system/linux/thread/SysMutex_pthread.o: ../../source/system/linux/thread/SysMutex_pthread.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysMutex_pthread.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/system/linux/thread/SysMutex_pthread.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysMutex_pthread.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/system/linux/thread/SysMutex_pthread.d

# Compiles file ../../source/system/linux/thread/SysSemaphore_pthread.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/system/linux/thread/SysSemaphore_pthread.d
gccDebug_GLES2/source/system/linux/thread/SysSemaphore_pthread.o: ../../source/system/linux/thread/SysSemaphore_pthread.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysSemaphore_pthread.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/system/linux/thread/SysSemaphore_pthread.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysSemaphore_pthread.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/system/linux/thread/SysSemaphore_pthread.d

# Compiles file ../../source/system/linux/thread/SysThread_pthread.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/system/linux/thread/SysThread_pthread.d
gccDebug_GLES2/source/system/linux/thread/SysThread_pthread.o: ../../source/system/linux/thread/SysThread_pthread.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysThread_pthread.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/system/linux/thread/SysThread_pthread.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysThread_pthread.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/system/linux/thread/SysThread_pthread.d

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders gccRelease_GLES2/source/system/Pad.o gccRelease_GLES2/source/system/linux/SysEnvironment_linux.o gccRelease_GLES2/source/system/linux/SysTimer_linux.o gccRelease_GLES2/source/system/linux/SysUtil_linux.o gccRelease_GLES2/source/system/linux/device/Pad_linux.o gccRelease_GLES2/source/system/linux/thread/SysEvent_pthread.o gccRelease_GLES2/source/system/linux/thread/SysMutex_pthread.o gccRelease_GLES2/source/system/linux/thread/SysSemaphore_pthread.o gccRelease_GLES2/source/system/linux/thread/SysThread_pthread.o 
	ar rcs ../lib/gccRelease_GLES2/libSystem.a gccRelease_GLES2/source/system/Pad.o gccRelease_GLES2/source/system/linux/SysEnvironment_linux.o gccRelease_GLES2/source/system/linux/SysTimer_linux.o gccRelease_GLES2/source/system/linux/SysUtil_linux.o gccRelease_GLES2/source/system/linux/device/Pad_linux.o gccRelease_GLES2/source/system/linux/thread/SysEvent_pthread.o gccRelease_GLES2/source/system/linux/thread/SysMutex_pthread.o gccRelease_GLES2/source/system/linux/thread/SysSemaphore_pthread.o gccRelease_GLES2/source/system/linux/thread/SysThread_pthread.o  $(Release_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/system/Pad.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/system/Pad.d
gccRelease_GLES2/source/system/Pad.o: ../../source/system/Pad.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/Pad.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/system/Pad.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/Pad.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/system/Pad.d

# Compiles file ../../source/system/linux/SysEnvironment_linux.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/system/linux/SysEnvironment_linux.d
gccRelease_GLES2/source/system/linux/SysEnvironment_linux.o: ../../source/system/linux/SysEnvironment_linux.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/linux/SysEnvironment_linux.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/system/linux/SysEnvironment_linux.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/linux/SysEnvironment_linux.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/system/linux/SysEnvironment_linux.d

# Compiles file ../../source/system/linux/SysTimer_linux.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/system/linux/SysTimer_linux.d
gccRelease_GLES2/source/system/linux/SysTimer_linux.o: ../../source/system/linux/SysTimer_linux.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/linux/SysTimer_linux.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/system/linux/SysTimer_linux.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/linux/SysTimer_linux.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/system/linux/SysTimer_linux.d

# Compiles file ../../source/system/linux/SysUtil_linux.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/system/linux/SysUtil_linux.d
gccRelease_GLES2/source/system/linux/SysUtil_linux.o: ../../source/system/linux/SysUtil_linux.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/linux/SysUtil_linux.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/system/linux/SysUtil_linux.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/linux/SysUtil_linux.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/system/linux/SysUtil_linux.d

# Compiles file ../../source/system/linux/device/Pad_linux.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/system/linux/device/Pad_linux.d
gccRelease_GLES2/source/system/linux/device/Pad_linux.o: ../../source/system/linux/device/Pad_linux.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/linux/device/Pad_linux.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/system/linux/device/Pad_linux.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/linux/device/Pad_linux.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/system/linux/device/Pad_linux.d

# Compiles file ../../source/system/linux/thread/SysEvent_pthread.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/system/linux/thread/SysEvent_pthread.d
gccRelease_GLES2/source/system/linux/thread/SysEvent_pthread.o: ../../source/system/linux/thread/SysEvent_pthread.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysEvent_pthread.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/system/linux/thread/SysEvent_pthread.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysEvent_pthread.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/system/linux/thread/SysEvent_pthread.d

# Compiles file ../../source/system/linux/thread/SysMutex_pthread.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/system/linux/thread/SysMutex_pthread.d
gccRelease_GLES2/source/system/linux/thread/SysMutex_pthread.o: ../../source/system/linux/thread/SysMutex_pthread.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysMutex_pthread.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/system/linux/thread/SysMutex_pthread.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysMutex_pthread.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/system/linux/thread/SysMutex_pthread.d

# Compiles file ../../source/system/linux/thread/SysSemaphore_pthread.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/system/linux/thread/SysSemaphore_pthread.d
gccRelease_GLES2/source/system/linux/thread/SysSemaphore_pthread.o: ../../source/system/linux/thread/SysSemaphore_pthread.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysSemaphore_pthread.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/system/linux/thread/SysSemaphore_pthread.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysSemaphore_pthread.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/system/linux/thread/SysSemaphore_pthread.d

# Compiles file ../../source/system/linux/thread/SysThread_pthread.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/system/linux/thread/SysThread_pthread.d
gccRelease_GLES2/source/system/linux/thread/SysThread_pthread.o: ../../source/system/linux/thread/SysThread_pthread.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/system/linux/thread/SysThread_pthread.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/system/linux/thread/SysThread_pthread.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/system/linux/thread/SysThread_pthread.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/system/linux/thread/SysThread_pthread.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLES2/source/system
	mkdir -p gccDebug_GLES2/source/system/linux
	mkdir -p gccDebug_GLES2/source/system/linux/device
	mkdir -p gccDebug_GLES2/source/system/linux/thread
	mkdir -p ../lib/gccDebug_GLES2
	mkdir -p gccRelease_GLES2/source/system
	mkdir -p gccRelease_GLES2/source/system/linux
	mkdir -p gccRelease_GLES2/source/system/linux/device
	mkdir -p gccRelease_GLES2/source/system/linux/thread
	mkdir -p ../lib/gccRelease_GLES2

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLES2/source/system/*.o
	rm -f gccDebug_GLES2/source/system/*.d
	rm -f gccDebug_GLES2/source/system/linux/*.o
	rm -f gccDebug_GLES2/source/system/linux/*.d
	rm -f gccDebug_GLES2/source/system/linux/device/*.o
	rm -f gccDebug_GLES2/source/system/linux/device/*.d
	rm -f gccDebug_GLES2/source/system/linux/thread/*.o
	rm -f gccDebug_GLES2/source/system/linux/thread/*.d
	rm -f ../lib/gccDebug_GLES2/libSystem.a
	rm -f gccRelease_GLES2/source/system/*.o
	rm -f gccRelease_GLES2/source/system/*.d
	rm -f gccRelease_GLES2/source/system/linux/*.o
	rm -f gccRelease_GLES2/source/system/linux/*.d
	rm -f gccRelease_GLES2/source/system/linux/device/*.o
	rm -f gccRelease_GLES2/source/system/linux/device/*.d
	rm -f gccRelease_GLES2/source/system/linux/thread/*.o
	rm -f gccRelease_GLES2/source/system/linux/thread/*.d
	rm -f ../lib/gccRelease_GLES2/libSystem.a

