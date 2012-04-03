#if !defined(__IZANAGI_IMG_LIB_DLL_H__)
#define __IZANAGI_IMG_LIB_DLL_H__

#include "izStd.h"

extern "C"
{

/** ImageLibを使うための初期化.
 */
IZ_API IZ_BOOL izInitImageLib(void* hWnd);

/** ImageLibの解放.
 */
IZ_API void izReleaseImageLib();

/** イメージマスタデータの読み込み.
 *
 * @param[in] path ファイルパス
 * @return イメージマスタデータ (CIMGMaster)
 */
IZ_API void* izReadImageMaster(const char* path);

/** イメージマスタデータの解放.
 *
 * @param[in] p イメージマスタデータ (CIMGMaster)
 */
IZ_API void izReleaseImageMaster(void* p);

/** テクスチャ数取得.
 *
 * @param[in] p イメージデータ (CIMGMaster)
 * @return テクスチャ数
 */
IZ_API IZ_UINT izGetTexNumInMaster(void* p);

/** テクスチャデータ取得.
 *
 * @param[in] p イメージデータ (CIMGMaster)
 * @param[in] texIdx 取得したテクスチャのインデックス
 * @return テクスチャデータ
 */
IZ_API void* izGetTextureInMaster(void* p, IZ_UINT texIdx);

/** テクスチャ内のイメージ数を取得.
 *
 * @param[in] p テクスチャデータ (CIMGTexture)
 * @return テクスチャ内のイメージ数
 */
IZ_API IZ_UINT izGetImageNumInTexture(void* p);

/** テクスチャのミップマップ数を取得.
 *
 * @param[in] p テクスチャデータ (CIMGTexture)
 * @return テクスチャのミップマップ数
 */
IZ_API IZ_UINT izGetMipMapNumInTexture(void* p);

/** イメージデータを取得.
 *
 * @param[in] p テクスチャデータ (CIMGTexture)
 * @param[in] imgIdx イメージデータへのインデックス
 * @param[in] level ミップマップレベル
 * @return イメージデータ
 */
IZ_API void* izGetImageInTexture(void* p, IZ_UINT imgIdx, IZ_UINT level);

/** イメージデータの幅を取得.
 *
 * @param[in] p イメージデータ (CIMGImage)
 * @return イメージデータの幅
 */
IZ_API IZ_UINT izGetImageWidth(void* p);

/** イメージデータの高さを取得.
 *
 * @param[in] p イメージデータ (CIMGImage)
 * @return イメージデータの高さ
 */
IZ_API IZ_UINT izGetImageHeight(void* p);

}

#endif	// #if !defined(__IZANAGI_IMG_LIB_DLL_H__)
