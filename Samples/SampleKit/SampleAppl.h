#if !defined(__IZ_SAMPLE_SAMPLE_APPL_H__)
#define __IZ_SAMPLE_SAMPLE_APPL_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"

/** サンプルアプリケーション初期化パラメータ
 */
struct SSampleApplParams {
    IZ_UINT allocatorBufSize;
    void* allocatorBuf;
    IZ_UINT screenWidth;
    IZ_UINT screenHeight;
    izanagi::WindowHandle deviceWindow;
	izanagi::WindowHandle focusWindow;
};

/** サンプルアプリケーションベースクラス.
 */
class CSampleAppl {
private:
	static CSampleAppl s_Instance;

public:
	static CSampleAppl& GetInstance() { return s_Instance; }

private:
    CSampleAppl() {}
	virtual ~CSampleAppl() = 0;

    NO_COPIABLE(CSampleAppl);

public:
	/** 初期化.
     */
	IZ_BOOL Init(const SSampleApplParams& params);

	/** 解放.
     */
	void Release();

    /** 更新.
     */
	virtual void Update() = 0;

    /** 描画.
     */
	void Render();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::CGraphicsDevice* device);

    // 解放
    virtual void ReleaseInternal();

    // 描画.
    virtual void RenderInternal(izanagi::CGraphicsDevice* device) = 0;
};

#endif	// #if !defined(__IZ_SAMPLE_SAMPLE_APPL_H__)
