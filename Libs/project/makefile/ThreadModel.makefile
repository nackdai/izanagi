# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../include" 

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
Debug_GLUT: create_folders gccDebug_GLUT/source/threadmodel/Parallel.o gccDebug_GLUT/source/threadmodel/ThreadModelTask.o gccDebug_GLUT/source/threadmodel/ThreadPool.o gccDebug_GLUT/source/threadmodel/ThreadModelJob.o gccDebug_GLUT/source/threadmodel/ThreadModelJobQueue.o gccDebug_GLUT/source/threadmodel/ThreadModelJobWorker.o gccDebug_GLUT/source/threadmodel/ThreadModelMessageQueue.o 
	ar rcs ../lib/gccDebug_GLUT/libThreadModel.a gccDebug_GLUT/source/threadmodel/Parallel.o gccDebug_GLUT/source/threadmodel/ThreadModelTask.o gccDebug_GLUT/source/threadmodel/ThreadPool.o gccDebug_GLUT/source/threadmodel/ThreadModelJob.o gccDebug_GLUT/source/threadmodel/ThreadModelJobQueue.o gccDebug_GLUT/source/threadmodel/ThreadModelJobWorker.o gccDebug_GLUT/source/threadmodel/ThreadModelMessageQueue.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../source/threadmodel/Parallel.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/threadmodel/Parallel.d
gccDebug_GLUT/source/threadmodel/Parallel.o: ../../source/threadmodel/Parallel.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/threadmodel/Parallel.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/threadmodel/Parallel.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/threadmodel/Parallel.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/threadmodel/Parallel.d

# Compiles file ../../source/threadmodel/ThreadModelTask.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/threadmodel/ThreadModelTask.d
gccDebug_GLUT/source/threadmodel/ThreadModelTask.o: ../../source/threadmodel/ThreadModelTask.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/threadmodel/ThreadModelTask.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/threadmodel/ThreadModelTask.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelTask.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/threadmodel/ThreadModelTask.d

# Compiles file ../../source/threadmodel/ThreadPool.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/threadmodel/ThreadPool.d
gccDebug_GLUT/source/threadmodel/ThreadPool.o: ../../source/threadmodel/ThreadPool.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/threadmodel/ThreadPool.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/threadmodel/ThreadPool.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/threadmodel/ThreadPool.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/threadmodel/ThreadPool.d

# Compiles file ../../source/threadmodel/ThreadModelJob.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/threadmodel/ThreadModelJob.d
gccDebug_GLUT/source/threadmodel/ThreadModelJob.o: ../../source/threadmodel/ThreadModelJob.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/threadmodel/ThreadModelJob.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/threadmodel/ThreadModelJob.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelJob.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/threadmodel/ThreadModelJob.d

# Compiles file ../../source/threadmodel/ThreadModelJobQueue.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/threadmodel/ThreadModelJobQueue.d
gccDebug_GLUT/source/threadmodel/ThreadModelJobQueue.o: ../../source/threadmodel/ThreadModelJobQueue.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/threadmodel/ThreadModelJobQueue.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/threadmodel/ThreadModelJobQueue.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelJobQueue.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/threadmodel/ThreadModelJobQueue.d

# Compiles file ../../source/threadmodel/ThreadModelJobWorker.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/threadmodel/ThreadModelJobWorker.d
gccDebug_GLUT/source/threadmodel/ThreadModelJobWorker.o: ../../source/threadmodel/ThreadModelJobWorker.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/threadmodel/ThreadModelJobWorker.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/threadmodel/ThreadModelJobWorker.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelJobWorker.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/threadmodel/ThreadModelJobWorker.d

# Compiles file ../../source/threadmodel/ThreadModelMessageQueue.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/threadmodel/ThreadModelMessageQueue.d
gccDebug_GLUT/source/threadmodel/ThreadModelMessageQueue.o: ../../source/threadmodel/ThreadModelMessageQueue.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/threadmodel/ThreadModelMessageQueue.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/threadmodel/ThreadModelMessageQueue.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/threadmodel/ThreadModelMessageQueue.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/threadmodel/ThreadModelMessageQueue.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLUT/source/threadmodel
	mkdir -p ../lib/gccDebug_GLUT

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLUT/source/threadmodel/*.o
	rm -f gccDebug_GLUT/source/threadmodel/*.d
	rm -f ../lib/gccDebug_GLUT/libThreadModel.a

