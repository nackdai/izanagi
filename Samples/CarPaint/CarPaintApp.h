#if !defined(__CAR_PAINT_APP_H__)
#define __CAR_PAINT_APP_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CarPaintApp : public izanagi::sample::CSampleApp {
public:
    CarPaintApp();
    virtual ~CarPaintApp();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

private:
    izanagi::sample::CSampleEnvBox* m_envbox{ nullptr };

    izanagi::CDebugMesh* m_Mesh{ nullptr };

	enum CarPaintShadeType {
		MultiToneOnly,
		EnvMap,
		MultiToneWithEnv,
		Flakes,
		All,

		Num,
	};

	CarPaintShadeType m_shaderType{ CarPaintShadeType::MultiToneOnly };

	izanagi::math::CVector4 m_paintColor[3];
	IZ_FLOAT m_normalScale{ 1.0f };
	IZ_FLOAT m_glossLevel{ 1.0f };
	IZ_FLOAT m_brightnessFactor{ 1.0f };
	IZ_FLOAT m_normalPerturbation{ 0.0f };
	IZ_FLOAT m_microflakePerturbationA{ 0.15f };
	IZ_FLOAT m_microflakePerturbation{ 0.5f };
	izanagi::math::CVector4 m_flakeColor;

    izanagi::sample::Shader m_shdCarPaint[CarPaintShadeType::Num];
    izanagi::sample::Shader m_shdEnvBox;

    izanagi::CImage* m_tex{ nullptr };

    izanagi::debugutil::ImGuiProc* m_imgui{ nullptr };
};

#endif    // #if !defined(__CAR_PAINT_APP_H__)