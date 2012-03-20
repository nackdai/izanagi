#include "IMGTexture.h"
#include "MipMapCreator.h"
#include "izToolKit.h"

using namespace izanagi;
using namespace tool;

// 空のテクスチャを作成する
CIMGTexture* CIMGTexture::CreateEmptyTexture(E_GRAPH_TEX_TYPE type)
{
	CIMGTexture* ret = new CIMGTexture(type);
	if (ret
		&& type == E_GRAPH_TEX_TYPE_CUBE)
	{
		ret->m_Images.resize(izanagi::E_GRAPH_CUBE_TEX_FACE_NUM);
	}
	return ret;
}

// テクスチャを削除する
void CIMGTexture::Delete(CIMGTexture* tex)
{
	if (tex) {
		tex->Clear();
		delete tex;
	}
}

// コンストラクタ
CIMGTexture::CIMGTexture(E_GRAPH_TEX_TYPE nType/*= E_GRAPH_TEX_TYPE_PLANE*/)
{
	memset(&m_TexInfo, 0, sizeof(m_TexInfo));
	m_TexInfo.level = 1;
	m_TexInfo.type = nType;
}

// デストラクタ
CIMGTexture::~CIMGTexture()
{
	Clear();
}

namespace {
	// MIPMAP作成可能レベルを計算
	inline IZ_UINT _ComputeMipLevel(
		IZ_UINT nWidth,
		IZ_UINT nHeight,
		E_GRAPH_PIXEL_FMT nFmt,
		IZ_UINT nMipLevel)
	{
		// 作成可能レベル
		IZ_UINT nMaxMipLevel = CMipMapCreator::ComputeMaxMipLevel(
								nWidth,
								nHeight,
								nFmt);

		// どちらか小さい方
		IZ_UINT ret = IZ_MIN(nMipLevel, nMaxMipLevel);

		return ret;
	}
}	// namespace

/**
* 初期化
*/
IZ_BOOL CIMGTexture::InitAsPlane(
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	IZ_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT nFmt,
	IZ_BOOL bIsAllocBuffer/*= IZ_TRUE*/)
{
	IZ_BOOL ret = IZ_FALSE;

	// 一応
	Clear();
	m_Images.resize(1);

	// 作成可能レベル
	nMipLevel = _ComputeMipLevel(
					nWidth, nHeight,
					nFmt,
					nMipLevel);

	m_Images[0].reserve(nMipLevel);

	for (IZ_UINT i = 0; i < nMipLevel; i++) {
		CIMGImage* pImage = new CIMGImage();

		IZ_UINT nW = IZ_MAX(1, nWidth >> i);
		IZ_UINT nH = IZ_MAX(1, nHeight >> i);

		ret = pImage->Init(
				nW, nH,
				nFmt,
				bIsAllocBuffer);
		if (!ret) {
			IZ_ASSERT(IZ_FALSE);
			// TODO
			// 後始末
			return IZ_FALSE;
		}

		m_Images[0].push_back(pImage);
	}

	if (ret) {
		// テクスチャ情報をセット
		SetTexInfo(
			E_GRAPH_TEX_TYPE_PLANE,
			nWidth,
			nHeight,
			nFmt,
			nMipLevel,
			0);
	}
	else {
		Clear();
	}

	return ret;
}

/**
* 初期化
*/
IZ_BOOL CIMGTexture::InitAsCube(
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	IZ_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT nFmt,
	IZ_BOOL bIsAllocBuffer/*= IZ_TRUE*/)
{
	IZ_BOOL ret = IZ_FALSE;

	// 一応
	Clear();
	m_Images.resize(izanagi::E_GRAPH_CUBE_TEX_FACE_NUM);

	// 作成可能レベル
	nMipLevel = _ComputeMipLevel(
					nWidth, nHeight,
					nFmt,
					nMipLevel);

	for (IZ_UINT i = 0; i < izanagi::E_GRAPH_CUBE_TEX_FACE_NUM; i++) {
		m_Images[i].reserve(nMipLevel);

		for (IZ_UINT n = 0; n < nMipLevel; n++) {
			CIMGImage* pImage = new CIMGImage();

			IZ_UINT nW = IZ_MAX(1, nWidth >> n);
			IZ_UINT nH = IZ_MAX(1, nHeight >> n);

			ret = pImage->Init(
					nW, nH,
					nFmt,
					bIsAllocBuffer);
			if (!ret) {
				IZ_ASSERT(IZ_FALSE);
				// TODO
				// 後始末
				return IZ_FALSE;
			}

			m_Images[i].push_back(pImage);
		}
	}

	if (ret) {
		// テクスチャ情報をセット
		SetTexInfo(
			E_GRAPH_TEX_TYPE_CUBE,
			nWidth,
			nHeight,
			nFmt,
			nMipLevel,
			0);
	}
	else {
		Clear();
	}

	return ret;
}

/**
* 初期化
*/
IZ_BOOL CIMGTexture::InitAsVolume(
	IZ_UINT nDepth,
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	IZ_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT nFmt,
	IZ_BOOL bIsAllocBuffer/*= IZ_TRUE*/)
{
	// TODO
	IZ_ASSERT(IZ_FALSE);
	return IZ_FALSE;
}

/**
* クリア
*/
void CIMGTexture::Clear()
{
	IZ_UINT nNum = GetImageNum();

	for (IZ_UINT i = 0; i < nNum; i++) {
		std::vector<CIMGImage*>& tImageList = m_Images[i];

		IZ_UINT len = (IZ_UINT)tImageList.size();
		for (IZ_UINT n = 0; n < len; n++) {
			CIMGImage* pImage = tImageList[n];

			delete pImage;
		}

		tImageList.clear();
	}
}

/**
* フォーマット変換
*/
IZ_BOOL CIMGTexture::ConvertPixelFormat(E_GRAPH_PIXEL_FMT nFmt)
{
	IZ_BOOL ret = IZ_FALSE;

	IZ_UINT nNum = GetImageNum();
	IZ_UINT nMipLevel = m_TexInfo.level;

	for (IZ_UINT i = 0; i < nNum; i++) {
		std::vector<CIMGImage*>& tImageList = m_Images[i];
		IZ_ASSERT((IZ_UINT)tImageList.size() == nMipLevel);

		// MIPMAPごとに行う
		for (IZ_UINT nLevel = 0; nLevel < nMipLevel; nLevel++) {
			CIMGImage* pImage = tImageList[nLevel];

			// フォーマット変換
			ret = pImage->ConvertPixelFormat(nFmt);
			if (!ret) {
				IZ_ASSERT(IZ_FALSE);
				break;
			}
		}
	}

	if (ret) {
		m_TexInfo.fmt = nFmt;
	}

	return ret;
}

/**
* MIPMAP作成
*/
IZ_BOOL CIMGTexture::CreateMipMap(IZ_UINT nMipLevel/*= 1000*/)
{
	if (nMipLevel == 1) {
		// 何もしない
		return IZ_TRUE;
	}

	IZ_BOOL ret = IZ_FALSE;

	IZ_UINT nNum = GetImageNum();
	E_GRAPH_PIXEL_FMT nFmt = m_TexInfo.fmt;
	IZ_UINT nCurMipLevel = m_TexInfo.level;

	IZ_UINT nWidth = 1 << m_TexInfo.w;
	IZ_UINT nHeight = 1 << m_TexInfo.h;

	// 作成可能レベル
	nMipLevel = _ComputeMipLevel(
					nWidth, nHeight,
					nFmt,
					nMipLevel);

	for (IZ_UINT i = 0; i < nNum; i++) {
		std::vector<CIMGImage*>& tImageList = m_Images[i];

		// TODO
		// 先頭以外は消す
		std::vector<CIMGImage*>::iterator it = tImageList.begin();
		IZ_ASSERT(it != tImageList.end());
		for (; it != tImageList.end(); it++) {
			tImageList.erase(it);
		}

		// 念のため
		IZ_ASSERT(tImageList[0]->GetFmt() == nFmt);
		IZ_ASSERT(tImageList[0]->GetWidth() == nWidth);
		IZ_ASSERT(tImageList[0]->GetHeight() == nHeight);

		for (IZ_UINT nLevel = nCurMipLevel; nLevel < nMipLevel; nLevel++) {
			IZ_UINT nW = nWidth >> nLevel;
			IZ_UINT nH = nHeight >> nLevel;

			// 前のレベルを元にする
			CIMGImage* pPrevImage = tImageList[nLevel - 1];

			// 新たに作成する分を増やす
			CIMGImage* pNewImage = new CIMGImage();
			ret = pNewImage->Init(nW, nH, nFmt);
			if (!ret) {
				IZ_ASSERT(IZ_FALSE);
				break;
			}

			tImageList.push_back(pNewImage);

			// MIPMAP作成
			CMipMapCreator::Convert(
				pPrevImage->GetDataAsByte(),
				pNewImage->GetDataAsByte(),
				nW, nH, nFmt);
		}
	}

	if (ret) {
		m_TexInfo.level = nMipLevel;
	}

	return ret;
}

/**
* 最下位MIPMAP削除
*/
IZ_BOOL CIMGTexture::DeleteLawestMipMap()
{
	IZ_BOOL ret = IZ_FALSE;

	IZ_UINT nNum = GetImageNum();
	IZ_UINT nCurMipLevel = m_TexInfo.level;

	if (nCurMipLevel == 1) {
		// 削除すると何もなくなるので、何もしない
		return IZ_TRUE;
	}

	for (IZ_UINT i = 0; i < nNum; i++) {
		std::vector<CIMGImage*>& tImageList = m_Images[i];

		// 最後の要素を取得
		std::vector<CIMGImage*>::iterator it = tImageList.end() - 1;
		CIMGImage* pImage = *it;

		tImageList.erase(it);
		delete pImage;
	}

	// レベルを１つ減らす
	m_TexInfo.level--;

	return IZ_TRUE;
}

// 出力
IZ_BOOL CIMGTexture::Write(IOutputStream* pOut)
{
	IZ_ASSERT(pOut != IZ_NULL);

	m_TexInfo.size = GetExportSize();

	// テクスチャヘッダ
	IZ_BOOL ret = IZ_OUTPUT_WRITE(
					pOut,
					&m_TexInfo,
					0,
					sizeof(m_TexInfo));
	VGOTO(ret, __EXIT__);

	// イメージデータ
	IZ_UINT nImageNum = GetImageNum();
	IZ_UINT nLevelMax = GetTexInfo().level;

	for (IZ_UINT i = 0; i < nImageNum; i++) {
		std::vector<CIMGImage*>& tImageList = m_Images[i];

		for (IZ_UINT nLevel = 0; nLevel < nLevelMax; nLevel++) {
			CIMGImage* pImage = tImageList[nLevel];

			ret = pImage->Write(pOut);
			VGOTO(ret, __EXIT__);
		}
	}

__EXIT__:
	return ret;
}

// Cubeテクスチャの面として追加する.
IZ_BOOL CIMGTexture::AddImageAsCubeFace(
	izanagi::E_GRAPH_CUBE_TEX_FACE face,
	std::vector<CIMGImage*>& images)
{
	VRETURN(m_Images.size() == izanagi::E_GRAPH_CUBE_TEX_FACE_NUM);
	VRETURN(images.size() > 0);
	
	if (m_Images[0].size() > 0) {
		// キューブマップの全ての面は同じ幅・高さである必要がある

		const CIMGImage* img = images[0];

		IZ_UINT width = GetWidth();
		IZ_UINT height = GetHeight();
		IZ_ASSERT(width > 0 && height > 0);

		VRETURN(img->GetWidth() == width && img->GetHeight() == height);
	}

	// 一番上のレベルのみ登録
	m_Images[face].push_back(images[0]);

	// テクスチャ情報を設定
	// XP面を基準とする
	if (face == izanagi::E_GRAPH_CUBE_TEX_FACE_X_P
		&& m_Images[izanagi::E_GRAPH_CUBE_TEX_FACE_X_P].size() > 0)
	{
		const CIMGImage* img = m_Images[izanagi::E_GRAPH_CUBE_TEX_FACE_X_P][0];

		SetTexInfo(
			izanagi::E_GRAPH_TEX_TYPE_CUBE,
			img->GetWidth(),
			img->GetHeight(),
			img->GetFmt(),
			1,
			0);
	}

	return IZ_TRUE;
}

// 読み込み
IZ_BOOL CIMGTexture::Read(IInputStream* pIn)
{
	IZ_ASSERT(pIn != IZ_NULL);

	// テクスチャヘッダ読み込み
	IZ_BOOL ret = IZ_INPUT_READ(pIn, &m_TexInfo, 0, sizeof(m_TexInfo));
	VGOTO(ret, __EXIT__);

	E_GRAPH_TEX_TYPE nTexType = static_cast<E_GRAPH_TEX_TYPE>(m_TexInfo.type);

	IZ_UINT nWidth = 1 << m_TexInfo.w;
	IZ_UINT nHeight = 1 << m_TexInfo.h;

	// 初期化
	switch (nTexType) {
	case E_GRAPH_TEX_TYPE_PLANE:
		ret = InitAsPlane(
				nWidth, nHeight,
				m_TexInfo.level,
				m_TexInfo.fmt);
		break;
	case E_GRAPH_TEX_TYPE_CUBE:
		ret = InitAsCube(
				nWidth, nHeight,
				m_TexInfo.level,
				m_TexInfo.fmt);
		break;
	case E_GRAPH_TEX_TYPE_VOLUME:
		ret = InitAsVolume(
				m_TexInfo.depth,
				nWidth, nHeight,
				m_TexInfo.level,
				m_TexInfo.fmt);
		break;
	default:
		ret = IZ_FALSE;
		break;
	}

	VGOTO(ret, __EXIT__);

	// イメージデータ読み込み
	IZ_UINT nImageNum = GetImageNum();
	for (IZ_UINT i = 0; i < nImageNum; i++) {
		std::vector<CIMGImage*>& tImageList = m_Images[i];

		for (IZ_UINT nLevel = 0; nLevel < m_TexInfo.level; nLevel++) {
			CIMGImage* pImage = tImageList[nLevel];

			ret = pImage->Read(pIn);
			VGOTO(ret, __EXIT__);
		}
	}

__EXIT__:
	if (!ret) {
		Clear();
	}
	return ret;
}
