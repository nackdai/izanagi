#if !defined(__IZANAGI_IMG_LIB_DLL_H__)
#define __IZANAGI_IMG_LIB_DLL_H__

#include "izStd.h"

extern "C"
{

/** ImageLib���g�����߂̏�����.
 */
IZ_API IZ_BOOL izInitImageLib(void* hWnd);

/** ImageLib�̉��.
 */
IZ_API void izReleaseImageLib();

/** �C���[�W�}�X�^�f�[�^�̓ǂݍ���.
 *
 * @param[in] path �t�@�C���p�X
 * @return �C���[�W�}�X�^�f�[�^ (CIMGMaster)
 */
IZ_API void* izReadImageMaster(const char* path);

/** �C���[�W�}�X�^�f�[�^�̉��.
 *
 * @param[in] p �C���[�W�}�X�^�f�[�^ (CIMGMaster)
 */
IZ_API void izReleaseImageMaster(void* p);

/** �e�N�X�`�����擾.
 *
 * @param[in] p �C���[�W�f�[�^ (CIMGMaster)
 * @return �e�N�X�`����
 */
IZ_API IZ_UINT izGetTexNumInMaster(void* p);

/** �e�N�X�`���f�[�^�擾.
 *
 * @param[in] p �C���[�W�f�[�^ (CIMGMaster)
 * @param[in] texIdx �擾�����e�N�X�`���̃C���f�b�N�X
 * @return �e�N�X�`���f�[�^
 */
IZ_API void* izGetTextureInMaster(void* p, IZ_UINT texIdx);

/** �e�N�X�`�����̃C���[�W�����擾.
 *
 * @param[in] p �e�N�X�`���f�[�^ (CIMGTexture)
 * @return �e�N�X�`�����̃C���[�W��
 */
IZ_API IZ_UINT izGetImageNumInTexture(void* p);

/** �e�N�X�`���̃~�b�v�}�b�v�����擾.
 *
 * @param[in] p �e�N�X�`���f�[�^ (CIMGTexture)
 * @return �e�N�X�`���̃~�b�v�}�b�v��
 */
IZ_API IZ_UINT izGetMipMapNumInTexture(void* p);

/** �C���[�W�f�[�^���擾.
 *
 * @param[in] p �e�N�X�`���f�[�^ (CIMGTexture)
 * @param[in] imgIdx �C���[�W�f�[�^�ւ̃C���f�b�N�X
 * @param[in] level �~�b�v�}�b�v���x��
 * @return �C���[�W�f�[�^
 */
IZ_API void* izGetImageInTexture(void* p, IZ_UINT imgIdx, IZ_UINT level);

/** �C���[�W�f�[�^�̕����擾.
 *
 * @param[in] p �C���[�W�f�[�^ (CIMGImage)
 * @return �C���[�W�f�[�^�̕�
 */
IZ_API IZ_UINT izGetImageWidth(void* p);

/** �C���[�W�f�[�^�̍������擾.
 *
 * @param[in] p �C���[�W�f�[�^ (CIMGImage)
 * @return �C���[�W�f�[�^�̍���
 */
IZ_API IZ_UINT izGetImageHeight(void* p);

}

#endif	// #if !defined(__IZANAGI_IMG_LIB_DLL_H__)
