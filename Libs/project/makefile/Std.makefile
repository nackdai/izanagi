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
Debug_GLES2: create_folders gccDebug_GLES2/../../source/std/StdColor.o gccDebug_GLES2/../../source/std/StdKey.o gccDebug_GLES2/../../source/std/StdLog.o gccDebug_GLES2/../../source/std/StdProfiler.o gccDebug_GLES2/../../source/std/StdUtf.o gccDebug_GLES2/../../source/std/allocator/ChunkedMemoryAllocator.o gccDebug_GLES2/../../source/std/allocator/SimpleMemoryAllocator.o gccDebug_GLES2/../../source/std/allocator/StandardMemoryAllocator.o gccDebug_GLES2/../../source/std/allocator/STLMemoryAllocator.o gccDebug_GLES2/../../source/std/binding/Binding.o gccDebug_GLES2/../../source/std/binding/BindingExpression.o gccDebug_GLES2/../../source/std/binding/BindingOperations.o gccDebug_GLES2/../../source/std/binding/DependencyObject.o gccDebug_GLES2/../../source/std/binding/DependencyPropertyChangedEventArgs.o 
	ar rcs ../lib/gccDebug_GLES2/libStd.a gccDebug_GLES2/../../source/std/StdColor.o gccDebug_GLES2/../../source/std/StdKey.o gccDebug_GLES2/../../source/std/StdLog.o gccDebug_GLES2/../../source/std/StdProfiler.o gccDebug_GLES2/../../source/std/StdUtf.o gccDebug_GLES2/../../source/std/allocator/ChunkedMemoryAllocator.o gccDebug_GLES2/../../source/std/allocator/SimpleMemoryAllocator.o gccDebug_GLES2/../../source/std/allocator/StandardMemoryAllocator.o gccDebug_GLES2/../../source/std/allocator/STLMemoryAllocator.o gccDebug_GLES2/../../source/std/binding/Binding.o gccDebug_GLES2/../../source/std/binding/BindingExpression.o gccDebug_GLES2/../../source/std/binding/BindingOperations.o gccDebug_GLES2/../../source/std/binding/DependencyObject.o gccDebug_GLES2/../../source/std/binding/DependencyPropertyChangedEventArgs.o  $(Debug_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/std/StdColor.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/StdColor.d
gccDebug_GLES2/../../source/std/StdColor.o: ../../source/std/StdColor.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/StdColor.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/StdColor.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/StdColor.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/StdColor.d

# Compiles file ../../source/std/StdKey.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/StdKey.d
gccDebug_GLES2/../../source/std/StdKey.o: ../../source/std/StdKey.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/StdKey.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/StdKey.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/StdKey.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/StdKey.d

# Compiles file ../../source/std/StdLog.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/StdLog.d
gccDebug_GLES2/../../source/std/StdLog.o: ../../source/std/StdLog.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/StdLog.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/StdLog.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/StdLog.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/StdLog.d

# Compiles file ../../source/std/StdProfiler.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/StdProfiler.d
gccDebug_GLES2/../../source/std/StdProfiler.o: ../../source/std/StdProfiler.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/StdProfiler.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/StdProfiler.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/StdProfiler.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/StdProfiler.d

# Compiles file ../../source/std/StdUtf.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/StdUtf.d
gccDebug_GLES2/../../source/std/StdUtf.o: ../../source/std/StdUtf.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/StdUtf.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/StdUtf.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/StdUtf.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/StdUtf.d

# Compiles file ../../source/std/allocator/ChunkedMemoryAllocator.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/allocator/ChunkedMemoryAllocator.d
gccDebug_GLES2/../../source/std/allocator/ChunkedMemoryAllocator.o: ../../source/std/allocator/ChunkedMemoryAllocator.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/allocator/ChunkedMemoryAllocator.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/allocator/ChunkedMemoryAllocator.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/allocator/ChunkedMemoryAllocator.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/allocator/ChunkedMemoryAllocator.d

# Compiles file ../../source/std/allocator/SimpleMemoryAllocator.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/allocator/SimpleMemoryAllocator.d
gccDebug_GLES2/../../source/std/allocator/SimpleMemoryAllocator.o: ../../source/std/allocator/SimpleMemoryAllocator.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/allocator/SimpleMemoryAllocator.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/allocator/SimpleMemoryAllocator.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/allocator/SimpleMemoryAllocator.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/allocator/SimpleMemoryAllocator.d

# Compiles file ../../source/std/allocator/StandardMemoryAllocator.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/allocator/StandardMemoryAllocator.d
gccDebug_GLES2/../../source/std/allocator/StandardMemoryAllocator.o: ../../source/std/allocator/StandardMemoryAllocator.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/allocator/StandardMemoryAllocator.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/allocator/StandardMemoryAllocator.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/allocator/StandardMemoryAllocator.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/allocator/StandardMemoryAllocator.d

# Compiles file ../../source/std/allocator/STLMemoryAllocator.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/allocator/STLMemoryAllocator.d
gccDebug_GLES2/../../source/std/allocator/STLMemoryAllocator.o: ../../source/std/allocator/STLMemoryAllocator.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/allocator/STLMemoryAllocator.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/allocator/STLMemoryAllocator.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/allocator/STLMemoryAllocator.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/allocator/STLMemoryAllocator.d

# Compiles file ../../source/std/binding/Binding.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/binding/Binding.d
gccDebug_GLES2/../../source/std/binding/Binding.o: ../../source/std/binding/Binding.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/binding/Binding.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/binding/Binding.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/binding/Binding.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/binding/Binding.d

# Compiles file ../../source/std/binding/BindingExpression.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/binding/BindingExpression.d
gccDebug_GLES2/../../source/std/binding/BindingExpression.o: ../../source/std/binding/BindingExpression.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/binding/BindingExpression.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/binding/BindingExpression.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/binding/BindingExpression.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/binding/BindingExpression.d

# Compiles file ../../source/std/binding/BindingOperations.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/binding/BindingOperations.d
gccDebug_GLES2/../../source/std/binding/BindingOperations.o: ../../source/std/binding/BindingOperations.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/binding/BindingOperations.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/binding/BindingOperations.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/binding/BindingOperations.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/binding/BindingOperations.d

# Compiles file ../../source/std/binding/DependencyObject.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/binding/DependencyObject.d
gccDebug_GLES2/../../source/std/binding/DependencyObject.o: ../../source/std/binding/DependencyObject.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/binding/DependencyObject.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/binding/DependencyObject.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/binding/DependencyObject.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/binding/DependencyObject.d

# Compiles file ../../source/std/binding/DependencyPropertyChangedEventArgs.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/std/binding/DependencyPropertyChangedEventArgs.d
gccDebug_GLES2/../../source/std/binding/DependencyPropertyChangedEventArgs.o: ../../source/std/binding/DependencyPropertyChangedEventArgs.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/std/binding/DependencyPropertyChangedEventArgs.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/std/binding/DependencyPropertyChangedEventArgs.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/std/binding/DependencyPropertyChangedEventArgs.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/std/binding/DependencyPropertyChangedEventArgs.d

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders gccRelease_GLES2/../../source/std/StdColor.o gccRelease_GLES2/../../source/std/StdKey.o gccRelease_GLES2/../../source/std/StdLog.o gccRelease_GLES2/../../source/std/StdProfiler.o gccRelease_GLES2/../../source/std/StdUtf.o gccRelease_GLES2/../../source/std/allocator/ChunkedMemoryAllocator.o gccRelease_GLES2/../../source/std/allocator/SimpleMemoryAllocator.o gccRelease_GLES2/../../source/std/allocator/StandardMemoryAllocator.o gccRelease_GLES2/../../source/std/allocator/STLMemoryAllocator.o gccRelease_GLES2/../../source/std/binding/Binding.o gccRelease_GLES2/../../source/std/binding/BindingExpression.o gccRelease_GLES2/../../source/std/binding/BindingOperations.o gccRelease_GLES2/../../source/std/binding/DependencyObject.o gccRelease_GLES2/../../source/std/binding/DependencyPropertyChangedEventArgs.o 
	ar rcs ../lib/gccRelease_GLES2/libStd.a gccRelease_GLES2/../../source/std/StdColor.o gccRelease_GLES2/../../source/std/StdKey.o gccRelease_GLES2/../../source/std/StdLog.o gccRelease_GLES2/../../source/std/StdProfiler.o gccRelease_GLES2/../../source/std/StdUtf.o gccRelease_GLES2/../../source/std/allocator/ChunkedMemoryAllocator.o gccRelease_GLES2/../../source/std/allocator/SimpleMemoryAllocator.o gccRelease_GLES2/../../source/std/allocator/StandardMemoryAllocator.o gccRelease_GLES2/../../source/std/allocator/STLMemoryAllocator.o gccRelease_GLES2/../../source/std/binding/Binding.o gccRelease_GLES2/../../source/std/binding/BindingExpression.o gccRelease_GLES2/../../source/std/binding/BindingOperations.o gccRelease_GLES2/../../source/std/binding/DependencyObject.o gccRelease_GLES2/../../source/std/binding/DependencyPropertyChangedEventArgs.o  $(Release_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/std/StdColor.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/StdColor.d
gccRelease_GLES2/../../source/std/StdColor.o: ../../source/std/StdColor.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/StdColor.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/StdColor.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/StdColor.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/StdColor.d

# Compiles file ../../source/std/StdKey.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/StdKey.d
gccRelease_GLES2/../../source/std/StdKey.o: ../../source/std/StdKey.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/StdKey.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/StdKey.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/StdKey.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/StdKey.d

# Compiles file ../../source/std/StdLog.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/StdLog.d
gccRelease_GLES2/../../source/std/StdLog.o: ../../source/std/StdLog.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/StdLog.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/StdLog.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/StdLog.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/StdLog.d

# Compiles file ../../source/std/StdProfiler.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/StdProfiler.d
gccRelease_GLES2/../../source/std/StdProfiler.o: ../../source/std/StdProfiler.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/StdProfiler.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/StdProfiler.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/StdProfiler.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/StdProfiler.d

# Compiles file ../../source/std/StdUtf.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/StdUtf.d
gccRelease_GLES2/../../source/std/StdUtf.o: ../../source/std/StdUtf.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/StdUtf.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/StdUtf.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/StdUtf.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/StdUtf.d

# Compiles file ../../source/std/allocator/ChunkedMemoryAllocator.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/allocator/ChunkedMemoryAllocator.d
gccRelease_GLES2/../../source/std/allocator/ChunkedMemoryAllocator.o: ../../source/std/allocator/ChunkedMemoryAllocator.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/allocator/ChunkedMemoryAllocator.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/allocator/ChunkedMemoryAllocator.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/allocator/ChunkedMemoryAllocator.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/allocator/ChunkedMemoryAllocator.d

# Compiles file ../../source/std/allocator/SimpleMemoryAllocator.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/allocator/SimpleMemoryAllocator.d
gccRelease_GLES2/../../source/std/allocator/SimpleMemoryAllocator.o: ../../source/std/allocator/SimpleMemoryAllocator.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/allocator/SimpleMemoryAllocator.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/allocator/SimpleMemoryAllocator.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/allocator/SimpleMemoryAllocator.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/allocator/SimpleMemoryAllocator.d

# Compiles file ../../source/std/allocator/StandardMemoryAllocator.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/allocator/StandardMemoryAllocator.d
gccRelease_GLES2/../../source/std/allocator/StandardMemoryAllocator.o: ../../source/std/allocator/StandardMemoryAllocator.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/allocator/StandardMemoryAllocator.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/allocator/StandardMemoryAllocator.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/allocator/StandardMemoryAllocator.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/allocator/StandardMemoryAllocator.d

# Compiles file ../../source/std/allocator/STLMemoryAllocator.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/allocator/STLMemoryAllocator.d
gccRelease_GLES2/../../source/std/allocator/STLMemoryAllocator.o: ../../source/std/allocator/STLMemoryAllocator.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/allocator/STLMemoryAllocator.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/allocator/STLMemoryAllocator.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/allocator/STLMemoryAllocator.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/allocator/STLMemoryAllocator.d

# Compiles file ../../source/std/binding/Binding.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/binding/Binding.d
gccRelease_GLES2/../../source/std/binding/Binding.o: ../../source/std/binding/Binding.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/binding/Binding.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/binding/Binding.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/binding/Binding.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/binding/Binding.d

# Compiles file ../../source/std/binding/BindingExpression.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/binding/BindingExpression.d
gccRelease_GLES2/../../source/std/binding/BindingExpression.o: ../../source/std/binding/BindingExpression.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/binding/BindingExpression.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/binding/BindingExpression.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/binding/BindingExpression.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/binding/BindingExpression.d

# Compiles file ../../source/std/binding/BindingOperations.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/binding/BindingOperations.d
gccRelease_GLES2/../../source/std/binding/BindingOperations.o: ../../source/std/binding/BindingOperations.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/binding/BindingOperations.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/binding/BindingOperations.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/binding/BindingOperations.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/binding/BindingOperations.d

# Compiles file ../../source/std/binding/DependencyObject.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/binding/DependencyObject.d
gccRelease_GLES2/../../source/std/binding/DependencyObject.o: ../../source/std/binding/DependencyObject.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/binding/DependencyObject.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/binding/DependencyObject.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/binding/DependencyObject.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/binding/DependencyObject.d

# Compiles file ../../source/std/binding/DependencyPropertyChangedEventArgs.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/std/binding/DependencyPropertyChangedEventArgs.d
gccRelease_GLES2/../../source/std/binding/DependencyPropertyChangedEventArgs.o: ../../source/std/binding/DependencyPropertyChangedEventArgs.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/std/binding/DependencyPropertyChangedEventArgs.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/std/binding/DependencyPropertyChangedEventArgs.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/std/binding/DependencyPropertyChangedEventArgs.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/std/binding/DependencyPropertyChangedEventArgs.d

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