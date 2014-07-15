set MediaDir=..\..\Media
set OutDir=.\out

if not exist %OutDir% (
	mkdir %OutDir%
)

ModelConverter.exe -i ".\data\Seymour.dae" -mtx 48 -e mdl -o "%OutDir%\Seymour.mdl"
ModelConverter.exe -i ".\data\Seymour.dae" -mtx 48 -e msh -o "%OutDir%\Seymour.msh"
ModelConverter.exe -i ".\data\Seymour.dae" -mtx 48 -e skl -o "%OutDir%\Seymour.skl"

MaterialConverter.exe -i ".\data\Seymour.dae" -o "%OutDir%\Seymour.mtrl"

AnimationConverter.exe -i ".\data\Seymour.dae" -o "%OutDir%\Seymour.anm"

ArchiveConverter.exe -i ".\data\Archive.xml" -o "%OutDir%\Archive.arc"  -c ".\data\Config.xml" -d "%MediaDir%" -td "." -h

ShaderConverter.exe -src ".\data\PhongShader.xml" -obj ".\shader_obj" -o "%OutDir%"

PostEffectConverter.exe -src ".\data\PostEffectHDR.fx" -obj ".\posteffect_obj" -asm -o "%OutDir%\PostEffectHDR.pes" -I "..\PostEffect/PostEffectShader/include" -co /nologo -c "%DXSDK_DIR%Utilities\Bin\x86\fxc"

FontConverter.exe -i ".\data\FontSample_Utf8.list" -o "%OutDir%\FontSample_Utf8.fnt" -aa 4 -c utf8 -ft ".\data\mikachan.ttf"

ImageBuilder.exe -i ".\EnvMap.xml" -o "%OutDir%\EnvMap.img"

EnvMapConverter.exe -i ".\data\uffizi_probe.png" -o "%OutDir%\MirrorMap.png" -ie c -oe m
EnvMapConverter.exe -i ".\data\uffizi_probe.png" -o "%OutDir%\LatiLong.png" -ie c -oe l
EnvMapConverter.exe -i ".\data\uffizi_probe.png" -o "%OutDir%\Angular.png" -ie c -oe a
EnvMapConverter.exe -i ".\data\uffizi_probe.png" -o "%OutDir%\Cross.png" -ie c -oe cr

NormalMapMaker.exe -i ".\data\HeightMap.png" -o "%OutDir%\NormalMap.png"

ShaderCompiler.exe -e mainVS -p vs_2_0 -i ".\data\PointLightShader.fx" -o "%OutDir%\PointLightShader.vs"
ShaderCompiler.exe -e mainPS -p ps_2_0 -i ".\data\PointLightShader.fx" -o "%OutDir%\PointLightShader.ps"
