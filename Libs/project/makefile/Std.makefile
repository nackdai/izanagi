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
Debug_GLUT: create_folders gccDebug_GLUT/source/std/StdColor.o gccDebug_GLUT/source/std/StdKey.o gccDebug_GLUT/source/std/StdLog.o gccDebug_GLUT/source/std/StdProfiler.o gccDebug_GLUT/source/std/StdUtf.o gccDebug_GLUT/source/std/allocator/ChunkedMemoryAllocator.o gccDebug_GLUT/source/std/allocator/SimpleMemoryAllocator.o gccDebug_GLUT/source/std/allocator/StandardMemoryAllocator.o gccDebug_GLUT/source/std/allocator/STLMemoryAllocator.o gccDebug_GLUT/source/std/binding/Binding.o gccDebug_GLUT/source/std/binding/BindingExpression.o gccDebug_GLUT/source/std/binding/BindingOperations.o gccDebug_GLUT/source/std/binding/DependencyObject.o gccDebug_GLUT/source/std/binding/DependencyPropertyChangedEventArgs.o 
	ar rcs ../lib/gccDebug_GLUT/libStd.a gccDebug_GLUT/source/std/StdColor.o gccDebug_GLUT/source/std/StdKey.o gccDebug_GLUT/source/std/StdLog.o gccDebug_GLUT/source/std/StdProfiler.o gccDebug_GLUT/source/std/StdUtf.o gccDebug_GLUT/source/std/allocator/ChunkedMemoryAllocator.o gccDebug_GLUT/source/std/allocator/SimpleMemoryAllocator.o gccDebug_GLUT/source/std/allocator/StandardMemoryAllocator.o gccDebug_GLUT/source/std/allocator/STLMemoryAllocator.o gccDebug_GLUT/source/std/binding/Binding.o gccDebug_GLUT/source/std/binding/BindingExpression.o gccDebug_GLUT/source/std/binding/BindingOperations.o gccDebug_GLUT/source/std/binding/DependencyObject.o gccDebug_GLUT/source/std/binding/DependencyPropertyChangedEventArgs.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../source/std/StdColor.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/StdColor.d
gccDebug_GLUT/source/std/StdColor.o: ../../source/std/StdColor.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/StdColor.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/StdColor.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/StdColor.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/StdColor.d

# Compiles file ../../source/std/StdKey.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/StdKey.d
gccDebug_GLUT/source/std/StdKey.o: ../../source/std/StdKey.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/StdKey.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/StdKey.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/StdKey.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/StdKey.d

# Compiles file ../../source/std/StdLog.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/StdLog.d
gccDebug_GLUT/source/std/StdLog.o: ../../source/std/StdLog.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/StdLog.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/StdLog.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/StdLog.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/StdLog.d

# Compiles file ../../source/std/StdProfiler.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/StdProfiler.d
gccDebug_GLUT/source/std/StdProfiler.o: ../../source/std/StdProfiler.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/StdProfiler.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/StdProfiler.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/StdProfiler.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/StdProfiler.d

# Compiles file ../../source/std/StdUtf.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/StdUtf.d
gccDebug_GLUT/source/std/StdUtf.o: ../../source/std/StdUtf.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/StdUtf.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/StdUtf.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/StdUtf.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/StdUtf.d

# Compiles file ../../source/std/allocator/ChunkedMemoryAllocator.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/allocator/ChunkedMemoryAllocator.d
gccDebug_GLUT/source/std/allocator/ChunkedMemoryAllocator.o: ../../source/std/allocator/ChunkedMemoryAllocator.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/allocator/ChunkedMemoryAllocator.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/allocator/ChunkedMemoryAllocator.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/allocator/ChunkedMemoryAllocator.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/allocator/ChunkedMemoryAllocator.d

# Compiles file ../../source/std/allocator/SimpleMemoryAllocator.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/allocator/SimpleMemoryAllocator.d
gccDebug_GLUT/source/std/allocator/SimpleMemoryAllocator.o: ../../source/std/allocator/SimpleMemoryAllocator.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/allocator/SimpleMemoryAllocator.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/allocator/SimpleMemoryAllocator.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/allocator/SimpleMemoryAllocator.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/allocator/SimpleMemoryAllocator.d

# Compiles file ../../source/std/allocator/StandardMemoryAllocator.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/allocator/StandardMemoryAllocator.d
gccDebug_GLUT/source/std/allocator/StandardMemoryAllocator.o: ../../source/std/allocator/StandardMemoryAllocator.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/allocator/StandardMemoryAllocator.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/allocator/StandardMemoryAllocator.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/allocator/StandardMemoryAllocator.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/allocator/StandardMemoryAllocator.d

# Compiles file ../../source/std/allocator/STLMemoryAllocator.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/allocator/STLMemoryAllocator.d
gccDebug_GLUT/source/std/allocator/STLMemoryAllocator.o: ../../source/std/allocator/STLMemoryAllocator.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/allocator/STLMemoryAllocator.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/allocator/STLMemoryAllocator.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/allocator/STLMemoryAllocator.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/allocator/STLMemoryAllocator.d

# Compiles file ../../source/std/binding/Binding.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/binding/Binding.d
gccDebug_GLUT/source/std/binding/Binding.o: ../../source/std/binding/Binding.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/binding/Binding.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/binding/Binding.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/binding/Binding.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/binding/Binding.d

# Compiles file ../../source/std/binding/BindingExpression.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/binding/BindingExpression.d
gccDebug_GLUT/source/std/binding/BindingExpression.o: ../../source/std/binding/BindingExpression.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/binding/BindingExpression.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/binding/BindingExpression.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/binding/BindingExpression.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/binding/BindingExpression.d

# Compiles file ../../source/std/binding/BindingOperations.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/binding/BindingOperations.d
gccDebug_GLUT/source/std/binding/BindingOperations.o: ../../source/std/binding/BindingOperations.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/binding/BindingOperations.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/binding/BindingOperations.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/binding/BindingOperations.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/binding/BindingOperations.d

# Compiles file ../../source/std/binding/DependencyObject.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/binding/DependencyObject.d
gccDebug_GLUT/source/std/binding/DependencyObject.o: ../../source/std/binding/DependencyObject.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/binding/DependencyObject.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/binding/DependencyObject.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/binding/DependencyObject.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/binding/DependencyObject.d

# Compiles file ../../source/std/binding/DependencyPropertyChangedEventArgs.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/std/binding/DependencyPropertyChangedEventArgs.d
gccDebug_GLUT/source/std/binding/DependencyPropertyChangedEventArgs.o: ../../source/std/binding/DependencyPropertyChangedEventArgs.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/std/binding/DependencyPropertyChangedEventArgs.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/std/binding/DependencyPropertyChangedEventArgs.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/std/binding/DependencyPropertyChangedEventArgs.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/std/binding/DependencyPropertyChangedEventArgs.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLUT/source/std
	mkdir -p gccDebug_GLUT/source/std/allocator
	mkdir -p gccDebug_GLUT/source/std/binding
	mkdir -p ../lib/gccDebug_GLUT

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLUT/source/std/*.o
	rm -f gccDebug_GLUT/source/std/*.d
	rm -f gccDebug_GLUT/source/std/allocator/*.o
	rm -f gccDebug_GLUT/source/std/allocator/*.d
	rm -f gccDebug_GLUT/source/std/binding/*.o
	rm -f gccDebug_GLUT/source/std/binding/*.d
	rm -f ../lib/gccDebug_GLUT/libStd.a

