// ShaderConverter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>

#include "Option.h"
#include "Preproc.h"
#include "ShaderConfig.h"
#include "ShaderConverterImpl.h"

#include "izToolKit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

//#define _VGOTO(b, label)  if (!(b)) { IZ_ASSERT(IZ_FALSE); nRetCode = 1; goto label; }
#define _VGOTO(b, label)    if (!(b)) { nRetCode = 1; goto label; }

namespace {
    izChar s_BUF[1024];

    inline BOOL _PreparePreproc(
        const COption& cOption,
        SShaderConfig& sConfig)
    {
        FILL_ZERO(s_BUF, sizeof(s_BUF));

        // 出力ファイルが空
        memcpy(s_BUF, sConfig.shader, min(sizeof(s_BUF), strlen(sConfig.shader)));

        // ファイル名取得
        izanagi::tool::CString file_name(
            izanagi::tool::CFileUtility::GetFileNameFromPath(s_BUF));

        // 拡張子削除
        VRETURN(
            izanagi::tool::CFileUtility::RemoveExtension(
                s_BUF,
                sizeof(s_BUF),
                file_name));

        if (cOption.obj_dir.empty()) {
            sConfig.preproc_file.format("%s.fx_", s_BUF);
        }
        else {
            izanagi::tool::CString tmp(s_BUF);

            // 中間ディレクトリに出力する
            VRETURN(
                izanagi::tool::CFileUtility::CombinePath(
                    s_BUF, 
                    sizeof(s_BUF),
                    cOption.obj_dir,
                    tmp));

            sConfig.preproc_file.format("%s.fx_", s_BUF);
        }

        return IZ_TRUE;
    }

    inline BOOL _RunPreproc(const SShaderConfig& sConfig)
    {
        COption cOptTmp;
        {
            cOptTmp.in_file = sConfig.shader;
            cOptTmp.preproc_file = sConfig.preproc_file;
            cOptTmp.includes.assign(sConfig.includes.begin(), sConfig.includes.end());
            cOptTmp.defines.assign(sConfig.defines.begin(), sConfig.defines.end());
            cOptTmp.isPreprocShader = TRUE;
        }

        VRETURN(cOptTmp.PreprocInputFile());
        return TRUE;
    }

    inline BOOL _PrepareExport(
        IZ_PCSTR lpszExportDir,
        SShaderConfig& sConfig)
    {
        FILL_ZERO(s_BUF, sizeof(s_BUF));

        // 出力ファイルが空
        memcpy(s_BUF, sConfig.preproc_file, min(sizeof(s_BUF), strlen(sConfig.preproc_file)));

        // ファイル名取得
        izanagi::tool::CString file_name(
            izanagi::tool::CFileUtility::GetFileNameFromPath(s_BUF));

        // 拡張子削除
        VRETURN(
            izanagi::tool::CFileUtility::RemoveExtension(
                s_BUF,
                sizeof(s_BUF),
                file_name));

        if (lpszExportDir != IZ_NULL) {
            if (!izanagi::tool::CFileUtility::IsExist(lpszExportDir)) {
                izanagi::tool::CFileUtility::CreateDir(lpszExportDir);
            }

            sConfig.out.format("%s/%s.shd", lpszExportDir, s_BUF);
        }
        else {
            sConfig.out.format("%s.shd", s_BUF);
        }

        return IZ_TRUE;
    }

    void _DisplayUsage()
    {
        printf(
            "ShaderConverter Usage\n"
            "\n"
            "[Usage]\n"
            "ShaderConverter -src [in] [options]\n"
            "\n"
            "   -src [in]  : Shader list file path(*.xml)\n"
            "   -o [export dir] : Directory for exporting files\n"
            "   -obj [dir] : Temporary directory for obj files\n"
            "   -t [type] : Compile type\n"
            "     dx9   : DirectX9\n"
            "     gles2 : OpenGLES2\n"
            " Usually belows are not used\n"
            "   -I [inclue paths] : Include paths for preprocessing shader list file\n"
            "   -D [deines] : Defines for preprocessing shader list file\n");
    }
}   // namespace

int main(int argc, char* argv[])
{
    int nRetCode = 0;

    COption cOption;
    try {
        // オプション解析
        _VGOTO(cOption.Analysis(argc, argv), __EXIT__);

        // オプションが不正かどうか
        _VGOTO(cOption.IsValid(), __EXIT__);

        // 相対パスを絶対パスにするためのベースとなるディレクトリを設定
        CShaderConfigManager::GetInstance().SetBaseDir(cOption.in_file);

        // 中間ファイル出力ディレクトリの指定がある
        if (!cOption.obj_dir.empty()) {
            if (!izanagi::tool::CFileUtility::IsExist(cOption.obj_dir)) {
                // 存在しないので作成する
                izanagi::tool::CFileUtility::CreateDir(cOption.obj_dir);
            }
        }

        if (cOption.IsPreproc()) {
            // プリプロセス処理のみを行う
            nRetCode = Preproc(cOption);
            goto __EXIT__;
        }

        // 自分自身をプリプロセス処理モードで呼び出す
        _VGOTO(cOption.PreprocInputFile(), __EXIT__);

        // 設定XMLファイルを解析
        xercesc::XMLPlatformUtils::Initialize();
        xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();
        parser->setContentHandler(&CShaderConfigManager::GetInstance());
        parser->parse(cOption.in_file);

        UINT nConfigNum = CShaderConfigManager::GetInstance().GetConfigNum();

        for (UINT i = 0; i < nConfigNum; i++) {
            SShaderConfig& sConfig = CShaderConfigManager::GetInstance().GetConfig(i);

            // コンパイルタイプの反映
            sConfig.type = cOption.type;

            // Prepare preproc.
            VRETURN(_PreparePreproc(cOption, sConfig));

            // Run preproc.
            VRETURN(_RunPreproc(sConfig));

            // Prepare export.
            VRETURN(
                _PrepareExport(
                    cOption.export_dir,
                    sConfig));

            _VGOTO(
                CShaderConverter::GetInstance().Begin(sConfig.preproc_file),
                __EXIT__);

            _VGOTO(
                CShaderConverter::GetInstance().CompileShader(
                    sConfig,
                    cOption.obj_dir),
                __EXIT__);

            _VGOTO(
                CShaderConverter::GetInstance().Export(sConfig),
                __EXIT__);
        }
    }
    catch (...) {
        printf("予期しないエラーが発生しました\n");
        nRetCode = 1;
    }

__EXIT__:
    return nRetCode;
}
