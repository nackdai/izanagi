#if !defined(__URANUS_GRAPH_H__)
#define __URANUS_GRAPH_H__

#if defined(__UN_DX9__)
	#include "unD3DDefs.h"

	#include "graph/dx9/GraphicsDevice.h"
	#include "graph/dx9/BaseTexture.h"
	#include "graph/dx9/Texture.h"
	#include "graph/dx9/CubeTexture.h"
	#include "graph/dx9/Surface.h"
	#include "graph/dx9/VertexBuffer.h"
	#include "graph/dx9/IndexBuffer.h"
	#include "graph/dx9/VertexShader.h"
	#include "graph/dx9/PixelShader.h"
	#include "graph/dx9/VertexDeclaration.h"

	#include "graph/dx9/GraphPerf.h"

	#include "graph/dx9/FxShader.h"
#elif defined(__UN_OGL__)
#endif

#include "graph/GraphDefs.h"
#include "graph/GraphicsDeviceUtil.h"

#endif	// #if !defined(__URANUS_GRAPH_H__)
