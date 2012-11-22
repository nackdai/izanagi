#if !defined(__IZANAGI_IMAGE_LIB_IMG_IMAGE_H__)
#define __IZANAGI_IMAGE_LIB_IMG_IMAGE_H__

#include <vector>
#include "izDefs.h"
#include "izGraph.h"

namespace izanagi {
	class IInputStream;
	class IOutputStream;

namespace tool {
	/**
	* データは RGBA8 or RGBA32F のどちらかで持つ
	*/
	class CIMGImage {
		friend class CIMGTexture;

	public:
		// Image内で保持されるフォーマット
		static graph::E_GRAPH_PIXEL_FMT GetFmtInImage(graph::E_GRAPH_PIXEL_FMT nFmt);

	private:
		CIMGImage();
		CIMGImage(
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			graph::E_GRAPH_PIXEL_FMT nFmt);

		~CIMGImage();

		NO_COPIABLE(CIMGImage);

	public:
		// 初期化
		IZ_BOOL Init(
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			graph::E_GRAPH_PIXEL_FMT nFmt,
			IZ_BOOL bIsAllocBuffer = IZ_TRUE);

		// フォーマット変換
		IZ_BOOL ConvertPixelFormat(graph::E_GRAPH_PIXEL_FMT nFmt);

		/** RGBA8としてピクセルデータを取得.
		 */
		IZ_BYTE* GetPixelDataAsBGRA8();

		/** 内部のテンポラリバッファをクリアする.
		 */
		void ClearTemporaryBuffer();

	private:
		// 読み込み
		IZ_BOOL Read(IInputStream* pIn);

		// 出力
		IZ_BOOL Write(IOutputStream* pOut);

	public:
		// データ取得
		inline std::vector<IZ_BYTE>& GetData();

		// byteとしてデータ取得
		inline IZ_BYTE* GetDataAsByte();

		// UINTとしてデータ取得
		inline IZ_UINT* GetDataAsUInt();

		// floatとしてデータ取得
		inline IZ_FLOAT* GetDataAsFloat();

	public:
		// 幅取得
		inline IZ_UINT GetWidth() const;

		// 高さ取得
		inline IZ_UINT GetHeight() const;

		// ピクセルフォーマット取得
		inline graph::E_GRAPH_PIXEL_FMT GetFmt() const;

		// データサイズ取得
		inline IZ_UINT GetSize() const;

	private:
		std::vector<IZ_BYTE> m_DataBuffer;

		IZ_UINT m_nWidth;
		IZ_UINT m_nHeight;
		graph::E_GRAPH_PIXEL_FMT m_Fmt;

		std::vector<IZ_BYTE> m_TmpBuffer;
	};

	// inline **********************************

	/**
	* データ取得
	*/
	std::vector<IZ_BYTE>& CIMGImage::GetData()
	{
		return m_DataBuffer;
	}

	/**
	* byteとしてデータ取得
	*/
	IZ_BYTE* CIMGImage::GetDataAsByte()
	{
		IZ_ASSERT(m_DataBuffer.size() > 0);
		return &m_DataBuffer[0];
	}

	/**
	* UINTとしてデータ取得
	*/
	IZ_UINT* CIMGImage::GetDataAsUInt()
	{
		IZ_ASSERT(m_DataBuffer.size() >= sizeof(IZ_UINT));

		IZ_BYTE* ret = GetDataAsByte();
		return reinterpret_cast<IZ_UINT*>(ret);
	}

	/**
	* floatとしてデータ取得
	*/
	IZ_FLOAT* CIMGImage::GetDataAsFloat()
	{
		IZ_ASSERT(m_DataBuffer.size() >= sizeof(IZ_FLOAT));

		IZ_BYTE* ret = GetDataAsByte();
		return reinterpret_cast<IZ_FLOAT*>(ret);
	}

	/**
	* 幅取得
	*/
	IZ_UINT CIMGImage::GetWidth() const
	{
		return m_nWidth;
	}

	/**
	* 高さ取得
	*/
	IZ_UINT CIMGImage::GetHeight() const
	{
		return m_nHeight;
	}

	/**
	* ピクセルフォーマット取得
	*/
	graph::E_GRAPH_PIXEL_FMT CIMGImage::GetFmt() const
	{
		return m_Fmt;
	}

	/**
	* データサイズ取得
	*/
	IZ_UINT CIMGImage::GetSize() const
	{
		IZ_UINT ret = static_cast<IZ_UINT>(m_DataBuffer.size());
		return ret;
	}
}	// namespace tool
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IMAGE_LIB_IMG_IMAGE_H__)
