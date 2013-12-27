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
Debug_GLES2: create_folders gccDebug_GLES2/../../source/threadmodel/Parallel.o gccDebug_GLES2/../../source/threadmodel/ThreadModelTask.o gccDebug_GLES2/../../source/threadmodel/ThreadPool.o gccDebug_GLES2/../../source/threadmodel/ThreadModelJob.o gccDebug_GLES2/../../source/threadmodel/ThreadModelJobQueue.o gccDebug_GLES2/../../source/threadmodel/ThreadModelJobWorker.o gccDebug_GLES2/../../source/threadmodel/ThreadModelMessageQueue.o 
	ar rcs ../lib/gccDebug_GLES2/libThreadModel.a gccDebug_GLES2/../../source/threadmodel/Parallel.o gccDebug_GLES2/../../source/threadmodel/ThreadModelTask.o gccDebug_GLES2/../../source/threadmodel/ThreadPool.o gccDebug_GLES2/../../source/threadmodel/ThreadModelJob.o gccDebug_GLES2/../../source/threadmodel/ThreadModelJobQueue.o gccDebug_GLES2/../../source/threadmodel/ThreadModelJobWorker.o gccDebug_GLES2/../../source/threadmodel/ThreadModelMessageQueue.o  $(Debug_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/threadmodel/Parallel.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/threadmodel/Parallel.d
gccDebug_GLES2/../../source/threadmodel/Parallel.o: ../../source/threadmodel/Parallel.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/threadmodel/Parallel.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/threadmodel/Parallel.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/threadmodel/Parallel.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/threadmodel/Parallel.d

# Compiles file ../../source/threadmodel/ThreadModelTask.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/threadmodel/ThreadModelTask.d
gccDebug_GLES2/../../source/threadmodel/ThreadModelTask.o: ../../source/threadmodel/ThreadModelTask.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/threadmodel/ThreadModelTask.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/threadmodel/ThreadModelTask.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelTask.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/threadmodel/ThreadModelTask.d

# Compiles file ../../source/threadmodel/ThreadPool.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/threadmodel/ThreadPool.d
gccDebug_GLES2/../../source/threadmodel/ThreadPool.o: ../../source/threadmodel/ThreadPool.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/threadmodel/ThreadPool.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/threadmodel/ThreadPool.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/threadmodel/ThreadPool.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/threadmodel/ThreadPool.d

# Compiles file ../../source/threadmodel/ThreadModelJob.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/threadmodel/ThreadModelJob.d
gccDebug_GLES2/../../source/threadmodel/ThreadModelJob.o: ../../source/threadmodel/ThreadModelJob.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/threadmodel/ThreadModelJob.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/threadmodel/ThreadModelJob.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelJob.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/threadmodel/ThreadModelJob.d

# Compiles file ../../source/threadmodel/ThreadModelJobQueue.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/threadmodel/ThreadModelJobQueue.d
gccDebug_GLES2/../../source/threadmodel/ThreadModelJobQueue.o: ../../source/threadmodel/ThreadModelJobQueue.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/threadmodel/ThreadModelJobQueue.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/threadmodel/ThreadModelJobQueue.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelJobQueue.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/threadmodel/ThreadModelJobQueue.d

# Compiles file ../../source/threadmodel/ThreadModelJobWorker.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/threadmodel/ThreadModelJobWorker.d
gccDebug_GLES2/../../source/threadmodel/ThreadModelJobWorker.o: ../../source/threadmodel/ThreadModelJobWorker.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/threadmodel/ThreadModelJobWorker.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/threadmodel/ThreadModelJobWorker.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelJobWorker.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/threadmodel/ThreadModelJobWorker.d

# Compiles file ../../source/threadmodel/ThreadModelMessageQueue.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/threadmodel/ThreadModelMessageQueue.d
gccDebug_GLES2/../../source/threadmodel/ThreadModelMessageQueue.o: ../../source/threadmodel/ThreadModelMessageQueue.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/threadmodel/ThreadModelMessageQueue.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/threadmodel/ThreadModelMessageQueue.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelMessageQueue.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/threadmodel/ThreadModelMessageQueue.d

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders gccRelease_GLES2/../../source/threadmodel/Parallel.o gccRelease_GLES2/../../source/threadmodel/ThreadModelTask.o gccRelease_GLES2/../../source/threadmodel/ThreadPool.o gccRelease_GLES2/../../source/threadmodel/ThreadModelJob.o gccRelease_GLES2/../../source/threadmodel/ThreadModelJobQueue.o gccRelease_GLES2/../../source/threadmodel/ThreadModelJobWorker.o gccRelease_GLES2/../../source/threadmodel/ThreadModelMessageQueue.o 
	ar rcs ../lib/gccRelease_GLES2/libThreadModel.a gccRelease_GLES2/../../source/threadmodel/Parallel.o gccRelease_GLES2/../../source/threadmodel/ThreadModelTask.o gccRelease_GLES2/../../source/threadmodel/ThreadPool.o gccRelease_GLES2/../../source/threadmodel/ThreadModelJob.o gccRelease_GLES2/../../source/threadmodel/ThreadModelJobQueue.o gccRelease_GLES2/../../source/threadmodel/ThreadModelJobWorker.o gccRelease_GLES2/../../source/threadmodel/ThreadModelMessageQueue.o  $(Release_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/threadmodel/Parallel.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/threadmodel/Parallel.d
gccRelease_GLES2/../../source/threadmodel/Parallel.o: ../../source/threadmodel/Parallel.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/threadmodel/Parallel.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/threadmodel/Parallel.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/threadmodel/Parallel.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/threadmodel/Parallel.d

# Compiles file ../../source/threadmodel/ThreadModelTask.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/threadmodel/ThreadModelTask.d
gccRelease_GLES2/../../source/threadmodel/ThreadModelTask.o: ../../source/threadmodel/ThreadModelTask.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/threadmodel/ThreadModelTask.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/threadmodel/ThreadModelTask.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelTask.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/threadmodel/ThreadModelTask.d

# Compiles file ../../source/threadmodel/ThreadPool.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/threadmodel/ThreadPool.d
gccRelease_GLES2/../../source/threadmodel/ThreadPool.o: ../../source/threadmodel/ThreadPool.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/threadmodel/ThreadPool.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/threadmodel/ThreadPool.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/threadmodel/ThreadPool.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/threadmodel/ThreadPool.d

# Compiles file ../../source/threadmodel/ThreadModelJob.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/threadmodel/ThreadModelJob.d
gccRelease_GLES2/../../source/threadmodel/ThreadModelJob.o: ../../source/threadmodel/ThreadModelJob.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/threadmodel/ThreadModelJob.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/threadmodel/ThreadModelJob.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelJob.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/threadmodel/ThreadModelJob.d

# Compiles file ../../source/threadmodel/ThreadModelJobQueue.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/threadmodel/ThreadModelJobQueue.d
gccRelease_GLES2/../../source/threadmodel/ThreadModelJobQueue.o: ../../source/threadmodel/ThreadModelJobQueue.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/threadmodel/ThreadModelJobQueue.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/threadmodel/ThreadModelJobQueue.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelJobQueue.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/threadmodel/ThreadModelJobQueue.d

# Compiles file ../../source/threadmodel/ThreadModelJobWorker.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/threadmodel/ThreadModelJobWorker.d
gccRelease_GLES2/../../source/threadmodel/ThreadModelJobWorker.o: ../../source/threadmodel/ThreadModelJobWorker.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/threadmodel/ThreadModelJobWorker.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/threadmodel/ThreadModelJobWorker.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelJobWorker.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/threadmodel/ThreadModelJobWorker.d

# Compiles file ../../source/threadmodel/ThreadModelMessageQueue.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/threadmodel/ThreadModelMessageQueue.d
gccRelease_GLES2/../../source/threadmodel/ThreadModelMessageQueue.o: ../../source/threadmodel/ThreadModelMessageQueue.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/threadmodel/ThreadModelMessageQueue.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/threadmodel/ThreadModelMessageQueue.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelMessageQueue.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/threadmodel/ThreadModelMessageQueue.d

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

