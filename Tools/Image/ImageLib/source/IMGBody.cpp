#include "IMGBody.h"
#include "izToolKit.h"
#include "ImageLibUtility.h"

using namespace izanagi;
using namespace tool;

// コンストラクタ
CIMGMaster::CIMGMaster()
{
}

// デストラクタ
CIMGMaster::~CIMGMaster()
{
    RemoveAll();
}

/**
* テクスチャ追加
*/
IZ_BOOL CIMGMaster::Add(CIMGTexture* pTex)
{
    m_Textures.push_back(pTex);
    return IZ_TRUE;
}

/**
* テクスチャ削除
*/
IZ_BOOL CIMGMaster::Remove(IZ_UINT nIdx)
{
    IZ_ASSERT(nIdx < m_Textures.size());

    std::vector<CIMGTexture*>::iterator it = m_Textures.begin();
    std::advance(it, nIdx);

    CIMGTexture* pTex = *it;

    m_Textures.erase(it);

    delete pTex;

    return IZ_TRUE;
}

/**
* テクスチャ全削除
*/
void CIMGMaster::RemoveAll()
{
    IZ_UINT nNum = (IZ_UINT)m_Textures.size();

    for (IZ_UINT i = 0; i < nNum; i++) {
        CIMGTexture* pTex = m_Textures[i];
        delete pTex;
    }

    m_Textures.clear();
}       

/**
* 読み込み
*/
IZ_BOOL CIMGMaster::Read(LPCSTR lpszPath)
{
    CFileInputStream cInputStream;
    if (!cInputStream.Open(lpszPath)) {
        IZ_ASSERT(IZ_FALSE);
        return IZ_FALSE;
    }

    IZ_BOOL ret = IZ_FALSE;
    IInputStream* pIn = &cInputStream;

    // ファイルヘッダ読み込み
    S_IMG_HEADER sHeader;
    ret = IZ_INPUT_READ(pIn, &sHeader, 0, sizeof(S_IMG_HEADER));
    VGOTO(ret, __EXIT__);

    // マジックナンバーチェック
    ret = IS_IMG_FORMAT(sHeader.magic);

    // IMGフォーマット以外でも通ることがあるので・・・
    //VGOTO(ret, __EXIT__);
    if (!ret) {
        goto __EXIT__;
    }

    // ジャンプテーブル分は飛ばす
    ret = pIn->Seek(sHeader.sizeJumpTbl, E_IO_STREAM_SEEK_POS_CUR);
    VGOTO(ret, __EXIT__);

    // テクスチャ読み込み
    for (IZ_UINT i = 0; i < sHeader.numTextures; i++) {
        CIMGTexture* pTex = new CIMGTexture();

        ret = pTex->Read(pIn);
        VGOTO(ret, __EXIT__);

        Add(pTex);
    }

__EXIT__:
    if (!ret) {
        RemoveAll();
    }
    cInputStream.Close();
    return ret;
}

/**
* 出力
*/
IZ_BOOL CIMGMaster::Write(LPCSTR lpszPath)
{
    CFileOutputStream cOutputStream;
    if (!cOutputStream.Open(lpszPath)) {
        IZ_ASSERT(IZ_FALSE);
        return IZ_FALSE;
    }

    IZ_BOOL ret = IZ_FALSE;

    // ジャンプテーブル
    std::vector<IZ_UINT> tJumpTable;

    // ファイルヘッダ
    S_IMG_HEADER sHeader;
    CImageUtil::SetImageHeader(&sHeader, GetTexNum());

    // ファイルヘッダの分を空ける
    ret = cOutputStream.Seek(sizeof(sHeader), E_IO_STREAM_SEEK_POS_CUR);
    VGOTO(ret, __EXIT__);

    // ジャンプテーブル作成
    tJumpTable.reserve(GetTexNum());

    // テクスチャデータ出力
    IZ_UINT nTexNum = GetTexNum();
    for (IZ_UINT i = 0; i < nTexNum; i++) {
        CIMGTexture* pTex = m_Textures[i];

        tJumpTable.push_back(pTex->GetExportSize());

        izanagi::IOutputStream* pOut = &cOutputStream;
        ret = pTex->Write(pOut);
        VGOTO(ret, __EXIT__);
    }

    // 先頭に戻る
    ret = cOutputStream.Seek(0, E_IO_STREAM_SEEK_POS_START);
    VGOTO(ret, __EXIT__);

    // ファイルヘッダ出力
    ret = IZ_OUTPUT_WRITE(&cOutputStream, &sHeader, 0, sizeof(sHeader));
    VGOTO(ret, __EXIT__);

    // ジャンプテーブル出力
    ret = IZ_OUTPUT_WRITE(&cOutputStream, &tJumpTable[0], 0, sizeof(IZ_UINT) * tJumpTable.size());
    VGOTO(ret, __EXIT__);

__EXIT__:
    cOutputStream.Finalize();
    return ret;
}
