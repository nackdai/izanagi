set MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
set TARGET=Build

%MSBUILD% project\vs2010\izanagi.sln /t:%TARGET% /p:Configuration=Release_DX9
