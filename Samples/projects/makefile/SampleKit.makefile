# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../Libs/project/lib/gccDebug_GLUT" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--start-group -lStd -lGraph -lDebugUtil -lSystem -lSceneGraph -lMath  -Wl,--end-group

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
Debug_GLUT: create_folders SampleKit/gccDebug_GLUT/SampleKit/SampleApp.o SampleKit/gccDebug_GLUT/SampleKit/SampleCamera.o SampleKit/gccDebug_GLUT/SampleKit/SampleEnvBox.o SampleKit/gccDebug_GLUT/SampleKit/SampleMain.o SampleKit/gccDebug_GLUT/SampleKit/SampleModelRenderHandler.o SampleKit/gccDebug_GLUT/SampleKit/SampleWindowProc.o 
	ar rcs SampleKit/gccDebug_GLUT/libSampleKit.a SampleKit/gccDebug_GLUT/SampleKit/SampleApp.o SampleKit/gccDebug_GLUT/SampleKit/SampleCamera.o SampleKit/gccDebug_GLUT/SampleKit/SampleEnvBox.o SampleKit/gccDebug_GLUT/SampleKit/SampleMain.o SampleKit/gccDebug_GLUT/SampleKit/SampleModelRenderHandler.o SampleKit/gccDebug_GLUT/SampleKit/SampleWindowProc.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../SampleKit/SampleApp.cpp for the Debug_GLUT configuration...
-include SampleKit/gccDebug_GLUT/SampleKit/SampleApp.d
SampleKit/gccDebug_GLUT/SampleKit/SampleApp.o: ../../SampleKit/SampleApp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../SampleKit/SampleApp.cpp $(Debug_GLUT_Include_Path) -o SampleKit/gccDebug_GLUT/SampleKit/SampleApp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../SampleKit/SampleApp.cpp $(Debug_GLUT_Include_Path) > SampleKit/gccDebug_GLUT/SampleKit/SampleApp.d

# Compiles file ../../SampleKit/SampleCamera.cpp for the Debug_GLUT configuration...
-include SampleKit/gccDebug_GLUT/SampleKit/SampleCamera.d
SampleKit/gccDebug_GLUT/SampleKit/SampleCamera.o: ../../SampleKit/SampleCamera.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../SampleKit/SampleCamera.cpp $(Debug_GLUT_Include_Path) -o SampleKit/gccDebug_GLUT/SampleKit/SampleCamera.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../SampleKit/SampleCamera.cpp $(Debug_GLUT_Include_Path) > SampleKit/gccDebug_GLUT/SampleKit/SampleCamera.d

# Compiles file ../../SampleKit/SampleEnvBox.cpp for the Debug_GLUT configuration...
-include SampleKit/gccDebug_GLUT/SampleKit/SampleEnvBox.d
SampleKit/gccDebug_GLUT/SampleKit/SampleEnvBox.o: ../../SampleKit/SampleEnvBox.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../SampleKit/SampleEnvBox.cpp $(Debug_GLUT_Include_Path) -o SampleKit/gccDebug_GLUT/SampleKit/SampleEnvBox.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../SampleKit/SampleEnvBox.cpp $(Debug_GLUT_Include_Path) > SampleKit/gccDebug_GLUT/SampleKit/SampleEnvBox.d

# Compiles file ../../SampleKit/SampleMain.cpp for the Debug_GLUT configuration...
-include SampleKit/gccDebug_GLUT/SampleKit/SampleMain.d
SampleKit/gccDebug_GLUT/SampleKit/SampleMain.o: ../../SampleKit/SampleMain.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../SampleKit/SampleMain.cpp $(Debug_GLUT_Include_Path) -o SampleKit/gccDebug_GLUT/SampleKit/SampleMain.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../SampleKit/SampleMain.cpp $(Debug_GLUT_Include_Path) > SampleKit/gccDebug_GLUT/SampleKit/SampleMain.d

# Compiles file ../../SampleKit/SampleModelRenderHandler.cpp for the Debug_GLUT configuration...
-include SampleKit/gccDebug_GLUT/SampleKit/SampleModelRenderHandler.d
SampleKit/gccDebug_GLUT/SampleKit/SampleModelRenderHandler.o: ../../SampleKit/SampleModelRenderHandler.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../SampleKit/SampleModelRenderHandler.cpp $(Debug_GLUT_Include_Path) -o SampleKit/gccDebug_GLUT/SampleKit/SampleModelRenderHandler.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../SampleKit/SampleModelRenderHandler.cpp $(Debug_GLUT_Include_Path) > SampleKit/gccDebug_GLUT/SampleKit/SampleModelRenderHandler.d

# Compiles file ../../SampleKit/SampleWindowProc.cpp for the Debug_GLUT configuration...
-include SampleKit/gccDebug_GLUT/SampleKit/SampleWindowProc.d
SampleKit/gccDebug_GLUT/SampleKit/SampleWindowProc.o: ../../SampleKit/SampleWindowProc.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../SampleKit/SampleWindowProc.cpp $(Debug_GLUT_Include_Path) -o SampleKit/gccDebug_GLUT/SampleKit/SampleWindowProc.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../SampleKit/SampleWindowProc.cpp $(Debug_GLUT_Include_Path) > SampleKit/gccDebug_GLUT/SampleKit/SampleWindowProc.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p SampleKit/gccDebug_GLUT/SampleKit

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f SampleKit/gccDebug_GLUT/SampleKit/*.o
	rm -f SampleKit/gccDebug_GLUT/SampleKit/*.d
	rm -f SampleKit/gccDebug_GLUT/libSampleKit.a

