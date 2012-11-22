#if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_DEFS_FXH__

/********************************************
   シェーダ記述用定義いろいろ
********************************************/

// シェーダIDをセット
#define PES_ID(str)	string __pes_ID__ = str

// パラメータ
#include "ShaderDefs/ShaderDefsParam.fxh"

// パス
#include "ShaderDefs/ShaderDefsPass.fxh"

// サンプラ
#include "ShaderDefs/ShaderDefsSampler.fxh"

// テクスチャ
#include "ShaderDefs/ShaderDefsTex.fxh"

// スクリプト
#include "ShaderDefs/ShaderDefsScript.fxh"

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_FXH__)
