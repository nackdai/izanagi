#if !defined(__UN_GRAPH_OGL_EXT_FUNC_PROXY_H__)
#define __UN_GRAPH_OGL_EXT_FUNC_PROXY_H__

#include "unDefs.h"
#include "unD3DDefs.h"
#include "graph/ogl/glext.h"

#define UN_OGL_DEF_EXT_PROC(type, name)		static type _##name
#define UN_OGL_CALL_EXT_PROC(type, name)	if (_##name == UN_NULL) { _##name = (type)wglGetProcAddress(#name); } (*_##name)

namespace uranus {
	class COGLExtFuncProxy {
	private:
		COGLExtFuncProxy();
		~COGLExtFuncProxy();

	private:
		UN_OGL_DEF_EXT_PROC(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);
		UN_OGL_DEF_EXT_PROC(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
		UN_OGL_DEF_EXT_PROC(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
		UN_OGL_DEF_EXT_PROC(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers);
		UN_OGL_DEF_EXT_PROC(PFNGLGENBUFFERSPROC, glGenBuffers);
		UN_OGL_DEF_EXT_PROC(PFNGLBUFFERDATAPROC, glBufferData);
		UN_OGL_DEF_EXT_PROC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
		UN_OGL_DEF_EXT_PROC(PFNGLGETBUFFERSUBDATAPROC, glGetBufferSubData);
		UN_OGL_DEF_EXT_PROC(PFNGLBINDBUFFERPROC, glBindBuffer);
		UN_OGL_DEF_EXT_PROC(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers);
		UN_OGL_DEF_EXT_PROC(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers);
		UN_OGL_DEF_EXT_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
		UN_OGL_DEF_EXT_PROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);

	public:
		static void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D)(target, attachment, textarget, texture, level);
		}

		static void glGenFramebuffers(GLsizei n, GLuint *framebuffers)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers)(n, framebuffers);
		}

		static void glBindFramebuffer(GLenum target, GLuint framebuffer)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer)(target, framebuffer);
		}

		static void glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers)(n, framebuffers);
		}

		static void glGenBuffers(GLsizei n, GLuint *buffers)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLGENBUFFERSPROC, glGenBuffers)(n, buffers);
		}

		static void glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLBUFFERDATAPROC, glBufferData)(target, size, data, usage);
		}

		static void glDeleteBuffers(GLsizei n, const GLuint *buffers)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers)(n, buffers);
		}

		static void glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLGETBUFFERSUBDATAPROC, glGetBufferSubData)(target, offset, size, data);
		}

		static void glBindBuffer(GLenum target, GLuint buffer)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLBINDBUFFERPROC, glBindBuffer)(target, buffer);
		}

		static void glGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers)(n, renderbuffers);
		}

		static void glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers)(n, renderbuffers);
		}

		static void glEnableVertexAttribArray(GLuint index)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray)(index);
		}

		static void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
		{
			UN_OGL_CALL_EXT_PROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer)(index, size, type, normalized, stride, pointer);
		}
	};
}	// namespace uranus

#define UN_OGL_EXT_PROC(name)	uranus::COGLExtFuncProxy::##name

#endif	// #if !defined(__UN_GRAPH_OGL_EXT_FUNC_PROXY_H__)
