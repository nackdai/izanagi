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
Debug_GLES2: create_folders gccDebug_GLES2/source/graph/BufferBase.o gccDebug_GLES2/source/graph/GraphicsDevice.o gccDebug_GLES2/source/graph/RenderState.o gccDebug_GLES2/source/graph/ShaderProgram.o gccDebug_GLES2/source/graph/2d/2DShader.o gccDebug_GLES2/source/graph/2d/2DRenderer.o gccDebug_GLES2/source/graph/dx9/2DShader_DX9.o gccDebug_GLES2/source/graph/dx9/CubeTexture_DX9.o gccDebug_GLES2/source/graph/dx9/D3D9ParamValueConverter.o gccDebug_GLES2/source/graph/dx9/GraphicsDeviceResourceFuncs.o gccDebug_GLES2/source/graph/dx9/GraphicsDevice_DX9.o gccDebug_GLES2/source/graph/dx9/GraphPerf_DX9.o gccDebug_GLES2/source/graph/dx9/IndexBuffer_DX9.o gccDebug_GLES2/source/graph/dx9/PixelShader_DX9.o gccDebug_GLES2/source/graph/dx9/RenderState_DX9.o gccDebug_GLES2/source/graph/dx9/RenderTarget_DX9.o gccDebug_GLES2/source/graph/dx9/ShaderProgram_DX9.o gccDebug_GLES2/source/graph/dx9/Surface_DX9.o gccDebug_GLES2/source/graph/dx9/Texture_DX9.o gccDebug_GLES2/source/graph/dx9/VertexBuffer_DX9.o gccDebug_GLES2/source/graph/dx9/VertexDeclaration_DX9.o gccDebug_GLES2/source/graph/dx9/VertexShader_DX9.o gccDebug_GLES2/source/graph/gles2/2DShader_GLES2.o gccDebug_GLES2/source/graph/gles2/CubeTexture_GLES2.o gccDebug_GLES2/source/graph/gles2/FrameBufferObject.o gccDebug_GLES2/source/graph/gles2/GraphicsDeviceResourceFuncs.o gccDebug_GLES2/source/graph/gles2/GraphicsDevice_GLES2.o gccDebug_GLES2/source/graph/gles2/GraphPerf_GLES2.o gccDebug_GLES2/source/graph/gles2/IndexBuffer_GLES2.o gccDebug_GLES2/source/graph/gles2/ParamValueConverter_GLES2.o gccDebug_GLES2/source/graph/gles2/PixelShader_GLES2.o gccDebug_GLES2/source/graph/gles2/RenderState_GLES2.o gccDebug_GLES2/source/graph/gles2/RenderTarget_GLES2.o gccDebug_GLES2/source/graph/gles2/ShaderProgram_GLES2.o gccDebug_GLES2/source/graph/gles2/Texture_GLES2.o gccDebug_GLES2/source/graph/gles2/VertexBuffer_GLES2.o gccDebug_GLES2/source/graph/gles2/VertexDeclaration_GLES2.o gccDebug_GLES2/source/graph/gles2/VertexShader_GLES2.o 
	ar rcs ../lib/gccDebug_GLES2/libGraph.a gccDebug_GLES2/source/graph/BufferBase.o gccDebug_GLES2/source/graph/GraphicsDevice.o gccDebug_GLES2/source/graph/RenderState.o gccDebug_GLES2/source/graph/ShaderProgram.o gccDebug_GLES2/source/graph/2d/2DShader.o gccDebug_GLES2/source/graph/2d/2DRenderer.o gccDebug_GLES2/source/graph/dx9/2DShader_DX9.o gccDebug_GLES2/source/graph/dx9/CubeTexture_DX9.o gccDebug_GLES2/source/graph/dx9/D3D9ParamValueConverter.o gccDebug_GLES2/source/graph/dx9/GraphicsDeviceResourceFuncs.o gccDebug_GLES2/source/graph/dx9/GraphicsDevice_DX9.o gccDebug_GLES2/source/graph/dx9/GraphPerf_DX9.o gccDebug_GLES2/source/graph/dx9/IndexBuffer_DX9.o gccDebug_GLES2/source/graph/dx9/PixelShader_DX9.o gccDebug_GLES2/source/graph/dx9/RenderState_DX9.o gccDebug_GLES2/source/graph/dx9/RenderTarget_DX9.o gccDebug_GLES2/source/graph/dx9/ShaderProgram_DX9.o gccDebug_GLES2/source/graph/dx9/Surface_DX9.o gccDebug_GLES2/source/graph/dx9/Texture_DX9.o gccDebug_GLES2/source/graph/dx9/VertexBuffer_DX9.o gccDebug_GLES2/source/graph/dx9/VertexDeclaration_DX9.o gccDebug_GLES2/source/graph/dx9/VertexShader_DX9.o gccDebug_GLES2/source/graph/gles2/2DShader_GLES2.o gccDebug_GLES2/source/graph/gles2/CubeTexture_GLES2.o gccDebug_GLES2/source/graph/gles2/FrameBufferObject.o gccDebug_GLES2/source/graph/gles2/GraphicsDeviceResourceFuncs.o gccDebug_GLES2/source/graph/gles2/GraphicsDevice_GLES2.o gccDebug_GLES2/source/graph/gles2/GraphPerf_GLES2.o gccDebug_GLES2/source/graph/gles2/IndexBuffer_GLES2.o gccDebug_GLES2/source/graph/gles2/ParamValueConverter_GLES2.o gccDebug_GLES2/source/graph/gles2/PixelShader_GLES2.o gccDebug_GLES2/source/graph/gles2/RenderState_GLES2.o gccDebug_GLES2/source/graph/gles2/RenderTarget_GLES2.o gccDebug_GLES2/source/graph/gles2/ShaderProgram_GLES2.o gccDebug_GLES2/source/graph/gles2/Texture_GLES2.o gccDebug_GLES2/source/graph/gles2/VertexBuffer_GLES2.o gccDebug_GLES2/source/graph/gles2/VertexDeclaration_GLES2.o gccDebug_GLES2/source/graph/gles2/VertexShader_GLES2.o  $(Debug_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/graph/BufferBase.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/BufferBase.d
gccDebug_GLES2/source/graph/BufferBase.o: ../../source/graph/BufferBase.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/BufferBase.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/BufferBase.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/BufferBase.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/BufferBase.d

# Compiles file ../../source/graph/GraphicsDevice.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/GraphicsDevice.d
gccDebug_GLES2/source/graph/GraphicsDevice.o: ../../source/graph/GraphicsDevice.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/GraphicsDevice.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/GraphicsDevice.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/GraphicsDevice.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/GraphicsDevice.d

# Compiles file ../../source/graph/RenderState.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/RenderState.d
gccDebug_GLES2/source/graph/RenderState.o: ../../source/graph/RenderState.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/RenderState.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/RenderState.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/RenderState.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/RenderState.d

# Compiles file ../../source/graph/ShaderProgram.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/ShaderProgram.d
gccDebug_GLES2/source/graph/ShaderProgram.o: ../../source/graph/ShaderProgram.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/ShaderProgram.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/ShaderProgram.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/ShaderProgram.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/ShaderProgram.d

# Compiles file ../../source/graph/2d/2DShader.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/2d/2DShader.d
gccDebug_GLES2/source/graph/2d/2DShader.o: ../../source/graph/2d/2DShader.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/2d/2DShader.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/2d/2DShader.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/2d/2DShader.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/2d/2DShader.d

# Compiles file ../../source/graph/2d/2DRenderer.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/2d/2DRenderer.d
gccDebug_GLES2/source/graph/2d/2DRenderer.o: ../../source/graph/2d/2DRenderer.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/2d/2DRenderer.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/2d/2DRenderer.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/2d/2DRenderer.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/2d/2DRenderer.d

# Compiles file ../../source/graph/dx9/2DShader_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/2DShader_DX9.d
gccDebug_GLES2/source/graph/dx9/2DShader_DX9.o: ../../source/graph/dx9/2DShader_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/2DShader_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/2DShader_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/2DShader_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/2DShader_DX9.d

# Compiles file ../../source/graph/dx9/CubeTexture_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/CubeTexture_DX9.d
gccDebug_GLES2/source/graph/dx9/CubeTexture_DX9.o: ../../source/graph/dx9/CubeTexture_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/CubeTexture_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/CubeTexture_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/CubeTexture_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/CubeTexture_DX9.d

# Compiles file ../../source/graph/dx9/D3D9ParamValueConverter.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/D3D9ParamValueConverter.d
gccDebug_GLES2/source/graph/dx9/D3D9ParamValueConverter.o: ../../source/graph/dx9/D3D9ParamValueConverter.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/D3D9ParamValueConverter.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/D3D9ParamValueConverter.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/D3D9ParamValueConverter.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/D3D9ParamValueConverter.d

# Compiles file ../../source/graph/dx9/GraphicsDeviceResourceFuncs.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/GraphicsDeviceResourceFuncs.d
gccDebug_GLES2/source/graph/dx9/GraphicsDeviceResourceFuncs.o: ../../source/graph/dx9/GraphicsDeviceResourceFuncs.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/GraphicsDeviceResourceFuncs.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/GraphicsDeviceResourceFuncs.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/GraphicsDeviceResourceFuncs.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/GraphicsDeviceResourceFuncs.d

# Compiles file ../../source/graph/dx9/GraphicsDevice_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/GraphicsDevice_DX9.d
gccDebug_GLES2/source/graph/dx9/GraphicsDevice_DX9.o: ../../source/graph/dx9/GraphicsDevice_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/GraphicsDevice_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/GraphicsDevice_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/GraphicsDevice_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/GraphicsDevice_DX9.d

# Compiles file ../../source/graph/dx9/GraphPerf_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/GraphPerf_DX9.d
gccDebug_GLES2/source/graph/dx9/GraphPerf_DX9.o: ../../source/graph/dx9/GraphPerf_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/GraphPerf_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/GraphPerf_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/GraphPerf_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/GraphPerf_DX9.d

# Compiles file ../../source/graph/dx9/IndexBuffer_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/IndexBuffer_DX9.d
gccDebug_GLES2/source/graph/dx9/IndexBuffer_DX9.o: ../../source/graph/dx9/IndexBuffer_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/IndexBuffer_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/IndexBuffer_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/IndexBuffer_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/IndexBuffer_DX9.d

# Compiles file ../../source/graph/dx9/PixelShader_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/PixelShader_DX9.d
gccDebug_GLES2/source/graph/dx9/PixelShader_DX9.o: ../../source/graph/dx9/PixelShader_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/PixelShader_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/PixelShader_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/PixelShader_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/PixelShader_DX9.d

# Compiles file ../../source/graph/dx9/RenderState_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/RenderState_DX9.d
gccDebug_GLES2/source/graph/dx9/RenderState_DX9.o: ../../source/graph/dx9/RenderState_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/RenderState_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/RenderState_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/RenderState_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/RenderState_DX9.d

# Compiles file ../../source/graph/dx9/RenderTarget_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/RenderTarget_DX9.d
gccDebug_GLES2/source/graph/dx9/RenderTarget_DX9.o: ../../source/graph/dx9/RenderTarget_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/RenderTarget_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/RenderTarget_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/RenderTarget_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/RenderTarget_DX9.d

# Compiles file ../../source/graph/dx9/ShaderProgram_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/ShaderProgram_DX9.d
gccDebug_GLES2/source/graph/dx9/ShaderProgram_DX9.o: ../../source/graph/dx9/ShaderProgram_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/ShaderProgram_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/ShaderProgram_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/ShaderProgram_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/ShaderProgram_DX9.d

# Compiles file ../../source/graph/dx9/Surface_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/Surface_DX9.d
gccDebug_GLES2/source/graph/dx9/Surface_DX9.o: ../../source/graph/dx9/Surface_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/Surface_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/Surface_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/Surface_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/Surface_DX9.d

# Compiles file ../../source/graph/dx9/Texture_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/Texture_DX9.d
gccDebug_GLES2/source/graph/dx9/Texture_DX9.o: ../../source/graph/dx9/Texture_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/Texture_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/Texture_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/Texture_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/Texture_DX9.d

# Compiles file ../../source/graph/dx9/VertexBuffer_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/VertexBuffer_DX9.d
gccDebug_GLES2/source/graph/dx9/VertexBuffer_DX9.o: ../../source/graph/dx9/VertexBuffer_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/VertexBuffer_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/VertexBuffer_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/VertexBuffer_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/VertexBuffer_DX9.d

# Compiles file ../../source/graph/dx9/VertexDeclaration_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/VertexDeclaration_DX9.d
gccDebug_GLES2/source/graph/dx9/VertexDeclaration_DX9.o: ../../source/graph/dx9/VertexDeclaration_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/VertexDeclaration_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/VertexDeclaration_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/VertexDeclaration_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/VertexDeclaration_DX9.d

# Compiles file ../../source/graph/dx9/VertexShader_DX9.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/dx9/VertexShader_DX9.d
gccDebug_GLES2/source/graph/dx9/VertexShader_DX9.o: ../../source/graph/dx9/VertexShader_DX9.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/dx9/VertexShader_DX9.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/dx9/VertexShader_DX9.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/VertexShader_DX9.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/dx9/VertexShader_DX9.d

# Compiles file ../../source/graph/gles2/2DShader_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/2DShader_GLES2.d
gccDebug_GLES2/source/graph/gles2/2DShader_GLES2.o: ../../source/graph/gles2/2DShader_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/2DShader_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/2DShader_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/2DShader_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/2DShader_GLES2.d

# Compiles file ../../source/graph/gles2/CubeTexture_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/CubeTexture_GLES2.d
gccDebug_GLES2/source/graph/gles2/CubeTexture_GLES2.o: ../../source/graph/gles2/CubeTexture_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/CubeTexture_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/CubeTexture_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/CubeTexture_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/CubeTexture_GLES2.d

# Compiles file ../../source/graph/gles2/FrameBufferObject.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/FrameBufferObject.d
gccDebug_GLES2/source/graph/gles2/FrameBufferObject.o: ../../source/graph/gles2/FrameBufferObject.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/FrameBufferObject.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/FrameBufferObject.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/FrameBufferObject.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/FrameBufferObject.d

# Compiles file ../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/GraphicsDeviceResourceFuncs.d
gccDebug_GLES2/source/graph/gles2/GraphicsDeviceResourceFuncs.o: ../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/GraphicsDeviceResourceFuncs.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/GraphicsDeviceResourceFuncs.d

# Compiles file ../../source/graph/gles2/GraphicsDevice_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/GraphicsDevice_GLES2.d
gccDebug_GLES2/source/graph/gles2/GraphicsDevice_GLES2.o: ../../source/graph/gles2/GraphicsDevice_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/GraphicsDevice_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/GraphicsDevice_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/GraphicsDevice_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/GraphicsDevice_GLES2.d

# Compiles file ../../source/graph/gles2/GraphPerf_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/GraphPerf_GLES2.d
gccDebug_GLES2/source/graph/gles2/GraphPerf_GLES2.o: ../../source/graph/gles2/GraphPerf_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/GraphPerf_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/GraphPerf_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/GraphPerf_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/GraphPerf_GLES2.d

# Compiles file ../../source/graph/gles2/IndexBuffer_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/IndexBuffer_GLES2.d
gccDebug_GLES2/source/graph/gles2/IndexBuffer_GLES2.o: ../../source/graph/gles2/IndexBuffer_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/IndexBuffer_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/IndexBuffer_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/IndexBuffer_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/IndexBuffer_GLES2.d

# Compiles file ../../source/graph/gles2/ParamValueConverter_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/ParamValueConverter_GLES2.d
gccDebug_GLES2/source/graph/gles2/ParamValueConverter_GLES2.o: ../../source/graph/gles2/ParamValueConverter_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/ParamValueConverter_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/ParamValueConverter_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/ParamValueConverter_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/ParamValueConverter_GLES2.d

# Compiles file ../../source/graph/gles2/PixelShader_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/PixelShader_GLES2.d
gccDebug_GLES2/source/graph/gles2/PixelShader_GLES2.o: ../../source/graph/gles2/PixelShader_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/PixelShader_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/PixelShader_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/PixelShader_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/PixelShader_GLES2.d

# Compiles file ../../source/graph/gles2/RenderState_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/RenderState_GLES2.d
gccDebug_GLES2/source/graph/gles2/RenderState_GLES2.o: ../../source/graph/gles2/RenderState_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/RenderState_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/RenderState_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/RenderState_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/RenderState_GLES2.d

# Compiles file ../../source/graph/gles2/RenderTarget_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/RenderTarget_GLES2.d
gccDebug_GLES2/source/graph/gles2/RenderTarget_GLES2.o: ../../source/graph/gles2/RenderTarget_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/RenderTarget_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/RenderTarget_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/RenderTarget_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/RenderTarget_GLES2.d

# Compiles file ../../source/graph/gles2/ShaderProgram_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/ShaderProgram_GLES2.d
gccDebug_GLES2/source/graph/gles2/ShaderProgram_GLES2.o: ../../source/graph/gles2/ShaderProgram_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/ShaderProgram_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/ShaderProgram_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/ShaderProgram_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/ShaderProgram_GLES2.d

# Compiles file ../../source/graph/gles2/Texture_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/Texture_GLES2.d
gccDebug_GLES2/source/graph/gles2/Texture_GLES2.o: ../../source/graph/gles2/Texture_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/Texture_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/Texture_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/Texture_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/Texture_GLES2.d

# Compiles file ../../source/graph/gles2/VertexBuffer_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/VertexBuffer_GLES2.d
gccDebug_GLES2/source/graph/gles2/VertexBuffer_GLES2.o: ../../source/graph/gles2/VertexBuffer_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/VertexBuffer_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/VertexBuffer_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/VertexBuffer_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/VertexBuffer_GLES2.d

# Compiles file ../../source/graph/gles2/VertexDeclaration_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/VertexDeclaration_GLES2.d
gccDebug_GLES2/source/graph/gles2/VertexDeclaration_GLES2.o: ../../source/graph/gles2/VertexDeclaration_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/VertexDeclaration_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/VertexDeclaration_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/VertexDeclaration_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/VertexDeclaration_GLES2.d

# Compiles file ../../source/graph/gles2/VertexShader_GLES2.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/graph/gles2/VertexShader_GLES2.d
gccDebug_GLES2/source/graph/gles2/VertexShader_GLES2.o: ../../source/graph/gles2/VertexShader_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/graph/gles2/VertexShader_GLES2.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/graph/gles2/VertexShader_GLES2.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/VertexShader_GLES2.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/graph/gles2/VertexShader_GLES2.d

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders gccRelease_GLES2/source/graph/BufferBase.o gccRelease_GLES2/source/graph/GraphicsDevice.o gccRelease_GLES2/source/graph/RenderState.o gccRelease_GLES2/source/graph/ShaderProgram.o gccRelease_GLES2/source/graph/2d/2DShader.o gccRelease_GLES2/source/graph/2d/2DRenderer.o gccRelease_GLES2/source/graph/dx9/2DShader_DX9.o gccRelease_GLES2/source/graph/dx9/CubeTexture_DX9.o gccRelease_GLES2/source/graph/dx9/D3D9ParamValueConverter.o gccRelease_GLES2/source/graph/dx9/GraphicsDeviceResourceFuncs.o gccRelease_GLES2/source/graph/dx9/GraphicsDevice_DX9.o gccRelease_GLES2/source/graph/dx9/GraphPerf_DX9.o gccRelease_GLES2/source/graph/dx9/IndexBuffer_DX9.o gccRelease_GLES2/source/graph/dx9/PixelShader_DX9.o gccRelease_GLES2/source/graph/dx9/RenderState_DX9.o gccRelease_GLES2/source/graph/dx9/RenderTarget_DX9.o gccRelease_GLES2/source/graph/dx9/ShaderProgram_DX9.o gccRelease_GLES2/source/graph/dx9/Surface_DX9.o gccRelease_GLES2/source/graph/dx9/Texture_DX9.o gccRelease_GLES2/source/graph/dx9/VertexBuffer_DX9.o gccRelease_GLES2/source/graph/dx9/VertexDeclaration_DX9.o gccRelease_GLES2/source/graph/dx9/VertexShader_DX9.o gccRelease_GLES2/source/graph/gles2/2DShader_GLES2.o gccRelease_GLES2/source/graph/gles2/CubeTexture_GLES2.o gccRelease_GLES2/source/graph/gles2/FrameBufferObject.o gccRelease_GLES2/source/graph/gles2/GraphicsDeviceResourceFuncs.o gccRelease_GLES2/source/graph/gles2/GraphicsDevice_GLES2.o gccRelease_GLES2/source/graph/gles2/GraphPerf_GLES2.o gccRelease_GLES2/source/graph/gles2/IndexBuffer_GLES2.o gccRelease_GLES2/source/graph/gles2/ParamValueConverter_GLES2.o gccRelease_GLES2/source/graph/gles2/PixelShader_GLES2.o gccRelease_GLES2/source/graph/gles2/RenderState_GLES2.o gccRelease_GLES2/source/graph/gles2/RenderTarget_GLES2.o gccRelease_GLES2/source/graph/gles2/ShaderProgram_GLES2.o gccRelease_GLES2/source/graph/gles2/Texture_GLES2.o gccRelease_GLES2/source/graph/gles2/VertexBuffer_GLES2.o gccRelease_GLES2/source/graph/gles2/VertexDeclaration_GLES2.o gccRelease_GLES2/source/graph/gles2/VertexShader_GLES2.o 
	ar rcs ../lib/gccRelease_GLES2/libGraph.a gccRelease_GLES2/source/graph/BufferBase.o gccRelease_GLES2/source/graph/GraphicsDevice.o gccRelease_GLES2/source/graph/RenderState.o gccRelease_GLES2/source/graph/ShaderProgram.o gccRelease_GLES2/source/graph/2d/2DShader.o gccRelease_GLES2/source/graph/2d/2DRenderer.o gccRelease_GLES2/source/graph/dx9/2DShader_DX9.o gccRelease_GLES2/source/graph/dx9/CubeTexture_DX9.o gccRelease_GLES2/source/graph/dx9/D3D9ParamValueConverter.o gccRelease_GLES2/source/graph/dx9/GraphicsDeviceResourceFuncs.o gccRelease_GLES2/source/graph/dx9/GraphicsDevice_DX9.o gccRelease_GLES2/source/graph/dx9/GraphPerf_DX9.o gccRelease_GLES2/source/graph/dx9/IndexBuffer_DX9.o gccRelease_GLES2/source/graph/dx9/PixelShader_DX9.o gccRelease_GLES2/source/graph/dx9/RenderState_DX9.o gccRelease_GLES2/source/graph/dx9/RenderTarget_DX9.o gccRelease_GLES2/source/graph/dx9/ShaderProgram_DX9.o gccRelease_GLES2/source/graph/dx9/Surface_DX9.o gccRelease_GLES2/source/graph/dx9/Texture_DX9.o gccRelease_GLES2/source/graph/dx9/VertexBuffer_DX9.o gccRelease_GLES2/source/graph/dx9/VertexDeclaration_DX9.o gccRelease_GLES2/source/graph/dx9/VertexShader_DX9.o gccRelease_GLES2/source/graph/gles2/2DShader_GLES2.o gccRelease_GLES2/source/graph/gles2/CubeTexture_GLES2.o gccRelease_GLES2/source/graph/gles2/FrameBufferObject.o gccRelease_GLES2/source/graph/gles2/GraphicsDeviceResourceFuncs.o gccRelease_GLES2/source/graph/gles2/GraphicsDevice_GLES2.o gccRelease_GLES2/source/graph/gles2/GraphPerf_GLES2.o gccRelease_GLES2/source/graph/gles2/IndexBuffer_GLES2.o gccRelease_GLES2/source/graph/gles2/ParamValueConverter_GLES2.o gccRelease_GLES2/source/graph/gles2/PixelShader_GLES2.o gccRelease_GLES2/source/graph/gles2/RenderState_GLES2.o gccRelease_GLES2/source/graph/gles2/RenderTarget_GLES2.o gccRelease_GLES2/source/graph/gles2/ShaderProgram_GLES2.o gccRelease_GLES2/source/graph/gles2/Texture_GLES2.o gccRelease_GLES2/source/graph/gles2/VertexBuffer_GLES2.o gccRelease_GLES2/source/graph/gles2/VertexDeclaration_GLES2.o gccRelease_GLES2/source/graph/gles2/VertexShader_GLES2.o  $(Release_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/graph/BufferBase.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/BufferBase.d
gccRelease_GLES2/source/graph/BufferBase.o: ../../source/graph/BufferBase.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/BufferBase.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/BufferBase.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/BufferBase.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/BufferBase.d

# Compiles file ../../source/graph/GraphicsDevice.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/GraphicsDevice.d
gccRelease_GLES2/source/graph/GraphicsDevice.o: ../../source/graph/GraphicsDevice.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/GraphicsDevice.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/GraphicsDevice.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/GraphicsDevice.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/GraphicsDevice.d

# Compiles file ../../source/graph/RenderState.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/RenderState.d
gccRelease_GLES2/source/graph/RenderState.o: ../../source/graph/RenderState.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/RenderState.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/RenderState.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/RenderState.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/RenderState.d

# Compiles file ../../source/graph/ShaderProgram.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/ShaderProgram.d
gccRelease_GLES2/source/graph/ShaderProgram.o: ../../source/graph/ShaderProgram.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/ShaderProgram.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/ShaderProgram.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/ShaderProgram.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/ShaderProgram.d

# Compiles file ../../source/graph/2d/2DShader.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/2d/2DShader.d
gccRelease_GLES2/source/graph/2d/2DShader.o: ../../source/graph/2d/2DShader.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/2d/2DShader.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/2d/2DShader.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/2d/2DShader.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/2d/2DShader.d

# Compiles file ../../source/graph/2d/2DRenderer.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/2d/2DRenderer.d
gccRelease_GLES2/source/graph/2d/2DRenderer.o: ../../source/graph/2d/2DRenderer.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/2d/2DRenderer.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/2d/2DRenderer.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/2d/2DRenderer.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/2d/2DRenderer.d

# Compiles file ../../source/graph/dx9/2DShader_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/2DShader_DX9.d
gccRelease_GLES2/source/graph/dx9/2DShader_DX9.o: ../../source/graph/dx9/2DShader_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/2DShader_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/2DShader_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/2DShader_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/2DShader_DX9.d

# Compiles file ../../source/graph/dx9/CubeTexture_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/CubeTexture_DX9.d
gccRelease_GLES2/source/graph/dx9/CubeTexture_DX9.o: ../../source/graph/dx9/CubeTexture_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/CubeTexture_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/CubeTexture_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/CubeTexture_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/CubeTexture_DX9.d

# Compiles file ../../source/graph/dx9/D3D9ParamValueConverter.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/D3D9ParamValueConverter.d
gccRelease_GLES2/source/graph/dx9/D3D9ParamValueConverter.o: ../../source/graph/dx9/D3D9ParamValueConverter.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/D3D9ParamValueConverter.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/D3D9ParamValueConverter.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/D3D9ParamValueConverter.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/D3D9ParamValueConverter.d

# Compiles file ../../source/graph/dx9/GraphicsDeviceResourceFuncs.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/GraphicsDeviceResourceFuncs.d
gccRelease_GLES2/source/graph/dx9/GraphicsDeviceResourceFuncs.o: ../../source/graph/dx9/GraphicsDeviceResourceFuncs.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/GraphicsDeviceResourceFuncs.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/GraphicsDeviceResourceFuncs.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/GraphicsDeviceResourceFuncs.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/GraphicsDeviceResourceFuncs.d

# Compiles file ../../source/graph/dx9/GraphicsDevice_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/GraphicsDevice_DX9.d
gccRelease_GLES2/source/graph/dx9/GraphicsDevice_DX9.o: ../../source/graph/dx9/GraphicsDevice_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/GraphicsDevice_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/GraphicsDevice_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/GraphicsDevice_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/GraphicsDevice_DX9.d

# Compiles file ../../source/graph/dx9/GraphPerf_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/GraphPerf_DX9.d
gccRelease_GLES2/source/graph/dx9/GraphPerf_DX9.o: ../../source/graph/dx9/GraphPerf_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/GraphPerf_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/GraphPerf_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/GraphPerf_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/GraphPerf_DX9.d

# Compiles file ../../source/graph/dx9/IndexBuffer_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/IndexBuffer_DX9.d
gccRelease_GLES2/source/graph/dx9/IndexBuffer_DX9.o: ../../source/graph/dx9/IndexBuffer_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/IndexBuffer_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/IndexBuffer_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/IndexBuffer_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/IndexBuffer_DX9.d

# Compiles file ../../source/graph/dx9/PixelShader_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/PixelShader_DX9.d
gccRelease_GLES2/source/graph/dx9/PixelShader_DX9.o: ../../source/graph/dx9/PixelShader_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/PixelShader_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/PixelShader_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/PixelShader_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/PixelShader_DX9.d

# Compiles file ../../source/graph/dx9/RenderState_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/RenderState_DX9.d
gccRelease_GLES2/source/graph/dx9/RenderState_DX9.o: ../../source/graph/dx9/RenderState_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/RenderState_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/RenderState_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/RenderState_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/RenderState_DX9.d

# Compiles file ../../source/graph/dx9/RenderTarget_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/RenderTarget_DX9.d
gccRelease_GLES2/source/graph/dx9/RenderTarget_DX9.o: ../../source/graph/dx9/RenderTarget_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/RenderTarget_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/RenderTarget_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/RenderTarget_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/RenderTarget_DX9.d

# Compiles file ../../source/graph/dx9/ShaderProgram_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/ShaderProgram_DX9.d
gccRelease_GLES2/source/graph/dx9/ShaderProgram_DX9.o: ../../source/graph/dx9/ShaderProgram_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/ShaderProgram_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/ShaderProgram_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/ShaderProgram_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/ShaderProgram_DX9.d

# Compiles file ../../source/graph/dx9/Surface_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/Surface_DX9.d
gccRelease_GLES2/source/graph/dx9/Surface_DX9.o: ../../source/graph/dx9/Surface_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/Surface_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/Surface_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/Surface_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/Surface_DX9.d

# Compiles file ../../source/graph/dx9/Texture_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/Texture_DX9.d
gccRelease_GLES2/source/graph/dx9/Texture_DX9.o: ../../source/graph/dx9/Texture_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/Texture_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/Texture_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/Texture_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/Texture_DX9.d

# Compiles file ../../source/graph/dx9/VertexBuffer_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/VertexBuffer_DX9.d
gccRelease_GLES2/source/graph/dx9/VertexBuffer_DX9.o: ../../source/graph/dx9/VertexBuffer_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/VertexBuffer_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/VertexBuffer_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/VertexBuffer_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/VertexBuffer_DX9.d

# Compiles file ../../source/graph/dx9/VertexDeclaration_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/VertexDeclaration_DX9.d
gccRelease_GLES2/source/graph/dx9/VertexDeclaration_DX9.o: ../../source/graph/dx9/VertexDeclaration_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/VertexDeclaration_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/VertexDeclaration_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/VertexDeclaration_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/VertexDeclaration_DX9.d

# Compiles file ../../source/graph/dx9/VertexShader_DX9.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/dx9/VertexShader_DX9.d
gccRelease_GLES2/source/graph/dx9/VertexShader_DX9.o: ../../source/graph/dx9/VertexShader_DX9.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/dx9/VertexShader_DX9.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/dx9/VertexShader_DX9.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/dx9/VertexShader_DX9.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/dx9/VertexShader_DX9.d

# Compiles file ../../source/graph/gles2/2DShader_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/2DShader_GLES2.d
gccRelease_GLES2/source/graph/gles2/2DShader_GLES2.o: ../../source/graph/gles2/2DShader_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/2DShader_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/2DShader_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/2DShader_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/2DShader_GLES2.d

# Compiles file ../../source/graph/gles2/CubeTexture_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/CubeTexture_GLES2.d
gccRelease_GLES2/source/graph/gles2/CubeTexture_GLES2.o: ../../source/graph/gles2/CubeTexture_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/CubeTexture_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/CubeTexture_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/CubeTexture_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/CubeTexture_GLES2.d

# Compiles file ../../source/graph/gles2/FrameBufferObject.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/FrameBufferObject.d
gccRelease_GLES2/source/graph/gles2/FrameBufferObject.o: ../../source/graph/gles2/FrameBufferObject.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/FrameBufferObject.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/FrameBufferObject.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/FrameBufferObject.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/FrameBufferObject.d

# Compiles file ../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/GraphicsDeviceResourceFuncs.d
gccRelease_GLES2/source/graph/gles2/GraphicsDeviceResourceFuncs.o: ../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/GraphicsDeviceResourceFuncs.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/GraphicsDeviceResourceFuncs.d

# Compiles file ../../source/graph/gles2/GraphicsDevice_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/GraphicsDevice_GLES2.d
gccRelease_GLES2/source/graph/gles2/GraphicsDevice_GLES2.o: ../../source/graph/gles2/GraphicsDevice_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/GraphicsDevice_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/GraphicsDevice_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/GraphicsDevice_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/GraphicsDevice_GLES2.d

# Compiles file ../../source/graph/gles2/GraphPerf_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/GraphPerf_GLES2.d
gccRelease_GLES2/source/graph/gles2/GraphPerf_GLES2.o: ../../source/graph/gles2/GraphPerf_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/GraphPerf_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/GraphPerf_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/GraphPerf_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/GraphPerf_GLES2.d

# Compiles file ../../source/graph/gles2/IndexBuffer_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/IndexBuffer_GLES2.d
gccRelease_GLES2/source/graph/gles2/IndexBuffer_GLES2.o: ../../source/graph/gles2/IndexBuffer_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/IndexBuffer_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/IndexBuffer_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/IndexBuffer_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/IndexBuffer_GLES2.d

# Compiles file ../../source/graph/gles2/ParamValueConverter_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/ParamValueConverter_GLES2.d
gccRelease_GLES2/source/graph/gles2/ParamValueConverter_GLES2.o: ../../source/graph/gles2/ParamValueConverter_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/ParamValueConverter_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/ParamValueConverter_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/ParamValueConverter_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/ParamValueConverter_GLES2.d

# Compiles file ../../source/graph/gles2/PixelShader_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/PixelShader_GLES2.d
gccRelease_GLES2/source/graph/gles2/PixelShader_GLES2.o: ../../source/graph/gles2/PixelShader_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/PixelShader_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/PixelShader_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/PixelShader_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/PixelShader_GLES2.d

# Compiles file ../../source/graph/gles2/RenderState_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/RenderState_GLES2.d
gccRelease_GLES2/source/graph/gles2/RenderState_GLES2.o: ../../source/graph/gles2/RenderState_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/RenderState_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/RenderState_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/RenderState_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/RenderState_GLES2.d

# Compiles file ../../source/graph/gles2/RenderTarget_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/RenderTarget_GLES2.d
gccRelease_GLES2/source/graph/gles2/RenderTarget_GLES2.o: ../../source/graph/gles2/RenderTarget_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/RenderTarget_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/RenderTarget_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/RenderTarget_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/RenderTarget_GLES2.d

# Compiles file ../../source/graph/gles2/ShaderProgram_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/ShaderProgram_GLES2.d
gccRelease_GLES2/source/graph/gles2/ShaderProgram_GLES2.o: ../../source/graph/gles2/ShaderProgram_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/ShaderProgram_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/ShaderProgram_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/ShaderProgram_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/ShaderProgram_GLES2.d

# Compiles file ../../source/graph/gles2/Texture_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/Texture_GLES2.d
gccRelease_GLES2/source/graph/gles2/Texture_GLES2.o: ../../source/graph/gles2/Texture_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/Texture_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/Texture_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/Texture_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/Texture_GLES2.d

# Compiles file ../../source/graph/gles2/VertexBuffer_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/VertexBuffer_GLES2.d
gccRelease_GLES2/source/graph/gles2/VertexBuffer_GLES2.o: ../../source/graph/gles2/VertexBuffer_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/VertexBuffer_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/VertexBuffer_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/VertexBuffer_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/VertexBuffer_GLES2.d

# Compiles file ../../source/graph/gles2/VertexDeclaration_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/VertexDeclaration_GLES2.d
gccRelease_GLES2/source/graph/gles2/VertexDeclaration_GLES2.o: ../../source/graph/gles2/VertexDeclaration_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/VertexDeclaration_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/VertexDeclaration_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/VertexDeclaration_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/VertexDeclaration_GLES2.d

# Compiles file ../../source/graph/gles2/VertexShader_GLES2.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/graph/gles2/VertexShader_GLES2.d
gccRelease_GLES2/source/graph/gles2/VertexShader_GLES2.o: ../../source/graph/gles2/VertexShader_GLES2.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/graph/gles2/VertexShader_GLES2.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/graph/gles2/VertexShader_GLES2.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/graph/gles2/VertexShader_GLES2.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/graph/gles2/VertexShader_GLES2.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLES2/source/graph
	mkdir -p gccDebug_GLES2/source/graph/2d
	mkdir -p gccDebug_GLES2/source/graph/dx9
	mkdir -p gccDebug_GLES2/source/graph/gles2
	mkdir -p ../lib/gccDebug_GLES2
	mkdir -p gccRelease_GLES2/source/graph
	mkdir -p gccRelease_GLES2/source/graph/2d
	mkdir -p gccRelease_GLES2/source/graph/dx9
	mkdir -p gccRelease_GLES2/source/graph/gles2
	mkdir -p ../lib/gccRelease_GLES2

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLES2/source/graph/*.o
	rm -f gccDebug_GLES2/source/graph/*.d
	rm -f gccDebug_GLES2/source/graph/2d/*.o
	rm -f gccDebug_GLES2/source/graph/2d/*.d
	rm -f gccDebug_GLES2/source/graph/dx9/*.o
	rm -f gccDebug_GLES2/source/graph/dx9/*.d
	rm -f gccDebug_GLES2/source/graph/gles2/*.o
	rm -f gccDebug_GLES2/source/graph/gles2/*.d
	rm -f ../lib/gccDebug_GLES2/libGraph.a
	rm -f gccRelease_GLES2/source/graph/*.o
	rm -f gccRelease_GLES2/source/graph/*.d
	rm -f gccRelease_GLES2/source/graph/2d/*.o
	rm -f gccRelease_GLES2/source/graph/2d/*.d
	rm -f gccRelease_GLES2/source/graph/dx9/*.o
	rm -f gccRelease_GLES2/source/graph/dx9/*.d
	rm -f gccRelease_GLES2/source/graph/gles2/*.o
	rm -f gccRelease_GLES2/source/graph/gles2/*.d
	rm -f ../lib/gccRelease_GLES2/libGraph.a

