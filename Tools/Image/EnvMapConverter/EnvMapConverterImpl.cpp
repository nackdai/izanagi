#include "EnvMapConverterImpl.h"
#include "TexProxy.h"

void Convert(
	izanagi::izanagi_tk::CTextureLite* inTex,
	EnvMapType inType,
	izanagi::izanagi_tk::CTextureLite* outTex,
	EnvMapType outType)
{
	CTexProxy* in = CTexProxy::CreateTexProxy(inTex, inType);
	CTexProxy* out = CTexProxy::CreateTexProxy(outTex, outType);

	IZ_UINT inWidth = inTex->GetWidth();
	IZ_UINT inHeight = inTex->GetHeight();

	IZ_UINT outWidth = outTex->GetWidth();
	IZ_UINT outHeight = outTex->GetHeight();

	izanagi::SVector vecRef;
	SFloatColor color;

	for (IZ_UINT y = 0; y < outHeight; y++) {
		for (IZ_UINT x = 0; x < outWidth; x++) {
			IZ_BOOL isValid = out->isValid(x, y);

			if (isValid) {
				out->getRef(x, y, vecRef);

				IZ_FLOAT u, v;
				in->getUVFromRef(vecRef, u ,v);

				in->getColor(u, v, color);
				out->putColor(x, y, color);
			}
		}
	}

	CTexProxy::DeleteTexProxy(in);
	CTexProxy::DeleteTexProxy(out);
}
