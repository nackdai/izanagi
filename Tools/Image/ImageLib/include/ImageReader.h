#if !defined(__IZANAGI_IMAGE_LIB_IMAGE_READER_H__)
#define __IZANAGI_IMAGE_LIB_IMAGE_READER_H__

#include "izDefs.h"
#include "IMGBody.h"
#include "IMGTexture.h"

namespace izanagi {
namespace tool {
	/**
	*/
	class CImageReader {
	private:
		static CImageReader s_cInstance;

	public:
		static CImageReader& GetInstance() { return s_cInstance; }

	private:
		CImageReader()
		{
			m_pReadFunc = IZ_NULL;
		}

		~CImageReader() {}

		NO_COPIABLE(CImageReader);

	public:
		/**
		*/
		typedef CIMGTexture* (*ReadFunc)(LPCSTR lpszPath, E_GRAPH_TEX_TYPE nType);

	public:
		// 読み込み
		CIMGBody* Read(
			LPCSTR lpszPath,
			E_GRAPH_TEX_TYPE nType = E_GRAPH_TEX_TYPE_PLANE);

	public:
		// 削除
		inline void Delete(CIMGBody*& p);

		// コールバックセット
		inline void SetReadFunc(ReadFunc pFunc);

	private:
		ReadFunc m_pReadFunc;
	};

	// inline **********************************

	/**
	* 削除
	*/
	void CImageReader::Delete(CIMGBody*& p)
	{
		if (p != IZ_NULL) {
			p->RemoveAll();
			delete p;
		}
		p = IZ_NULL;
	}

	/**
	* コールバックセット
	*/
	void CImageReader::SetReadFunc(ReadFunc pFunc)
	{
		m_pReadFunc = pFunc;
	}

}	// namespace tool
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IMAGE_LIB_IMAGE_READER_H__)
