# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../include" -I"../../../External/glew/include" -I"../../../External/freeglut/include" 

# Library paths...
Debug_GLUT_Library_Path=

# Additional libraries...
Debug_GLUT_Libraries=

# Preprocessor definitions...
Debug_GLUT_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _LIB -D __IZ_DEBUG__ -D __IZ_OGL__ -D __IZ_GLUT__ -D __IZANAGI_NO_USE_D3D__ 

# Implictly linked object files...
Debug_GLUT_Implicitly_Linked_Objects=

# Compiler flags...
Debug_GLUT_Compiler_Flags=-O0 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug_GLUT 

# Builds the Debug_GLUT configuration...
.PHONY: Debug_GLUT
Debug_GLUT: create_folders gccDebug_GLUT/source/system/SysWindow_glut.o gccDebug_GLUT/source/system/linux/FileStream_linux.o gccDebug_GLUT/source/system/linux/SysEnvironment_linux.o gccDebug_GLUT/source/system/linux/SysTimer_linux.o gccDebug_GLUT/source/system/linux/SysUtil_linux.o gccDebug_GLUT/source/system/linux/device/Pad_linux.o gccDebug_GLUT/source/system/linux/thread/SysEvent_pthread.o gccDebug_GLUT/source/system/linux/thread/SysMutex_pthread.o gccDebug_GLUT/source/system/linux/thread/SysSemaphore_pthread.o gccDebug_GLUT/source/system/linux/thread/SysThread_pthread.o 
	ar rcs ../lib/gccDebug_GLUT/libSystem.a gccDebug_GLUT/source/system/SysWindow_glut.o gccDebug_GLUT/source/system/linux/FileStream_linux.o gccDebug_GLUT/source/system/linux/SysEnvironment_linux.o gccDebug_GLUT/source/system/linux/SysTimer_linux.o gccDebug_GLUT/source/system/linux/SysUtil_linux.o gccDebug_GLUT/source/system/linux/device/Pad_linux.o gccDebug_GLUT/source/system/linux/thread/SysEvent_pthread.o gccDebug_GLUT/source/system/linux/thread/SysMutex_pthread.o gccDebug_GLUT/source/system/linux/thread/SysSemaphore_pthread.o gccDebug_GLUT/source/system/linux/thread/SysThread_pthread.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../source/system/SysWindow_glut.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/system/SysWindow_glut.d
gccDebug_GLUT/source/system/SysWindow_glut.o: ../../source/system/SysWindow_glut.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/system/SysWindow_glut.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/system/SysWindow_glut.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/system/SysWindow_glut.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/system/SysWindow_glut.d

# Compiles file ../../source/system/linux/FileStream_linux.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/system/linux/FileStream_linux.d
gccDebug_GLUT/source/system/linux/FileStream_linux.o: ../../source/system/linux/FileStream_linux.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/system/linux/FileStream_linux.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/system/linux/FileStream_linux.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/system/linux/FileStream_linux.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/system/linux/FileStream_linux.d

# Compiles file ../../source/system/linux/SysEnvironment_linux.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/system/linux/SysEnvironment_linux.d
gccDebug_GLUT/source/system/linux/SysEnvironment_linux.o: ../../source/system/linux/SysEnvironment_linux.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/system/linux/SysEnvironment_linux.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/system/linux/SysEnvironment_linux.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/system/linux/SysEnvironment_linux.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/system/linux/SysEnvironment_linux.d

# Compiles file ../../source/system/linux/SysTimer_linux.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/system/linux/SysTimer_linux.d
gccDebug_GLUT/source/system/linux/SysTimer_linux.o: ../../source/system/linux/SysTimer_linux.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/system/linux/SysTimer_linux.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/system/linux/SysTimer_linux.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/system/linux/SysTimer_linux.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/system/linux/SysTimer_linux.d

# Compiles file ../../source/system/linux/SysUtil_linux.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/system/linux/SysUtil_linux.d
gccDebug_GLUT/source/system/linux/SysUtil_linux.o: ../../source/system/linux/SysUtil_linux.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/system/linux/SysUtil_linux.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/system/linux/SysUtil_linux.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/system/linux/SysUtil_linux.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/system/linux/SysUtil_linux.d

# Compiles file ../../source/system/linux/device/Pad_linux.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/system/linux/device/Pad_linux.d
gccDebug_GLUT/source/system/linux/device/Pad_linux.o: ../../source/system/linux/device/Pad_linux.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/system/linux/device/Pad_linux.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/system/linux/device/Pad_linux.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/system/linux/device/Pad_linux.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/system/linux/device/Pad_linux.d

# Compiles file ../../source/system/linux/thread/SysEvent_pthread.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/system/linux/thread/SysEvent_pthread.d
gccDebug_GLUT/source/system/linux/thread/SysEvent_pthread.o: ../../source/system/linux/thread/SysEvent_pthread.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/system/linux/thread/SysEvent_pthread.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/system/linux/thread/SysEvent_pthread.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/system/linux/thread/SysEvent_pthread.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/system/linux/thread/SysEvent_pthread.d

# Compiles file ../../source/system/linux/thread/SysMutex_pthread.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/system/linux/thread/SysMutex_pthread.d
gccDebug_GLUT/source/system/linux/thread/SysMutex_pthread.o: ../../source/system/linux/thread/SysMutex_pthread.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/system/linux/thread/SysMutex_pthread.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/system/linux/thread/SysMutex_pthread.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/system/linux/thread/SysMutex_pthread.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/system/linux/thread/SysMutex_pthread.d

# Compiles file ../../source/system/linux/thread/SysSemaphore_pthread.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/system/linux/thread/SysSemaphore_pthread.d
gccDebug_GLUT/source/system/linux/thread/SysSemaphore_pthread.o: ../../source/system/linux/thread/SysSemaphore_pthread.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/system/linux/thread/SysSemaphore_pthread.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/system/linux/thread/SysSemaphore_pthread.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/system/linux/thread/SysSemaphore_pthread.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/system/linux/thread/SysSemaphore_pthread.d

# Compiles file ../../source/system/linux/thread/SysThread_pthread.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/system/linux/thread/SysThread_pthread.d
gccDebug_GLUT/source/system/linux/thread/SysThread_pthread.o: ../../source/system/linux/thread/SysThread_pthread.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/system/linux/thread/SysThread_pthread.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/system/linux/thread/SysThread_pthread.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/system/linux/thread/SysThread_pthread.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/system/linux/thread/SysThread_pthread.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLUT/source/system
	mkdir -p gccDebug_GLUT/source/system/linux
	mkdir -p gccDebug_GLUT/source/system/linux/device
	mkdir -p gccDebug_GLUT/source/system/linux/thread
	mkdir -p ../lib/gccDebug_GLUT

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLUT/source/system/*.o
	rm -f gccDebug_GLUT/source/system/*.d
	rm -f gccDebug_GLUT/source/system/linux/*.o
	rm -f gccDebug_GLUT/source/system/linux/*.d
	rm -f gccDebug_GLUT/source/system/linux/device/*.o
	rm -f gccDebug_GLUT/source/system/linux/device/*.d
	rm -f gccDebug_GLUT/source/system/linux/thread/*.o
	rm -f gccDebug_GLUT/source/system/linux/thread/*.d
	rm -f ../lib/gccDebug_GLUT/libSystem.a

