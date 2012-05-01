#if !defined(__IZ_SAMPLE_SAMPLE_ENV_BOX_H__)
#define __IZ_SAMPLE_SAMPLE_ENV_BOX_H__

#include <izDefs.h>
#include <izStd.h>
#include <izGraph.h>

namespace izanagi {
namespace sample {

class CSampleEnvBox : public izanagi::CObject {
public:
	/** インスタンス作成
	 */
	static CSampleEnvBox* CreateSampleEnvBox(
		izanagi::IMemoryAllocator* allocator,
		izanagi::CGraphicsDevice* device);

private:
	CSampleEnvBox();
	~CSampleEnvBox();

	NO_COPIABLE(CSampleEnvBox);

	IZ_DEFINE_INTERNAL_RELEASE();

private:
	// 初期化
	IZ_BOOL Init(izanagi::CGraphicsDevice* device);

public:
	/** 描画
	 */
	void Render(izanagi::CGraphicsDevice* device);

private:
	enum {
		VTX_NUM = 8,
		IDX_NUM = 10 + 1 + 5 + 5,
	};

	struct SVertex {
		IZ_FLOAT pos[3];
	};

	static const SVertex sVtxList[VTX_NUM];
	static const IZ_USHORT sIdxList[IDX_NUM];

private:
	izanagi::IMemoryAllocator* m_Allocator;

	izanagi::CVertexBuffer* m_VB;
	izanagi::CIndexBuffer* m_IB;
	izanagi::CVertexDeclaration* m_VD;
};

}
}

#endif	// #if !defined(__IZ_SAMPLE_SAMPLE_ENV_BOX_H__)
