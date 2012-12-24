#if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_SCRIPT_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_DEFS_SCRIPT_FXH__

/////////////////////////////////////
// テクニック・パス共通

#define SetRenderTarget(tex)        string RenderColorTarget=#tex   // レンダーターゲット指定
#define SetDefaultRenderTarget()    string RenderColorTarget=""     // レンダーターゲットを元に戻す
#define SetClearColor(col)          int ClearColor=col          // クリアカラー指定

/*************
   簡易設定
*************/

// シーン開始
#define BeginScene(tex) SetRenderTarget(tex)

// シーン開始（クリアカラー有り）
#define BeginSceneWithClearColor(tex, col)\
    SetRenderTarget(tex);\
    SetClearColor(col)

// シーン開始（レンダーターゲットを元に戻す）
#define BeginSceneByDefault()   SetDefaultRenderTarget()

// シーン開始（レンダーターゲットを元に戻す、クリアカラー有り）
#define BeginSceneByDefaultWithClearColor(col)\
    SetDefaultRenderTarget();\
    SetClearColor(col)

#endif  // #if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_SCRIPT_FXH__)
