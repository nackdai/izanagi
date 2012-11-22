#include "EnvMapConverterImpl.h"
#include "TexProxy.h"

void Convert(
	std::vector<izanagi::tool::CTextureLite*>& inTex,
	EnvMapType inType,
	std::vector<izanagi::tool::CTextureLite*>& outTex,
	EnvMapType outType)
{
	CTexProxy* in = CTexProxy::CreateTexProxy(inTex, inType);
	CTexProxy* out = CTexProxy::CreateTexProxy(outTex, outType);

	IZ_UINT inWidth = inTex[0]->GetWidth();
	IZ_UINT inHeight = inTex[0]->GetHeight();

	IZ_UINT outWidth = outTex[0]->GetWidth();
	IZ_UINT outHeight = outTex[0]->GetHeight();

	izanagi::SVector vecRef;
	SFloatColor color;

	for (size_t i = 0; i < outTex.size(); i++) {
		for (IZ_UINT y = 0; y < outHeight; y++) {
			for (IZ_UINT x = 0; x < outWidth; x++) {
				IZ_BOOL isValid = out->isValid(x, y);

				if (isValid) {
					out->getRef(
						x, y,
						vecRef,
						(izanagi::graph::E_GRAPH_CUBE_TEX_FACE)i);

					IZ_FLOAT u, v;
					in->getUVFromRef(vecRef, u ,v);

					in->getColor(u, v, color);
					out->putColor(x, y, color);
				}
			}
		}
	}

	CTexProxy::DeleteTexProxy(in);
	CTexProxy::DeleteTexProxy(out);
}
