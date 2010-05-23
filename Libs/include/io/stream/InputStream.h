#if !defined(__URANUS_IO_INPUT_STREAN_H__)
#define __URANUS_IO_INPUT_STREAN_H__

#include "../../unDefs.h"
#include "IODefs.h"

namespace uranus {
	/**
	* ���̓C���^�[�t�F�[�X�N���X
	*/
	class IInputStream {
	public:
		IInputStream() {}
		virtual ~IInputStream() {}

	protected:
		IInputStream(const IInputStream& rhs);
		const IInputStream& operator=(const IInputStream& rhs);

	public:
		// �ǂݍ���
		virtual UN_UINT Read(void* buf, UN_INT offset, size_t size) = 0;

		// �T�C�Y�擾
		virtual UN_LONG GetSize() = 0;

		// ���݈ʒu�擾
		virtual UN_LONG GetCurPos() = 0;

		// �V�[�N
		virtual UN_BOOL Seek(UN_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos) = 0;

		// �I������
		virtual void Finalize() = 0;
	};
}	// namespace uranus

// ���[�e�B���e�B
#ifndef UN_INPUT_READ
#define UN_INPUT_READ(in, p, offset, size)			((in)->Read((p), (offset), (size)) == (size))
#endif	// #ifndef UN_INPUT_READ

#ifndef UN_INPUT_READ_VRETURN
#define UN_INPUT_READ_VRETURN(in, p, offset, size)	VRETURN(UN_INPUT_READ(in, p, offset, size))
#endif	// #ifndef UN_INPUT_READ

#endif	// #if !defined(__URANUS_IO_INPUT_STREAN_H__)
