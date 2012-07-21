set MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
set TARGET=Rebuild

%MSBUILD% projects\vs2010\Tools.sln /t:%TARGET% /p:Configuration=Release
