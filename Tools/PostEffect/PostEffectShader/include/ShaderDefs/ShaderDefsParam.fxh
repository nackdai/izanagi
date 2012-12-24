#if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_PARAM_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_DEFS_PARAM_FXH__

////////////////////////////////////////////////////////
// パラメータ

//****************************************
// アノテーション

// UIコントロール設定用
#define SetUIWidget(widget)         string UIWidget = widget
#define SetUIName(name)             string UIName = name
#define SetUIHelp(help)             string UIHelp = help

#define SetUIMax1(v0)               float1 UIMax = v0
#define SetUIMax2(v0, v1)           float2 UIMax = {v0, v1}
#define SetUIMax3(v0, v1, v2)       float3 UIMax = {v0, v1, v2}
#define SetUIMax4(v0, v1, v2, v3)   float4 UIMax = {v0, v1, v2, v3}

#define SetUIMin1(v0)               float1 UIMin = v0
#define SetUIMin2(v0, v1)           float2 UIMin = {v0, v1}
#define SetUIMin3(v0, v1, v2)       float3 UIMin = {v0, v1, v2}
#define SetUIMin4(v0, v1, v2, v3)   float4 UIMin = {v0, v1, v2, v3}

#define SetUIStep1(v0)              float1 UIStep = v0
#define SetUIStep2(v0, v1)          float2 UIStep = {v0, v1}
#define SetUIStep3(v0, v1, v2)      float3 UIStep = {v0, v1, v2}
#define SetUIStep4(v0, v1, v2, v3)  float4 UIStep = {v0, v1, v2, v3}

#if 0
// UIコントロール(UIWidget)設定値
#define UI_WIDGET_SLIDER        "Slider"        // スライダー
#define UI_WIDGET_SPINNER       "Spinner"       // スピナー
#define UI_WIDGET_COLOR         "Color"         // カラーピッカー
#define UI_WIDGET_GRAYSCALE     "GrayScale"     // 全ての成分を同じ値にする必要がある
#define UI_WIDGET_NUMERIC       "Numeric"       // 数値指定
#define UI_WIDGET_STRING        "String"        // 文字列指定
#define UI_WIDGET_TEXTURE       "Texture"       // テクスチャ指定
#define UI_WIDGET_DIRECTION     "Direction"     // ライトの方向とか（ポストエフェクトでは不要かな・・・）
#define UI_WIDGET_OBJECT        "Object"        // アノテーションに依存
#define UI_WIDGET_NONE          "None"          // このアノテーションを表示しない
#endif

// エフェクト内では使用しない場合でも、削除しない
#define SetDoNotStrip(b)    bool DoNotStrip = b

#endif  // #if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_PARAM_FXH__)
