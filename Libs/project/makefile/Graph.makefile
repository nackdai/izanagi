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
Debug_GLUT: create_folders gccDebug_GLUT/source/graph/BufferBase.o gccDebug_GLUT/source/graph/GraphicsDevice.o gccDebug_GLUT/source/graph/RenderState.o gccDebug_GLUT/source/graph/ShaderProgram.o gccDebug_GLUT/source/graph/2d/2DShader.o gccDebug_GLUT/source/graph/2d/2DRenderer.o gccDebug_GLUT/source/graph/gles2/2DShader_GLES2.o gccDebug_GLUT/source/graph/gles2/CubeTexture_GLES2.o gccDebug_GLUT/source/graph/gles2/FrameBufferObject.o gccDebug_GLUT/source/graph/gles2/GraphicsDeviceResourceFuncs.o gccDebug_GLUT/source/graph/gles2/GraphicsDevice_GLES2.o gccDebug_GLUT/source/graph/gles2/GraphPerf_GLES2.o gccDebug_GLUT/source/graph/gles2/IndexBuffer_GLES2.o gccDebug_GLUT/source/graph/gles2/ParamValueConverter_GLES2.o gccDebug_GLUT/source/graph/gles2/PixelShader_GLES2.o gccDebug_GLUT/source/graph/gles2/RenderState_GLES2.o gccDebug_GLUT/source/graph/gles2/RenderTarget_GLES2.o gccDebug_GLUT/source/graph/gles2/ShaderProgram_GLES2.o gccDebug_GLUT/source/graph/gles2/Texture_GLES2.o gccDebug_GLUT/source/graph/gles2/VertexBuffer_GLES2.o gccDebug_GLUT/source/graph/gles2/VertexDeclaration_GLES2.o gccDebug_GLUT/source/graph/gles2/VertexShader_GLES2.o gccDebug_GLUT/source/graph/ogl/GraphicsDevice_OGL.o 
	ar rcs ../lib/gccDebug_GLUT/libGraph.a gccDebug_GLUT/source/graph/BufferBase.o gccDebug_GLUT/source/graph/GraphicsDevice.o gccDebug_GLUT/source/graph/RenderState.o gccDebug_GLUT/source/graph/ShaderProgram.o gccDebug_GLUT/source/graph/2d/2DShader.o gccDebug_GLUT/source/graph/2d/2DRenderer.o gccDebug_GLUT/source/graph/gles2/2DShader_GLES2.o gccDebug_GLUT/source/graph/gles2/CubeTexture_GLES2.o gccDebug_GLUT/source/graph/gles2/FrameBufferObject.o gccDebug_GLUT/source/graph/gles2/GraphicsDeviceResourceFuncs.o gccDebug_GLUT/source/graph/gles2/GraphicsDevice_GLES2.o gccDebug_GLUT/source/graph/gles2/GraphPerf_GLES2.o gccDebug_GLUT/source/graph/gles2/IndexBuffer_GLES2.o gccDebug_GLUT/source/graph/gles2/ParamValueConverter_GLES2.o gccDebug_GLUT/source/graph/gles2/PixelShader_GLES2.o gccDebug_GLUT/source/graph/gles2/RenderState_GLES2.o gccDebug_GLUT/source/graph/gles2/RenderTarget_GLES2.o gccDebug_GLUT/source/graph/gles2/ShaderProgram_GLES2.o gccDebug_GLUT/source/graph/gles2/Texture_GLES2.o gccDebug_GLUT/source/graph/gles2/VertexBuffer_GLES2.o gccDebug_GLUT/source/graph/gles2/VertexDeclaration_GLES2.o gccDebug_GLUT/source/graph/gles2/VertexShader_GLES2.o gccDebug_GLUT/source/graph/ogl/GraphicsDevice_OGL.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../source/graph/BufferBase.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/BufferBase.d
gccDebug_GLUT/source/graph/BufferBase.o: ../../source/graph/BufferBase.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/BufferBase.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/BufferBase.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/BufferBase.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/BufferBase.d

# Compiles file ../../source/graph/GraphicsDevice.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/GraphicsDevice.d
gccDebug_GLUT/source/graph/GraphicsDevice.o: ../../source/graph/GraphicsDevice.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/GraphicsDevice.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/GraphicsDevice.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/GraphicsDevice.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/GraphicsDevice.d

# Compiles file ../../source/graph/RenderState.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/RenderState.d
gccDebug_GLUT/source/graph/RenderState.o: ../../source/graph/RenderState.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/RenderState.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/RenderState.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/RenderState.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/RenderState.d

# Compiles file ../../source/graph/ShaderProgram.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/ShaderProgram.d
gccDebug_GLUT/source/graph/ShaderProgram.o: ../../source/graph/ShaderProgram.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/ShaderProgram.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/ShaderProgram.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/ShaderProgram.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/ShaderProgram.d

# Compiles file ../../source/graph/2d/2DShader.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/2d/2DShader.d
gccDebug_GLUT/source/graph/2d/2DShader.o: ../../source/graph/2d/2DShader.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/2d/2DShader.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/2d/2DShader.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/2d/2DShader.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/2d/2DShader.d

# Compiles file ../../source/graph/2d/2DRenderer.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/2d/2DRenderer.d
gccDebug_GLUT/source/graph/2d/2DRenderer.o: ../../source/graph/2d/2DRenderer.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/2d/2DRenderer.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/2d/2DRenderer.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/2d/2DRenderer.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/2d/2DRenderer.d

# Compiles file ../../source/graph/gles2/2DShader_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/2DShader_GLES2.d
gccDebug_GLUT/source/graph/gles2/2DShader_GLES2.o: ../../source/graph/gles2/2DShader_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/2DShader_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/2DShader_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/2DShader_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/2DShader_GLES2.d

# Compiles file ../../source/graph/gles2/CubeTexture_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/CubeTexture_GLES2.d
gccDebug_GLUT/source/graph/gles2/CubeTexture_GLES2.o: ../../source/graph/gles2/CubeTexture_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/CubeTexture_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/CubeTexture_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/CubeTexture_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/CubeTexture_GLES2.d

# Compiles file ../../source/graph/gles2/FrameBufferObject.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/FrameBufferObject.d
gccDebug_GLUT/source/graph/gles2/FrameBufferObject.o: ../../source/graph/gles2/FrameBufferObject.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/FrameBufferObject.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/FrameBufferObject.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/FrameBufferObject.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/FrameBufferObject.d

# Compiles file ../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/GraphicsDeviceResourceFuncs.d
gccDebug_GLUT/source/graph/gles2/GraphicsDeviceResourceFuncs.o: ../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/GraphicsDeviceResourceFuncs.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/GraphicsDeviceResourceFuncs.d

# Compiles file ../../source/graph/gles2/GraphicsDevice_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/GraphicsDevice_GLES2.d
gccDebug_GLUT/source/graph/gles2/GraphicsDevice_GLES2.o: ../../source/graph/gles2/GraphicsDevice_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/GraphicsDevice_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/GraphicsDevice_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/GraphicsDevice_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/GraphicsDevice_GLES2.d

# Compiles file ../../source/graph/gles2/GraphPerf_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/GraphPerf_GLES2.d
gccDebug_GLUT/source/graph/gles2/GraphPerf_GLES2.o: ../../source/graph/gles2/GraphPerf_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/GraphPerf_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/GraphPerf_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/GraphPerf_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/GraphPerf_GLES2.d

# Compiles file ../../source/graph/gles2/IndexBuffer_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/IndexBuffer_GLES2.d
gccDebug_GLUT/source/graph/gles2/IndexBuffer_GLES2.o: ../../source/graph/gles2/IndexBuffer_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/IndexBuffer_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/IndexBuffer_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/IndexBuffer_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/IndexBuffer_GLES2.d

# Compiles file ../../source/graph/gles2/ParamValueConverter_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/ParamValueConverter_GLES2.d
gccDebug_GLUT/source/graph/gles2/ParamValueConverter_GLES2.o: ../../source/graph/gles2/ParamValueConverter_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/ParamValueConverter_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/ParamValueConverter_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/ParamValueConverter_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/ParamValueConverter_GLES2.d

# Compiles file ../../source/graph/gles2/PixelShader_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/PixelShader_GLES2.d
gccDebug_GLUT/source/graph/gles2/PixelShader_GLES2.o: ../../source/graph/gles2/PixelShader_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/PixelShader_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/PixelShader_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/PixelShader_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/PixelShader_GLES2.d

# Compiles file ../../source/graph/gles2/RenderState_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/RenderState_GLES2.d
gccDebug_GLUT/source/graph/gles2/RenderState_GLES2.o: ../../source/graph/gles2/RenderState_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/RenderState_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/RenderState_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/RenderState_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/RenderState_GLES2.d

# Compiles file ../../source/graph/gles2/RenderTarget_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/RenderTarget_GLES2.d
gccDebug_GLUT/source/graph/gles2/RenderTarget_GLES2.o: ../../source/graph/gles2/RenderTarget_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/RenderTarget_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/RenderTarget_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/RenderTarget_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/RenderTarget_GLES2.d

# Compiles file ../../source/graph/gles2/ShaderProgram_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/ShaderProgram_GLES2.d
gccDebug_GLUT/source/graph/gles2/ShaderProgram_GLES2.o: ../../source/graph/gles2/ShaderProgram_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/ShaderProgram_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/ShaderProgram_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/ShaderProgram_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/ShaderProgram_GLES2.d

# Compiles file ../../source/graph/gles2/Texture_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/Texture_GLES2.d
gccDebug_GLUT/source/graph/gles2/Texture_GLES2.o: ../../source/graph/gles2/Texture_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/Texture_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/Texture_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/Texture_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/Texture_GLES2.d

# Compiles file ../../source/graph/gles2/VertexBuffer_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/VertexBuffer_GLES2.d
gccDebug_GLUT/source/graph/gles2/VertexBuffer_GLES2.o: ../../source/graph/gles2/VertexBuffer_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/VertexBuffer_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/VertexBuffer_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/VertexBuffer_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/VertexBuffer_GLES2.d

# Compiles file ../../source/graph/gles2/VertexDeclaration_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/VertexDeclaration_GLES2.d
gccDebug_GLUT/source/graph/gles2/VertexDeclaration_GLES2.o: ../../source/graph/gles2/VertexDeclaration_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/VertexDeclaration_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/VertexDeclaration_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/VertexDeclaration_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/VertexDeclaration_GLES2.d

# Compiles file ../../source/graph/gles2/VertexShader_GLES2.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/gles2/VertexShader_GLES2.d
gccDebug_GLUT/source/graph/gles2/VertexShader_GLES2.o: ../../source/graph/gles2/VertexShader_GLES2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/gles2/VertexShader_GLES2.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/gles2/VertexShader_GLES2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/gles2/VertexShader_GLES2.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/gles2/VertexShader_GLES2.d

# Compiles file ../../source/graph/ogl/GraphicsDevice_OGL.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/graph/ogl/GraphicsDevice_OGL.d
gccDebug_GLUT/source/graph/ogl/GraphicsDevice_OGL.o: ../../source/graph/ogl/GraphicsDevice_OGL.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/graph/ogl/GraphicsDevice_OGL.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/graph/ogl/GraphicsDevice_OGL.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/graph/ogl/GraphicsDevice_OGL.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/graph/ogl/GraphicsDevice_OGL.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLUT/source/graph
	mkdir -p gccDebug_GLUT/source/graph/2d
	mkdir -p gccDebug_GLUT/source/graph/gles2
	mkdir -p gccDebug_GLUT/source/graph/ogl
	mkdir -p ../lib/gccDebug_GLUT

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLUT/source/graph/*.o
	rm -f gccDebug_GLUT/source/graph/*.d
	rm -f gccDebug_GLUT/source/graph/2d/*.o
	rm -f gccDebug_GLUT/source/graph/2d/*.d
	rm -f gccDebug_GLUT/source/graph/gles2/*.o
	rm -f gccDebug_GLUT/source/graph/gles2/*.d
	rm -f gccDebug_GLUT/source/graph/ogl/*.o
	rm -f gccDebug_GLUT/source/graph/ogl/*.d
	rm -f ../lib/gccDebug_GLUT/libGraph.a

