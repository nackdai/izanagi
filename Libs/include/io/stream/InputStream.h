#if !defined(__IZANAGI_IO_INPUT_STREAN_H__)
#define __IZANAGI_IO_INPUT_STREAN_H__

#include "izDefs.h"
#include "IODefs.h"

namespace izanagi {
	/**
	* 入力インターフェースクラス
	*/
	class IInputStream {
	public:
		IInputStream() {}
		virtual ~IInputStream() {}

	protected:
		IInputStream(const IInputStream& rhs);
		const IInputStream& operator=(const IInputStream& rhs);

	public:
		// 読み込み
		virtual IZ_UINT Read(void* buf, IZ_INT offset, size_t size) = 0;

		// サイズ取得
		virtual IZ_LONG GetSize() = 0;

		// 現在位置取得
		virtual IZ_LONG GetCurPos() = 0;

		// シーク
		virtual IZ_BOOL Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos) = 0;

		// 終了処理
		virtual void Finalize() = 0;
	};
}	// namespace izanagi

// ユーティリティ
#ifndef IZ_INPUT_READ
	#define IZ_INPUT_READ(in, p, offset, size)			((in)->Read((p), (offset), (size)) == (size))
#endif	// #ifndef IZ_INPUT_READ

#ifndef IZ_INPUT_READ_ASSERT
	#define IZ_INPUT_READ_ASSERT(in, p, offset, size)	if (!IZ_INPUT_READ(in, p, offset, size)) { IZ_ASSERT(IZ_FALSE); }
#endif	// #ifndef IZ_INPUT_READ_ASSERT

#ifndef IZ_INPUT_READ_VRETURN
	#define IZ_INPUT_READ_VRETURN(in, p, offset, size)	VRETURN(IZ_INPUT_READ(in, p, offset, size))
#endif	// #ifndef IZ_INPUT_READ

#ifndef IZ_INPUT_READ_VGOTO
	#define IZ_INPUT_READ_VGOTO(in, p, offset, size, label)	VGOTO(IZ_INPUT_READ(in, p, offset, size), lable)
#endif	// #ifndef IZ_INPUT_READ_VGOTO

#endif	// #if !defined(__IZANAGI_IO_INPUT_STREAN_H__)
