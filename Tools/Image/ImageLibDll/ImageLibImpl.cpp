#include "ImageLibDll.h"
#include "izToolKit.h"
#include "izImageLib.h"

#include "ImageGraphicsDevice.h"
#include "ImageReaderImpl.h"

/** ImageLib���g�����߂̏�����.
 */
IZ_BOOL izInitImageLib(void* hWnd)
{
	izanagi::tool::CGraphicsDeviceLite* device = izanagi::InitGraphicsDevice(hWnd);
	
	if (device != IZ_NULL) {
		// �e�N�X�`���ǂݍ��ݏ������Z�b�g
		izanagi::tool::CImageReader::GetInstance().SetReadFunc(CImageReaderImpl::Read);
	}

	return (device != IZ_NULL);
}

/** ImageLib�̉��.
 */
void izReleaseImageLib()
{
	izanagi::ReleaseGraphicsDevice();
}

/** �C���[�W�}�X�^�f�[�^�̓ǂݍ���.
 *
 * @param[in] path �t�@�C���p�X
 * @return �C���[�W�}�X�^�f�[�^ (CIMGMaster)
 */
void* izReadImageMaster(const char* path)
{
	izanagi::tool::CGraphicsDeviceLite* device = izanagi::GetGraphicsDevice();
	VRETURN_NULL(device != IZ_NULL);

	// TODO
	izanagi::tool::CIMGMaster* master = izanagi::tool::CImageReader::GetInstance().Read(
		path,
		izanagi::E_GRAPH_TEX_TYPE_PLANE);

	return master;
}

/** �C���[�W�}�X�^�f�[�^�̉��.
 *
 * @param[in] p �C���[�W�}�X�^�f�[�^ (CIMGMaster)
 */
void izReleaseImageMaster(void* p)
{
	VRETURN_NONE(p != IZ_NULL);
	izanagi::tool::CIMGMaster* master =  reinterpret_cast<izanagi::tool::CIMGMaster*>(p);
	izanagi::tool::CImageReader::GetInstance().Delete(master);
}

/** �e�N�X�`�����擾.
 *
 * @param[in] p �C���[�W�f�[�^ (CIMGMaster)
 * @return �e�N�X�`����
 */
IZ_UINT izGetTexNumInMaster(void* p)
{
	VRETURN_ZERO(p != IZ_NULL);
	izanagi::tool::CIMGMaster* master =  reinterpret_cast<izanagi::tool::CIMGMaster*>(p);
	IZ_UINT ret = master->GetTexNum();
	return ret;
}

/** �e�N�X�`���f�[�^�擾.
 *
 * @param[in] p �C���[�W�f�[�^ (CIMGMaster)
 * @param[in] texIdx �擾�����e�N�X�`���̃C���f�b�N�X
 * @return �e�N�X�`���f�[�^
 */
void* izGetTextureInMaster(void* p, IZ_UINT texIdx)
{
	VRETURN_NULL(p != IZ_NULL);

	izanagi::tool::CIMGMaster* master =  reinterpret_cast<izanagi::tool::CIMGMaster*>(p);
	VRETURN_NULL(texIdx < master->GetTexNum());

	izanagi::tool::CIMGTexture* tex = master->GetTexture(texIdx);
	return tex;
}

/** �e�N�X�`�����̃C���[�W�����擾.
 *
 * @param[in] p �e�N�X�`���f�[�^ (CIMGTexture)
 * @return �e�N�X�`�����̃C���[�W��
 */
IZ_UINT izGetImageNumInTexture(void* p)
{
	VRETURN_ZERO(p != IZ_NULL);
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	IZ_UINT ret = tex->GetImageNum();
	return ret;
}

/** �e�N�X�`���̃~�b�v�}�b�v�����擾.
 *
 * @param[in] p �e�N�X�`���f�[�^ (CIMGTexture)
 * @return �e�N�X�`���̃~�b�v�}�b�v��
 */
IZ_UINT izGetMipMapNumInTexture(void* p)
{
	VRETURN_ZERO(p != IZ_NULL);
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);

	// NOTE
	// �ǂ̃C���[�W�ł��~�b�v�}�b�v���͓���
	size_t ret = tex->GetImage(0).size();
	return (IZ_UINT)ret;
}

/** �C���[�W�f�[�^���擾.
 *
 * @param[in] p �e�N�X�`���f�[�^ (CIMGTexture)
 * @param[in] imgIdx �C���[�W�f�[�^�ւ̃C���f�b�N�X
 * @param[in] level �~�b�v�}�b�v���x��
 * @return �C���[�W�f�[�^
 */
void* izGetImageInTexture(void* p, IZ_UINT imgIdx, IZ_UINT level)
{
	VRETURN_NULL(p != IZ_NULL);
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	
	VRETURN_ZERO(imgIdx < izGetImageNumInTexture(p));
	VRETURN_ZERO(level < izGetMipMapNumInTexture(p));
	return tex->GetImage(imgIdx)[level];
}

/** �C���[�W�f�[�^�̕����擾.
 *
 * @param[in] p �C���[�W�f�[�^ (CIMGImage)
 * @return �C���[�W�f�[�^�̕�
 */
IZ_UINT izGetImageWidth(void* p)
{
	VRETURN_ZERO(p != IZ_NULL);
	izanagi::tool::CIMGImage* img = reinterpret_cast<izanagi::tool::CIMGImage*>(p);
	IZ_UINT ret = img->GetWidth();
	return ret;
}

/** �C���[�W�f�[�^�̍������擾.
 *
 * @param[in] p �C���[�W�f�[�^ (CIMGImage)
 * @return �C���[�W�f�[�^�̍���
 */
IZ_UINT izGetImageHeight(void* p)
{
	VRETURN_ZERO(p != IZ_NULL);
	izanagi::tool::CIMGImage* img = reinterpret_cast<izanagi::tool::CIMGImage*>(p);
	IZ_UINT ret = img->GetHeight();
	return ret;
}

/** BGRA8�Ƃ��ăs�N�Z���f�[�^���擾.
 *
 * @param[in] p �C���[�W�f�[�^ (CIMGImage)
 * @return �s�N�Z���f�[�^�̃|�C���^
 */
void* izGetPixelDataAsBGRA8(void* p)
{
	VRETURN_ZERO(p != IZ_NULL);
	izanagi::tool::CIMGImage* img = reinterpret_cast<izanagi::tool::CIMGImage*>(p);
	return img->GetPixelDataAsBGRA8();
}

/** �擾�����s�N�Z���f�[�^�����.
 *
 * @param[in] p �C���[�W�f�[�^ (CIMGImage)
 */
void izReleasePixelData(void* p)
{
	VRETURN_VAL(p != IZ_NULL,);
	izanagi::tool::CIMGImage* img = reinterpret_cast<izanagi::tool::CIMGImage*>(p);
	img->ClearTemporaryBuffer();
}

static const char* format[] = {
	"RGBA8",
	"BGRA8",

	"RGBA4",
	"RGB10A2",
	"A8",
	
	"R32F",
	"RGBA16F",
	"RGBA32F",

	"DXT1",
	"DXT3",
	"DXT5",

	"RGBX8",
	"D24S8",
};

static const char* address[] = {
	"WRAP",
	"MIRROR",
	"CLAMP",
	"BORDER",
	"MIRRORONCE",
};

static const char* filter[] = {
	"NONE",				// filtering disabled (valid for mip filter only)
	"POINT",			// nearest
	"LINEAR",			// linear interpolation
	"ANISOTROPIC",		// anisotropic
	"PYRAMIDALQUAD",	// 4-sample tent
	"GAUSSIANQUAD",
};

static const char* GetString(
	const char* strings[],
	IZ_UINT type)
{
	VRETURN_NULL(type < COUNTOF(strings));
	return strings[type];
}

const char* izGetImagePixelFormat(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		izanagi::E_GRAPH_PIXEL_FMT fmt = tex->GetTexInfo().fmt;
		VRETURN_NULL(fmt < COUNTOF(format));
		return format[fmt];
	}
	return IZ_NULL;
}

const char* izGetAddressU(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		const char* ret = GetString(address, tex->GetTexInfo().addressU);
		return ret;
	}
	return IZ_NULL;
}

const char* izGetAddressV(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		const char* ret = GetString(address, tex->GetTexInfo().addressV);
		return ret;
	}
	return IZ_NULL;
}

const char* izGetMinFilger(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		const char* ret = GetString(address, tex->GetTexInfo().minFilter);
		return ret;
	}
	return IZ_NULL;
}

const char* izGetMagFilger(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		const char* ret = GetString(address, tex->GetTexInfo().magFilter);
		return ret;
	}
	return IZ_NULL;
}

const char* izGetMipFilger(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		const char* ret = GetString(address, tex->GetTexInfo().mipFilter);
		return ret;
	}
	return IZ_NULL;
}

IZ_BOOL izConvertFormat(void* p, IZ_UINT fmt)
{
	// TODO
	return IZ_TRUE;
}

void izSetAddressU(void* p, IZ_UINT addr)
{
}

void izSetAddressV(void* p, IZ_UINT addr)
{
}

void izSetMinFilter(void* p, IZ_UINT filter)
{
}

void SetMagFilter(void* p, IZ_UINT filter)
{
}

void izSetMipFilter(void* p, IZ_UINT filter)
{
}
