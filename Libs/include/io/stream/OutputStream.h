#if !defined(__URANUS_IO_OUTPUT_STREAM_H__)
#define __URANUS_IO_OUTPUT_STREAM_H__

#include "IODefs.h"

namespace uranus {
	/**
	*/
	class IOutputStream {
	public:
		IOutputStream() {}
		virtual ~IOutputStream() {}

	protected:
		IOutputStream(const IOutputStream& rhs);
		const IOutputStream& operator=(const IOutputStream& rhs);

	public:
		virtual void Finalize() = 0;

		// �o��
		virtual UN_UINT Write(const void* buf, UN_UINT offset, size_t size) = 0;

		// �T�C�Y�擾
		virtual UN_UINT GetSize() = 0;

		// ���݈ʒu�擾
		virtual UN_UINT GetCurPos() = 0;

		// �V�[�N
		virtual UN_BOOL Seek(UN_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos) = 0;

		virtual UN_BOOL IsValid() = 0;
	};
}	// namespace uranus

// ���[�e�B���e�B
#ifndef UN_OUTPUT_WRITE
#define UN_OUTPUT_WRITE(out, p, offset, size)			((out)->Write((p), (offset), (size)) == (size))
#endif	// #ifndef UN_INPUT_READ

#ifndef UN_OUTPUT_WRITE_VRETURN
#define UN_OUTPUT_WRITE_VRETURN(out, p, offset, size)	VRETURN(UN_OUTPUT_WRITE(out, p, offset, size))
#endif	// #ifndef UN_INPUT_READ


#endif	// #if !defined(__URANUS_IO_OUTPUT_STREAM_H__)
